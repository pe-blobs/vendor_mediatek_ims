/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "RtcSmsMessage.h"
#include "RtcSmsHeader.h"
#include "RtcSmsAddress.h"
#include "RfxLog.h"

/*****************************************************************************
 * Define
 *****************************************************************************/
#define INVALID_HEX_CHAR 16
#define RFX_LOG_TAG "RtcSmsMessage"

/*****************************************************************************
 * static functions
 *****************************************************************************/
static unsigned char hexCharToInt(unsigned char c) {
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);

    return INVALID_HEX_CHAR;
}

/*****************************************************************************
 * External functions
 *****************************************************************************/
unsigned char* convertHexStringToBytes(unsigned char* hexString, size_t dataLen) {
    if (dataLen % 2 != 0) {
        return NULL;
    }

    unsigned char* bytes = (unsigned char*)calloc(dataLen / 2, sizeof(unsigned char));
    if (bytes == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < dataLen; i += 2) {
        unsigned char hexChar1 = hexCharToInt(hexString[i]);
        unsigned char hexChar2 = hexCharToInt(hexString[i + 1]);

        if (hexChar1 == INVALID_HEX_CHAR || hexChar2 == INVALID_HEX_CHAR) {
            free(bytes);
            return NULL;
        }
        bytes[i / 2] = ((hexChar1 << 4) | hexChar2);
    }

    return bytes;
}

/*****************************************************************************
 * RtcSmsMessage
 *****************************************************************************/
RtcSmsMessage::RtcSmsMessage()
    : mSmsAddress(NULL),
      mUserDataHeader(NULL),
      mEncodingType(ENCODING_UNKNOWN),
      mNumField(0),
      mError(false) {}

RtcSmsMessage::~RtcSmsMessage() {
    if (mUserDataHeader != NULL) {
        delete mUserDataHeader;
        mUserDataHeader = NULL;
    }
    if (mSmsAddress != NULL) {
        delete mSmsAddress;
        mSmsAddress = NULL;
    }
}

void RtcSmsMessage::createSmsUserDataHeader(unsigned char* header, int len, bool is3Gpp2) {
    mUserDataHeader = new RtcSmsUserDataHeader(header, len, is3Gpp2);
}

/*****************************************************************************
 * RtcGsmSmsMessage
 *****************************************************************************/
RtcGsmSmsMessage::RtcGsmSmsMessage(unsigned char* hexString, size_t dataLen)
    : mPdu(NULL),
      mHexPdu((const char*)hexString),
      mCur(0),
      mLenExceptUserData(0),
      mDataLen(dataLen / 2) {
    if ((mPdu = convertHexStringToBytes(hexString, dataLen)) != NULL) {
        parse();
    }
}

RtcGsmSmsMessage::RtcGsmSmsMessage(const RtcGsmSmsMessage& message, int encodingType, int numFields,
                                   const Vector<unsigned char>& userDataPayload)
    : mPdu(NULL), mCur(0), mLenExceptUserData(0), mDataLen(0) {
    if (encodingType != message.getEncodingType()) {
        setError();
        return;
    }
    if (encodingType == ENCODING_16BIT || encodingType == ENCODING_7BIT) {
        mHexPdu.append(message.getHexPdu().string(), (message.getLengthExceptUserData() - 1) * 2);
        int udl = numFields;
        if (encodingType == ENCODING_16BIT) {
            udl = 2 * numFields;
        }
        mHexPdu.appendFormat("%02X", udl);
        Vector<unsigned char>::const_iterator it;
        for (it = userDataPayload.begin(); it != userDataPayload.end(); it++) {
            mHexPdu.appendFormat("%02X", *it);
        }
    }
}

RtcGsmSmsMessage::~RtcGsmSmsMessage() {
    if (mPdu != NULL) {
        free(mPdu);
        mPdu = NULL;
    }
}

void RtcGsmSmsMessage::parse() {
    // SC address
    int scLen = getByte();
    mCur += scLen;

    // TP-Message-Type-Indicator
    int firstByte = getByte();
    int mti = firstByte & 0x3;
    switch (mti) {
        // TP-Message-Type-Indicator
        // 9.2.3
        case 0:
        case 3:
            parseSmsDeliver(firstByte);
            break;
        case 1:
        case 2:
        default:
            break;
    }
}

void RtcGsmSmsMessage::parseSmsDeliver(int firstByte) {
    bool hasUserDataHeader = (firstByte & 0x40) == 0x40;

    // Address
    int addressLength = mPdu[mCur] & 0xff;
    int lengthBytes = 2 + (addressLength + 1) / 2;
    setSmsAddress(new RtcGsmSmsAddress(mPdu, mCur, lengthBytes));
    mCur += lengthBytes;

    // TP-Protocol-Identifier (TP-PID)
    getByte();

    // TP-Data-Coding-Scheme
    int dataCodingScheme = getByte();
    setEncodingType(parseDataCodingScheme(dataCodingScheme));

    // SCTimestamp
    mCur += 7;
    mLenExceptUserData = mCur + 1;
    int numField = getByte();
    if (getEncodingType() == ENCODING_16BIT) {
        numField /= 2;
    }
    setNumberField(numField);
    if (mDataLen - mCur <= 0) {
        RFX_LOG_D(RFX_LOG_TAG, "parseSmsDeliver len: %d, %d, %d", mDataLen, mCur, mDataLen - mCur);
        setError();
        return;
    }
    getUDP().appendArray(&mPdu[mCur], mDataLen - mCur);

    if (hasUserDataHeader) {
        int userDataHeaderLength = getByte();
        createSmsUserDataHeader(&mPdu[mCur], userDataHeaderLength, false);
    }
}

int RtcGsmSmsMessage::parseDataCodingScheme(int dataCodingScheme) {
    bool hasMessageClass = false;
    bool userDataCompressed = false;
    int encodingType = ENCODING_UNKNOWN;
    // Look up the data encoding scheme
    if ((dataCodingScheme & 0x80) == 0) {
        userDataCompressed = (0 != (dataCodingScheme & 0x20));
        hasMessageClass = (0 != (dataCodingScheme & 0x10));

        if (userDataCompressed) {
            // Unsupported
        } else {
            switch ((dataCodingScheme >> 2) & 0x3) {
                case 0:  // GSM 7 bit default alphabet
                    encodingType = ENCODING_7BIT;
                    break;
                case 2:  // UCS 2 (16bit)
                    encodingType = ENCODING_16BIT;
                    break;
                default:
                    // Unsupported
                    break;
            }
        }
    } else {
        // Unsupported
    }
    return encodingType;
}

int RtcGsmSmsMessage::getByte() { return mPdu[mCur++] & 0xff; }

/*****************************************************************************
 * RtcCdmaSmsMessage
 *****************************************************************************/
RtcCdmaSmsMessage::RtcCdmaSmsMessage(RIL_CDMA_SMS_Message* pMessage) {
    if ((pMessage->bIsServicePresent != 1) && (pMessage->uTeleserviceID == TELESERVICE_WMT ||
                                               pMessage->uTeleserviceID == TELESERVICE_WEMT)) {
        InboundSmsInfo info;
        RmcCdmaSmsConverter::getInboundSmsInfo(pMessage, &info);
        setUDP(info.userDataPayload);
        int encodingType;
        switch (info.msgEncoding) {
            case UserData::ENCODING_7BIT_ASCII:
            case UserData::ENCODING_GSM_7BIT_ALPHABET:
                encodingType = ENCODING_7BIT;
                break;
            case UserData::ENCODING_UNICODE_16:
                encodingType = ENCODING_16BIT;
                break;
            default:
                encodingType = ENCODING_UNKNOWN;
                break;
        }
        setEncodingType(encodingType);
        setNumberField(info.numFields);
        if (info.hasUserDataHeader) {
            int userDataHeaderLength = info.userDataPayload[0];
            createSmsUserDataHeader((unsigned char*)info.userDataPayload.array() + 1,
                                    userDataHeaderLength, true);
        }
        setSmsAddress(new RtcCdmaSmsAddress(pMessage->sAddress));
    }
    mMsg = *pMessage;
}

RtcCdmaSmsMessage::RtcCdmaSmsMessage(const RtcCdmaSmsMessage& message, int encodingType,
                                     int numFields, const Vector<unsigned char>& userDataPayload) {
    if (encodingType != message.getEncodingType()) {
        setError();
        memset(&mMsg, 0, sizeof(mMsg));
        return;
    }
    int msgEncoding = UserData::ENCODING_OCTET;
    switch (encodingType) {
        case ENCODING_7BIT:
            msgEncoding = UserData::ENCODING_GSM_7BIT_ALPHABET;
            break;

        case ENCODING_16BIT:
            msgEncoding = UserData::ENCODING_UNICODE_16;
            break;
        default:
            setError();
            memset(&mMsg, 0, sizeof(mMsg));
            return;
    }
    mMsg = message.getMessage();
    RmcCdmaSmsConverter::replaceUserDataPayload(&mMsg, userDataPayload, msgEncoding, 0, numFields);
}

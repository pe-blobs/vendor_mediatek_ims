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

#include "RtcEmbmsUtils.h"
#include "RfxMessageId.h"
#include <telephony/mtk_ril.h>
#include <cutils/jstring.h>
#include "rfx_properties.h"
#include <mtk_properties.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "RfxRootController.h"
#define RFX_LOG_TAG "RtcEmbmsUtil"

extern int RFX_SLOT_COUNT;

/*****************************************************************************
 * Class RtcEmbmsUtils
 *****************************************************************************/

RtcEmbmsUtils::RtcEmbmsUtils() {}

RtcEmbmsUtils::~RtcEmbmsUtils() {}

RtcEmbmsSessionInfo* RtcEmbmsUtils::findSessionByTransId(Vector<RtcEmbmsSessionInfo*>* list,
                                                         int trans_id, int* pIndex) {
    RtcEmbmsSessionInfo* pSessionInfo = NULL;
    *pIndex = -1;

    if (list != NULL) {
        int size = list->size();
        for (int i = 0; i < size; i++) {
            pSessionInfo = list->itemAt(i);
            if (pSessionInfo->mTransId == trans_id) {
                RFX_LOG_D(RFX_LOG_TAG, "Find trans_id:%d", pSessionInfo->mTransId);
                *pIndex = i;
                break;
            }
        }
    }
    if (*pIndex == -1) {
        pSessionInfo = NULL;
    }
    return pSessionInfo;
}

RtcEmbmsSessionInfo* RtcEmbmsUtils::findSessionByTmgi(Vector<RtcEmbmsSessionInfo*>* list,
                                                      int tmgi_len, char* pTmgi, int* pIndex) {
    RtcEmbmsSessionInfo* pSessionInfo = NULL;
    *pIndex = -1;

    if (list != NULL) {
        int size = list->size();
        for (int i = 0; i < size; i++) {
            pSessionInfo = list->itemAt(i);
            RFX_LOG_D(RFX_LOG_TAG, "tmgi[%d]:[%s],len[%d]", i, pSessionInfo->mTmgi,
                      pSessionInfo->mTmgiLen);
            if (pSessionInfo->mTmgiLen == tmgi_len) {
                if (strcmp(pSessionInfo->mTmgi, pTmgi) == 0) {
                    *pIndex = i;
                    RFX_LOG_D(RFX_LOG_TAG, "find tmgi[%d]:%s", i, pSessionInfo->mTmgi);
                    break;
                }
            }
        }
    }
    if (*pIndex == -1) {
        pSessionInfo = NULL;
    }
    return pSessionInfo;
}

void RtcEmbmsUtils::freeSessionList(Vector<RtcEmbmsSessionInfo*>* list) {
    if (list != NULL) {
        int size = list->size();
        for (int i = 0; i < size; i++) {
            delete list->itemAt(i);
        }
        list->clear();
    }
}

int RtcEmbmsUtils::getDefaultDataSlotId() {
    RfxRootController* root = RFX_OBJ_GET_INSTANCE(RfxRootController);
    // Default get slot 0 StatusManager.
    int data = root->getNonSlotScopeStatusManager()->getIntValue(RFX_STATUS_KEY_DEFAULT_DATA_SIM);
    // For log too much issue, only print log in your local code.
    // RFX_LOG_D(RFX_LOG_TAG, "getDefaultDataSlotId: %d", data);
    return data;
}

bool RtcEmbmsUtils::revertTmgi(const uint8_t* input, char* output, int length) {
    int i = 0;
    char tmp_char;
    RFX_LOG_D(RFX_LOG_TAG, "start revertTmgi");

    if (input == NULL || length != EMBMS_MAX_BYTES_TMGI) {
        printf("revertTmgi error: input null or wrong len\n");
        return false;
    }
    int result = sprintf(output, "%02X%02X%02X%02X%02X%02X", input[0], input[1], input[2], input[3],
                         input[4], input[5]);
    if (result < 0) {
        RFX_LOG_E(RFX_LOG_TAG, "sprintf fail for output");
    }
    RFX_LOG_D(RFX_LOG_TAG, "revertTmgi from %s", output);

    // digit[0~5] are service id
    // digit[6~11] are: mcc2, mcc1, mnc3(might be F), mcc3, mnc2, mnc1
    // swap digit 6&7
    tmp_char = output[6];
    output[6] = output[7];
    output[7] = tmp_char;
    // swap digit 8&9
    tmp_char = output[8];
    output[8] = output[9];
    output[9] = tmp_char;
    // swap digit 9&11
    tmp_char = output[9];
    output[9] = output[11];
    output[11] = tmp_char;
    // check if digit 11 is F
    if (output[11] == 'F' || output[11] == 'f') {
        output[11] = '\0';
    }

    RFX_LOG_D(RFX_LOG_TAG, "revertTmgi to [%s]", output);
    return true;
}

bool RtcEmbmsUtils::convertTmgi(const char* input, uint8_t* output) {
    int inputLen = 0, i = 0;
    char tmpTmgiStr[EMBMS_MAX_LEN_TMGI + 1];
    uint8_t tmpValue = 0, tmpValue2;
    RFX_LOG_D(RFX_LOG_TAG, "start convertTmgi");
    memset(tmpTmgiStr, 0, sizeof(tmpTmgiStr));

    if (input == NULL) {
        RFX_LOG_E(RFX_LOG_TAG, "convertTmgi error: input null");
        return false;
    }

    inputLen = strlen(input);

    if (inputLen != EMBMS_MAX_LEN_TMGI && inputLen != (EMBMS_MAX_LEN_TMGI - 1)) {
        RFX_LOG_E(RFX_LOG_TAG, "convertTmgi error: wrong len");
        return false;
    } else if (inputLen == EMBMS_MAX_LEN_TMGI - 1) {
        tmpTmgiStr[EMBMS_MAX_LEN_TMGI - 1] = 'F';
    }
    strncpy(tmpTmgiStr, input, inputLen);

    // Swap MCC1 & MCC2
    tmpValue = tmpTmgiStr[6];
    tmpTmgiStr[6] = tmpTmgiStr[7];
    tmpTmgiStr[7] = tmpValue;

    // Swap MCC3 & MNC3
    tmpValue = tmpTmgiStr[8];
    tmpValue2 = tmpTmgiStr[9];
    tmpTmgiStr[8] = tmpTmgiStr[11];
    tmpTmgiStr[9] = tmpValue;
    tmpTmgiStr[11] = tmpValue2;

    for (i = 0; i < EMBMS_MAX_BYTES_TMGI; i++) {
        int result = sscanf(&tmpTmgiStr[2 * i], "%2hhx", output + i);
        if (result < 0) {
            RFX_LOG_E(RFX_LOG_TAG, "sscanf fail");
        }
    }
    RFX_LOG_D(RFX_LOG_TAG, "convertTmgi from [%s] to [%02X%02X%02X%02X%02X%02X]", input, output[0],
              output[1], output[2], output[3], output[4], output[5]);
    return true;
}

bool RtcEmbmsUtils::isEmbmsSupport() {
    bool ret = false;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.radio.embms.support", prop, "-1");
    if (!strcmp(prop, "1")) {
        return true;
    } else if (!strcmp(prop, "0")) {
        return false;
    }

    rfx_property_get("ro.vendor.mtk_embms_support", prop, "0");
    if (!strcmp(prop, "1")) {
        ret = true;
    }

    return ret;
}

bool RtcEmbmsUtils::isAtCmdEnableSupport() {
    bool ret = false;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    // for RJIL old middleware version
    rfx_property_get("persist.vendor.radio.embms.atenable", prop, "1");
    if (RtcEmbmsUtils::isRjilSupport()) {
        return false;
    }

    if (!strcmp(prop, "1")) {
        return true;
    } else if (!strcmp(prop, "0")) {
        return false;
    }

    return ret;
}

// for RJIL old middleware version with spec v1.8
bool RtcEmbmsUtils::isRjilSupport() {
    // On android N all use latest Middleware, for RJIL also support requirement >= 1.9
    return false;
}

bool RtcEmbmsUtils::isDualLteSupport() {
    // On android N all use latest Middleware, for RJIL also support requirement >= 1.9
    bool ret = false;
    char prop[RFX_PROPERTY_VALUE_MAX] = {0};

    rfx_property_get("persist.vendor.radio.mtk_ps2_rat", prop, "G");
    RFX_LOG_D(RFX_LOG_TAG, "isDualLteSupport:%s", prop);
    if (strchr(prop, 'L') != NULL) {
        ret = true;
    }
    return ret;
}

char* RtcEmbmsUtils::addLogMask(int input) {
    char* out = NULL;
    int len = asprintf(&out, "%X", input);
    if (len == 1) {
        out[0] = '*';
        return out;
    }
    for (int i = 0; i < (len / 2); ++i) {
        out[i] = '*';
    }
    return out;
}

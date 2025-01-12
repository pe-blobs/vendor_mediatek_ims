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

#include "RfxMclMessage.h"
#include "RfxIdToStringUtils.h"
#include <libmtkrilutils.h>

RfxMclMessage::RfxMclMessage()
    : m_type(REQUEST),
      m_id(-1),
      m_data(NULL),
      m_channel_id(-1),
      m_slot_id(-1),
      m_client_id(-1),
      m_token(-1),
      m_raw_urc(NULL),
      m_raw_urc2(NULL),
      m_err(RIL_E_SUCCESS),
      m_delayTime(0),
      m_priority(MTK_RIL_REQUEST_PRIORITY::MTK_RIL_REQUEST_PRIORITY_MEDIUM),
      rilToken(NULL),
      m_key(RfxStatusKeyEnum(-1)),
      m_value(RfxVariant()),
      m_force_notify(false),
      m_is_default(false),
      m_update_for_mock(false),
      mSendToMainProtocol(false),
      m_main_protocol_slot_id(0),
      mTimeStamp(0),
      mAddAtFront(false) {}

RfxMclMessage::~RfxMclMessage() {
    if (m_data) delete (m_data);
    if (m_raw_urc) delete (m_raw_urc);
    if (m_raw_urc2) delete (m_raw_urc2);
}

String8 RfxMclMessage::toString() const {
    char raw_urc1_str[MAX_HIDEN_LOG_LEN] = {0};
    char raw_urc2_str[MAX_HIDEN_LOG_LEN] = {0};
    int index = 0;

    if (m_raw_urc != NULL) {
        char* raw_urc1 = m_raw_urc->getLine();
        if ((index = needToHidenLog(raw_urc1)) >= 0) {
            strncpy(raw_urc1_str, (String8::format("%s:***", getHidenLogPreFix(index))).string(),
                    (MAX_HIDEN_LOG_LEN - 1));
        }
    }
    if (m_raw_urc2 != NULL) {
        char* raw_urc2 = m_raw_urc2->getLine();
        index = 0;
        if ((index = needToHidenLog(raw_urc2)) >= 0) {
            strncpy(raw_urc2_str, (String8::format("%s:***", getHidenLogPreFix(index))).string(),
                    (MAX_HIDEN_LOG_LEN - 1));
        }
    }
    return String8::format(
            "type = %d, id = %s(%d), channel id = %d, slot id = %d, \
client_id = %d, token = %d, raw urc = %s, raw urc2 = %s, err = %d, \
mSendToMainProtocol = %d, m_priority = %d",
            m_type, RFX_ID_TO_STR(m_id), m_id, m_channel_id, m_slot_id, m_client_id, m_token,
            (strlen(raw_urc1_str) == 0 ? (m_raw_urc == NULL ? "" : m_raw_urc->getLine())
                                       : raw_urc1_str),
            (strlen(raw_urc2_str) == 0 ? (m_raw_urc2 == NULL ? "" : m_raw_urc2->getLine())
                                       : raw_urc2_str),
            m_err, mSendToMainProtocol, m_priority);
}

sp<RfxMclMessage> RfxMclMessage::obtainRequest(int id, RfxBaseData* data, int slot_id, int token,
                                               bool sendToMainProtocol, RIL_Token rilToken,
                                               nsecs_t timeStamp, bool addAtFront) {
    sp<RfxMclMessage> msg = new RfxMclMessage();
    msg->m_type = REQUEST;
    msg->m_id = id;
    msg->m_data = RfxDataCloneManager::copyData(id, data, REQUEST);
    msg->m_slot_id = slot_id;
    msg->m_client_id = -1;
    msg->m_token = token;
    msg->mSendToMainProtocol = sendToMainProtocol;
    msg->rilToken = rilToken;
    msg->mTimeStamp = timeStamp;
    msg->mAddAtFront = addAtFront;
    return msg;
}

sp<RfxMclMessage> RfxMclMessage::obtainRequest(int id, RfxBaseData* data, int slot_id, int token,
                                               bool sendToMainProtocol, RIL_Token rilToken,
                                               nsecs_t delayTime, nsecs_t timeStamp,
                                               bool addAtFront) {
    sp<RfxMclMessage> msg = obtainRequest(id, data, slot_id, token, sendToMainProtocol, rilToken,
                                          timeStamp, addAtFront);
    msg->m_delayTime = delayTime;
    return msg;
}

sp<RfxMclMessage> RfxMclMessage::obtainResponse(int id, RIL_Errno err, const RfxBaseData& data,
                                                sp<RfxMclMessage> msg, bool copyData) {
    sp<RfxMclMessage> newMsg = new RfxMclMessage();
    newMsg->m_type = RESPONSE;
    newMsg->m_id = id;
    newMsg->m_err = err;
    newMsg->m_channel_id = msg->m_channel_id;
    newMsg->m_slot_id = msg->m_slot_id;
    msg->m_client_id = -1;
    newMsg->m_token = msg->m_token;
    if (copyData) {
        newMsg->m_data = RfxDataCloneManager::copyData(id, msg->getData(), RESPONSE);
    } else {
        newMsg->m_data = RfxDataCloneManager::copyData(id, &data, RESPONSE);
    }

    return newMsg;
}

sp<RfxMclMessage> RfxMclMessage::obtainResponse(RIL_Errno err, const RfxBaseData& data,
                                                sp<RfxMclMessage> msg) {
    return RfxMclMessage::obtainResponse(msg->getId(), err, data, msg, false);
}

sp<RfxMclMessage> RfxMclMessage::obtainRawUrc(int channel_id, RfxAtLine* line1, RfxAtLine* line2) {
    sp<RfxMclMessage> msg = new RfxMclMessage();
    msg->m_type = RAW_URC;
    // msg->m_channel_id = channel_id % RIL_CHANNEL_OFFSET;
    msg->m_channel_id = channel_id;
    msg->m_slot_id = channel_id / RIL_CHANNEL_OFFSET;
    msg->m_client_id = -1;
    msg->m_raw_urc = line1;
    msg->m_raw_urc2 = line2;
    return msg;
}

sp<RfxMclMessage> RfxMclMessage::obtainUrc(int id, int slot_id, const RfxBaseData& data) {
    sp<RfxMclMessage> msg = new RfxMclMessage();
    msg->m_type = URC;
    msg->m_id = id;
    msg->m_slot_id = slot_id;
    msg->m_client_id = -1;
    msg->m_data = RfxDataCloneManager::copyData(id, &data, URC);
    return msg;
}

sp<RfxMclMessage> RfxMclMessage::obtainEvent(int id, const RfxBaseData& data, int channel_Id,
                                             int slot_id, int client_id, int token,
                                             nsecs_t delayTime, MTK_RIL_REQUEST_PRIORITY priority) {
    sp<RfxMclMessage> msg = new RfxMclMessage();
    msg->m_type = EVENT;
    msg->m_id = id;
    msg->m_data = RfxDataCloneManager::copyData(id, &data, EVENT);
    msg->m_channel_id = channel_Id % RIL_CHANNEL_OFFSET;
    msg->m_slot_id = slot_id;
    msg->m_client_id = client_id;
    msg->m_token = token;
    msg->m_delayTime = delayTime;
    msg->m_priority = priority;
    return msg;
}

sp<RfxMclMessage> RfxMclMessage::obtainStatusSync(int slot_id, RfxStatusKeyEnum key,
                                                  const RfxVariant value, bool force_notify,
                                                  bool is_default, bool update_for_mock) {
    sp<RfxMclMessage> msg = new RfxMclMessage();
    msg->m_type = STATUS_SYNC;
    msg->m_slot_id = slot_id;
    msg->m_client_id = -1;
    msg->m_key = key;
    msg->m_value = value;
    msg->m_force_notify = force_notify;
    msg->m_is_default = is_default;
    msg->m_update_for_mock = update_for_mock;
    return msg;
}

sp<RfxMclMessage> RfxMclMessage::obtainSapRequest(int id, RfxBaseData* data, int slot_id,
                                                  int token) {
    sp<RfxMclMessage> msg = new RfxMclMessage();
    msg->m_type = SAP_REQUEST;
    msg->m_id = id;
    msg->m_data = RfxDataCloneManager::copyData(id, data, REQUEST);
    msg->m_slot_id = slot_id;
    msg->m_token = token;
    return msg;
}

sp<RfxMclMessage> RfxMclMessage::obtainSapResponse(int id, RIL_Errno e, const RfxBaseData& data,
                                                   sp<RfxMclMessage> msg, bool copyData) {
    sp<RfxMclMessage> newMsg = new RfxMclMessage();
    newMsg->m_type = SAP_RESPONSE;
    newMsg->m_id = id;
    newMsg->m_err = e;
    newMsg->m_channel_id = msg->m_channel_id;
    newMsg->m_slot_id = msg->m_slot_id;
    newMsg->m_token = msg->m_token;
    if (copyData) {
        newMsg->m_data = RfxDataCloneManager::copyData(msg->getId(), msg->getData(), RESPONSE);
    } else {
        newMsg->m_data = RfxDataCloneManager::copyData(msg->getId(), &data, RESPONSE);
    }

    return newMsg;
}

sp<RfxMclMessage> RfxMclMessage::obtainSapUrc(int id, int slot_id, const RfxBaseData& data) {
    sp<RfxMclMessage> msg = new RfxMclMessage();
    msg->m_type = SAP_URC;
    msg->m_id = id;
    msg->m_slot_id = slot_id;
    msg->m_data = RfxDataCloneManager::copyData(id, &data, URC);
    return msg;
}

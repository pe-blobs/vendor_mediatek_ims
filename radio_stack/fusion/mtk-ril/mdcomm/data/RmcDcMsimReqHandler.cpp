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
#include "RfxVoidData.h"
#include "RfxIntsData.h"
#include "RmcDcMsimReqHandler.h"
#include <mtk_properties.h>

#define RFX_LOG_TAG "RmcDcMsimReqHandler"

/*****************************************************************************
 * Class RmcDcMsimReqHandler
 *****************************************************************************/
RFX_IMPLEMENT_HANDLER_CLASS(RmcDcMsimReqHandler, RIL_CMD_PROXY_5);

RmcDcMsimReqHandler::RmcDcMsimReqHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    // The use of EDALLOW=3 to notify modem current project is single PS attach.
    char multiPs[MTK_PROPERTY_VALUE_MAX] = {0};
    mtk_property_get("ro.vendor.mtk_data_config", multiPs, "0");
    // Check if Single-PS or Multi-PS
    if (atoi(multiPs) != 1) {
        RFX_LOG_D(RFX_LOG_TAG, "Single PS project - multiPs= %s, send EDALLOW=3", multiPs);
        atSendCommand(String8::format("AT+EDALLOW=3"));
    }

    // TODO: Since AOSP doesn't send ALLOW_DATA on single SIM project from O1,
    // Remove it if AOSP send data allow in single PS project again.
    if (RfxRilUtils::rfxGetSimCount() == 1 && slot_id == 0) {
        RFX_LOG_D(RFX_LOG_TAG, "Single SIM project, send EDALLOW=1 at boot time");
        atSendCommand(String8::format("AT+EDALLOW=1"));
    }

    // clear allow flag when initialized
    updateDataAllowStatus(m_slot_id, 0);

    // From Gen97, MD has ability to set EDALLOW=1/0 it self.
    // AP no need to sent EDALLOW=1/0 anymore, so set allow flag
    // as true to make the EAPNACT can always pass the check.
    char MdEdallowTriggered[] = "MD trigger edallow";
    int isMdSelfEdallow = getFeatureVersion(MdEdallowTriggered, 0);
    logD(RFX_LOG_TAG, "isMdSelfEdallow = %d", isMdSelfEdallow);
    if (isMdSelfEdallow == 1) {
        logD(RFX_LOG_TAG, "set [%d] slot allow = 1", m_slot_id);
        updateDataAllowStatus(m_slot_id, 1);
    }

    const int requestList[] = {
            RFX_MSG_REQUEST_ALLOW_DATA,
            RFX_MSG_REQUEST_DATA_CONNECTION_ATTACH,
            RFX_MSG_REQUEST_DATA_CONNECTION_DETACH,
            RFX_MSG_REQUEST_RECOVERY_ALLOW_DATA,
    };

    registerToHandleRequest(requestList, sizeof(requestList) / sizeof(int));
}

RmcDcMsimReqHandler::~RmcDcMsimReqHandler() {}

void RmcDcMsimReqHandler::onHandleRequest(const sp<RfxMclMessage>& msg) {
    int request = msg->getId();
    switch (request) {
        case RFX_MSG_REQUEST_ALLOW_DATA:
        case RFX_MSG_REQUEST_RECOVERY_ALLOW_DATA:
            handleRequestAllowData(msg);
            break;
        case RFX_MSG_REQUEST_DATA_CONNECTION_ATTACH:
            handleDataConnectionAttachRequest(msg);
            break;
        case RFX_MSG_REQUEST_DATA_CONNECTION_DETACH:
            handleDataConnectionDetachRequest(msg);
            break;
        default:
            RFX_LOG_D(RFX_LOG_TAG, "unknown request, ignore!");
            break;
    }
}

void RmcDcMsimReqHandler::handleRequestAllowData(const sp<RfxMclMessage>& msg) {
    const int* pRspData = (const int*)msg->getData()->getData();
    int dataAllowed = pRspData[0];
    setDataAllowed(dataAllowed, msg);
}

// The use of setDataAllowed() is to indicate that which SIM is allowed to use data.
// AP should make sure both two protocols are NOT set to ON at the same time in the
// case of default data switch flow.
// <Usage> AT+EDALLOW= <data_allowed_on_off>
//                   0 -> off
//                   1 -> on
void RmcDcMsimReqHandler::setDataAllowed(int allowed, const sp<RfxMclMessage>& msg) {
    sp<RfxAtResponse> p_response;
    sp<RfxMclMessage> response;
    sp<RfxMclMessage> urc;
    AT_CME_Error atCause = CME_SUCCESS;
    RIL_Errno cause = RIL_E_SUCCESS;

    RFX_LOG_D(RFX_LOG_TAG, "setDataAllowed: allowed= %d", allowed);

    updatePreDataAllowStatus(m_slot_id, allowed);

    p_response = atSendCommand(String8::format("AT+EDALLOW=%d", allowed));
    if (p_response->isAtResponseFail()) {
        // Return the error cause (e.g. 4117) for the upper to do error handling.
        atCause = p_response->atGetCmeError();
        RFX_LOG_E(RFX_LOG_TAG, "setDataAllowed: response error");
        goto error;
    }

    updateDataAllowStatus(m_slot_id, allowed);
    response = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
    responseToTelCore(response);

    return;
error:
    // 4117: multi PS allowed error
    if (atCause == CME_MULTI_ALLOW_ERR) {
        cause = RIL_E_OEM_MULTI_ALLOW_ERR;
    }
    RFX_LOG_E(RFX_LOG_TAG, "setDataAllowed: modem response ERROR cause= %d", cause);
    response =
            RfxMclMessage::obtainResponse(msg->getId(), (RIL_Errno)cause, RfxVoidData(), msg, true);
    responseToTelCore(response);
    return;
}

// Support requirements to do ps attach or ps&cs attach base on the parameter.
// This function is used only for Java Framework to trigger data-stall mechanism.
// Detach request and attach request will be always executed successively, Attach
// request is always after detach request.
void RmcDcMsimReqHandler::handleDataConnectionAttachRequest(const sp<RfxMclMessage>& msg) {
    RFX_LOG_D(RFX_LOG_TAG, "[%s]", __FUNCTION__);
    sp<RfxAtResponse> response;
    sp<RfxMclMessage> responseToTcl;
    int max_retry_count = 8;
    int retry = 0;
    int interval = 0;

    // retry 30s, 1,1,2,2,4,4,8,8
    while (retry < max_retry_count) {
        response = atSendCommand(String8::format("AT+EGTYPE=4"));
        if (response == NULL || response->isAtResponseFail()) {
            responseToTcl = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR,
                                                          RfxVoidData(), msg, true);
            retry++;
            interval = (1 << (retry >> 1)) * 1000 * 1000;
            usleep(interval);
        } else {
            responseToTcl =
                    RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
            break;
        }
    }

    responseToTelCore(responseToTcl);
}

// Support requirements to do ps detach or ps&cs detach base on the parameter.
// This function is used only for Java Framework to trigger data-stall mechanism.
// Detach request and attach request will be always executed successively, Detach
// request is always the first one to be executed.
void RmcDcMsimReqHandler::handleDataConnectionDetachRequest(const sp<RfxMclMessage>& msg) {
    RFX_LOG_D(RFX_LOG_TAG, "[%s]", __FUNCTION__);
    sp<RfxAtResponse> response;
    sp<RfxMclMessage> responseToTcl;

    response = atSendCommand(String8::format("AT+EGTYPE=5"));
    if (response != NULL && !response->isAtResponseFail()) {
        responseToTcl =
                RfxMclMessage::obtainResponse(msg->getId(), RIL_E_SUCCESS, RfxVoidData(), msg);
    } else {
        responseToTcl = RfxMclMessage::obtainResponse(msg->getId(), RIL_E_MODEM_ERR, RfxVoidData(),
                                                      msg, true);
    }

    responseToTelCore(responseToTcl);
}

void RmcDcMsimReqHandler::updateDataAllowStatus(int slotId, int allow) {
    String8 tempString8Value;
    if (allow == 1) {
        getMclStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_SLOT_ALLOW, 1);
    } else {
        getMclStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_SLOT_ALLOW, 0);
    }
    RFX_LOG_D(RFX_LOG_TAG, "[updateDataAllowStatus] SIM[%d]: %d , allowed = %d", slotId,
              getMclStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_SLOT_ALLOW, 0), allow);
}

void RmcDcMsimReqHandler::updatePreDataAllowStatus(int slotId, int allow) {
    String8 tempString8Value;
    if (allow == 1) {
        getMclStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_SLOT_ALLOW, -2);
    } else {
        getMclStatusManager(slotId)->setIntValue(RFX_STATUS_KEY_SLOT_ALLOW, -1);
    }
    RFX_LOG_D(RFX_LOG_TAG, "[updatePreDataAllowStatus] SIM[%d]: %d , allowed = %d", slotId,
              getMclStatusManager(slotId)->getIntValue(RFX_STATUS_KEY_SLOT_ALLOW, 0), allow);
}

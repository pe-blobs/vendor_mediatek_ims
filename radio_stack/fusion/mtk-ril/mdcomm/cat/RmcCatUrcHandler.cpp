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

#include "RmcCatUrcHandler.h"

#define RFX_LOG_TAG "RmcCatUrcHandler"

// register handler to channel
RFX_IMPLEMENT_HANDLER_CLASS(RmcCatUrcHandler, RIL_CMD_PROXY_URC);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_STK_NOTIFY);
RFX_REGISTER_DATA_TO_EVENT_ID(RfxIntsData, RFX_MSG_EVENT_STK_QUERY_CPIN_STATE);

RmcCatUrcHandler::RmcCatUrcHandler(int slot_id, int channel_id)
    : RfxBaseHandler(slot_id, channel_id) {
    const char* urc[] = {"+STKPCI: 0", "+STKPCI: 1", "+STKPCI: 2",
                         "+STKCTRL:",  "+EUTKST:",   "+BIP:"};

    static const int event_list[] = {RFX_MSG_EVENT_STK_IS_RUNNING};

    registerToHandleURC(urc, sizeof(urc) / sizeof(char*));
    registerToHandleEvent(event_list, sizeof(event_list) / sizeof(int));
}

RmcCatUrcHandler::~RmcCatUrcHandler() {}

void RmcCatUrcHandler::onHandleEvent(const sp<RfxMclMessage>& msg) {
    int id = msg->getId();
    logD(RFX_LOG_TAG, "onHandleEvent: msg id: %d", id);
    switch (id) {
        default:
            logD(RFX_LOG_TAG, "onHandleEvent: should not be here");
            break;
    }
}

void RmcCatUrcHandler::onHandleUrc(const sp<RfxMclMessage>& msg) {
    char* urc = msg->getRawUrc()->getLine();
    if (strStartsWith(urc, "+STKPCI: 0")) {
        handleStkProactiveCommand(msg);
    } else if (strStartsWith(urc, "+STKPCI: 1")) {
        handleStkEventNotify(msg);
    } else if (strStartsWith(urc, "+STKPCI: 2")) {
        handleStkSessionEnd(msg);
    } else if (strStartsWith(urc, "+STKCTRL")) {
        // handleStkCallControl(msg);
        // C2K specific start
    } else if (strStartsWith(urc, "+EUTKST:")) {
        //   logD(RFX_LOG_TAG, "onHandleUrc: currently no need to handle EUTKST");
        // C2K specific end
    } else if (strStartsWith(urc, "+BIP:")) {
        handleBipEventNotify(msg);
    } else {
        logE(RFX_LOG_TAG, "onHandleUrc: should not be here");
    }
}

void RmcCatUrcHandler::handleStkProactiveCommand(const sp<RfxMclMessage>& msg) {
    int i, err, ind, urc_len = 0, cmdType = -1;
    int cmdId = -1;
    char* cmd = NULL;
    char* pProCmd = NULL;
    char* tempUrc = NULL;
    RfxAtLine* line = msg->getRawUrc();

    logD(RFX_LOG_TAG, "handleStkProactiveCommand");

    line->atTokStart(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleStkProactiveCommand: parse : error!");
        return;
    }

    ind = line->atTokNextint(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleStkProactiveCommand: parse ind error!");
        return;
    }

    cmd = line->atTokNextstr(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleStkProactiveCommand: parse urc error!");
        return;
    }

    if (line->atTokHasmore()) {
        cmdId = line->atTokNextint(&err);
        if (err < 0) {
            logE(RFX_LOG_TAG, "handleStkProactiveCommand: parse cmd id  error!");
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_STK_CMD_ID, -1);
            return;
        } else {
            getMclStatusManager()->setIntValue(RFX_STATUS_KEY_STK_CMD_ID, cmdId);
        }
    } else {
        getMclStatusManager()->setIntValue(RFX_STATUS_KEY_STK_CMD_ID, -1);
    }

    sp<RfxMclMessage> urc = RfxMclMessage::obtainUrc(RFX_MSG_URC_STK_PROACTIVE_COMMAND, m_slot_id,
                                                     RfxStringData(cmd));
    responseToTelCore(urc);
}

void RmcCatUrcHandler::handleStkEventNotify(const sp<RfxMclMessage>& msg) {
    int i, err, ind, urc_len = 0, cmdType = -1;
    char* cmd = NULL;
    RfxAtLine* line = msg->getRawUrc();

    logD(RFX_LOG_TAG, "handleStkEventNotify");
    line->atTokStart(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleStkEventNotify: parse : error!");
        return;
    }

    ind = line->atTokNextint(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleStkEventNotify: parse ind error!");
        return;
    }

    cmd = line->atTokNextstr(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleStkEventNotify: parse urc error!");
        return;
    }

    sp<RfxMclMessage> urc =
            RfxMclMessage::obtainUrc(RFX_MSG_URC_STK_EVENT_NOTIFY, m_slot_id, RfxStringData(cmd));
    responseToTelCore(urc);
    // Todo: May add the logic of RIL_UNSOL_STK_CALL_SETUP
}

void RmcCatUrcHandler::handleBipEventNotify(const sp<RfxMclMessage>& msg) {
    char* cmd = msg->getRawUrc()->getLine();

    logD(RFX_LOG_TAG, "handleBipEventNotify: %s", cmd);

    sp<RfxMclMessage> urc =
            RfxMclMessage::obtainUrc(RFX_MSG_URC_STK_EVENT_NOTIFY, m_slot_id, RfxStringData(cmd));
    responseToTelCore(urc);
}

void RmcCatUrcHandler::onHandleTimer() {}

void RmcCatUrcHandler::handleStkSessionEnd(const sp<RfxMclMessage>& msg) {
    RFX_UNUSED(msg);
    sp<RfxMclMessage> urc =
            RfxMclMessage::obtainUrc(RFX_MSG_URC_STK_SESSION_END, m_slot_id, RfxVoidData());
    responseToTelCore(urc);

    return;
}

void RmcCatUrcHandler::handleStkCallControl(const sp<RfxMclMessage>& msg) {
    logD(RFX_LOG_TAG, "handleStkCallControl");
    int i, err;
    char* responseStr[NUM_STK_CALL_CTRL] = {0};
    RfxAtLine* line = msg->getRawUrc();

    line->atTokStart(&err);
    if (err < 0) {
        logE(RFX_LOG_TAG, "handleStkCallControl: parse : error!");
        return;
    }

    for (i = 0; i < NUM_STK_CALL_CTRL; i++) {
        responseStr[i] = line->atTokNextstr(&err);
        if (err < 0) {
            logE(RFX_LOG_TAG, "handleStkCallControl: something wrong with item [%d]", i);
        }  // goto error;
    }

    sp<RfxMclMessage> urc =
            RfxMclMessage::obtainUrc(RFX_MSG_URC_STK_CC_ALPHA_NOTIFY, m_slot_id,
                                     RfxStringsData(responseStr, NUM_STK_CALL_CTRL));
    responseToTelCore(urc);
    return;
}

bool RmcCatUrcHandler::onCheckIfRejectMessage(const sp<RfxMclMessage>& msg,
                                              RIL_RadioState radioState) {
    bool reject = false;
    if (RADIO_STATE_UNAVAILABLE == radioState) {
        if (strStartsWith(msg->getRawUrc()->getLine(), "+STKPCI:")) {
            reject = false;
        } else {
            reject = true;
        }
    }
    logD(RFX_LOG_TAG, "onCheckIfRejectMessage: %d %d", radioState, reject);
    return reject;
}

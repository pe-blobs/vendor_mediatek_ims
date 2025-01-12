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

#include <stdlib.h>
#include "RfxDispatchThread.h"
#include "RfxIdToMsgIdUtils.h"
#include "RfxVoidData.h"
#include "RfxMessageId.h"

/*************************************************************
 * RfxDispatchThread
 *************************************************************/
extern "C" void* rfx_process_request_messages_loop(void* arg);
extern "C" void* rfx_process_response_messages_loop(void* arg);
extern "C" void* rfx_process_urc_messages_loop(void* arg);
extern "C" void* rfx_process_status_sync_messages_loop(void* arg);

RfxDispatchThread* RfxDispatchThread::s_self = NULL;

RfxDispatchThread::RfxDispatchThread() {
    memset(&requestThreadId, 0, sizeof(pthread_t));
    memset(&responseThreadId, 0, sizeof(pthread_t));
    memset(&urcThreadId, 0, sizeof(pthread_t));
    memset(&statusSyncThreadId, 0, sizeof(pthread_t));
}

RfxDispatchThread::~RfxDispatchThread() {}

#define RFX_LOG_TAG "RfxDisThread"

RfxDispatchThread* RfxDispatchThread::init() {
    RFX_LOG_D(RFX_LOG_TAG, "RfxDispatchThread init");
    s_self = new RfxDispatchThread();
    s_self->run("Ril Proxy request dispatch thread");
    return s_self;
}

bool RfxDispatchThread::threadLoop() {
    // init process message thread (request)
    pthread_attr_t reqAttr;
    PthreadPtr reqPptr = rfx_process_request_messages_loop;
    int result;
    pthread_attr_init(&reqAttr);
    pthread_attr_setdetachstate(&reqAttr, PTHREAD_CREATE_DETACHED);

    // Start request processing loop thread
    result = pthread_create(&requestThreadId, &reqAttr, reqPptr, this);
    if (result < 0) {
        RFX_LOG_D(RFX_LOG_TAG, "pthread_create failed with result:%d", result);
    }

    // init process message thread (response)
    pthread_attr_t resAttr;
    PthreadPtr resPptr = rfx_process_response_messages_loop;
    pthread_attr_init(&resAttr);
    pthread_attr_setdetachstate(&resAttr, PTHREAD_CREATE_DETACHED);

    // Start request processing loop thread
    result = pthread_create(&responseThreadId, &resAttr, resPptr, this);
    if (result < 0) {
        RFX_LOG_D(RFX_LOG_TAG, "pthread_create failed with result:%d", result);
    }

    // init process message thread (urc)
    pthread_attr_t urcAttr;
    PthreadPtr urcPptr = rfx_process_urc_messages_loop;
    pthread_attr_init(&urcAttr);
    pthread_attr_setdetachstate(&urcAttr, PTHREAD_CREATE_DETACHED);

    // Start request processing loop thread
    result = pthread_create(&urcThreadId, &urcAttr, urcPptr, this);
    if (result < 0) {
        RFX_LOG_D(RFX_LOG_TAG, "pthread_create failed with result:%d", result);
    }

    // init process message (update status)
    pthread_attr_t statusSyncAttr;
    PthreadPtr statusSyncPptr = rfx_process_status_sync_messages_loop;
    pthread_attr_init(&statusSyncAttr);
    pthread_attr_setdetachstate(&statusSyncAttr, PTHREAD_CREATE_DETACHED);

    // Start request processing loop thread
    result = pthread_create(&statusSyncThreadId, &statusSyncAttr, statusSyncPptr, this);
    if (result < 0) {
        RFX_LOG_D(RFX_LOG_TAG, "pthread_create failed with result:%d", result);
    }

    return true;
}

void RfxDispatchThread::enqueueRequestMessage(int request, void* data, size_t datalen, RIL_Token t,
                                              RIL_SOCKET_ID socket_id, int clientId) {
    RFX_LOG_D(RFX_LOG_TAG, "enqueueRequestMessage: request: %d", request);
    RfxRequestInfo* requestInfo = (RfxRequestInfo*)t;

    // use msgId to create RfxMessage
    int msgId = request;
    if (RFX_MESSAGE_ID_BEGIN >= request) {
        msgId = RfxIdToMsgIdUtils::idToMsgId(request);
    }
    if (INVALID_ID == msgId) {
        // send NOT_SUPPORT to RILJ
        sp<RfxMessage> resMsg =
                RfxMessage::obtainResponse(socket_id, request, requestInfo->token, INVALID_ID, -1,
                                           RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0, t);
        MessageObj* obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
    } else {
        sp<RfxMessage> msg = RfxMessage::obtainRequest(socket_id, msgId, requestInfo->token, data,
                                                       datalen, t, clientId);
        MessageObj* obj = createMessageObj(msg);
        dispatchRequestQueue.enqueue(obj);
    }
}

void RfxDispatchThread::enqueueSapRequestMessage(int request, void* data, size_t datalen,
                                                 RIL_Token t, RIL_SOCKET_ID socket_id) {
    RFX_LOG_D(RFX_LOG_TAG, "enqueueSapRequestMessage: request: %d", request);
    RfxSapSocketRequest* sapRequest = (RfxSapSocketRequest*)t;

    // use msgId to create RfxMessage
    int msgId = RfxIdToMsgIdUtils::sapIdToMsgId(request);
    if (INVALID_ID == msgId) {
        // send NOT_SUPPORT to BT process
        sp<RfxMessage> resMsg = RfxMessage::obtainSapResponse(
                socket_id, request, sapRequest->token, INVALID_ID, -1,
                (RIL_Errno)3 /* SAP error number is different with RIL. 3 is not_support */, NULL,
                0, t);
        MessageObj* obj = createMessageObj(resMsg);
        dispatchResponseQueue.enqueue(obj);
    } else {
        sp<RfxMessage> msg =
                RfxMessage::obtainSapRequest(socket_id, msgId, sapRequest->token, data, datalen, t);
        MessageObj* obj = createMessageObj(msg);
        dispatchRequestQueue.enqueue(obj);
    }
}

void RfxDispatchThread::enqueueResponseMessage(const sp<RfxMclMessage>& msg) {
    MessageObj* obj = pendingQueue.checkAndDequeue(msg->getToken());
    if (obj == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "enqueueResponseMessage(): No correspending request!");
        return;
    }

    sp<RfxMessage> message = RfxMessage::obtainResponse(
            msg->getSlotId(), obj->msg->getPId(), obj->msg->getPToken(), obj->msg->getId(),
            obj->msg->getToken(), msg->getError(), msg->getData(), obj->msg->getPTimeStamp(),
            obj->msg->getRilToken(), obj->msg->getClientId());
    MessageObj* dispatchObj = createMessageObj(message);

    dispatchResponseQueue.enqueue(dispatchObj);
    delete (obj);
}

void RfxDispatchThread::enqueueSapResponseMessage(const sp<RfxMclMessage>& msg) {
    MessageObj* obj = pendingQueue.checkAndDequeue(msg->getToken());
    if (obj == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "enqueueSapResponseMessage(): No correspending request!");
        return;
    }

    sp<RfxMessage> message = RfxMessage::obtainSapResponse(
            msg->getSlotId(), obj->msg->getPId(), obj->msg->getPToken(), obj->msg->getId(),
            obj->msg->getToken(), msg->getError(), msg->getData(), obj->msg->getPTimeStamp(),
            obj->msg->getRilToken());
    MessageObj* dispatchObj = createMessageObj(message);

    dispatchResponseQueue.enqueue(dispatchObj);
    delete (obj);
}

void RfxDispatchThread::enqueueUrcMessage(const sp<RfxMclMessage>& msg) {
    sp<RfxMessage> message =
            RfxMessage::obtainUrc(msg->getSlotId(), msg->getId(), *(msg->getData()));
    MessageObj* obj = createMessageObj(message);

    dispatchUrcQueue.enqueue(obj);
}

void RfxDispatchThread::enqueueSapUrcMessage(const sp<RfxMclMessage>& msg) {
    sp<RfxMessage> message =
            RfxMessage::obtainSapUrc(msg->getSlotId(), msg->getId(), *(msg->getData()));
    MessageObj* obj = createMessageObj(message);

    dispatchResponseQueue.enqueue(obj);
}

void RfxDispatchThread::enqueueStatusSyncMessage(const sp<RfxMclMessage>& msg) {
    sp<RfxMessage> message = RfxMessage::obtainStatusSync(
            msg->getSlotId(), msg->getStatusKey(), msg->getStatusValue(), msg->getForceNotify(),
            msg->getIsDefault(), msg->getIsUpdateForMock());
    MessageObj* obj = createMessageObj(message);

    dispatchStatusSyncQueue.enqueue(obj);
}

void RfxDispatchThread::updateConnectionState(RIL_SOCKET_ID socket_id, int isConnected) {
    sp<RfxMessage> message = RfxMessage::obtainStatusSync(socket_id, RFX_STATUS_CONNECTION_STATE,
                                                          RfxVariant((isConnected ? true : false)),
                                                          true, false, false);
    MessageObj* obj = createMessageObj(message);

    dispatchStatusSyncQueue.enqueue(obj);
}

void RfxDispatchThread::addMessageToPendingQueue(const sp<RfxMessage>& message) {
    RFX_LOG_D(RFX_LOG_TAG, "addMessageToPendingQueue pRequest = %d, pToken = %d, token = %d",
              message->getPId(), message->getPToken(), message->getToken());
    MessageObj* obj = createMessageObj(message);
    pendingQueue.enqueue(obj);
}

void RfxDispatchThread::processRequestMessageLooper() {
    MessageObj* obj = dispatchRequestQueue.dequeue();

    RfxMainThread::waitLooper();
    RfxMainThread::enqueueMessage(obj->msg);
    delete (obj);
}

void RfxDispatchThread::processResponseMessageLooper() {
    MessageObj* obj = dispatchResponseQueue.dequeue();

    RfxMainThread::waitLooper();
    RfxMainThread::enqueueMessage(obj->msg);
    delete (obj);
}

void RfxDispatchThread::processUrcMessageLooper() {
    MessageObj* obj = dispatchUrcQueue.dequeue();

    RfxMainThread::waitLooper();
    RfxMainThread::enqueueMessage(obj->msg);
    delete (obj);
}

void RfxDispatchThread::processStatusSyncMessageLooper() {
    MessageObj* obj = dispatchStatusSyncQueue.dequeue();

    RfxMainThread::waitLooper();
    RfxMainThread::enqueueMessage(obj->msg);
    delete (obj);
}

void RfxDispatchThread::clearPendingQueue() {
    RFX_LOG_D(RFX_LOG_TAG, "clearPendingQueue start");
    while (pendingQueue.empty() == 0) {
        MessageObj* obj = pendingQueue.dequeue();
        sp<RfxMessage> response = RfxMessage::obtainResponse(RIL_E_RADIO_NOT_AVAILABLE, obj->msg);
        RfxRilAdapter* adapter = RfxRilAdapter::getInstance();
        if (adapter != NULL) {
            adapter->responseToRilj(response);
        }
        delete (obj);
    }
    RFX_LOG_D(RFX_LOG_TAG, "clearPendingQueue end");
}

void RfxDispatchThread::removeMessageFromPendingQueue(int token) {
    RFX_LOG_D(RFX_LOG_TAG, "removeMessageFromPendingQueue, token = %d", token);
    MessageObj* obj = pendingQueue.checkAndDequeue(token);
    delete (obj);
}

MessageObj* createMessageObj(const sp<RfxMessage>& message) {
    MessageObj* obj = new MessageObj();
    obj->msg = message;
    obj->p_next = NULL;
    return obj;
}

extern "C" void* rfx_process_request_messages_loop(void* arg) {
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_request_messages_loop");
    while (1) {
        RfxDispatchThread* dispatchThread = (RfxDispatchThread*)arg;
        dispatchThread->processRequestMessageLooper();
    }
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_request_messages_loop close");
    return NULL;
}

extern "C" void* rfx_process_response_messages_loop(void* arg) {
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_response_messages_loop");
    while (1) {
        RfxDispatchThread* dispatchThread = (RfxDispatchThread*)arg;
        dispatchThread->processResponseMessageLooper();
    }
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_response_messages_loop close");
    return NULL;
}

extern "C" void* rfx_process_urc_messages_loop(void* arg) {
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_urc_messages_loop");
    while (1) {
        RfxDispatchThread* dispatchThread = (RfxDispatchThread*)arg;
        dispatchThread->processUrcMessageLooper();
    }
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_urc_messages_loop close");
    return NULL;
}

extern "C" void* rfx_process_status_sync_messages_loop(void* arg) {
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_status_sync_messages_loop");
    while (1) {
        RfxDispatchThread* dispatchThread = (RfxDispatchThread*)arg;
        dispatchThread->processStatusSyncMessageLooper();
    }
    RFX_LOG_D(RFX_LOG_TAG, "rfx_process_status_sync_messages_loop close");
    return NULL;
}

/*void RfxDispatchThread::enqueueRequestAckMessage(RILD_RadioTechnology_Group source, int slotId,
        int token, Parcel* parcel) {
    MessageObj *obj = pendingQueue.getClonedObj(token);
    if (obj == NULL) {
        RFX_LOG_D(RFX_LOG_TAG, "enqueueRequestAckMessage(): No correspending request!");
        return;
    }

    sp<RfxMessage> message;
    // TODO: FIXME
    if (obj->msg->getClientId() != -1) {
        //message = RfxMessage::obtainRequestAck(slotId, obj->msg->getPId(),
        //obj->msg->getPToken(), obj->msg->getId(), obj->msg->getToken(), source,
        //parcel, obj->msg->getClientId(), obj->msg->getPTimeStamp());
    } else {
        //message = RfxMessage::obtainRequestAck(obj->msg->getSlotId(), obj->msg->getPId(),
        //obj->msg->getPToken(), obj->msg->getId(), obj->msg->getToken(), source,
        //parcel, obj->msg->getPTimeStamp(), obj->msg->getRilToken());
    }
    message->setSentOnCdmaCapabilitySlot(obj->msg->getSentOnCdmaCapabilitySlot());
    MessageObj *dispatchObj = createMessageObj(message);

    dispatchResponseQueue.enqueue(dispatchObj);
    delete(obj);

}*/

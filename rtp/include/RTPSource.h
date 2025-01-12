/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef _IMS_RTP_SOURCE_H_

#define _IMS_RTP_SOURCE_H_

#include <stdint.h>

#include <media/stagefright/foundation/ABase.h>
#include <utils/List.h>
#include <utils/RefBase.h>

#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/ALooper.h>
#include "RTPBase.h"
#include "RxAdaptationInfo.h"

using namespace android;
using android::status_t;

class RxAdaptationInfo;

namespace imsma {
struct RTPAssembler;  // ToDo: how to resolve the inter-include issue

class RTPSource : public RefBase {
  public:
    enum {
        kWhatTimeUpdate = 'tmup',
        kWhatSendTMMBR = 'tmbr',
        kWhatDropCall = 'drop',
        kWhatUpdateDebugInfo = 'upde',
        kWhatNoRTP = 'noda',
    };
    RTPSource(uint32_t srcId, rtp_rtcp_config_t* pConfigPram, int32_t iTrackIndex,
              sp<AMessage>& notify);

    void processRTPPacket(const sp<ABuffer>& buffer);
    status_t processSenderInfo(const sp<ABuffer>& buffer);
    // void timeUpdate(uint32_t rtpTime, uint64_t ntpTime);
    // void byeReceived();

    List<sp<ABuffer> >* queue() { return &mQueue; }
    void setSsrc(uint32_t newSsrc) { mID = newSsrc; }

    bool isCSD(const sp<ABuffer>& accessUnit);
    bool GetdebugInfo(bool needNotify, int32_t* uiEncBitRate, uint32_t Operator);
    void reset();

    status_t addReceiverReportBlock(const sp<ABuffer>& buffer);
    // void addSDES(const AString& cname, const sp<ABuffer> &buffer);
    // void addFIR(const sp<ABuffer> &buffer);

    status_t peerPausedSendStream();
    status_t peerResumedSendStream();

    void start();
    void stop();

    /******for adaptation start********/
    void processTMMBN(sp<ABuffer> tmmbn_fci);
    void updateConfigParams(rtp_rtcp_config_t* pRTPNegotiatedParams);
    const sp<ABuffer> getNewTMMBRInfo();
    void EstimateTSDelay(int64_t mediaTimeUs);
    bool pollingCheckTSDelay();
    void clearTSDelayInfo();
    /******for adaptation end********/

  protected:
    virtual ~RTPSource();

  private:
    uint32_t mID;
    int32_t mTrackIndex;
    // rtp_rtcp_config_t mConfigParam;
    sp<AMessage> mNotify;

    RxAdaptationInfo* mAdaInfo;

    mutable Mutex mLock;
    // uint32_t mRRintervalUs;

    AString mCName;  // ToDo: need same with modem, need same with RTPSender?

    uint32_t mHighestSeqNumber;
    bool mHighestSeqNumberSet;
    int64_t mFirstPacketRecvTimeUs;
    uint32_t mFirstPacketSeqNum;
    uint32_t mClockRate;

    List<sp<ABuffer> > mQueue;
    sp<RTPAssembler> mAssembler;

    uint32_t getLostCount();
    uint32_t getIDamageCount();

    // bool mIssueFIRRequests;
    // int64_t mLastFIRRequestUs;
    // uint8_t mNextFIRSeqNo;

    bool queuePacket(const sp<ABuffer>& buffer);
    uint32_t extendSeqNumber(uint32_t seqNum, uint32_t mHighestSeqNumber);
    void calculateArrivalJitter(const sp<ABuffer>& buffer);

    void flushQueue();

    /******for adaptation start********/
    void updateStatisticInfo(const sp<ABuffer> buffer);
    bool checkAllowIncrEncBR();
    void NotifyTMMBR();
    /******for adaptation end********/

    bool mSupportTMMBR;
    bool mWaitingTMMBN;

    DISALLOW_EVIL_CONSTRUCTORS(RTPSource);
};

}  // namespace imsma

#endif  // _IMS_RTP_SOURCE_H_

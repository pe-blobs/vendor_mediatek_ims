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

#ifndef __RFX_IMS_CONFERENCE_CALL_MESSAGE_HANDLER_H__
#define __RFX_IMS_CONFERENCE_CALL_MESSAGE_HANDLER_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <map>
#include <string.h>
#include <string>
#include <vector>

#include "RfxMainThread.h"
#include "RfxXmlParser.h"
#include "RfxSignal.h"
#include "utils/RefBase.h"
#include "utils/StrongPointer.h"
//#include "utils/string.h"
#include "utils/Looper.h"
//#include "utils/Vector.h"

using ::android::Looper;
using ::android::MessageHandler;
using ::android::RefBase;
using ::android::sp;
// using ::android::string;
// using ::android::Vector;

using namespace std;
/*****************************************************************************
 * Class RtcImsConferenceCallMessageHandler
 *****************************************************************************/
class ConferenceCallUser;

class RtcImsConferenceCallMessageHandler : public DefaultHandler {
  public:
    // user (string) :Tel or SIP URI
    static const string USER;
    // user > display text (string)
    static const string DISPLAY_TEXT;
    // user > endpoint (string) : URI or GRUU or Phone number
    static const string ENDPOINT;
    // user > endpoint >status
    static const string STATUS;
    // attribute > entity
    static const string ENTITY;

    static const string HOST_INFO;
    static const string URI;
    static const string STATE;
    static const string USER_COUNT;
    static const string MAX_USER_COUNT;
    static const string VERSION;
    static const string XML_TAG;
    static const string CONFERENCE_INFO;
    static const string MEDIA;

    /**
     * status-type (String) :
     * "pending" : Endpoint is not yet in the session, but it is anticipated that he/she will
     *      join in the near future.
     * "dialing-out" : Focus has dialed out to connect the endpoint to the conference,
     *      but the endpoint is not yet in the roster (probably being authenticated).
     * "dialing-in" : Endpoint is dialing into the conference, not yet in the roster
     *      (probably being authenticated).
     * "alerting" : PSTN alerting or SIP 180 Ringing was returned for the outbound call;
     *      endpoint is being alerted.
     * "on-hold" : Active signaling dialog exists between an endpoint and a focus,
     *      but endpoint is "on-hold" for this conference, i.e., he/she is neither "hearing"
     *      the conference mix nor is his/her media being mixed in the conference.
     * "connected" : Endpoint is a participant in the conference. Depending on the media policies,
     *      he/she can send and receive media to and from other participants.
     * "disconnecting" : Focus is in the process of disconnecting the endpoint
     *      (e.g. in SIP a DISCONNECT or BYE was sent to the endpoint).
     * "disconnected" : Endpoint is not a participant in the conference, and no active dialog
     *      exists between the endpoint and the focus.
     * "muted-via-focus" : Active signaling dialog exists beween an endpoint and a focus and
     *      the endpoint can "listen" to the conference, but the endpoint's media is not being
     *      mixed into the conference.
     * "connect-fail" : Endpoint fails to join the conference by rejecting the conference call.
     */
    static const string STATUS_PENDING;
    static const string STATUS_DIALING_OUT;
    static const string STATUS_DIALING_IN;
    static const string STATUS_ALERTING;
    static const string STATUS_ON_HOLD;
    static const string STATUS_CONNECTED;
    static const string STATUS_DISCONNECTING;
    static const string STATUS_DISCONNECTED;
    static const string STATUS_MUTED_VIA_FOCUS;
    static const string STATUS_CONNECT_FAIL;
    // conference -info : SIP status code (integer)
    static const string SIP_STATUS_CODE;

  public:
    RtcImsConferenceCallMessageHandler();
    virtual ~RtcImsConferenceCallMessageHandler();
    vector<sp<ConferenceCallUser>> getUsers() const;
    void setMaxUserCount(string maxUserCount);
    int getMaxUserCount() const;
    void setCallId(const int& callId);
    int getCallId() const;
    int getUserCount() const;
    string getHostInfo() const;
    int getVersion() const;
    int getCEPState() const;
    int updateCEPState(string val);
    //    void startDocument();
    //    void characters(string ch, int start, int length);
    void startElement(string nodeName, string nodeValue, string attributeName,
                      string attributeValue);
    void endElement(string nodeName);

  public:
    enum { CEP_STATE_UNKNOWN, CEP_STATE_FULL, CEP_STATE_PARTIAL };

  private:
    int mCallId;
    int mIndex;
    int mUserCount;
    int mMaxUserCount;
    bool mParsingHostInfo;
    int mVersion;
    int mCEPState;
    string mHostInfo;
    sp<ConferenceCallUser> mUser;
    vector<sp<ConferenceCallUser>> mUsers;
    bool mMediaStart;
};

class ConferenceCallUser : public virtual RefBase {
  public:
    ConferenceCallUser();
    virtual ~ConferenceCallUser();

  public:
    string mEndPoint;     // Get from <endpoint entity="xxx">
    string mEntity;       // Get from <user entity="xxx">
    string mDisplayText;  // Get from <display-text>xxx</display-text>
    string mStatus;       // Get from <status>xxx</status>
    string mUserAddr;     // Parse from user entity, may be retored to local number
    int mIndex;           // Index in the xml full report
};
#endif /* __RFX_IMS_CONFERENCE_CALL_MESSAGE_HANDLER_H__ */

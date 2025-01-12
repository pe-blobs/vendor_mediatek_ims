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

#ifdef MTK_USE_HIDL
{RIL_REQUEST_GET_SIM_STATUS, radio::getIccCardStatusResponse},
        {RIL_REQUEST_ENTER_SIM_PIN, radio::supplyIccPinForAppResponse},
        {RIL_REQUEST_ENTER_SIM_PUK, radio::supplyIccPukForAppResponse},
        {RIL_REQUEST_ENTER_SIM_PIN2, radio::supplyIccPin2ForAppResponse},
        {RIL_REQUEST_ENTER_SIM_PUK2, radio::supplyIccPuk2ForAppResponse},
        {RIL_REQUEST_CHANGE_SIM_PIN, radio::changeIccPinForAppResponse},
        {RIL_REQUEST_CHANGE_SIM_PIN2, radio::changeIccPin2ForAppResponse},
        {RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION,
         radio::supplyNetworkDepersonalizationResponse},
        {RIL_REQUEST_GET_CURRENT_CALLS, radio::getCurrentCallsResponse},
        {RIL_REQUEST_DIAL, radio::dialResponse},
        {RIL_REQUEST_EMERGENCY_DIAL, radio::emergencyDialResponse},
        {RIL_REQUEST_GET_IMSI, radio::getIMSIForAppResponse},
        {RIL_REQUEST_HANGUP, radio::hangupConnectionResponse},
        {RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND, radio::hangupWaitingOrBackgroundResponse},
        {RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND,
         radio::hangupForegroundResumeBackgroundResponse},
        {RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE,
         radio::switchWaitingOrHoldingAndActiveResponse},
        {RIL_REQUEST_CONFERENCE, radio::conferenceResponse},
        {RIL_REQUEST_UDUB, radio::rejectCallResponse},
        {RIL_REQUEST_LAST_CALL_FAIL_CAUSE, radio::getLastCallFailCauseResponse},
        {RIL_REQUEST_SIGNAL_STRENGTH, radio::getSignalStrengthResponse},
        {RIL_REQUEST_VOICE_REGISTRATION_STATE, radio::getVoiceRegistrationStateResponse},
        {RIL_REQUEST_DATA_REGISTRATION_STATE, radio::getDataRegistrationStateResponse},
        {RIL_REQUEST_OPERATOR, radio::getOperatorResponse},
        {RIL_REQUEST_RADIO_POWER, radio::setRadioPowerResponse},
        {RIL_REQUEST_DTMF, radio::sendDtmfResponse}, {RIL_REQUEST_SEND_SMS, radio::sendSmsResponse},
        {RIL_REQUEST_SEND_SMS_EXPECT_MORE, radio::sendSMSExpectMoreResponse},
        {RIL_REQUEST_SETUP_DATA_CALL, radio::setupDataCallResponse},
        {RIL_REQUEST_SIM_IO, radio::iccIOForAppResponse},
        {RIL_REQUEST_SEND_USSD, radio::sendUssdResponse},
        {RIL_REQUEST_CANCEL_USSD, radio::cancelPendingUssdResponse},
        {RIL_REQUEST_GET_CLIR, radio::getClirResponse},
        {RIL_REQUEST_SET_CLIR, radio::setClirResponse},
        {RIL_REQUEST_QUERY_CALL_FORWARD_STATUS, radio::getCallForwardStatusResponse},
        {RIL_REQUEST_SET_CALL_FORWARD, radio::setCallForwardResponse},
        {RIL_REQUEST_QUERY_CALL_WAITING, radio::getCallWaitingResponse},
        {RIL_REQUEST_SET_CALL_WAITING, radio::setCallWaitingResponse},
        {RIL_REQUEST_SMS_ACKNOWLEDGE, radio::acknowledgeLastIncomingGsmSmsResponse},
        {RIL_REQUEST_GET_IMEI, NULL}, {RIL_REQUEST_GET_IMEISV, NULL},
        {RIL_REQUEST_ANSWER, radio::acceptCallResponse},
        {RIL_REQUEST_DEACTIVATE_DATA_CALL, radio::deactivateDataCallResponse},
        {RIL_REQUEST_QUERY_FACILITY_LOCK, radio::getFacilityLockForAppResponse},
        {RIL_REQUEST_SET_FACILITY_LOCK, radio::setFacilityLockForAppResponse},
        {RIL_REQUEST_CHANGE_BARRING_PASSWORD, radio::setBarringPasswordResponse},
        {RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE, radio::getNetworkSelectionModeResponse},
        {RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC,
         radio::setNetworkSelectionModeAutomaticResponse},
        {RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL, radio::setNetworkSelectionModeManualResponse},
        {RIL_REQUEST_QUERY_AVAILABLE_NETWORKS, radio::getAvailableNetworksResponse},
        {RIL_REQUEST_DTMF_START, radio::startDtmfResponse},
        {RIL_REQUEST_DTMF_STOP, radio::stopDtmfResponse},
        {RIL_REQUEST_BASEBAND_VERSION, radio::getBasebandVersionResponse},
        {RIL_REQUEST_SEPARATE_CONNECTION, radio::separateConnectionResponse},
        {RIL_REQUEST_SET_MUTE, radio::setMuteResponse},
        {RIL_REQUEST_GET_MUTE, radio::getMuteResponse},
        {RIL_REQUEST_QUERY_CLIP, radio::getClipResponse},
        {RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE, NULL},
        {RIL_REQUEST_DATA_CALL_LIST, radio::getDataCallListResponse},
        {RIL_REQUEST_OEM_HOOK_RAW, radio::sendRequestRawResponse},
        {RIL_REQUEST_OEM_HOOK_STRINGS, radio::sendRequestStringsResponse},
        {RIL_REQUEST_SCREEN_STATE,
         radio::sendDeviceStateResponse},  // Note the response function is different.
        {RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION, radio::setSuppServiceNotificationsResponse},
        {RIL_REQUEST_WRITE_SMS_TO_SIM, radio::writeSmsToSimResponse},
        {RIL_REQUEST_DELETE_SMS_ON_SIM, radio::deleteSmsOnSimResponse},
        {RIL_REQUEST_SET_BAND_MODE, radio::setBandModeResponse},
        {RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE, radio::getAvailableBandModesResponse},
        {RIL_REQUEST_STK_GET_PROFILE, NULL}, {RIL_REQUEST_STK_SET_PROFILE, NULL},
        {RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND, radio::sendEnvelopeResponse},
        {RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE, radio::sendTerminalResponseToSimResponse},
        {RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM,
         radio::handleStkCallSetupRequestFromSimResponse},
        {RIL_REQUEST_EXPLICIT_CALL_TRANSFER, radio::explicitCallTransferResponse},
        {RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE, radio::setPreferredNetworkTypeResponse},
        {RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE, radio::getPreferredNetworkTypeResponse},
        {RIL_REQUEST_GET_NEIGHBORING_CELL_IDS, radio::getNeighboringCidsResponse},
        {RIL_REQUEST_SET_LOCATION_UPDATES, radio::setLocationUpdatesResponse},
        {RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE, radio::setCdmaSubscriptionSourceResponse},
        {RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE, radio::setCdmaRoamingPreferenceResponse},
        {RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE, radio::getCdmaRoamingPreferenceResponse},
        {RIL_REQUEST_SET_TTY_MODE, radio::setTTYModeResponse},
        {RIL_REQUEST_QUERY_TTY_MODE, radio::getTTYModeResponse},
        {RIL_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE,
         radio::setPreferredVoicePrivacyResponse},
        {RIL_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE,
         radio::getPreferredVoicePrivacyResponse},
        {RIL_REQUEST_CDMA_FLASH, radio::sendCDMAFeatureCodeResponse},
        {RIL_REQUEST_CDMA_BURST_DTMF, radio::sendBurstDtmfResponse},
        {RIL_REQUEST_CDMA_VALIDATE_AND_WRITE_AKEY, NULL},
        {RIL_REQUEST_CDMA_SEND_SMS, radio::sendCdmaSmsResponse},
        {RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE, radio::acknowledgeLastIncomingCdmaSmsResponse},
        {RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG, radio::getGsmBroadcastConfigResponse},
        {RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG, radio::setGsmBroadcastConfigResponse},
        {RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION, radio::setGsmBroadcastActivationResponse},
        {RIL_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG, radio::getCdmaBroadcastConfigResponse},
        {RIL_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG, radio::setCdmaBroadcastConfigResponse},
        {RIL_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION, radio::setCdmaBroadcastActivationResponse},
        {RIL_REQUEST_CDMA_SUBSCRIPTION, radio::getCDMASubscriptionResponse},
        {RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM, radio::writeSmsToRuimResponse},
        {RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM, radio::deleteSmsOnRuimResponse},
        {RIL_REQUEST_DEVICE_IDENTITY, radio::getDeviceIdentityResponse},
        {RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE, radio::exitEmergencyCallbackModeResponse},
        {RIL_REQUEST_GET_SMSC_ADDRESS, radio::getSmscAddressResponse},
        {RIL_REQUEST_SET_SMSC_ADDRESS, radio::setSmscAddressResponse},
        {RIL_REQUEST_REPORT_SMS_MEMORY_STATUS, radio::reportSmsMemoryStatusResponse},
        {RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING, radio::reportStkServiceIsRunningResponse},
        {RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE, radio::getCdmaSubscriptionSourceResponse},
        {RIL_REQUEST_ISIM_AUTHENTICATION, radio::requestIsimAuthenticationResponse},
        {RIL_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU,
         radio::acknowledgeIncomingGsmSmsWithPduResponse},
        {RIL_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS, radio::sendEnvelopeWithStatusResponse},
        {RIL_REQUEST_VOICE_RADIO_TECH, radio::getVoiceRadioTechnologyResponse},
        {RIL_REQUEST_GET_CELL_INFO_LIST, radio::getCellInfoListResponse},
        {RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE, radio::setCellInfoListRateResponse},
        {RIL_REQUEST_SET_INITIAL_ATTACH_APN, radio::setInitialAttachApnResponse},
        {RIL_REQUEST_IMS_REGISTRATION_STATE, radio::getImsRegistrationStateResponse},
        {RIL_REQUEST_IMS_SEND_SMS, radio::sendImsSmsResponse},
        {RIL_REQUEST_SIM_TRANSMIT_APDU_BASIC, radio::iccTransmitApduBasicChannelResponse},
        {RIL_REQUEST_SIM_OPEN_CHANNEL, radio::iccOpenLogicalChannelResponse},
        {RIL_REQUEST_SIM_CLOSE_CHANNEL, radio::iccCloseLogicalChannelResponse},
        {RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL, radio::iccTransmitApduLogicalChannelResponse},
        {RIL_REQUEST_NV_READ_ITEM, radio::nvReadItemResponse},
        {RIL_REQUEST_NV_WRITE_ITEM, radio::nvWriteItemResponse},
        {RIL_REQUEST_NV_WRITE_CDMA_PRL, radio::nvWriteCdmaPrlResponse},
        {RIL_REQUEST_NV_RESET_CONFIG, radio::nvResetConfigResponse},
        {RIL_REQUEST_SET_UICC_SUBSCRIPTION, radio::setUiccSubscriptionResponse},
        {RIL_REQUEST_ALLOW_DATA, radio::setDataAllowedResponse},
        {RIL_REQUEST_GET_HARDWARE_CONFIG, radio::getHardwareConfigResponse},
        {RIL_REQUEST_SIM_AUTHENTICATION, radio::requestIccSimAuthenticationResponse},
        {RIL_REQUEST_GET_DC_RT_INFO, NULL}, {RIL_REQUEST_SET_DC_RT_INFO_RATE, NULL},
        {RIL_REQUEST_SET_DATA_PROFILE, radio::setDataProfileResponse},
        {RIL_REQUEST_SHUTDOWN, radio::requestShutdownResponse},
        {RIL_REQUEST_GET_RADIO_CAPABILITY, radio::getRadioCapabilityResponse},
        {RIL_REQUEST_SET_RADIO_CAPABILITY, radio::setRadioCapabilityResponse},
        {RIL_REQUEST_START_LCE, radio::startLceServiceResponse},
        {RIL_REQUEST_STOP_LCE, radio::stopLceServiceResponse},
        {RIL_REQUEST_PULL_LCEDATA, radio::pullLceDataResponse},
        {RIL_REQUEST_GET_ACTIVITY_INFO, radio::getModemActivityInfoResponse},
        {RIL_REQUEST_SET_ALLOWED_CARRIERS, radio::setAllowedCarriersResponse},
        {RIL_REQUEST_GET_ALLOWED_CARRIERS, radio::getAllowedCarriersResponse},
        {RIL_REQUEST_SEND_DEVICE_STATE, radio::sendDeviceStateResponse},
        {RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER, radio::setIndicationFilterResponse},
        {RIL_REQUEST_SET_SIM_CARD_POWER, radio::setSimCardPowerResponse},
        {RIL_REQUEST_START_NETWORK_SCAN, radio::startNetworkScanResponse},
        {RIL_REQUEST_STOP_NETWORK_SCAN, radio::stopNetworkScanResponse},
        {RIL_REQUEST_START_KEEPALIVE, radio::startKeepaliveResponse},
        {RIL_REQUEST_STOP_KEEPALIVE, radio::stopKeepaliveResponse},
        {RIL_REQUEST_SET_CARRIER_INFO_IMSI_ENCRYPTION,
         radio::setCarrierInfoForImsiEncryptionResponse},
        {RIL_REQUEST_SET_SIGNAL_STRENGTH_REPORTING_CRITERIA,
         radio::setSignalStrengthReportingCriteriaResponse},
        {RIL_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA,
         radio::setLinkCapacityReportingCriteriaResponse},
        {RIL_REQUEST_GET_SLOT_STATUS, radioConfig::getSimSlotsStatusResponse},
        {RIL_REQUEST_SET_LOGICAL_TO_PHYSICAL_SLOT_MAPPING, radioConfig::setSimSlotsMappingResponse},
        {RIL_REQUEST_SET_PREFERRED_DATA_MODEM, radioConfig::setPreferredDataModemResponse},
        {RIL_REQUEST_ENABLE_MODEM, radio::enableModemResponse},
        {RIL_REQUEST_SET_SYSTEM_SELECTION_CHANNELS, radio::setSystemSelectionChannelsResponse}, {
    RIL_REQUEST_GET_PHONE_CAPABILITY, radioConfig::getPhoneCapabilityResponse
}
#else

#ifdef C2K_RIL

#define VENDOR_RIL 1
// Define GSM RILD channel id to avoid necessary build error
#define RIL_CMD_PROXY_1 RIL_CHANNEL_1
#define RIL_CMD_PROXY_2 RIL_CHANNEL_1
#define RIL_CMD_PROXY_3 RIL_CHANNEL_1
#define RIL_CMD_PROXY_4 RIL_CHANNEL_1
#define RIL_CMD_PROXY_5 RIL_CHANNEL_1
#define RIL_CMD_PROXY_6 RIL_CHANNEL_1
#else
#define VENDOR_RIL 1
// GSM RILD Channel definition
#define RIL_CHANNEL_1 RIL_CMD_PROXY_1
#define RIL_CHANNEL_2 RIL_CMD_PROXY_1
#define RIL_CHANNEL_3 RIL_CMD_PROXY_1
#define RIL_CHANNEL_4 RIL_CMD_PROXY_1
#define RIL_CHANNEL_5 RIL_CMD_PROXY_1
#define RIL_CHANNEL_6 RIL_CMD_PROXY_1
#define RIL_CHANNEL_7 RIL_CMD_PROXY_1
#endif

#ifdef MTK_MUX_CHANNEL_64
// To send GSM SMS through non-realtime channel if the platform support 64 MUX channels
#define RIL_CHANNEL_GSM_SEND_SMS RIL_CMD_PROXY_8
#else
#define RIL_CHANNEL_GSM_SEND_SMS RIL_CMD_PROXY_1
#endif

#ifdef MTK_MUX_CHANNEL_64
#define MTK_RIL_CHANNEL_SIM RIL_CMD_PROXY_7
#else
#define MTK_RIL_CHANNEL_SIM RIL_CMD_PROXY_1
#endif
{RIL_REQUEST_GET_SIM_STATUS, dispatchVoid, responseSimStatus, MTK_RIL_CHANNEL_SIM, RIL_CHANNEL_1},
        {RIL_REQUEST_ENTER_SIM_PIN, dispatchStrings, responseInts, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_ENTER_SIM_PUK, dispatchStrings, responseInts, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_ENTER_SIM_PIN2, dispatchStrings, responseInts, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_ENTER_SIM_PUK2, dispatchStrings, responseInts, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_CHANGE_SIM_PIN, dispatchStrings, responseInts, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_CHANGE_SIM_PIN2, dispatchStrings, responseInts, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION, dispatchStrings, responseInts,
         RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_GET_CURRENT_CALLS, dispatchVoid, responseCallList, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_DIAL, dispatchDial, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_EMERGENCY_DIAL, dispatchEmergencyDial, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_GET_IMSI, dispatchStrings, responseString, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_HANGUP, dispatchInts, responseVoid, RIL_CMD_PROXY_4, RIL_CHANNEL_2},
        {RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND, dispatchVoid, responseVoid, RIL_CMD_PROXY_4,
         RIL_CHANNEL_2},
        {RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND, dispatchVoid, responseVoid,
         RIL_CMD_PROXY_4, RIL_CHANNEL_2},
        {RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE, dispatchVoid, responseVoid,
         RIL_CMD_PROXY_4, RIL_CHANNEL_2},
        {RIL_REQUEST_CONFERENCE, dispatchVoid, responseVoid, RIL_CMD_PROXY_4, RIL_CHANNEL_2},
        {RIL_REQUEST_UDUB, dispatchVoid, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_LAST_CALL_FAIL_CAUSE, dispatchVoid, responseFailCause, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_SIGNAL_STRENGTH, dispatchVoid, responseInts, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_VOICE_REGISTRATION_STATE, dispatchVoid, responseStrings, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_DATA_REGISTRATION_STATE, dispatchVoid, responseStrings, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_OPERATOR, dispatchVoid, responseStrings, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_RADIO_POWER, dispatchInts, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_3},
        {RIL_REQUEST_DTMF, dispatchString, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_SEND_SMS, dispatchStrings, responseSMS, RIL_CHANNEL_GSM_SEND_SMS,
         RIL_CHANNEL_5},
        {RIL_REQUEST_SEND_SMS_EXPECT_MORE, dispatchStrings, responseSMS, RIL_CHANNEL_GSM_SEND_SMS,
         RIL_CHANNEL_5},
        {RIL_REQUEST_SETUP_DATA_CALL, dispatchDataCall, responseSetupDataCall, RIL_CMD_PROXY_5,
         RIL_CHANNEL_4},
        {RIL_REQUEST_SIM_IO, dispatchSIM_IO, responseSIM_IO, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_SEND_USSD, dispatchString, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_CANCEL_USSD, dispatchVoid, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_GET_CLIR, dispatchVoid, responseInts, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_SET_CLIR, dispatchInts, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_QUERY_CALL_FORWARD_STATUS, dispatchCallForward, responseCallForwards,
         RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_SET_CALL_FORWARD, dispatchCallForward, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_QUERY_CALL_WAITING, dispatchInts, responseInts, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1}, /* Solve [ALPS00284553] Change to 1, mtk04070, 20120516 */
        {RIL_REQUEST_SET_CALL_WAITING, dispatchInts, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_SMS_ACKNOWLEDGE, dispatchInts, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_5},
        {RIL_REQUEST_GET_IMEI, dispatchVoid, responseString, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_GET_IMEISV, dispatchVoid, responseString, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_ANSWER, dispatchVoid, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_DEACTIVATE_DATA_CALL, dispatchStrings, responseVoid, RIL_CMD_PROXY_5,
         RIL_CHANNEL_4},
        {RIL_REQUEST_QUERY_FACILITY_LOCK, dispatchStrings, responseInts, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SET_FACILITY_LOCK, dispatchStrings, responseInts, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_CHANGE_BARRING_PASSWORD, dispatchStrings, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE, dispatchVoid, responseInts, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC, dispatchVoid, responseVoid, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL, dispatchString, responseVoid, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_QUERY_AVAILABLE_NETWORKS, dispatchVoid, responseStrings, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_DTMF_START, dispatchString, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_DTMF_STOP, dispatchVoid, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_BASEBAND_VERSION, dispatchVoid, responseString, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_SEPARATE_CONNECTION, dispatchInts, responseVoid, RIL_CMD_PROXY_4,
         RIL_CHANNEL_2},
        {RIL_REQUEST_SET_MUTE, dispatchInts, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_GET_MUTE, dispatchVoid, responseInts, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_QUERY_CLIP, dispatchVoid, responseInts, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE, dispatchVoid, responseInts, RIL_CMD_PROXY_5,
         RIL_CHANNEL_4},
        {RIL_REQUEST_DATA_CALL_LIST, dispatchVoid, responseDataCallList, RIL_CMD_PROXY_5,
         RIL_CHANNEL_4},
        {RIL_REQUEST_RESET_RADIO, dispatchVoid, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_OEM_HOOK_RAW, dispatchRaw, responseRaw, RIL_CMD_PROXY_6, RIL_CHANNEL_6},
        {RIL_REQUEST_OEM_HOOK_STRINGS, dispatchStrings, responseStrings, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_SCREEN_STATE, dispatchInts, responseVoid, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_SET_SUPP_SVC_NOTIFICATION, dispatchInts, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_WRITE_SMS_TO_SIM, dispatchSmsWrite, responseInts, RIL_CMD_PROXY_1,
         RIL_CHANNEL_5},
        {RIL_REQUEST_DELETE_SMS_ON_SIM, dispatchInts, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_SET_BAND_MODE, dispatchInts, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_3},
        {RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE, dispatchVoid, responseInts, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_STK_GET_PROFILE, dispatchVoid, responseString, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_STK_SET_PROFILE, dispatchString, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_STK_SEND_ENVELOPE_COMMAND, dispatchString, responseString, RIL_CMD_PROXY_1,
         RIL_CHANNEL_3},
        {RIL_REQUEST_STK_SEND_TERMINAL_RESPONSE, dispatchString, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_3},
        {RIL_REQUEST_STK_HANDLE_CALL_SETUP_REQUESTED_FROM_SIM, dispatchInts, responseVoid,
         RIL_CMD_PROXY_1, RIL_CHANNEL_3},
        {RIL_REQUEST_EXPLICIT_CALL_TRANSFER, dispatchVoid, responseVoid, RIL_CMD_PROXY_4,
         RIL_CHANNEL_2},
        {RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE, dispatchInts, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_3},
        {RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE, dispatchVoid, responseInts, RIL_CMD_PROXY_1,
         RIL_CHANNEL_3},
        {RIL_REQUEST_GET_NEIGHBORING_CELL_IDS, dispatchVoid, responseCellList, RIL_CMD_PROXY_4,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SET_LOCATION_UPDATES, dispatchInts, responseVoid, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE, dispatchInts, responseVoid, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_CDMA_SET_ROAMING_PREFERENCE, dispatchInts, responseVoid, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_CDMA_QUERY_ROAMING_PREFERENCE, dispatchVoid, responseInts, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_SET_TTY_MODE, dispatchInts, responseVoid, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_QUERY_TTY_MODE, dispatchVoid, responseInts, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_CDMA_SET_PREFERRED_VOICE_PRIVACY_MODE, dispatchInts, responseVoid,
         RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_CDMA_QUERY_PREFERRED_VOICE_PRIVACY_MODE, dispatchVoid, responseInts,
         RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_CDMA_FLASH, dispatchString, responseVoid, RIL_CMD_PROXY_2, RIL_CHANNEL_2},
        {RIL_REQUEST_CDMA_BURST_DTMF, dispatchStrings, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_CDMA_VALIDATE_AND_WRITE_AKEY, dispatchString, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_CDMA_SEND_SMS, dispatchCdmaSms, responseSMS, RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE, dispatchCdmaSmsAck, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_5},
        {RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG, dispatchVoid, responseGsmBrSmsCnf,
         RIL_CMD_PROXY_2, RIL_CHANNEL_5},
        {RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG, dispatchGsmBrSmsCnf, responseVoid,
         RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION, dispatchInts, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_5},
        {RIL_REQUEST_CDMA_GET_BROADCAST_SMS_CONFIG, dispatchVoid, responseCdmaBrSmsCnf,
         RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_CDMA_SET_BROADCAST_SMS_CONFIG, dispatchCdmaBrSmsCnf, responseVoid,
         RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_CDMA_SMS_BROADCAST_ACTIVATION, dispatchInts, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_5},
        {RIL_REQUEST_CDMA_SUBSCRIPTION, dispatchVoid, responseStrings, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_CDMA_WRITE_SMS_TO_RUIM, dispatchRilCdmaSmsWriteArgs, responseInts,
         RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_CDMA_DELETE_SMS_ON_RUIM, dispatchInts, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_5},
        {RIL_REQUEST_DEVICE_IDENTITY, dispatchVoid, responseStrings, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_EXIT_EMERGENCY_CALLBACK_MODE, dispatchVoid, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_GET_SMSC_ADDRESS, dispatchVoid, responseString, RIL_CMD_PROXY_2,
         RIL_CHANNEL_5},
        {RIL_REQUEST_SET_SMSC_ADDRESS, dispatchString, responseVoid, RIL_CMD_PROXY_2,
         RIL_CHANNEL_5},
        {RIL_REQUEST_REPORT_SMS_MEMORY_STATUS, dispatchInts, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_5},
        {RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING, dispatchVoid, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_3},
        {RIL_REQUEST_CDMA_GET_SUBSCRIPTION_SOURCE, dispatchVoid, responseInts, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_ISIM_AUTHENTICATION, dispatchString, responseString, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_ACKNOWLEDGE_INCOMING_GSM_SMS_WITH_PDU, dispatchStrings, responseVoid,
         RIL_CMD_PROXY_1, RIL_CHANNEL_5},
        {RIL_REQUEST_STK_SEND_ENVELOPE_WITH_STATUS, dispatchString, responseSIM_IO, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},
        {RIL_REQUEST_VOICE_RADIO_TECH, dispatchVoid, responseInts, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_GET_CELL_INFO_LIST, dispatchVoid, responseCellInfoList, RIL_CMD_PROXY_2,
         RIL_CHANNEL_2},  // ALPS01286560: getallcellinfo might be pending by PLMN list such long
                          // time request and case SWT .
        {RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE, dispatchInts, responseVoid, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_SET_INITIAL_ATTACH_APN, dispatchSetInitialAttachApn, responseVoid,
         RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},  // GSM CH should go with RIL_REQUEST_RADIO_POWER, C2K CH dont care
        {RIL_REQUEST_IMS_REGISTRATION_STATE, dispatchVoid, responseInts, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_IMS_SEND_SMS, dispatchImsSms, responseSMS, RIL_CHANNEL_GSM_SEND_SMS,
         RIL_CHANNEL_5},
        {RIL_REQUEST_SIM_TRANSMIT_APDU_BASIC, dispatchSIM_APDU, responseSIM_IO, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SIM_OPEN_CHANNEL, dispatchOpenChannelParams, responseInts, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SIM_CLOSE_CHANNEL, dispatchInts, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_SIM_TRANSMIT_APDU_CHANNEL, dispatchSIM_APDU, responseSIM_IO, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_NV_READ_ITEM, dispatchNVReadItem, responseString, RIL_CMD_PROXY_3,
         RIL_CHANNEL_1},
        {RIL_REQUEST_NV_WRITE_ITEM, dispatchNVWriteItem, responseVoid, RIL_CMD_PROXY_3,
         RIL_CHANNEL_1},
        {RIL_REQUEST_NV_WRITE_CDMA_PRL, dispatchRaw, responseVoid, RIL_CMD_PROXY_3, RIL_CHANNEL_1},
        {RIL_REQUEST_NV_RESET_CONFIG, dispatchInts, responseVoid, RIL_CMD_PROXY_3, RIL_CHANNEL_1},
        {RIL_REQUEST_SET_UICC_SUBSCRIPTION, dispatchUiccSubscripton, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_ALLOW_DATA, dispatchInts, responseVoid, RIL_CMD_PROXY_5, RIL_CHANNEL_4},
        {RIL_REQUEST_GET_HARDWARE_CONFIG, dispatchVoid, responseHardwareConfig, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SIM_AUTHENTICATION, dispatchSimAuthentication, responseSIM_IO, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_GET_DC_RT_INFO, dispatchVoid, responseDcRtInfo, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SET_DC_RT_INFO_RATE, dispatchInts, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SET_DATA_PROFILE, dispatchDataProfile, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SHUTDOWN, dispatchVoid, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_GET_RADIO_CAPABILITY, dispatchVoid, responseRadioCapability, RIL_CMD_PROXY_1,
         RIL_CHANNEL_3},
        {RIL_REQUEST_SET_RADIO_CAPABILITY, dispatchRadioCapability, responseRadioCapability,
         RIL_CMD_PROXY_1, RIL_CHANNEL_3},
        {RIL_REQUEST_START_LCE, dispatchInts, responseLceStatus, RIL_CMD_PROXY_5, RIL_CHANNEL_4},
        {RIL_REQUEST_STOP_LCE, dispatchVoid, responseLceStatus, RIL_CMD_PROXY_5, RIL_CHANNEL_4},
        {RIL_REQUEST_PULL_LCEDATA, dispatchVoid, responseLceData, RIL_CMD_PROXY_5, RIL_CHANNEL_4},
        {RIL_REQUEST_GET_ACTIVITY_INFO, dispatchVoid, responseActivityData, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SET_ALLOWED_CARRIERS, dispatchCarrierRestrictions, responseInts,
         RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_GET_ALLOWED_CARRIERS, dispatchVoid, responseCarrierRestrictions,
         RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_SEND_DEVICE_STATE, dispatchInts, responseVoid, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_SET_UNSOLICITED_RESPONSE_FILTER, dispatchInts, responseVoid, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_SET_SIM_CARD_POWER, dispatchInts, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_START_NETWORK_SCAN, dispatchNetworkScan, responseVoid, RIL_CMD_PROXY_3,
         RIL_CHANNEL_3},
        {RIL_REQUEST_STOP_NETWORK_SCAN, dispatchVoid, responseVoid, RIL_CMD_PROXY_3, RIL_CHANNEL_3},
        {RIL_REQUEST_START_KEEPALIVE, dispatchKeepAlive, responseKeepAlive, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_STOP_KEEPALIVE, dispatchInts, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_1},
        {RIL_REQUEST_SET_CARRIER_INFO_IMSI_ENCRYPTION, dispatchVoid, responseVoid, RIL_CMD_PROXY_1,
         RIL_CHANNEL_1},
        {RIL_REQUEST_SET_LINK_CAPACITY_REPORTING_CRITERIA, dispatchLinkCapacityReportingCriteria,
         responseVoid, RIL_CMD_PROXY_5, RIL_CHANNEL_4},
        {RIL_REQUEST_SET_PREFERRED_DATA_MODEM, dispatchInts, responseVoid, RIL_CMD_PROXY_5,
         RIL_CHANNEL_4},
        {RIL_REQUEST_ENABLE_MODEM, dispatchInts, responseVoid, RIL_CMD_PROXY_1, RIL_CHANNEL_3}, {
    RIL_REQUEST_GET_PHONE_CAPABILITY, dispatchVoid, responsePhoneCapability, RIL_CMD_PROXY_1,
            RIL_CHANNEL_3
}
#endif /* MTK_USE_HIDL */

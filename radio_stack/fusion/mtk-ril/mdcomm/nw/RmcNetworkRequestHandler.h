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

#ifndef __RMC_NETWORK_REQUEST_HANDLER_H__
#define __RMC_NETWORK_REQUEST_HANDLER_H__

#include "RmcNetworkHandler.h"
#include "RfxNeighboringCellData.h"
#include "RfxCellInfoData.h"
#include "RfxNetworkScanData.h"
#include "RfxNetworkScanResultData.h"
#include "RfxSsrcData.h"
#include "RfxSscData.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RmcNwReqHdlr"

/* RIL Network Enumeration */
typedef enum {
    GSM_BAND_900 = 0x02,
    GSM_BAND_1800 = 0x08,
    GSM_BAND_1900 = 0x10,
    GSM_BAND_850 = 0x80
} GSM_BAND_ENUM;

typedef enum {
    UMTS_BAND_I = 0x0001,
    UMTS_BAND_II = 0x0002,
    UMTS_BAND_III = 0x0004,
    UMTS_BAND_IV = 0x0008,
    UMTS_BAND_V = 0x0010,
    UMTS_BAND_VI = 0x0020,
    UMTS_BAND_VII = 0x0040,
    UMTS_BAND_VIII = 0x0080,
    UMTS_BAND_IX = 0x0100,
    UMTS_BAND_X = 0x0200
} UMTS_BAND_ENUM;

typedef enum {
    BM_AUTO_MODE,
    BM_EURO_MODE,
    BM_US_MODE,
    BM_JPN_MODE,
    BM_AUS_MODE,
    BM_AUS2_MODE,
    BM_CELLULAR_MODE,
    BM_PCS_MODE,
    BM_CLASS_3,
    BM_CLASS_4,
    BM_CLASS_5,
    BM_CLASS_6,
    BM_CLASS_7,
    BM_CLASS_8,
    BM_CLASS_9,
    BM_CLASS_10,
    BM_CLASS_11,
    BM_CLASS_15,
    BM_CLASS_16,
    BM_40_BROKEN = 100,
    BM_FOR_DESENSE_RADIO_ON = 200,
    BM_FOR_DESENSE_RADIO_OFF = 201,
    BM_FOR_DESENSE_RADIO_ON_ROAMING = 202,
    BM_FOR_DESENSE_B8_OPEN = 203
} BAND_MODE;

typedef enum {
    CDMA_ROAMING_MODE_RADIO_DEFAULT = -1,
    CDMA_ROAMING_MODE_HOME = 0,
    CDMA_ROAMING_MODE_AFFILIATED = 1,
    CDMA_ROAMING_MODE_ANY = 2
} CDMA_ROAMING_MODE;

typedef enum {
    // Legacy version which use ECREG to optimize URC
    LEGACY = 0,
    // EREG5 to control URC state when phase out ECREG
    SUPPORT_EREG5 = 5
} EREG_REPORT_MODE;

#define RIL_NW_INIT_INT \
    { 0, 0, 0, 0 }

class RmcNetworkRequestHandler : public RmcNetworkHandler {
    RFX_DECLARE_HANDLER_CLASS(RmcNetworkRequestHandler);

  public:
    RmcNetworkRequestHandler(int slot_id, int channel_id);
    virtual ~RmcNetworkRequestHandler();

    static bool isInService(int regState) {
        if (regState == 1 || regState == 5) {
            return true;
        }
        return false;
    }

  protected:
    virtual void onHandleRequest(const sp<RfxMclMessage>& msg);

    virtual void onHandleTimer();

    virtual void onHandleEvent(const sp<RfxMclMessage>& msg);

  private:
    void updateCellularPsState();
    void requestSignalStrength(const sp<RfxMclMessage>& msg);
    void requestSetNetworkSelectionAutomatic(const sp<RfxMclMessage>& msg);
    void requestSetNetworkSelectionManual(const sp<RfxMclMessage>& msg);
    void requestSetNetworkSelectionManualWithAct(const sp<RfxMclMessage>& msg);
    void requestSetBandMode(const sp<RfxMclMessage>& msg);
    void requestQueryAvailableBandMode(const sp<RfxMclMessage>& msg);
    void requestGetNeighboringCellIds(const sp<RfxMclMessage>& msg);
    void requestSetLocationUpdates(const sp<RfxMclMessage>& msg);
    void requestGetCellInfoList(const sp<RfxMclMessage>& msg);
    void requestSetCellInfoListRate(const sp<RfxMclMessage>& msg);
    void requestGetPOLCapability(const sp<RfxMclMessage>& msg);
    void requestGetPOLList(const sp<RfxMclMessage>& msg);
    void requestSetPOLEntry(const sp<RfxMclMessage>& msg);
    void requestSetCdmaRoamingPreference(const sp<RfxMclMessage>& msg);
    void requestQueryCdmaRoamingPreference(const sp<RfxMclMessage>& msg);
    void requestGetFemtocellList(const sp<RfxMclMessage>& msg);
    void requestAbortFemtocellList(const sp<RfxMclMessage>& msg);
    void requestSelectFemtocell(const sp<RfxMclMessage>& msg);
    void requestScreenState(const sp<RfxMclMessage>& msg);
    void requestSetUnsolicitedResponseFilter(const sp<RfxMclMessage>& msg);
    void requestQueryFemtoCellSystemSelectionMode(const sp<RfxMclMessage>& msg);
    void requestSetFemtoCellSystemSelectionMode(const sp<RfxMclMessage>& msg);
    void requestAntennaConf(const sp<RfxMclMessage>& msg);
    void requestAntennaInfo(const sp<RfxMclMessage>& msg);
    void requestSetServiceState(const sp<RfxMclMessage>& msg);
    void requestSetPseudoCellMode(const sp<RfxMclMessage>& msg);
    void requestGetPseudoCellInfo(const sp<RfxMclMessage>& msg);
    void updatePseudoCellMode();
    void updateSignalStrength();
    void setUnsolResponseFilterSignalStrength(bool enable);
    void setUnsolResponseFilterNetworkState(bool enable);
    void setUnsolResponseFilterLinkCapacityEstimate(bool enable);
    void requestQueryCurrentBandMode();
    void triggerPollNetworkState();
    int isEnableModulationReport();
    void handleConfirmRatBegin(const sp<RfxMclMessage>& msg);
    void handleCsNetworkStateEvent(const sp<RfxMclMessage>& msg);
    void handlePsNetworkStateEvent(const sp<RfxMclMessage>& msg);
    void setRoamingEnable(const sp<RfxMclMessage>& msg);
    void getRoamingEnable(const sp<RfxMclMessage>& msg);
    int isDisable2G();
    void requestSetLteReleaseVersion(const sp<RfxMclMessage>& msg);
    void requestGetLteReleaseVersion(const sp<RfxMclMessage>& msg);
    void currentPhysicalChannelConfigs(bool forceUpdate);
    void requestSetSignalStrengthReportingCriteria(const sp<RfxMclMessage>& msg);
    void requestSetSystemSelectionChannels(const sp<RfxMclMessage>& msg);
    void requestGetTs25Name(const sp<RfxMclMessage>& msg);
    void requestEnableCaPlusFilter(const sp<RfxMclMessage>& msg);
    bool handleGetDataContextIds(const sp<RfxMclMessage>& msg);
    void fillCidToPhysicalChannelConfig(RIL_PhysicalChannelConfig* pcc);
    void requestGetEhrpdInfo(const sp<RfxMclMessage>& msg);
    void registerCellularQualityReport(const sp<RfxMclMessage>& msg);  // MUSE WFC requirement
    void requestGetSuggestedPlmnList(const sp<RfxMclMessage>& msg);
    // NR request
    void requestConfigA2Offset(const sp<RfxMclMessage>& msg);
    void requestConfigB1Offset(const sp<RfxMclMessage>& msg);
    void requestEnableSCGFailure(const sp<RfxMclMessage>& msg);
    void requestDisableNr(const sp<RfxMclMessage>& msg);
    void requestSetTxPower(const sp<RfxMclMessage>& msg);
    void requestSearchStoreFrenquencyInfo(const sp<RfxMclMessage>& msg);
    void requestSearchRat(const sp<RfxMclMessage>& msg);
    void requestSetBackgroundSearchTimer(const sp<RfxMclMessage>& msg);

  protected:
    int m_slot_id;
    int m_channel_id;
    int m_emergency_only;
    int m_csgListOngoing = 0;
    int m_csgListAbort = 0;

    // Add for band de-sense feature.
    int bands[4] = RIL_NW_INIT_INT;
    // MTK_TC1_FEATURE for Antenna Testing start
    int antennaTestingType = 0;
    // MTK_TC1_FEATURE for Antenna Testing end
    // EREG=5 & EGREG=5 support
    int support_ereg_5 = 1;
};

#endif

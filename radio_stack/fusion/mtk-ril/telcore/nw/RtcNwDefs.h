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

#ifndef __RTC_NW_DEFS_H__
#define __RTC_NW_DEFS_H__

/*****************************************************************************
 * Include
 *****************************************************************************/
#include <telephony/mtk_ril.h>

/*****************************************************************************
 * Define
 *****************************************************************************/

/* Define the combined preferred network type. */
typedef enum {
    RAT_SWITCH_FAIL = -1,  // RAT switch failed
    RAT_SWITCH_SUCC = 0,   // RAT switch success
} RatSwitchResult;

typedef enum {
    PLMN_SEARCH_START = 0,
    PLMN_SEARCH_COMPLETE = 1,
} PlmnSearchStage;

/* Define the network selecting mode. */
typedef enum {
    NWS_MODE_UNKNOWN = -1,  // for CSFB (GSM) network selecting mode
    NWS_MODE_CDMALTE = 0,   // for CDMALTE (CDMA) network selecting mode
    NWS_MODE_CSFB = 1,      // for CSFB (GSM) network selecting mode
} NwsMode;

/* Define the network selecting mode. */
typedef enum {
    GMSS_RAT_3GPP2 = 0,    // Any RAT in 3GPP2 RAT group depend on other information.
    GMSS_RAT_3GPP = 1,     // Any RAT in 3GPP RAT group depend on other information.
    GMSS_RAT_C2K1X = 2,    // CDMA2000 1x
    GMSS_RAT_C2KHRPD = 3,  // CDMA2000 HRPD
    GMSS_RAT_GSM = 4,      // GERAN (GSM)
    GMSS_RAT_WCDMA = 5,    // UTRAN (WCDMA)
    GMSS_RAT_LTE = 6,      // EUTRAN (LTE)
    GMSS_RAT_INVALID = 7   // Invalid  RAT
} GmssRat;

/* Define the GMSS status. */
typedef enum {
    GMSS_STATUS_SELECT,    // GMSS select.
    GMSS_STATUS_ATTACHED,  // GMSS attached.
    GMSS_STATUS_ECC        // GMSS ECC.
} GmssStatus;

/*  <MSPL_set> integer type;
 *  The set(no priority) of Multimode System Selection (MMSS) System Priority List.
 *  Enum specified as:
 *  RAT_NONE              = 0,
 *  RAT_GSM               = 1,
 *  RAT_UMTS              = 2,
 *  RAT_GSM_UMTS          = RAT_GSM | RAT_UMTS,                    // 3
 *  RAT_LTE               = 4,
 *  RAT_GSM_LTE           = RAT_GSM | RAT_LTE,                     // 5
 *  RAT_UMTS_LTE          = RAT_UMTS | RAT_LTE,                    // 6
 *  RAT_GSM_UMTS_LTE      = RAT_GSM | RAT_UMTS | RAT_LTE,          // 7
 *  RAT_COM_GSM           = 8,
 *  RAT_C2K               = 16,
 *  RAT_GSM_C2K           = RAT_GSM | RAT_C2K,                     // 17
 *  RAT_UMTS_C2K          = RAT_UMTS | RAT_C2K,                    // 18
 *  RAT_GSM_UMTS_C2K      = RAT_GSM | RAT_UMTS | RAT_C2K,          // 19
 *  RAT_LTE_C2K           = RAT_LTE | RAT_C2K,                     // 20
 *  RAT_GSM_LTE_C2K       = RAT_GSM | RAT_LTE | RAT_C2K,           // 21
 *  RAT_UMTS_LTE_C2K      = RAT_UMTS | RAT_LTE | RAT_C2K,          // 22
 *  RAT_GSM_UMTS_LTE_C2K  = RAT_GSM | RAT_UMTS | RAT_LTE | RAT_C2K // 23
 *  RAT_1xRTT             = 32,
 *  RAT_HRPD              = 64,
 *  RAT_1xRTT_HRPD        = RAT_1xRTT | RAT_HRPD                   // 96
 */

typedef enum {
    MSPL_RAT_NONE = 0,
    MSPL_RAT_GSM = 1 << 0,      // 1
    MSPL_RAT_UMTS = 1 << 1,     // 2
    MSPL_RAT_LTE = 1 << 2,      // 4
    MSPL_RAT_COM_GSM = 1 << 3,  // 8
    MSPL_RAT_C2K = 1 << 4,      // 16
    MSPL_RAT_1xRTT = 1 << 5,    // 32
    MSPL_RAT_HRPD = 1 << 6      // 64
} MSPL;

typedef struct {
    int32_t rat;
    int32_t mcc;
    int32_t status;
    int32_t mspl;
    int32_t ishome;
} GmssInfo;

#endif /* __RTC_NW_DEFS_H__ */

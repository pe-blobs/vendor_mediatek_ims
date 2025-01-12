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

#ifndef RMC_GSM_SMS_DEFS_H
#define RMC_GSM_SMS_DEFS_H 1

#define MAX_SMSC_LENGTH 11
#define MAX_TPDU_LENGTH 164
#define RIL_SMS_MEM_TYPE_TOTAL 3

#define TPDU_MAX_TPDU_SIZE (175)
#define TPDU_MAX_ADDR_LEN (11)
#define TPDU_ONE_MSG_OCTET (140)

#define TPDU_MTI_BITS (0x03)
#define TPDU_MTI_DELIVER (0x00)
#define TPDU_MTI_SUBMIT (0x01)
#define TPDU_MTI_UNSPECIFIED (0x02)
#define TPDU_MTI_RESERVED (0x03)

#define TPDU_VPF_BITS (0x18)
#define TPDU_VPF_NOT_PRESENT (0x00)
#define TPDU_VPF_ENHANCED (0x01)
#define TPDU_VPF_RELATIVE (0x02)
#define TPDU_VPF_ABSOLUTE (0x03)

/*------------------------------
 * Protocol Identifier (PID)
 *------------------------------*/
#define TPDU_PID_TYPE_0 (0x40)
#define TPDU_PID_REP_TYPE_1 (0x41)     /* Replace Type 1 */
#define TPDU_PID_REP_TYPE_2 (0x42)     /* Replace Type 2 */
#define TPDU_PID_REP_TYPE_3 (0x43)     /* Replace Type 3 */
#define TPDU_PID_REP_TYPE_4 (0x44)     /* Replace Type 4 */
#define TPDU_PID_REP_TYPE_5 (0x45)     /* Replace Type 5 */
#define TPDU_PID_REP_TYPE_6 (0x46)     /* Replace Type 6 */
#define TPDU_PID_REP_TYPE_7 (0x47)     /* Replace Type 7 */
#define TPDU_PID_RCM (0x5f)            /* Return Call Message */
#define TPDU_PID_ANSI_136_RDATA (0x7c) /* ANSI-136 R-DATA */
#define TPDU_PID_ME_DOWNLOAD (0x7d)    /* ME Data Download */
#define TPDU_PID_ME_DE_PERSONAL (0x7e) /* ME De-personalization */
#define TPDU_PID_SIM_DOWNLOAD (0x7f)   /* SIM Data Download */

#define TPDU_PID_CHECK (0xC0)
#define TPDU_PID_MASK (0xE0)
#define TPDU_PID_RESERVED (0x80)

/*------------------------------------
 * Data Coding Scheme (DCS) Checking
 *------------------------------------*/
#define TPDU_DCS_DEFAULT (0x00)
#define TPDU_DCS_CODING1 (0xc0)
#define TPDU_DCS_CODING2 (0xf0)
#define TPDU_DCS_RESERVE_BIT (0x08)
#define TPDU_DCS_ALPHABET_CHECK (0x0c)
#define TPDU_DCS_COMPRESS_CHECK (0x20)

/******************************
 * Although CB CHANNEL ID can be 65535 defined in the spec.
 * But currently, all application only support the ID 0-999
 * so we define the MAX_CB_CHANNEL_ID as 6500
 * > MTK60048 - Change the value of MAX_CB_CHANNEL_ID to support CB service in EU
 *   EU-Alert level 1                                            : 4370
 *   EU-Alert level 2                                            : 4371-4372
 *   EU-Alert level 3                                            : 4373-4378
 *   EU-Info Message                                             : 6400
 *   EU-Amber                                                    : 4379
 *   CMAS CBS Message Identifier for the Required Monthly Test.  : 4380
 ******************************/
#define MAX_CB_CHANNEL_ID 6500
#define MAX_CB_DCS_ID 256

typedef enum {
    RIL_SMS_REC_UNREAD,
    RIL_SMS_REC_RDAD,
    RIL_SMS_STO_UNSENT,
    RIL_SMS_STO_SENT,
    RIL_SMS_MESSAGE_MAX
} RIL_SMS_MESSAGE_STAT;

typedef enum {
    SMS_ENCODING_7BIT,
    SMS_ENCODING_8BIT,
    SMS_ENCODING_16BIT,
    SMS_ENCODING_UNKNOWN
} SMS_ENCODING_ENUM;

typedef enum {
    SMS_MESSAGE_CLASS_0,
    SMS_MESSAGE_CLASS_1,
    SMS_MESSAGE_CLASS_2,
    SMS_MESSAGE_CLASS_3,
    SMS_MESSAGE_CLASS_UNSPECIFIED
} SMS_MESSAGE_CLASS_ENUM;

typedef enum {
    TPDU_PID_DEFAULT_PID = 0x00, /* Text SMS */
    TPDU_PID_TELEX_PID = 0x21,   /* Telex */
    TPDU_PID_G3_FAX_PID = 0x22,  /* Group 3 telefax */
    TPDU_PID_G4_FAX_PID = 0x23,  /* Group 4 telefax */
    TPDU_PID_VOICE_PID = 0x24,   /* Voice Telephone */
    TPDU_PID_ERMES_PID = 0x25,   /* ERMES (European Radio Messaging System) */
    TPDU_PID_PAGING_PID = 0x26,  /* National Paging system */
    TPDU_PID_X400_PID = 0x31,    /* Any public X.400-based message system */
    TPDU_PID_EMAIL_PID = 0x32    /* E-mail SMS */
} TPDU_PID_ENUM;

typedef enum {
    TPDU_NO_ERROR = 0x00,

    /* TP-DA Error */
    TPDU_DA_LENGTH_ERROR = 0x01,

    /* TP-VPF Error */
    TPDU_VPF_NO_SUPPORT = 0x02,

    /* TP-MTI Error */
    TPDU_MTI_SUBMIT_ERROR = 0x03,
    TPDU_MTI_DELIVER_ERROR = 0x04,

    /* TP-OA Error */
    TPDU_OA_LENGTH_ERROR = 0x05,

    /* Length Error */
    TPDU_MSG_LEN_EXCEEDED = 0x06,

    /* TP-PID Error */
    TELEMATIC_INT_WRK_NOT_SUPPORT = 0x80, /* telematic interworking not support */
    SMS_TYPE0_NOT_SUPPORT = 0x81,         /* short message type 0 not support */
    CANNOT_REPLACE_MSG = 0x82,
    UNSPECIFIED_PID_ERROR = 0x8F,

    /* DCS error */
    ALPHABET_NOT_SUPPORT = 0x90,  /* data coding scheme (alphabet) not support */
    MSG_CLASS_NOT_SUPPORT = 0x91, /* message class not support */
    UNSPECIFIED_TP_DCS = 0x9f,
} TPDU_ERROR_CAUSE_ENUM;

typedef enum {
    /* Check for telematics support */
    TPDU_TELEMATICS_CHECK = 0x20,

    /* Check for TYPE0 short messages */
    TPDU_MSG_TYPE0_CHECK = 0x3F,

    /* Check for optional paramter
     * of TPDU header PID, DCS, UDL */
    TPDU_PARAM_CHECK = 0x03,
    TPDU_DCS_PRESENT = 0x02,
    TPDU_PID_PRESENT = 0x01,

    /* Check for report messages
     * coming for SMS COMMAND */
    TPDU_COMMAND_CHECK = 0xFF,

    /* Check for length of TPDU */
    TPDU_TPDU_LEN_CHECK = 175
} TPDU_MSG_CHECK_ENUM;

enum {
    CMS_ERROR_NON_CMS = -1,
    CMS_SUCCESS = 0,
    CMS_CM_UNASSIGNED_NUM = 1,          // Unassigned (unallocated) number
    CMS_CM_OPR_DTR_BARRING = 8,         // Operator determined barring
    CMS_CM_CALL_BARRED = 10,            // Call barred
    CMS_CM_CALL_REJECTED = 21,          // Short message transfer rejected
    CMS_CM_DEST_OUT_OF_ORDER = 27,      // Destination out of order
    CMS_CM_INVALID_NUMBER_FORMAT = 28,  // Unidentified subscriber
    CMS_CM_FACILITY_REJECT = 29,        // Facility rejected
    CMS_CM_RES_STATUS_ENQ = 30,         // Unknown subscriber
    CMS_CM_NETWORK_OUT_OF_ORDER = 38,   // Network out of order
    CMS_CM_REQ_FAC_NOT_SUBS = 50,       // Requested facility not subscribed
    CMS_CM_REQ_FACILITY_UNAVAIL = 69,   // Requested facility not implemented
    CMS_CM_INVALID_TI_VALUE = 81,       // Invalid short message transfer reference value
    CMS_CM_SEMANTIC_ERR = 95,           // Semantically incorrect message
    CMS_CM_MSG_TYPE_UNIMPL = 97,        // Message type non-existent or not implemented
    CMS_CM_IE_NON_EX = 99,              // Information element non-existent or not implemented
    CMS_CM_SMS_CONNECTION_BROKEN = 226,
    CMS_CM_SIM_IS_FULL = 322,
    CMS_UNKNOWN = 500,
    SMS_MO_SMS_NOT_ALLOW = 529,
    CMS_CM_MM_CAUSE_START = 2048,
    CMS_CM_MM_IMSI_UNKNOWN_IN_HLR = 0x02 + CMS_CM_MM_CAUSE_START,
    CMS_CM_MM_ILLEGAL_MS = 0x03 + CMS_CM_MM_CAUSE_START,
    CMS_CM_MM_ILLEGAL_ME = 0x06 + CMS_CM_MM_CAUSE_START,
    CMS_CM_RR_PLMN_SRCH_REJ_EMERGENCY = 0x74 + CMS_CM_MM_CAUSE_START,
    CMS_CM_MM_AUTH_FAILURE = 0x76 + CMS_CM_MM_CAUSE_START,
    CMS_CM_MM_IMSI_DETACH = 0x77 + CMS_CM_MM_CAUSE_START,
    CMS_CM_MM_EMERGENCY_NOT_ALLOWED = 0x7d + CMS_CM_MM_CAUSE_START,
    CMS_CM_MM_ACCESS_CLASS_BARRED = 0x7f + CMS_CM_MM_CAUSE_START,
    CMS_MTK_FDN_CHECK_FAILURE = 2601,
    CMS_MTK_REQ_RETRY = 6145,
} AT_CMS_ERROR;

typedef enum {
    ERR_ACK_NONE,
    ERR_ACK_CMT_ACK_ONLY,
    ERR_ACK_CMT_ACK_RESET,
    ERR_ACK_CDS_ACK_ONLY,
    ERR_ACK_CDS_ACK_RESET,
} NEW_SMS_ERR_ACK_TYPE;

typedef enum {
    GSM_CB_QUERY_CHANNEL = 0,
    GSM_CB_QUERY_LANGUAGE,

    GSM_CB_QUERY_END
} GSM_CB_QUERY_MODE;

typedef struct RIL_SMS_GSM_CB_CHANNEL_NODE {
    int start;
    int end;
    struct RIL_SMS_GSM_CB_CHANNEL_NODE* pNext;
} RIL_SMS_GSM_CB_CHANNEL_LIST;

#endif /* RMC_GSM_SMS_DEFS_H */

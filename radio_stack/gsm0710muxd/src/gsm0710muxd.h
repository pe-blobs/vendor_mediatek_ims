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

#ifndef __GSM0710MUXD_H
#define __GSM0710MUXD_H

#if defined(ANDROID_SIM_COUNT_2)
#define SIM_COUNT 2
#elif defined(ANDROID_SIM_COUNT_3)
#define SIM_COUNT 3
#elif defined(ANDROID_SIM_COUNT_4)
#define SIM_COUNT 4
#else
#define SIM_COUNT 1
#endif

/******************************************************************************/

#define GSM0710_WRITE_RETRIES 5
// Defines how often the modem is polled when automatic restarting is
// enabled The value is in seconds
#define GSM0710_POLLING_INTERVAL 5
/* Note by LS: GSM0710_BUFFER_SIZE must be larger than MAX N1 value of all channnels */
/* In this way, assemble_frame_thread can extract a complete MUX frame from the serial buffer then
 * signal the thread_serial_device_read() to continue put data into serial buffer again */
#define GSM0710_BUFFER_SIZE 4096

/******************************************************************************/

#define MUXD_PTY_READ_ROUND 4

#define MUXD_MALLOC_RETRY 3

/* Add by LS: Define the solution how to handle the partial write problem */
#ifndef RX_FLOW_CTRL_M1
#define RX_FLOW_CTRL_M1
#endif
#define RX_FLOW_CTRL_HIGH_WATERMARK 4096

/******************************************************************************/

#ifdef MUX_ANDROID
#define MUXD_CH_NUM_CCH 1
#if defined(MTK_MUX_CHANNEL_64)
#define MUXD_CH_NUM_RILD 10
#else
#define MUXD_CH_NUM_RILD 5
#endif
#define MUXD_CH_NUM_PDP 0
#define MUXD_CH_ATCI 1
#define MUXD_CH_NUM_CSD 1

#ifdef __ANDROID_VT_SUPPORT__
#define MUXD_CH_NUM_VT 1
#else
#define MUXD_CH_NUM_VT 0
#endif

#define MUXD_VT_CH_NUM 24
#define MUXD_CSD_CH_NUM 30

#if defined(__CCMNI_SUPPORT__) && defined(__MUX_UT__)
#define MUXD_CH_NUM_TEST 3
#else
#define MUXD_CH_NUM_TEST 0
#endif

#ifdef FUSION_RILD
#define MUXD_CH_NUM_NW 2
#else
#define MUXD_CH_NUM_NW 0
#endif

#endif /*MUX_ANDROID*/

/******************************************************************************/

#ifdef __MUX_UT__
/* MAX_NON_GEMINI_NON_DATA_CHNL_NUM is only used in UT mode */
#define MAX_NON_GEMINI_NON_DATA_CHNL_NUM 5

/* Channel Number is the DLCI value filled in the MUX Frame, not the index values used for s_path[]
 */
#define DEFAULT_MUX_UT_LB_TEST_CHNL_NUM 6
#define DEFAULT_MUX_UT_TX_TEST_CHNL_NUM 7
#define DEFAULT_MUX_UT_RX_TEST_CHNL_NUM 8
#endif

/******************************************************************************/

#ifndef MUX_ANDROID
#include <syslog.h>
//#define LOG(lvl, f, ...) do{if(lvl<=syslog_level)syslog(lvl,"%s:%d:%s(): " f "\n", __FILE__,
//__LINE__, __FUNCTION__, ##__VA_ARGS__);}while(0)
#define LOGMUX(lvl, f, ...)                                                                     \
    do {                                                                                        \
        if (lvl <= syslog_level) {                                                              \
            if (logtofile) {                                                                    \
                fprintf(muxlogfile, "%d:%s(): " f "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__); \
                fflush(muxlogfile);                                                             \
            } else                                                                              \
                fprintf(stderr, "%d:%s(): " f "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__);     \
        }                                                                                       \
    } while (0)
#else  // will enable logging using android logging framework (not to file)

#define LOG_TAG "RILMUXD"

#include "mtk_log.h"  //all Android LOG macros are defined here.

// just dummy defines since were not including syslog.h.
#define LOG_EMERG 0
#define LOG_ALERT 1
#define LOG_CRIT 2
#define LOG_ERR 3
#define LOG_WARNING 4
#define LOG_NOTICE 5
#define LOG_INFO 6
#define LOG_DEBUG 7
#define LOG_VERBOSE 8

#define LOGMUX(lvl, f, ...)                                                             \
    {                                                                                   \
        switch (lvl) {                                                                  \
            case LOG_WARNING:                                                           \
            case LOG_ERR:                                                               \
                mtkLogE(LOG_TAG, "%d:%s(): " f, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
                break;                                                                  \
            case LOG_INFO:                                                              \
                mtkLogI(LOG_TAG, "%d:%s(): " f, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
                break;                                                                  \
            case LOG_VERBOSE:                                                           \
                mtkLogV(LOG_TAG, "%d:%s(): " f, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
                break;                                                                  \
            default:                                                                    \
            case LOG_DEBUG:                                                             \
                mtkLogD(LOG_TAG, "%d:%s(): " f, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
                break;                                                                  \
        }                                                                               \
    }

#endif /*MUX_ANDROID*/

/******************************************************************************/

#define SYSCHECK(c)                                                                   \
    do {                                                                              \
        if ((c) < 0) {                                                                \
            LOGMUX(LOG_ERR, "system-error: '%s' (code: %d)", strerror(errno), errno); \
            return -1;                                                                \
        }                                                                             \
    } while (0)
#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

/******************************************************************************/

/* increases buffer pointer by one and wraps around if necessary */
// void gsm0710_buffer_inc(GSM0710_Buffer *buf, void&* p);
/* This macro gsm0710_buffer_inc is used in gsm0710_base_buffer_get_frame(): buf is defined, not
 * necessary to pass the buf to this macro */
#define gsm0710_buffer_inc(readp, datacount)       \
    do {                                           \
        readp++;                                   \
        datacount--;                               \
        if (readp == buf->endp) readp = buf->data; \
    } while (0)

/* Tells how many chars are saved into the buffer. */
// int gsm0710_buffer_length(GSM0710_Buffer *buf);
//#define gsm0710_buffer_length(buf) ((buf->readp > buf->writep) ? (GSM0710_BUFFER_SIZE -
//(buf->readp - buf->writep)) : (buf->writep-buf->readp))
#define gsm0710_buffer_length(buf) (buf->datacount)

/* tells how much free space there is in the buffer */
// int gsm0710_buffer_free(GSM0710_Buffer *buf);
//#define gsm0710_buffer_free(buf) ((buf->readp > buf->writep) ? ((buf->readp - buf->writep)-1) :
//(GSM0710_BUFFER_SIZE - (buf->writep-buf->readp))-1)

/* Note by LS: Why does it use the buf->datacount to calculate the free space directly? */
/* Because only after the data pointed by the buf->readp is checked, the gsm0710_buffer_inc is
 * allowed to be used to decrease the value of buf->datacount */
/* In this way, the available free space is less than or equal to the actual free space size due to
 * un-updated buf->readp and buf->datacount */

#define gsm0710_buffer_free(buf) (GSM0710_BUFFER_SIZE - buf->datacount)

/******************************************************************************/

/* Add by LS to test by local define compile option */
#ifndef __PRODUCTION_RELEASE__
// for dump ccci information change log assert to aee API
#define Gsm0710Muxd_Assert(index)                         \
    {                                                     \
        LOGMUX(LOG_ERR, "ASSERT : ERROR_CODE=%d", index); \
        mtkAssert("ASSERT!!!!");                          \
    }
#else
static int g_set_force_assert_flag = 0;
static int g_set_alarm_flag = 0;
#define Gsm0710Muxd_Assert(index)                         \
    {                                                     \
        LOGMUX(LOG_ERR, "ASSERT : ERROR_CODE=%d", index); \
        g_set_force_assert_flag = 1;                      \
        set_main_exit_signal(SIGUSR2);                    \
    }
#endif /* __PRODUCTION_RELEASE__ */

/******************************************************************************/

#define BAUDRATE B460800

/*MUX defines */
#define GSM0710_FRAME_FLAG 0xF9           // basic mode flag for frame start and end
#define GSM0710_FRAME_ADV_FLAG 0x7E       // advanced mode flag for frame start and end
#define GSM0710_FRAME_ADV_ESC 0x7D        // advanced mode escape symbol
#define GSM0710_FRAME_ADV_ESC_COPML 0x20  // advanced mode escape complement mask
#define GSM0710_FRAME_ADV_ESCAPED_SYMS                                        \
    {                                                                         \
        GSM0710_FRAME_ADV_FLAG, GSM0710_FRAME_ADV_ESC, 0x11, 0x91, 0x13, 0x93 \
    }  // advanced mode escaped symbols: Flag, Escape, XON and XOFF
// bits: Poll/final, Command/Response, Extension
#define GSM0710_PF 0x10  // 16
#define GSM0710_CR 0x02  // 2
#define GSM0710_EA 0x01  // 1
// type of frames (i.e., Frame Structure -> Control Field)
#define GSM0710_TYPE_SABM 0x2F  // 47 Set Asynchronous Balanced Mode
#define GSM0710_TYPE_UA 0x63    // 99 Unnumbered Acknowledgement
#define GSM0710_TYPE_DM 0x0F    // 15 Disconnected Mode
#define GSM0710_TYPE_DISC 0x43  // 67 Disconnect
#define GSM0710_TYPE_UIH 0xEF   // 239 Unnumbered information with header check
#define GSM0710_TYPE_UI 0x03    // 3 Unnumbered Acknowledgement
// control channel commands (i.e., Frame Structure -> Information Field -> type filed: control
// channel commands)
#define GSM0710_CONTROL_PN (0x80 | GSM0710_EA)    // ?? DLC parameter negotiation
#define GSM0710_CONTROL_CLD (0xC0 | GSM0710_EA)   // 193 Multiplexer close down
#define GSM0710_CONTROL_PSC (0x40 | GSM0710_EA)   // ??? Power Saving Control
#define GSM0710_CONTROL_TEST (0x20 | GSM0710_EA)  // 33 Test Command
#define GSM0710_CONTROL_MSC (0xE0 | GSM0710_EA)   // 225 Modem Status Command
#define GSM0710_CONTROL_NSC (0x10 | GSM0710_EA)   // 17 Non Supported Command Response
#define GSM0710_CONTROL_RPN (0x90 | GSM0710_EA)   // ?? Remote Port Negotiation Command
#define GSM0710_CONTROL_RLS (0x50 | GSM0710_EA)   // ?? Remote Line Status Command
#define GSM0710_CONTROL_SNC (0xD0 | GSM0710_EA)   // ?? Service Negotiation Command
// V.24 signals: flow control, ready to communicate, ring indicator,
// data valid three last ones are not supported by Siemens TC_3x
#define GSM0710_SIGNAL_FC 0x02
#define GSM0710_SIGNAL_RTC 0x04
#define GSM0710_SIGNAL_RTR 0x08
#define GSM0710_SIGNAL_IC 0x40  // 64
#define GSM0710_SIGNAL_DV 0x80  // 128
#define GSM0710_SIGNAL_DTR 0x04
#define GSM0710_SIGNAL_DSR 0x04
#define GSM0710_SIGNAL_RTS 0x08
#define GSM0710_SIGNAL_CTS 0x08
#define GSM0710_SIGNAL_DCD 0x80  // 128

/* Add by LS: Actually, the value N_TTY_BUF_SIZE is defined as 4096 in tty.h */
#define PTY_CHNL_BUF_SIZE 4096
/* Add by LS: ACK_T1_TIMEOUT is used the "second" as the basic unit */
#define ACK_T1_TIMEOUT 2

#define MAX_RESYNC_GET_FRAME_COUNT 3

/******************************************************************************/
/* TYPES */
/******************************************************************************/

typedef enum MSC_FC_CMD_STATE { FC_NONE = 0, FC_OFF_SENDING, FC_ON_SENDING } MSC_FC_CMD_STATE;

typedef enum MuxerStates {
    MUX_STATE_OPENING,
    MUX_STATE_INITILIZING,
    /* Add by LS for PN negotiaiton procedure */
    MUX_STATE_PARM_NEG,
    MUX_STATE_SABM_CHNL,
    MUX_STATE_MUXING,
    MUX_STATE_CLOSING,
    MUX_STATE_PEER_CLOSING,
    MUX_STATE_OFF,
    MUX_STATES_COUNT  // keep this the last
} MuxerStates;

/* Add by LS */
typedef enum SETUP_PTY_CHNL_TYPE {
    SETUP_PTY_CHNL_WO_RESTART = 0,
    SETUP_PTY_CHNL_W_RESTART
} SETUP_PTY_CHNL_TYPE;

/* Add by LS */
typedef enum SHUTDOWN_DEV_TYPE {
    SHUTDOWN_DEV_WO_ACTIVE_FINALIZED = 0,
    SHUTDOWN_DEV_W_ACTIVE_FINALIZED
} SHUTDOWN_DEV_TYPE;

/* Add by LS: Define the Gsm0710Muxd General Error Code */
typedef enum GSM0710MUXD_GENERAL_ERR_CODE {
    GSM0710MUXD_SUCCESS = 0,
    GSM0710MUXD_EXCEED_SUPPORTED_VP_NUM,
    GSM0710MUXD_CREATE_THREAD_ERR,
    GSM0710MUXD_PTY_READ_ERR,
    GSM0710MUXD_PTY_WRITE_ERR,
    GSM0710MUXD_GET_CWD_ERR,
    GSM0710MUXD_OPEN_SERIAL_DEV_ERR,
    GSM0710MUXD_EXCEED_SUPPORTED_MAX_CHNL_NUM_ERR,
    GSM0710MUXD_START_MUXER_ERR,
    GSM0710MUXD_UNKNOWN_ERR,
    GSM0710MUXD_CHANNEL_CONFIG_ERR,
    GSM0710MUXD_ALLOC_CHANNEL_ERR,
    GSM0710MUXD_FRAMELIST_INIT_ERR,
    GSM0710MUXD_RXTHREAD_ERR,
    GSM0710MUXD_TXTHREAD_ERR,
    GSM0710MUXD_SERIAL_READ_ERR,
    GSM0710MUXD_SERIAL_WRITE_ERR,
    GSM0710MUXD_SETUP_PTY_ERR,
    GSM0710MUXD_RESTART_PTY_ERR,
    GSM0710MUXD_TXTHREAD_SELECT_ERR,
    GSM0710MUXD_OUT_OF_MEMORY,
    GSM0710MUXD_ERR_CODE_COUNT
} GSM0710MUXD_GENERAL_ERR_CODE;

/******************************************************************************/

typedef struct GSM0710_Frame {
    unsigned char channel;
    unsigned char control;
    int length;
    unsigned char* data;

} GSM0710_Frame;

typedef struct GSM0710_FrameList {
    GSM0710_Frame* frame;
    struct GSM0710_FrameList* next;

} GSM0710_FrameList;

/* Size of struct GSM0710_Buffer is larger than GSM0710_BUFFER_SIZE */
/* Note by LS: basic mode and advanced mode should be exclusive:
 * but adv_data[] is copied from the data[] one by one byte in the gsm0710_advanced_buffer_get_frame
 */
typedef struct GSM0710_Buffer {
    unsigned char data[GSM0710_BUFFER_SIZE];
    unsigned char* readp;
    unsigned char* writep;
    unsigned char* endp;
    unsigned int datacount;
    int newdataready;   /*newdataready = 1: new data written to internal buffer. newdataready=0:
                           acknowledged by assembly thread*/
    int input_sleeping; /*input_sleeping = 1 if ser_read_thread (input to buffer) is waiting because
                           buffer is full */
    int flag_found;     // set if last character read was flag
    unsigned long received_count;
    unsigned long dropped_count;
    unsigned char adv_data[GSM0710_BUFFER_SIZE];
    unsigned int adv_length;
    int adv_found_esc;

    // mutex
    pthread_mutex_t datacount_lock;
    pthread_mutex_t newdataready_lock;
    pthread_mutex_t bufferready_lock;

    // signal
    pthread_cond_t newdataready_signal;
    pthread_cond_t bufferready_signal;

} GSM0710_Buffer;

/* Add by LS */
typedef struct Gsm0710MuxdFrameEntry {
    unsigned char* frame_data;
    int frame_length;
    struct Gsm0710MuxdFrameEntry* next;

} Gsm0710MuxdFrameEntry;

typedef struct Channel  // Channel data
{
    int id;  // gsm 0710 channel id

    int fd;
    char* devicename;
    char* ptsname;
    char* ptslink;

    unsigned char opened : 1, reopen : 1;
    unsigned char sabm_ua_pending : 1, disc_ua_pending : 1;

#if 0
    char* origin;
    int remaining;
    unsigned char *tmp;
#endif

    int negotiated_N1;
    unsigned char v24_signals;
    pthread_mutex_t reopen_mutex;
#ifdef __MUXD_FLOWCONTROL__
    /* For TX flow control usage of each non-control channel */
    unsigned char tx_fc_off : 1, rx_fc_off : 1, rx_thread : 1;
    pthread_mutex_t tx_fc_lock;
    pthread_cond_t tx_fc_on_signal;

    pthread_mutex_t rx_fc_lock;
    pthread_cond_t rx_fc_on_signal;

    GSM0710_FrameList* rx_fl;
    unsigned int rx_fl_total;
    unsigned int rx_fl_written;

    // Thread
    pthread_t push_thread_id;
#endif /* __MUXD_FLOWCONTROL__ */
    pthread_t poll_thread_id;

} Channel;

typedef struct Serial {
    char* devicename;
    int fd;
    MuxerStates state;
    GSM0710_Buffer* in_buf;  // input buffer
    GSM0710_Buffer* in_buf_for_bootup;
#ifdef __MUX_UT__
    GSM0710_Buffer* ut_in_buf;  // ut input buffer
#endif

    unsigned char* adv_frame_buf;
    time_t frame_receive_time;
    int ping_number;
#if (defined(MUX_ANDROID) && !defined(REVERSE_MTK_CHANGE))
    int is_socket;
#endif
} Serial;

/* Struct is used for passing fd, read function and read funtion arg to a device polling thread */
typedef struct Poll_Thread_Arg {
    int fd;
    int (*read_function_ptr)(void*);
    void* read_function_arg;
} Poll_Thread_Arg;

typedef struct Channel_Config {
    int pn_dlci;
    int pn_n1;

    char* s_path;

} Channel_Config;

/******************************************************************************/

extern Serial serial;
extern Channel channellist[];

extern int syslog_level;
extern int android_log_lvl_convert[];
extern FILE* muxlogfile;

extern unsigned char close_channel_cmd[];
extern unsigned char test_channel_cmd[];
extern unsigned char msc_channel_cmd[];

/******************************************************************************/
#endif /* __GSM0710MUXD_H */

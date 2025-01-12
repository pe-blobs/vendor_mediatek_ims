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

#ifndef __LIMITER_EXP_H__
#define __LIMITER_EXP_H__

// Notice: All the buffer pointers are required to be
//         four-byte-aligned to avoid the potential on-target process error !!!

#define LIMITER_FRAME_SAMPLE_COUNT 64
// Notice: Limiter SWIP frame size is 64-sample (one pair of L / R counts as one sample),
//         If input / output PCM sample count is not multiple of 64,
//         driver needs to take care of the residual PCM samples !!!

typedef enum {
    LMTR_IN_Q33P31_OUT_Q1P31 = 0,     // 64-bit Q33.31 input, 32-bit Q1.31 output
    LMTR_IN_FLOAT32_OUT_FLOAT32 = 1,  // 32-bit floating input, 32-bit floating output
    LMTR_IN_Q5P27_OUT_Q5P27 = 2       // 32-bit Q5.27 input, 32-bit Q5.27 output
} LMTR_PCM_FORMAT;

typedef enum {
    LMTR_TO_NORMAL_STATE = 0,    // Change to normal state
    LMTR_TO_BYPASS_STATE = 1,    // Change to bypass state
    LMTR_CHANGE_PCM_FORMAT = 2,  // Change PCM format
    LMTR_RESET = 3               // Clear history buffer
} LMTR_COMMAND;

typedef enum {
    LMTR_NORMAL_STATE = 0,  // Normal state
    LMTR_BYPASS_STATE = 1   // Bypass state
} LMTR_STATE;

typedef void Limiter_Handle;

/**************************************************************************************\
|   STRUCTURE                                                                          |
|                                                                                      |
|       Limiter_InitParam                                                              |
|                                                                                      |
|   MEMBERS                                                                            |
|                                                                                      |
|   Channel             Channel number, only support: 1 or 2                           |
|                                                                                      |
|   Sampling_Rate       Input signal sampling rate, unit: Hz                           |
|                                                                                      |
|   PCM_Format          Input / output PCM format                                      |
|                       0 (LMTR_IN_Q33P31_OUT_Q1P31):                                  |
|                           64-bit Q33.31 input, 32-bit Q1.31 output                   |
|                       1 (LMTR_IN_FLOAT32_OUT_FLOAT32):                               |
|                           32-bit floating input, 32-bit floating output              |
|                       2 (LMTR_IN_Q5P27_OUT_Q5P27):                                   |
|                           32-bit Q5.27 input, 32-bit Q5.27 output                    |
|                                                                                      |
|   State               Initial engine state                                           |
|                       0 (LMTR_NORMAL_STATE):                                         |
|                           Set the engine to normal state.                            |
|                       1 (LMTR_BYPASS_STATE):                                         |
|                           Set the engine to bypass state.                            |
|                                                                                      |
\**************************************************************************************/
typedef struct {
    unsigned int Channel;
    unsigned int Sampling_Rate;
    unsigned int PCM_Format;
    unsigned int State;
} Limiter_InitParam;

/**************************************************************************************\
|   STRUCTURE                                                                          |
|                                                                                      |
|       Limiter_RuntimeParam                                                           |
|                                                                                      |
|   MEMBERS                                                                            |
|                                                                                      |
|   Command             Runtime process command.                                       |
|                       0 (LMTR_TO_NORMAL_STATE):                                      |
|                           [Bypass state]                                             |
|                               Enter the "normal state".                              |
|                           [Other state]                                              |
|                               Ignore this command.                                   |
|                       1 (LMTR_TO_BYPASS_STATE):                                      |
|                           [Normal state]                                             |
|                               Enter the "bypass state".                              |
|                           [Other state]                                              |
|                               Ignore this command.                                   |
|                       2 (LMTR_CHANGE_PCM_FORMAT):                                    |
|                           [Any state]                                                |
|                               Change the history buffer PCM format in the next       |
|                               Limiter_Process.                                       |
|                       3 (LMTR_RESET):                                                |
|                           [Any state]                                                |
|                               Clear the history buffer directly.                     |
|                           Note: This command can be used to replace Limiter_Reset.   |
|                                                                                      |
|   PCM_Format          Input / output PCM format, only valid for the                  |
|                       LMTR_CHANGE_PCM_FORMAT command                                 |
|                       0 (LMTR_IN_Q33P31_OUT_Q1P31):                                  |
|                           64-bit Q33.31 input, 32-bit Q1.31 output                   |
|                       1 (LMTR_IN_FLOAT32_OUT_FLOAT32):                               |
|                           32-bit floating input, 32-bit floating output              |
|                       2 (LMTR_IN_Q5P27_OUT_Q5P27):                                   |
|                           32-bit Q5.27 input, 32-bit Q5.27 output                    |
|                                                                                      |
\**************************************************************************************/
typedef struct {
    unsigned int Command;
    unsigned int PCM_Format;
} Limiter_RuntimeParam;

/**************************************************************************************\
|   STRUCTURE                                                                          |
|                                                                                      |
|       Limiter_RuntimeStatus                                                          |
|                                                                                      |
|   MEMBERS                                                                            |
|                                                                                      |
|   PCM_Format          Input / output PCM format                                      |
|                       0 (LMTR_IN_Q33P31_OUT_Q1P31):                                  |
|                           64-bit Q33.31 input, 32-bit Q1.31 output                   |
|                       1 (LMTR_IN_FLOAT32_OUT_FLOAT32):                               |
|                           32-bit floating input, 32-bit floating output              |
|                       2 (LMTR_IN_Q5P27_OUT_Q5P27):                                   |
|                           32-bit Q5.27 input, 32-bit Q5.27 output                    |
|                                                                                      |
|   State               Current engine state                                           |
|                       0 (LMTR_NORMAL_STATE):                                         |
|                           The engine is in normal state.                             |
|                       1 (LMTR_BYPASS_STATE):                                         |
|                           The engine is in bypass mode.                              |
|                                                                                      |
\**************************************************************************************/

typedef struct {
    unsigned int PCM_Format;
    unsigned int State;
} Limiter_RuntimeStatus;

/*
    Get the required buffer sizes of limiter
    Return value                    < 0  : Error
                                    >= 0 : Normal
    p_internal_buf_size_in_byte [O] Required internal buffer size in bytes
    p_temp_buf_size_in_byte     [O] Required temp buffer size in bytes
    PCM_Format                  [I] Input / output PCM format
*/
int Limiter_GetBufferSize(unsigned int* p_internal_buf_size_in_byte,
                          unsigned int* p_temp_buf_size_in_byte, unsigned int PCM_Format);

/*
    Initialize the limiter
    Return value        < 0  : Error
                        >= 0 : Normal
    pp_handle       [I] Pointer to the handle of the limiter
                    [O] Assign the handle pointer from p_internal_buf
    p_internal_buf  [I] Pointer to the internal buffer
    p_init_param    [I] Pointer to the initial parameters
*/
int Limiter_Open(Limiter_Handle** pp_handle, void* p_internal_buf, Limiter_InitParam* p_init_param);

/*
    Process 16-bit / 32-bit / 64-bit data from input buffer to output buffer
    Return value        < 0  : Error
                        >= 0 : Normal
    p_handle        [I] Handle of the limiter
    p_temp_buf      [I] Pointer to the temp buffer used in processing the data
    p_in_buf        [I] Pointer to the input PCM buffer
                        For stereo input, the layout of LR is L/R/L/R...
    p_in_byte_cnt   [I] Valid input buffer size in bytes
                    [O] Consumed input buffer size in bytes
    p_ou_buf        [I] Pointer to the output PCM buffer
                        For stereo output, the layout of LR is L/R/L/R...
    p_ou_byte_cnt   [I] Available output buffer size in bytes
                    [O] Produced output buffer size in bytes
*/
int Limiter_Process(Limiter_Handle* p_handle, char* p_temp_buf, void* p_in_buf,
                    unsigned int* p_in_byte_cnt, void* p_ou_buf, unsigned int* p_ou_byte_cnt);

/*
    Set the runtime parameters
    Return value            < 0  : Error
                            >= 0 : Normal
    p_handle            [I] Handle of the limiter
    p_runtime_param     [I] Pointer to the runtime parameters
*/
int Limiter_SetParameters(Limiter_Handle* p_handle, Limiter_RuntimeParam* p_runtime_param);

/*
    Get the runtime status
    Return value            < 0  : Error
                            >= 0 : Normal
    p_handle            [I] Handle of the limiter
    p_runtime_status    [O] Pointer to the runtime status
*/
int Limiter_GetStatus(Limiter_Handle* p_handle, Limiter_RuntimeStatus* p_runtime_status);

/*
    Clear the internal status for the discontinuous input buffer
    (such as change output device)
    Return value        < 0  : Error
                        >= 0 : Normal
    p_handle        [I] Handle of the limiter
*/
int Limiter_Reset(Limiter_Handle* p_handle);

/*
                      +------- Main Feature Version
                      |+------ Sub Feature Version
                      ||+----- Performance Improvement Version
                      |||
    LIMITER_VERSION 0xABC
                      |||
            +---------+||
            | +--------+|
            | | +-------+
            | | |
    Version 1.0.0: (Scholar Chang)
        First release
    Version 1.1.0: (Scholar Chang)
        Support PCM format "LMTR_IN_FLOAT32_OUT_FLOAT32".
    Version 1.2.0: (Scholar Chang)
        Support PCM format "LMTR_IN_Q5P27_OUT_Q5P27".
        Support "Limiter_SetParameters" for the following runtime usages:
        (1) Change PCM format.
        (2) Switch between normal mode / bypass mode.
        (3) Reset (Same purpose as "Limiter_Reset").
        Support "Limiter_GetStatus" to check the engine runtime status.
*/
int Limiter_GetVersion(void);

#endif  // __LIMITER_EXP_H__

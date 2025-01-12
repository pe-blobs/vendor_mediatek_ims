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

#ifndef VCODECCAP_H
#define VCODECCAP_H

#include <utils/KeyedVector.h>
#include <utils/Vector.h>

using namespace android;

typedef struct sensor_level_info {
    video_level_t level;
    uint32_t width;
    uint32_t height;
} sensor_level_info_t;

typedef struct vcodec_level_property_value {
    uint32_t max_mbps;
    uint32_t max_smbps;
    uint32_t max_fs;
    uint32_t max_cpb;
    uint32_t max_dpb;
    uint32_t max_br;
} vcodec_level_property_value_t;

typedef struct _vdec_cap {
    video_profile_t profile;
    video_level_t max_level;

    _vdec_cap() {
        profile = VIDEO_PROFILE_UNKNOWN;
        max_level = VIDEO_LEVEL_UNKNOWN;
    }
} vdec_cap_t;

typedef struct _venc_cap {
    video_profile_t profile;
    video_level_t max_level;

    _venc_cap() {
        profile = VIDEO_PROFILE_UNKNOWN;
        max_level = VIDEO_LEVEL_UNKNOWN;
    }
} venc_cap_t;

typedef struct _video_default_cap {
    video_profile_t profile;
    video_level_t level;

    _video_default_cap() {
        profile = VIDEO_PROFILE_UNKNOWN;
        level = VIDEO_LEVEL_UNKNOWN;
    }
} video_default_cap_t;

typedef struct _video_property {
    video_format_t format;
    video_profile_t profile;
    video_level_t level;
    uint32_t fps;
    uint32_t bitrate;  // in kbps
    uint32_t Iinterval;

    _video_property() {
        format = VIDEO_H264;
        profile = VIDEO_PROFILE_UNKNOWN;
        level = VIDEO_LEVEL_UNKNOWN;
        fps = 0;
        bitrate = 0;
        Iinterval = 0;
    }
} video_property_t;

typedef struct QualityInfo {
    video_quality_t quality;
    video_format_t format;
    video_profile_t profile;
    video_level_t level;
} QualityInfo_t;

typedef struct MediaProfileList {
    int mOPID;
    video_quality_t mDefault_quality;
    Vector<video_format_t> mVideoFormatList;
    Vector<QualityInfo_t> mQualityList[VIDEO_FORMAT_NUM];
    Vector<video_media_profile_t*> mMediaProfile[VIDEO_FORMAT_NUM];
    KeyedVector<video_quality_t, video_media_profile_t> mQualityMediaProfile[VIDEO_FORMAT_NUM];
} MediaProfileList_t;

/**
 * @par Enumeration
 *   VENC_DRV_MRESULT_T
 * @par Description
 *   This is the return value for eVEncDrvXXX()
 */
typedef enum __VENC_DRV_MRESULT_T {
    VENC_DRV_MRESULT_OK = 0,          /* /< Return Success */
    VENC_DRV_MRESULT_FAIL,            /* /< Return Fail */
    VENC_DRV_MRESULT_MAX = 0x0FFFFFFF /* /< Max VENC_DRV_MRESULT_T value */
} VENC_DRV_MRESULT_T;

void getDefaultH264MediaProfileByOperator(int opid, video_media_profile_t** ppvideo_media_profile_t,
                                          int* pCount);
void getDefaultHEVCMediaProfileByOperator(int opid, video_media_profile_t** ppvideo_media_profile_t,
                                          int* pCount);
void createH264QualityMediaProfileByOperator(int opid, QualityInfo_t* pQualityInfo);
void createHEVCQualityMediaProfileByOperator(int opid, QualityInfo_t* pQualityInfo);
MediaProfileList_t* getMediaProfileListInst(uint32_t opID);
video_media_profile_t* getMediaProfileEntry(MediaProfileList_t* pMediaProfileList_t,
                                            video_format_t format, video_profile_t profile,
                                            video_level_t level);

char* toString(video_profile_t profile);
char* toString(video_level_t level);
void printBinary(unsigned char* ptrBS, int iLen);
uint32_t getLevel(video_format_t format, video_level_t level);
video_level_t getLevel(video_format_t format, uint32_t level);
uint32_t getProfile(video_format_t format, video_profile_t profile);
video_profile_t getProfile(video_format_t format, uint32_t u4Profile);
char* getOperatorName(int opid);
video_property_t* getVideoProperty();

#endif

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

#include "VTService.h"
#include "VTDef.h"
#include "VTMsgDispatcher.h"

#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <utils/Log.h>
#include <stdint.h>

namespace VTService {

sp<IVTClient> gVTSClient = NULL;
sp<VTCore> VTService::mVTCore = NULL;
bool mRildReady = false;

extern void vt_callback(int type, void* data, int len);
extern vt_srv_cntx_struct g_vt;
extern vt_srv_msg_struct g_vt_srv_msg_hdr;

static int getCallingPid() { return IPCThreadState::self()->getCallingPid(); }

void VTService_instantiate() {
    VT_LOGE("[VTS] before addService");
    usleep(500000);
    sp<VTService> s = new VTService();
    int ret = defaultServiceManager()->addService(String16("media.VTS"), s);
    VT_LOGE("[VTS] after addService %d", ret);
    signal(SIGPIPE, SIG_IGN);

    memset(&g_vt, 0, sizeof(vt_srv_cntx_struct));
    memset(&g_vt_srv_msg_hdr, 0, sizeof(vt_srv_msg_struct));

    VT_Connect(vt_callback);

    VTService::mVTCore = new VTCore(s);
}

void VTService::binderDied(const wp<IBinder>& who) {
    RFX_UNUSED(who);

    if (NULL != g_vt.core.get()) {
        // Do not close socket when binder died. Beacause after phone process
        // crash, will binder die. But no need close socket in this case.
        // VT_Close();

        g_vt.core->clearAll();

        // for IPO case, won't call VTService_instantiate() again
        // so we just need to clear, not free it
        // mVTCore = NULL;
    }
}

status_t VTService::setupVTService(const sp<IVTClient>& client) {
    VT_LOGD("[VTS] setupVTService+");

    sp<IServiceManager> sm = defaultServiceManager();
    do {
        mPhoneBinder = sm->getService(String16("phone"));
        if (mPhoneBinder != 0) {
            VT_LOGD("[VTS] get phone service");
            break;
        }
        VT_LOGD("[VTS] Cannot get phone service");
        usleep(500000);  // 0.5 s
    } while (true);

    mPhoneBinder->linkToDeath(this);

    do {
        mImsBinder = sm->getService(String16("mtkIms"));
        if (mImsBinder != 0) {
            VT_LOGD("[VTS] get ims service");
            break;
        }
        VT_LOGD("[VTS] Cannot ims service");
        usleep(500000);  // 0.5 s
    } while (true);

    mImsBinder->linkToDeath(this);

    gVTSClient = client;

    VT_STATUS_REQ vt_status;
    vt_status.call_id = 0;
    vt_status.sim_slot_id = 0;
    vt_status.status = 1;  // 1:Ready, 0:Not readay, 2: restarted

    int ret = VT_Send(VT_SRV_CALL_4G, MSG_ID_RILD_VTSERVICE_STATUS,
                      reinterpret_cast<void*>(&vt_status), sizeof(VT_STATUS_REQ));

    VT_LOGD("[VTS] setupVTService-");
    return NO_ERROR;
}

void VTService::releaseVTService() {
    gVTSClient = NULL;
    mPhoneBinder->unlinkToDeath(this);
}

status_t VTService::initialization(int mode, int id, int sim_id) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->initialization(mode, id, sim_id);
}

status_t VTService::finalization(int id) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->finalization(id);
}

status_t VTService::setCamera(int id, int cam) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->setCamera(id, cam);
}

status_t VTService::setPreviewSurface(int id, const sp<VTSurface>& surface) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->setPreviewSurface(id, surface);
}

status_t VTService::setDisplaySurface(int id, const sp<VTSurface>& surface) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->setDisplaySurface(id, surface);
}

status_t VTService::setCameraParameters(int sim_id, int sensorCnt, sensor_info_vilte_t* sensor) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->setCameraParameters(sim_id, sensorCnt, sensor);
}

status_t VTService::setCameraParametersOnly(int sensorCnt, sensor_info_vilte_t* sensor) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->setCameraParametersOnly(sensorCnt, sensor);
}

status_t VTService::setCameraParametersWithSim(int sim_id, int major_sim_id, int sensorCnt,
                                               sensor_info_vilte_t* sensor) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->setCameraParametersWithSim(sim_id, major_sim_id, sensorCnt, sensor);
}

status_t VTService::setDeviceOrientation(int id, int rotation) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->setDeviceOrientation(id, rotation);
}

status_t VTService::setUIMode(int id, VT_SRV_UI_MODE mode) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->setUIMode(id, mode);
}

status_t VTService::requestSessionModify(int id, const String8& config) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->requestSessionModify(id, config);
}

status_t VTService::responseSessionModify(int id, const String8& config) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->responseSessionModify(id, config);
}

status_t VTService::snapshot(int id, VT_SRV_SNAPSHOT_TYPE type, String8 savingImgURI) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->snapshot(id, type, savingImgURI);
}

status_t VTService::startRecording(int id, VT_SRV_RECORD_TYPE type, String8 path, int maxSize) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->startRecording(id, type, path, maxSize);
}

status_t VTService::stopRecording(int id) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->stopRecording(id);
}

status_t VTService::switchFeature(int id, int feature, int isOn) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->switchFeature(id, feature, isOn);
}

status_t VTService::updateNetworkTable(bool is_add, int network_id, String8 if_name) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->updateNetworkTable(is_add, network_id, if_name);
}

status_t VTService::triggerGetOperatorId() {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->triggerGetOperatorId();
}

status_t VTService::tagSocketWithUid(int uid) {
    if (g_vt.core.get() == NULL) {
        VT_LOGE("[VTS] g_vt.core == NULL");
        return NO_INIT;
    }

    return g_vt.core->tagSocketWithUid(uid);
}

void VTService::notifyCallback(int32_t id, int32_t msgType, int32_t arg1, int32_t arg2,
                               int32_t arg3, const String8& obj1, const String8& obj2,
                               const sp<IGraphicBufferProducer>& obj3) {
    // Sometimes message notify before gVTSClient connected, we must wait here
    int count = 0;
    if (VT_SRV_NOTIFY_RILD_READY == msgType) {
        if (gVTSClient != NULL) {
            VT_LOGD("[VTS] VTClient connected already, Send vtservice status ready");

            VT_STATUS_REQ vt_status;
            vt_status.call_id = 0;
            vt_status.sim_slot_id = 0;
            vt_status.status = 1;  // 1:Ready, 0:Not readay

            int ret = VT_Send(VT_SRV_CALL_4G, MSG_ID_RILD_VTSERVICE_STATUS,
                              reinterpret_cast<void*>(&vt_status), sizeof(VT_STATUS_REQ));
        } else {
            VT_LOGD("[VTS] VTClient not connect yet");
            mRildReady = true;
        }
        return;
    }
    while (gVTSClient == 0) {
        if ((count % 5) == 0) {
            VT_LOGI("[VTS] gVTSClient == NULL");
        }

        sleep(1);
        count++;
    }

    gVTSClient->notifyCallback(id, msgType, arg1, arg2, arg3, obj1, obj2, obj3);
}

sp<IVTClient> VTService::getClient() {
    sp<IVTClient> client = 0;

    if (mVTClient != 0) {
        client = mVTClient.promote();
        if (client == 0) {
            mVTClient.clear();
        }
    }
    return client;
}

status_t VTService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
    int sensorCnt = 0;
    sensor_info_vilte_t* sensor = NULL;

    switch (code) {
        case SETUP_SERVICE: {
            CHECK_INTERFACE(IVTService, data, reply);
            sp<IVTClient> VTClient = interface_cast<IVTClient>(data.readStrongBinder());
            reply->writeInt32(setupVTService(VTClient));
            return NO_ERROR;
        } break;

        case RELEASE_SERVICE: {
            CHECK_INTERFACE(IVTService, data, reply);
            releaseVTService();
            return NO_ERROR;
        } break;

        case INITIALIZATION: {
            CHECK_INTERFACE(IVTService, data, reply);
            int mode = data.readInt32();
            int id = data.readInt32();
            int sim_id = data.readInt32();
            initialization(mode, id, sim_id);
            return NO_ERROR;
        } break;

        case FINALIZATION: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            finalization(id);
            return NO_ERROR;
        } break;

        case SET_CAMERA: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            int cam = data.readInt32();
            reply->writeInt32(setCamera(id, cam));
            return NO_ERROR;
        } break;

        case SET_LOCAL_SURFACE: {
            CHECK_INTERFACE(IVTService, data, reply);

            /* Id structure
             * 32                              16          9      8              0
             * --------------------------------+--------------------------------+
             * |                               |           |      |             |
             * |                        Sim ID |           |  Set |     Call ID |
             * |                               |           |Suface|             |
             * +--------------------------------+-------------------------------+
             */
            int id = data.readInt32();
            bool bIsSurfaceNotNull = id & 0x100;
            (bIsSurfaceNotNull) ? id = (id & (~0x100)) : id;

            VT_LOGD("[VTS] SET_LOCAL_SURFACE: id = %d", id);
            if (bIsSurfaceNotNull) {
                sp<IGraphicBufferProducer> localGraphicBuffer =
                        interface_cast<IGraphicBufferProducer>(data.readStrongBinder());
                sp<VTSurface> localSurface = new VTSurface(localGraphicBuffer);
                VT_LOGD("[VTS] SET_LOCAL_SURFACE: surface = %p", localSurface.get());
                reply->writeInt32(setPreviewSurface(id, localSurface));
                localSurface = NULL;
                return NO_ERROR;
            } else {
                VT_LOGD("[VTS] SET_LOCAL_SURFACE: surface = NULL");
                reply->writeInt32(setPreviewSurface(id, NULL));
                return NO_ERROR;
            }
        } break;

        case SET_PEER_SURFACE: {
            CHECK_INTERFACE(IVTService, data, reply);

            /* Id structure
             * 32                              16          9      8              0
             * --------------------------------+--------------------------------+
             * |                               |           |      |             |
             * |                        Sim ID |           |  Set |     Call ID |
             * |                               |           |Suface|             |
             * +--------------------------------+-------------------------------+
             */
            int id = data.readInt32();
            bool bIsSurfaceNotNull = id & 0x100;
            (bIsSurfaceNotNull) ? id = (id & (~0x100)) : id;

            VT_LOGD("[VTS] SET_PEER_SURFACE: id = %d", id);
            if (bIsSurfaceNotNull) {
                sp<IGraphicBufferProducer> localGraphicBuffer =
                        interface_cast<IGraphicBufferProducer>(data.readStrongBinder());
                sp<VTSurface> peerSurface = new VTSurface(localGraphicBuffer);
                VT_LOGD("[VTS] SET_PEER_SURFACE: surface = %p", peerSurface.get());
                reply->writeInt32(setDisplaySurface(id, peerSurface));
                peerSurface = NULL;
                return NO_ERROR;
            } else {
                VT_LOGD("[VTS] SET_PEER_SURFACE: surface = NULL");
                reply->writeInt32(setDisplaySurface(id, NULL));
                return NO_ERROR;
            }
        } break;

        case SET_CAMERA_PARAM: {
            CHECK_INTERFACE(IVTService, data, reply);
            int sim_id = data.readInt32();
            sensorCnt = data.readInt32();
            if (sensorCnt > 0 && sensorCnt < VT_MAX_SENSOR_COUNT) {
                sensor = new sensor_info_vilte_t[sensorCnt];
                data.read(sensor, sizeof(sensor_info_vilte_t) * sensorCnt);
            }
            reply->writeInt32(setCameraParameters(sim_id, sensorCnt, sensor));
            if (sensorCnt > 0 && sensorCnt < VT_MAX_SENSOR_COUNT) {
                delete[] sensor;
            }
            return NO_ERROR;
        } break;

        case SET_CAMERA_PARAM_WITH_SIM: {
            CHECK_INTERFACE(IVTService, data, reply);
            int sim_id = data.readInt32();
            int major_sim_id = data.readInt32();
            sensorCnt = data.readInt32();
            if (sensorCnt > 0 && sensorCnt < VT_MAX_SENSOR_COUNT) {
                sensor = new sensor_info_vilte_t[sensorCnt];
                data.read(sensor, sizeof(sensor_info_vilte_t) * sensorCnt);
            }
            reply->writeInt32(setCameraParametersWithSim(sim_id, major_sim_id, sensorCnt, sensor));
            if (sensorCnt > 0 && sensorCnt < VT_MAX_SENSOR_COUNT) {
                delete[] sensor;
            }
            return NO_ERROR;
        } break;

        case SET_CAMERA_PARAM_ONLY: {
            CHECK_INTERFACE(IVTService, data, reply);
            sensorCnt = data.readInt32();
            if (sensorCnt > 0 && sensorCnt < VT_MAX_SENSOR_COUNT) {
                sensor = new sensor_info_vilte_t[sensorCnt];
                data.read(sensor, sizeof(sensor_info_vilte_t) * sensorCnt);
            }
            reply->writeInt32(setCameraParametersOnly(sensorCnt, sensor));
            if (sensorCnt > 0 && sensorCnt < VT_MAX_SENSOR_COUNT) {
                delete[] sensor;
            }
            return NO_ERROR;
        } break;

        case SET_ROTATION: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            int rotation = data.readInt32();
            reply->writeInt32(setDeviceOrientation(id, rotation));
            return NO_ERROR;
        } break;

        case SET_UI_MODE: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            VT_SRV_UI_MODE mode = (VT_SRV_UI_MODE)data.readInt32();
            reply->writeInt32(setUIMode(id, mode));
            return NO_ERROR;
        } break;

        case REQ_SESSION_MODIFY: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            const String8 config(data.readString8());
            reply->writeInt32(requestSessionModify(id, config));
            return NO_ERROR;
        } break;

        case RES_SESSION_MODIFY: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            const String8 config(data.readString8());
            reply->writeInt32(responseSessionModify(id, config));
            return NO_ERROR;
        } break;

        case SNAPSHOT: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            VT_SRV_SNAPSHOT_TYPE type = (VT_SRV_SNAPSHOT_TYPE)data.readInt32();
            String8 savingImgURI(data.readString8());
            reply->writeInt32(snapshot(id, type, savingImgURI));
            return NO_ERROR;
        } break;

        case START_RECORD: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            VT_SRV_RECORD_TYPE type = (VT_SRV_RECORD_TYPE)data.readInt32();
            String8 path(data.readString8());
            int maxSize = data.readInt32();
            reply->writeInt32(startRecording(id, type, path, maxSize));
            return NO_ERROR;
        } break;

        case STOP_RECORD: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            reply->writeInt32(stopRecording(id));
            return NO_ERROR;
        } break;

        case SWITCH_FEATURE: {
            CHECK_INTERFACE(IVTService, data, reply);
            int id = data.readInt32();
            int feature = data.readInt32();
            int isOn = data.readInt32();
            reply->writeInt32(switchFeature(id, feature, isOn));
            return NO_ERROR;
        } break;

        case UPDATE_NETWORK_TABLE: {
            CHECK_INTERFACE(IVTService, data, reply);
            bool is_add = data.readBool();
            int network_id = data.readInt32();
            String8 if_name(data.readString8());
            reply->writeInt32(updateNetworkTable(is_add, network_id, if_name));
            return NO_ERROR;
        } break;

        case TRIGGER_GET_OPERATOR: {
            CHECK_INTERFACE(IVTService, data, reply);
            reply->writeInt32(triggerGetOperatorId());
            return NO_ERROR;
        } break;

        case TAG_SOCKET_WITH_UID: {
            CHECK_INTERFACE(IVTService, data, reply);
            int uid = data.readInt32();
            reply->writeInt32(tagSocketWithUid(uid));
            return NO_ERROR;
        } break;

        default:
            return BnVTService::onTransact(code, data, reply, flags);
    }
}

}  // namespace VTService

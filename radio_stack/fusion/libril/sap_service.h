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

#ifndef SAP_SERVICE_H
#define SAP_SERVICE_H

#include <telephony/mtk_ril.h>
#include <ril_internal.h>
#include <RilSapSocket.h>
#include <vendor/mediatek/ims/radio_stack/platformlib/common/libmtkrilutils/proto/sap-api.pb.h>

namespace sap {

void registerService(const RIL_RadioFunctions* callbacks);
void processResponse(MsgHeader* rsp, RilSapSocket* sapSocket);
void processUnsolResponse(MsgHeader* rsp, RilSapSocket* sapSocket);

}  // namespace sap

#endif  // RIL_SERVICE_H

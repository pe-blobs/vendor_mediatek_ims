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

// System headers required for setgroups, etc.
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <sys/prctl.h>
#include <private/android_filesystem_config.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>

// for system property
#include <cutils/properties.h>

#include "main_vtservice.h"

using namespace android;

int main(void) {
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();

    ALOGI("[VT][SRV]ServiceManager: %p", sm.get());

    ALOGI("[VT][SRV]before VTService_instantiate");
    VTService::VTService_instantiate();
    ALOGI("[VT][SRV]after VTService_instantiate");

    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();
}

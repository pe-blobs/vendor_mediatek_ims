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

#include <stdio.h>
#include <string.h>
#include <cutils/properties.h>
#include <stdlib.h>
#include "ccci_lib_platform.h"

#define AB_PROPERTY_NAME "ro.boot.slot_suffix"

void AB_image_get(char* buf) {
    if (property_get(AB_PROPERTY_NAME, buf, NULL) == 0) buf[0] = 0;
}

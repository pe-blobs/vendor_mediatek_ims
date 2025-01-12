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

#include "ares_setup.h"

#include "ares.h"
#include "ares_library_init.h"
#include "ares_private.h"

/* library-private global and unique instance vars */

#ifdef USE_WINSOCK
fpGetNetworkParams_t ares_fpGetNetworkParams = ZERO_NULL;
fpSystemFunction036_t ares_fpSystemFunction036 = ZERO_NULL;
fpGetAdaptersAddresses_t ares_fpGetAdaptersAddresses = ZERO_NULL;
#endif

/* library-private global vars with source visibility restricted to this file */

static unsigned int ares_initialized;
static int ares_init_flags;

#ifdef USE_WINSOCK
static HMODULE hnd_iphlpapi;
static HMODULE hnd_advapi32;
#endif

static int ares_win32_init(void) {
#ifdef USE_WINSOCK

    hnd_iphlpapi = 0;
    hnd_iphlpapi = LoadLibrary("iphlpapi.dll");
    if (!hnd_iphlpapi) return ARES_ELOADIPHLPAPI;

    ares_fpGetNetworkParams =
            (fpGetNetworkParams_t)GetProcAddress(hnd_iphlpapi, "GetNetworkParams");
    if (!ares_fpGetNetworkParams) {
        FreeLibrary(hnd_iphlpapi);
        return ARES_EADDRGETNETWORKPARAMS;
    }

    ares_fpGetAdaptersAddresses =
            (fpGetAdaptersAddresses_t)GetProcAddress(hnd_iphlpapi, "GetAdaptersAddresses");
    if (!ares_fpGetAdaptersAddresses) {
        /* This can happen on clients before WinXP, I don't
           think it should be an error, unless we don't want to
           support Windows 2000 anymore */
    }

    /*
     * When advapi32.dll is unavailable or advapi32.dll has no SystemFunction036,
     * also known as RtlGenRandom, which is the case for Windows versions prior
     * to WinXP then c-ares uses portable rand() function. Then don't error here.
     */

    hnd_advapi32 = 0;
    hnd_advapi32 = LoadLibrary("advapi32.dll");
    if (hnd_advapi32) {
        ares_fpSystemFunction036 =
                (fpSystemFunction036_t)GetProcAddress(hnd_advapi32, "SystemFunction036");
    }

#endif
    return ARES_SUCCESS;
}

static void ares_win32_cleanup(void) {
#ifdef USE_WINSOCK
    if (hnd_advapi32) FreeLibrary(hnd_advapi32);
    if (hnd_iphlpapi) FreeLibrary(hnd_iphlpapi);
#endif
}

int ares_library_init(int flags) {
    int res;

    if (ares_initialized) return ARES_SUCCESS;
    ares_initialized++;

    if (flags & ARES_LIB_INIT_WIN32) {
        res = ares_win32_init();
        if (res != ARES_SUCCESS) return res;
    }

    ares_init_flags = flags;

    return ARES_SUCCESS;
}

void ares_library_cleanup(void) {
    if (!ares_initialized) return;
    ares_initialized--;

    if (ares_init_flags & ARES_LIB_INIT_WIN32) ares_win32_cleanup();

    ares_init_flags = ARES_LIB_INIT_NONE;
}

int ares_library_initialized(void) {
#ifdef USE_WINSOCK
    if (!ares_initialized) return ARES_ENOTINITIALIZED;
#endif
    return ARES_SUCCESS;
}

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

typedef enum {
    ARES_DATATYPE_UNKNOWN = 1, /* unknown data type     - introduced in 1.7.0 */
    ARES_DATATYPE_SRV_REPLY,   /* struct ares_srv_reply - introduced in 1.7.0 */
    ARES_DATATYPE_TXT_REPLY,   /* struct ares_txt_reply - introduced in 1.7.0 */
    ARES_DATATYPE_ADDR_NODE,   /* struct ares_addr_node - introduced in 1.7.1 */
    ARES_DATATYPE_MX_REPLY,    /* struct ares_mx_reply   - introduced in 1.7.2 */
#if 0
  ARES_DATATYPE_ADDR6TTL,     /* struct ares_addrttl   */
  ARES_DATATYPE_ADDRTTL,      /* struct ares_addr6ttl  */
  ARES_DATATYPE_HOSTENT,      /* struct hostent        */
  ARES_DATATYPE_OPTIONS,      /* struct ares_options   */
#endif
    ARES_DATATYPE_LAST /* not used              - introduced in 1.7.0 */
} ares_datatype;

#define ARES_DATATYPE_MARK 0xbead

/*
 * ares_data struct definition is internal to c-ares and shall not
 * be exposed by the public API in order to allow future changes
 * and extensions to it without breaking ABI.  This will be used
 * internally by c-ares as the container of multiple types of data
 * dynamically allocated for which a reference will be returned
 * to the calling application.
 *
 * c-ares API functions returning a pointer to c-ares internally
 * allocated data will actually be returning an interior pointer
 * into this ares_data struct.
 *
 * All this is 'invisible' to the calling application, the only
 * requirement is that this kind of data must be free'ed by the
 * calling application using ares_free_data() with the pointer
 * it has received from a previous c-ares function call.
 */

struct ares_data {
    ares_datatype type; /* Actual data type identifier. */
    unsigned int mark;  /* Private ares_data signature. */
    union {
        struct ares_txt_reply txt_reply;
        struct ares_srv_reply srv_reply;
        struct ares_addr_node addr_node;
        struct ares_mx_reply mx_reply;
    } data;
};

void* ares_malloc_data(ares_datatype type);

ares_datatype ares_get_datatype(void* dataptr);

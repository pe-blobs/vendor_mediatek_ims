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

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#else
#include "nameser.h"
#endif
#ifdef HAVE_ARPA_NAMESER_COMPAT_H
#include <arpa/nameser_compat.h>
#endif

#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_data.h"
#include "ares_private.h"

int ares_parse_mx_reply(const unsigned char* abuf, int alen, struct ares_mx_reply** mx_out) {
    unsigned int qdcount, ancount, i;
    const unsigned char *aptr, *vptr;
    int status, rr_type, rr_class, rr_len;
    long len;
    char *hostname = NULL, *rr_name = NULL;
    struct ares_mx_reply* mx_head = NULL;
    struct ares_mx_reply* mx_last = NULL;
    struct ares_mx_reply* mx_curr;

    /* Set *mx_out to NULL for all failure cases. */
    *mx_out = NULL;

    /* Give up if abuf doesn't have room for a header. */
    if (alen < HFIXEDSZ) return ARES_EBADRESP;

    /* Fetch the question and answer count from the header. */
    qdcount = DNS_HEADER_QDCOUNT(abuf);
    ancount = DNS_HEADER_ANCOUNT(abuf);
    if (qdcount != 1) return ARES_EBADRESP;
    if (ancount == 0) return ARES_ENODATA;

    /* Expand the name from the question, and skip past the question. */
    aptr = abuf + HFIXEDSZ;
    status = ares_expand_name(aptr, abuf, alen, &hostname, &len);
    if (status != ARES_SUCCESS) return status;

    if (aptr + len + QFIXEDSZ > abuf + alen) {
        free(hostname);
        return ARES_EBADRESP;
    }
    aptr += len + QFIXEDSZ;

    /* Examine each answer resource record (RR) in turn. */
    for (i = 0; i < ancount; i++) {
        /* Decode the RR up to the data field. */
        status = ares_expand_name(aptr, abuf, alen, &rr_name, &len);
        if (status != ARES_SUCCESS) {
            break;
        }
        aptr += len;
        if (aptr + RRFIXEDSZ > abuf + alen) {
            status = ARES_EBADRESP;
            break;
        }
        rr_type = DNS_RR_TYPE(aptr);
        rr_class = DNS_RR_CLASS(aptr);
        rr_len = DNS_RR_LEN(aptr);
        aptr += RRFIXEDSZ;

        /* Check if we are really looking at a MX record */
        if (rr_class == C_IN && rr_type == T_MX) {
            /* parse the MX record itself */
            if (rr_len < 2) {
                status = ARES_EBADRESP;
                break;
            }

            /* Allocate storage for this MX answer appending it to the list */
            mx_curr = ares_malloc_data(ARES_DATATYPE_MX_REPLY);
            if (!mx_curr) {
                status = ARES_ENOMEM;
                break;
            }
            if (mx_last) {
                mx_last->next = mx_curr;
            } else {
                mx_head = mx_curr;
            }
            mx_last = mx_curr;

            vptr = aptr;
            mx_curr->priority = DNS__16BIT(vptr);
            vptr += sizeof(unsigned short);

            status = ares_expand_name(vptr, abuf, alen, &mx_curr->host, &len);
            if (status != ARES_SUCCESS) break;
        }

        /* Don't lose memory in the next iteration */
        free(rr_name);
        rr_name = NULL;

        /* Move on to the next record */
        aptr += rr_len;
    }

    if (hostname) free(hostname);
    if (rr_name) free(rr_name);

    /* clean up on error */
    if (status != ARES_SUCCESS) {
        if (mx_head) ares_free_data(mx_head);
        return status;
    }

    /* everything looks fine, return the data */
    *mx_out = mx_head;

    return ARES_SUCCESS;
}

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
#ifdef HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#else
#include "nameser.h"
#endif
#ifdef HAVE_ARPA_NAMESER_COMPAT_H
#include <arpa/nameser_compat.h>
#endif

#include <stdlib.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_private.h"

struct qquery {
    ares_callback callback;
    void* arg;
};

static void qcallback(void* arg, int status, int timeouts, unsigned char* abuf, int alen);

void ares__rc4(rc4_key* key, unsigned char* buffer_ptr, int buffer_len) {
    unsigned char x;
    unsigned char y;
    unsigned char* state;
    unsigned char xorIndex;
    short counter;

    x = key->x;
    y = key->y;

    state = &key->state[0];
    for (counter = 0; counter < buffer_len; counter++) {
        x = (unsigned char)((x + 1) % 256);
        y = (unsigned char)((state[x] + y) % 256);
        ARES_SWAP_BYTE(&state[x], &state[y]);

        xorIndex = (unsigned char)((state[x] + state[y]) % 256);

        buffer_ptr[counter] = (unsigned char)(buffer_ptr[counter] ^ state[xorIndex]);
    }
    key->x = x;
    key->y = y;
}

static struct query* find_query_by_id(ares_channel channel, unsigned short id) {
    unsigned short qid;
    struct list_node* list_head;
    struct list_node* list_node;
    DNS_HEADER_SET_QID(((unsigned char*)&qid), id);

    /* Find the query corresponding to this packet. */
    list_head = &(channel->queries_by_qid[qid % ARES_QID_TABLE_SIZE]);
    for (list_node = list_head->next; list_node != list_head; list_node = list_node->next) {
        struct query* q = list_node->data;
        if (q->qid == qid) return q;
    }
    return NULL;
}

/* a unique query id is generated using an rc4 key. Since the id may already
   be used by a running query (as infrequent as it may be), a lookup is
   performed per id generation. In practice this search should happen only
   once per newly generated id
*/
static unsigned short generate_unique_id(ares_channel channel) {
    unsigned short id;

    do {
        id = ares__generate_new_id(&channel->id_key);
    } while (find_query_by_id(channel, id));

    return (unsigned short)id;
}

int ares_query(ares_channel channel, const char* name, int dnsclass, int type,
               ares_callback callback, void* arg) {
    struct qquery* qquery;
    unsigned char* qbuf;
    int qlen, rd, status, result;

    /* Compose the query. */
    rd = !(channel->flags & ARES_FLAG_NORECURSE);
    status = ares_mkquery(name, dnsclass, type, channel->next_id, rd, &qbuf, &qlen);
    if (status != ARES_SUCCESS) {
        if (qbuf != NULL) free(qbuf);
        return status;
    }

    channel->next_id = generate_unique_id(channel);

    /* Allocate and fill in the query structure. */
    qquery = malloc(sizeof(struct qquery));
    if (!qquery) {
        ares_free_string(qbuf);
        return ARES_ENOMEM;
    }
    qquery->callback = callback;
    qquery->arg = arg;

    /* Send it off.  qcallback will be called when we get an answer. */
    result = ares_send(channel, qbuf, qlen, qcallback, qquery);
    ares_free_string(qbuf);
    return result;
}

static void qcallback(void* arg, int status, int timeouts, unsigned char* abuf, int alen) {
    struct qquery* qquery = (struct qquery*)arg;
    unsigned int ancount;
    int rcode;

    if (status != ARES_SUCCESS)
        qquery->callback(qquery->arg, status, timeouts, abuf, alen);
    else {
        /* Pull the response code and answer count from the packet. */
        rcode = DNS_HEADER_RCODE(abuf);
        ancount = DNS_HEADER_ANCOUNT(abuf);

        /* Convert errors. */
        switch (rcode) {
            case NOERROR:
                status = (ancount > 0) ? ARES_SUCCESS : ARES_ENODATA;
                break;
            case FORMERR:
                status = ARES_EFORMERR;
                break;
            case SERVFAIL:
                status = ARES_ESERVFAIL;
                break;
            case NXDOMAIN:
                status = ARES_ENOTFOUND;
                break;
            case NOTIMP:
                status = ARES_ENOTIMP;
                break;
            case REFUSED:
                status = ARES_EREFUSED;
                break;
        }
        qquery->callback(qquery->arg, status, timeouts, abuf, alen);
    }
    free(qquery);
}

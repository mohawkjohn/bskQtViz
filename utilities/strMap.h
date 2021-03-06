/*
 ISC License

 Copyright (c) 2016-2017, Autonomous Vehicle Systems Lab, University of Colorado at Boulder

 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

 */
/*
* strMap.h
*
* Defines map implementation for use in shunting yard algorithm
*
* University of Colorado, Autonomous Vehicle Systems (AVS) Lab
* Unpublished Copyright (c) 2012-2015 University of Colorado, All Rights Reserved
*/

#ifndef _STRMAP_H_
#define _STRMAP_H_

typedef struct StrMapPair_t {
    char *key;
    double value;
} StrMapPair_t;

typedef struct StrMapBucket_t {
    unsigned int count;
    StrMapPair_t *pairs;
} StrMapBucket_t;

typedef struct StrMap_t {
    unsigned int count;
    StrMapBucket_t *buckets;
} StrMap_t;

StrMap_t *strMapAlloc(unsigned int capacity);
void      strMapFree(StrMap_t *map);
int       strMapGet(const StrMap_t *map, const char *key, double *value);
int       strMapExists(const StrMap_t *map, const char *key);
int       strMapPut(StrMap_t *map, const char *key, double value);
int       strMapGetCount(StrMap_t *map);

#endif

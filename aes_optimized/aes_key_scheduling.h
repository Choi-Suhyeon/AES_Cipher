#ifndef AES_OPTIMIZED_KEY_SCHEDULING_H
#define AES_OPTIMIZED_KEY_SCHEDULING_H

#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    enum {
        SZ_128 = 0,
        SZ_192 = 1,
        SZ_256 = 2,
    } sz;
    union {
        uint8_t sz128[16];
        uint8_t sz192[24];
        uint8_t sz256[32];
    } k;
} MasterKey;

void schedule_aes_key(const MasterKey *, uint8_t *);

#endif //AES_OPTIMIZED_KEY_SCHEDULING_H

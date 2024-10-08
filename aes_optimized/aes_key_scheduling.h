#ifndef AES_OPTIMIZED_KEY_SCHEDULING_H
#define AES_OPTIMIZED_KEY_SCHEDULING_H

#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

typedef union {
    uint8_t k128[16];
    uint8_t k192[24];
    uint8_t k256[32];
} MasterKey;

void schedule_aes_key(const MasterKey *, uint32_t k_sz, uint8_t *);

#endif //AES_OPTIMIZED_KEY_SCHEDULING_H

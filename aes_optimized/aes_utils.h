#ifndef AES_OPTIMIZED_AES_UTILS_H
#define AES_OPTIMIZED_AES_UTILS_H

#include <stdint.h>

typedef union {
    uint8_t  bytes[16];
    uint32_t words[4];
} State;

static inline void add_round_key(State * const inout, const State * const rk) {
    inout->words[0] ^= rk->words[0];
    inout->words[1] ^= rk->words[1];
    inout->words[2] ^= rk->words[2];
    inout->words[3] ^= rk->words[3];
}

#endif //AES_OPTIMIZED_AES_UTILS_H

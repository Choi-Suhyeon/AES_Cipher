#ifndef AES_OPTIMIZED_CRYPTO_COMMON_H
#define AES_OPTIMIZED_CRYPTO_COMMON_H

#include <memory.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef union {
    uint8_t sz128[16];
    uint8_t sz192[24];
    uint8_t sz256[32];
} MasterKey;

typedef union {
    uint8_t  bytes[16];
    uint32_t words[4];
} State;

typedef struct {
    uint8_t * data;
    uint32_t size;
} Text;

typedef void (* EncDecAlgo) (const State *, const State *, State *);
typedef void (* SchedAlgo) (const MasterKey *, uint8_t *);

static inline void xor_two_states(State * const inout, const State * const s) {
    inout->words[0] ^= s->words[0];
    inout->words[1] ^= s->words[1];
    inout->words[2] ^= s->words[2];
    inout->words[3] ^= s->words[3];
}

Text new_text(uint32_t);
void delete_text(Text *);
void exit_with_err(char *, int);

#endif //AES_OPTIMIZED_CRYPTO_COMMON_H

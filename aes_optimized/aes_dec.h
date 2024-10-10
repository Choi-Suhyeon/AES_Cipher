#ifndef AES_OPTIMIZED_AES_DEC_H
#define AES_OPTIMIZED_AES_DEC_H

#include "crypto_common.h"

void dec_with_aes128(const State *, const State *, State *);
void dec_with_aes192(const State *, const State *, State *);
void dec_with_aes256(const State *, const State *, State *);

#endif //AES_OPTIMIZED_AES_DEC_H

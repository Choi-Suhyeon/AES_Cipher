#ifndef AES_OPTIMIZED_AES_ENC_H
#define AES_OPTIMIZED_AES_ENC_H

#include "crypto_common.h"

void enc_with_aes128(const State *, const State *, State *);
void enc_with_aes192(const State *, const State *, State *);
void enc_with_aes256(const State *, const State *, State *);

#endif //AES_OPTIMIZED_AES_ENC_H

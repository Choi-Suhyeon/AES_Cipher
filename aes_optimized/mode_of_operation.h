#ifndef AES_OPTIMIZED_MODE_OF_OPERATION_H
#define AES_OPTIMIZED_MODE_OF_OPERATION_H

#include "crypto_common.h"

#define AES_TOTAL_RK_SZ 0x100

typedef struct {
    uint32_t  total_rk_sz;
    State     iv;
    MasterKey mk;
    SchedAlgo sched;
    union {
        EncDecAlgo enc;
        EncDecAlgo dec;
    };
} CryptoContext;

void enc_with_ecb(Text *, const CryptoContext *);
void dec_with_ecb(Text *, const CryptoContext *);
void enc_with_cbc(Text *, const CryptoContext *);
void dec_with_cbc(Text *, const CryptoContext *);

#endif //AES_OPTIMIZED_MODE_OF_OPERATION_H

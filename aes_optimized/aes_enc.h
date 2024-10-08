#ifndef AES_OPTIMIZED_AES_ENC_H
#define AES_OPTIMIZED_AES_ENC_H

#include "aes_key_scheduling.h"
#include "aes_utils.h"

void enc_with_aes(State *, const MasterKey *, uint32_t);

#endif //AES_OPTIMIZED_AES_ENC_H

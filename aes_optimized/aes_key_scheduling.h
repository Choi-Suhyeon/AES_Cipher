#ifndef AES_OPTIMIZED_KEY_SCHEDULING_H
#define AES_OPTIMIZED_KEY_SCHEDULING_H


#include "crypto_common.h"

void schedule_aes128(const MasterKey *, uint8_t *);
void schedule_aes192(const MasterKey *, uint8_t *);
void schedule_aes256(const MasterKey *, uint8_t *);

#endif //AES_OPTIMIZED_KEY_SCHEDULING_H

#ifndef AES_OPTIMIZED_PADDING_H
#define AES_OPTIMIZED_PADDING_H

#include "crypto_common.h"

void pad_with_pkcs7(Text *, uint32_t);
void unpad_with_pkcs7(Text *);

#endif //AES_OPTIMIZED_PADDING_H

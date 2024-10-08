#ifndef AES_OPTIMIZED_AES_UTILS_H
#define AES_OPTIMIZED_AES_UTILS_H

#include <stdint.h>

typedef union {
    uint8_t  bytes[16];
    uint32_t words[4];
} State;

#endif //AES_OPTIMIZED_AES_UTILS_H

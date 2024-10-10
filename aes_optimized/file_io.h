#ifndef AES_OPTIMIZED_FILE_IO_H
#define AES_OPTIMIZED_FILE_IO_H

#include "crypto_common.h"

uint32_t read_from(const char *, Text *);
uint32_t write_to(const char *, const Text *);

#endif //AES_OPTIMIZED_FILE_IO_H

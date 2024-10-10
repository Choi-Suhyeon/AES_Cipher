#include "crypto_common.h"

Text new_text(uint32_t size) {
    Text result = {
        .size = size - size % 0x10 + 0x10,
        .data = NULL,
    };

    if (result.size <= size) {
        exit_with_err("exceeded the maximum data size allowed for encryption and decryption", 1);
    }

    if (!(result.data = malloc(result.size))) {
        exit_with_err("failed to allocate memory space", 1);
    }

    return result;
}

void delete_text(Text * const text) {
    free(text->data);
    text->data = NULL;
}

void exit_with_err(char * msg, int status) {
    msg = msg ? msg : "an unexpected error occurred during execution";

    fprintf(stderr, "[error] : %s\n", msg);
    exit(status ? status : 1);
}
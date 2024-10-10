#include "padding.h"

void pad_with_pkcs7(Text * const text, uint32_t sz_without_padding) {
    uint8_t padding_sz = (uint8_t)(0x10 - sz_without_padding % 0x10);

    memset(text->data + sz_without_padding, padding_sz, padding_sz);
}

void unpad_with_pkcs7(Text * const text) {
    text->size -= text->data[text->size - 1];
}

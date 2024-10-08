#include "debug.h"

void swap(uint8_t *, uint8_t *);

void debug_state(const char * const title, const State * const s) {
    uint8_t tmp[0x10];

    memmove(tmp, s, sizeof tmp);
    printf("[%s]\n", title);

    swap(tmp + 0x1, tmp + 0x4);
    swap(tmp + 0x2, tmp + 0x8);
    swap(tmp + 0x3, tmp + 0xC);
    swap(tmp + 0x6, tmp + 0x9);
    swap(tmp + 0x7, tmp + 0xD);
    swap(tmp + 0xB, tmp + 0xE);

    for (uint32_t i = 0; i < 16; i++) {
        printf("%02X ", tmp[i]);
        printf("%s", (i + 1) % 4 == 0 ? "\n" : "");
    }

    puts("");
}

void swap(uint8_t * const x, uint8_t * const y) {
    *x ^= *y;
    *y ^= *x;
    *x ^= *y;
}
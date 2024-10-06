#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "operation.h"

const uint8_t kRCons[] = {
    0x01, 0x02, 0x04, 0x08, 0x10,
    0x20, 0x40, 0x80, 0x1B, 0x36,
};

void add_rk(uint8_t * const s1, const uint8_t * const s2) {
    *(uint32_t *)(s1 + 0x0) ^= *(uint32_t *)(s2 + 0x0);
    *(uint32_t *)(s1 + 0x4) ^= *(uint32_t *)(s2 + 0x4);
    *(uint32_t *)(s1 + 0x8) ^= *(uint32_t *)(s2 + 0x8);
    *(uint32_t *)(s1 + 0xC) ^= *(uint32_t *)(s2 + 0xC);
}

void sub_bytes(uint8_t * const s) {
    for (uint32_t i = 0; i < 0x10; i++) {
        s[i] = s_box(s[i]);
    }
}

void inv_sub_bytes(uint8_t * const s) {
    for (uint32_t i = 0; i < 0x10; i++) {
        s[i] = inv_s_box(s[i]);
    }
}

void shift_rows(uint8_t * const s) {
    uint8_t r[0x10] = {
        s[0x0], s[0x5], s[0xA], s[0xF],
        s[0x4], s[0x9], s[0xE], s[0x3],
        s[0x8], s[0xD], s[0x2], s[0x7],
        s[0xC], s[0x1], s[0x6], s[0xB],
    };

    memmove(s, r, sizeof r);
}

void inv_shift_rows(uint8_t * const s) {
    uint8_t r[0x10] = {
        s[0x0], s[0xD], s[0xA], s[0x7],
        s[0x4], s[0x1], s[0xE], s[0xB],
        s[0x8], s[0x5], s[0x2], s[0xF],
        s[0xC], s[0x9], s[0x6], s[0x3],
    };

    memmove(s, r, sizeof r);
}

void mix_cols(uint8_t * const s) {
    uint8_t r[0x10];

    for (uint32_t i = 0; i < 0x10; i += 4) {
        r[i + 0] = MUL2(s[i + 0]) ^ MUL3(s[i + 1]) ^ s[i + 2] ^ s[i + 3];
        r[i + 1] = s[i + 0] ^ MUL2(s[i + 1]) ^ MUL3(s[i + 2]) ^ s[i + 3];
        r[i + 2] = s[i + 0] ^ s[i + 1] ^ MUL2(s[i + 2]) ^ MUL3(s[i + 3]);
        r[i + 3] = MUL3(s[i + 0]) ^ s[i + 1] ^ s[i + 2] ^ MUL2(s[i + 3]);
    }

    memmove(s, r, 0x10);
}

void inv_mix_cols(uint8_t * const s) {
    uint8_t r[0x10];

    for (uint32_t i = 0; i < 0x10; i += 4) {
        r[i + 0] = MULE(s[i + 0]) ^ MULB(s[i + 1]) ^ MULD(s[i + 2]) ^ MUL9(s[i + 3]);
        r[i + 1] = MUL9(s[i + 0]) ^ MULE(s[i + 1]) ^ MULB(s[i + 2]) ^ MULD(s[i + 3]);
        r[i + 2] = MULD(s[i + 0]) ^ MUL9(s[i + 1]) ^ MULE(s[i + 2]) ^ MULB(s[i + 3]);
        r[i + 3] = MULB(s[i + 0]) ^ MULD(s[i + 1]) ^ MUL9(s[i + 2]) ^ MULE(s[i + 3]);
    }

    memmove(s, r, 0x10);
}

void aes_enc(uint8_t * pt, uint8_t * rk, uint32_t k_sz, uint8_t * ct) {
    uint32_t nr = (k_sz >> 5) + 6;
    uint8_t tmp[0x10];

    memmove(tmp, pt, sizeof tmp);
    add_rk(tmp, rk);

    for (uint32_t i = 1; i < nr; i++) {
        sub_bytes(tmp);
        shift_rows(tmp);
        mix_cols(tmp);
        add_rk(tmp, rk + 0x10 * i);
    }

    sub_bytes(tmp);
    shift_rows(tmp);
    add_rk(tmp, rk + 0x10 * nr);

    memmove(ct, tmp, sizeof tmp);
}

void aes_dec(uint8_t * ct, uint8_t * rk, uint32_t k_sz, uint8_t * pt) {
    uint32_t nr = (k_sz >> 5) + 6;
    uint8_t tmp[0x10];

    memmove(tmp, ct, sizeof tmp);
    add_rk(tmp, rk + 0x10 * nr);

    for (uint32_t i = nr - 1; i > 0; i--) {
        inv_shift_rows(tmp);
        inv_sub_bytes(tmp);
        add_rk(tmp, rk + 0x10 * i);
        inv_mix_cols(tmp);
    }

    inv_shift_rows(tmp);
    inv_sub_bytes(tmp);
    add_rk(tmp, rk);

    memmove(pt, tmp, sizeof tmp);
}

static inline void sub_word(uint8_t * x) {
    x[0] = s_box(x[0]);
    x[1] = s_box(x[1]);
    x[2] = s_box(x[2]);
    x[3] = s_box(x[3]);
}

void rk_gen128(uint8_t * mk, uint8_t * rk) {
    memmove(rk, mk, 0x10);

    for (uint32_t i = 0; i < 0x10 * 0xA; i += 0x10) {
        uint8_t t[4];

        memmove(t, rk + 0xC + i, sizeof t);
        rol32_u8s(t, 8);
        sub_word(t);
        t[0] ^= kRCons[i / 0x10];

        *(uint32_t *)(rk + 0x10 + i) = *(uint32_t *)(rk + 0x0 + i) ^ *(uint32_t *)t;
        *(uint32_t *)(rk + 0x14 + i) = *(uint32_t *)(rk + 0x4 + i) ^ *(uint32_t *)(rk + 0x10 + i);
        *(uint32_t *)(rk + 0x18 + i) = *(uint32_t *)(rk + 0x8 + i) ^ *(uint32_t *)(rk + 0x14 + i);
        *(uint32_t *)(rk + 0x1C + i) = *(uint32_t *)(rk + 0xC + i) ^ *(uint32_t *)(rk + 0x18 + i);
    }
}

void rk_gen192(uint8_t * mk, uint8_t * rk) {
    memmove(rk, mk, 0x18);

    for (uint32_t i = 0; i < 0x18 * 0x8; i += 0x18) {
        uint8_t t[4];

        memmove(t, rk + 0x14 + i, sizeof t);
        rol32_u8s(t, 8);
        sub_word(t);
        t[0] ^= kRCons[i / 0x18];

        *(uint32_t *)(rk + 0x18 + i) = *(uint32_t *)(rk + 0x00 + i) ^ *(uint32_t *)t;
        *(uint32_t *)(rk + 0x1C + i) = *(uint32_t *)(rk + 0x04 + i) ^ *(uint32_t *)(rk + 0x18 + i);
        *(uint32_t *)(rk + 0x20 + i) = *(uint32_t *)(rk + 0x08 + i) ^ *(uint32_t *)(rk + 0x1C + i);
        *(uint32_t *)(rk + 0x24 + i) = *(uint32_t *)(rk + 0x0C + i) ^ *(uint32_t *)(rk + 0x20 + i);
        *(uint32_t *)(rk + 0x28 + i) = *(uint32_t *)(rk + 0x10 + i) ^ *(uint32_t *)(rk + 0x24 + i);
        *(uint32_t *)(rk + 0x2C + i) = *(uint32_t *)(rk + 0x14 + i) ^ *(uint32_t *)(rk + 0x28 + i);
    }
}

void rk_gen256(uint8_t * mk, uint8_t * rk) {
    memmove(rk, mk, 0x20);

    for (uint32_t i = 0; i < 0x20 * 0x7; i += 0x20) {
        uint8_t t[4];

        memmove(t, rk + 0x1C + i, sizeof t);
        rol32_u8s(t, 8);
        sub_word(t);
        t[0] ^= kRCons[i / 0x20];

        *(uint32_t *)(rk + 0x20 + i) = *(uint32_t *)(rk + 0x00 + i) ^ *(uint32_t *)t;
        *(uint32_t *)(rk + 0x24 + i) = *(uint32_t *)(rk + 0x04 + i) ^ *(uint32_t *)(rk + 0x20 + i);
        *(uint32_t *)(rk + 0x28 + i) = *(uint32_t *)(rk + 0x08 + i) ^ *(uint32_t *)(rk + 0x24 + i);
        *(uint32_t *)(rk + 0x2C + i) = *(uint32_t *)(rk + 0x0C + i) ^ *(uint32_t *)(rk + 0x28 + i);

        memmove(t, rk + 0x2C + i, sizeof t);
        sub_word(t);

        *(uint32_t *)(rk + 0x30 + i) = *(uint32_t *)(rk + 0x10 + i) ^ *(uint32_t *)t;
        *(uint32_t *)(rk + 0x34 + i) = *(uint32_t *)(rk + 0x14 + i) ^ *(uint32_t *)(rk + 0x30 + i);
        *(uint32_t *)(rk + 0x38 + i) = *(uint32_t *)(rk + 0x18 + i) ^ *(uint32_t *)(rk + 0x34 + i);
        *(uint32_t *)(rk + 0x3C + i) = *(uint32_t *)(rk + 0x1C + i) ^ *(uint32_t *)(rk + 0x38 + i);
    }
}

void aes_key_schedule(uint8_t * mk, uint32_t k_sz, uint8_t * rk) {
    switch (k_sz) {
        case 128: rk_gen128(mk, rk); break;
        case 192: rk_gen192(mk, rk); break;
        case 256: rk_gen256(mk, rk); break;
        default:  fputs("fatal: unsupported master key size is given", stderr); exit(1);
    }
}

int main(void) {
    /*
    uint8_t pt[0x10] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
    };

    uint8_t mk128[0x10] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
    };

    uint8_t mk192[0x18] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    };

    uint8_t mk256[0x20] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
    };

    uint8_t rk[0x100] = { 0x00, };
    uint8_t ct[0x10]  = { 0x00, };
    uint32_t k_sz = 128;

    aes_key_schedule(mk128, k_sz, rk);

    aes_enc(pt, rk, k_sz, ct);

    for (int i = 0; i < 16; i++) {
        printf("%02X ", ct[i]);

        if ((i + 1) % 4 == 0) {
            puts("");
        }
    }
    puts("end");
    */

    /*
    uint8_t ct[16] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
    };

    uint8_t mk[16] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
    };

    uint8_t rk[0x100] = {};
    uint8_t pt[0x16] = {};
    uint32_t k_sz = 128;

    aes_key_schedule(mk, k_sz, rk);
    aes_dec(ct, rk, k_sz, pt);

    for (int i = 0; i < 16; i++) {
        printf("%02X ", pt[i]);

        if ((i + 1) % 4 == 0) {
            puts("");
        }
    }
    */

    /*
    uint8_t ct[16] = {
        0x40, 0x24, 0xBD, 0xEB, 0x3B, 0x37, 0xA4, 0x05,
        0x90, 0xEA, 0xE9, 0xEA, 0x3A, 0xBE, 0x32, 0x39,
    };

    uint8_t mk192[0x18] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    };

    uint8_t rk[0x100] = {};
    uint8_t pt[0x16] = {};
    uint32_t k_sz = 192;

    aes_key_schedule(mk192, k_sz, rk);
    aes_dec(ct, rk, k_sz, pt);

    for (int i = 0; i < 16; i++) {
        printf("%02X ", pt[i]);

        if ((i + 1) % 4 == 0) {
            puts("");
        }
    }
    */

    uint8_t ct[16] = {
        0xE0, 0x1E, 0x60, 0xA1, 0xA3, 0xA6, 0xED, 0x28,
        0xCB, 0x07, 0x95, 0xD8, 0x67, 0x9E, 0x8D, 0x53,
    };

    uint8_t mk256[0x20] = {
            0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
            0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
    };

    uint8_t rk[0x100] = {};
    uint8_t pt[0x16] = {};
    uint32_t k_sz = 256;

    aes_key_schedule(mk256, k_sz, rk);
    aes_dec(ct, rk, k_sz, pt);

    for (int i = 0; i < 16; i++) {
        printf("%02X ", pt[i]);

        if ((i + 1) % 4 == 0) {
            puts("");
        }
    }

    return 0;
}
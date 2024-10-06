#include "operation.h"

uint8_t mul(uint8_t a, uint8_t b) {
    uint8_t r = 0;

    for (uint32_t i = 0; i < 8; i++) {
        r ^= (int8_t)((a & 1) << 7) >> 7 & b;
        b = MUL2(b);
        a >>= 1;
    }

    return r;
}

uint8_t inv(uint8_t a) {
    uint8_t r = a;

    r = mul(a, mul(r, r));
    r = mul(a, mul(r, r));
    r = mul(a, mul(r, r));
    r = mul(a, mul(r, r));
    r = mul(a, mul(r, r));
    r = mul(a, mul(r, r));

    return mul(r, r);
}

uint8_t s_box(uint8_t a) {
    uint8_t
        tmp = inv(a),
        r   = 0;

    r ^= (int8_t)((tmp &   1) << 7) >> 7 & 0x1F;
    r ^= (int8_t)((tmp &   2) << 6) >> 7 & 0x3E;
    r ^= (int8_t)((tmp &   4) << 5) >> 7 & 0x7C;
    r ^= (int8_t)((tmp &   8) << 4) >> 7 & 0xF8;
    r ^= (int8_t)((tmp &  16) << 3) >> 7 & 0xF1;
    r ^= (int8_t)((tmp &  32) << 2) >> 7 & 0xE3;
    r ^= (int8_t)((tmp &  64) << 1) >> 7 & 0xC7;
    r ^= (int8_t)((tmp & 128) << 0) >> 7 & 0x8F;

    return r ^ 0x63;
}

uint8_t inv_s_box(uint8_t a) {
    uint8_t
        tmp = a ^ 0x63,
        r   = 0;

    r ^= (int8_t)((tmp &   1) << 7) >> 7 & 0x4A;
    r ^= (int8_t)((tmp &   2) << 6) >> 7 & 0x94;
    r ^= (int8_t)((tmp &   4) << 5) >> 7 & 0x29;
    r ^= (int8_t)((tmp &   8) << 4) >> 7 & 0x52;
    r ^= (int8_t)((tmp &  16) << 3) >> 7 & 0xA4;
    r ^= (int8_t)((tmp &  32) << 2) >> 7 & 0x49;
    r ^= (int8_t)((tmp &  64) << 1) >> 7 & 0x92;
    r ^= (int8_t)((tmp & 128) << 0) >> 7 & 0x25;

    return inv(r);
}

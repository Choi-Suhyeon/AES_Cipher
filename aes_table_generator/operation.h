#ifndef AES_TABLE_GENERATOR_OPERATION_H
#define AES_TABLE_GENERATOR_OPERATION_H

#include <stdint.h>
#include <memory.h>

#define MUL2(a) (((a) << 1) ^ ((int8_t)((a) & 0x80) >> 7 & 0x1B))
#define MUL3(a) ((a) ^ MUL2(a))
#define MUL4(a) (MUL2(MUL2(a)))
#define MUL8(a) (MUL2(MUL2(MUL2(a))))
#define MUL9(a) ((a) ^ MUL8(a))
#define MULB(a) ((a) ^ MUL2(a) ^ MUL8(a))
#define MULD(a) ((a) ^ MUL4(a) ^ MUL8(a))
#define MULE(a) (MUL2(a) ^ MUL4(a) ^ MUL8(a))

static inline uint32_t rol32(uint32_t v, uint32_t n) {
    return (v << n) | (v >> (32 - n));
}

static inline uint32_t ror32(uint32_t v, uint32_t n) {
    return (v >> n) | (v << (32 - n));
}

static inline void rol32_u8s(uint8_t * v, uint32_t n) {
    uint32_t r = ror32(*(uint32_t *)v, n);
    memmove(v, &r, sizeof r);
}

static inline void ror32_u8s(uint8_t * v, uint32_t n) {
    uint32_t r = rol32(*(uint32_t *)v, n);
    memmove(v, &r, sizeof r);
}

uint8_t mul(uint8_t, uint8_t);
uint8_t inv(uint8_t);
uint8_t s_box(uint8_t);
uint8_t inv_s_box(uint8_t);

#endif //AES_TABLE_GENERATOR_OPERATION_H

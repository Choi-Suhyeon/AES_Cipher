#ifndef AES_OPERATION_H
#define AES_OPERATION_H

#include <stdint.h>
#include <memory.h>

// mix columns 연산에서 임의의 1byte를 1, 2, 3, 9, B, D, E와 곱하는 연산이 필요.
/*
MUL2(a)
 MSB가 1일 경우 감산을 해야 하기 때문에 결과를 0x1B와 xor해야 됨.
 0x80과 &를 해서 MSB의 값을 구하고 int8_t로 형변환 한 후 7만큼 오른쪽 시프트
 그러면 MSB가 1이면 1로 모든 비트가 채워지고 0이면 0으로 모든 비트가 채워짐.
 0x1B와 & 연산을 하면 MSB가 1일 때만 0x1B와 xor 할 수 있음.
*/

/*
MUL3(a)
 = 3*a = a(x + 1) = ax + a = MUL2(a) ^ a
 */

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

#endif //AES_OPERATION_H

#include "mode_of_operation.h"

uint8_t * get_rk(const CryptoContext * ctx);

void enc_with_ecb(Text * inout, const CryptoContext * ctx) {
    uint8_t * rk = get_rk(ctx);

    for (uint32_t i = 0; i < inout->size; i += sizeof(State)) {
        ctx->enc((State *)(inout->data + i), (State *)rk, (State *)(inout->data + i));
    }

    free(rk);
}

void dec_with_ecb(Text * inout, const CryptoContext * ctx) {
    enc_with_ecb(inout, ctx);
}

void enc_with_cbc(Text * inout, const CryptoContext * ctx) {
    const State * iv = &ctx->iv;

    uint8_t * rk = get_rk(ctx);

    for (uint32_t i = 0; i < inout->size; i += sizeof(State)) {
        State * ps = (State *)(inout->data + i);

        xor_two_states(ps, iv);
        ctx->enc(ps, (State *)rk, ps);

        iv = ps;
    }

    free(rk);
}

void dec_with_cbc(Text * inout, const CryptoContext * ctx) {
    State iv, next_iv;

    uint8_t * rk = get_rk(ctx);

    memmove(&iv, &ctx->iv, sizeof iv);

    for (uint32_t i = 0; i < inout->size; i += sizeof(State)) {
        State * ps = (State *)(inout->data + i);

        memmove(&next_iv, ps, sizeof next_iv);
        ctx->dec(ps, (State *)rk, ps);
        xor_two_states(ps, &iv);
        memmove(&iv, &next_iv, sizeof iv);
    }

    free(rk);
}

uint8_t * get_rk(const CryptoContext * ctx) {
    uint8_t * rk = NULL;

    if (!(rk = calloc(ctx->total_rk_sz, sizeof *rk))) {
        exit_with_err("failed to allocate memory space", 1);
    }

    ctx->sched(&ctx->mk, rk);

    return rk;
}

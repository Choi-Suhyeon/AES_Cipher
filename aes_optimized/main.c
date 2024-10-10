#include <string.h>

#include "aes_enc.h"
#include "aes_dec.h"
#include "aes_key_scheduling.h"
#include "mode_of_operation.h"
#include "file_io.h"
#include "padding.h"

#ifdef _MSC_VER
#include "getopt.h"
#else
#include <getopt.h>
#endif

typedef struct {
    char
        * kind,
        * size;
} Crypto;

void print_usage(void);
int elem(const char *, const char * const *, uint32_t);
Crypto get_crypto(const char *);

int main(int argc, char ** argv) {
    int opt;

    const char
        * paddings[]   = { "pkcs7" },
        * algorithms[] = { "aes128", "aes192", "aes256" },
        * modes[]      = { "cbc", "ecb" };

    const char
        * padding   = paddings[0],
        * algorithm = algorithms[0],
        * mode      = modes[0];

    static struct option long_options[] = {
        { "padding",   required_argument, 0, 'p' },
        { "algorithm", required_argument, 0, 'a' },
        { "mode",      required_argument, 0, 'm' },
        { 0, 0, 0, 0 },
    };

    if (argc < 3) {
        print_usage();
    }

    uint32_t counter[3] = { 0 };

    while((opt = getopt_long(argc, argv, "p:a:m:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'p':
                padding = optarg;
                counter[0]++;
                break;
            case 'a':
                algorithm = optarg;
                counter[1]++;
                break;
            case 'm':
                mode = optarg;
                counter[2]++;
                break;
            default:
                print_usage();
        }
    }

    const char * const command        = argv[optind];
    int                file_paths_idx = optind + 1;

    if (strcmp(command, "enc") != 0 && strcmp(command, "dec") != 0) {
        print_usage();
    }

    if (counter[0] > 1 || counter[1] > 1 || counter[2] > 1) {
        exit_with_err("the same option has been provided more than once", 1);
    }

    if (!elem(padding, paddings, sizeof paddings / sizeof *paddings)) {
        exit_with_err("invalid padding provided", 1);
    }

    if (!elem(algorithm, algorithms, sizeof algorithms / sizeof *algorithms)) {
        exit_with_err("invalid algorithm provided", 1);
    }

    if (!elem(mode, modes, sizeof modes / sizeof *modes)) {
        exit_with_err("invalid mode provided", 1);
    }

    Crypto crypto = get_crypto(algorithm);
    int    is_enc = !strcmp(command, "enc");

    State iv = { 0 };
    MasterKey mk = {
        .sz128 = {
            0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
            0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
        }
    };

    CryptoContext ctx;

    ctx.total_rk_sz = AES_TOTAL_RK_SZ;
    ctx.sched       =
        !strcmp(algorithm, "aes128") ? schedule_aes128 :
        !strcmp(algorithm, "aes192") ? schedule_aes192 :
        !strcmp(algorithm, "aes256") ? schedule_aes256 :
        NULL;

    memmove(&ctx.iv.bytes, &iv, sizeof iv);

    if (!strcmp(crypto.size, "128")) {
        memmove(&ctx.mk.sz128, mk.sz128, 128 >> 3);
        ctx.enc = is_enc ? enc_with_aes128 : dec_with_aes128;
    }
    else if (!strcmp(crypto.size, "192")) {
        memmove(&ctx.mk.sz192, mk.sz128, 192 >> 3);
        ctx.enc = is_enc ? enc_with_aes192 : dec_with_aes192;
    }
    else {
        memmove(&ctx.mk.sz256, mk.sz128, 256 >> 3);
        ctx.enc = is_enc ? enc_with_aes256 : dec_with_aes256;
    }

    void (* mode_of_operation) (Text *, const CryptoContext *);
    Text text;

    if (is_enc) {
        mode_of_operation = !strcmp(mode, "cbc") ? enc_with_cbc : enc_with_ecb;

        for (int i = file_paths_idx; i < argc; i++) {
            uint32_t size_without_padding;

            if (!(size_without_padding = read_from(argv[i], &text))) {
                puts("read failed");
                exit(1);
            }

            pad_with_pkcs7(&text, size_without_padding);
            mode_of_operation(&text, &ctx);
            write_to(argv[i], &text);
            delete_text(&text);
        }
    }
    else {
        mode_of_operation = !strcmp(mode, "cbc") ? dec_with_cbc : dec_with_ecb;

        for (int i = file_paths_idx; i < argc; i++) {

            if (!read_from(argv[i], &text)) {
                puts("read failed");
                exit(1);
            }

            mode_of_operation(&text, &ctx);
            unpad_with_pkcs7(&text);
            write_to(argv[i], &text);
            delete_text(&text);
        }
    }

    free(crypto.kind);

    return 0;
}

void print_usage(void) {
    exit(1);
}

int elem(const char * const s, const char * const * const arr, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        if (!strcmp(s, arr[i])) {
            return 1;
        }
    }

    return 0;
}

Crypto get_crypto(const char * const algorithm) {
    Crypto             result = {};
    unsigned long long len    = strlen(algorithm);

    if (!(result.kind = calloc(len + 2, sizeof(char)))) {
        return result;
    }

    result.size = result.kind + len - 2;

    memmove(result.kind, algorithm, len - 3);
    memmove(result.size, algorithm + len - 3, 3);

    return result;
}
#include <stdint.h>
#include <stdio.h>

#include "operation.h"

void gen_enc_tbl(void) {
    // encryption table 0
    printf("const uint32_t kEncTbl0[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = s_box((uint8_t)i);

        printf("0x%02X%02X%02X%02X, ", (uint8_t)MUL3(subbed), subbed, subbed, (uint8_t)MUL2(subbed));
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    // encryption table 1
    printf("const uint32_t kEncTbl1[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = s_box((uint8_t)i);

        printf("0x%02X%02X%02X%02X, ", subbed, subbed, (uint8_t)MUL2(subbed), (uint8_t)MUL3(subbed));
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    // encryption table 2
    printf("const uint32_t kEncTbl2[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = s_box((uint8_t)i);

        printf("0x%02X%02X%02X%02X, ", subbed, (uint8_t)MUL2(subbed), (uint8_t)MUL3(subbed), subbed);
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    // encryption table 3
    printf("const uint32_t kEncTbl3[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = s_box((uint8_t)i);

        printf("0x%02X%02X%02X%02X, ", (uint8_t)MUL2(subbed), (uint8_t)MUL3(subbed), subbed, subbed);
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");
}

void gen_dec_tbl(void) {
    // encryption table 0
    printf("const uint32_t kDecTbl0[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = inv_s_box((uint8_t)i);

        printf("0x%02X%02X%02X%02X, ", (uint8_t)MULB(subbed), (uint8_t)MULD(subbed), (uint8_t)MUL9(subbed), (uint8_t)MULE(subbed));
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    // encryption table 1
    printf("const uint32_t kDecTbl1[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = inv_s_box((uint8_t)i);

        printf("0x%02X%02X%02X%02X, ", (uint8_t)MULD(subbed), (uint8_t)MUL9(subbed), (uint8_t)MULE(subbed), (uint8_t)MULB(subbed));
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    // encryption table 2
    printf("const uint32_t kDecTbl2[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = inv_s_box((uint8_t)i);

        printf("0x%02X%02X%02X%02X, ", (uint8_t)MUL9(subbed), (uint8_t)MULE(subbed), (uint8_t)MULB(subbed), (uint8_t)MULD(subbed));
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    // encryption table 3
    printf("const uint32_t kDecTbl3[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = inv_s_box((uint8_t)i);

        printf("0x%02X%02X%02X%02X, ", (uint8_t)MULE(subbed), (uint8_t)MULB(subbed), (uint8_t)MULD(subbed), (uint8_t)MUL9(subbed));
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");
}

void gen_s_box(void) {
    printf("const uint8_t kSBox[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        printf("0x%02X, ", s_box((uint8_t)i));
        printf("%s", (i + 1) % 16 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");
}

void gen_inv_s_box(void) {
    printf("const uint8_t kInvSBox[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        printf("0x%02X, ", inv_s_box((uint8_t)i));
        printf("%s", (i + 1) % 16 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");
}

int main() {
    gen_inv_s_box();

    return 0;
}

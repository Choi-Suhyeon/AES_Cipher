#include <stdint.h>
#include <stdio.h>

#include "operation.h"


int main() {
    // encryption table 0
    printf("const uint32_t kEncTbl0[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = s_box((uint8_t)i);

        printf("0x%02x%02x%02x%02x, ", (uint8_t)MUL3(subbed), subbed, subbed, (uint8_t)MUL2(subbed));
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    // encryption table 1
    printf("const uint32_t kEncTbl1[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = s_box((uint8_t)i);

        printf("0x%02x%02x%02x%02x, ", subbed, subbed, (uint8_t)MUL2(subbed), (uint8_t)MUL3(subbed));
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    // encryption table 2
    printf("const uint32_t kEncTbl2[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = s_box((uint8_t)i);

        printf("0x%02x%02x%02x%02x, ", subbed, (uint8_t)MUL2(subbed), (uint8_t)MUL3(subbed), subbed);
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    // encryption table 3
    printf("const uint32_t kEncTbl3[0x100] = {\n    ");

    for (uint32_t i = 0; i < 0x100; i++) {
        uint8_t subbed = s_box((uint8_t)i);

        printf("0x%02x%02x%02x%02x, ", (uint8_t)MUL2(subbed), (uint8_t)MUL3(subbed), subbed, subbed);
        printf("%s", (i + 1) % 8 == 0 ? "\n    " : "");
    }

    puts("\b\b\b\b};\n");

    return 0;
}

#include "file_io.h"

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#pragma warning(disable: 6031)
#endif

uint32_t read_from(const char * file_name, Text * const out) {
    FILE * fp = NULL;

    if (!(fp = fopen(file_name, "rb"))) {
        return 0;
    }

    fseek(fp, 0, SEEK_END);

    uint32_t length = ftell(fp);

    *out = new_text(length);

    fseek(fp, 0, SEEK_SET);
    fread(out->data, 1, length, fp);
    fclose(fp);

    return length;
}

uint32_t write_to(const char * file_name, const Text * const in) {
    FILE * fp = NULL;

    if (!(fp = fopen(file_name, "wb"))) {
        return 0;
    }

    fwrite(in->data, 1, in->size, fp);
    fclose(fp);

    return 1;
}

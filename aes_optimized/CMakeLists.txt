cmake_minimum_required(VERSION 3.27)
project(aes_optimized C)

set(CMAKE_C_STANDARD 17)

if(MSVC)
    add_compile_options(/O2)
    add_executable(aes_optimized
        main.c    crypto_common.c aes_key_scheduling.c debug.c
        aes_enc.c aes_dec.c       mode_of_operation.c  file_io.c
        padding.c getopt.c
    )
else()
    add_compile_options(-O2)
    add_executable(aes_optimized
        main.c    crypto_common.c aes_key_scheduling.c debug.c
        aes_enc.c aes_dec.c       mode_of_operation.c  file_io.c
        padding.c
    )
endif()



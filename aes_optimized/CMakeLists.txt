cmake_minimum_required(VERSION 3.27)
project(aes_optimized C)

set(CMAKE_C_STANDARD 17)

if(MSVC)
    add_compile_options(/O2)
else()
    add_compile_options(-O2)
endif()

add_executable(aes_optimized main.c aes_utils.c aes_key_scheduling.c debug.c aes_enc.c)

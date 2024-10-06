cmake_minimum_required(VERSION 3.27)
project(aes C)

set(CMAKE_C_STANDARD 17)

add_compile_options(-O2)
add_executable(aes aes.c operation.c)

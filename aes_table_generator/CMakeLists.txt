cmake_minimum_required(VERSION 3.27)
project(aes_table_generator C)

set(CMAKE_C_STANDARD 17)

add_executable(aes_table_generator main.c operation.c)

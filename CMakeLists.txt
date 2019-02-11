cmake_minimum_required(VERSION 3.2.2)
project(Turing)

set(CMAKE_C_STANDARD 11)

file(GLOB base_SRC "c/headers/*.h" "c/*.c")
set(SOURCE_FILES_PROD ${base_SRC})

add_executable(Turing ${SOURCE_FILES_PROD})
cmake_minimum_required(VERSION 3.10)
project(cpu_sim C)

set(CMAKE_C_STANDARD 11)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_executable(cpu_sim
    src/main.c
    src/cpu.c
    src/assembler.c
)

target_include_directories(cpu_sim PRIVATE src)

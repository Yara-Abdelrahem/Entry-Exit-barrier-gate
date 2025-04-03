cmake_minimum_required(VERSION 3.11)
project(NVIC_lib C)

add_library(NVIC_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/nvic.c
)

target_include_directories(NVIC_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)


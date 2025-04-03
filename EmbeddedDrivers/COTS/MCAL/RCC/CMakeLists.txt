cmake_minimum_required(VERSION 3.11)
project(RCC_lib C)

add_library(RCC_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rcc.c
)

target_include_directories(RCC_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)

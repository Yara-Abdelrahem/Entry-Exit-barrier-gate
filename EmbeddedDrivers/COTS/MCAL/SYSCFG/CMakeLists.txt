cmake_minimum_required(VERSION 3.11)
project(SYSCFG_lib C)

add_library(SYSCFG_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/syscfg.c
)

target_include_directories(SYSCFG_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)

cmake_minimum_required(VERSION 3.11)
project(EXTI_lib C)

add_library(EXTI_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/exti.c
)

target_include_directories(EXTI_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)


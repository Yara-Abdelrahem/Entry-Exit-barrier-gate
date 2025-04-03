cmake_minimum_required(VERSION 3.11)
project(TIM2_5_lib C)

add_library(TIM2_5_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tim2_5.c
)

target_include_directories(TIM2_5_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)

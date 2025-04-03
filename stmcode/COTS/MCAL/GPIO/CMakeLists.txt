cmake_minimum_required(VERSION 3.11)
project(GPIO_lib C)

add_library(GPIO_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GPIO.c
)

target_include_directories(GPIO_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)
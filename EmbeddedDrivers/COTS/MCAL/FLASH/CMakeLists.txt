cmake_minimum_required(VERSION 3.11)
project(FLASH_lib C)

add_library(FLASH_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flash.c
)

target_include_directories(FLASH_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)
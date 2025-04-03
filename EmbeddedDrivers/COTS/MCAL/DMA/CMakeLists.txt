cmake_minimum_required(VERSION 3.11)
project(DMA_lib C)

add_library(DMA_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dma.c
)

target_include_directories(DMA_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)


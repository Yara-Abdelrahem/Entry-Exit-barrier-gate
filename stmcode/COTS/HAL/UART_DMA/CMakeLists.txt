CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0)
project(UART_DMA_lib C)

set(MCAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MCAL")

add_library(UART_DMA_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/uart_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/uart_dma_cfg.c
)

target_link_libraries(UART_DMA_lib PUBLIC MCAL_lib)   

target_include_directories(UART_DMA_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
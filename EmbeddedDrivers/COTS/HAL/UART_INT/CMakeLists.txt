CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0)
project(UART_INT_lib C)

set(MCAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MCAL")

#add_subdirectory(${MCAL_LIB_PATH}  "${CMAKE_BINARY_DIR}/MCAL") 
add_library(UART_INT_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/uart_int.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/uart_int_cfg.c
)

target_link_libraries(UART_INT_lib PUBLIC MCAL_lib)   

target_include_directories(UART_INT_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
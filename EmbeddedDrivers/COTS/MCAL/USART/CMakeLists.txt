cmake_minimum_required(VERSION 3.11)
project(USART_lib C)

if (NOT TARGET RCC_lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../RCC "${CMAKE_BINARY_DIR}/RCC")
endif()

if (NOT TARGET NVIC_lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../NVIC "${CMAKE_BINARY_DIR}/NVIC")
endif()

add_library(USART_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usart.c
)

target_link_libraries(USART_lib PRIVATE RCC_lib)
target_link_libraries(USART_lib PRIVATE NVIC_lib)

target_include_directories(USART_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)

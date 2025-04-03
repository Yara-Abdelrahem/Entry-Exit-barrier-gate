cmake_minimum_required(VERSION 3.11)
project(SYSTICK_lib C)

if (NOT TARGET RCC_lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../RCC "${CMAKE_BINARY_DIR}/RCC")
endif()


add_library(SYSTICK_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/systick.c
)

target_link_libraries(SYSTICK_lib PRIVATE RCC_lib)

target_include_directories(SYSTICK_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/
)


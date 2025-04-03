CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0)
project(ULTRASONIC_lib C)

set(MCAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MCAL")

if (NOT TARGET SCHED_lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../SCHED "${CMAKE_BINARY_DIR}/RCC")
endif()

add_library(ULTRASONIC_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ultrasonic.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ultrasonic_cfg.c
)
target_link_libraries(ULTRASONIC_lib PRIVATE SCHED_lib)
target_link_libraries(ULTRASONIC_lib PUBLIC MCAL_lib)   

target_include_directories(ULTRASONIC_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
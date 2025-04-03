CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0)
project(SCHED_lib C)

set(MCAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MCAL")

#add_subdirectory(${MCAL_LIB_PATH}  "${CMAKE_BINARY_DIR}/MCAL") 
add_library(SCHED_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scheduler.c
)

target_link_libraries(SCHED_lib PUBLIC MCAL_lib)   

target_include_directories(SCHED_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
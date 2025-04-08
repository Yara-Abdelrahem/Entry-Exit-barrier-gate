CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0)
project(SWITCH_ASYNC_lib C)

set(MCAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MCAL")

if (NOT TARGET SCHED_lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../SCHED "${CMAKE_BINARY_DIR}/RCC")
endif()

add_library(SWITCH_ASYNC_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/switch_async.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/switch_async_cfg.c
)
target_link_libraries(SWITCH_ASYNC_lib PRIVATE SCHED_lib)
target_link_libraries(SWITCH_ASYNC_lib PUBLIC MCAL_lib)   

target_include_directories(SWITCH_ASYNC_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
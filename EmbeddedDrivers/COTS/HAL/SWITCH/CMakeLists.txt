CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0)
project(SWITCH_lib C)

set(MCAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MCAL")

#add_subdirectory(${MCAL_LIB_PATH}  "${CMAKE_BINARY_DIR}/MCAL") 
add_library(SWITCH_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/switch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/switch_cfg.c
)

target_link_libraries(SWITCH_lib PUBLIC MCAL_lib)   

target_include_directories(SWITCH_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
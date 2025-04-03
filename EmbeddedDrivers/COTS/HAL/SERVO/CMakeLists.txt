CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0)
project(SERVO_lib C)

set(MCAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MCAL")

#add_subdirectory(${MCAL_LIB_PATH}  "${CMAKE_BINARY_DIR}/MCAL") 
add_library(SERVO_lib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/servo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/servo_cfg.c
)

target_link_libraries(SERVO_lib PUBLIC MCAL_lib)   

target_include_directories(SERVO_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
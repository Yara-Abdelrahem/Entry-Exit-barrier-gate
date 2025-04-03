set(SCRIPT_PATH 
"${CMAKE_CURRENT_SOURCE_DIR}/../../COTS/CMAKE/cortexm4.cmake")
set(MY_TOOLCHAIN_PATH 
"${CMAKE_CURRENT_SOURCE_DIR}/../../COTS/COMMON")

set(HAL_LIBS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../COTS/HAL")


###############################################################
set(PROJECT_NAME entry_exit)

include(${SCRIPT_PATH})

cmake_minimum_required(VERSION 3.11)
project(${PROJECT_NAME} C ASM)
enable_language(ASM)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s)

add_executable(${PROJECT_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${MY_TOOLCHAIN_PATH}/startup.s  
)

add_subdirectory(${HAL_LIBS_PATH}  "${CMAKE_BINARY_DIR}/HAL_LIBS")

target_link_libraries(${PROJECT_NAME} PRIVATE HAL_lib)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
    COMMENT "Generating binary and hex files..."
)
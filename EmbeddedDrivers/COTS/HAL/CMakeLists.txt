cmake_minimum_required(VERSION 3.11.0)
project(HAL_Drivers C)

set(HAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(HAL_lib INTERFACE)
set(MCAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../MCAL")

add_subdirectory(${MCAL_LIB_PATH}  "${CMAKE_BINARY_DIR}/MCAL") 

set(HAL_SUBDIRS
    "LED"
    "SWITCH"
    "SCHED"
    "UART_INT" 
    "UART_DMA"
    "LCD"
    "SWITCH_ASYNC" 
    "SERVO" 
    "ULTRASONIC"
)
foreach(SUBDIR ${HAL_SUBDIRS})
    set(LIB_PATH "${HAL_LIB_PATH}/${SUBDIR}")
    if (NOT TARGET ${SUBDIR}_lib)
        add_subdirectory(${LIB_PATH} "${CMAKE_BINARY_DIR}/${SUBDIR}")
    endif()
    target_link_libraries(HAL_lib INTERFACE ${SUBDIR}_lib)
endforeach()
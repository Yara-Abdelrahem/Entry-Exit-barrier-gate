cmake_minimum_required(VERSION 3.11.0)
project(MCAL_Drivers C)

set(MCAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

set(MCAL_TARGETS)

set(MCAL_SUBDIRS
    "RCC"
    "GPIO"
    "NVIC"
    "EXTI"
    "SYSCFG"
    "DMA"
    "USART"
    "SYSTICK"
    "TIM2_5"
)

foreach(SUBDIR ${MCAL_SUBDIRS})
    set(LIB_PATH "${MCAL_LIB_PATH}/${SUBDIR}")
    if (NOT TARGET ${SUBDIR}_lib)
        add_subdirectory(${LIB_PATH} "${CMAKE_BINARY_DIR}/${SUBDIR}")
        list(APPEND MCAL_TARGETS ${SUBDIR}_lib)
    endif()
endforeach()

add_library(MCAL_lib INTERFACE)

target_link_libraries(MCAL_lib INTERFACE ${MCAL_TARGETS})
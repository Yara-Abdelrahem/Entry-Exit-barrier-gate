# Basic system configuration
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Toolchain paths
set(TOOLCHAIN_PATH "/usr/bin")
set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-gcc)
set(CMAKE_AR ${TOOLCHAIN_PATH}/arm-none-eabi-ar)
set(CMAKE_SIZE ${TOOLCHAIN_PATH}/arm-none-eabi-size)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}/arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}/arm-none-eabi-objdump)

# CPU and FPU configuration - IMPORTANT FOR STM32F401
set(CPU_OPTIONS "-mcpu=cortex-m4 -mthumb")

# Use hard float ABI for better FPU performance
set(FPU_OPTIONS "-mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# Common compiler flags
set(COMMON_FLAGS "${CPU_OPTIONS} ${FPU_OPTIONS} -Wall -Wextra -ffunction-sections -fdata-sections -fno-common -fmessage-length=0")

# Debug options
set(DEBUG_OPTIONS "-Og -g -gdwarf-2")

# Optimization options (choose one)
set(RELEASE_OPTIONS "-O2 -flto -ffast-math")

# Language specific flags
set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${DEBUG_OPTIONS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${DEBUG_OPTIONS} -std=gnu++14 -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp")

# Linker flags
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/../../COTS/COMMON/linkerscript.ld")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS} -Wl,--gc-sections -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_BINARY_DIR}/info.map -specs=nosys.specs -specs=nano.specs")

# Important for FPU initialization
add_definitions(-DARM_MATH_CM4 -D__FPU_PRESENT=1)

# Search paths
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
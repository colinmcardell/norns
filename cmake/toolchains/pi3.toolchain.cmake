# CMake toolchain file for Raspberry Pi 3 (armv8/cortex-a53)

# System information
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armv8)

# Specify the path to the cross compiler
set(TOOLCHAIN_PREFIX arm-linux-gnueabihf)

# Cross compiler paths
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# PI3-specific compilation flags
add_compile_options(
  -mcpu=cortex-a53
  -mtune=cortex-a53
  -mfpu=neon-fp-armv8
  -mfloat-abi=hard
  -ftree-vectorize
)

# Set platform definitions
add_definitions(
  -DPLATFORM_PI3=1
  -DPLATFORM_RPI=1
  -DMEMORY_CONSTRAINED=1
)
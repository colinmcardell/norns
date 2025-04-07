# CMake toolchain file for Compute Module 3 (armv7/cortex-a7)

# System information
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armv7l)

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

# CM3-specific compilation flags
add_compile_options(
  -mcpu=cortex-a7
  -mtune=cortex-a7
  -mfpu=neon-vfpv4
  -mfloat-abi=hard
  -ftree-vectorize
)

# Set platform definitions
add_definitions(
  -DPLATFORM_CM3=1
  -DPLATFORM_RPI=1
  -DMEMORY_CONSTRAINED=1
)
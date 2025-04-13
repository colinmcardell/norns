# CMake toolchain file for Desktop development builds

# System information - use native architecture
# No specific settings for cross-compilation since this is a native build

# Desktop-specific compilation flags
add_compile_options(
  -march=native  # Optimize for the current CPU architecture
)

# Set platform definitions
add_definitions(
  -DPLATFORM_DESKTOP=1
  -DMEMORY_CONSTRAINED=0
)

# Desktop builds are typically for development and debugging
set(CMAKE_BUILD_TYPE_INIT "Debug")
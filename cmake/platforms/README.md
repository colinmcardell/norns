# Norns Platform Configuration Files

This directory contains platform-specific configuration files that define settings for building Norns on different hardware targets.

## Overview

The platform configuration system works in conjunction with the toolchain system to provide a comprehensive cross-platform build solution. While toolchain files (in `../toolchains/`) define the cross-compilation environment, these platform files define platform-specific settings like:

- Audio buffer sizes
- Memory constraint flags
- CPU-specific optimization flags
- Number of inputs/outputs
- SuperCollider dependencies

## Available Platforms

Currently, we support the following platforms:

- **DESKTOP**: Standard desktop environments (Linux, macOS)
- **PI3**: Raspberry Pi 3 (armv8/cortex-a53)
- **CM3**: Compute Module 3 (armv7/cortex-a7)

## How to Use

### Option 1: Combined with Toolchain Files (Recommended)

For cross-compilation, use a toolchain file together with the platform option:

```bash
# Configure a build for Raspberry Pi 3
cmake -DPLATFORM=PI3 -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/pi3.toolchain.cmake ..

# Build a specific target
cmake --build . --target maiden-repl
```

### Option 2: Standalone Platform Configuration

For native builds where you just want the platform-specific settings:

```bash
# Configure a build for the current platform
cmake -DPLATFORM=DESKTOP ..
```

## Platform Detection

The system can auto-detect the platform when running on Raspberry Pi hardware:

```cmake
include(cmake/platforms/platform_utils.cmake)
detect_platform()
configure_platform()
```

This will:
1. Check if running on Raspberry Pi hardware
2. Set the appropriate platform (PI3, CM3, or DESKTOP)
3. Load the platform-specific settings

## Adding a New Platform

To add support for a new platform:

1. Create a new file named `<platform_name>.cmake` in this directory
2. Define platform-specific variables:
   - `NORNS_PLATFORM_FLAGS`: Compiler flags for the platform
   - `NORNS_PLATFORM_DEFINES`: Preprocessor defines
   - `NORNS_AUDIO_BUFFER_SIZE`: Appropriate audio buffer size
   - `NORNS_PRECISION_FLAGS`: Floating-point optimization flags
   - `NORNS_MAX_INPUTS`/`NORNS_MAX_OUTPUTS`: Audio I/O channel counts
   - `NORNS_REQUIRE_SUPERCOLLIDER`: Whether SuperCollider is required
3. Implement the `apply_platform_settings()` function that applies these settings to a target

## Integration with Components

In a component's CMakeLists.txt, use:

```cmake
if(COMMAND configure_for_platform)
    configure_for_platform(your_target_name)
endif()
```

This applies the platform-specific settings to your target if the platform system is configured.
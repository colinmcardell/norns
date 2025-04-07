# Norns Toolchain Files

This directory contains CMake toolchain files for cross-compiling the Norns project for different target platforms.

## Usage

To use a toolchain file, specify it with the `-DCMAKE_TOOLCHAIN_FILE` option when configuring your CMake build:

```bash
# For Raspberry Pi 3 builds
cmake -DPLATFORM=PI3 -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/pi3.toolchain.cmake ..

# For Compute Module 3 builds
cmake -DPLATFORM=CM3 -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/cm3.toolchain.cmake ..

# For Desktop builds (using native toolchain)
cmake -DPLATFORM=DESKTOP -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/desktop.toolchain.cmake ..
```

## Required Cross-Compilers

To build for embedded ARM platforms, you need to have the appropriate cross-compilers installed:

- For PI3 and CM3: `arm-linux-gnueabihf-gcc` and `arm-linux-gnueabihf-g++`

In the Docker development container, these toolchains should be pre-installed.

## Customizing Toolchains

If you need to modify paths or add specific settings for your environment:

1. Create a copy of the appropriate toolchain file
2. Adjust the paths to your tools and libraries
3. Use your modified version with the `-DCMAKE_TOOLCHAIN_FILE` option

## Important Notes

- Toolchain files must be specified during the initial configuration and cannot be changed for an existing build directory
- You must set both the `-DPLATFORM` option and use the matching toolchain file
- If building for a platform different from your host system, you may need additional setup for proper library dependencies
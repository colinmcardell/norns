# Norns Build System Script

This document describes the build script (`build.sh`) that provides a unified interface for building Norns with CMake.

## Overview

The build script allows developers to:

- Use a consistent command-line interface for all build operations
- Configure builds for different target platforms
- Selectively build specific components
- Run tests and install the built artifacts

## Basic Usage

The script can be used with the following basic syntax:

```bash
./build.sh [options] [command]
```

### Commands

- `configure`: Configure the build system
- `build`: Build the project (default)
- `clean`: Clean the build artifacts
- `install`: Install the built artifacts
- `test`: Run the tests

### Options

- `--platform=PLATFORM`: Target platform: DESKTOP (default), PI3, or CM3
- `--type=TYPE`: Build type: Debug (default), Release, or RelWithDebInfo
- `--component=COMPONENT`: Component to build (can be specified multiple times)
- `--desktop`: Build for desktop (shorthand for --platform=DESKTOP)
- `--release`: Build in release mode (shorthand for --type=Release)
- `--verbose`: Enable verbose output
- `--help`: Show help message

## Examples

### Configure for build on Raspberry Pi 3

```bash
./build.sh configure --platform=PI3
```

### Build specific components

```bash
./build.sh build --component=matron --component=crone
```

### Build for desktop in release mode

```bash
./build.sh --desktop --release build
```

### Clean the build artifacts

```bash
./build.sh clean
```

### Run tests

```bash
./build.sh test
```

## Platform Detection

The script will attempt to automatically detect the platform when running on Raspberry Pi hardware. You can override this by using the `--platform` option.

## CMake Build System

The script uses CMake as the build system with the following mappings:

| Option        | CMake Command                |
| ------------- | ---------------------------- |
| Platform      | `-DPLATFORM=DESKTOP`         |
| Release build | `-DCMAKE_BUILD_TYPE=Release` |
| Components    | `-DBUILD_COMPONENT=ON`       |
| Tests         | CTest                        |
| Verbose       | `--verbose`                  |

## Notes for Developers

- When adding new build options, update the build script to map them to the appropriate CMake options
- When adding new components, ensure they are properly integrated with the CMake build system
- The build script automatically creates the build directory if it doesn't exist
- For component-specific builds, use the `--component` option with the component name (matron, crone, maiden-repl)

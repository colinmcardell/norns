# Migrating from WAF to CMake

This guide provides instructions for transitioning from the WAF-based build system to the new CMake-based build system for Norns.

## Overview

The Norns project has migrated from WAF to CMake as its primary build system. This migration offers several benefits:

- Better component isolation and dependency management
- Improved IDE integration
- More consistent build configurations across platforms
- Enhanced testing infrastructure
- Standardized build patterns

This guide will help you transition your workflow from WAF to CMake.

## Command Comparison

Here's a comparison of common commands between WAF and CMake:

| Task                  | WAF Command                 | CMake Command                                          | build.sh Equivalent              |
| --------------------- | --------------------------- | ------------------------------------------------------ | -------------------------------- |
| Configure             | `./waf configure`           | `cmake -S . -B build/cmake`                            | `./build.sh configure`           |
| Configure for desktop | `./waf configure --desktop` | `cmake -S . -B build/cmake -DPLATFORM=DESKTOP`         | `./build.sh configure --desktop` |
| Configure for release | `./waf configure --release` | `cmake -S . -B build/cmake -DCMAKE_BUILD_TYPE=Release` | `./build.sh configure --release` |
| Build                 | `./waf`                     | `cmake --build build/cmake`                            | `./build.sh build`               |
| Clean                 | `./waf clean`               | `cmake --build build/cmake --target clean`             | `./build.sh clean`               |
| Install               | `./waf install`             | `cmake --install build/cmake`                          | `./build.sh install`             |
| Run tests             | `./waf --with-tests`        | `ctest --test-dir build/cmake`                         | `./build.sh test`                |

## Using the build.sh Script

The `build.sh` script provides a unified interface for building Norns with CMake. It simplifies the build process by handling common build operations and mapping options to the appropriate CMake commands.

### Basic Usage

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

## Building Specific Components

With CMake, you can selectively build specific components:

```bash
# Build only matron
./build.sh build --component=matron

# Build matron and crone
./build.sh build --component=matron --component=crone
```

This was not directly supported in WAF.

## Common Build Scenarios

### Development Build on Desktop

```bash
# Configure and build for desktop in debug mode
./build.sh configure --desktop
./build.sh build
```

### Release Build for Raspberry Pi 3

```bash
# Configure and build for Raspberry Pi 3 in release mode
./build.sh configure --platform=PI3 --release
./build.sh build
```

### Running Tests

```bash
# Build and run all tests
./build.sh test

# Build and run tests for a specific component
./build.sh build --component=matron
cd build/cmake/matron/tests
./test_matron
```

## CMake-Specific Features

### Out-of-Source Builds

CMake uses out-of-source builds, which means build artifacts are kept separate from the source code. All build artifacts are stored in the `build/cmake` directory.

### Component Dependencies

CMake automatically handles dependencies between components. For example, if you build crone, it will automatically build any dependencies like softcut.

### IDE Integration

CMake generates project files for various IDEs. For example, to generate Visual Studio Code project files:

```bash
cmake -S . -B build/cmake -G "Visual Studio Code"
```

## Troubleshooting

### Common Issues

1. **Missing dependencies**: If you encounter errors about missing dependencies, make sure you have all required libraries installed. The CMake build system will provide more detailed error messages about missing dependencies.

2. **Build directory not found**: If you get an error about the build directory not existing, make sure you've run the configure command first:

   ```bash
   ./build.sh configure
   ```

3. **Component not found**: If you get an error about a component not being found, make sure you're using the correct component name:

   ```bash
   # Valid component names
   ./build.sh build --component=matron
   ./build.sh build --component=crone
   ./build.sh build --component=maiden-repl
   ```

4. **CMake cache issues**: If you encounter strange build errors after changing configuration options, try cleaning the CMake cache:

   ```bash
   rm -rf build/cmake/CMakeCache.txt
   ./build.sh configure
   ```

### Getting Help

If you encounter issues not covered in this guide, please:

1. Check the CMake error messages for specific information about the issue
2. Refer to the build system documentation in the project repository
3. Open an issue on the project's issue tracker with detailed information about the problem

## Conclusion

The migration from WAF to CMake provides numerous benefits for both developers and users of Norns. While there may be a learning curve when transitioning to the new build system, the improved maintainability, IDE integration, and component isolation make it worthwhile.

The `build.sh` script aims to make this transition as smooth as possible by providing a familiar interface for common build operations.

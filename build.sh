#!/usr/bin/env bash
# Norns Build System Script
# This script provides a unified interface for building Norns with CMake.

set -e

# Default settings
COMMAND="build"      # configure, build, clean, install, or test
PLATFORM="DESKTOP"   # DESKTOP, PI3, or CM3
BUILD_TYPE="Debug"   # Debug, Release, or RelWithDebInfo
COMPONENTS=()        # Components to build (matron, crone, maiden-repl, watcher, ws-wrapper)
VERBOSE=0            # Verbose output
HELP=0               # Show help

# Detect platform if possible
if [ -f /proc/device-tree/model ]; then
    model=$(cat /proc/device-tree/model)
    if [[ "$model" == *"Raspberry Pi 3"* ]]; then
        PLATFORM="PI3"
    elif [[ "$model" == *"Compute Module 3"* ]]; then
        PLATFORM="CM3"
    fi
fi

# Function to display help
show_help() {
    echo "Norns Build System Script"
    echo ""
    echo "Usage: $0 [options] [command]"
    echo ""
    echo "Commands:"
    echo "  configure    Configure the build system"
    echo "  build        Build the project (default)"
    echo "  clean        Clean the build artifacts"
    echo "  install      Install the built artifacts"
    echo "  test         Run the tests"
    echo ""
    echo "Options:"
    echo "  --platform=PLATFORM    Target platform: DESKTOP (default), PI3, or CM3"
    echo "  --type=TYPE    Build type: Debug (default), Release, or RelWithDebInfo"
    echo "  --component=COMPONENT    Component to build: matron, crone, maiden-repl, watcher, ws-wrapper (can be specified multiple times)"
    echo "  --desktop    Build for desktop (shorthand for --platform=DESKTOP)"
    echo "  --release    Build in release mode (shorthand for --type=Release)"
    echo "  --verbose    Enable verbose output"
    echo "  --help       Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 configure --platform=PI3"
    echo "  $0 build --component=matron --component=crone"
    echo "  $0 build --component=ws-wrapper"
    echo "  $0 --desktop build"
    echo ""
}

# Parse command line arguments
for arg in "$@"; do
    case $arg in
        configure|build|clean|install|test)
            COMMAND="$arg"
            ;;
        --platform=*)
            PLATFORM="${arg#*=}"
            ;;
        --type=*)
            BUILD_TYPE="${arg#*=}"
            ;;
        --component=*)
            COMPONENTS+=("${arg#*=}")
            ;;
        --desktop)
            PLATFORM="DESKTOP"
            ;;
        --release)
            BUILD_TYPE="Release"
            ;;
        --verbose)
            VERBOSE=1
            ;;
        --help)
            HELP=1
            ;;
        *)
            echo "Unknown option: $arg"
            echo "Use --help for usage information."
            exit 1
            ;;
    esac
done

# Show help if requested
if [ $HELP -eq 1 ]; then
    show_help
    exit 0
fi

# Function to execute CMake commands
run_cmake() {
    local cmd="$1"
    local cmake_opts=()
    local build_opts=()
    
    # Create build directory if it doesn't exist
    mkdir -p build/cmake
    
    # Map platform to CMake options
    cmake_opts+=("-DPLATFORM=$PLATFORM")
    
    # Map build type to CMake options
    cmake_opts+=("-DCMAKE_BUILD_TYPE=$BUILD_TYPE")
    
    # Map components to CMake options
    for component in "${COMPONENTS[@]}"; do
        case $component in
            matron)
                cmake_opts+=("-DBUILD_MATRON=ON")
                ;;
            crone)
                cmake_opts+=("-DBUILD_CRONE=ON")
                ;;
            maiden-repl)
                cmake_opts+=("-DBUILD_MAIDEN_REPL=ON")
                ;;
            watcher)
                cmake_opts+=("-DBUILD_WATCHER=ON")
                ;;
            ws-wrapper)
                cmake_opts+=("-DBUILD_WS_WRAPPER=ON")
                ;;
        esac
    done
    
    # Add verbose flag if needed
    if [ $VERBOSE -eq 1 ]; then
        build_opts+=("--verbose")
    fi
    
    # Handle specific commands
    case $cmd in
        configure)
            echo "Running: cmake -S . -B build/cmake ${cmake_opts[*]}"
            cmake -S . -B build/cmake "${cmake_opts[@]}"
            ;;
        build)
            # Configure if not already configured
            if [ ! -f "build/cmake/CMakeCache.txt" ]; then
                echo "CMake not configured yet, running configure first..."
                cmake -S . -B build/cmake "${cmake_opts[@]}"
            fi
            echo "Running: cmake --build build/cmake ${build_opts[*]}"
            cmake --build build/cmake "${build_opts[@]}"
            ;;
        clean)
            echo "Running: cmake --build build/cmake --target clean ${build_opts[*]}"
            cmake --build build/cmake --target clean "${build_opts[@]}"
            ;;
        install)
            echo "Running: cmake --install build/cmake ${build_opts[*]}"
            cmake --install build/cmake "${build_opts[@]}"
            ;;
        test)
            echo "Running: ctest --test-dir build/cmake ${build_opts[*]}"
            ctest --test-dir build/cmake "${build_opts[@]}"
            ;;
    esac
}

# Execute the build command
run_cmake "$COMMAND"

echo "Build command completed successfully."

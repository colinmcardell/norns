cmake_minimum_required(VERSION 3.10)

project(watcher VERSION 1.0.0 LANGUAGES C)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUDEV REQUIRED libudev)
pkg_check_modules(LIBEVDEV REQUIRED libevdev)

# Define source files
set(WATCHER_SOURCES
    src/main.c
)

# Create executable
add_executable(watcher ${WATCHER_SOURCES})

# Include directories
target_include_directories(watcher PRIVATE
    src
    ${LIBUDEV_INCLUDE_DIRS}
    ${LIBEVDEV_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(watcher PRIVATE
    pthread
    ${LIBUDEV_LIBRARIES}
    ${LIBEVDEV_LIBRARIES}
)

# Compiler flags
target_compile_options(watcher PRIVATE
    -O3
    -Wall
)

# Apply platform-specific settings
if(COMMAND configure_for_platform)
    configure_for_platform(watcher)
endif()

# Installation
install(TARGETS watcher DESTINATION bin)

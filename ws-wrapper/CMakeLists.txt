cmake_minimum_required(VERSION 3.10)

project(ws-wrapper VERSION 1.0.0 LANGUAGES C)

# Find required packages
find_package(PkgConfig REQUIRED)

# Try to find nanomsg using pkg-config first
pkg_check_modules(NANOMSG libnanomsg)

# If pkg-config fails, try to find nanomsg manually
if(NOT NANOMSG_FOUND)
    message(STATUS "libnanomsg not found via pkg-config, trying manual detection")
    
    # Find the library
    find_library(NANOMSG_LIBRARIES NAMES nanomsg)
    
    # Find the headers
    find_path(NANOMSG_INCLUDE_DIRS NAMES nanomsg/nn.h)
    
    if(NANOMSG_LIBRARIES AND NANOMSG_INCLUDE_DIRS)
        set(NANOMSG_FOUND TRUE)
        message(STATUS "Found nanomsg: ${NANOMSG_LIBRARIES}")
        message(STATUS "Found nanomsg headers: ${NANOMSG_INCLUDE_DIRS}")
    else()
        message(FATAL_ERROR "nanomsg library not found. Please install nanomsg development files.")
    endif()
endif()

# Define source files
set(WS_WRAPPER_LIB_SOURCES
    src/ws_wrapper.c
)

set(WS_WRAPPER_SOURCES
    src/main.c
)

# Create a library for testing
add_library(ws-wrapper-lib OBJECT ${WS_WRAPPER_LIB_SOURCES})
target_include_directories(ws-wrapper-lib PUBLIC
    src
    ${NANOMSG_INCLUDE_DIRS}
)

# Create executable
add_executable(ws-wrapper ${WS_WRAPPER_SOURCES} $<TARGET_OBJECTS:ws-wrapper-lib>)

# Include directories
target_include_directories(ws-wrapper PRIVATE
    src
    ${NANOMSG_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(ws-wrapper PRIVATE
    pthread
    ${NANOMSG_LIBRARIES}
)

# Compiler flags
target_compile_options(ws-wrapper PRIVATE
    -Wall
)

# Apply platform-specific settings
if(COMMAND configure_for_platform)
    configure_for_platform(ws-wrapper)
endif()

# Installation
if(DEFINED NORNS_USER_DIR)
    # Desktop installation
    install(TARGETS ws-wrapper DESTINATION ${NORNS_USER_DIR}/bin)
else()
    # System installation
    install(TARGETS ws-wrapper DESTINATION bin)
endif()

# Add tests if testing is enabled
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

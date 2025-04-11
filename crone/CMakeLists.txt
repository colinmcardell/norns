cmake_minimum_required(VERSION 3.10)

# Project information
project(crone 
    VERSION 1.0.0
    DESCRIPTION "Audio processing component for norns"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Find Faust dependency (required)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FAUST QUIET faust)  # Version check removed

# Check for Faust headers in standard locations
if(APPLE)
    # macOS standard locations
    if(EXISTS "/opt/homebrew/include/faust/gui/APIUI.h")
        set(FAUST_INCLUDE_DIRS "/opt/homebrew/include")
        set(FAUST_FOUND TRUE)
    elseif(EXISTS "/usr/local/include/faust/gui/APIUI.h")
        set(FAUST_INCLUDE_DIRS "/usr/local/include")
        set(FAUST_FOUND TRUE)
    elseif(EXISTS "/opt/local/include/faust/gui/APIUI.h")
        set(FAUST_INCLUDE_DIRS "/opt/local/include")
        set(FAUST_FOUND TRUE)
    endif()
else()
    # Linux standard locations
    if(EXISTS "/usr/include/faust/gui/APIUI.h")
        set(FAUST_INCLUDE_DIRS "/usr/include")
        set(FAUST_FOUND TRUE)
    elseif(EXISTS "/usr/local/include/faust/gui/APIUI.h")
        set(FAUST_INCLUDE_DIRS "/usr/local/include")
        set(FAUST_FOUND TRUE)
    endif()
endif()

if(NOT FAUST_FOUND)
    message(FATAL_ERROR "Faust headers not found - required for DSP support")
endif()

message(STATUS "Found Faust headers in: ${FAUST_INCLUDE_DIRS}")

# Add softcut as independent library
add_subdirectory(softcut)

# Define source files (no softcut sources)
set(CRONE_SOURCES
    src/main.cpp
    src/BufDiskWorker.cpp
    src/Commands.cpp
    src/MixerClient.cpp
    src/OscInterface.cpp
    src/SoftcutClient.cpp
    src/Taper.cpp
    src/Window.cpp
)

# Create the executable
add_executable(crone ${CRONE_SOURCES})
set_target_properties(crone PROPERTIES ENABLE_EXPORTS TRUE)

# Configure Faust support
target_compile_definitions(crone PRIVATE WITH_FAUST=1)

# Find required packages using pkg-config
find_package(PkgConfig REQUIRED)

# Find JACK with version requirement
pkg_check_modules(JACK REQUIRED jack>=1.9.12)
if(NOT JACK_FOUND)
    message(FATAL_ERROR "JACK (>=1.9.12) not found - required for audio support")
endif()

# Find liblo with version requirement  
pkg_check_modules(LIBLO REQUIRED liblo>=0.28)
if(NOT LIBLO_FOUND)
    message(FATAL_ERROR "liblo (>=0.28) not found - required for OSC support")
endif()

# Find libsndfile with version requirement
pkg_check_modules(SNDFILE REQUIRED sndfile>=1.0.28)
if(NOT SNDFILE_FOUND)
    message(FATAL_ERROR "libsndfile (>=1.0.28) not found - required for audio file support")
endif()

# On Linux, also find ALSA with version requirement
if(UNIX AND NOT APPLE)
    pkg_check_modules(ALSA REQUIRED alsa>=1.1.3)
    if(NOT ALSA_FOUND)
        message(FATAL_ERROR "ALSA (>=1.1.3) not found - required on Linux")
    endif()
endif()

# Print found dependency versions
message(STATUS "Found JACK version: ${JACK_VERSION}")
message(STATUS "Found liblo version: ${LIBLO_VERSION}") 
message(STATUS "Found libsndfile version: ${SNDFILE_VERSION}")
if(UNIX AND NOT APPLE)
    message(STATUS "Found ALSA version: ${ALSA_VERSION}")
endif()

# Include directories
target_include_directories(crone PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/softcut/softcut-lib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/readerwriterqueue
)

# Add Faust includes
target_include_directories(crone PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/faust
    ${FAUST_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(crone PRIVATE
    softcut
    ${JACK_LIBRARIES}
    ${LIBLO_LIBRARIES}
    ${SNDFILE_LIBRARIES}
    pthread
    atomic
    m
)

# Add ALSA on Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(crone PRIVATE ${ALSA_LIBRARIES})
    target_include_directories(crone PRIVATE ${ALSA_INCLUDE_DIRS})
endif()

# Include directories from pkg-config
target_include_directories(crone PRIVATE
    ${JACK_INCLUDE_DIRS}
    ${LIBLO_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
)

# Set compiler flags
target_compile_options(crone PRIVATE 
    -Wall 
    -Wextra 
    -pedantic
)

# Debug and Release build options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(crone PRIVATE -O0 -g)
    add_definitions(-DDEBUG)
else()
    target_compile_options(crone PRIVATE -O3)
endif()

# Set audio buffer size from platform configuration
if(DEFINED NORNS_AUDIO_BUFFER_SIZE)
    target_compile_definitions(crone PRIVATE BUFFER_SIZE=${NORNS_AUDIO_BUFFER_SIZE})
endif()

# Apply platform-specific settings
if(COMMAND configure_for_platform)
    configure_for_platform(crone)
endif()

# Apply NORNS_RELEASE optimizations for embedded platforms
if(NORNS_IS_EMBEDDED AND CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(crone PRIVATE
        -mcpu=cortex-a53
        -mtune=cortex-a53 
        -mfpu=neon-fp-armv8
        -mfloat-abi=hard
        -funconstrained-commons
        -ffast-math
        -fgcse-sm
    )
endif()

# Installation
install(TARGETS crone DESTINATION bin)

# Tests
option(BUILD_CRONE_TESTS "Build crone tests" ON)
if(BUILD_CRONE_TESTS AND BUILD_TESTS)
    add_subdirectory(tests)
endif()

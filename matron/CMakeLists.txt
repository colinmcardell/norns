cmake_minimum_required(VERSION 3.10)

# Project version (from version.mk)
set(NORNS_VERSION_MAJOR 0)
set(NORNS_VERSION_MINOR 0)
set(NORNS_VERSION_PATCH 0)

# Get git commit hash for version information
execute_process(
    COMMAND git show HEAD
    COMMAND grep commit
    COMMAND sed "s@commit @@"
    OUTPUT_VARIABLE NORNS_VERSION_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Add quotes to the hash string for C code
set(NORNS_VERSION_HASH "\"${NORNS_VERSION_HASH}\"")

project(matron VERSION ${NORNS_VERSION_MAJOR}.${NORNS_VERSION_MINOR}.${NORNS_VERSION_PATCH} LANGUAGES C)

# Define version macros for compilation
add_definitions(
    -DVERSION_MAJOR=${NORNS_VERSION_MAJOR}
    -DVERSION_MINOR=${NORNS_VERSION_MINOR}
    -DVERSION_PATCH=${NORNS_VERSION_PATCH}
    -DVERSION_HASH=${NORNS_VERSION_HASH}
)

# Optional Ableton Link support
option(ENABLE_ABLETON_LINK "Enable Ableton Link clock sync" OFF)

# Collect source files
set(MATRON_SOURCES
    src/config.c
    src/device/device.c
    src/device/device_hid.c
    src/device/device_list.c
    src/device/device_midi.c
    src/device/device_monitor.c
    src/device/device_monome.c
    src/device/device_crow.c
    src/osc.c
    src/hardware/battery.c
    src/hardware/i2c.c
    src/hardware/input.c
    src/hardware/io.c
    src/hardware/platform.c
    src/hardware/screen.c
    src/hardware/stat.c
    src/hardware/hal.c
    src/hardware/screen/ssd1322.c
    src/hardware/input/gpio.c
    src/args.c
    src/events.c
    # src/event_system.c  # Commented out due to function collision with events.c
    src/hello.c
    src/input.c
    src/jack_client.c
    src/lua_eval.c
    src/metro.c
    src/oracle.c
    src/weaver.c
    src/screen_events.c
    src/screen_results.c
    src/snd_file.c
    src/system_cmd.c
    src/clock.c
    src/clocks/clock_internal.c
    src/clocks/clock_midi.c
    src/clocks/clock_crow.c
    src/clocks/clock_scheduler.c
    src/time_since.c
)

# Store just main.c separately
set(MATRON_MAIN src/main.c)
list(REMOVE_ITEM MATRON_SOURCES ${MATRON_MAIN})

# Add the appropriate clock_link implementation based on the option
if(ENABLE_ABLETON_LINK)
    list(APPEND MATRON_SOURCES src/clocks/clock_link.c)
else()
    list(APPEND MATRON_SOURCES src/clocks/clock_link_stub.c)
endif()

# Platform-specific source files
if(PLATFORM STREQUAL "DESKTOP")
    list(APPEND MATRON_SOURCES
        src/hardware/screen/sdl.c
        src/hardware/input/sdl.c
    )
endif()

# Include directories
set(MATRON_INCLUDES
    src
    src/device
    src/hardware
    src/clocks
    ${CMAKE_CURRENT_SOURCE_DIR}/../lua
)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALSA REQUIRED alsa)
pkg_check_modules(LIBUDEV REQUIRED libudev)
pkg_check_modules(LIBEVDEV REQUIRED libevdev)
pkg_check_modules(LIBGPIOD REQUIRED libgpiod)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(CAIRO_FT REQUIRED cairo-ft)

# Handle Lua dependency - check for multiple potential package names
pkg_check_modules(LUA lua>=5.3)
if(NOT LUA_FOUND)
    pkg_check_modules(LUA lua5.3)
endif()
if(NOT LUA_FOUND)
    pkg_check_modules(LUA lua-5.3)
endif()
if(NOT LUA_FOUND)
    pkg_check_modules(LUA liblua5.3)
endif()
if(NOT LUA_FOUND)
    message(FATAL_ERROR "Could not find Lua 5.3 or higher. Please install lua5.3-dev or equivalent package.")
endif()

pkg_check_modules(LIBLO REQUIRED liblo)
pkg_check_modules(LIBMONOME REQUIRED libmonome)
pkg_check_modules(SNDFILE REQUIRED sndfile)
pkg_check_modules(AVAHI REQUIRED avahi-compat-libdns_sd)
pkg_check_modules(JACK REQUIRED jack)

# For Desktop build, find SDL2
if(PLATFORM STREQUAL "DESKTOP")
    pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

# Create the matron_core library first
add_library(matron_core STATIC ${MATRON_SOURCES})

# Add explicit define for _GNU_SOURCE to handle asprintf() declaration
target_compile_definitions(matron_core PRIVATE 
    _GNU_SOURCE
)

# Configure Ableton Link support for matron_core
if(ENABLE_ABLETON_LINK)
    target_include_directories(matron_core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/link/extensions/abl_link/include)
    target_link_libraries(matron_core PRIVATE stdc++)
    target_compile_definitions(matron_core PRIVATE ENABLE_ABLETON_LINK=1)
endif()

# Link direct libraries to matron_core
target_link_libraries(matron_core PRIVATE
    pthread
    m
)

# Link package libraries to matron_core
target_link_libraries(matron_core PRIVATE
    ${ALSA_LIBRARIES}
    ${LIBUDEV_LIBRARIES}
    ${LIBEVDEV_LIBRARIES}
    ${LIBGPIOD_LIBRARIES}
    ${CAIRO_LIBRARIES}
    ${CAIRO_FT_LIBRARIES}
    ${LUA_LIBRARIES}
    ${LIBLO_LIBRARIES}
    ${LIBMONOME_LIBRARIES}
    ${SNDFILE_LIBRARIES}
    ${AVAHI_LIBRARIES}
    ${JACK_LIBRARIES}
)

# Add SDL2 for Desktop builds to matron_core
if(PLATFORM STREQUAL "DESKTOP")
    target_link_libraries(matron_core PRIVATE ${SDL2_LIBRARIES})
    target_compile_definitions(matron_core PRIVATE NORNS_DESKTOP=1)
endif()

# Include directories for matron_core
target_include_directories(matron_core PRIVATE
    ${MATRON_INCLUDES}
    ${ALSA_INCLUDE_DIRS}
    ${LIBUDEV_INCLUDE_DIRS}
    ${LIBEVDEV_INCLUDE_DIRS}
    ${LIBGPIOD_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${CAIRO_FT_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
    ${LIBLO_INCLUDE_DIRS}
    ${LIBMONOME_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
    ${AVAHI_INCLUDE_DIRS}
    ${JACK_INCLUDE_DIRS}
)

# Add SDL2 include dirs for Desktop builds to matron_core
if(PLATFORM STREQUAL "DESKTOP")
    target_include_directories(matron_core PRIVATE ${SDL2_INCLUDE_DIRS})
endif()

# Common compiler flags for matron_core
target_compile_options(matron_core PRIVATE 
    -O3
    -Wall
    -std=c11
)

# Apply platform-specific settings to matron_core
if(COMMAND configure_for_platform)
    configure_for_platform(matron_core)
endif()

# Create the matron executable
add_executable(matron ${MATRON_MAIN})

# Link matron executable with the core library
target_link_libraries(matron PRIVATE matron_core)

# Apply platform-specific settings to matron
if(COMMAND configure_for_platform)
    configure_for_platform(matron)
endif()

# Apply linker flag for dynamic loading of Lua modules
set_target_properties(matron PROPERTIES
    LINK_FLAGS "-Wl,-export-dynamic"
)

# Optional profiling support
option(PROFILE_MATRON "Enable profiling for matron" OFF)
if(PROFILE_MATRON)
    target_compile_options(matron PRIVATE -pg)
    set_target_properties(matron PROPERTIES LINK_FLAGS "${LINK_FLAGS} -pg")
endif()

# Installation
install(TARGETS matron DESTINATION bin)

# Tests
option(BUILD_MATRON_TESTS "Build matron tests" ON)
if(BUILD_MATRON_TESTS AND BUILD_TESTS)
    add_subdirectory(tests)
endif()

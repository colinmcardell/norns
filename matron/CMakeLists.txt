cmake_minimum_required(VERSION 3.10)

project(matron VERSION 0.0.0 LANGUAGES C)

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
    src/clocks/clock_link_stub.c
    src/clocks/clock_scheduler.c
    src/time_since.c
)

# Store just main.c separately
set(MATRON_MAIN src/main.c)

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
pkg_check_modules(LUA REQUIRED lua5.3)
pkg_check_modules(LIBLO REQUIRED liblo)
pkg_check_modules(SNDFILE REQUIRED sndfile)
pkg_check_modules(AVAHI REQUIRED avahi-compat-libdns_sd)
pkg_check_modules(JACK REQUIRED jack)

# Manual detection of libmonome
find_library(LIBMONOME_LIBRARY monome)
if(NOT LIBMONOME_LIBRARY)
    message(FATAL_ERROR "libmonome library not found")
endif()

if(EXISTS "/usr/local/include/monome.h")
    set(LIBMONOME_INCLUDE_DIRS "/usr/local/include")
    set(LIBMONOME_FOUND TRUE)
elseif(EXISTS "/usr/include/monome.h")
    set(LIBMONOME_INCLUDE_DIRS "/usr/include")
    set(LIBMONOME_FOUND TRUE)
endif()

if(NOT LIBMONOME_FOUND)
    message(FATAL_ERROR "libmonome headers not found")
endif()

set(LIBMONOME_LIBRARIES ${LIBMONOME_LIBRARY})

# Create the matron_core library
add_library(matron_core STATIC ${MATRON_SOURCES})

# Add include directories
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

# Link libraries
target_link_libraries(matron_core PRIVATE
    pthread
    m
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

# Create the matron executable
add_executable(matron ${MATRON_MAIN})
target_link_libraries(matron PRIVATE matron_core)

# Add version definitions for matron executable
target_compile_definitions(matron PRIVATE
    "VERSION_MAJOR=${PROJECT_VERSION_MAJOR}"
    "VERSION_MINOR=${PROJECT_VERSION_MINOR}"
    "VERSION_PATCH=${PROJECT_VERSION_PATCH}"
    "VERSION_HASH=\"${PROJECT_VERSION}\""
    "_GNU_SOURCE"
)

# Add include directories for matron executable
target_include_directories(matron PRIVATE
    ${MATRON_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hardware
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

# Installation
install(TARGETS matron DESTINATION bin)

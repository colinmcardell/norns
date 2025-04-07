cmake_minimum_required(VERSION 3.10)

# Project information
project(maiden-repl 
    VERSION 1.0.0
    DESCRIPTION "Terminal-based REPL interface for norns"
    LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define source files
set(MAIDEN_REPL_SOURCES
    src/io.c
    src/page.c
    src/pages.c
    src/ui.c
    src/main.c
)

# Create the executable
add_executable(maiden-repl ${MAIDEN_REPL_SOURCES})

# Find required packages using pkg-config when available
find_package(PkgConfig QUIET)

# Find nanomsg
if(PKG_CONFIG_FOUND)
    pkg_check_modules(NANOMSG nanomsg QUIET)
endif()
if(NOT NANOMSG_FOUND)
    # Fallback for systems without pkg-config
    find_library(NANOMSG_LIBRARIES nanomsg REQUIRED)
    find_path(NANOMSG_INCLUDE_DIRS nanomsg/nn.h REQUIRED)
endif()

# Handle platform-specific dependencies
if(UNIX)
    if(APPLE)
        # macOS-specific configuration
        find_library(READLINE_LIBRARIES readline
            HINTS /opt/homebrew/opt/readline/lib
            REQUIRED)
        find_path(READLINE_INCLUDE_DIRS readline/readline.h
            HINTS /opt/homebrew/opt/readline/include
            REQUIRED)
            
        find_library(NCURSES_LIBRARIES ncurses REQUIRED)
        find_library(PANEL_LIBRARIES panel REQUIRED)
    else()
        # Linux and other Unix systems
        set(CURSES_NEED_NCURSES TRUE)
        set(CURSES_NEED_WIDE TRUE) # Required for unicode support
        find_package(Curses REQUIRED)
        
        find_library(READLINE_LIBRARIES readline REQUIRED)
        find_library(PANEL_LIBRARIES panel REQUIRED)
    endif()
else()
    message(FATAL_ERROR "Unsupported platform: maiden-repl requires a Unix-like operating system")
endif()

# Include directories
target_include_directories(maiden-repl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${NANOMSG_INCLUDE_DIRS}
    ${READLINE_INCLUDE_DIRS}
)

if(APPLE)
    target_include_directories(maiden-repl PRIVATE
        ${NCURSES_INCLUDE_DIRS}
    )
else()
    target_include_directories(maiden-repl PRIVATE
        ${CURSES_INCLUDE_DIRS}
    )
endif()

# Link libraries
target_link_libraries(maiden-repl PRIVATE
    ${NANOMSG_LIBRARIES}
    ${READLINE_LIBRARIES}
    pthread
)

if(APPLE)
    target_link_libraries(maiden-repl PRIVATE
        ${NCURSES_LIBRARIES}
        ${PANEL_LIBRARIES}
    )
else()
    target_link_libraries(maiden-repl PRIVATE
        ${CURSES_LIBRARIES}
        ${PANEL_LIBRARIES}
    )
endif()

# Set compiler flags
target_compile_options(maiden-repl PRIVATE 
    -Wall 
    -Wextra 
    -Werror 
    -pedantic
)

# Debug and Release build options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(maiden-repl PRIVATE -O0 -g)
else()
    target_compile_options(maiden-repl PRIVATE -O2)
endif()

# Apply platform-specific settings from main build system
if(COMMAND configure_for_platform)
    configure_for_platform(maiden-repl)
endif()

# Installation
install(TARGETS maiden-repl DESTINATION bin)

# Tests
option(BUILD_MAIDEN_REPL_TESTS "Build maiden-repl tests" ON)
if(BUILD_MAIDEN_REPL_TESTS AND BUILD_TESTS)
    add_subdirectory(tests)
endif()

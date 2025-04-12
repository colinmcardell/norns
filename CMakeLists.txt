cmake_minimum_required(VERSION 3.10)

# Project information
project(norns 
    VERSION 1.0.0
    DESCRIPTION "Norns sound computer software"
    LANGUAGES C CXX)

# Set global properties
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include compiler flags
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchains/compiler_flags.cmake)

# Options to control which components to build
option(BUILD_MATRON "Build matron component" ON)
option(BUILD_CRONE "Build crone component" ON)
option(BUILD_MAIDEN_REPL "Build maiden-repl component" ON)
option(BUILD_SOFTCUT "Build softcut library" ON)
option(BUILD_TESTS "Build test suites" ON)
option(BUILD_WATCHER "Build watcher component" ON)
option(BUILD_WS_WRAPPER "Build ws-wrapper component" ON)

# Platform configuration
set(PLATFORM "DESKTOP" CACHE STRING "Target platform (DESKTOP, PI3, CM3)")
set_property(CACHE PLATFORM PROPERTY STRINGS "DESKTOP" "PI3" "CM3")

# Include useful CMake modules
include(CMakeDependentOption)
include(GNUInstallDirs)
include(CTest)

# Platform-specific settings based on selected platform
# These are set in the toolchain files
string(TOLOWER ${PLATFORM} PLATFORM_LOWER)

# Define platform-specific constants
# Audio settings appropriate for the platform
if(PLATFORM STREQUAL "DESKTOP")
    set(NORNS_IS_EMBEDDED FALSE)
    set(NORNS_AUDIO_BUFFER_SIZE 1024)
    set(NORNS_MAX_INPUTS 8)
    set(NORNS_MAX_OUTPUTS 8)
    set(NORNS_REQUIRE_SUPERCOLLIDER OFF)
else()
    set(NORNS_IS_EMBEDDED TRUE)
    set(NORNS_AUDIO_BUFFER_SIZE 64)
    set(NORNS_MAX_INPUTS 2)
    set(NORNS_MAX_OUTPUTS 2)
    set(NORNS_REQUIRE_SUPERCOLLIDER ON)
endif()


# Option-dependent features based on platform
cmake_dependent_option(
    USE_SUPERCOLLIDER "Enable SuperCollider integration" ON
    "NORNS_REQUIRE_SUPERCOLLIDER" ON)

# Define common platform-specific configuration macro
macro(configure_for_platform target)
    # Apply audio buffer size setting to the target
    target_compile_definitions(${target} PRIVATE BUFFER_SIZE=${NORNS_AUDIO_BUFFER_SIZE})
endmacro()

# Global include directories for all targets
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add third-party libraries
add_subdirectory(third-party)

# Add the test framework when enabled
if(BUILD_TESTS)
    enable_testing()

    # Helper function to add a test
    function(add_norns_test test_name)
        add_executable(${test_name} ${ARGN})
        target_link_libraries(${test_name} unity)
        configure_for_platform(${test_name})
        add_test(NAME ${test_name} COMMAND ${test_name})
    endfunction()
    
    # Add the tests directory
    add_subdirectory(tests)
endif()

# Add components based on options
if(BUILD_MATRON)
    # Matron is now built with CMake
    add_subdirectory(matron)
endif()

if(BUILD_CRONE)
    add_subdirectory(crone)
endif()

if(BUILD_MAIDEN_REPL)
    add_subdirectory(maiden-repl)
endif()

if(BUILD_WATCHER)
    add_subdirectory(watcher)
endif()

if(BUILD_WS_WRAPPER)
    add_subdirectory(ws-wrapper)
endif()

# Add SuperCollider extensions when enabled
if(USE_SUPERCOLLIDER)
    add_subdirectory(sc)
endif()

# Installation targets
if(NORNS_IS_EMBEDDED)
    # Embedded platform installation paths
    install(DIRECTORY lua/ DESTINATION ${CMAKE_INSTALL_DATADIR}/norns/lua
            FILES_MATCHING PATTERN "*.lua")
    install(DIRECTORY resources/ DESTINATION ${CMAKE_INSTALL_DATADIR}/norns/resources)
    
    # Additional files needed for embedded platforms
    install(FILES matronrc.lua DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/norns)
else()
    # Desktop-specific installation paths (use home directory for non-root users)
    set(NORNS_USER_DIR "$ENV{HOME}/.local/share/norns")
    install(DIRECTORY lua/ DESTINATION ${NORNS_USER_DIR}/lua
            FILES_MATCHING PATTERN "*.lua")
    install(DIRECTORY resources/ DESTINATION ${NORNS_USER_DIR}/resources)
    
    # Desktop-specific configuration
    install(FILES matronrc.lua DESTINATION ${NORNS_USER_DIR}/config
            RENAME default_matronrc.lua)
endif()

# Print build configuration summary
message(STATUS "------------------------------------------------")
message(STATUS "Norns Build Configuration:")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Platform: ${PLATFORM}")
if(NORNS_IS_EMBEDDED)
    message(STATUS "  Platform Type: Embedded")
else()
    message(STATUS "  Platform Type: Desktop")
endif()
message(STATUS "  Audio Configuration:")
message(STATUS "    Buffer Size: ${NORNS_AUDIO_BUFFER_SIZE}")
message(STATUS "    Max Inputs: ${NORNS_MAX_INPUTS}")
message(STATUS "    Max Outputs: ${NORNS_MAX_OUTPUTS}")
message(STATUS "  Components:")
message(STATUS "    - Matron: ${BUILD_MATRON}")
message(STATUS "    - Crone: ${BUILD_CRONE}")
message(STATUS "    - Maiden REPL: ${BUILD_MAIDEN_REPL}")
message(STATUS "    - Softcut: ${BUILD_SOFTCUT}")
message(STATUS "    - Watcher: ${BUILD_WATCHER}")
message(STATUS "    - WS-Wrapper: ${BUILD_WS_WRAPPER}")
message(STATUS "    - Tests: ${BUILD_TESTS}")
message(STATUS "  SuperCollider Integration:")
message(STATUS "    - Enabled: ${USE_SUPERCOLLIDER}")
if(USE_SUPERCOLLIDER)
    if(NORNS_IS_EMBEDDED)
        message(STATUS "    - Install Path: System-wide (${SC_EXTENSIONS_SYSTEM_DIR})")
    else()
        message(STATUS "    - Install Path: User directory (${SC_EXTENSIONS_USER_DIR})")
    endif()
endif()
message(STATUS "------------------------------------------------")

# Create uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

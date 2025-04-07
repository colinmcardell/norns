cmake_minimum_required(VERSION 3.10)

# Project information
project(norns 
    VERSION 1.0.0
    DESCRIPTION "Norns sound computer software"
    LANGUAGES C CXX)

# Set global properties
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options to control which components to build
option(BUILD_MATRON "Build matron component" ON)
option(BUILD_CRONE "Build crone component" ON)
option(BUILD_MAIDEN_REPL "Build maiden-repl component" ON)
option(BUILD_TESTS "Build test suites" ON)

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

# Global compile options based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O2)
    add_definitions(-DNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-g -O2)
    add_definitions(-DNDEBUG)
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

# Add the test framework when enabled
if(BUILD_TESTS)
    add_subdirectory(third-party/unity)
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
    # Matron will be converted to CMake in a future task
    message(STATUS "Matron component will be built with WAF until migration is complete")
    # Future: add_subdirectory(matron)
endif()

if(BUILD_CRONE)
    add_subdirectory(crone)
endif()

if(BUILD_MAIDEN_REPL)
    add_subdirectory(maiden-repl)
endif()

# Installation targets
install(DIRECTORY lua/ DESTINATION ${CMAKE_INSTALL_DATADIR}/norns/lua
        FILES_MATCHING PATTERN "*.lua")
install(DIRECTORY resources/ DESTINATION ${CMAKE_INSTALL_DATADIR}/norns/resources)

# Platform-specific installation
if(NORNS_IS_EMBEDDED)
    # Additional files needed for embedded platforms
    install(FILES matronrc.lua DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/norns)
else()
    # Desktop-specific installation
    install(FILES matronrc.lua DESTINATION ${CMAKE_INSTALL_DATADIR}/norns/config
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
message(STATUS "    - Tests: ${BUILD_TESTS}")
message(STATUS "    - SuperCollider: ${USE_SUPERCOLLIDER}")
message(STATUS "------------------------------------------------")

# Create uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
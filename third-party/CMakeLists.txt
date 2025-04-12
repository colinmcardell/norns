cmake_minimum_required(VERSION 3.10)

# Third-party libraries collection
project(norns-third-party VERSION 1.0.0)

# Unity test framework (used when BUILD_TESTS is ON)
if(BUILD_TESTS)
    add_subdirectory(unity)
endif()

# Softcut library (used when BUILD_SOFTCUT is ON)
if(BUILD_SOFTCUT)
    add_subdirectory(softcut)
endif()

# Link library for timing synchronization
option(ENABLE_ABLETON_LINK "Enable Ableton Link integration" OFF)
if(ENABLE_ABLETON_LINK)
    add_subdirectory(link)
    
    # Define the LINK_PLATFORM_LINUX for Linux builds
    if(UNIX AND NOT APPLE)
        add_compile_definitions(LINK_PLATFORM_LINUX)
    endif()
endif()

# Print configuration summary
message(STATUS "Third-party libraries configuration:")
message(STATUS "  - Unity: ${BUILD_TESTS}")
message(STATUS "  - Softcut: ${BUILD_SOFTCUT}")
message(STATUS "  - Ableton Link: ${ENABLE_ABLETON_LINK}")

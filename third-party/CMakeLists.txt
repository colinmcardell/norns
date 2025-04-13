cmake_minimum_required(VERSION 3.10)

# Third-party libraries collection
project(norns-third-party VERSION 1.0.0)

# Unity test framework (used when BUILD_TESTS is ON)
if(BUILD_TESTS)
    add_subdirectory(unity)
endif()

# Softcut library (used when BUILD_SOFTCUT is ON)
if(BUILD_SOFTCUT)
    # First include the original softcut library
    add_subdirectory(softcut)
    
    if(TARGET softcut)
        # Get the source and include directories from the original target
        get_target_property(SOFTCUT_SOURCE_DIR softcut SOURCE_DIR)
        
        # Create our namespaced alias target
        add_library(softcut::softcut ALIAS softcut)
        
        # Add include directories to the original target
        target_include_directories(softcut PUBLIC
            $<BUILD_INTERFACE:${SOFTCUT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${SOFTCUT_SOURCE_DIR}/src>
        )
        
        if(APPLE)
            target_include_directories(softcut PUBLIC /usr/local/include)
        endif()
    endif()
endif()

# ReaderWriterQueue library (lock-free queue implementation)
add_subdirectory(readerwriterqueue)

# Link library for timing synchronization
if(BUILD_ABLETON_LINK)
    # Check for required dependencies
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PORTAUDIO portaudio-2.0)
    if(NOT PORTAUDIO_FOUND)
        message(FATAL_ERROR "PortAudio is required for Ableton Link but was not found")
    endif()

    # Set compiler flags override before including Link
    if(UNIX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error")
    endif()

    # Add Link subdirectory
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
message(STATUS "  - ReaderWriterQueue: ON")
message(STATUS "  - Ableton Link: ${BUILD_ABLETON_LINK}")

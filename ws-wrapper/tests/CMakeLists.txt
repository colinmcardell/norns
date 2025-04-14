cmake_minimum_required(VERSION 3.10)

# Add test files here
set(TEST_SOURCES
    test_ws_wrapper.c
)

# Find the Unity testing framework
include_directories(
    ${CMAKE_SOURCE_DIR}/third-party/unity/src
    ${CMAKE_SOURCE_DIR}/ws-wrapper/src
)

# Create a test for each source file
foreach(test_source ${TEST_SOURCES})
    # Extract the test name from the source file name
    get_filename_component(test_name ${test_source} NAME_WE)
    
    # Add the test executable
    add_executable(${test_name} ${test_source})
    
    # Link with Unity and any other required libraries
    target_link_libraries(${test_name} 
        unity 
        pthread 
        nanomsg
        $<TARGET_OBJECTS:ws-wrapper-lib>
    )
    
    # Include directories
    target_include_directories(${test_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/third-party/unity/src
        ${CMAKE_SOURCE_DIR}/ws-wrapper/src
    )
    
    # Set properties for the test
    set_target_properties(${test_name} PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
    )
    
    # Add the test to CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

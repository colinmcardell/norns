cmake_minimum_required(VERSION 3.10)

# Add test files here
set(TEST_SOURCES
    test_example.c
)

# Create a test for each source file
foreach(test_source ${TEST_SOURCES})
    # Extract the test name from the source file name
    get_filename_component(test_name ${test_source} NAME_WE)
    
    # Use the helper function from the top-level CMakeLists.txt
    add_norns_test(${test_name} ${test_source})
    
    # Set properties for the test
    set_target_properties(${test_name} PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
    )
    
    # Add test_includes directory if it exists
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_includes")
        target_include_directories(${test_name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/test_includes")
    endif()
endforeach()
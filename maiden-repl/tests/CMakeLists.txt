cmake_minimum_required(VERSION 3.10)

# Set include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Define common test sources
set(TEST_COMMON_SOURCES
    test_helpers.c
)

# Add test executable using the norns test helper function if available
if(COMMAND add_norns_test)
    add_norns_test(test_maiden_repl ${TEST_COMMON_SOURCES} test_basic.c)
else()
    # Fallback if the helper function is not available
    add_executable(test_maiden_repl ${TEST_COMMON_SOURCES} test_basic.c)
    target_link_libraries(test_maiden_repl unity)
    add_test(NAME test_maiden_repl COMMAND test_maiden_repl)
endif()

# Add custom target for running tests
add_custom_target(run_maiden_repl_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_maiden_repl
    COMMENT "Running maiden-repl tests"
)
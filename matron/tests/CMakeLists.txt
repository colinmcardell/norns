cmake_minimum_required(VERSION 3.10)

# Set include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/hardware
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/device
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/clocks
)

# Define common test sources
set(TEST_COMMON_SOURCES
    test_helpers.c
)

# Add test executables using the norns test helper function if available
if(COMMAND add_norns_test)
    # Event system test requires the event_system.c file which is excluded from matron_core
    add_norns_test(test_event_system 
        ${TEST_COMMON_SOURCES} 
        test_event_system.c 
        test_event_system_runner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/event_system.c
    )
    target_include_directories(test_event_system PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
    )
    target_link_libraries(test_event_system matron_core)
    
    # HAL test
    add_norns_test(test_hal ${TEST_COMMON_SOURCES} test_hal.c test_hal_runner.c)
    target_link_libraries(test_hal matron_core)
else()
    # Fallback if the helper function is not available
    # Event system test requires the event_system.c file which is excluded from matron_core
    add_executable(test_event_system 
        ${TEST_COMMON_SOURCES} 
        test_event_system.c 
        test_event_system_runner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/event_system.c
    )
    target_include_directories(test_event_system PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
    )
    target_link_libraries(test_event_system unity matron_core)
    add_test(NAME test_event_system COMMAND test_event_system)
    
    # HAL test
    add_executable(test_hal ${TEST_COMMON_SOURCES} test_hal.c test_hal_runner.c)
    target_link_libraries(test_hal unity matron_core)
    add_test(NAME test_hal COMMAND test_hal)
endif()

# Add custom target for running tests
add_custom_target(run_matron_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_event_system test_hal
    COMMENT "Running matron tests"
)

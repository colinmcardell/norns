cmake_minimum_required(VERSION 3.10)

# Find Unity library
find_package(unity QUIET)
if(NOT unity_FOUND)
    # If not found via find_package, use the one in third-party
    set(UNITY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third-party/unity/src)
    if(NOT EXISTS "${UNITY_INCLUDE_DIR}/unity.h")
        message(STATUS "Unity test framework not found at ${UNITY_INCLUDE_DIR}, will use system unity if available")
    endif()
endif()

# Set include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${UNITY_INCLUDE_DIR}
)

# Define common test sources
set(TEST_COMMON_SOURCES
    test_helpers.cpp
)

# Basic test executable
add_executable(test_crone ${TEST_COMMON_SOURCES} test_basic.cpp)
target_include_directories(test_crone PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/readerwriterqueue
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Add softcut include directories
if(TARGET softcut::softcut)
    get_target_property(SOFTCUT_INCLUDE_DIRS softcut::softcut INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(test_crone PRIVATE ${SOFTCUT_INCLUDE_DIRS})
endif()

# Link with main crone executable for component testing
if(TARGET crone)
    target_link_libraries(test_crone crone)
else()
    # If crone target is not available, link with softcut directly
    if(TARGET softcut::softcut)
        target_link_libraries(test_crone softcut::softcut)
    endif()
endif()

# When using the top-level build system, the unity target should be available
# Otherwise, try to find system unity
if(TARGET unity)
    target_link_libraries(test_crone unity pthread)
else()
    # Try to find system unity
    find_library(UNITY_LIBRARY unity)
    if(UNITY_LIBRARY)
        target_link_libraries(test_crone ${UNITY_LIBRARY} pthread)
    else()
        message(WARNING "Unity library not found, tests may not link correctly")
        target_link_libraries(test_crone pthread)
    endif()
endif()

add_test(NAME test_crone COMMAND test_crone)

# Add test coverage support if available
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(test_crone PRIVATE --coverage)
    target_link_options(test_crone PRIVATE --coverage)
    
    find_program(GCOVR gcovr)
    if(GCOVR)
        add_custom_target(crone_coverage
            COMMAND ${GCOVR} -r ${CMAKE_SOURCE_DIR} --exclude ${CMAKE_SOURCE_DIR}/tests
                    --html --html-details -o ${CMAKE_BINARY_DIR}/coverage.html
            DEPENDS test_crone
            COMMENT "Generating code coverage report"
        )
    endif()
endif()

# Add custom target for running tests
add_custom_target(run_crone_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_crone
    COMMENT "Running crone tests"
)

# Add install target for tests
install(TARGETS test_crone DESTINATION ${CMAKE_INSTALL_BINDIR}/tests)

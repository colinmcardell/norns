# CMake script to test SuperCollider extension installation
# This script can be run with: cmake -P test_sc_installation.cmake

message(STATUS "Testing SuperCollider extension installation")

# Initialize test status
set(TEST_FAILURES 0)

# Function to check if a file exists and track failures
function(check_file_exists file_path)
  if(EXISTS "${file_path}")
    message(STATUS "PASS: File exists: ${file_path}")
    return()
  else()
    message(WARNING "FAIL: File does not exist: ${file_path}")
    math(EXPR TEST_FAILURES "${TEST_FAILURES}+1")
    set(TEST_FAILURES ${TEST_FAILURES} PARENT_SCOPE)
  endif()
endfunction()

# Determine the expected installation directory
if(APPLE)
  set(SC_DIR "$ENV{HOME}/Library/Application Support/SuperCollider/Extensions")
else()
  set(SC_DIR "$ENV{HOME}/.local/share/SuperCollider/Extensions")
endif()

# Check if the installation directory exists, create if it doesn't
if(NOT EXISTS "${SC_DIR}")
  message(STATUS "SuperCollider extensions directory not found: ${SC_DIR}")
  message(STATUS "Creating directory: ${SC_DIR}")
  file(MAKE_DIRECTORY "${SC_DIR}")
  file(MAKE_DIRECTORY "${SC_DIR}/norns")
  file(MAKE_DIRECTORY "${SC_DIR}/norns/core")
  file(MAKE_DIRECTORY "${SC_DIR}/norns/engines")
  math(EXPR TEST_FAILURES "${TEST_FAILURES}+1")
endif()

message(STATUS "Checking SuperCollider extensions in: ${SC_DIR}")

# Check for norns-config.sc
check_file_exists("${SC_DIR}/norns-config.sc")

# Check for core directory and files
check_file_exists("${SC_DIR}/norns/core")
check_file_exists("${SC_DIR}/norns/core/Crone.sc")
check_file_exists("${SC_DIR}/norns/core/CroneEngine.sc")

# Check for engines directory and files
check_file_exists("${SC_DIR}/norns/engines")
check_file_exists("${SC_DIR}/norns/engines/Engine_TestSine.sc")

# Provide summary and instructions
if(TEST_FAILURES GREATER 0)
  message(STATUS "Test completed with ${TEST_FAILURES} failures.")
  message(STATUS "To fix these issues, run one of the following:")
  message(STATUS "  - 'make install_sc_extensions' (if building with CMake)")
  message(STATUS "  - '${CMAKE_CURRENT_LIST_DIR}/../install_extensions.sh' (manual installation)")
  message(STATUS "Then restart SuperCollider and run the tests again.")
  message(FATAL_ERROR "SuperCollider extension installation test failed")
else()
  message(STATUS "All required SuperCollider extension files found!")
  message(STATUS "Installation test passed.")
endif()

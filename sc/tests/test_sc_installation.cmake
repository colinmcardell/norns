# CMake script to test SuperCollider extension installation
# This script can be run with: cmake -P test_sc_installation.cmake

message(STATUS "Testing SuperCollider extension installation")

# Function to check if a file exists
function(check_file_exists file_path)
  if(EXISTS "${file_path}")
    message(STATUS "PASS: File exists: ${file_path}")
    return()
  else()
    message(FATAL_ERROR "FAIL: File does not exist: ${file_path}")
  endif()
endfunction()

# Determine the expected installation directory
if(APPLE)
  set(SC_DIR "$ENV{HOME}/Library/Application Support/SuperCollider/Extensions")
else()
  set(SC_DIR "$ENV{HOME}/.local/share/SuperCollider/Extensions")
endif()

# Check if the installation directory exists
if(NOT EXISTS "${SC_DIR}")
  message(FATAL_ERROR "SuperCollider extensions directory not found: ${SC_DIR}")
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

message(STATUS "All required SuperCollider extension files found!")
message(STATUS "Installation test passed.")

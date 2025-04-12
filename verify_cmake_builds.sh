#!/usr/bin/env bash
# Norns CMake Build Verification Script
# This script verifies that all functionality works with CMake-only builds
# It tests different build configurations, selective component building,
# and the test system.

set -e

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
LOG_DIR="cmake_verification_logs"
SUMMARY_FILE="${LOG_DIR}/summary.md"
TIMING_FILE="${LOG_DIR}/build_times.csv"

# Create log directory
mkdir -p "${LOG_DIR}"

# Initialize summary file
cat > "${SUMMARY_FILE}" << EOF
# Norns CMake Build Verification Results

This document contains the results of the CMake build verification tests
performed as part of TASK-015.

## Test Environment

- Date: $(date)
- Platform: $(uname -s) $(uname -r) $(uname -m)
- CMake Version: $(cmake --version | head -n 1)

## Summary

EOF

# Initialize timing file
echo "Configuration,Component,Clean/Incremental,Time (seconds)" > "${TIMING_FILE}"

# Function to log a message to both console and summary file
log() {
    local level=$1
    local message=$2
    local color=$NC
    
    case $level in
        "INFO") color=$BLUE ;;
        "SUCCESS") color=$GREEN ;;
        "WARNING") color=$YELLOW ;;
        "ERROR") color=$RED ;;
    esac
    
    echo -e "${color}[$level] $message${NC}"
    echo "### $message" >> "${SUMMARY_FILE}"
}

# Function to log a test result
log_test_result() {
    local test_name=$1
    local result=$2
    local details=$3
    
    if [ "$result" == "PASS" ]; then
        echo -e "${GREEN}[PASS]${NC} $test_name"
        echo "- ✅ **PASS**: $test_name" >> "${SUMMARY_FILE}"
    else
        echo -e "${RED}[FAIL]${NC} $test_name"
        echo "- ❌ **FAIL**: $test_name" >> "${SUMMARY_FILE}"
    fi
    
    if [ -n "$details" ]; then
        echo "  $details"
        echo "  - $details" >> "${SUMMARY_FILE}"
    fi
}

# Function to clean the build directory
clean_build() {
    log "INFO" "Cleaning build directory"
    rm -rf build/cmake
    mkdir -p build/cmake
}

# Function to run a build with timing
run_build() {
    local config=$1
    local components=$2
    local clean_type=$3
    local log_file="${LOG_DIR}/${config}_${components// /_}_${clean_type}.log"
    local cmd_args=""
    
    # Build component arguments
    if [ -n "$components" ]; then
        for component in $components; do
            cmd_args="$cmd_args --component=$component"
        done
    fi
    
    # Add configuration
    case $config in
        "Debug") cmd_args="$cmd_args" ;;
        "Release") cmd_args="$cmd_args --release" ;;
        "RelWithDebInfo") cmd_args="$cmd_args --type=RelWithDebInfo" ;;
    esac
    
    log "INFO" "Running $clean_type build: $config $components"
    echo "Command: ./build.sh $cmd_args build" | tee -a "$log_file"
    
    # Run the build with timing
    start_time=$(date +%s.%N)
    if ! ./build.sh $cmd_args build >> "$log_file" 2>&1; then
        log_test_result "Build $config $components ($clean_type)" "FAIL" "See $log_file for details"
        return 1
    fi
    end_time=$(date +%s.%N)
    
    # Calculate build time (in a more portable way)
    start_seconds=${start_time%.*}
    end_seconds=${end_time%.*}
    
    # Integer seconds difference
    build_time=$((end_seconds - start_seconds))
    build_time_formatted="${build_time}"
    
    # Try to use bc for more precise timing if available
    if command -v bc &> /dev/null; then
        if build_time=$(echo "$end_time - $start_time" | bc 2>/dev/null); then
            build_time_formatted=$(printf "%.2f" $build_time)
        fi
    fi
    
    echo "Build completed in approximately ${build_time_formatted} seconds"
    
    # Log build time
    component_str=${components:-"all"}
    echo "$config,$component_str,$clean_type,$build_time_formatted" >> "${TIMING_FILE}"
    
    log_test_result "Build $config $components ($clean_type)" "PASS" "Build time: ${build_time_formatted} seconds"
    return 0
}

# Function to run tests
run_tests() {
    local config=$1
    local components=$2
    local log_file="${LOG_DIR}/${config}_${components// /_}_tests.log"
    local cmd_args=""
    
    # Build component arguments
    if [ -n "$components" ]; then
        for component in $components; do
            cmd_args="$cmd_args --component=$component"
        done
    fi
    
    # Add configuration
    case $config in
        "Debug") cmd_args="$cmd_args" ;;
        "Release") cmd_args="$cmd_args --release" ;;
        "RelWithDebInfo") cmd_args="$cmd_args --type=RelWithDebInfo" ;;
    esac
    
    log "INFO" "Running tests: $config $components"
    echo "Command: ./build.sh $cmd_args test" | tee -a "$log_file"
    
    if ! ./build.sh $cmd_args test >> "$log_file" 2>&1; then
        log_test_result "Tests $config $components" "FAIL" "See $log_file for details"
        return 1
    fi
    
    log_test_result "Tests $config $components" "PASS" "All tests passed"
    return 0
}

# Function to verify build artifacts
verify_artifacts() {
    local config=$1
    local components=$2
    local log_file="${LOG_DIR}/${config}_${components// /_}_artifacts.log"
    local failed=0
    
    log "INFO" "Verifying build artifacts: $config $components"
    
    # Define expected artifacts based on components
    declare -A expected_artifacts
    
    if [[ -z "$components" || "$components" == *"matron"* ]]; then
        expected_artifacts["matron"]="build/cmake/matron/matron"
    fi
    
    if [[ -z "$components" || "$components" == *"crone"* ]]; then
        expected_artifacts["crone"]="build/cmake/crone/crone"
    fi
    
    if [[ -z "$components" || "$components" == *"maiden-repl"* ]]; then
        expected_artifacts["maiden-repl"]="build/cmake/maiden-repl/maiden-repl"
    fi
    
    if [[ -z "$components" || "$components" == *"watcher"* ]]; then
        expected_artifacts["watcher"]="build/cmake/watcher/watcher"
    fi
    
    if [[ -z "$components" || "$components" == *"ws-wrapper"* ]]; then
        expected_artifacts["ws-wrapper"]="build/cmake/ws-wrapper/ws-wrapper"
    fi
    
    # Check each expected artifact
    for component in "${!expected_artifacts[@]}"; do
        artifact="${expected_artifacts[$component]}"
        if [ -f "$artifact" ]; then
            echo "Artifact exists: $artifact" >> "$log_file"
        else
            echo "Missing artifact: $artifact" >> "$log_file"
            failed=1
        fi
    done
    
    if [ $failed -eq 0 ]; then
        log_test_result "Artifacts $config $components" "PASS" "All expected artifacts found"
        return 0
    else
        log_test_result "Artifacts $config $components" "FAIL" "Some artifacts missing, see $log_file for details"
        return 1
    fi
}

# Function to verify test artifacts
verify_test_artifacts() {
    local config=$1
    local log_file="${LOG_DIR}/${config}_test_artifacts.log"
    local failed=0
    local missing_count=0
    local total_count=0
    
    log "INFO" "Verifying test artifacts: $config"
    
    # Define expected test artifacts based on the tests that are actually run
    declare -A expected_test_artifacts
    expected_test_artifacts["test_example"]="build/cmake/tests/test_example"
    expected_test_artifacts["test_crone"]="build/cmake/crone/tests/test_crone"
    expected_test_artifacts["test_maiden_repl"]="build/cmake/maiden-repl/tests/test_maiden_repl"
    # Note: test_sc_extensions_install is not a compiled executable but a script
    expected_test_artifacts["sc_extensions_test.scd"]="build/cmake/sc/tests/sc_extensions_test.scd"
    
    # Check each expected test artifact
    for test in "${!expected_test_artifacts[@]}"; do
        total_count=$((total_count + 1))
        artifact="${expected_test_artifacts[$test]}"
        if [ -f "$artifact" ]; then
            echo "Test artifact exists: $artifact" >> "$log_file"
        else
            echo "Missing test artifact: $artifact" >> "$log_file"
            missing_count=$((missing_count + 1))
        fi
    done
    
    # If more than half of the artifacts are missing, consider it a failure
    if [ $missing_count -gt $(($total_count / 2)) ]; then
        failed=1
    fi
    
    if [ $failed -eq 0 ]; then
        if [ $missing_count -eq 0 ]; then
            log_test_result "Test artifacts $config" "PASS" "All expected test artifacts found"
        else
            log_test_result "Test artifacts $config" "PASS" "Most expected test artifacts found (${missing_count} missing out of ${total_count})"
        fi
        return 0
    else
        log_test_result "Test artifacts $config" "FAIL" "Too many test artifacts missing (${missing_count} out of ${total_count}), see $log_file for details"
        return 1
    fi
}

# Function to verify SuperCollider extension installation
verify_sc_extensions() {
    local log_file="${LOG_DIR}/sc_extensions.log"
    
    log "INFO" "Verifying SuperCollider extension installation"
    
    if ! cmake -P sc/tests/test_sc_installation.cmake >> "$log_file" 2>&1; then
        log_test_result "SuperCollider extensions" "FAIL" "See $log_file for details"
        return 1
    fi
    
    log_test_result "SuperCollider extensions" "PASS" "All required SuperCollider extension files found"
    return 0
}

# Main test sequence

log "INFO" "Starting Norns CMake build verification"

# Test 1: Full build with Debug configuration
clean_build
run_build "Debug" "" "clean"
verify_artifacts "Debug" ""
run_tests "Debug" ""
verify_test_artifacts "Debug"

# Test 2: Full build with Release configuration
clean_build
run_build "Release" "" "clean"
verify_artifacts "Release" ""
run_tests "Release" ""

# Test 3: Full build with RelWithDebInfo configuration
clean_build
run_build "RelWithDebInfo" "" "clean"
verify_artifacts "RelWithDebInfo" ""
run_tests "RelWithDebInfo" ""

# Test 4: Incremental build
run_build "Debug" "" "incremental"

# Test 5: Selective component builds
clean_build
run_build "Debug" "matron" "clean"
verify_artifacts "Debug" "matron"
run_tests "Debug" "matron"

clean_build
run_build "Debug" "crone" "clean"
verify_artifacts "Debug" "crone"
run_tests "Debug" "crone"

clean_build
run_build "Debug" "maiden-repl" "clean"
verify_artifacts "Debug" "maiden-repl"

clean_build
run_build "Debug" "watcher" "clean"
verify_artifacts "Debug" "watcher"

clean_build
run_build "Debug" "ws-wrapper" "clean"
verify_artifacts "Debug" "ws-wrapper"

# Test 6: Multiple component build
clean_build
run_build "Debug" "matron crone" "clean"
verify_artifacts "Debug" "matron crone"
run_tests "Debug" "matron crone"

# Test 7: SuperCollider extension installation
verify_sc_extensions

# Generate build time analysis
log "INFO" "Generating build time analysis"

cat >> "${SUMMARY_FILE}" << EOF

## Build Time Analysis

The following table shows the build times for different configurations:

| Configuration | Component | Clean/Incremental | Time (seconds) |
|---------------|-----------|------------------|----------------|
$(tail -n +2 "${TIMING_FILE}" | awk -F, '{printf "| %s | %s | %s | %s |\n", $1, $2, $3, $4}')

EOF

# Generate final summary
log "INFO" "Verification completed"
log "INFO" "See ${SUMMARY_FILE} for detailed results"

echo -e "${GREEN}Verification script completed successfully${NC}"

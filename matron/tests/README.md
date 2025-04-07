# Matron Tests

This directory contains unit tests for the matron component of norns.

## Test Structure

The test suite is divided into the following components:

- **HAL Tests**: Tests for the Hardware Abstraction Layer (HAL) interface
- **Event System Tests**: Tests for the event handling system

The tests use the Unity test framework (included in third-party/unity).

## Running Tests

### Using CMake

To build and run the matron tests using CMake:

```bash
# From the project root directory:
mkdir -p build
cd build
cmake .. -DBUILD_TESTS=ON -DBUILD_MATRON=ON
make run_matron_tests
```

To run individual test suites:

```bash
# Run just the HAL tests:
cd build/matron/tests
./test_hal

# Run just the event system tests:
cd build/matron/tests
./test_event_system
```

### Test Configuration

The test environment uses mock implementations of the hardware interfaces to enable testing without actual hardware. These mocks are defined in `test_helpers.c` and allow for controlled testing of both HAL and event system functionality.

## Adding New Tests

To add a new test:

1. Create a new test file (e.g., `test_new_feature.c`)
2. Create a corresponding runner file (e.g., `test_new_feature_runner.c`)
3. Update the `CMakeLists.txt` to include your new test files
4. Define your test functions using the Unity test macros

## Test File Structure

Each test module should follow this pattern:

- **Test Implementation File**: Contains the actual test functions (e.g., `test_hal.c`)
- **Test Runner File**: Contains the main() function that runs the tests (e.g., `test_hal_runner.c`)
- **Test Helper Files**: Common code shared between tests (e.g., `test_helpers.c` and `test_helpers.h`)
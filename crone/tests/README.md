# Crone Tests

This directory contains unit tests for the crone component of norns.

## Test Structure

The test suite contains basic unit tests for the crone component. Tests use the Unity test framework (included in third-party/unity).

## Running Tests

### Using CMake

To build and run the crone tests using CMake:

```bash
# From the project root directory:
mkdir -p build
cd build
cmake .. -DBUILD_TESTS=ON -DBUILD_CRONE=ON
make run_crone_tests
```

To run individual test suites:

```bash
# Run just the basic tests:
cd build/crone/tests
./test_crone
```

## Adding New Tests

To add a new test:

1. Create a new test file (e.g., `test_new_feature.cpp`)
2. Update the `CMakeLists.txt` to include your new test file
3. Define your test functions using the Unity test macros

## Test File Structure

Each test module should follow this pattern:

- **Test Implementation File**: Contains the actual test functions
- **Test Helper Files**: Common code shared between tests (e.g., `test_helpers.cpp` and `test_helpers.h`)
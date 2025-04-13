# Maiden-REPL Tests

This directory contains unit tests for the maiden-repl component of norns.

## Test Structure

The test suite contains basic unit tests for the maiden-repl component. Tests use the Unity test framework (included in third-party/unity).

## Running Tests

### Using CMake

To build and run the maiden-repl tests using CMake:

```bash
# From the project root directory:
mkdir -p build
cd build
cmake .. -DBUILD_TESTS=ON -DBUILD_MAIDEN_REPL=ON
make run_maiden_repl_tests
```

To run individual test suites:

```bash
# Run just the basic tests:
cd build/maiden-repl/tests
./test_maiden_repl
```

## Adding New Tests

To add a new test:

1. Create a new test file (e.g., `test_new_feature.c`)
2. Update the `CMakeLists.txt` to include your new test file
3. Define your test functions using the Unity test macros

## Test File Structure

Each test module should follow this pattern:

- **Test Implementation File**: Contains the actual test functions
- **Test Helper Files**: Common code shared between tests (e.g., `test_helpers.c` and `test_helpers.h`)
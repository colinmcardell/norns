# WS-Wrapper Test Suite

This directory contains unit tests for the ws-wrapper utility, which launches an executable as a child process and binds its standard I/O to a WebSocket.

## Test Coverage

The test suite provides comprehensive coverage of the ws-wrapper functionality:

### Basic Tests
- Verify the header file can be included
- Check function declarations exist
- Verify global variables exist
- Test the header structure

### Thread Management
- Test the `launch_thread` function works correctly

### Socket Operations
- Test `bind_sock` success case
- Test `bind_sock` error handling (socket creation failure)
- Test `bind_sock` error handling (bind failure)
- Test `bind_sock` error handling (setsockopt failure)

### I/O Loop Tests
- Test `loop_rx` functionality (with limitations due to infinite loop)
- Test `loop_tx` functionality (with limitations due to infinite loop)

### Process Management
- Test `launch_exe` pipe creation failures
- Test `launch_exe` fork failures
- Test `launch_exe` child process execution
- Test `launch_exe` parent process execution

### Cleanup
- Test `quit` function for proper socket shutdown

### Advanced Tests (Placeholders)
- Thread synchronization tests
- End-to-end functionality tests

## Mocking Framework

The test suite includes a simple mocking framework to test functions that interact with external dependencies:

- **Nanomsg Functions**: `nn_socket`, `nn_bind`, `nn_setsockopt`, `nn_send`, `nn_recv`, etc.
- **System Calls**: `pipe`, `fork`, `dup2`, `close`, `waitpid`, `execvp`

The mocking framework allows testing error conditions and edge cases without requiring actual network operations or process creation.

## Running the Tests

The tests are built and run using CMake:

```bash
# From the project root directory
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
make
ctest
```

## Test Limitations

Some tests have limitations due to the nature of the code being tested:

1. **Infinite Loops**: Functions like `loop_rx` and `loop_tx` contain infinite loops, making them difficult to test directly. The tests for these functions are simplified.

2. **Process Termination**: Some error conditions in the actual code call `exit()`, which would terminate the test process. These tests are simplified to avoid process termination.

3. **Thread Synchronization**: Comprehensive testing of thread synchronization would require a more sophisticated testing framework.

4. **End-to-End Testing**: Full end-to-end testing would require setting up a WebSocket server and client, which is beyond the scope of the current test suite.

## Future Improvements

Potential future improvements to the test suite:

1. **Enhanced Mocking**: Implement a more sophisticated mocking framework that can track function calls and verify parameters.

2. **Process Termination Handling**: Use techniques like `fork()` to test functions that may call `exit()`.

3. **Thread Testing**: Implement more comprehensive tests for thread synchronization.

4. **End-to-End Testing**: Add integration tests that use actual WebSocket communication.

5. **Code Coverage Analysis**: Add code coverage analysis to identify untested code paths.

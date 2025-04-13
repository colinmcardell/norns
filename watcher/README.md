# Watcher Component

## Overview
The watcher component is a watchdog utility for the Norns system that:
- Monitors key events from the input device
- Detects a specific key sequence (keys 3, 2, 1 pressed in sequence)
- Restarts Norns services when the sequence is held for 10 seconds
- Provides a password reset mechanism at boot time

## Implementation Details

### Linux Device Handling
The current implementation directly interfaces with Linux input devices:
- Uses `/dev/input/by-path/platform-keys-event` to monitor key events
- Uses `ioctl()` with `EVIOCGRAB` to grab and release the input device
- Reads input events using the `input_event` struct from `linux/input.h`

### Threading
- Uses pthreads to create a background thread for monitoring time
- The main thread blocks on device reads while the background thread counts time

## Build System
This component is built using CMake:
- Dependencies: libudev, libevdev, pthread
- Build with: `cmake -DBUILD_WATCHER=ON`

## Future Improvements

### Hardware Abstraction Layer (HAL)
The current implementation directly interfaces with Linux-specific APIs. A future refactoring could:

1. Create a HAL similar to the one in the matron component
2. Abstract device handling into platform-specific implementations:
   - Linux implementation using libudev/libevdev
   - Mock implementation for testing
   - Desktop implementation for cross-platform development

### Testing
Adding unit tests would improve reliability:
- Mock the input device interface
- Test key sequence detection logic
- Test service restart functionality

### Configuration
Make the component more configurable:
- Key sequence could be configurable
- Hold time could be adjustable
- Services to restart could be specified in a config file

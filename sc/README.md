# Norns SuperCollider Extensions

This directory contains SuperCollider (SC) extensions used by the Norns sound computer. These extensions provide the interface between the Norns system and SuperCollider, enabling audio processing, synthesis, and other sound-related functionality.

## Directory Structure

- `core/`: Core SC classes for Norns functionality
  - `Crone.sc`: Main interface between Norns and SuperCollider
  - `CroneAudioContext.sc`: Audio routing and processing
  - `CroneDefs.sc`: SynthDef management
  - `CroneEffects.sc`: Audio effects
  - `CroneEngine.sc`: Base class for audio engines
  - `CronePoll.sc`: Data polling system
  - `CroneTester.sc`: Testing utilities

- `engines/`: Audio engines and utilities
  - Various engine implementations (e.g., `Engine_PolyPerc.sc`, `Engine_TestSine.sc`)
  - Utility classes (e.g., `AudioTaper.sc`, `BufUtil.sc`)

- `tests/`: Test files for SC extensions

- `norns-config.sc`: Configuration file that sets up include paths

## Integration with CMake

The SC extensions are integrated with the CMake build system, allowing for:

1. **Optional SC dependency**: SuperCollider integration can be enabled/disabled using the `USE_SUPERCOLLIDER` CMake option.
2. **Platform-specific installation**: Extensions are installed to the appropriate location based on the platform.
3. **Development workflow**: A custom target `install_sc_extensions` is provided for easy installation during development.

### CMake Options

- `USE_SUPERCOLLIDER`: Enable/disable SuperCollider integration (default: ON for embedded platforms, optional for desktop)
- `SC_EXTENSIONS_USER_DIR`: Path to user's SuperCollider extensions directory (auto-detected by default)
- `SC_EXTENSIONS_SYSTEM_DIR`: Path to system-wide SuperCollider extensions directory (used on embedded platforms)

## Installation

### Automatic Installation (via CMake)

When building Norns with CMake, the SC extensions will be installed automatically if SuperCollider integration is enabled:

```bash
# Configure with SC enabled (default for embedded platforms)
cmake -DUSE_SUPERCOLLIDER=ON ..

# Build and install
make
make install
```

To install only the SC extensions during development:

```bash
make install_sc_extensions
```

### Manual Installation

For quick manual installation, you can use the provided script:

```bash
# Install to default location
./install_extensions.sh

# Or specify a custom location
./install_extensions.sh /path/to/sc/extensions
```

If you prefer to install the extensions manually:

1. Copy `norns-config.sc` to your SC Extensions directory
2. Create a `norns` directory in your SC Extensions directory
3. Copy the `core` and `engines` directories into the `norns` directory

## Communication Architecture

The Norns system communicates with SuperCollider via OSC (Open Sound Control):

1. **Crone** (C++ component) sends OSC messages to SuperCollider
2. **SuperCollider** processes these messages and performs audio operations
3. **Polling system** allows data to be sent back from SC to the Norns system

## Development

When developing new SC extensions:

1. Add new SC classes to the appropriate directory (`core/` or `engines/`)
2. Update the file lists in `sc/CMakeLists.txt` if necessary
3. Install the extensions using `make install_sc_extensions`
4. Test your changes

## Testing

Basic tests for SC extensions can be found in the `tests/` directory. These can be run directly in SuperCollider to verify functionality.

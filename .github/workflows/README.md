# Norns CI Pipeline

This directory contains the GitHub Actions workflow configuration for the Norns CI pipeline. The CI pipeline builds and tests the project on multiple platforms using CMake.

## Overview

The CI pipeline consists of the following jobs:

1. **Linux Build & Test**: Builds and tests the project on Ubuntu using Docker
2. **macOS Build & Test**: Builds and tests the project on macOS
3. **Raspberry Pi Build**: Cross-compiles the project for Raspberry Pi
4. **Documentation Generation**: Generates API documentation and deploys it to GitHub Pages

## Workflow Triggers

The CI workflow is triggered on:

- Push to the `main` branch
- Pull requests to the `main` branch
- Manual trigger (workflow_dispatch)

## Job Details

### Linux Build & Test

This job builds and tests the project on Ubuntu using Docker. It:

- Uses the `linux-build.Dockerfile` to create a consistent build environment
- Builds the project with both Debug and Release configurations
- Runs tests for all components
- Caches Docker layers to speed up builds

### macOS Build & Test

This job builds and tests the project on macOS. It:

- Installs dependencies using Homebrew
- Builds the project with Debug configuration
- Runs tests for all components

### Raspberry Pi Build

This job cross-compiles the project for Raspberry Pi. It:

- Uses the `pi-build.Dockerfile` to create a cross-compilation environment
- Builds the project for the Raspberry Pi 3 platform
- Verifies that the build succeeds

### Documentation Generation

This job generates API documentation and deploys it to GitHub Pages. It:

- Sets up Python and installs dependencies
- Generates documentation using ldoc
- Deploys the documentation to GitHub Pages (only on push to main)

## Docker Files

The CI pipeline uses two Docker files:

1. **linux-build.Dockerfile**: Used for building and testing on Linux
2. **pi-build.Dockerfile**: Used for cross-compiling for Raspberry Pi

These Docker files install all necessary dependencies and set up the build environment.

## Using the CI Pipeline

### Running Locally

You can run the CI pipeline locally using Docker:

```bash
# Build the Docker image for Linux
docker build -f .github/workflows/linux-build.Dockerfile -t norns-build:latest .

# Run tests in the Docker container
docker run --rm norns-build:latest /bin/bash -c "cd /norns && ./build.sh test"

# Build the Docker image for Raspberry Pi
docker build -f .github/workflows/pi-build.Dockerfile -t norns-pi-build:latest .

# Cross-compile for Raspberry Pi
docker run --rm norns-pi-build:latest /bin/bash -c "cd /norns && ./build.sh build --platform=PI3 --type=Release"
```

### Troubleshooting

If the CI pipeline fails, check the following:

1. **Build Errors**: Look at the build logs to see if there are any compilation errors
2. **Test Failures**: Check the test logs to see if any tests are failing
3. **Docker Issues**: Make sure the Docker files are up to date with the latest dependencies
4. **Platform-Specific Issues**: Some issues may only occur on specific platforms

## Maintenance

When updating the CI pipeline, consider the following:

1. **Dependencies**: Keep the Docker files up to date with the latest dependencies
2. **Build Options**: Make sure the build options match the project requirements
3. **Test Coverage**: Ensure that all components are being tested
4. **Cross-Compilation**: Verify that cross-compilation for Raspberry Pi still works
5. **Documentation**: Update this README when making significant changes to the CI pipeline

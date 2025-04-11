#!/bin/bash
# Script to install SuperCollider extensions for development purposes
# This script demonstrates how to use the CMake-based installation system

# Default installation directory
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    DEFAULT_SC_DIR="$HOME/Library/Application Support/SuperCollider/Extensions"
else
    # Linux
    DEFAULT_SC_DIR="$HOME/.local/share/SuperCollider/Extensions"
fi

# Parse command line arguments
SC_DIR=${1:-$DEFAULT_SC_DIR}

# Create directories if they don't exist
mkdir -p "$SC_DIR"
mkdir -p "$SC_DIR/norns/core"
mkdir -p "$SC_DIR/norns/engines"

# Install norns-config.sc to the root extensions directory
cp norns-config.sc "$SC_DIR/"

# Install core files
cp core/*.sc "$SC_DIR/norns/core/"

# Install engine files
cp engines/*.sc "$SC_DIR/norns/engines/"

echo "SuperCollider extensions installed to: $SC_DIR"
echo "To test the installation, run the test script in SuperCollider:"
echo "  1. Open SuperCollider"
echo "  2. Open the file: $(pwd)/tests/sc_extensions_test.scd"
echo "  3. Run the script to verify the installation"

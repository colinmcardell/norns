#!/bin/bash
# Script to set up SuperCollider extensions after container startup

SC_EXTENSIONS_DIR="$HOME/.local/share/SuperCollider/Extensions"
SC_SOURCE_DIR="/workspace/sc"

# Function to check if source files exist
check_source_files() {
    if [ ! -f "$SC_SOURCE_DIR/install_extensions.sh" ] || \
       [ ! -f "$SC_SOURCE_DIR/norns-config.sc" ] || \
       [ ! -d "$SC_SOURCE_DIR/core" ] || \
       [ ! -d "$SC_SOURCE_DIR/engines" ]; then
        echo "Warning: SuperCollider source files not found in $SC_SOURCE_DIR"
        return 1
    fi
    return 0
}

# Setup SuperCollider extensions if source files exist
setup_sc_extensions() {
    echo "Setting up SuperCollider extensions..."
    
    # Make temporary directory
    mkdir -p /tmp/sc
    
    # Copy files with proper permissions
    cp -r "$SC_SOURCE_DIR/install_extensions.sh" "$SC_SOURCE_DIR/norns-config.sc" /tmp/sc/
    cp -r "$SC_SOURCE_DIR/core" /tmp/sc/
    cp -r "$SC_SOURCE_DIR/engines" /tmp/sc/
    
    # Ensure script is executable
    chmod +x /tmp/sc/install_extensions.sh
    
    # Run installation
    cd /tmp/sc && ./install_extensions.sh
    
    # Clean up
    rm -rf /tmp/sc
    echo "SuperCollider extensions setup complete"
}

# Main execution
mkdir -p "$SC_EXTENSIONS_DIR"

if check_source_files; then
    setup_sc_extensions
else
    echo "SuperCollider setup skipped - source files not available."
    echo "To install manually later, run: /workspace/.devcontainer/sc_setup.sh"
fi

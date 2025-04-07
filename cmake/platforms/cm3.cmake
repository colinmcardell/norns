message(STATUS "Configuring for CM3 platform")

# Check if we're on a compatible ARM platform
execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE HOST_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Detected host architecture: ${HOST_ARCH}")

# Only apply specific ARM flags if we're on a completely compatible platform
# For CM3, we ideally want armv7l (32-bit ARM v7) which matches Cortex-A7
# When cross-compiling or on a different ARM architecture, skip CPU-specific flags
if(HOST_ARCH STREQUAL "armv7l")
    # Native build on compatible ARM - apply full CPU-specific optimizations
    set(NORNS_PLATFORM_FLAGS 
        -mcpu=cortex-a7
        -mtune=cortex-a7
        -mfpu=neon-vfpv4
        -mfloat-abi=hard
        -ftree-vectorize
    )
    message(STATUS "Using native ARM compilation flags (armv7l)")
else()
    # Not on compatible ARM platform, omit ARM-specific flags
    set(NORNS_PLATFORM_FLAGS "")
    if(HOST_ARCH MATCHES "^(arm|aarch)")
        message(STATUS "ARM architecture detected but not armv7l, using compatible compilation mode")
    else()
        message(STATUS "Non-ARM architecture detected, using cross-compilation mode")
    endif()
endif()

# Platform-specific defines (always applied regardless of host platform)
set(NORNS_PLATFORM_DEFINES
    -DPLATFORM_CM3=1
    -DPLATFORM_RPI=1
    -DMEMORY_CONSTRAINED=1
)

# Audio settings appropriate for CM3-based Norns hardware
set(NORNS_AUDIO_BUFFER_SIZE 64)

# Optimize for size and speed given memory constraints
if(HOST_ARCH STREQUAL "armv7l")
    set(NORNS_PRECISION_FLAGS "-ffast-math")
else()
    set(NORNS_PRECISION_FLAGS "")
endif()

# Set the maximum number of inputs/outputs based on platform hardware
set(NORNS_MAX_INPUTS 2)
set(NORNS_MAX_OUTPUTS 2)

# SuperCollider is required for CM3 builds
set(NORNS_REQUIRE_SUPERCOLLIDER ON)

# Function to apply platform-specific settings to a target
function(apply_platform_settings target)
    # Apply platform flags only if they exist
    if(NORNS_PLATFORM_FLAGS)
        target_compile_options(${target} PRIVATE ${NORNS_PLATFORM_FLAGS})
    endif()
    
    # Apply precision flags only if they exist
    if(NORNS_PRECISION_FLAGS)
        target_compile_options(${target} PRIVATE ${NORNS_PRECISION_FLAGS})
    endif()
    
    # Always apply platform-specific defines
    target_compile_definitions(${target} PRIVATE ${NORNS_PLATFORM_DEFINES})
endfunction()
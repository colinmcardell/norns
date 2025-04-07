message(STATUS "Configuring for PI3 platform")

# Check if we're on a compatible ARM platform
execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE HOST_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Detected host architecture: ${HOST_ARCH}")

# Only apply specific ARM flags if we're on a completely compatible platform
# For PI3, we ideally want aarch64 or armv8l (64-bit ARM v8) which matches Cortex-A53
# When cross-compiling or on a different ARM architecture, skip CPU-specific flags
if(HOST_ARCH MATCHES "^(aarch64|armv8)")
    # Native build on compatible ARM - apply full CPU-specific optimizations
    set(NORNS_PLATFORM_FLAGS 
        -mcpu=cortex-a53
        -mtune=cortex-a53
        -mfpu=neon-fp-armv8
        -mfloat-abi=hard
        -ftree-vectorize
    )
    message(STATUS "Using native ARM compilation flags (aarch64/armv8)")
else()
    # Not on compatible ARM platform, omit ARM-specific flags
    set(NORNS_PLATFORM_FLAGS "")
    if(HOST_ARCH MATCHES "^(arm|aarch)")
        message(STATUS "ARM architecture detected but not aarch64/armv8, using compatible compilation mode")
    else()
        message(STATUS "Non-ARM architecture detected, using cross-compilation mode")
    endif()
endif()

# Platform-specific defines (always applied regardless of host platform)
set(NORNS_PLATFORM_DEFINES
    -DPLATFORM_PI3=1
    -DPLATFORM_RPI=1
    -DMEMORY_CONSTRAINED=1
)

# Audio settings appropriate for Norns hardware
set(NORNS_AUDIO_BUFFER_SIZE 64)

# Optimize for size and speed given memory constraints
if(HOST_ARCH MATCHES "^(aarch64|armv8)")
    set(NORNS_PRECISION_FLAGS "-ffast-math")
else()
    set(NORNS_PRECISION_FLAGS "")
endif()

# Set the maximum number of inputs/outputs based on platform hardware
set(NORNS_MAX_INPUTS 2)
set(NORNS_MAX_OUTPUTS 2)

# SuperCollider is required for PI3 builds
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
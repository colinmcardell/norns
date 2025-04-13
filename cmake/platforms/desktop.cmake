message(STATUS "Configuring for DESKTOP platform")

# No specific CPU optimizations for desktop as we want it to be portable
set(NORNS_PLATFORM_FLAGS "-march=native")

# Desktop build typically has more memory available
set(NORNS_PLATFORM_DEFINES
    -DPLATFORM_DESKTOP=1
    -DMEMORY_CONSTRAINED=0
)

# On desktop, set audio buffer size larger for stability
set(NORNS_AUDIO_BUFFER_SIZE 1024)

# Desktop may have better precision requirements
set(NORNS_PRECISION_FLAGS "-ffp-contract=fast")

# Set the maximum number of inputs/outputs based on platform
set(NORNS_MAX_INPUTS 8)
set(NORNS_MAX_OUTPUTS 8)

# Make SuperCollider non-required for desktop builds
set(NORNS_REQUIRE_SUPERCOLLIDER OFF)

# Function to apply platform-specific settings to a target
function(apply_platform_settings target)
    target_compile_options(${target} PRIVATE ${NORNS_PLATFORM_FLAGS} ${NORNS_PRECISION_FLAGS})
    target_compile_definitions(${target} PRIVATE ${NORNS_PLATFORM_DEFINES})
endfunction()
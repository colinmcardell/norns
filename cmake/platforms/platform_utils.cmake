# Platform utility functions for Norns build system

# Auto-detect platform if not specified
function(detect_platform)
    if(NOT DEFINED PLATFORM)
        # Check for Raspberry Pi
        if(EXISTS "/sys/firmware/devicetree/base/model")
            file(READ "/sys/firmware/devicetree/base/model" DEVICE_MODEL)
            
            # Check for Raspberry Pi 3
            if(DEVICE_MODEL MATCHES "Raspberry Pi 3")
                set(PLATFORM "PI3" CACHE STRING "Target platform" FORCE)
            # Check for Compute Module 3
            elseif(DEVICE_MODEL MATCHES "Compute Module 3")
                set(PLATFORM "CM3" CACHE STRING "Target platform" FORCE)
            # Default to PI3 for other Raspberry Pi variants
            elseif(DEVICE_MODEL MATCHES "Raspberry Pi")
                set(PLATFORM "PI3" CACHE STRING "Target platform" FORCE)
            endif()
        endif()
        
        # If still not set, default to DESKTOP
        if(NOT DEFINED PLATFORM)
            set(PLATFORM "DESKTOP" CACHE STRING "Target platform" FORCE)
        endif()
    endif()
    
    # Validate that the platform is one we support
    if(NOT PLATFORM MATCHES "^(DESKTOP|PI3|CM3)$")
        message(FATAL_ERROR "Invalid PLATFORM value: ${PLATFORM}. Must be one of: DESKTOP, PI3, CM3")
    endif()
endfunction()

# Configure platform-specific settings
function(configure_platform)
    # Convert platform name to lowercase for file naming
    string(TOLOWER ${PLATFORM} PLATFORM_LOWER)
    
    # Get the absolute path to the platform file
    set(PLATFORM_FILE "${CMAKE_SOURCE_DIR}/cmake/platforms/${PLATFORM_LOWER}.cmake")
    
    # Check if the platform file exists
    if(NOT EXISTS ${PLATFORM_FILE})
        message(FATAL_ERROR "Platform file not found: ${PLATFORM_FILE}")
    endif()
    
    # Include the platform-specific file
    include(${PLATFORM_FILE})
    
    # Set common variables based on platform
    if(PLATFORM STREQUAL "DESKTOP")
        set(NORNS_IS_EMBEDDED FALSE PARENT_SCOPE)
    else()
        set(NORNS_IS_EMBEDDED TRUE PARENT_SCOPE)
    endif()
    
    # Export platform settings to parent scope
    set(NORNS_PLATFORM_FLAGS ${NORNS_PLATFORM_FLAGS} PARENT_SCOPE)
    set(NORNS_PLATFORM_DEFINES ${NORNS_PLATFORM_DEFINES} PARENT_SCOPE)
    set(NORNS_AUDIO_BUFFER_SIZE ${NORNS_AUDIO_BUFFER_SIZE} PARENT_SCOPE)
    set(NORNS_PRECISION_FLAGS ${NORNS_PRECISION_FLAGS} PARENT_SCOPE)
    set(NORNS_MAX_INPUTS ${NORNS_MAX_INPUTS} PARENT_SCOPE)
    set(NORNS_MAX_OUTPUTS ${NORNS_MAX_OUTPUTS} PARENT_SCOPE)
    set(NORNS_REQUIRE_SUPERCOLLIDER ${NORNS_REQUIRE_SUPERCOLLIDER} PARENT_SCOPE)
endfunction()
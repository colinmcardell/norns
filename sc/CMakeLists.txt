cmake_minimum_required(VERSION 3.10)

# SuperCollider extensions configuration
project(norns_sc_extensions
    VERSION 1.0.0
    DESCRIPTION "Norns SuperCollider extensions"
    LANGUAGES NONE)

# Find SuperCollider user extension directories
if(APPLE)
    # macOS standard locations
    set(SC_EXTENSIONS_USER_DIR "$ENV{HOME}/Library/Application Support/SuperCollider/Extensions" CACHE PATH "SuperCollider user extensions directory")
elseif(UNIX)
    # Linux standard locations
    set(SC_EXTENSIONS_USER_DIR "$ENV{HOME}/.local/share/SuperCollider/Extensions" CACHE PATH "SuperCollider user extensions directory")
endif()

# System-wide installation path (used mainly for embedded platforms)
if(NORNS_IS_EMBEDDED)
    set(SC_EXTENSIONS_SYSTEM_DIR "/usr/share/SuperCollider/Extensions" CACHE PATH "SuperCollider system extensions directory")
endif()

# Define the installation directory based on platform
if(NORNS_IS_EMBEDDED)
    set(SC_INSTALL_DIR ${SC_EXTENSIONS_SYSTEM_DIR})
else()
    set(SC_INSTALL_DIR ${SC_EXTENSIONS_USER_DIR})
endif()

# Define SC extension files to install
set(SC_CORE_FILES
    core/Crone.sc
    core/CroneAudioContext.sc
    core/CroneDefs.sc
    core/CroneEffects.sc
    core/CroneEngine.sc
    core/CronePoll.sc
    core/CroneTester.sc
)

set(SC_ENGINE_FILES
    engines/AudioTaper.sc
    engines/BufUtil.sc
    engines/CroneGenEngine.sc
    engines/Engine_PolyPerc.sc
    engines/Engine_PolySub.sc
    engines/Engine_SimplePassThru.sc
    engines/Engine_TestSine.sc
    engines/PatchMatrix.sc
)

set(SC_CONFIG_FILES
    norns-config.sc
)

# Installation targets
if(USE_SUPERCOLLIDER)
    # Install norns-config.sc to the root of the extensions directory
    install(FILES ${SC_CONFIG_FILES} DESTINATION ${SC_INSTALL_DIR})
    
    # Install core files to a 'norns/core' subdirectory
    install(FILES ${SC_CORE_FILES} DESTINATION ${SC_INSTALL_DIR}/norns/core)
    
    # Install engine files to a 'norns/engines' subdirectory
    install(FILES ${SC_ENGINE_FILES} DESTINATION ${SC_INSTALL_DIR}/norns/engines)
    
    # Print installation information
    message(STATUS "SuperCollider extensions will be installed to: ${SC_INSTALL_DIR}")
else()
    message(STATUS "SuperCollider integration is disabled")
endif()

# Add a custom target to install SC extensions during development
if(USE_SUPERCOLLIDER)
    add_custom_target(install_sc_extensions
        COMMAND ${CMAKE_COMMAND} -DCOMPONENT=sc_extensions -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
        COMMENT "Installing SuperCollider extensions to ${SC_INSTALL_DIR}"
    )
endif()

# Testing support for SC extensions
if(BUILD_TESTS AND USE_SUPERCOLLIDER)
    # Add a test that verifies SC extensions can be loaded
    # This would require SC to be installed and running
    add_test(NAME test_sc_extensions_install
             COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_sc_installation.cmake
    )
    
    # Skip the test if SC is not enabled
    set_tests_properties(test_sc_extensions_install PROPERTIES
        SKIP_RETURN_CODE 77
    )
    
    # Copy the SuperCollider test script to the build directory
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/sc_extensions_test.scd
        ${CMAKE_CURRENT_BINARY_DIR}/tests/sc_extensions_test.scd
        COPYONLY
    )
    
    # Add a message to help developers run the tests
    add_custom_target(test_sc_extensions
        COMMAND ${CMAKE_COMMAND} -E echo "Running SuperCollider extension tests..."
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_sc_installation.cmake
        COMMENT "Testing SuperCollider extensions installation"
    )
endif()

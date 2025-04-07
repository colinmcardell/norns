cmake_minimum_required(VERSION 3.7)
project(crone VERSION 1.0.0)

add_subdirectory(softcut/softcut-lib)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set (SRC src/main.cpp
        src/BufDiskWorker.cpp
        src/Commands.cpp
        src/MixerClient.cpp
        src/OscInterface.cpp
        src/SoftcutClient.cpp
        src/Taper.cpp
        src/Window.cpp
        softcut/softcut-lib/src/FadeCurves.cpp
        softcut/softcut-lib/src/ReadWriteHead.cpp
        softcut/softcut-lib/src/SubHead.cpp
        softcut/softcut-lib/src/Svf.cpp
        softcut/softcut-lib/src/Voice.cpp )

add_executable(crone ${SRC})

include_directories(./faust ./softcut/softcut-lib/include lib/readerwriterqueue)

# Configure Faust include paths based on platform
if(APPLE)
    include_directories(/usr/local/include)
    # Check for Homebrew first, then MacPorts for Faust
    if(EXISTS "/opt/homebrew/include/faust")
        include_directories(/opt/homebrew/include)
    elseif(EXISTS "/usr/local/include/faust")
        include_directories(/usr/local/include)
    elseif(EXISTS "/opt/local/include/faust")
        include_directories(/opt/local/include)
    else()
        message(WARNING "Faust headers not found in standard locations")
    endif()
elseif(UNIX)
    # Standard locations on Linux
    if(EXISTS "/usr/include/faust")
        include_directories(/usr/include)
    elseif(EXISTS "/usr/local/include/faust")
        include_directories(/usr/local/include)
    else()
        message(WARNING "Faust headers not found in standard locations")
    endif()
endif()

if(UNIX)
    if(APPLE)
        find_library(lo_lib liblo.dylib REQUIRED)
        find_library(sndfile_lib libsndfile.a REQUIRED)
        find_library(jack_lib libjack.dylib REQUIRED)
        target_link_libraries(crone ${lo_lib} ${sndfile_lib} ${jack_lib})
        include_directories(/usr/local/include)
    else()
        target_link_libraries(crone lo)
        target_link_libraries(crone jack)
        target_link_libraries(crone pthread)
        target_link_libraries(crone asound)
        target_link_libraries(crone sndfile)
    endif()
    target_link_libraries(crone softcut)
else()
    # nope
endif()

# Set audio buffer size from platform configuration
if(DEFINED NORNS_AUDIO_BUFFER_SIZE)
    target_compile_definitions(crone PRIVATE BUFFER_SIZE=${NORNS_AUDIO_BUFFER_SIZE})
endif()

# Apply platform-specific settings
if(COMMAND configure_for_platform)
    configure_for_platform(crone)
    configure_for_platform(softcut)
endif()

set_target_properties(crone PROPERTIES
CXX_STANDARD 14
CXX_STANDARD_REQUIRED YES
CXX_EXTENSIONS YES
)

target_compile_options(crone PRIVATE -Wall -Wextra -pedantic)

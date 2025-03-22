#include <stdbool.h>
#include <stdio.h>

#include "hardware/hal.h"

// HAL implementation structures with default values for testing
static hal_screen_t screen_hal = {.width = 128, .height = 64};

static hal_input_t input_hal;
static hal_audio_t audio_hal;
static hal_system_t system_hal;

// Initialize all hardware components
bool hal_init(void) {
    bool success = true;

    // Initialize the screen HAL
    if (screen_hal.init) {
        success = success && screen_hal.init();
    }

    // Initialize the input HAL
    if (input_hal.init) {
        success = success && input_hal.init();
    }

    // Initialize the audio HAL
    if (audio_hal.init) {
        success = success && audio_hal.init();
    }

    // Initialize the system monitor HAL
    if (system_hal.init) {
        success = success && system_hal.init();
    }

    return success;
}

// Shutdown all hardware components
void hal_deinit(void) {
    // Deinitialize in reverse order
    if (system_hal.deinit) {
        system_hal.deinit();
    }

    if (audio_hal.deinit) {
        audio_hal.deinit();
    }

    if (input_hal.deinit) {
        input_hal.deinit();
    }

    if (screen_hal.deinit) {
        screen_hal.deinit();
    }
}

// Get access to HAL implementations
hal_screen_t *hal_screen(void) {
    return &screen_hal;
}

hal_input_t *hal_input(void) {
    return &input_hal;
}

hal_audio_t *hal_audio(void) {
    return &audio_hal;
}

hal_system_t *hal_system(void) {
    return &system_hal;
}
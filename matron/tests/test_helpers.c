#include "test_helpers.h"
#include <stdbool.h>
#include <stddef.h>

// Mock function state tracking variables
bool screen_init_called = false;
bool screen_deinit_called = false;
bool input_init_called = false;
bool input_init_returns_error = false;
bool input_deinit_called = false;
bool audio_init_called = false;
bool audio_deinit_called = false;
bool system_init_called = false;
bool system_deinit_called = false;

// Static mock implementation structures
static hal_screen_t mock_screen = {.width = 128, // Set default width for tests
                                   .height = 64, // Set default height for tests
                                   .init = NULL,
                                   .deinit = NULL,
                                   .clear = NULL,
                                   .refresh = NULL};

static hal_input_t mock_input = {0};
static hal_audio_t mock_audio = {0};
static hal_system_t mock_system = {0};

// Mock implementations
bool mock_screen_init(void) {
    screen_init_called = true;
    return true;
}

void mock_screen_deinit(void) {
    screen_deinit_called = true;
}

void mock_screen_clear(void) {
    // Empty implementation
}

void mock_screen_refresh(void) {
    // Empty implementation
}

bool mock_input_init(void) {
    input_init_called = true;
    return !input_init_returns_error;
}

void mock_input_deinit(void) {
    input_deinit_called = true;
}

void mock_input_scan(void) {
    // Empty implementation
}

bool mock_audio_init(void) {
    audio_init_called = true;
    return true;
}

void mock_audio_deinit(void) {
    audio_deinit_called = true;
}

bool mock_audio_start(void) {
    return true;
}

void mock_audio_stop(void) {
    // Empty implementation
}

bool mock_system_init(void) {
    system_init_called = true;
    return true;
}

void mock_system_deinit(void) {
    system_deinit_called = true;
}

float mock_system_get_cpu_usage(void) {
    return 0.0f;
}

float mock_system_get_temperature(void) {
    return 25.0f;
}

// HAL initialization
bool hal_init(void) {
    // Initialize all components and return false if any fail
    if (!hal_screen()->init()) {
        return false;
    }

    if (!hal_input()->init()) {
        hal_screen()->deinit();
        return false;
    }

    if (!hal_audio()->init()) {
        hal_input()->deinit();
        hal_screen()->deinit();
        return false;
    }

    if (!hal_system()->init()) {
        hal_audio()->deinit();
        hal_input()->deinit();
        hal_screen()->deinit();
        return false;
    }

    return true;
}

// HAL deinitialization
void hal_deinit(void) {
    hal_system()->deinit();
    hal_audio()->deinit();
    hal_input()->deinit();
    hal_screen()->deinit();
}

// HAL getter implementations
hal_screen_t *hal_screen(void) {
    // Initialize on first access if not already done
    if (mock_screen.init == NULL) {
        mock_screen.init = mock_screen_init;
        mock_screen.deinit = mock_screen_deinit;
        mock_screen.clear = mock_screen_clear;
        mock_screen.refresh = mock_screen_refresh;
    }
    return &mock_screen;
}

hal_input_t *hal_input(void) {
    // Initialize on first access if not already done
    if (mock_input.init == NULL) {
        mock_input.init = mock_input_init;
        mock_input.deinit = mock_input_deinit;
        mock_input.scan = mock_input_scan;
    }
    return &mock_input;
}

hal_audio_t *hal_audio(void) {
    // Initialize on first access if not already done
    if (mock_audio.init == NULL) {
        mock_audio.init = mock_audio_init;
        mock_audio.deinit = mock_audio_deinit;
        mock_audio.start = mock_audio_start;
        mock_audio.stop = mock_audio_stop;
    }
    return &mock_audio;
}

hal_system_t *hal_system(void) {
    // Initialize on first access if not already done
    if (mock_system.init == NULL) {
        mock_system.init = mock_system_init;
        mock_system.deinit = mock_system_deinit;
        mock_system.get_cpu_usage = mock_system_get_cpu_usage;
        mock_system.get_temperature = mock_system_get_temperature;
    }
    return &mock_system;
}

// Unity setup and teardown functions
void setUp(void) {
    // Reset state tracking for each test
    screen_init_called = false;
    screen_deinit_called = false;
    input_init_called = false;
    input_init_returns_error = false;
    input_deinit_called = false;
    audio_init_called = false;
    audio_deinit_called = false;
    system_init_called = false;
    system_deinit_called = false;
}

void tearDown(void) {
    // Common cleanup after each test
}
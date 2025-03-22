#include "test_helpers.h"

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

// Mock implementations
bool mock_screen_init(void) {
    screen_init_called = true;
    return true;
}

void mock_screen_deinit(void) {
    screen_deinit_called = true;
}

bool mock_input_init(void) {
    input_init_called = true;
    return !input_init_returns_error;
}

void mock_input_deinit(void) {
    input_deinit_called = true;
}

bool mock_audio_init(void) {
    audio_init_called = true;
    return true;
}

void mock_audio_deinit(void) {
    audio_deinit_called = true;
}

bool mock_system_init(void) {
    system_init_called = true;
    return true;
}

void mock_system_deinit(void) {
    system_deinit_called = true;
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

    // Set up mock implementations
    hal_screen()->init = mock_screen_init;
    hal_screen()->deinit = mock_screen_deinit;
    hal_input()->init = mock_input_init;
    hal_input()->deinit = mock_input_deinit;
    hal_audio()->init = mock_audio_init;
    hal_audio()->deinit = mock_audio_deinit;
    hal_system()->init = mock_system_init;
    hal_system()->deinit = mock_system_deinit;
}

void tearDown(void) {
    // Common cleanup after each test
}
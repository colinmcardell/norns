#include <stdbool.h>
#include <stdio.h>

#include "hardware/hal.h"
#include "test_helpers.h"
#include "unity.h"

// Test HAL initialization and deinitialization
void test_hal_init_deinit(void) {
    // Test successful initialization
    bool result = hal_init();
    TEST_ASSERT_TRUE(result);

    // Verify all components were initialized
    TEST_ASSERT_TRUE(screen_init_called);
    TEST_ASSERT_TRUE(input_init_called);
    TEST_ASSERT_TRUE(audio_init_called);
    TEST_ASSERT_TRUE(system_init_called);

    // Test deinitialization
    hal_deinit();

    // Verify all components were deinitialized
    TEST_ASSERT_TRUE(screen_deinit_called);
    TEST_ASSERT_TRUE(input_deinit_called);
    TEST_ASSERT_TRUE(audio_deinit_called);
    TEST_ASSERT_TRUE(system_deinit_called);

    // Test initialization failure
    input_init_returns_error = true;

    // Reset tracking
    screen_init_called = false;
    input_init_called = false;
    audio_init_called = false;
    system_init_called = false;

    // HAL init should fail if any component fails
    result = hal_init();
    TEST_ASSERT_FALSE(result);

    // Input init should be called and fail
    TEST_ASSERT_TRUE(input_init_called);
}

// Test screen HAL
void test_hal_screen(void) {
    // Test screen dimensions
    TEST_ASSERT_GREATER_THAN(0, hal_screen()->width);
    TEST_ASSERT_GREATER_THAN(0, hal_screen()->height);

    // Create a dummy function for testing function pointers
    bool (*init_func)(void) = hal_screen()->init;
    void (*deinit_func)(void) = hal_screen()->deinit;

    // Verify function pointers are set
    TEST_ASSERT_NOT_NULL(init_func);
    TEST_ASSERT_NOT_NULL(deinit_func);

    // Call functions through HAL
    bool result = hal_screen()->init();
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(screen_init_called);

    hal_screen()->deinit();
    TEST_ASSERT_TRUE(screen_deinit_called);
}

// Test input HAL
void test_hal_input(void) {
    // Create a dummy function for testing function pointers
    bool (*init_func)(void) = hal_input()->init;
    void (*deinit_func)(void) = hal_input()->deinit;

    // Verify function pointers are set
    TEST_ASSERT_NOT_NULL(init_func);
    TEST_ASSERT_NOT_NULL(deinit_func);

    // Call functions through HAL
    bool result = hal_input()->init();
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(input_init_called);

    hal_input()->deinit();
    TEST_ASSERT_TRUE(input_deinit_called);
}

// Test audio HAL
void test_hal_audio(void) {
    // Create a dummy function for testing function pointers
    bool (*init_func)(void) = hal_audio()->init;
    void (*deinit_func)(void) = hal_audio()->deinit;

    // Verify function pointers are set
    TEST_ASSERT_NOT_NULL(init_func);
    TEST_ASSERT_NOT_NULL(deinit_func);

    // Call functions through HAL
    bool result = hal_audio()->init();
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(audio_init_called);

    hal_audio()->deinit();
    TEST_ASSERT_TRUE(audio_deinit_called);
}

// Test system HAL
void test_hal_system(void) {
    // Create a dummy function for testing function pointers
    bool (*init_func)(void) = hal_system()->init;
    void (*deinit_func)(void) = hal_system()->deinit;

    // Verify function pointers are set
    TEST_ASSERT_NOT_NULL(init_func);
    TEST_ASSERT_NOT_NULL(deinit_func);

    // Call functions through HAL
    bool result = hal_system()->init();
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(system_init_called);

    hal_system()->deinit();
    TEST_ASSERT_TRUE(system_deinit_called);
}
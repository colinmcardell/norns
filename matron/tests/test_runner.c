#include "hardware/hal.h"
#include "unity.h"
#include <stdio.h>

// Test function declarations
extern void test_event_system_init(void);
extern void test_event_system_create_free(void);
extern void test_event_system_post_process(void);
extern void test_event_system_subscribe_unsubscribe(void);

extern void test_hal_init_deinit(void);
extern void test_hal_screen(void);
extern void test_hal_input(void);
extern void test_hal_audio(void);
extern void test_hal_system(void);

// Mock function state tracking
extern bool screen_init_called;
extern bool screen_deinit_called;
extern bool input_init_called;
extern bool input_init_returns_error;
extern bool input_deinit_called;
extern bool audio_init_called;
extern bool audio_deinit_called;
extern bool system_init_called;
extern bool system_deinit_called;

// Main test runner
int main(void) {
    UNITY_BEGIN();

    // Event system tests
    RUN_TEST(test_event_system_init);
    RUN_TEST(test_event_system_create_free);
    RUN_TEST(test_event_system_post_process);
    RUN_TEST(test_event_system_subscribe_unsubscribe);

    // HAL tests
    RUN_TEST(test_hal_init_deinit);
    RUN_TEST(test_hal_screen);
    RUN_TEST(test_hal_input);
    RUN_TEST(test_hal_audio);
    RUN_TEST(test_hal_system);

    return UNITY_END();
}
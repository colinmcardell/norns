#include "hardware/hal.h"
#include "unity.h"
#include <stdio.h>

// HAL test function declarations
extern void test_hal_init_deinit(void);
extern void test_hal_screen(void);
extern void test_hal_input(void);
extern void test_hal_audio(void);
extern void test_hal_system(void);

// HAL test runner
int main(void) {
    UNITY_BEGIN();

    // Run HAL tests
    RUN_TEST(test_hal_init_deinit);
    RUN_TEST(test_hal_screen);
    RUN_TEST(test_hal_input);
    RUN_TEST(test_hal_audio);
    RUN_TEST(test_hal_system);

    return UNITY_END();
}
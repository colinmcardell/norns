#include "unity.h"
#include <stdio.h>

// Event system test function declarations
extern void test_event_system_init(void);
extern void test_event_system_create_free(void);
extern void test_event_system_post_process(void);
extern void test_event_system_subscribe_unsubscribe(void);

// Event system test runner
int main(void) {
    UNITY_BEGIN();

    // Run event system tests
    RUN_TEST(test_event_system_init);
    RUN_TEST(test_event_system_create_free);
    RUN_TEST(test_event_system_post_process);
    RUN_TEST(test_event_system_subscribe_unsubscribe);

    return UNITY_END();
}
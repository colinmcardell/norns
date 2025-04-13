#include "test_helpers.h"
#include <stdbool.h>
#include <stddef.h>

// Test that basic assertions work
void test_basic_assertions(void) {
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);
    TEST_ASSERT_EQUAL_INT(42, 42);
}

// Test that the test framework is properly set up
void test_framework_setup(void) {
    TEST_ASSERT_NOT_NULL(setUp);
    TEST_ASSERT_NOT_NULL(tearDown);
}

// Main entry point for tests
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_basic_assertions);
    RUN_TEST(test_framework_setup);

    return UNITY_END();
}
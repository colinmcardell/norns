#include "test_helpers.h"

// Basic test to verify the infrastructure works
void test_basic_assertions(void) {
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);
    TEST_ASSERT_EQUAL_INT(42, 42);
}

// Main entry point for tests
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_basic_assertions);

    return UNITY_END();
}
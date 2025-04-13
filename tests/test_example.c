/**
 * @file test_example.c
 * @brief Example test file demonstrating the Unity testing framework integration
 *
 * This file serves as a template and reference for creating tests in the Norns project.
 * It demonstrates how to:
 *  - Set up tests with Unity
 *  - Create test cases
 *  - Run assertions
 *  - Handle test setup/teardown
 *  - Execute the test runner
 *
 * This example verifies that the Unity test framework is properly integrated
 * with the CMake build system. Developers can use this as a reference when
 * creating new test files.
 */

#include "unity.h"

// Function to test
int add(int a, int b) {
    return a + b;
}

// Test setup and teardown
void setUp(void) {
    // This function runs before each test
}

void tearDown(void) {
    // This function runs after each test
}

// Test cases
void test_addition(void) {
    TEST_ASSERT_EQUAL_INT(5, add(2, 3));
    TEST_ASSERT_EQUAL_INT(10, add(7, 3));
    TEST_ASSERT_EQUAL_INT(0, add(0, 0));
    TEST_ASSERT_EQUAL_INT(-2, add(-5, 3));
}

void test_addition_fails(void) {
    TEST_ASSERT_EQUAL_INT(5, add(2, 3));
    // This test will pass, but demonstrates how a failure would be reported
    // TEST_ASSERT_EQUAL_INT(10, add(1, 1)); // This would fail
}

// Test runner
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_addition);
    RUN_TEST(test_addition_fails);
    return UNITY_END();
}
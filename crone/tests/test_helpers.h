#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

// Unity test framework
#ifdef __cplusplus
extern "C" {
#endif
#include "unity.h"
#ifdef __cplusplus
}
#endif

// Basic C++ includes for tests
#include <string>

// Helper functions
bool isTestEnvironmentValid();

// setUp and tearDown functions for tests
#ifdef __cplusplus
extern "C" {
#endif
void setUp(void);
void tearDown(void);
#ifdef __cplusplus
}
#endif

#endif /* TEST_HELPERS_H */

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "hardware/hal.h"
#include <stdbool.h>

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

// Mock function declarations
bool mock_screen_init(void);
void mock_screen_deinit(void);
bool mock_input_init(void);
void mock_input_deinit(void);
bool mock_audio_init(void);
void mock_audio_deinit(void);
bool mock_system_init(void);
void mock_system_deinit(void);

// Unity setup and teardown functions
void setUp(void);
void tearDown(void);

#endif /* TEST_HELPERS_H */
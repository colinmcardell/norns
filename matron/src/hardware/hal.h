#pragma once

#include <stdbool.h>
#include <stdint.h>

// Only include cairo when not in testing mode
#ifndef TESTING
#include <cairo/cairo.h>
#endif

/**
 * @file hal.h
 * @brief Hardware Abstraction Layer for norns
 *
 * Provides clean interfaces to hardware components without
 * exposing implementation details to application code
 */

// For testing mode, define a stub type
#ifdef TESTING
typedef struct {
} cairo_surface_t;
#endif

// Initialize all hardware components
bool hal_init(void);

// Shutdown all hardware components
void hal_deinit(void);

// Screen HAL
typedef struct {
    // Low-level hardware control
    bool (*init)(void);
    void (*deinit)(void);
    void (*set_brightness)(int brightness);
    void (*set_contrast)(int contrast);
    void (*set_invert)(bool invert);

    // Drawing operations
    void (*clear)(void);
    void (*refresh)(void);
    cairo_surface_t *(*get_surface)(void);

    // Dimensions
    int width;
    int height;
} hal_screen_t;

// Input HAL
typedef struct {
    bool (*init)(void);
    void (*deinit)(void);
    void (*scan)(void); // Poll for new input events
} hal_input_t;

// Audio HAL
typedef struct {
    bool (*init)(void);
    void (*deinit)(void);
    bool (*start)(void);
    void (*stop)(void);
    void (*set_level_adc)(float level);
    void (*set_level_dac)(float level);
    float (*get_cpu_load)(void);
} hal_audio_t;

// System monitor HAL
typedef struct {
    bool (*init)(void);
    void (*deinit)(void);
    float (*get_cpu_usage)(void);
    float (*get_temperature)(void);
    float (*get_battery_level)(void);
    bool (*is_charging)(void);
} hal_system_t;

// Get access to HAL implementations
hal_screen_t *hal_screen(void);
hal_input_t *hal_input(void);
hal_audio_t *hal_audio(void);
hal_system_t *hal_system(void);
#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @file event_system.h
 * @brief Improved event system for norns
 *
 * Clean event system with separation of hardware and application concerns.
 * The event system allows hardware events to be processed by the application
 * without tight coupling between the components.
 */

// Event types
typedef enum {
    EVENT_KEY_PRESS,
    EVENT_KEY_RELEASE,
    EVENT_ENCODER_DELTA,
    EVENT_SCREEN_REFRESH,
    EVENT_BATTERY_CHANGE,
    EVENT_POWER_CHANGE,
    EVENT_SYSTEM_STATS,
    EVENT_AUDIO_LEVEL,
    EVENT_MIDI_DEVICE_ADDED,
    EVENT_MIDI_DEVICE_REMOVED,
    EVENT_MIDI_EVENT,
    EVENT_GRID_DEVICE_ADDED,
    EVENT_GRID_DEVICE_REMOVED,
    EVENT_GRID_KEY,
    EVENT_ARC_DEVICE_ADDED,
    EVENT_ARC_DEVICE_REMOVED,
    EVENT_ARC_DELTA,
    EVENT_HID_DEVICE_ADDED,
    EVENT_HID_DEVICE_REMOVED,
    EVENT_HID_EVENT,
    EVENT_CROW_DEVICE_ADDED,
    EVENT_CROW_DEVICE_REMOVED,
    EVENT_CROW_EVENT,
    EVENT_OSC,
    EVENT_CLOCK,
    EVENT_METRO,
    EVENT_SYSTEM_CMD,
    EVENT_ENGINE,
    EVENT_SOFTCUT,
    EVENT_CUSTOM,
    EVENT_QUIT,
    // Count must be last
    EVENT_COUNT
} event_type_t;

// Event data structures for each event type
typedef struct {
    int number;
    bool state;
} event_key_t;

typedef struct {
    int number;
    int delta;
} event_encoder_t;

typedef struct {
    float percent;
    float current;
} event_battery_t;

typedef struct {
    bool present;
} event_power_t;

typedef struct {
    float disk;
    float temp;
    float cpu;
    float cpu1;
    float cpu2;
    float cpu3;
    float cpu4;
} event_system_stats_t;

typedef struct {
    float input_level;
    float output_level;
} event_audio_level_t;

// Main event data union
typedef struct {
    event_type_t type;
    union {
        event_key_t key;
        event_encoder_t encoder;
        event_battery_t battery;
        event_power_t power;
        event_system_stats_t system;
        event_audio_level_t audio;
        // Add other event data types as needed
        // This will be expanded as we migrate the full system

        // Keep compatibility with existing event system
        void *_legacy_data;
    } data;
} event_t;

// Event handler type
typedef void (*event_handler_t)(event_t *event, void *user_data);

/**
 * Initialize the event system
 *
 * @return true if initialization was successful
 */
bool event_system_init(void);

/**
 * Clean up event system resources
 */
void event_system_deinit(void);

/**
 * Post an event to the system
 *
 * @param event The event to post
 */
void event_post(event_t *event);

/**
 * Subscribe to events of a specific type
 *
 * @param type The event type to subscribe to
 * @param handler The function to call when an event of this type occurs
 * @param user_data User data to pass to the handler
 * @return A handler ID that can be used to unsubscribe
 */
int event_subscribe(event_type_t type, event_handler_t handler, void *user_data);

/**
 * Unsubscribe from events
 *
 * @param type The event type to unsubscribe from
 * @param handler_id The handler ID returned from event_subscribe
 */
void event_unsubscribe(event_type_t type, int handler_id);

/**
 * Process pending events
 *
 * This function processes all pending events in the queue.
 * Call this regularly from the main event loop.
 */
void events_process(void);

/**
 * Create a new event
 *
 * @param type The event type
 * @return A new event structure with the specified type
 */
event_t *event_create(event_type_t type);

/**
 * Free an event
 *
 * @param event The event to free
 */
void event_free(event_t *event);
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "event_system.h"
#include "unity.h"

// Test data for event handling
static int event_handler_call_count = 0;
static event_type_t last_event_type = EVENT_COUNT;
static bool handler_user_data_correct = false;

// Event handler for testing
static void test_event_handler(event_t *event, void *user_data) {
    event_handler_call_count++;
    last_event_type = event->type;

    // Check user data is passed correctly
    handler_user_data_correct = (user_data != NULL && *((int *)user_data) == 42);
}

// Test initialization and cleanup of event system
void test_event_system_init(void) {
    bool result = event_system_init();
    TEST_ASSERT_TRUE(result);

    // Make sure we can initialize without errors
    event_system_deinit();
}

// Test event creation and freeing
void test_event_system_create_free(void) {
    // Create different types of events and make sure they're created with right type
    for (event_type_t type = 0; type < EVENT_COUNT; type++) {
        event_t *event = event_create(type);
        TEST_ASSERT_NOT_NULL(event);
        TEST_ASSERT_EQUAL(type, event->type);
        event_free(event);
    }
}

// Test event posting and processing
void test_event_system_post_process(void) {
    bool result = event_system_init();
    TEST_ASSERT_TRUE(result);

    // Subscribe to the event type we'll test
    int user_data = 42;
    int handler_id = event_subscribe(EVENT_KEY_PRESS, test_event_handler, &user_data);
    TEST_ASSERT_GREATER_OR_EQUAL(0, handler_id);

    // Reset our test counters
    event_handler_call_count = 0;
    last_event_type = EVENT_COUNT;
    handler_user_data_correct = false;

    // Create and post an event
    event_t *event = event_create(EVENT_KEY_PRESS);
    TEST_ASSERT_NOT_NULL(event);

    event->data.key.number = 1;
    event->data.key.state = true;

    event_post(event);

    // Process the event
    events_process();

    // Check that our handler was called with the right event
    TEST_ASSERT_EQUAL(1, event_handler_call_count);
    TEST_ASSERT_EQUAL(EVENT_KEY_PRESS, last_event_type);
    TEST_ASSERT_TRUE(handler_user_data_correct);

    // Clean up
    event_unsubscribe(EVENT_KEY_PRESS, handler_id);
    event_system_deinit();
}

// Test event subscription and unsubscription
void test_event_system_subscribe_unsubscribe(void) {
    bool result = event_system_init();
    TEST_ASSERT_TRUE(result);

    // Clear counters
    event_handler_call_count = 0;
    last_event_type = EVENT_COUNT;

    // Test data
    int user_data = 42;

    // Subscribe to multiple event types
    int handler_id1 = event_subscribe(EVENT_KEY_PRESS, test_event_handler, &user_data);
    int handler_id2 = event_subscribe(EVENT_ENCODER_DELTA, test_event_handler, &user_data);

    TEST_ASSERT_GREATER_OR_EQUAL(0, handler_id1);
    TEST_ASSERT_GREATER_OR_EQUAL(0, handler_id2);
    TEST_ASSERT_NOT_EQUAL(handler_id1, handler_id2); // IDs should be unique

    // Post an event of first type
    event_t *event1 = event_create(EVENT_KEY_PRESS);
    event_post(event1);

    // Process events
    events_process();

    // Verify handling
    TEST_ASSERT_EQUAL(1, event_handler_call_count);
    TEST_ASSERT_EQUAL(EVENT_KEY_PRESS, last_event_type);

    // Post an event of second type
    event_t *event2 = event_create(EVENT_ENCODER_DELTA);
    event_post(event2);

    // Process events
    events_process();

    // Verify handling
    TEST_ASSERT_EQUAL(2, event_handler_call_count);
    TEST_ASSERT_EQUAL(EVENT_ENCODER_DELTA, last_event_type);

    // Unsubscribe from first type
    event_unsubscribe(EVENT_KEY_PRESS, handler_id1);

    // Post an event of first type again
    event_t *event3 = event_create(EVENT_KEY_PRESS);
    event_post(event3);

    // Process events
    events_process();

    // Verify the handler was not called (count should still be 2)
    TEST_ASSERT_EQUAL(2, event_handler_call_count);
    TEST_ASSERT_EQUAL(EVENT_ENCODER_DELTA, last_event_type);

    // Post an event of second type again
    event_t *event4 = event_create(EVENT_ENCODER_DELTA);
    event_post(event4);

    // Process events
    events_process();

    // Verify the handler was called
    TEST_ASSERT_EQUAL(3, event_handler_call_count);
    TEST_ASSERT_EQUAL(EVENT_ENCODER_DELTA, last_event_type);

    // Unsubscribe from second type
    event_unsubscribe(EVENT_ENCODER_DELTA, handler_id2);

    // Post both types of events
    event_t *event5 = event_create(EVENT_KEY_PRESS);
    event_t *event6 = event_create(EVENT_ENCODER_DELTA);
    event_post(event5);
    event_post(event6);

    // Process events
    events_process();
    events_process();

    // Verify the handler was not called (count should still be 3)
    TEST_ASSERT_EQUAL(3, event_handler_call_count);

    // Clean up
    event_system_deinit();
}
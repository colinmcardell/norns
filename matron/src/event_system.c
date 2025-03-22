#include <assert.h>
#include <pthread.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "event_system.h"

// Event queue node structure
typedef struct event_node {
    struct event_node *next;
    struct event_node *prev;
    event_t *event;
} event_node_t;

// Event queue structure
typedef struct {
    event_node_t *head;
    event_node_t *tail;
    size_t size;
    pthread_cond_t nonempty;
    pthread_mutex_t lock;
} event_queue_t;

// Event handler node structure
typedef struct {
    event_handler_t handler;
    void *user_data;
    int id;
} event_handler_node_t;

// Global variables
static event_queue_t event_queue;
static event_handler_node_t *event_handlers[EVENT_COUNT];
static size_t event_handler_counts[EVENT_COUNT];
static int next_handler_id = 1;
static bool quit_flag = false;

// Initialize event system
bool event_system_init(void) {
    // Initialize queue
    event_queue.size = 0;
    event_queue.head = NULL;
    event_queue.tail = NULL;

    // Initialize synchronization primitives
    if (pthread_mutex_init(&event_queue.lock, NULL) != 0) {
        return false;
    }

    if (pthread_cond_init(&event_queue.nonempty, NULL) != 0) {
        pthread_mutex_destroy(&event_queue.lock);
        return false;
    }

    // Initialize handler arrays
    memset(event_handlers, 0, sizeof(event_handlers));
    memset(event_handler_counts, 0, sizeof(event_handler_counts));

    return true;
}

// Clean up event system
void event_system_deinit(void) {
    // Free any events in the queue
    pthread_mutex_lock(&event_queue.lock);

    event_node_t *node = event_queue.head;
    while (node != NULL) {
        event_node_t *next = node->next;
        event_free(node->event);
        free(node);
        node = next;
    }

    // Reset queue
    event_queue.head = NULL;
    event_queue.tail = NULL;
    event_queue.size = 0;

    pthread_mutex_unlock(&event_queue.lock);

    // Destroy synchronization primitives
    pthread_mutex_destroy(&event_queue.lock);
    pthread_cond_destroy(&event_queue.nonempty);

    // Free handler arrays
    for (int i = 0; i < EVENT_COUNT; i++) {
        free(event_handlers[i]);
        event_handlers[i] = NULL;
        event_handler_counts[i] = 0;
    }
}

// Create a new event
event_t *event_create(event_type_t type) {
    event_t *event = (event_t *)calloc(1, sizeof(event_t));
    if (event) {
        event->type = type;
    }
    return event;
}

// Free an event
void event_free(event_t *event) {
    // Handle special cases that need additional cleanup
    // To be expanded with actual resource cleanup as needed

    free(event);
}

// Add an event to the queue
void event_post(event_t *event) {
    assert(event != NULL);

    // Create a new node for the event
    event_node_t *node = (event_node_t *)calloc(1, sizeof(event_node_t));
    if (!node) {
        // Memory allocation failed
        event_free(event);
        return;
    }

    node->event = event;

    // Lock the queue
    pthread_mutex_lock(&event_queue.lock);

    // Signal if the queue was empty
    if (event_queue.size == 0) {
        pthread_cond_signal(&event_queue.nonempty);
    }

    // Add to the queue
    if (event_queue.size == 0) {
        // First node
        insque(node, NULL);
        event_queue.head = node;
    } else {
        // Add to tail
        insque(node, event_queue.tail);
    }

    event_queue.tail = node;
    event_queue.size++;

    // Unlock the queue
    pthread_mutex_unlock(&event_queue.lock);
}

// Subscribe to events
int event_subscribe(event_type_t type, event_handler_t handler, void *user_data) {
    if (type >= EVENT_COUNT || handler == NULL) {
        return -1;
    }

    // Allocate or expand the handlers array for this event type
    size_t new_count = event_handler_counts[type] + 1;
    event_handler_node_t *new_handlers =
        (event_handler_node_t *)realloc(event_handlers[type], new_count * sizeof(event_handler_node_t));

    if (!new_handlers) {
        return -1;
    }

    // Add the new handler
    event_handlers[type] = new_handlers;
    event_handler_node_t *node = &new_handlers[event_handler_counts[type]];
    node->handler = handler;
    node->user_data = user_data;
    node->id = next_handler_id++;

    event_handler_counts[type] = new_count;

    return node->id;
}

// Unsubscribe from events
void event_unsubscribe(event_type_t type, int handler_id) {
    if (type >= EVENT_COUNT) {
        return;
    }

    // Find the handler with the given ID
    for (size_t i = 0; i < event_handler_counts[type]; i++) {
        if (event_handlers[type][i].id == handler_id) {
            // Remove this handler by shifting all subsequent handlers
            if (i < event_handler_counts[type] - 1) {
                memmove(&event_handlers[type][i], &event_handlers[type][i + 1],
                        (event_handler_counts[type] - i - 1) * sizeof(event_handler_node_t));
            }

            event_handler_counts[type]--;

            // Resize the array if it's not empty
            if (event_handler_counts[type] > 0) {
                event_handlers[type] = (event_handler_node_t *)realloc(
                    event_handlers[type], event_handler_counts[type] * sizeof(event_handler_node_t));
            } else {
                // Free the array if it's empty
                free(event_handlers[type]);
                event_handlers[type] = NULL;
            }

            return;
        }
    }
}

// Process a single event
static void process_event(event_t *event) {
    if (event->type >= EVENT_COUNT) {
        return;
    }

    // Handle quit event specially
    if (event->type == EVENT_QUIT) {
        quit_flag = true;
        return;
    }

    // Call all registered handlers for this event type
    for (size_t i = 0; i < event_handler_counts[event->type]; i++) {
        event_handlers[event->type][i].handler(event, event_handlers[event->type][i].user_data);
    }
}

// Process pending events
void events_process(void) {
    event_t *event = NULL;

    pthread_mutex_lock(&event_queue.lock);

    // Check if there are any events
    if (event_queue.size > 0) {
        // Get the oldest event
        event_node_t *node = event_queue.head;
        event = node->event;

        // Update the head pointer
        event_queue.head = node->next;
        if (node == event_queue.tail) {
            // This was the last node
            assert(event_queue.size == 1);
            event_queue.tail = NULL;
        }

        // Remove from the queue
        remque(node);
        free(node);

        // Update size
        event_queue.size--;
    }

    pthread_mutex_unlock(&event_queue.lock);

    // Process the event if we got one
    if (event != NULL) {
        process_event(event);
        event_free(event);
    }
}

// Main event loop (blocks until quit)
void event_loop(void) {
    while (!quit_flag) {
        event_t *event = NULL;

        pthread_mutex_lock(&event_queue.lock);

        // Wait if the queue is empty
        while (event_queue.size == 0 && !quit_flag) {
            pthread_cond_wait(&event_queue.nonempty, &event_queue.lock);
        }

        // Get an event if available
        if (event_queue.size > 0) {
            event_node_t *node = event_queue.head;
            event = node->event;

            // Update the head pointer
            event_queue.head = node->next;
            if (node == event_queue.tail) {
                // This was the last node
                assert(event_queue.size == 1);
                event_queue.tail = NULL;
            }

            // Remove from the queue
            remque(node);
            free(node);

            // Update size
            event_queue.size--;
        }

        pthread_mutex_unlock(&event_queue.lock);

        // Process the event if we got one
        if (event != NULL) {
            process_event(event);
            event_free(event);
        }
    }
}

// Check if should quit
bool event_should_quit(void) {
    return quit_flag;
}

// Request quit
void event_request_quit(void) {
    event_t *event = event_create(EVENT_QUIT);
    if (event) {
        event_post(event);
    } else {
        // Fallback if allocation fails
        quit_flag = true;
    }
}
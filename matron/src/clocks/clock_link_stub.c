// Stub implementation for clock_link functionality when Ableton Link is disabled
#include "clock_link.h"

void clock_link_init(void) {
}

void clock_link_start(void) {
}

void clock_link_stop(void) {
}

double clock_link_get_tempo(void) {
    return 120.0;
}

double clock_link_get_beat(void) {
    return 0.0;
}

void clock_link_set_tempo(double bpm) {
}

void clock_link_set_quantum(double quantum) {
}

void clock_link_set_beat(double beat) {
}

int clock_link_enabled(void) {
    return 0;
}

void clock_link_join_session(void) {
}

void clock_link_leave_session(void) {
}

#pragma once

#include <stdbool.h>

void clock_link_init(void);
void clock_link_start(void);
void clock_link_stop(void);
double clock_link_get_tempo(void);
double clock_link_get_beat(void);
void clock_link_set_tempo(double bpm);
void clock_link_set_quantum(double quantum);
void clock_link_set_beat(double beat);
int clock_link_enabled(void);
void clock_link_join_session(void);
void clock_link_leave_session(void);
void clock_link_set_transport_start(void);
void clock_link_set_transport_stop(void);
void clock_link_set_start_stop_sync(bool sync_enabled);

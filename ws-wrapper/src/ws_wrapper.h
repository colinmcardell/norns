/*
 * ws-wrapper
 *
 * this utility launches an arbitrary executable as a child process,
 * and binds the child's standard I/O to a ws socket
 *
 */

#ifndef WS_WRAPPER_H
#define WS_WRAPPER_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// Function declarations
void bind_sock(int *sock, int *eid, char *url);
void *loop_rx(void *p);
void *loop_tx(void *p);
void launch_thread(pthread_t *tid, void *(*start_routine)(void *), void *data);
int launch_exe(int argc, char **argv);
void quit(void);

#endif /* WS_WRAPPER_H */

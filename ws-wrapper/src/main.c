/*
 * ws-wrapper
 *
 * this utility launches an arbitrary executable as a child process,
 * and binds the child's standard I/O to a ws socket
 *
 */

#include "ws_wrapper.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: ws-wrapper WS_SOCKET BINARY <child args...>");
        return 1;
    }

    setvbuf(stdout, NULL, _IONBF, 0);
    return launch_exe(argc, argv);
}

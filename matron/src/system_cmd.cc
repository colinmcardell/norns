#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "events.h"
#include "sidecar.h"

#include "system_cmd.h"

//----------------------------
//--- types and variables

#ifdef NORNS_TEST
void *(*system_cmd_test_realloc)(void *, size_t) = realloc;
#define CAPTURE_REALLOC system_cmd_test_realloc
#else
#define CAPTURE_REALLOC realloc
#endif

// accumulated command output, capped at SYSTEM_CMD_CAPTURE_MAX
typedef struct {
    char *buf;
    size_t len;
    bool failed;
    bool truncated;
    int cb_ref;              // unused on the sync path
    sidecar_status_t status; // sync path only
} capture_t;

static void capture_append(void *ctx, const char *chunk, size_t size) {
    capture_t *cap = (capture_t *)ctx;
    if (cap->truncated) {
        return;
    }
    if (size > SYSTEM_CMD_CAPTURE_MAX - cap->len) {
        size = SYSTEM_CMD_CAPTURE_MAX - cap->len;
        cap->truncated = true;
        fprintf(stderr, "system_cmd: output exceeded cap, truncating\n");
    }
    if (size == 0) {
        return;
    }
    char *grown = (char *)CAPTURE_REALLOC(cap->buf, cap->len + size);
    if (grown == NULL) {
        cap->truncated = true;
        fprintf(stderr, "system_cmd: out of memory, truncating\n");
        return;
    }
    memcpy(grown + cap->len, chunk, size);
    cap->buf = grown;
    cap->len += size;
}

static void capture_discard_on_transport_failure(capture_t *cap, sidecar_result_t result) {
    if (result != SIDECAR_TRANSPORT_FAILED) {
        return;
    }
    cap->failed = true;
    free(cap->buf);
    cap->buf = NULL;
    cap->len = 0;
}

static void capture_null_terminate(capture_t *cap) {
    char *out = (char *)CAPTURE_REALLOC(cap->buf, cap->len + 1);
    if (out == NULL) {
        fprintf(stderr, "system_cmd: out of memory, dropping output\n");
        free(cap->buf);
        cap->truncated = true;
        cap->len = 0;
        out = (char *)calloc(1, 1);
        if (out == NULL) {
            cap->buf = NULL;
            return;
        }
    }
    out[cap->len] = '\0';
    cap->buf = out;
    cap->len += 1;
}

// FIXME the settings password change passes a plaintext password through here,
// and stderr reaches the journal and maiden. fixing it needs a way to pass a
// secret on stdin rather than in the command string
static void log_cmd_failure(const char *label, const char *cmd, const char *err) {
    fprintf(stderr, "%s: command (%s) failed: %s\n", label, cmd != NULL ? cmd : "", err != NULL ? err : "unknown error");
}

static void post_cmd_done(const char *cmd, void *ctx, const sidecar_status_t *status) {
    capture_t *cap = (capture_t *)ctx;
    capture_discard_on_transport_failure(cap, status->result);

    if (status->result == SIDECAR_TRANSPORT_FAILED) {
        log_cmd_failure("system_cmd", cmd, status->err);
    }

    // FIXME failure and silent success both arrive as "". telling them apart
    // needs an extra callback argument, which changes the signature every
    // script's norns.system_cmd callback is written against
    capture_null_terminate(cap);
    union event_data *ev = event_data_new(EVENT_SYSTEM_CMD);
    ev->system_cmd.capture = cap->buf; // freed when the event is handled
    ev->system_cmd.cb_ref = cap->cb_ref;
    event_post(ev);
    free(cap);
}

static void finish_sync_capture(const char *cmd, void *ctx, const sidecar_status_t *status) {
    (void)cmd;
    capture_t *cap = (capture_t *)ctx;
    capture_discard_on_transport_failure(cap, status->result);
    cap->status = *status;
    cap->status.err = NULL;
}

//-------------------------------
//-- extern function definitions

bool system_cmd(const char *cmd, int cb_ref) {
    capture_t *cap = (capture_t *)calloc(1, sizeof(capture_t));
    if (cap == NULL) {
        return false;
    }
    cap->cb_ref = cb_ref;
    if (!sidecar_client_cmd_async(cmd, SIDECAR_CMD_TIMEOUT_DEFAULT_MS, cap, capture_append, post_cmd_done)) {
        free(cap);
        return false;
    }
    return true;
}

bool system_cmd_sync(const char *cmd, char **out, size_t *size, sidecar_status_t *status, uint32_t timeout_ms) {
    capture_t cap = {};
    sidecar_client_cmd(cmd, timeout_ms, &cap, capture_append, finish_sync_capture);
    if (status != NULL) {
        *status = cap.status;
    }
    if (cap.failed) {
        *out = NULL;
        *size = 0;
        return false;
    }
    capture_null_terminate(&cap);
    *out = cap.buf;
    *size = cap.len;
    return true;
}

// tests for matron/src/system_cmd.cc

#include <cstdlib>
#include <cstring>
#include <doctest/doctest.h>
#include <vector>

#include "events.h"
#include "system_cmd.h"

#include "sidecar.h"
#include "system_cmd_stubs.h"

// helpers for the tests below.
namespace {

void start_async(int cb_ref) {
    system_cmd_stubs_reset();
    REQUIRE(system_cmd("x", cb_ref));
    REQUIRE(g_last_on_chunk != nullptr);
    REQUIRE(g_last_on_done != nullptr);
    REQUIRE(g_last_ctx != nullptr);
}

void emit_chunk(const char *chunk, size_t size) {
    g_last_on_chunk(g_last_ctx, chunk, size);
}

void emit_done(sidecar_result_t result, const char *err) {
    const sidecar_status_t status = {result, false, result == SIDECAR_OK ? 0 : 1, err};
    g_last_on_done(g_last_cmd, g_last_ctx, &status);
}

void free_event(union event_data *ev) {
    free(ev->system_cmd.capture);
    free(ev);
}

void *failing_realloc(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    return nullptr;
}

} // namespace

TEST_CASE("system_cmd: chunks accumulate into one whole-output event") {
    start_async(7);
    emit_chunk("hel", 3);
    emit_chunk("lo", 2);
    CHECK(g_event_post_count == 0); // nothing posted until the command is done
    emit_done(SIDECAR_OK, nullptr);

    CHECK(g_event_post_count == 1);
    REQUIRE(g_last_event != nullptr);
    CHECK(g_last_event->system_cmd.cb_ref == 7);
    REQUIRE(g_last_event->system_cmd.capture != nullptr);
    CHECK(std::strcmp(g_last_event->system_cmd.capture, "hello") == 0);

    free_event(g_last_event);
}

TEST_CASE("system_cmd: a silent successful command posts one event with empty capture") {
    start_async(11);
    emit_done(SIDECAR_OK, nullptr); // no chunks

    CHECK(g_event_post_count == 1);
    REQUIRE(g_last_event != nullptr);
    CHECK(g_last_event->system_cmd.cb_ref == 11);
    REQUIRE(g_last_event->system_cmd.capture != nullptr);
    CHECK(g_last_event->system_cmd.capture[0] == '\0');

    free_event(g_last_event);
}

TEST_CASE("system_cmd: transport failure posts one event with empty capture") {
    start_async(42);
    emit_chunk("par", 3); // partial output, discarded on failure
    emit_done(SIDECAR_TRANSPORT_FAILED, "sidecar did not answer");

    CHECK(g_event_post_count == 1);
    REQUIRE(g_last_event != nullptr);
    CHECK(g_last_event->system_cmd.cb_ref == 42);
    REQUIRE(g_last_event->system_cmd.capture != nullptr);
    CHECK(g_last_event->system_cmd.capture[0] == '\0');

    free_event(g_last_event);
}

TEST_CASE("system_cmd: a failed enqueue is reported and posts nothing") {
    system_cmd_stubs_reset();
    g_cmd_async_result = false;

    CHECK_FALSE(system_cmd("x", 8));
    CHECK(g_event_post_count == 0);
}

TEST_CASE("system_cmd: non-zero exit delivers output without the error text") {
    start_async(9);
    emit_chunk("out", 3);
    emit_done(SIDECAR_CMD_FAILED, "exit status 1");

    CHECK(g_event_post_count == 1);
    REQUIRE(g_last_event != nullptr);
    CHECK(g_last_event->system_cmd.cb_ref == 9);
    REQUIRE(g_last_event->system_cmd.capture != nullptr);
    CHECK(std::strcmp(g_last_event->system_cmd.capture, "out") == 0);

    free_event(g_last_event);
}

TEST_CASE("system_cmd: accumulated output is capped, command still completes") {
    start_async(21);
    emit_chunk("hello", 5);

    std::vector<char> block(256 * 1024, 'x');
    for (int i = 0; i < 5; ++i) {
        emit_chunk(block.data(), block.size());
    }
    CHECK(g_event_post_count == 0);
    emit_done(SIDECAR_OK, nullptr);

    CHECK(g_event_post_count == 1);
    REQUIRE(g_last_event != nullptr);
    CHECK(g_last_event->system_cmd.cb_ref == 21);
    REQUIRE(g_last_event->system_cmd.capture != nullptr);
    CHECK(std::strlen(g_last_event->system_cmd.capture) == SYSTEM_CMD_CAPTURE_MAX);
    CHECK(std::strncmp(g_last_event->system_cmd.capture, "hello", 5) == 0);

    free_event(g_last_event);
}

TEST_CASE("system_cmd: output that cannot be stored truncates, leaving no gap") {
    start_async(22);
    emit_chunk("111", 3);

    system_cmd_test_realloc = failing_realloc;
    emit_chunk("222", 3);
    system_cmd_test_realloc = realloc; // allocation works again

    emit_chunk("333", 3);
    emit_done(SIDECAR_OK, nullptr);

    CHECK(g_event_post_count == 1);
    REQUIRE(g_last_event != nullptr);
    CHECK(g_last_event->system_cmd.cb_ref == 22);
    REQUIRE(g_last_event->system_cmd.capture != nullptr);
    CHECK(std::strcmp(g_last_event->system_cmd.capture, "111") == 0); // not "111333"

    free_event(g_last_event);
}

TEST_CASE("system_cmd: output that cannot be terminated is dropped, command still completes") {
    start_async(23);
    emit_chunk("111", 3);

    system_cmd_test_realloc = failing_realloc;
    emit_done(SIDECAR_OK, nullptr);
    system_cmd_test_realloc = realloc;

    CHECK(g_event_post_count == 1);
    REQUIRE(g_last_event != nullptr);
    CHECK(g_last_event->system_cmd.cb_ref == 23);
    REQUIRE(g_last_event->system_cmd.capture != nullptr);
    CHECK(g_last_event->system_cmd.capture[0] == '\0');

    free_event(g_last_event);
}

TEST_CASE("system_cmd_sync: concatenates chunks into a null character terminated buffer") {
    system_cmd_stubs_reset();
    g_sync_chunks[0] = "hel";
    g_sync_chunks[1] = "lo";
    g_sync_chunk_count = 2;

    char *out = nullptr;
    size_t size = 0;
    REQUIRE(system_cmd_sync("x", &out, &size));
    CHECK(std::strcmp(g_last_cmd, "x") == 0);
    REQUIRE(out != nullptr);
    CHECK(std::strcmp(out, "hello") == 0);
    CHECK(size == 6); // includes the terminator

    free(out);
}

TEST_CASE("system_cmd_sync: no output yields empty string with size 1") {
    system_cmd_stubs_reset();

    char *out = nullptr;
    size_t size = 0;
    REQUIRE(system_cmd_sync("x", &out, &size));
    REQUIRE(out != nullptr);
    CHECK(out[0] == '\0');
    CHECK(size == 1);

    free(out);
}

TEST_CASE("system_cmd_sync: non-zero exit keeps the output and still succeeds") {
    system_cmd_stubs_reset();
    g_sync_chunks[0] = "partial";
    g_sync_chunk_count = 1;
    g_sync_result = SIDECAR_CMD_FAILED;
    g_sync_errmsg = "exit status 1";

    // matching os.execute, a command that runs and fails still returns what
    // it printed. only a transport failure discards the output
    char *out = nullptr;
    size_t size = 0;
    CHECK(system_cmd_sync("x", &out, &size));
    REQUIRE(out != nullptr);
    CHECK(std::strcmp(out, "partial") == 0);

    free(out);
}

TEST_CASE("system_cmd_sync: transport failure yields NULL out and returns false") {
    system_cmd_stubs_reset();
    g_sync_chunks[0] = "par"; // partial output, discarded on failure
    g_sync_chunk_count = 1;
    g_sync_result = SIDECAR_TRANSPORT_FAILED;
    g_sync_errmsg = "sidecar did not answer";

    char sentinel = 0;
    char *out = &sentinel;
    size_t size = 99;
    CHECK_FALSE(system_cmd_sync("x", &out, &size));
    CHECK(out == nullptr);
    CHECK(size == 0);
}

//---------------------------------
//--- how a command ended

TEST_CASE("system_cmd_sync: a command that exits zero reports OK") {
    system_cmd_stubs_reset();

    char *out = nullptr;
    size_t size = 0;
    sidecar_status_t status = {};
    REQUIRE(system_cmd_sync("x", &out, &size, &status));
    CHECK(status.result == SIDECAR_OK);
    CHECK_FALSE(status.signalled);
    CHECK(status.code == 0);

    free(out);
}

TEST_CASE("system_cmd_sync: a non-zero exit reports its own code") {
    system_cmd_stubs_reset();
    g_sync_result = SIDECAR_CMD_FAILED;
    g_sync_code = 7;

    char *out = nullptr;
    size_t size = 0;
    sidecar_status_t status = {};
    CHECK(system_cmd_sync("x", &out, &size, &status));
    CHECK(status.result == SIDECAR_CMD_FAILED);
    CHECK_FALSE(status.signalled);
    CHECK(status.code == 7);

    free(out);
}

TEST_CASE("system_cmd_sync: a signalled command is told apart from an exit") {
    system_cmd_stubs_reset();
    g_sync_result = SIDECAR_CMD_FAILED;
    g_sync_signalled = true;
    g_sync_code = 9;

    char *out = nullptr;
    size_t size = 0;
    sidecar_status_t status = {};
    CHECK(system_cmd_sync("x", &out, &size, &status));
    CHECK(status.signalled);
    CHECK(status.code == 9);

    free(out);
}

TEST_CASE("system_cmd_sync: a transport failure still fills in the status") {
    system_cmd_stubs_reset();
    g_sync_result = SIDECAR_TRANSPORT_FAILED;
    g_sync_errmsg = "sidecar did not answer";

    char *out = nullptr;
    size_t size = 0;
    sidecar_status_t status = {};
    CHECK_FALSE(system_cmd_sync("x", &out, &size, &status));
    CHECK(status.result == SIDECAR_TRANSPORT_FAILED);
}

TEST_CASE("system_cmd_sync: the status err text does not outlive the call") {
    system_cmd_stubs_reset();
    g_sync_result = SIDECAR_CMD_FAILED;
    g_sync_errmsg = "exit status 1";
    g_sync_code = 1;

    char *out = nullptr;
    size_t size = 0;
    sidecar_status_t status = {};
    CHECK(system_cmd_sync("x", &out, &size, &status));
    CHECK(status.err == nullptr);

    free(out);
}

TEST_CASE("system_cmd_sync: a caller wanting no status still works") {
    system_cmd_stubs_reset();
    g_sync_result = SIDECAR_CMD_FAILED;
    g_sync_code = 3;

    char *out = nullptr;
    size_t size = 0;
    CHECK(system_cmd_sync("x", &out, &size));
    REQUIRE(out != nullptr);

    free(out);
}

//---------------------------------
//--- silence ceilings

TEST_CASE("system_cmd_sync: the default ceiling travels with the request") {
    system_cmd_stubs_reset();

    char *out = nullptr;
    size_t size = 0;
    REQUIRE(system_cmd_sync("x", &out, &size));
    CHECK(g_last_timeout_ms == SIDECAR_CMD_TIMEOUT_DEFAULT_MS);

    free(out);
}

TEST_CASE("system_cmd_sync: a caller can ask for no ceiling at all") {
    system_cmd_stubs_reset();

    char *out = nullptr;
    size_t size = 0;
    sidecar_status_t status = {};
    REQUIRE(system_cmd_sync("x", &out, &size, &status, SIDECAR_CMD_TIMEOUT_NONE));
    CHECK(g_last_timeout_ms == 0);

    free(out);
}

TEST_CASE("system_cmd: the async path keeps the default ceiling") {
    system_cmd_stubs_reset();
    REQUIRE(system_cmd("x", 5));
    CHECK(g_last_timeout_ms == SIDECAR_CMD_TIMEOUT_DEFAULT_MS);

    emit_done(SIDECAR_OK, nullptr);
    free_event(g_last_event);
}

/* Synthetic host checks for request submission and the wait/poll contract. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/resource_submit.h"

#if defined(DOTR_TEST_QUEUE_SELECT)
/* Synthetic queue fixtures for the head-trimming priority selector. */
struct DotrResourceSubmitState dotr_ee_resource_submit_state_0031c2f0;
extern int lSzFile_NextRequest(void);

static struct DotrResourceSubmitState *const state = &dotr_ee_resource_submit_state_0031c2f0;

static void queue(unsigned short head, unsigned short count)
{
    int i;
    memset(state, 0x5a, sizeof(*state));
    for (i = 0; i < 256; i++) {
        state->records[i].status = -1;
        state->records[i].priority = 0x7fff; /* dead records must never win */
    }
    state->head = head;
    state->count = count;
}

static void live(int index, short priority)
{
    state->records[index].status = 0;
    state->records[index].priority = priority;
}

int main(void)
{
    /* Empty queue: nothing is selected or changed. */
    queue(9, 0);
    assert(lSzFile_NextRequest() == -1 && state->head == 9 && state->count == 0);

    /* Dead head records are dropped; the head stays put when the count reaches zero. */
    queue(4, 3);
    assert(lSzFile_NextRequest() == -1 && state->head == 6 && state->count == 0);
    queue(255, 1);
    assert(lSzFile_NextRequest() == -1 && state->head == 255 && state->count == 0);

    /* Trimming stops at the first live record and wraps the head at 256. */
    queue(254, 5);
    live(1, 3);
    assert(lSzFile_NextRequest() == 1 && state->head == 1 && state->count == 2);

    /* Strictly greater priority wins, so the earliest of equals is kept. */
    queue(10, 6);
    live(10, 2); live(12, 7); live(13, 7); live(15, 1);
    assert(lSzFile_NextRequest() == 12 && state->head == 10 && state->count == 6);

    /* Dead records behind a live head are skipped, not removed. */
    queue(10, 3);
    live(10, 0);
    assert(lSzFile_NextRequest() == 10 && state->head == 10 && state->count == 3);

    /* The scan wraps and ignores live records beyond the count. */
    queue(254, 4);
    live(254, 1); live(0, 9); live(2, 0x7ffe);
    assert(lSzFile_NextRequest() == 0 && state->head == 254 && state->count == 4);

    /* Signed halfword priority: negative priorities never beat the -1 start. */
    queue(0, 2);
    live(0, -1); live(1, -5);
    assert(lSzFile_NextRequest() == -1 && state->head == 0 && state->count == 2);
    live(1, 0);
    assert(lSzFile_NextRequest() == 1);

    /* A full queue is scanned through index 255 and back to the head. */
    queue(128, 256);
    live(128, 0); live(127, 5);
    assert(lSzFile_NextRequest() == 127 && state->head == 128 && state->count == 256);

    /* Storage outside head, count and the trimmed queue is untouched. */
    assert(state->unknown_0000[0] == 0x5a && state->unknown_6480[0] == 0x5a);
    assert(state->records[127].status == 0 && state->records[126].status == -1);
    puts("resource_queue_select: ok");
    return 0;
}
#elif defined(DOTR_TEST_REQUEST_SETUP)
/* Request setup (0x1feda0) with string, lookup and file dependencies stubbed.
 * The stubs' string semantics are assumptions.
 */
struct DotrResourceSubmitEntry dotr_ee_resource_submit_entries_00293090[97];
char data_002d0638[] = "synthetic-label";
char data_002d063d[] = "/SHARED.BIN";

enum { LOOKUP = 1, COPY, FIND, LENGTH, COMPARE, GROUP, OPEN, ATTACH };

static char entry_names[97][24];
static int trace[512], trace_count;
static int directory_present, group_result, group_argument, open_result, attached_handle;
static const char *opened_name;

static void record_event(int event)
{
    assert(trace_count < (int)(sizeof(trace) / sizeof(trace[0])));
    trace[trace_count++] = event;
}

static int count_events(int event)
{
    int i, total = 0;
    for (i = 0; i < trace_count; i++) {
        total += trace[i] == event;
    }
    return total;
}

void lSzFile_ReadCB(struct DotrResourceSubmitRecord *record, int completed)
{
    (void)record;
    (void)completed;
    assert(0);
}

char *func_00197548(const char *label)
{
    assert(label == data_002d0638);
    record_event(LOOKUP);
    return directory_present ? data_002d0638 : 0;
}

char *func_0019bdd8(char *destination, const char *source)
{
    assert(strlen(source) < 128);
    record_event(COPY);
    return strcpy(destination, source);
}

char *func_0019c478(const char *string, int character)
{
    record_event(FIND);
    return strrchr(string, character);
}

int func_0019bef0(const char *string)
{
    record_event(LENGTH);
    return (int)strlen(string);
}

int func_0019c028(const char *left, const char *right, int length)
{
    record_event(COMPARE);
    return strncmp(left, right, (size_t)length);
}

int SzDuel_Xax2New(int group)
{
    record_event(GROUP);
    group_argument = group;
    return group_result;
}

int SzMem_StrDup(const char *name)
{
    record_event(OPEN);
    opened_name = name;
    return open_result;
}

void lSzFile_StrNormalize(char *text)
{
    record_event(ATTACH);
    attached_handle = (int)(DotrResourceSubmitWord)text;
}

static struct DotrResourceSubmitRecord record;

static void reset(void)
{
    int i;
    memset(&record, 0xff, sizeof(record));
    memset(dotr_ee_resource_submit_entries_00293090, 0, sizeof(dotr_ee_resource_submit_entries_00293090));
    for (i = 0; i < 97; i++) {
        sprintf(entry_names[i], "\\DIR\\FILE%02d.BIN;1", i);
        dotr_ee_resource_submit_entries_00293090[i].name = entry_names[i];
        dotr_ee_resource_submit_entries_00293090[i].first_sector = 0x1000 + i;
        dotr_ee_resource_submit_entries_00293090[i].sector_count = 3 + i;
    }
    strcpy(entry_names[40], "\\SHARED.BIN;1");
    strcpy(entry_names[5], "NOSEPARATOR");
    trace_count = 0;
    directory_present = 1;
    group_result = group_argument = 0;
    open_result = 77;
    attached_handle = 0;
    opened_name = 0;
}

static char *sector_name(unsigned int sector)
{
    return (char *)(DotrResourceSubmitWord)(0x80000000u | sector);
}

static int submit(int type, int priority, char *name, int byte_size, int byte_offset, int unknown_010)
{
    return lSzFile_SetReadRequestParam(&record, type, priority, name, 1234, byte_size, byte_offset, unknown_010,
                         lSzFile_ReadCB, -5678);
}

static void check_common_fields(int type, int priority, unsigned int size, unsigned int offset,
                                unsigned int unknown_010)
{
    assert(record.selector == (short)type && record.priority == priority);
    /* Expected values are the stored 31-bit patterns; the fields read back signed. */
    assert(((unsigned int)record.byte_size & 0x7fffffff) == size);
    assert(((unsigned int)record.byte_offset & 0x7fffffff) == offset);
    assert(((unsigned int)record.unknown_010 & 0x7fffffff) == unknown_010);
    assert(record.flag_008 == 0 && record.flag_00c == 0);
    assert(record.unknown_014 == 1234 && record.callback == lSzFile_ReadCB);
    assert(record.unknown_01c == -5678);
}

static void check_sector_mode(unsigned int sector)
{
    assert(record.handle == 0 && record.sector_mode == -1 && record.status == (int)sector);
    assert(count_events(OPEN) == 0 && count_events(ATTACH) == 0);
}

int main(void)
{
    static char plain[] = "host:plain.bin";
    static char slash[] = "dir/FILE07.BIN";
    static char backslash[] = "dir\\FILE96.BIN";
    static char missing[] = "dir/ABSENT.BIN";
    static char group[] = "dir/m123.GrP";
    static char group_upper[] = "dir\\M009.GRP";
    static char not_group[] = "dir/M12X.GRP";
    static char short_group[] = "dir/M1.GRP";
    struct DotrResourceSubmitRecord saved;

    assert(offsetof(struct DotrResourceSubmitRecord, selector) == 4);
    assert(offsetof(struct DotrResourceSubmitRecord, priority) == 6);
    assert(offsetof(struct DotrResourceSubmitRecord, unknown_014) == 0x14);
    assert(offsetof(struct DotrResourceSubmitEntry, first_sector) == 8);
    assert(offsetof(struct DotrResourceSubmitEntry, sector_count) == 0x18);
    assert(sizeof(struct DotrResourceSubmitEntry) == 32);

    /* The code tests bit 31 of the name pointer. Host addresses with that bit
     * set cannot represent ordinary target names, so skip (exit code 77, CTest's skip code).
     */
    {
        char *names[] = { plain, slash, backslash, missing, group, group_upper, not_group, short_group };
        size_t n;
        for (n = 0; n < sizeof(names) / sizeof(names[0]); n++) {
            if ((DotrResourceSubmitWord)names[n] & 0x80000000) {
                puts("skipped: host data addresses have bit 31 set");
                return 77;
            }
        }
    }

    /* Null record or name: no dependency is called and nothing is written. */
    reset();
    memcpy(&saved, &record, sizeof(saved));
    assert(lSzFile_SetReadRequestParam(0, 0x1400, 1, plain, 0, 0, 0, 0, lSzFile_ReadCB, 0) == 0);
    assert(submit(0x1400, 1, 0, 0, 0, 0) == 0);
    assert(trace_count == 0 && memcmp(&saved, &record, sizeof(saved)) == 0);

    /* Named request without a directory label: opened and attached by handle. */
    reset();
    directory_present = 0;
    assert(submit(0x1400, 9, slash, -1, 4096, -1) == 1);
    assert(trace_count == 3 && trace[0] == LOOKUP && trace[1] == OPEN && trace[2] == ATTACH);
    assert(opened_name == slash && attached_handle == 77 && record.handle == 77);
    assert(record.sector_mode == 0 && record.status == 0);
    check_common_fields(0x1400, 9, 0x7fffffff, 4096, 0x7fffffff);

    /* Open failure stores the zero handle and stops; priority is clamped at zero. */
    reset();
    directory_present = 0;
    open_result = 0;
    assert(submit(7, -3, plain, 0, 0, 0) == 0);
    assert(record.handle == 0 && count_events(ATTACH) == 0 && record.priority == -1);
    reset();
    directory_present = 0;
    assert(submit(0x12345, -3, plain, 100, 7, 9) == 1);
    check_common_fields(0x12345, 0, 100, 7, 9);

    /* Sector-mode names skip the lookup body and require 2048-byte multiples. */
    reset();
    assert(submit(0x1400, 1, sector_name(0x2345), -1, 0, 0) == 0);
    assert(submit(0x1400, 1, sector_name(0x2345), 2047, 0, 0) == 0);
    assert(submit(0x1400, 1, sector_name(0x2345), 2048, 1, 0) == 0);
    assert(submit(0x1400, 1, sector_name(0x2345), 2048, -1, 0) == 0);
    assert(submit(0x1400, 1, sector_name(0x2345), 2048, 0, 100) == 0);
    assert(count_events(COPY) == 0 && count_events(OPEN) == 0 && count_events(LOOKUP) == 5);
    assert(submit(0x1400, 1, sector_name(0x2345), 0, 0, 0) == 1);
    check_sector_mode(0x2345);
    check_common_fields(0x1400, 1, 0, 0, 0);
    assert(submit(0x1400, 1, sector_name(0x7fffffff), 4096, -2048, -1) == 1);
    check_sector_mode(0x7fffffff);
    check_common_fields(0x1400, 1, 4096, 0x7ffff800, 0x7fffffff);

    /* Directory match through '/' and, failing that, through a backslash. */
    reset();
    assert(submit(0x1400, 1, slash, -1, 0, -1) == 1);
    check_sector_mode(0x1007);
    check_common_fields(0x1400, 1, (3 + 7) << 11, 0, 0x7fffffff);
    assert(count_events(COMPARE) == 7 && count_events(GROUP) == 0);
    reset();
    assert(submit(0x1400, 1, backslash, 2048, 0, 0) == 1);
    check_sector_mode(0x1060);
    check_common_fields(0x1400, 1, 2048, 0, 0); /* A nonnegative size is preserved. */
    assert(count_events(COMPARE) == 96); /* Entry 5 has no backslash and is skipped. */

    /* No table match, or no separator at all, falls back to opening the name. */
    reset();
    assert(submit(0x1400, 1, missing, -1, 0, 0) == 1);
    assert(count_events(COMPARE) == 96 && opened_name == missing && record.sector_mode == 0);
    reset();
    assert(submit(0x1400, 1, plain, -1, 0, 0) == 1);
    assert(count_events(FIND) == 2 && count_events(LENGTH) == 0 && opened_name == plain);

    /* Type 0x1400 "Mnnn.GRP" names map to the shared entry plus a group offset. */
    reset();
    group_result = 3;
    assert(submit(0x1400, 1, group, -1, 2048, 0) == 1);
    assert(group_argument == 123 && count_events(GROUP) == 1);
    check_sector_mode(0x1028);
    check_common_fields(0x1400, 1, (3 + 40) << 11, 2048 + ((3 * 257) << 11), 0);
    reset();
    group_result = 1;
    assert(submit(0x1400, 1, group_upper, -1, 0, 0) == 1);
    assert(group_argument == 9 && record.byte_offset == (257u << 11));
    reset();
    assert(submit(0x1401, 1, group, -1, 0, 0) == 1);
    assert(count_events(GROUP) == 0 && opened_name == group);
    reset();
    assert(submit(0x1400, 1, not_group, -1, 0, 0) == 1);
    assert(count_events(GROUP) == 0 && opened_name == not_group);
    reset();
    assert(submit(0x1400, 1, short_group, -1, 0, 0) == 1);
    assert(count_events(GROUP) == 0 && opened_name == short_group);
    return 0;
}

#else
/* On the target these three symbols and the submit view share one address
 * range; on the host they are separate objects kept consistent by the test.
 */
struct DotrResourceSubmitState dotr_ee_resource_submit_state_0031c2f0;
struct DotrResourceCachePrefix gSzFile_fl_prefix;
struct DotrResourceFlags data_00322980[1];
struct DotrResourceQueueRecord ReadRequestQueueArray[256];

enum { SUBMIT = 1, SEARCH, FETCH, CONSUME };

static struct DotrResourceSubmitState *const state = &dotr_ee_resource_submit_state_0031c2f0;
static int trace[64], trace_count;
static int submit_result, pending, fetch_results[8], fetch_calls, consumed[8], consume_calls;
static struct DotrResourceQueueRecord *seen_record;
static char *seen_name;
static char name_text[] = "synthetic-name";
static int seen[8];
static int final_bit_0;

static void record_event(int event)
{
    assert(trace_count < (int)(sizeof(trace) / sizeof(trace[0])));
    trace[trace_count++] = event;
}

void lSzFile_ReadCB(struct DotrResourceSubmitRecord *record, int completed)
{
    (void)record;
    (void)completed;
    assert(0);
}

int lSzFile_SetReadRequestParam(struct DotrResourceSubmitRecord *record, int type, int priority, char *name,
                  int unknown_014, int byte_size, int byte_offset, int unknown_010,
                  DotrResourceSubmitCallback callback, int unknown_01c)
{
    record_event(SUBMIT);
    seen_record = (struct DotrResourceQueueRecord *)record;
    seen_name = name;
    seen[0] = type; seen[1] = priority; seen[3] = unknown_014;
    seen[4] = byte_size; seen[5] = byte_offset; seen[6] = unknown_010; seen[7] = unknown_01c;
    assert(callback == lSzFile_ReadCB);
    return submit_result;
}

/* Ends the original unbounded wait after a controlled number of polls. */
struct DotrResourceQueueRecord *SzFile_InqRequestX(int selector, int grouped)
{
    assert(selector == -1 && grouped == 0);
    record_event(SEARCH);
    if (pending == 0) {
        /* The result flag is read only after the queue search reports empty. */
        state->flags.bit_0 = final_bit_0 ? -1 : 0;
        return 0;
    }
    pending--;
    return &state->records[7];
}

int lSzFile_NextRequest(void)
{
    record_event(FETCH);
    return fetch_results[fetch_calls++];
}

int lSzFile_DoRequestMain(int cached_result)
{
    record_event(CONSUME);
    consumed[consume_calls++] = cached_result;
    return 1;
}

static void reset(unsigned short head, unsigned short count)
{
    memset(state, 0x5a, sizeof(*state));
    memset(&gSzFile_fl_prefix, 0x5a, sizeof(gSzFile_fl_prefix));
    state->head = head;
    state->count = count;
    state->flags.bit_0 = 0;
    state->flags.bit_1 = 0;
    data_00322980[0].bit_1 = 0;
    gSzFile_fl_prefix.cached_result = 91;
    trace_count = fetch_calls = consume_calls = pending = 0;
    submit_result = 1;
    final_bit_0 = 1;
    seen_record = 0;
    seen_name = 0;
    memset(seen, 0, sizeof(seen));
}

static void check_untouched_records(void)
{
    size_t i;
    const unsigned char *bytes = (const unsigned char *)state->records;
    for (i = 0; i < sizeof(state->records); i++) {
        assert(bytes[i] == 0x5a);
    }
}

static void check_direct_submit(void)
{
    static const struct { unsigned short head, count; int index; } slots[] = {
        { 0, 0, 0 }, { 0, 255, 255 }, { 250, 10, 4 }, { 255, 0, 255 }, { 255, 1, 0 }, { 128, 128, 0 }
    };
    size_t i;

    /* A zero name and a full queue both return null before the dependency. */
    reset(3, 4);
    assert(SzFile_ReadRequestParam(1, 2, 0, 4, 5, 6, 7, lSzFile_ReadCB, 9) == 0);
    reset(3, 256);
    assert(SzFile_ReadRequestParam(1, 2, name_text, 4, 5, 6, 7, lSzFile_ReadCB, 9) == 0);
    assert(trace_count == 0 && state->count == 256 && state->head == 3);

    for (i = 0; i < sizeof(slots) / sizeof(slots[0]); i++) {
        reset(slots[i].head, slots[i].count);
        assert(SzFile_ReadRequestParam(INT_MIN, -2, name_text, INT_MAX, 5, -6, 7, lSzFile_ReadCB, 9)
               == &state->records[slots[i].index]);
        assert(trace_count == 1 && seen_record == &state->records[slots[i].index]);
        assert(seen[0] == INT_MIN && seen[1] == -2 && seen_name == name_text && seen[3] == INT_MAX);
        assert(seen[4] == 5 && seen[5] == -6 && seen[6] == 7 && seen[7] == 9);
        assert(state->count == slots[i].count + 1 && state->head == slots[i].head);
        check_untouched_records();

        /* A dependency failure leaves the count unchanged. */
        reset(slots[i].head, slots[i].count);
        submit_result = 0;
        assert(SzFile_ReadRequestParam(1, 2, name_text, 4, 5, 6, 7, lSzFile_ReadCB, 9) == 0);
        assert(trace_count == 1 && state->count == slots[i].count);
    }
}

/* mode 0: 0x1ffd30 through the linked 0x1fecb0; mode 1: 0x1ffe00 inline path. */
static struct DotrResourceQueueRecord *submit_and_wait(int mode)
{
    return mode ? SzFile_ReadParam(21, name_text, 23, 24, 25, 26) : SzFile_Read(name_text, 23, 24);
}

static void check_submit_arguments(int mode, int index)
{
    assert(trace[0] == SUBMIT && seen_record == &state->records[index]);
    assert(seen[0] == 0x1400 && seen_name == name_text && seen[3] == 23 && seen[4] == 24 && seen[7] == 0);
    if (mode) {
        assert(seen[1] == 21 && seen[5] == 25 && seen[6] == 26);
    } else {
        assert(seen[1] == 0 && seen[5] == 0 && seen[6] == -1);
    }
}

static void check_wait(int mode)
{
    static const int cached_branch[] = {
        SUBMIT, SEARCH, CONSUME, SEARCH, CONSUME, SEARCH
    };
    static const int fetch_branch[] = {
        SUBMIT, SEARCH, FETCH, SEARCH, FETCH, CONSUME, SEARCH, FETCH, CONSUME, SEARCH
    };

    /* Submission failures return null without any search or poll. */
    reset(250, 10);
    submit_result = 0;
    assert(submit_and_wait(mode) == 0);
    assert(trace_count == 1 && state->count == 10);
    check_submit_arguments(mode, 4);
    reset(0, 256);
    assert(submit_and_wait(mode) == 0 && trace_count == 0);
    if (mode) {
        reset(0, 0);
        assert(SzFile_ReadParam(21, 0, 23, 24, 25, 26) == 0 && trace_count == 0);
    }

    /* An already empty queue: one search, result selected by flag bit 0. */
    reset(255, 1);
    assert(submit_and_wait(mode) == &state->records[0]);
    assert(trace_count == 2 && trace[1] == SEARCH && state->count == 2);
    check_submit_arguments(mode, 0);
    reset(255, 1);
    final_bit_0 = 0;
    assert(submit_and_wait(mode) == 0);
    assert(trace_count == 2 && state->count == 2);

    /* Flag bit 1 set: each poll consumes the cached result without fetching. */
    reset(0, 0);
    data_00322980[0].bit_1 = -1;
    pending = 2;
    assert(submit_and_wait(mode) == &state->records[0]);
    assert(trace_count == 6 && memcmp(trace, cached_branch, sizeof(cached_branch)) == 0);
    assert(fetch_calls == 0 && consume_calls == 2 && consumed[0] == 91 && consumed[1] == 91);

    /* Flag bit 1 clear: negative fetches skip the consumer; others are cached. */
    reset(0, 255);
    pending = 3;
    fetch_results[0] = -1;
    fetch_results[1] = 0;
    fetch_results[2] = 200;
    assert(submit_and_wait(mode) == &state->records[255]);
    assert(trace_count == 10 && memcmp(trace, fetch_branch, sizeof(fetch_branch)) == 0);
    assert(consume_calls == 2 && consumed[0] == 0 && consumed[1] == 200);
    assert(gSzFile_fl_prefix.cached_result == 200 && state->count == 256);
    check_untouched_records();
}

int main(void)
{
    assert(offsetof(struct DotrResourceQueueRecord, priority) == 6);
    assert(offsetof(struct DotrResourceSubmitState, records) == 0x4080);
    assert(offsetof(struct DotrResourceSubmitState, cached_result) == 0x6688);
    assert(offsetof(struct DotrResourceSubmitState, head) == 0x668c);
    assert(offsetof(struct DotrResourceSubmitState, count) == 0x668e);
    assert(offsetof(struct DotrResourceSubmitState, flags) == 0x6690);
    assert(offsetof(struct DotrResourceSubmitState, cached_result)
           == offsetof(struct DotrResourceCachePrefix, cached_result));
    assert(sizeof(struct DotrResourceSubmitFlags) == 1);

    check_direct_submit();
    check_wait(0);
    check_wait(1);
    return 0;
}
#endif

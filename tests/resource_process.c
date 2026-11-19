/* Synthetic host checks for the queue load step and its release routine. */
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/resource_process.h"

struct DotrResourceSubmitState dotr_ee_resource_submit_state_0031c2f0;
static struct DotrResourceSubmitState *const state = &dotr_ee_resource_submit_state_0031c2f0;

/* Even indices keep the host's wider record view pointer-aligned. On the host
 * that view is 40 bytes, so its last field overlaps the following queue record;
 * each case therefore uses one record and leaves its successor unused.
 */
enum { INDEX = 6 };

struct Event {
    int kind, a, b, c;
};
static struct Event trace[64];
static int trace_count;

static void record_event(int kind, int a, int b, int c)
{
    assert(trace_count < 64);
    trace[trace_count].kind = kind;
    trace[trace_count].a = a;
    trace[trace_count].b = b;
    trace[trace_count].c = c;
    trace_count++;
}

static void expect(int position, int kind, int a, int b, int c)
{
    assert(position < trace_count);
    assert(trace[position].kind == kind && trace[position].a == a);
    assert(trace[position].b == b && trace[position].c == c);
}

static struct DotrResourceSubmitRecord *view(int index)
{
    return (struct DotrResourceSubmitRecord *)&state->records[index];
}

#if defined(DOTR_TEST_COMPLETION)
/* 0x200170 without its `sync`/`ei` block, which the host build omits: this
 * validates only the surrounding C behavior, not synchronization or interrupt
 * effects. Dependency semantics are assumptions.
 */
enum { QUERY = 1, FLUSH, RELEASE };
static int query_result;

int func_00191ef8(void)
{
    record_event(QUERY, 0, 0, 0);
    return query_result;
}

void func_0017bb90(int unknown)
{
    record_event(FLUSH, unknown, 0, 0);
}

void lSzFile_TermRequest(int index, int completed)
{
    /* The busy flag is still set when the release routine is entered. */
    record_event(RELEASE, index, completed, state->flags.bit_1);
}

static void reset(int busy, int result)
{
    memset(state, 0, sizeof(*state));
    state->cached_result = 42;
    state->flags.bit_0 = -1;
    state->flags.bit_1 = busy ? -1 : 0;
    query_result = result;
    trace_count = 0;
}

int main(void)
{
    int reason;
    (void)view;

    /* Any argument other than 1 does nothing, whatever the flag. */
    for (reason = -1; reason <= 3; reason++) {
        if (reason != 1) {
            reset(1, 0);
            lRhFile_CdCallback(reason);
            assert(trace_count == 0 && state->flags.bit_1 == -1);
        }
    }

    /* No request in flight: no query. */
    reset(0, 0);
    lRhFile_CdCallback(1);
    assert(trace_count == 0 && state->flags.bit_1 == 0);

    /* Zero query result: flush, then release the cached selection as completed.
     * This routine itself leaves the flag for the release routine to clear.
     */
    reset(1, 0);
    lRhFile_CdCallback(1);
    assert(trace_count == 3);
    expect(0, QUERY, 0, 0, 0);
    expect(1, FLUSH, 0, 0, 0);
    expect(2, RELEASE, 42, 1, -1);
    assert(state->flags.bit_1 == -1 && state->cached_result == 42);

    /* Nonzero query result: only the busy flag is cleared, so the load step retries. */
    reset(1, 7);
    lRhFile_CdCallback(1);
    assert(trace_count == 1 && state->flags.bit_1 == 0 && state->flags.bit_0 == -1);

    puts("resource_completion: ok (C behavior only; sync/ei omitted)");
    return 0;
}
#elif defined(DOTR_TEST_PAIR_THUNK)
/* 0x1ffcb0 forwards its selector with a zero second argument and returns the result. */
static struct DotrResourcePair pairs[2];
static struct DotrResourcePair *lookup_result;

struct DotrResourcePair *SzFile_InqGroupX(int selector, int grouped)
{
    record_event(1, selector, grouped, 0);
    return lookup_result;
}

int main(void)
{
    (void)view;
    lookup_result = &pairs[1];
    assert(SzFile_InqGroup(0x1400) == &pairs[1]);
    lookup_result = 0;
    assert(SzFile_InqGroup(-1) == 0);
    assert(trace_count == 2);
    expect(0, 1, 0x1400, 0, 0);
    expect(1, 1, -1, 0, 0);
    puts("resource_pair_thunk: ok");
    return 0;
}
#elif defined(DOTR_TEST_RELEASE)
/* 0x1ffac0 with the real queue and pair searches linked in. On the target
 * ReadRequestQueueArray is the same storage as the state's records; here it is a
 * separate object standing for the other queued requests.
 */
struct DotrResourceQueueRecord ReadRequestQueueArray[256];
struct DotrResourcePair FileGroupEntryArray[64];

enum { CALLBACK = 1, CLOSE, RELEASE, PAIR_LOOKUP, PAIR_CALL };

static void callback(struct DotrResourceSubmitRecord *record, int completed)
{
    record_event(CALLBACK, record == view(INDEX), completed, record->status);
}

int func_0017fd20(int descriptor)
{
    record_event(CLOSE, descriptor, 0, 0);
    return 0;
}

void SzMem_Free(int address)
{
    record_event(RELEASE, address, 0, 0);
}

/* Stub for the thunk SzFile_InqGroup (0x1ffcb0; a jump to SzFile_InqGroupX with a zero second argument). */
struct DotrResourcePair *SzFile_InqGroup(int selector)
{
    record_event(PAIR_LOOKUP, selector, 0, 0);
    return SzFile_InqGroupX(selector, 0);
}

static void pair_function(int selector)
{
    record_event(PAIR_CALL, selector, FileGroupEntryArray[9].selector, 0);
}

static struct DotrResourceSubmitRecord *reset(unsigned short head, unsigned short count)
{
    struct DotrResourceSubmitRecord *record = view(INDEX);
    int i;
    memset(state, 0, sizeof(*state));
    for (i = 0; i < 256; i++) {
        state->records[i].status = -1;
        ReadRequestQueueArray[i].status = -1;
    }
    for (i = 0; i < 64; i++) {
        FileGroupEntryArray[i].selector = -1;
        FileGroupEntryArray[i].value = 0;
    }
    state->head = head;
    state->count = count;
    state->flags.bit_0 = -1;
    state->flags.bit_1 = -1;
    record->selector = 0x1400;
    record->handle = 0;
    trace_count = 0;
    return record;
}

int main(void)
{
    struct DotrResourceSubmitRecord *record;
    intptr_t pair_address = (intptr_t)pair_function;

    /* The pair table stores its function in a 32-bit word. */
    if (pair_address != (int)pair_address) {
        puts("skipped: host code addresses do not fit a 32-bit word");
        return 77;
    }

    /* Minimal release: nothing to call, the lookup misses, bit 1 is cleared. */
    record = reset(0, 0);
    lSzFile_TermRequest(INDEX, 1);
    assert(trace_count == 1);
    expect(0, PAIR_LOOKUP, 0x1400, 0, 0);
    assert(record->status == -1 && state->head == 0 && state->count == 0);
    assert(state->flags.bit_1 == 0 && state->flags.bit_0 == -1);

    /* Order for an open file request that failed: callback (with the flag and
     * the still-open descriptor), close, name release, buffer release, lookup.
     */
    record = reset(INDEX, 3);
    record->callback = callback;
    record->status = 5;
    record->handle = 0x7001;
    record->unknown_014 = 0x7002;
    record->flag_00c = -1;
    lSzFile_TermRequest(INDEX, 0);
    assert(trace_count == 5);
    expect(0, CALLBACK, 1, 0, 5);
    expect(1, CLOSE, 5, 0, 0);
    expect(2, RELEASE, 0x7001, 0, 0);
    expect(3, RELEASE, 0x7002, 0, 0);
    expect(4, PAIR_LOOKUP, 0x1400, 0, 0);
    assert(record->status == -1 && record->handle == 0 && record->unknown_014 == 0x7002);
    /* The released record was the head: one fewer, head advanced. */
    assert(state->head == INDEX + 1 && state->count == 2 && state->flags.bit_1 == 0);

    /* A completed request keeps its buffer; sector mode has no descriptor to close. */
    record = reset(INDEX, 1);
    record->callback = callback;
    record->status = 900;
    record->sector_mode = -1;
    record->unknown_014 = 0x7002;
    record->flag_00c = -1;
    lSzFile_TermRequest(INDEX, 1);
    assert(trace_count == 2);
    expect(0, CALLBACK, 1, 1, 900);
    expect(1, PAIR_LOOKUP, 0x1400, 0, 0);
    assert(record->status == -1);
    /* Count reached zero, so the head is not advanced. */
    assert(state->head == INDEX && state->count == 0);

    /* A failed request without the owned-buffer flag, or without a buffer, frees nothing. */
    record = reset(0, 0);
    record->unknown_014 = 0x7002;
    lSzFile_TermRequest(INDEX, 0);
    assert(trace_count == 1);
    record = reset(0, 0);
    record->flag_00c = -1;
    lSzFile_TermRequest(INDEX, 0);
    assert(trace_count == 1);

    /* Another live request with the same selector suppresses the pair lookup. */
    record = reset(0, 0);
    ReadRequestQueueArray[200].status = 0;
    ReadRequestQueueArray[200].selector = 0x1400;
    FileGroupEntryArray[9].selector = 0x1400;
    FileGroupEntryArray[9].value = (int)pair_address;
    lSzFile_TermRequest(INDEX, 1);
    assert(trace_count == 0 && FileGroupEntryArray[9].selector == 0x1400);

    /* Last request of a selector: the pair's function sees the selector while
     * the pair is still registered, then the pair is vacated.
     */
    ReadRequestQueueArray[200].selector = 0x1401;
    lSzFile_TermRequest(INDEX, 1);
    assert(trace_count == 2);
    expect(0, PAIR_LOOKUP, 0x1400, 0, 0);
    expect(1, PAIR_CALL, 0x1400, 0x1400, 0);
    assert(FileGroupEntryArray[9].selector == -1 && FileGroupEntryArray[9].value == (int)pair_address);

    /* A pair without a function is vacated without a call. */
    record = reset(0, 0);
    FileGroupEntryArray[63].selector = 0x1400;
    lSzFile_TermRequest(INDEX, 1);
    assert(trace_count == 1 && FileGroupEntryArray[63].selector == -1);

    /* Head bookkeeping: only the head record shrinks the queue; the head wraps at 256. */
    record = reset(INDEX + 2, 4);
    lSzFile_TermRequest(INDEX, 1);
    assert(state->head == INDEX + 2 && state->count == 4);
    reset(0, 0);
    state->head = 254;
    state->count = 2;
    view(254)->selector = 7;
    lSzFile_TermRequest(254, 1);
    assert(state->head == 255 && state->count == 1);
    state->head = 255;
    state->count = 9;
    state->records[255].selector = 7;
    lSzFile_TermRequest(255, 1);
    assert(state->head == 0 && state->count == 8);

    puts("resource_release: ok");
    return 0;
}
#else
/* 0x1ff480 with every dependency stubbed; their semantics are assumptions. */
struct DotrResourceReadMode data_00293cb0[1] = {{{0, 1, 0, 0}}};
char data_002d0648[] = "<format>";
char data_002d064d[] = "<prefix>";

enum { READY = 1, ALLOCATE, FLUSH, SECTOR_READ, FORMAT, OPEN, SEEK, CLOSE, CONTROL, READ, RELEASE };

static int ready_result, allocate_result, sector_read_result;
static int open_results[2], open_calls, seek_results[2], seek_calls;
static int control_results[4], control_values[4], control_calls, read_result;
static const char *formatted_path;

int func_00191a58(int unknown)
{
    record_event(READY, unknown, 0, 0);
    return ready_result;
}

int SzMem_Alloc(int byte_count)
{
    record_event(ALLOCATE, byte_count, 0, 0);
    return allocate_result;
}

void func_0017bb60(int unknown)
{
    record_event(FLUSH, unknown, 0, 0);
}

int func_00191d18(int sector, int sector_count, int buffer, struct DotrResourceReadMode *mode)
{
    /* The mode is a local copy of the four initialized bytes. */
    assert(mode != data_00293cb0 && memcmp(mode, data_00293cb0, 4) == 0);
    record_event(SECTOR_READ, sector, sector_count, buffer);
    return sector_read_result;
}

int func_0019b648(char *destination, const char *format, ...)
{
    va_list arguments;
    const char *prefix;
    int handle;
    va_start(arguments, format);
    prefix = va_arg(arguments, const char *);
    handle = va_arg(arguments, int);
    va_end(arguments);
    assert(format == data_002d0648 && prefix == data_002d064d);
    formatted_path = destination;
    strcpy(destination, "<path>");
    record_event(FORMAT, handle, 0, 0);
    return 6;
}

int func_0017fa98(const char *path, int flags)
{
    assert(path == formatted_path && strcmp(path, "<path>") == 0);
    record_event(OPEN, flags, 0, 0);
    return open_results[open_calls++];
}

int func_0017fea0(int descriptor, int offset, int origin)
{
    record_event(SEEK, descriptor, offset, origin);
    return seek_results[seek_calls++];
}

int func_0017fd20(int descriptor)
{
    record_event(CLOSE, descriptor, 0, 0);
    return 0;
}

int func_00180608(int descriptor, int request, int *result)
{
    record_event(CONTROL, descriptor, request, 0);
    *result = control_values[control_calls];
    return control_results[control_calls++];
}

int func_001800d8(int descriptor, int buffer, int byte_count)
{
    record_event(READ, descriptor, buffer, byte_count);
    return read_result;
}

void lSzFile_TermRequest(int index, int completed)
{
    record_event(RELEASE, index, completed, 0);
}

static struct DotrResourceSubmitRecord *reset(void)
{
    struct DotrResourceSubmitRecord *record = view(INDEX);
    memset(state, 0, sizeof(*state));
    memset(control_results, 0, sizeof(control_results));
    memset(control_values, 0, sizeof(control_values));
    state->flags.bit_0 = -1;
    record->handle = 0x7001;
    ready_result = 2;
    allocate_result = 0x6000;
    sector_read_result = 1;
    open_results[0] = 3;
    open_results[1] = 4;
    seek_results[0] = 0;
    seek_results[1] = 0;
    read_result = 0;
    trace_count = open_calls = seek_calls = control_calls = 0;
    return record;
}

static void sector_cases(void)
{
    struct DotrResourceSubmitRecord *record;

    /* A read already in flight (bit 1) defers the sector request untouched. */
    record = reset();
    record->sector_mode = -1;
    state->flags.bit_1 = -1;
    assert(lSzFile_DoRequestMain(INDEX) == 0 && trace_count == 0 && record->flag_008 == 0);

    /* The started flag is set before the readiness poll; not ready returns 0. */
    record = reset();
    record->sector_mode = -1;
    ready_result = 6;
    assert(lSzFile_DoRequestMain(INDEX) == 0 && trace_count == 1);
    expect(0, READY, 1, 0, 0);
    assert(record->flag_008 == -1 && state->flags.bit_1 == 0);

    /* Allocation failure releases the request as failed. */
    record = reset();
    record->sector_mode = -1;
    record->byte_size = 0x1800;
    allocate_result = 0;
    assert(lSzFile_DoRequestMain(INDEX) == 0 && trace_count == 3);
    expect(1, ALLOCATE, 0x1800, 0, 0);
    expect(2, RELEASE, INDEX, 0, 0);
    assert(record->unknown_014 == 0 && record->flag_00c == 0);

    /* Owned buffer, defaulted length, sector arithmetic and call order. */
    record = reset();
    record->sector_mode = -1;
    record->status = 1000;
    record->byte_size = 0x3000;
    record->byte_offset = 0x1000;
    record->unknown_010 = -1;
    assert(lSzFile_DoRequestMain(INDEX) == 0 && trace_count == 4);
    expect(0, READY, 1, 0, 0);
    expect(1, ALLOCATE, 0x3000, 0, 0);
    expect(2, FLUSH, 0, 0, 0);
    expect(3, SECTOR_READ, 1002, 4, 0x6000);
    assert(record->unknown_014 == 0x6000 && record->flag_00c == -1);
    assert(record->unknown_010 == 0x2000 && record->status == 1000);
    assert(state->flags.bit_1 == -1 && state->flags.bit_0 == -1);

    /* A supplied buffer and explicit length are used as given. */
    record = reset();
    record->sector_mode = -1;
    record->status = 20;
    record->byte_size = 0x3000;
    record->unknown_010 = 0x800;
    record->unknown_014 = 0x5000;
    assert(lSzFile_DoRequestMain(INDEX) == 0 && trace_count == 3);
    expect(2, SECTOR_READ, 20, 1, 0x5000);
    assert(record->flag_00c == 0 && record->unknown_010 == 0x800);

    /* A refused read leaves bit 1 clear so the next poll retries. */
    record = reset();
    record->sector_mode = -1;
    sector_read_result = 0;
    assert(lSzFile_DoRequestMain(INDEX) == 0 && trace_count == 3 && state->flags.bit_1 == 0);
}

static void file_cases(void)
{
    struct DotrResourceSubmitRecord *record;
    int failure;

    /* Unknown size: probe open, seek to end, close, reopen, round to 16,
     * allocate, seek to the offset, poll until idle, flush and start the read.
     */
    record = reset();
    record->byte_size = -1;
    record->byte_offset = 0x30;
    record->unknown_010 = -1;
    seek_results[0] = 0x101;
    control_values[0] = 1; /* seek still busy */
    control_values[2] = 1; /* read still busy */
    assert(lSzFile_DoRequestMain(INDEX) == 0 && trace_count == 12);
    expect(0, FORMAT, 0x7001, 0, 0);
    expect(1, OPEN, 1, 0, 0);
    expect(2, SEEK, 3, 0, 2);
    expect(3, CLOSE, 3, 0, 0);
    expect(4, OPEN, 0x8001, 0, 0);
    expect(5, ALLOCATE, 0x110, 0, 0);
    expect(6, SEEK, 4, 0x30, 0);
    expect(7, CONTROL, 4, 1, 0);
    expect(8, CONTROL, 4, 1, 0);
    expect(9, FLUSH, 0, 0, 0);
    expect(10, READ, 4, 0x6000, 0xe0);
    expect(11, CONTROL, 4, 1, 0);
    assert(record->status == 4 && record->byte_size == 0x110 && record->unknown_010 == 0xe0);
    assert(record->flag_008 == -1 && record->flag_00c == -1 && state->flags.bit_1 == -1);

    /* Later polls only query the started read; idle completes the request. */
    trace_count = control_calls = 0;
    control_values[0] = 1;
    assert(lSzFile_DoRequestMain(INDEX) == 0 && trace_count == 1);
    assert(lSzFile_DoRequestMain(INDEX) == 1 && trace_count == 4);
    expect(1, CONTROL, 4, 1, 0);
    expect(2, FLUSH, 0, 0, 0);
    expect(3, RELEASE, INDEX, 1, 0);

    /* Known size, supplied buffer, no offset: one open, no seek or allocation. */
    record = reset();
    record->byte_size = 0x21;
    record->unknown_010 = 0x11;
    record->unknown_014 = 0x5000;
    open_results[0] = 9;
    assert(lSzFile_DoRequestMain(INDEX) == 1 && trace_count == 7);
    expect(1, OPEN, 0x8001, 0, 0);
    expect(2, FLUSH, 0, 0, 0);
    expect(3, READ, 9, 0x5000, 0x20);
    expect(4, CONTROL, 9, 1, 0);
    expect(6, RELEASE, INDEX, 1, 0);
    assert(record->byte_size == 0x30 && record->flag_00c == 0);

    /* Zero size skips rounding and allocation even without a buffer. */
    record = reset();
    record->unknown_010 = 0;
    assert(lSzFile_DoRequestMain(INDEX) == 1 && trace_count == 7);
    expect(3, READ, 3, 0, 0);

    /* Every failure path releases the request as failed and returns 0. */
    for (failure = 0; failure < 9; failure++) {
        static const int events[9] = {3, 4, 6, 7, 8, 9, 11, 2, 3};
        record = reset();
        record->byte_size = failure < 7 ? -1 : 0x20;
        record->byte_offset = failure < 7 ? 0x30 : 0;
        record->unknown_010 = 0x10;
        if (failure >= 7) {
            record->flag_008 = failure == 7 ? -1 : 0;
            record->unknown_014 = 0x5000;
        }
        seek_results[0] = 0x100;
        switch (failure) {
        case 0: open_results[0] = -1; break;     /* probe open */
        case 1: seek_results[0] = -1; break;     /* size query */
        case 2: open_results[1] = -1; break;     /* reopen */
        case 3: allocate_result = 0; break;      /* buffer */
        case 4: seek_results[1] = -1; break;     /* seek to offset */
        case 5: control_results[0] = -1; break;  /* poll after seek */
        case 6: read_result = -1; break;         /* read start */
        case 7: control_results[0] = -1; break;  /* poll of a started read */
        default: open_results[0] = -1; break;    /* single open */
        }
        assert(lSzFile_DoRequestMain(INDEX) == 0);
        assert(trace_count == events[failure]);
        expect(trace_count - 1, RELEASE, INDEX, 0, 0);
        assert(state->flags.bit_1 == -1);
    }
}

int main(void)
{
    /* A negative index (no selected record) does nothing. */
    reset();
    assert(lSzFile_DoRequestMain(-1) == 0 && trace_count == 0 && state->flags.bit_1 == 0);
    sector_cases();
    file_cases();
    puts("resource_process: ok");
    return 0;
}
#endif

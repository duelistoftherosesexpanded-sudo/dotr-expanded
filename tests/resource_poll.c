/* Synthetic host checks for two callers of the submit/poll chain (0x2003e0, 0x2006c0). */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/resource_process.h"

struct DotrResourceSubmitState dotr_ee_resource_submit_state_0031c2f0;
static struct DotrResourceSubmitState *const state = &dotr_ee_resource_submit_state_0031c2f0;
char data_002d0648[] = "<path format>";
char data_002d064d[] = "<prefix>";
char data_002d0655[] = "<fallback>";
char data_002d0680[] = "<label>";
char data_002d0686[] = "<directory format>";

enum { SUBMIT = 1, SEARCH, POLL, LOOKUP, FORMAT, ATTACH, OPEN, CLOSE };
static int trace[32], trace_count;
static struct DotrResourceQueueRecord queued;
static struct DotrResourceQueueRecord *submit_result;
static int pending, final_bit_0, open_result, closed;
static char *lookup_result, *attached;
static const char *format_first[2], *format_second[2];
static char *format_destination[2];
static int format_calls;
static struct {
    int type, priority, unknown_014, byte_size, byte_offset, unknown_010, unknown_01c;
    char *name;
    DotrResourceSubmitCallback callback;
} submitted;

static void record_event(int kind)
{
    assert(trace_count < 32);
    trace[trace_count++] = kind;
}

static void expect_trace(const int *expected, int count)
{
    assert(trace_count == count);
    assert(memcmp(trace, expected, (size_t)count * sizeof(int)) == 0);
}

void lSzFile_ReadCB(struct DotrResourceSubmitRecord *record, int completed)
{
    (void)record;
    (void)completed;
    assert(0);
}

struct DotrResourceQueueRecord *SzFile_ReadRequestParam(int type, int priority, char *name, int unknown_014,
                                              int byte_size, int byte_offset, int unknown_010,
                                              DotrResourceSubmitCallback callback, int unknown_01c)
{
    record_event(SUBMIT);
    submitted.type = type;
    submitted.priority = priority;
    submitted.name = name;
    submitted.unknown_014 = unknown_014;
    submitted.byte_size = byte_size;
    submitted.byte_offset = byte_offset;
    submitted.unknown_010 = unknown_010;
    submitted.callback = callback;
    submitted.unknown_01c = unknown_01c;
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
    return &queued;
}

int SzFile_DoRequestS(void)
{
    record_event(POLL);
    return 0;
}

char *func_00197548(const char *label)
{
    assert(label == data_002d0680);
    record_event(LOOKUP);
    return lookup_result;
}

int func_0019b648(char *destination, const char *format, ...)
{
    va_list arguments;
    assert(format_calls < 2);
    assert(format == (format_calls ? data_002d0648 : data_002d0686));
    va_start(arguments, format);
    format_first[format_calls] = va_arg(arguments, const char *);
    format_second[format_calls] = va_arg(arguments, const char *);
    va_end(arguments);
    format_destination[format_calls] = destination;
    strcpy(destination, format_calls ? "<path>" : "<directory>");
    format_calls++;
    record_event(FORMAT);
    return 0;
}

void lSzFile_StrNormalize(char *text)
{
    assert(strcmp(text, "<directory>") == 0);
    attached = text;
    record_event(ATTACH);
}

int func_0017fa98(const char *path, int flags)
{
    assert(path == format_destination[1] && strcmp(path, "<path>") == 0 && flags == 1);
    record_event(OPEN);
    return open_result;
}

int func_0017fd20(int descriptor)
{
    closed = descriptor;
    record_event(CLOSE);
    return 0;
}

static void reset(void)
{
    memset(state, 0, sizeof(*state));
    memset(&submitted, 0, sizeof(submitted));
    state->unknown_6684 = 0x1234;
    submit_result = &queued;
    pending = 0;
    final_bit_0 = 1;
    open_result = 5;
    closed = -1;
    lookup_result = 0;
    attached = 0;
    trace_count = format_calls = 0;
}

static void submit_wait_cases(void)
{
    static const int failed[] = {SUBMIT};
    static const int waited[] = {SUBMIT, SEARCH, POLL, SEARCH, POLL, SEARCH};
    static const int immediate[] = {SUBMIT, SEARCH};

    /* A negative first argument submits nothing. */
    reset();
    assert(RhFile_ReadAndBlock(-1, 1, 2, 3) == 0 && trace_count == 0);

    /* A refused submission returns 0 without searching or polling. */
    reset();
    submit_result = 0;
    assert(RhFile_ReadAndBlock(0, 1, 2, 3) == 0);
    expect_trace(failed, 1);

    /* Argument mapping: the size is also the length; the name is the stored word. */
    reset();
    pending = 2;
    assert(RhFile_ReadAndBlock(0, 0x800, 0x3000, 0x5000) == 1);
    expect_trace(waited, 6);
    assert(submitted.type == 0x1400 && submitted.priority == 2);
    assert(submitted.name == (char *)(DotrResourceSubmitWord)0x1234);
    assert(submitted.unknown_014 == 0x5000 && submitted.byte_size == 0x3000);
    assert(submitted.byte_offset == 0x800 && submitted.unknown_010 == 0x3000);
    assert(submitted.callback == lSzFile_ReadCB && submitted.unknown_01c == 0);

    /* The result is 1 only if result bit 0 is set once the queue is empty. */
    reset();
    final_bit_0 = 0;
    assert(RhFile_ReadAndBlock(0x7fffffff, 0, 0, 0) == 0);
    expect_trace(immediate, 2);
}

static void probe_cases(void)
{
    static char name[] = "<name>";
    static char found[] = "<found>";
    static const int opened[] = {LOOKUP, FORMAT, SEARCH, POLL, SEARCH, ATTACH, FORMAT, OPEN, CLOSE};
    static const int missing[] = {LOOKUP, FORMAT, SEARCH, ATTACH, FORMAT, OPEN};

    /* A null name does nothing. */
    reset();
    assert(RhFile_ExistFile(0) == 0 && trace_count == 0);

    /* Found directory: format, wait until idle, process the text in place,
     * format the device path, probe-open and close.
     */
    reset();
    lookup_result = found;
    pending = 1;
    assert(RhFile_ExistFile(name) == 1);
    expect_trace(opened, 9);
    assert(format_first[0] == found && format_second[0] == name);
    assert(attached == format_destination[0]);
    assert(format_first[1] == data_002d064d && format_second[1] == format_destination[0]);
    assert(format_destination[0] != format_destination[1] && closed == 5);

    /* Missing directory falls back to the fixed text; a failed open returns 0 unclosed. */
    reset();
    open_result = -1;
    assert(RhFile_ExistFile(name) == 0);
    expect_trace(missing, 6);
    assert(format_first[0] == data_002d0655 && closed == -1);

    /* Descriptor zero is a success and is closed. */
    reset();
    open_result = 0;
    assert(RhFile_ExistFile(name) == 1 && closed == 0);
}

int main(void)
{
    submit_wait_cases();
    probe_cases();
    puts("resource_poll: ok");
    return 0;
}

/* Synthetic host checks for the notifying submit (0x200510), its record
 * callback (0x200680) and the name release (0x2004c0).
 */
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/resource_process.h"

struct DotrResourceSubmitState dotr_ee_resource_submit_state_0031c2f0;
static struct DotrResourceSubmitState *const state = &dotr_ee_resource_submit_state_0031c2f0;
char data_002d0655[] = "<fallback>";
char data_002d0680[] = "<label>";
char data_002d0686[] = "<format>";

static char *lookup_result;
static const char *format_first, *format_second;
static char *format_destination;
static int format_calls, released[4], release_calls, notified[4], notify_calls, setup_result;
static struct {
    struct DotrResourceSubmitRecord *record;
    int type, priority, unknown_014, byte_size, byte_offset, unknown_010, unknown_01c, calls;
    int function_word;
    DotrResourceSubmitCallback callback;
} setup;

char *func_00197548(const char *label)
{
    assert(label == data_002d0680);
    return lookup_result;
}

int func_0019b648(char *destination, const char *format, ...)
{
    va_list arguments;
    assert(format == data_002d0686);
    va_start(arguments, format);
    format_first = va_arg(arguments, const char *);
    format_second = va_arg(arguments, const char *);
    va_end(arguments);
    format_destination = destination;
    strcpy(destination, "<path>");
    format_calls++;
    return 0;
}

int lSzFile_SetReadRequestParam(struct DotrResourceSubmitRecord *record, int type, int priority, char *name,
                  int unknown_014, int byte_size, int byte_offset, int unknown_010,
                  DotrResourceSubmitCallback callback, int unknown_01c)
{
    assert(name == format_destination && strcmp(name, "<path>") == 0);
    setup.record = record;
    setup.type = type;
    setup.priority = priority;
    setup.unknown_014 = unknown_014;
    setup.byte_size = byte_size;
    setup.byte_offset = byte_offset;
    setup.unknown_010 = unknown_010;
    setup.callback = callback;
    setup.unknown_01c = unknown_01c;
    /* The function word is stored before the request is set up. */
    setup.function_word = state->unknown_6680;
    setup.calls++;
    return setup_result;
}

void SzMem_Free(int address)
{
    released[release_calls++] = address;
}

static void notify(int value)
{
    notified[notify_calls++] = value;
}

/* Synthetic nonnull function values; they are stored, never called. */
static void (*word(int value))(int)
{
    return (void (*)(int))(intptr_t)value;
}

static void reset(unsigned short head, unsigned short count)
{
    memset(state, 0, sizeof(*state));
    memset(&setup, 0, sizeof(setup));
    state->head = head;
    state->count = count;
    lookup_result = 0;
    setup_result = 1;
    format_calls = release_calls = notify_calls = 0;
}

int main(void)
{
    static char name[] = "<name>";
    static char found[] = "<found>";
    struct DotrResourceSubmitRecord record;
    intptr_t notify_address = (intptr_t)notify;

    /* 0x2004c0: a negative argument does nothing; otherwise a nonzero name
     * word is released once and cleared, and the result is 1 either way.
     */
    reset(0, 0);
    state->unknown_6684 = 0x4321;
    assert(RhFile_CloseFile(-1) == 0 && release_calls == 0 && state->unknown_6684 == 0x4321);
    assert(RhFile_CloseFile(0) == 1 && release_calls == 1 && released[0] == 0x4321);
    assert(state->unknown_6684 == 0);
    assert(RhFile_CloseFile(5) == 1 && release_calls == 1);

    /* 0x200510: a null name does nothing at all. */
    reset(3, 2);
    state->unknown_6680 = 77;
    assert(RhFile_ReadAndCallbackByFileName(0, 1, 2, 3, word(4), 5, 6) == 0);
    assert(format_calls == 0 && setup.calls == 0 && state->unknown_6680 == 77);

    /* Success: the next slot after the queued records, wrapped at 256; size is
     * also the length; a nonzero flag selects priority 8.
     */
    reset(254, 3);
    lookup_result = found;
    assert(RhFile_ReadAndCallbackByFileName(name, 0x800, 0x3000, 0x5000, word(0x1111), 0x2222, 1) == 1);
    assert(format_first == found && format_second == name && setup.calls == 1);
    assert(setup.record == (struct DotrResourceSubmitRecord *)&state->records[1]);
    assert(setup.type == 0x1400 && setup.priority == 8 && setup.unknown_014 == 0x5000);
    assert(setup.byte_size == 0x3000 && setup.byte_offset == 0x800 && setup.unknown_010 == 0x3000);
    assert(setup.callback == lRhFile_ReadCB && setup.unknown_01c == 0x2222);
    assert(setup.function_word == 0x1111 && state->count == 4 && state->head == 254);

    /* A zero flag selects priority 7; the missing directory uses the fallback. */
    reset(0, 0);
    assert(RhFile_ReadAndCallbackByFileName(name, 0, 0, 0, 0, 0, 0) == 1);
    assert(setup.priority == 7 && format_first == data_002d0655);
    assert(setup.record == (struct DotrResourceSubmitRecord *)&state->records[0] && state->count == 1);

    /* A full queue or a refused setup returns 0 with the count unchanged; the
     * function word has already been replaced.
     */
    reset(9, 256);
    assert(RhFile_ReadAndCallbackByFileName(name, 0, 0, 0, word(0x3333), 0, 0) == 0);
    assert(setup.calls == 0 && state->count == 256 && state->unknown_6680 == 0x3333);
    reset(9, 255);
    setup_result = 0;
    assert(RhFile_ReadAndCallbackByFileName(name, 0, 0, 0, word(0x3333), 0, 0) == 0);
    assert(setup.calls == 1 && state->count == 255);
    assert(setup.record == (struct DotrResourceSubmitRecord *)&state->records[8]);

    /* 0x200680: no stored function, no call. */
    reset(0, 0);
    memset(&record, 0, sizeof(record));
    record.unknown_01c = 0x2222;
    lRhFile_ReadCB(&record, 1);
    assert(notify_calls == 0);

    /* The stored function is a 32-bit word on the target. */
    if (notify_address != (int)notify_address) {
        puts("skipped: host code addresses do not fit a 32-bit word");
        return 77;
    }
    /* It receives the record's word at +0x1c, whatever the completion flag. */
    state->unknown_6680 = (int)notify_address;
    lRhFile_ReadCB(&record, 0);
    lRhFile_ReadCB(&record, 1);
    assert(notify_calls == 2 && notified[0] == 0x2222 && notified[1] == 0x2222);
    assert(state->unknown_6680 == (int)notify_address);

    puts("resource_notify: ok");
    return 0;
}

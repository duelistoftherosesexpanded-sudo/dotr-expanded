/* Synthetic host checks for the queue initializer (0x1feb80) and the
 * plain submit wrapper (0x1fec70).
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/resource_submit.h"

/* On the target the record and pair tables lie inside the cleared state; on
 * the host they are separate objects.
 */
struct DotrResourceSubmitState dotr_ee_resource_submit_state_0031c2f0;
struct DotrResourceQueueRecord ReadRequestQueueArray[256];
struct DotrResourcePair FileGroupEntryArray[64];
static struct DotrResourceSubmitState *const state = &dotr_ee_resource_submit_state_0031c2f0;

static int clear_calls, submit_calls;
static struct DotrResourceQueueRecord queued;
static struct {
    int type, priority, unknown_014, byte_size, byte_offset, unknown_010, unknown_01c;
    char *name;
    DotrResourceSubmitCallback callback;
} submitted;

/* Assumed to fill bytes like memset; the initializer's one call clears the whole state. */
void *RhMem_FillMemory(void *destination, int value, int byte_count)
{
    assert(destination == state && value == 0 && byte_count == 0x66a0);
    assert(byte_count == (int)sizeof(*state));
    clear_calls++;
    return memset(destination, value, (size_t)byte_count);
}

static void callback(struct DotrResourceSubmitRecord *record, int completed)
{
    (void)record;
    (void)completed;
}

struct DotrResourceQueueRecord *SzFile_ReadRequestParam(int type, int priority, char *name, int unknown_014,
                                              int byte_size, int byte_offset, int unknown_010,
                                              DotrResourceSubmitCallback function, int unknown_01c)
{
    submitted.type = type;
    submitted.priority = priority;
    submitted.name = name;
    submitted.unknown_014 = unknown_014;
    submitted.byte_size = byte_size;
    submitted.byte_offset = byte_offset;
    submitted.unknown_010 = unknown_010;
    submitted.callback = function;
    submitted.unknown_01c = unknown_01c;
    submit_calls++;
    return submit_calls == 1 ? &queued : 0;
}

int main(void)
{
    static char name[] = "<name>";
    int i;

    /* 0x1feb80: clear the state, vacate every record and pair, reset the
     * queue, set the cached selection to -1 and clear the busy bit.
     */
    memset(state, 0x5a, sizeof(*state));
    memset(ReadRequestQueueArray, 0x5a, sizeof(ReadRequestQueueArray));
    memset(FileGroupEntryArray, 0x5a, sizeof(FileGroupEntryArray));
    SzFile_Init();
    assert(clear_calls == 1);
    for (i = 0; i < 256; i++) {
        assert(ReadRequestQueueArray[i].status == -1);
        assert(ReadRequestQueueArray[i].selector == 0x5a5a); /* only the first word is written */
    }
    for (i = 0; i < 64; i++) {
        assert(FileGroupEntryArray[i].selector == -1 && FileGroupEntryArray[i].value == 0x5a5a5a5a);
    }
    assert(state->head == 0 && state->count == 0 && state->cached_result == -1);
    assert(state->flags.bit_0 == 0 && state->flags.bit_1 == 0);
    assert(state->unknown_6680 == 0 && state->unknown_6684 == 0 && state->unknown_6691[14] == 0);

    /* 0x1fec70: priority 0, offset 0 and length -1 are fixed; the rest is
     * forwarded and the result returned unchanged.
     */
    assert(SzFile_ReadRequest(0x1234, name, 0x5000, 0x800, callback, 77) == &queued);
    assert(submitted.type == 0x1234 && submitted.priority == 0 && submitted.name == name);
    assert(submitted.unknown_014 == 0x5000 && submitted.byte_size == 0x800);
    assert(submitted.byte_offset == 0 && submitted.unknown_010 == -1);
    assert(submitted.callback == callback && submitted.unknown_01c == 77);
    assert(SzFile_ReadRequest(0, 0, 0, 0, 0, 0) == 0 && submit_calls == 2);

    puts("resource_init: ok");
    return 0;
}

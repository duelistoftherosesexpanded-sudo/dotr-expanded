/* Synthetic host checks for the queue searches and for the calls made to stubbed callees. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/resource_helpers.h"

struct DotrResourceQueueRecord ReadRequestQueueArray[256];
struct DotrResourcePair FileGroupEntryArray[64];
struct DotrResourceCachePrefix gSzFile_fl_prefix;
struct DotrResourceFlags data_00322980[1];
char data_002d0655[] = "synthetic-fallback";
volatile int data_10000800[1];

static char lookup_value[] = "synthetic-directory";
static const char *expected_label;
static int use_lookup, lookup_calls, fetch_calls, consume_calls;
static int fetch_result, expected_cached, consume_result;

char *func_00197548(const char *label)
{
    assert(label == expected_label);
    lookup_calls++;
    return use_lookup ? lookup_value : 0;
}

int lSzFile_NextRequest(void)
{
    fetch_calls++;
    return fetch_result;
}

int lSzFile_DoRequestMain(int cached)
{
    assert(cached == expected_cached);
    consume_calls++;
    return consume_result;
}

static void reset_records(void)
{
    int i;
    memset(ReadRequestQueueArray, 0x5a, sizeof(ReadRequestQueueArray));
    for (i = 0; i < 256; i++) {
        ReadRequestQueueArray[i].status = -1;
    }
    for (i = 0; i < 64; i++) {
        FileGroupEntryArray[i].selector = -1;
        FileGroupEntryArray[i].value = 0x13579;
    }
}

int main(void)
{
    struct DotrResourcePair saved[64];
    struct DotrResourceQueueRecord queue_saved[256];
    char label[] = "synthetic-input";
    int i, mode;
    assert(offsetof(struct DotrResourceQueueRecord, selector) == 4);
    assert(offsetof(struct DotrResourceCachePrefix, cached_result) == 0x6688);
    assert(sizeof(struct DotrResourceFlags) == 1);

    expected_label = label;
    for (use_lookup = 0; use_lookup < 2; use_lookup++) {
        assert(SzFile_GetEnv(label) == (use_lookup ? lookup_value : data_002d0655));
    }
    assert(lookup_calls == 2);

    reset_records();
    memcpy(saved, FileGroupEntryArray, sizeof(saved));
    assert(SzFile_EntryGroup(1029, INT_MIN) == 1);
    saved[0].selector = 1029;
    saved[0].value = INT_MIN;
    assert(memcmp(saved, FileGroupEntryArray, sizeof(saved)) == 0);
    assert(SzFile_InqGroupX(-1, 0) == &FileGroupEntryArray[0]);
    assert(SzFile_InqGroupX(1029, 0) == &FileGroupEntryArray[0]);
    assert(SzFile_InqGroupX(1, 1) == &FileGroupEntryArray[0]);
    assert(SzFile_InqGroupX(5, 0) == 0);
    for (i = 0; i < 63; i++) {
        FileGroupEntryArray[i].selector = i;
    }
    assert(SzFile_EntryGroup(777, INT_MAX) == 1);
    assert(FileGroupEntryArray[63].selector == 777 && FileGroupEntryArray[63].value == INT_MAX);
    memcpy(saved, FileGroupEntryArray, sizeof(saved));
    assert(SzFile_EntryGroup(-7, 42) == 0);
    assert(memcmp(saved, FileGroupEntryArray, sizeof(saved)) == 0);
    assert(SzFile_InqGroupX(777, 0) == &FileGroupEntryArray[63]);
    for (mode = 0; mode < 2; mode++) {
        FileGroupEntryArray[0].selector = -1;
        assert(SzFile_InqGroupX(INT_MIN, mode) == &FileGroupEntryArray[1]);
    }
    reset_records();
    assert(SzFile_InqGroupX(INT_MIN, 1) == 0);
    assert(SzFile_EntryGroup(-7, 42) == 1);
    assert(FileGroupEntryArray[0].selector == -7 && FileGroupEntryArray[0].value == 42);

    reset_records();
    assert(SzFile_InqRequestX(-1, 0) == 0 && SzFile_InqRequestEx() == 0);
    ReadRequestQueueArray[255].status = 0;
    ReadRequestQueueArray[255].selector = 3077;
    memcpy(queue_saved, ReadRequestQueueArray, sizeof(queue_saved));
    assert(SzFile_InqRequestX(-1, 0) == &ReadRequestQueueArray[255]);
    assert(SzFile_InqRequestX(3077, 0) == &ReadRequestQueueArray[255]);
    assert(SzFile_InqRequestX(3, 1) == &ReadRequestQueueArray[255]);
    assert(SzFile_InqRequestX(3077, 1) == 0);
    assert(SzFile_InqRequestEx() == 1);
    assert(memcmp(queue_saved, ReadRequestQueueArray, sizeof(queue_saved)) == 0);
    ReadRequestQueueArray[0].status = 7;
    ReadRequestQueueArray[0].selector = -32768;
    assert(SzFile_InqRequestX(INT_MIN, INT_MIN) == &ReadRequestQueueArray[0]);
    assert(SzFile_InqRequestX(3, 1) == &ReadRequestQueueArray[255]);
    ReadRequestQueueArray[255].selector = 5120;
    assert(SzFile_InqRequestEx() == 0); /* Group 5 is not among the groups 0..4 it asks. */

    for (mode = 0; mode < 3; mode++) {
        memset(&gSzFile_fl_prefix, 0x5a, sizeof(gSzFile_fl_prefix));
        gSzFile_fl_prefix.cached_result = 91;
        data_00322980[0].bit_1 = mode == 2 ? -1 : 0;
        fetch_calls = consume_calls = 0;
        fetch_result = mode == 0 ? -7 : 123;
        consume_result = INT_MIN;
        expected_cached = mode == 2 ? 91 : fetch_result;
        assert(SzFile_DoRequestS() == (mode == 0 ? 0 : INT_MIN));
        assert(fetch_calls == (mode == 2 ? 0 : 1));
        assert(consume_calls == (mode == 0 ? 0 : 1));
        assert(gSzFile_fl_prefix.cached_result == expected_cached);
        for (i = 0; i < (int)sizeof(gSzFile_fl_prefix.unknown_000); i++) {
            assert(gSzFile_fl_prefix.unknown_000[i] == 0x5a);
        }
    }
    /* The hardware timer loop is compiled but not run on the host. */
    return 0;
}

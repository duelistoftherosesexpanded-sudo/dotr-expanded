/* Synthetic host checks for the resource initialization call contract. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/resource_boot.h"

enum { INIT_RPC = 1, CD_INIT, REBOOT, SYNC, HEAP, LOAD_RESET, CD_MODE, FS_RESET,
       READY, MODULE, LOOKUP, SEARCH, BUFFER, CALLBACK };

struct DotrResourceBootModule gSzFile_aIopList[4];
struct DotrResourceBootEntry mapFile[97];
struct DotrResourceBootState dotr_ee_resource_boot_state_0031c2f0;
char data_002d0638[] = "synthetic-label";
char data_002d0657[] = "synthetic-image";
char data_002d066f[] = "";

static char names[97][8];
static char module_names[3][8] = { "first", "second", "third" };
static int trace[1024], trace_count;
static int reboot_failures, sync_failures, ready_failures;
static int failing_module, module_calls, search_calls, lookup_result;

static void record(int event)
{
    assert(trace_count < (int)(sizeof(trace) / sizeof(trace[0])));
    trace[trace_count++] = event;
}

static int count(int event)
{
    int i, total = 0;
    for (i = 0; i < trace_count; i++) {
        total += trace[i] == event;
    }
    return total;
}

void func_0017e240(int mode) { assert(mode == 0); record(INIT_RPC); }
void func_00191778(int mode) { assert(mode == 0); record(CD_INIT); }
void func_00182838(void) { record(HEAP); }
void func_00182cc8(void) { record(LOAD_RESET); }
void func_00191c50(int mode) { assert(mode == 2); record(CD_MODE); }
void func_0017fa60(void) { record(FS_RESET); }
void lRhFile_CdCallback(void) { assert(0); }

int func_00183a38(const char *image)
{
    assert(image == data_002d0657);
    record(REBOOT);
    return reboot_failures-- > 0 ? 0 : 1;
}

int func_00183a00(void)
{
    record(SYNC);
    return sync_failures-- > 0 ? 0 : -5;
}

int func_00191a58(int mode)
{
    assert(mode == 0);
    record(READY);
    /* Any value other than 2, including 0 and 6, is treated as not ready. */
    return ready_failures-- > 0 ? (ready_failures & 1 ? 6 : 0) : 2;
}

int RhFile_LoadIopModuleWrapper(char *name, int argument)
{
    assert(module_calls < 3);
    assert(name == module_names[module_calls] && argument == 10 + module_calls);
    record(MODULE);
    return module_calls++ == failing_module ? 0 : 1;
}

char *func_00197548(const char *label)
{
    assert(label == data_002d0638);
    record(LOOKUP);
    return lookup_result ? data_002d0638 : 0;
}

int func_00190ff0(struct DotrResourceBootFile *file, const char *name)
{
    int index = search_calls++;
    assert(index < 97 && name == names[index]);
    /* The preceding call must be a ready poll that returned 2. */
    assert(trace[trace_count - 1] == READY);
    record(SEARCH);
    ready_failures = index % 3;
    if (index % 4 == 1) {
        return 0;
    }
    memset(file, 0xa5, sizeof(*file));
    file->first_sector = 1000u + (unsigned int)index;
    file->byte_size = index == 96 ? 0xffffffffu : 2048u * (unsigned int)index + 2047u;
    return index == 0 ? -1 : 1;
}

void func_00190c48(int argument, void *buffer, int size)
{
    assert(argument == 0 && size == 0x4000);
    assert(buffer == dotr_ee_resource_boot_state_0031c2f0.buffer_080);
    record(BUFFER);
}

void func_00190a90(void (*callback)(void))
{
    assert(callback == lRhFile_CdCallback);
    assert(trace[trace_count - 1] == BUFFER);
    record(CALLBACK);
}

static void reset(void)
{
    int i;
    memset(&dotr_ee_resource_boot_state_0031c2f0, 0x5a, sizeof(dotr_ee_resource_boot_state_0031c2f0));
    memset(mapFile, 0x5a, sizeof(mapFile));
    for (i = 0; i < 97; i++) {
        mapFile[i].name = names[i];
    }
    for (i = 0; i < 3; i++) {
        gSzFile_aIopList[i].name = module_names[i];
        gSzFile_aIopList[i].argument = 10 + i;
    }
    gSzFile_aIopList[3].name = 0;
    gSzFile_aIopList[3].argument = 77;
    trace_count = module_calls = search_calls = 0;
    reboot_failures = sync_failures = ready_failures = 0;
    failing_module = -1;
    lookup_result = 1;
}

static void check_prefix(int reboots, int syncs, int readies)
{
    int i, at = 0;
    assert(trace[at++] == INIT_RPC && trace[at++] == CD_INIT);
    for (i = 0; i < reboots; i++) {
        assert(trace[at++] == REBOOT);
    }
    for (i = 0; i < syncs; i++) {
        assert(trace[at++] == SYNC);
    }
    assert(trace[at++] == INIT_RPC && trace[at++] == HEAP && trace[at++] == LOAD_RESET);
    assert(trace[at++] == CD_INIT && trace[at++] == CD_MODE && trace[at++] == FS_RESET);
    for (i = 0; i < readies; i++) {
        assert(trace[at++] == READY);
    }
    assert(trace[at] == MODULE || trace[at] == LOOKUP);
}

int main(void)
{
    static struct DotrResourceBootEntry saved[97];
    static struct DotrResourceBootState saved_state;
    int i, fail;

    assert(offsetof(struct DotrResourceBootState, buffer_080) == 0x80);
    assert(offsetof(struct DotrResourceBootFile, byte_size) == 4);
    assert(offsetof(struct DotrResourceBootEntry, sector_count)
           - offsetof(struct DotrResourceBootEntry, first_sector) == 16);

    /* Each module failure returns 0 before the lookup, table scan and final calls. */
    for (fail = 0; fail < 3; fail++) {
        reset();
        failing_module = fail;
        reboot_failures = 2;
        sync_failures = 3;
        ready_failures = 4;
        memcpy(saved, mapFile, sizeof(saved));
        assert(RhFile_Init() == 0);
        check_prefix(3, 4, 5);
        assert(module_calls == fail + 1 && trace[trace_count - 1] == MODULE);
        assert(count(LOOKUP) == 0 && count(SEARCH) == 0);
        assert(count(BUFFER) == 0 && count(CALLBACK) == 0);
        assert(memcmp(saved, mapFile, sizeof(saved)) == 0);
    }

    /* An empty module table and a null lookup skip the scan but still succeed. */
    reset();
    gSzFile_aIopList[0].name = 0;
    lookup_result = 0;
    memcpy(saved, mapFile, sizeof(saved));
    assert(RhFile_Init() == 1);
    check_prefix(1, 1, 1);
    assert(module_calls == 0 && search_calls == 0 && count(READY) == 1);
    assert(trace[trace_count - 3] == LOOKUP && trace[trace_count - 2] == BUFFER);
    assert(trace[trace_count - 1] == CALLBACK);
    assert(memcmp(saved, mapFile, sizeof(saved)) == 0);

    /* Full path: three modules, then all 97 entries with mixed search results. */
    reset();
    memcpy(saved, mapFile, sizeof(saved));
    memcpy(&saved_state, &dotr_ee_resource_boot_state_0031c2f0, sizeof(saved_state));
    assert(RhFile_Init() == 1);
    check_prefix(1, 1, 1);
    assert(module_calls == 3 && search_calls == 97);
    assert(count(MODULE) == 3 && count(LOOKUP) == 1 && count(SEARCH) == 97);
    assert(count(INIT_RPC) == 2 && count(CD_INIT) == 2);
    assert(trace[trace_count - 3] == SEARCH && trace[trace_count - 2] == BUFFER);
    assert(trace[trace_count - 1] == CALLBACK);
    for (i = 0; i < 97; i++) {
        if (i % 4 == 1) {
            saved[i].name = data_002d066f;
        } else {
            saved[i].first_sector = 1000 + i;
            saved[i].sector_count = i == 96 ? 0x1fffff : i;
        }
    }
    /* Only the three fields the function writes change; other entry bytes are preserved. */
    assert(memcmp(saved, mapFile, sizeof(saved)) == 0);
    assert(memcmp(&saved_state, &dotr_ee_resource_boot_state_0031c2f0, sizeof(saved_state)) == 0);
    return 0;
}

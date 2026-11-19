/* Synthetic host tests for the movie_update group: the call traces of selected dispatcher paths
 * and the state they change. Stubbed callees have assumed semantics. */
#include "dotr/ee/movie_update.h"
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

struct MovieUpdatePrefix dotr_ee_movie_update_state_00317250;
char data_002be80f[] = "MV";
char data_002be809[] = "%s/%s";
const char *data_0028ff10[] = {"intro.pss", "outro.pss"};
static char directory[] = "fixture";
static int file_present, results[2];
static unsigned int path_selection;
struct Call { unsigned int address; int args[5]; };
static struct Call calls[128];
static unsigned int call_count;

static void record(unsigned int address, unsigned int argc, ...)
{
    va_list args;
    unsigned int i;
    assert(call_count < sizeof(calls) / sizeof(calls[0]));
    memset(&calls[call_count], 0, sizeof(calls[call_count]));
    calls[call_count].address = address;
    va_start(args, argc);
    for (i = 0; i < argc; ++i) calls[call_count].args[i] = va_arg(args, int);
    va_end(args);
    ++call_count;
}

#define STUB0(name, address) void name(void) { record(address, 0); }
#define STUB1(name, address) void name(int a) { record(address, 1, a); }
#define STUB2(name, address) void name(int a, int b) { record(address, 2, a, b); }
#define STUB3(name, address) void name(int a, int b, int c) { record(address, 3, a, b, c); }
#define STUB4(name, address) void name(int a, int b, int c, int d) { record(address, 4, a, b, c, d); }
#define STUB5(name, address) void name(int a, int b, int c, int d, int e) { record(address, 5, a, b, c, d, e); }
STUB0(SzDuel_Initialize, 0x001ad960u)
STUB0(lSzGame_InitMemCheck, 0x001ac510u)
STUB0(SzDuel_InitTrade, 0x001c21a0u)
STUB1(SzDebug_SetHideFlag, 0x002051c0u)
STUB1(SzBattle_Init, 0x001e4f00u)
STUB1(SzView_InitCamera, 0x001fc9b0u)
STUB1(ShSlot_Init, 0x00285880u)
STUB1(ShFace_Init, 0x00246450u)
STUB1(SzDuel_InitMapEdit, 0x001cc4b0u)
STUB1(SzEvent_Init, 0x001e77e0u)
STUB1(SzData_SetRoseFlag, 0x002145d0u)
STUB1(TkCamp_Init, 0x0021c510u)
STUB1(lSzGame_InitTitle, 0x001ac610u)
STUB1(SzDuel_InitCustomDuel, 0x001c5d70u)
STUB1(SzDuel_Init3DView, 0x001c1090u)
STUB1(SzDuel_InitBuildDeck, 0x001b8520u)
STUB2(SzBattle_Entry, 0x001e5260u)
STUB2(SzText_LoadMerge, 0x002009e0u)
STUB2(SzModel_SetDrawFlag, 0x001f30b0u)
STUB2(SzDuel_SetupDuel, 0x001ddd80u)
STUB3(SzModel_LoadMerge, 0x001e7f70u)
STUB3(SzDuel_SetBusyFlag, 0x001e3850u)
STUB3(SzSound_TransferSEGrp, 0x00217590u)
STUB3(SzDuel_Init, 0x001add00u)
STUB4(SzDuel_SetUnitStatus, 0x001e1e70u)
STUB4(SzDuel_SetUnitOpenRate, 0x001e1d70u)
STUB4(SzDuel_SetUnitCtlRateM, 0x001e1c10u)
STUB5(SzDuel_AddUnit, 0x001df580u)

void SzDuel_SetPlayer(int selector, unsigned int value)
{
    assert(value < 1000);
    record(0x001de500u, 2, selector, (int)value);
}
int SzData_GetCurrentDeck(int selector)
{
    assert(selector == 0 || selector == 1);
    record(0x002158b0u, 1, selector);
    return selector ? 42 : 21;
}
int SzData_GetLeader(int selector, int value)
{
    assert(selector == 0 || selector == 1);
    assert(value == (selector ? 42 : 21));
    record(0x002159d0u, 2, selector, value);
    return results[selector];
}
char *func_00197548(const char *name)
{
    assert(name == data_002be80f);
    record(0x00197548u, 0);
    return file_present ? directory : 0;
}
char *SzFile_GetEnv(const char *name)
{
    assert(name == data_002be80f);
    record(0x001fff90u, 0);
    return directory;
}
int func_0019b648(char *destination, const char *format, ...)
{
    const char *actual_directory, *filename;
    va_list args;
    assert(format == data_002be809);
    va_start(args, format);
    actual_directory = va_arg(args, const char *);
    filename = va_arg(args, const char *);
    va_end(args);
    assert(actual_directory == directory);
    assert(filename == data_0028ff10[path_selection]);
    record(0x0019b648u, 0);
    strcpy(destination, "fixture/movie.pss");
    return 17;
}
int SzGame_PlayMovie(const char *path, int arg1, int arg2)
{
    assert(strcmp(path, "fixture/movie.pss") == 0);
    record(0x001a83e0u, 2, arg1, arg2);
    return 0;
}

static void seed(void)
{
    unsigned int i;
    memset(&dotr_ee_movie_update_state_00317250, 0x5a,
           sizeof(dotr_ee_movie_update_state_00317250));
    for (i = 0; i < 37; ++i)
        dotr_ee_movie_update_state_00317250.menu_nums[i] = 10000 + i * 1337;
    dotr_ee_movie_update_state_00317250.byte_21e = -1;
    dotr_ee_movie_update_state_00317250.byte_230 = 0;
    results[0] = 999;
    results[1] = 2000;
    file_present = 1;
    call_count = 0;
}
static void unchanged(const struct MovieUpdatePrefix *before)
{
    assert(memcmp(before, &dotr_ee_movie_update_state_00317250, sizeof(*before)) == 0);
}
static void address(unsigned int position, unsigned int expected)
{
    assert(position < call_count && calls[position].address == expected);
}

int main(void)
{
    struct MovieUpdatePrefix before;
    unsigned int i, selector, present;
    int mode;
    assert(offsetof(struct MovieUpdatePrefix, menu_nums) == 0x138);
    assert(offsetof(struct MovieUpdatePrefix, byte_21e) == 0x21e);
    assert(offsetof(struct MovieUpdatePrefix, bits_22e) == 0x22e);
    assert(offsetof(struct MovieUpdatePrefix, byte_230) == 0x230);

    seed();
    before = dotr_ee_movie_update_state_00317250;
    SzGame_ModeInit(-1);
    assert(call_count == 85);
    for (i = 0; i < 84; ++i) {
        address(i, 0x001f30b0u);
        assert(calls[i].args[0] == (int)i && calls[i].args[1] == 0);
    }
    address(84, 0x00217590u);
    ((unsigned char *)&before)[0x22e] |= 0x80;
    unchanged(&before);

    for (mode = 0; mode <= 1; ++mode) {
        seed();
        dotr_ee_movie_update_state_00317250.menu_nums[3] = UINT_MAX;
        dotr_ee_movie_update_state_00317250.menu_nums[4] = 0x80000000u;
        before = dotr_ee_movie_update_state_00317250;
        SzGame_ModeInit(mode);
        assert(call_count == (unsigned int)(8 - mode));
        selector = (unsigned int)(1 - mode);
        if (mode == 0) {
            address(0, 0x002009e0u);
            assert(calls[0].args[0] == 1 && calls[0].args[1] == 0);
        }
        address(selector, 0x002145d0u);
        address(selector + 4, 0x001de500u);
        address(selector + 5, 0x001de500u);
        assert(calls[selector + 4].args[1] == (int)(UINT_MAX % 1000));
        assert(calls[selector + 5].args[1] == (int)(0x80000000u % 1000));
        address(selector + 6, 0x001add00u);
        assert(calls[selector + 6].args[2] == mode);
        unchanged(&before);
    }

    for (mode = 5; mode <= 19; mode += 14) {
        for (selector = 0; selector <= 1; ++selector) {
            seed();
            results[0] = selector ? -7 : 999;
            results[1] = selector ? 999 : 2000;
            before = dotr_ee_movie_update_state_00317250;
            SzGame_ModeInit(mode);
            assert(call_count == (unsigned int)(mode == 5 ? 15 : 16));
            if (mode == 5) {
                address(0, 0x002009e0u);
                assert(calls[0].args[0] == 12 && calls[0].args[1] == 0);
            } else address(2, 0x002051c0u);
            address(8, 0x001df580u);
            address(9, 0x001df580u);
            assert(calls[8].args[0] == 0 && calls[8].args[1] == 3 &&
                   calls[8].args[2] == 6 && calls[8].args[3] == (selector ? -7 : 0) && calls[8].args[4] == 1);
            assert(calls[9].args[0] == 1 && calls[9].args[1] == 3 &&
                   calls[9].args[2] == 0 && calls[9].args[3] == (selector ? 0 : 2000) && calls[9].args[4] == 1);
            unchanged(&before);
        }
    }

    for (present = 0; present <= 1; ++present) {
        for (selector = 0; selector <= 1; ++selector) {
            for (i = 0; i <= 1; ++i) {
                seed();
                file_present = (int)present;
                path_selection = selector;
                dotr_ee_movie_update_state_00317250.byte_230 = (unsigned char)selector;
                dotr_ee_movie_update_state_00317250.menu_nums[10] = i;
                before = dotr_ee_movie_update_state_00317250;
                SzGame_ModeInit(10);
                assert(call_count == (present && i == 0 ? 9u : 6u));
                address(2, 0x00197548u);
                if (present && i == 0) {
                    address(3, 0x001fff90u);
                    address(4, 0x0019b648u);
                    address(5, 0x001a83e0u);
                    assert(calls[5].args[0] == 1 && calls[5].args[1] == 1);
                    before.byte_230 = (unsigned char)(1 - selector);
                }
                unchanged(&before);
            }
        }
    }

    seed();
    before = dotr_ee_movie_update_state_00317250;
    SzGame_ModeInit(11);
    assert(call_count == 14);
    before.menu_nums[28] = before.menu_nums[29] = 3;
    unchanged(&before);

    for (i = 0; i <= 1; ++i) {
        seed();
        dotr_ee_movie_update_state_00317250.menu_nums[15] = i;
        before = dotr_ee_movie_update_state_00317250;
        SzGame_ModeInit(15);
        assert(call_count == (i ? 5u : 4u));
        address(call_count - 1, 0x00285880u);
        assert(calls[call_count - 1].args[0] == (int)i);
        before.menu_nums[15] = 0;
        unchanged(&before);
    }

    for (mode = -1; mode <= 0; ++mode) {
        seed();
        dotr_ee_movie_update_state_00317250.byte_21e = (signed char)mode;
        before = dotr_ee_movie_update_state_00317250;
        SzGame_ModeInit(7);
        assert(call_count == (mode == -1 ? 3u : 2u));
        address(call_count - 1, 0x001c1090u);
        assert(calls[call_count - 1].args[0] == -1);
        unchanged(&before);
    }

    seed();
    before = dotr_ee_movie_update_state_00317250;
    SzGame_ModeInit(INT_MIN);
    SzGame_ModeInit(2);
    SzGame_ModeInit(INT_MAX);
    assert(call_count == 0);
    unchanged(&before);
    return 0;
}

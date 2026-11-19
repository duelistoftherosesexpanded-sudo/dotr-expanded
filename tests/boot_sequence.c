/* Synthetic host tests for the boot_sequence group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <setjmp.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/boot_sequence.h"

/* Every callee is a stub that records its address and arguments. */
static unsigned log_id[160];
static int log_arg[160][7];
static int log_n;
static int open_result, ready_result, load_result, flag_result, mode_result;
static int state_word;

static int note(unsigned id, int a, int b, int c, int d, int e, int f, int g)
{
    int *row = log_arg[log_n];
    assert(log_n < 160);
    log_id[log_n++] = id;
    row[0] = a; row[1] = b; row[2] = c; row[3] = d; row[4] = e; row[5] = f; row[6] = g;
    return 0;
}

#define STUB0(name, addr) int name(void) { return note(0x##addr, 0, 0, 0, 0, 0, 0, 0); }
#define STUB1(name, addr) int name(int a) { return note(0x##addr, a, 0, 0, 0, 0, 0, 0); }
#define STUB2(name, addr) int name(int a, int b) { return note(0x##addr, a, b, 0, 0, 0, 0, 0); }

STUB0(SzFile_Init, 001feb80) STUB0(SzText_Init, 002007d0) STUB0(SzPad_Init, 002031f0) STUB0(SzSound_Init, 00217410) STUB0(SzModel_Init, 001e7a60)
STUB0(SzView_Init, 001f74b0) STUB0(SzFade_Init, 001fcc20) STUB0(SzGame_Init, 001a6c80) STUB0(SzEffect_Init, 001f3700) STUB0(SzDebug_Init, 00203cd0)
STUB0(SzMsg_Initialize, 001fd2b0) STUB0(SzData_Initialize, 00214290) STUB0(SzDuel_Initialize, 001ad960) STUB0(SzBattle_Initialize, 001e4e90) STUB0(lMain_LoadModel, 00100780)
STUB0(RhBase_Term, 00209870) STUB0(SzPad_Status, 00203320) STUB0(SzSound_VSync, 00217550) STUB0(SzFile_DoRequest, 001ff3d0) STUB0(SzGame_ModeAct, 001aa260)
STUB0(SzDuel_DoUnitRequest, 001dd220) STUB0(Main_Draw, 00100850) STUB0(SzGame_AddFrameCount, 001a8730) STUB0(SzPad_Act, 00203840) STUB0(func_0019a5c8, 0019a5c8)
STUB0(SzPad_VSync, 002038f0) STUB0(SzGame_VSync, 001a86f0)
STUB1(SzView_Destroy, 001f7720) STUB1(SzModel_Destroy, 001e7de0) STUB1(SzFade_SetKillFlag, 001fd0b0) STUB1(SzGame_SetFrameSpeed, 001a8750) STUB1(SzGame_SetStabilizer, 001a8770)
STUB2(SzGame_SetMode, 001a89c0) STUB2(SzFade_SetPauseFlag, 001fd130) STUB2(SzView_DoRequest, 001fa220)

int RhBase_Init(int a, int b, int c, int d, int e, int f, int g)
{
    note(0x00209510, a, b, c, d, e, f, g);
    return open_result;
}
int RhBase_SetClearColorEx(int a, int b, int c, int d) { return note(0x00209e90, a, b, c, d, 0, 0, 0); }
int SzGame_ClearGsMem(int a, int b, int c, int d, int e, int f) { return note(0x001a8520, a, b, c, d, e, f, 0); }
int SzFade_Entry(int a, int b, int c, int d, int e, int f) { return note(0x001fcc70, a, b, c, d, e, f, 0); }
int SzPad_SetParam(int a, int b, int c) { return note(0x002032c0, a, b, c, 0, 0, 0, 0); }
int SzView_Create(int a) { note(0x001f74c0, a, 0, 0, 0, 0, 0, 0); return ready_result; }
int lMain_InitModel2(void) { note(0x00100350, 0, 0, 0, 0, 0, 0, 0); return load_result; }
int RhBase_FlipBuffer(void) { note(0x002099d0, 0, 0, 0, 0, 0, 0, 0); return mode_result; }
int SzGame_GetStabilizer(void) { note(0x001a87a0, 0, 0, 0, 0, 0, 0, 0); return flag_result; }
int *SzPad_Term(void) { note(0x002032a0, 0, 0, 0, 0, 0, 0, 0); return &state_word; }

/* 0x100520 never returns: the 0x1aaf60 stub answers from poll_by and leaves
 * through poll_exit when its answers run out. */
static jmp_buf poll_exit;
static int poll_by[4], poll_count, poll_calls, entry_calls, round_len[4], round_first[4];
static void (*entry_seen)(void);

void SzEffect_StoneProc(void) {}
int func_00209f60_entry(void (*entry)(void))
{
    entry_seen = entry;
    entry_calls++;
    return 0;
}
int SzGame_GetMode(int a)
{
    assert(a == 1);
    if (poll_calls > 0) {
        round_len[poll_calls - 1] = log_n;
        round_first[poll_calls - 1] = (int)log_id[0];
    }
    if (poll_calls == poll_count) {
        longjmp(poll_exit, 1);
    }
    log_n = 0;
    return poll_by[poll_calls++];
}

static void expect(int at, unsigned id, int a, int b, int c, int d, int e, int f, int g)
{
    const int want[7] = { a, b, c, d, e, f, g };
    assert(at < log_n);
    assert(log_id[at] == id);
    assert(memcmp(log_arg[at], want, sizeof want) == 0);
}

static void expect_ids(const unsigned *ids, int count)
{
    int i;
    assert(log_n == count);
    for (i = 0; i < count; i++)
        assert(log_id[i] == ids[i]);
}

static void check_start(void)
{
    static const unsigned full[] = {
        0x1feb80, 0x209510, 0x209e90, 0x1a8520, 0x2007d0, 0x2031f0, 0x217410, 0x1e7a60,
        0x1f74b0, 0x1fcc20, 0x1a6c80, 0x1f3700, 0x203cd0, 0x2032c0, 0x2032c0, 0x1f74c0,
        0x1fd2b0, 0x214290, 0x1ad960, 0x1e4e90, 0x100350, 0x1a89c0, 0x1fcc70, 0x1fd0b0,
        0x1fd130, 0x100780,
    };

    /* first step fails: nothing after 0x209510 */
    log_n = 0; open_result = 0; ready_result = 1; load_result = 1;
    assert(Main_Init() == 0);
    expect_ids(full, 2);
    expect(1, 0x209510, 0, 10, 0x31, 0x600000, 0x780000, 0x180000, 0x4000000);

    /* 0x1f74c0 fails: stops after it */
    log_n = 0; open_result = 1; ready_result = 0;
    assert(Main_Init() == 0);
    expect_ids(full, 16);

    /* 0x100350 fails: stops after it */
    log_n = 0; ready_result = 5; load_result = 0;
    assert(Main_Init() == 0);
    expect_ids(full, 21);

    /* every step succeeds */
    log_n = 0; load_result = 2;
    assert(Main_Init() == 1);
    expect_ids(full, 26);
    expect(2, 0x209e90, 0, 0, 0, 0, 0, 0, 0);
    expect(3, 0x1a8520, 10, 0, 0, 0, 0x400, 0x400, 0);
    expect(13, 0x2032c0, 0, 12, 1, 0, 0, 0, 0);
    expect(14, 0x2032c0, 1, 12, 1, 0, 0, 0, 0);
    expect(15, 0x1f74c0, 0, 0, 0, 0, 0, 0, 0);
    expect(21, 0x1a89c0, 0x24, 0, 0, 0, 0, 0, 0);
    expect(22, 0x1fcc70, 1, 0, 1, 0, 0x5a, 0, 0);
    expect(23, 0x1fd0b0, 1, 0, 0, 0, 0, 0, 0);
    expect(24, 0x1fd130, 1, 1, 0, 0, 0, 0, 0);
}

static void check_release(void)
{
    int i;

    log_n = 0;
    assert(Main_Term() == &state_word);
    assert(log_n == 0x54 + 3);
    expect(0, 0x1f7720, 0, 0, 0, 0, 0, 0, 0);
    for (i = 0; i < 0x54; i++)
        expect(1 + i, 0x1e7de0, i, 0, 0, 0, 0, 0, 0);
    expect(0x55, 0x209870, 0, 0, 0, 0, 0, 0, 0);
    expect(0x56, 0x2032a0, 0, 0, 0, 0, 0, 0, 0);
}

static void check_tail(void)
{
    static const unsigned ids[] = { 0x19a5c8, 0x2038f0, 0x1a86f0 };

    log_n = 0;
    assert(Main_VSync() == 0);
    expect_ids(ids, 3);
}

static void check_shutdown(void)
{
    static const unsigned set[] = {
        0x203320, 0x217550, 0x1ff3d0, 0x1aa260, 0x1fa220, 0x1dd220, 0x100850, 0x2099d0,
        0x1a8750, 0x1a87a0, 0x1a8750, 0x19a5c8, 0x2038f0, 0x1a86f0, 0x1a8730, 0x203840,
    };
    static const unsigned clear[] = {
        0x203320, 0x217550, 0x1ff3d0, 0x1aa260, 0x1fa220, 0x1dd220, 0x100850, 0x2099d0,
        0x1a8750, 0x1a87a0, 0x1a8770, 0x19a5c8, 0x2038f0, 0x1a86f0, 0x1a8730, 0x203840,
    };

    /* 0x1a87a0 non-zero: SzGame_SetFrameSpeed(1) */
    log_n = 0; mode_result = 7; flag_result = 3;
    Main_LoopProc();
    expect_ids(set, 16);
    expect(4, 0x1fa220, -1, -1, 0, 0, 0, 0, 0);
    expect(8, 0x1a8750, 7, 0, 0, 0, 0, 0, 0);
    expect(10, 0x1a8750, 1, 0, 0, 0, 0, 0, 0);

    /* 0x1a87a0 zero: SzGame_SetStabilizer(1) */
    log_n = 0; mode_result = 0; flag_result = 0;
    Main_LoopProc();
    expect_ids(clear, 16);
    expect(8, 0x1a8750, 0, 0, 0, 0, 0, 0, 0);
    expect(10, 0x1a8770, 1, 0, 0, 0, 0, 0, 0);
}

static void check_main_loop(void)
{
    /* three rounds: 0x100780 runs before 0x100560 only when the answer is 1 */
    poll_by[0] = 1; poll_by[1] = 0; poll_by[2] = 2;
    poll_count = 3; poll_calls = 0; entry_calls = 0;
    mode_result = 0; flag_result = 0;
    if (setjmp(poll_exit) == 0) {
        Main_Loop();
        assert(0);
    }
    assert(entry_calls == 1 && entry_seen == SzEffect_StoneProc && poll_calls == 3);
    /* 0x100560 logs sixteen calls (see check_shutdown) */
    assert(round_len[0] == 17 && round_first[0] == 0x100780);
    assert(round_len[1] == 16 && round_first[1] == 0x203320);
    assert(round_len[2] == 16 && round_first[2] == 0x203320);
}

int main(void)
{
    check_start();
    check_release();
    check_tail();
    check_shutdown();
    check_main_loop();
    return 0;
}

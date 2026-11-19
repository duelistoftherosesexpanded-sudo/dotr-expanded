/* Synthetic host tests for the tkcamp_act group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/tkcamp_act.h"

union DotrTkCampBoard dotr_tkcamp_board;
union DotrTkCampWork dotr_tkcamp_work;
short data_00298260_tkcamp[4 * 0x13];
struct DotrTkCampQ data_0029b000_tkcamp;
struct DotrTkCampQ data_0029b010_tkcamp;
signed char data_0030f1f6_tkcamp;
signed char data_0030f213_tkcamp;
signed char data_0030f227_tkcamp;
signed char data_0030f244_tkcamp;

#define WB(a) (*(signed char *)DOTR_TKCAMP_AT(a))
#define WH(a) (*(short *)DOTR_TKCAMP_AT(a))
#define WF(a) (*(float *)DOTR_TKCAMP_AT(a))
#define BB(o) (dotr_tkcamp_board.bytes[o])
#define BH(o) (*(short *)(dotr_tkcamp_board.bytes + (o)))

/* Every stubbed call is logged as (callee address, up to three words). */
static long calls[64][4];
static int n_calls;
static unsigned int pad_masks[3]; /* buttons reported by SzPad_Check per kind */
static int effect_busy, model_loading, deck = 7, leader = 0x1234, xax = 0x55;
static struct DotrTkCampVec4 view_sp = { 1.0f, 2.0f, 3.0f, 4.0f };
static struct DotrTkCampQ pos_seen, rot_seen;

static void log_call(long id, long a, long b, long c)
{
    assert(n_calls < 64);
    calls[n_calls][0] = id;
    calls[n_calls][1] = a;
    calls[n_calls][2] = b;
    calls[n_calls][3] = c;
    n_calls++;
}
/* Index of the first logged call to id at or after `from`, or -1. */
static int find(long id, int from)
{
    int i;

    for (i = from; i < n_calls; i++) {
        if (calls[i][0] == id) {
            return i;
        }
    }
    return -1;
}
static int count(long id)
{
    int i, n = 0;

    for (i = 0; i < n_calls; i++) {
        n += calls[i][0] == id;
    }
    return n;
}
static int called(long id, long a, long b, long c)
{
    int i;

    for (i = 0; i < n_calls; i++) {
        if (calls[i][0] == id && calls[i][1] == a && calls[i][2] == b && calls[i][3] == c) {
            return 1;
        }
    }
    return 0;
}

int func_00195070_tkcamp(int value) { return value < 0 ? -value : value; }
void func_00199b50_tkcamp(const signed char *fmt, ...) { log_call(0x199b50, (long)fmt, 0, 0); }
int func_001a8320_tkcamp(int a, int b) { log_call(0x1a8320, a, b, 0); return 0; }
void func_001a8870_tkcamp(int a, int b) { log_call(0x1a8870, a, b, 0); }
void func_001a8920_tkcamp(void) { log_call(0x1a8920, 0, 0, 0); }
void func_001aafa0_tkcamp(int a) { log_call(0x1aafa0, a, 0, 0); }
int func_001ab1d0_tkcamp(void) { log_call(0x1ab1d0, 0, 0, 0); return 0; }
int func_001de1a0_tkcamp(int a) { log_call(0x1de1a0, a, 0, 0); return xax; }
void func_001e3ff0_tkcamp(int a, int b, int c) { log_call(0x1e3ff0, a, b, c); }
int func_001e40a0_tkcamp(int a) { log_call(0x1e40a0, a, 0, 0); return 0; }
void func_001e42b0_tkcamp(void) { log_call(0x1e42b0, 0, 0, 0); }
void func_001e42c0_tkcamp(int a, int b, int c, int d) { log_call(0x1e42c0, a, b, c); assert(d == 0x91 || d == -1); }
int func_001e4430_tkcamp(int a) { log_call(0x1e4430, a, 0, 0); return 0; }
void func_001e4530_tkcamp(void) { log_call(0x1e4530, 0, 0, 0); }
int func_001e8780_tkcamp(int a, int b, int c, int d) { log_call(0x1e8780, a, b, c); assert(d == 1); return 0; }
int func_001f12a0_tkcamp(int a) { log_call(0x1f12a0, a, 0, 0); return model_loading; }
int func_001f22b0_tkcamp(int index, void *src)
{
    log_call(0x1f22b0, index, 0, 0);
    memcpy(&pos_seen, src, sizeof(pos_seen));
    return 0;
}
int func_001f2430_tkcamp(int index, void *src)
{
    log_call(0x1f2430, index, 0, 0);
    memcpy(&rot_seen, src, sizeof(rot_seen));
    return 0;
}
int func_001f5240_tkcamp(int a, int b) { assert(a == -1 && b == -1); return effect_busy; }
DotrTkCampWord func_001f9ad0_tkcamp(int a) { (void)a; return (DotrTkCampWord)&view_sp; }
int func_001f9fe0_tkcamp(int a, void *b, void *c)
{
    assert(b == DOTR_TKCAMP_AT(0x01ED0030) && c == DOTR_TKCAMP_AT(0x01ED0040));
    log_call(0x1f9fe0, a, 0, 0);
    return 0;
}
int func_00203a40_tkcamp(int pad, int kind, int mask)
{
    assert(pad == 0 && kind >= 0 && kind < 3);
    return (pad_masks[kind] & (unsigned int)mask) != 0;
}
int func_00203bc0_tkcamp(int pad) { (void)pad; return 0; }
unsigned char func_00203c40_tkcamp(int pad, int axis) { (void)pad; (void)axis; return 0x80; }
void func_00205cb0_tkcamp(void *out, float x, float y) { (void)out; (void)x; (void)y; }
void func_00206430_tkcamp(void *a, void *b, void *c, void *d) { (void)a; (void)b; (void)c; (void)d; }
void func_00214990_tkcamp(int a, int b) { log_call(0x214990, a, b, 0); }
int func_002158b0_tkcamp(int a) { log_call(0x2158b0, a, 0, 0); return deck; }
int func_002159d0_tkcamp(int a, int b) { log_call(0x2159d0, a, b, 0); return leader; }
int func_00217710_tkcamp(int a, int b) { log_call(0x217710, a, b, 0); return 0; }
void func_00224a40_tkcamp(void) { log_call(0x224a40, 0, 0, 0); }
void func_00226600_tkcamp(void) { log_call(0x226600, 0, 0, 0); }
int func_0022d140_tkcamp(void) { log_call(0x22d140, 0, 0, 0); return 0; }
void func_0023b1f0_tkcamp(int a) { log_call(0x23b1f0, a, 0, 0); }
void func_0023bb00_tkcamp(int a) { log_call(0x23bb00, a, 0, 0); }
void func_0023bc80_tkcamp(void) { log_call(0x23bc80, 0, 0, 0); }
void func_00245d80_tkcamp(unsigned char *face)
{
    assert(face == DOTR_TKCAMP_AT(0x01ED5424));
    log_call(0x245d80, 0, 0, 0);
}

static void reset(short event)
{
    memset(&dotr_tkcamp_board, 0, sizeof(dotr_tkcamp_board));
    memset(&dotr_tkcamp_work, 0, sizeof(dotr_tkcamp_work));
    memset(data_00298260_tkcamp, 0, sizeof(data_00298260_tkcamp));
    memset(pad_masks, 0, sizeof(pad_masks));
    n_calls = 0;
    effect_busy = model_loading = 0;
    BH(0xc2) = event;
}
/* The frame ends normally: Taku_Camp_System first, then SzView_SetCamera and TkCamp_Act2. */
static void assert_full_frame(void)
{
    int sys = find(0x226600, 0);

    assert(sys >= 0 && find(0x1f9fe0, sys) > sys && find(0x23b1f0, sys) == n_calls - 1);
    assert(calls[n_calls - 1][1] == 0);
}

/* Branch coverage over the fades, the cursor spot hit test and its menus, two event cases,
 * the button-0x40 pass and the four camera modes (byte 0x1ed53b3). */
static void check_fades(void)
{
    /* Event below 10000: both fades rise, the window fade sets up the face. */
    reset(0x10);
    WB(0x01ED006E) = 1; WH(0x01ED0070) = 0xF0;
    WB(0x01ED006A) = 1; WH(0x01ED006C) = 0x10;
    WB(0x01ED5481) = 0; WB(0x01ED547A) = 5;
    TkCamp_Act();
    assert(WH(0x01ED0070) == 0x100 && WB(0x01ED5374) == 0);
    assert(WH(0x01ED006C) == 0x24 && WB(0x01ED540A) == 1 && WB(0x01ED5406) == 0);
    assert(WB(0x01ED547A) == 0 && WB(0x01ED5481) == 1 && WB(0x01ED5482) == 0);
    assert(WH(0x01ED536E) == 0x10 && count(0x245d80) == 0);
    assert_full_frame();

    /* Below 10000 with both fades off: both fall to 0; the face is closed once, the chosen
     * label byte is copied. */
    reset(0x10);
    WH(0x01ED0070) = 0x10; WH(0x01ED006C) = 0x10; WB(0x01ED540A) = 1;
    BB(0xc9) = 2; WB(0x01ED53C1) = 1;
    TkCamp_Act();
    assert(WH(0x01ED0070) == 0 && WB(0x01ED5374) == 1 && WH(0x01ED006C) == 0);
    assert(WB(0x01ED53C2) == 2 && WB(0x01ED53C1) == 0 && WB(0x01ED540A) == 0 && count(0x245d80) == 1);

    /* Event 10000 or more: both fades forced off and falling (partly). */
    reset(0x2800);
    WB(0x01ED006E) = 1; WB(0x01ED006A) = 1; WH(0x01ED0070) = 0x40; WH(0x01ED006C) = 0x40;
    WB(0x01ED540A) = 1;
    TkCamp_Act();
    assert(WB(0x01ED006E) == 0 && WB(0x01ED006A) == 0 && WB(0x01ED5374) == 1);
    assert(WH(0x01ED0070) == 0x2C && WH(0x01ED006C) == 0x2C && count(0x245d80) == 0);
    assert_full_frame();
}

static void spot_frame(short menu, unsigned int buttons)
{
    reset(0x10);
    WB(0x01ED5376) = 1;
    WB(0x01ED006E) = 1; WH(0x01ED0070) = 0x100; /* window fade done: 0x1ed5374 cleared */
    data_00298260_tkcamp[2 * 0x13 + 0] = 100;     /* spot 2: box (100, 50) - (120, 70) */
    data_00298260_tkcamp[2 * 0x13 + 1] = 50;
    data_00298260_tkcamp[2 * 0x13 + 2] = 120;
    data_00298260_tkcamp[2 * 0x13 + 3] = 70;
    WH(0x01ED0000) = 98;                        /* inside by the 2-pixel margin */
    WH(0x01ED0002) = 72;
    WH(0x01ED0080) = menu;
    pad_masks[0] = buttons;
}

static void check_spot_menus(void)
{
    /* On a spot, menu 0, button 0x40: sound 4, menu 6 pushed, the frame ends early. */
    spot_frame(0, 0x40);
    TkCamp_Act();
    assert(WB(0x01ED547A) == 1);
    assert(n_calls == 3 && called(0x217710, 0, 4, 0) && called(0x1a8320, 6, 0, 0) && called(0x1a8870, 6, 0, 0));
    /* Just outside the margin: no hit, the frame runs on. */
    spot_frame(0, 0x40);
    WH(0x01ED0000) = 97;
    TkCamp_Act();
    assert(WB(0x01ED547A) == 0 && count(0x1a8870) == 0);
    /* Menu 1 (memory card), button 0x40: card processing starts, state 1. */
    spot_frame(1, 0x40);
    TkCamp_Act();
    assert(n_calls == 2 && called(0x1aafa0, 1, 0, 0) && WB(0x01ED543C) == 1);
    /* State 1: the card process runs and the frame ends. */
    spot_frame(1, 0);
    WB(0x01ED543C) = 1;
    TkCamp_Act();
    assert(n_calls == 1 && calls[0][0] == 0x1ab1d0 && WB(0x01ED543C) == 1);
    /* Menu 2: the option popup opens for the side, then slides in by 64 a step. */
    spot_frame(2, 0x40);
    BB(0) = 1;
    TkCamp_Act();
    assert(called(0x1e3ff0, 1, 0, 0x200) && WB(0x01ED543C) == 1 && WB(0x01ED543D) == 0);
    spot_frame(2, 0);
    WB(0x01ED543C) = 1; WB(0x01ED543D) = 7;
    TkCamp_Act();
    assert(called(0x1e3ff0, -1, 0, 0) && WB(0x01ED543C) == 2 && WB(0x01ED543D) == 8);
    /* The effect still running: nothing of the spot block. */
    spot_frame(0, 0x40);
    effect_busy = 1;
    TkCamp_Act();
    assert(WB(0x01ED547A) == 0 && count(0x217710) == 0);
    assert_full_frame();
    /* Off any spot, button 0x100: the message popup 0x91 opens and the frame ends. */
    spot_frame(0, 0x100);
    WH(0x01ED0000) = 0;
    TkCamp_Act();
    assert(n_calls == 2 && called(0x217710, 0, 4, 0) && called(0x1e42c0, 7, 0, 0x200) && WB(0x01ED543E) == 1);
}

static void check_events(void)
{
    /* 0x2742 in step 2, button 0x800 with entry 0 chosen: the practice duel starts. */
    reset(0x2742);
    WB(0x01ED53A6) = 2;
    pad_masks[0] = 0x800;
    TkCamp_Act();
    assert(WB(0x01ED53A6) == 3 && called(0x217710, 0, 1, 0) && called(0x23bb00, 0, 0, 0));
    assert_full_frame();
    /* 0x276a with board byte 1 set: left/right only sound 6, no choice. */
    reset(0x276a);
    WB(0x01ED53A6) = 2; WB(0x01ED53A8) = 1; BB(1) = 1;
    pad_masks[0] = 0x1000;
    TkCamp_Act();
    assert(called(0x217710, 0, 6, 0) && WB(0x01ED53A8) == 1 && count(0x23bb00) == 0);
    /* 0x2760 at stage 4: the deck is set up and, the model being free, leader model 0x1e is
     * placed and loaded; stage 5. */
    reset(0x2760);
    WB(0x01ED5494) = 4; WB(0x01ED53A8) = 1;
    *(unsigned short *)DOTR_TKCAMP_AT(0x01ED53FA) = 0x321;
    data_0029b000_tkcamp.q = 0x11; data_0029b010_tkcamp.q = 0x22;
    TkCamp_Act();
    assert(called(0x214990, 0, 0x321, 0) && called(0x1f22b0, 0x1e, 0, 0) && called(0x1f2430, 0x1e, 0, 0));
    assert(pos_seen.q == 0x11 && rot_seen.q == 0x22 && WH(0x01ED5404) == leader);
    assert(called(0x2159d0, 0, deck, 0) && called(0x1de1a0, leader, 0, 0) && called(0x1e8780, 0x1e, -1, xax));
    assert(WB(0x01ED5494) == 5);
    /* The model still loading: only the deck. */
    reset(0x2760);
    WB(0x01ED5494) = 4;
    model_loading = 1;
    TkCamp_Act();
    assert(count(0x214990) == 1 && count(0x1e8780) == 0 && WB(0x01ED5494) == 5);
    /* A new event number clears the step and the flag byte. */
    reset(0x30);
    WH(0x01ED536E) = 0x31; WH(0x01ED536C) = 9; WB(0x01ED537A) = 1;
    TkCamp_Act();
    assert(WH(0x01ED536C) == 0 && WB(0x01ED537A) == 0 && WH(0x01ED536E) == 0x30);
    /* Event 10001 or more with button 0x40: step 4 -> 5 and 0x1ed53ac 2 -> 3, BUN_HANTEI. */
    reset(0x2711);
    WB(0x01ED537B) = 4; WB(0x01ED53AC) = 2; WB(0x01ED53B2) = 9;
    pad_masks[0] = 0x40;
    TkCamp_Act();
    assert(WB(0x01ED537B) == 5 && WB(0x01ED53AC) == 3 && WB(0x01ED53B2) == 0 && count(0x22d140) == 1);
}

static void check_cameras(void)
{
    /* Mode 1: rising height clamped at 0x1000; focus zeroed, eye from it. */
    reset(0x10);
    WB(0x01ED53B3) = 1; WB(0x01ED0062) = 1; WH(0x01ED0064) = 0xFF8; WF(0x01ED0044) = 5.0f;
    TkCamp_Act();
    assert(WH(0x01ED0064) == 0x1000 && WB(0x01ED0062) == 0);
    assert(WF(0x01ED0040) == 0.0f && WF(0x01ED0030) == 0.0f && WF(0x01ED0038) == 1.0f);
    assert(WF(0x01ED0034) == 20.0f + 5.0f + 1700.0f);
    /* Mode 2 without shake: fixed eye, focus from the tilt byte and the height. */
    reset(0x10);
    WB(0x01ED53B3) = 2; WH(0x01ED0064) = 0x800; WB(0x01ED5437) = 50;
    TkCamp_Act();
    assert(WF(0x01ED0038) == 512.0f && WF(0x01ED0040) == -32.0f && WF(0x01ED0044) == 20.2f + 384.0f);
    /* Mode 2 shaking (state 1): the strength rises by 5, frame 1: x -0.1, y -0.1. */
    reset(0x10);
    WB(0x01ED53B3) = 2; WB(0x01ED547F) = 1; *(unsigned char *)DOTR_TKCAMP_AT(0x01ED547E) = 1;
    TkCamp_Act();
    assert(WB(0x01ED5480) == 5 && WF(0x01ED548C) == -0.1f && WF(0x01ED5490) == -0.1f);
    assert(WF(0x01ED0030) == -0.1f && *(unsigned char *)DOTR_TKCAMP_AT(0x01ED547E) == 2);
    /* Mode 3 shake state 3 down to 0: state 4. */
    reset(0x10);
    WB(0x01ED53B3) = 3; WB(0x01ED547F) = 3; WB(0x01ED5480) = 5;
    TkCamp_Act();
    assert(WB(0x01ED5480) == 0 && WB(0x01ED547F) == 4);
    /* Default mode, falling height clamped at 0. */
    reset(0x10);
    WB(0x01ED0062) = 2; WH(0x01ED0064) = 4; WF(0x01ED0040) = 7.0f; WF(0x01ED0048) = 1.0f;
    TkCamp_Act();
    assert(WH(0x01ED0064) == 0 && WB(0x01ED0062) == 0);
    assert(WF(0x01ED0030) == 7.0f && WF(0x01ED0034) == 96.0f && WF(0x01ED0038) == 97.0f);
    assert_full_frame();
}

int main(void)
{
    check_fades();
    check_spot_menus();
    check_events();
    check_cameras();
    puts("tkcamp_act: ok");
    return 0;
}

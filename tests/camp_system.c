/* Synthetic host tests for the camp_system group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_system.h"

union DotrCampSystemWork dotr_camp_system_work;
struct DotrCampSystemSpot data_00298ba0_campsystem[22];
struct DotrCampSystemBoard data_00299e10_campsystem;
struct DotrCampSystemList data_00299020_campsystem[2][16];
int data_0034f6f0_campsystem;

/* The state every run is expected to leave: copied from the fixtures before the call, then
 * changed by the test exactly as the function should have changed the real ones. */
static union DotrCampSystemWork want_work;
static struct DotrCampSystemBoard want_board;
static int want_word;

#define WB(o) (*(signed char *)(dotr_camp_system_work.bytes + (o)))
#define WH(o) (*(short *)(dotr_camp_system_work.bytes + (o)))
#define WF(o) (*(float *)(dotr_camp_system_work.bytes + (o)))
#define XB(o) (*(signed char *)(want_work.bytes + (o)))
#define XH(o) (*(short *)(want_work.bytes + (o)))
#define XF(o) (*(float *)(want_work.bytes + (o)))
#define board data_00299e10_campsystem
#define spot data_00298ba0_campsystem
#define STEP 0x536c

/* What was called, one word per call, in order. */
static char seen[512];
static int ledar_answer, ido_answer, ivent_clears_bgm;
/* The board's location byte `watch` as the route and flow callees saw it. */
static int watch, watched;

static void said(const char *format, ...)
{
    va_list list;
    size_t used = strlen(seen);

    va_start(list, format);
    vsnprintf(seen + used, sizeof seen - used, format, list);
    va_end(list);
    assert(strlen(seen) + 1 < sizeof seen);
}
static void said_list(const short *list, int count)
{
    int i;

    assert(count >= 0 && count <= 8);
    for (i = 0; i < count; i++) {
        said("%c%d", i ? ',' : ':', list[i]);
    }
    said(") ");
}

int func_00229140_campsystem(int a, int b, int limit)
{
    said("kettei(%d,%d,%d) ", a, b, limit);
    return 0;
}
int func_002294b0_campsystem(int side, int pick)
{
    said("katimake(%d,%d) ", side, pick);
    return 0;
}
void func_00217980_campsystem(int n, int b)
{
    said("bgm(%d,%d) ", n, b);
}
void func_0021e330_campsystem(void)
{
    said("ivent ");
    if (ivent_clears_bgm) {
        WB(0x546c) = 0;
        XB(0x546c) = 0;
    }
}
void Taku_Fune_init(int a, int b)
{
    said("fune(%d,%d) ", a, b);
}
void func_00228660_campsystem(int side, int row, int value, short *list, int count)
{
    said("root(%d,%d,%d", side, row, value);
    said_list(list, count);
    watched = board.lv[watch];
}
int My_LedarModel_a_set(int dir)
{
    said("ledar(%d) ", dir);
    return ledar_answer;
}
int Taku_Tyumoku_ido(void)
{
    said("ido ");
    return ido_answer;
}
void func_00228460_campsystem(int side, int row, short *list, int count, int flag)
{
    said("flow(%d,%d,%d", side, row, flag);
    said_list(list, count);
    watched = board.lv[watch];
}
int func_00228f50_campsystem(int side, int row)
{
    said("ido1(%d,%d) ", side, row);
    return 0;
}

/* Location k of side s lists the routes 2 + s, 5 + s, ... (k % 4 + 1 of them). */
static void reset(int side, int ticks)
{
    int k, s, i;

    memset(&dotr_camp_system_work, 0x11, sizeof dotr_camp_system_work);
    memset(&board, 0, sizeof board);
    memset(data_00299020_campsystem, 0, sizeof data_00299020_campsystem);
    memset(spot, 0, sizeof spot);
    for (k = 0; k < 22; k++) {
        spot[k].x = (float)(1000 + k);
        spot[k].y = (float)(2000 + k);
        spot[k].n = (short)(k % 4 + 1);
        for (s = 0; s < 2; s++) {
            for (i = 0; i < 4; i++) {
                spot[k].list[s][i] = (short)((2 + s + 3 * i + k) % 16);
            }
        }
    }
    board.b0 = (signed char)side;
    board.h194 = (short)ticks;
    board.b198 = 7;
    board.b199 = 55;
    board.b201 = 55;
    WB(0x18c) = 0;
    WB(0x546c) = 1;
    WH(STEP) = 0;
    WF(0x40) = 40.0f;
    WF(0x48) = 48.0f;
    WF(0x50) = 50.0f;
    WF(0x58) = 58.0f;
    WF(0x1b8) = 184.0f;
    WF(0x1c0) = 192.0f;
    data_0034f6f0_campsystem = 0x77;
    ledar_answer = ido_answer = 1;
    ivent_clears_bgm = 0;
    watch = 0;
}

static void run(void)
{
    want_work = dotr_camp_system_work;
    want_board = board;
    want_word = data_0034f6f0_campsystem;
    seen[0] = 0;
    watched = -99;
    Taku_Camp_System();
}
/* The pre-checks of a tick from 100 to 199. */
static void low_tick(void)
{
    XB(0x537a) = 0;
    want_word = 0;
}
static int left(const char *text)
{
    return strcmp(seen, text) == 0
        && memcmp(&dotr_camp_system_work, &want_work, sizeof want_work) == 0
        && memcmp(&board, &want_board, sizeof board) == 0
        && data_0034f6f0_campsystem == want_word;
}
static void floats(void)
{
    XF(0x40) = 184.0f;
    XF(0x48) = 192.0f;
}
static void five_bytes(void)
{
    XH(0x60) = 0;
    XB(0x5372) = 1;
    XB(0x5373) = 0;
    XB(0x537b) = 0;
    XB(0x53a0) = 0;
    XB(0x5375) = 0;
}

/* The work-area byte at 0x18c set: only the focus-move helpers run. */
static void check_all_done(void)
{
    static const struct {
        int ticks;
        const char *calls;
    } rows[] = {
        { 0, "kettei(%d,0,0) " }, { 150, "kettei(%d,0,0) " }, { 199, "kettei(%d,0,0) " }, { -5, "kettei(%d,0,0) " },
        { 200, "katimake(%d,7) " }, { 299, "katimake(%d,7) " }, { 300, "katimake(%d,7) " }, { 399, "katimake(%d,7) " },
        { 400, "" }, { 104, "kettei(%d,0,0) " }, { 10000, "" }, { 32767, "" },
    };
    int side, gate;
    size_t i;
    char text[64], more[80];

    for (side = 0; side < 2; side++) {
        for (gate = 0; gate < 2; gate++) {
            for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
                reset(side, rows[i].ticks);
                WB(0x18c) = 1;
                WB(0x546c) = (signed char)gate;
                run();
                if (rows[i].ticks < 200) {
                    XB(0x537a) = 0;
                    XB(0x537b) = 0;
                }
                XB(0x53b4) = 0;
                XB(0x53b5) = 0;
                XH(0x53b6) = 0;
                XH(0x53b8) = 0;
                XB(0x546c) = 1;
                want_word = 0;
                snprintf(text, sizeof text, rows[i].calls, side);
                /* BGM 4 once, also for tick 104 and for ticks from 10000; no event call. */
                snprintf(more, sizeof more, "%s%s", text, gate ? "" : "bgm(4,1) ");
                assert(left(more));
            }
        }
    }
    /* Any non-zero byte counts. */
    reset(0, 250);
    WB(0x18c) = -1;
    board.b198 = -3;
    run();
    XB(0x53b4) = 0;
    XB(0x53b5) = 0;
    XH(0x53b6) = 0;
    XH(0x53b8) = 0;
    want_word = 0;
    assert(left("katimake(0,-3) "));
}

/* The checks before the event call, on ticks no case handles. */
static void check_gates(void)
{
    static const int quiet[] = { 1, 99, 150, 199, 200, 214, 300, 314, 9999 };
    int side;
    size_t i;

    for (side = 0; side < 2; side++) {
        for (i = 0; i < sizeof quiet / sizeof quiet[0]; i++) {
            int t = quiet[i];

            reset(side, t);
            run();
            if (t >= 100 && t < 200) {
                low_tick();
            }
            assert(left("ivent "));
            /* BGM 4 when it was not requested yet. */
            reset(side, t);
            WB(0x546c) = 0;
            run();
            if (t >= 100 && t < 200) {
                low_tick();
            }
            XB(0x546c) = 1;
            assert(left("bgm(4,1) ivent "));
        }
        /* Not for ticks from 10000; a negative tick is below 10000. */
        reset(side, 10000);
        WB(0x546c) = 0;
        run();
        assert(left("ivent "));
        reset(side, 32767);
        WB(0x546c) = 0;
        run();
        assert(left("ivent "));
        reset(side, -1);
        WB(0x546c) = 0;
        run();
        XB(0x546c) = 1;
        assert(left("bgm(4,1) ivent "));
    }
    /* Tick 104 never asks for it (side 0 has the plain flow case there). */
    reset(0, 104);
    WB(0x546c) = 0;
    run();
    low_tick();
    assert(left("ivent flow(0,3,1:5,8,11,14) "));
}

static void check_side0_simple(void)
{
    static const struct {
        int ticks;
        const char *calls;
        int b201;
    } rows[] = {
        { 0, "ivent kettei(0,0,0) ", -1 },
        { 102, "ivent flow(0,1,0:3,6) ", -1 },
        { 103, "ivent flow(0,2,1:4,7,10) ", -1 },
        { 104, "ivent flow(0,3,1:5,8,11,14) ", -1 },
        { 105, "ivent flow(0,4,1:6) ", -1 },
        { 106, "ivent flow(0,5,1:7,10) ", -1 },
        { 107, "ivent flow(0,6,1:8,11,14) ", -1 },
        { 108, "ivent flow(0,7,0:9,12,15,2) ", -1 },
        { 109, "ivent flow(0,8,1:10) ", -1 },
        { 110, "ivent ido1(0,9) kettei(0,9,0) ", -1 },
        { 111, "ivent ido1(0,10) kettei(0,10,0) ", -1 },
        { 112, "ivent ido1(0,11) kettei(0,11,0) ", -1 },
        { 113, "ivent ido1(0,11) kettei(0,11,0) ", -1 },
        { 114, "ivent ", -1 }, { 201, "ivent ", -1 }, { 202, "ivent ", -1 },
        { 203, "ivent katimake(0,2) ", -1 }, { 204, "ivent katimake(0,3) ", -1 },
        { 205, "ivent katimake(0,4) ", -1 }, { 206, "ivent katimake(0,5) ", -1 },
        { 207, "ivent katimake(0,6) ", -1 }, { 208, "ivent katimake(0,7) ", -1 },
        { 209, "ivent katimake(0,8) ", -1 }, { 210, "ivent katimake(0,9) ", -1 },
        { 211, "ivent katimake(0,10) ", -1 },
        { 212, "ivent katimake(0,11) ", 0 }, { 213, "ivent katimake(0,11) ", 1 },
        { 301, "ivent ", -1 }, { 302, "ivent ", -1 },
        { 303, "ivent katimake(0,2) ", -1 }, { 304, "ivent katimake(0,3) ", -1 },
        { 305, "ivent katimake(0,4) ", -1 }, { 306, "ivent katimake(0,5) ", -1 },
        { 307, "ivent katimake(0,6) ", -1 }, { 308, "ivent katimake(0,7) ", -1 },
        { 309, "ivent katimake(0,8) ", -1 }, { 310, "ivent katimake(0,9) ", -1 },
        { 311, "ivent katimake(0,10) ", -1 },
        { 312, "ivent katimake(0,11) ", 0 }, { 313, "ivent katimake(0,11) ", 1 },
        { 10000, "ivent ", -1 }, { 11020, "ivent ", -1 },
    };
    size_t i;

    for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
        reset(0, rows[i].ticks);
        run();
        if (rows[i].ticks == 0) {
            XB(0x537a) = 0;
            want_word = 0;
        }
        if (rows[i].ticks >= 100 && rows[i].ticks < 200) {
            low_tick();
        }
        if (rows[i].b201 >= 0) {
            want_board.b201 = (signed char)rows[i].b201;
        }
        assert(left(rows[i].calls));
    }
}

/* Side 0, tick 100: the opening, five steps; the location byte 11 is 1 while it runs. */
static void check_side0_100(void)
{
    reset(0, 100);
    watch = 11;
    board.lv[11] = 9;
    run();
    low_tick();
    XH(0x1c) = 1;
    XH(0x1a) = 2;
    XH(STEP) = 1;
    want_board.b199 = 11;
    want_board.lv[11] = 0;
    assert(left("ivent fune(0,0) root(0,0,0:13) ") && watched == 1);

    /* Step 1 needs the leader model, the focus move and the halfword at 0x1c == 2. */
    reset(0, 100);
    board.lv[11] = 9;
    WH(STEP) = 1;
    WH(0x1c) = 2;
    run();
    low_tick();
    XH(STEP) = 2;
    want_board.lv[11] = 0;
    assert(left("ivent ledar(0) ido fune(1,0) "));
    reset(0, 100);
    WH(STEP) = 1;
    WH(0x1c) = 2;
    ledar_answer = 0;
    run();
    low_tick();
    assert(left("ivent ledar(0) "));
    reset(0, 100);
    WH(STEP) = 1;
    WH(0x1c) = 2;
    ido_answer = 0;
    run();
    low_tick();
    assert(left("ivent ledar(0) ido "));
    reset(0, 100);
    WH(STEP) = 1;
    WH(0x1c) = 1;
    run();
    low_tick();
    assert(left("ivent ledar(0) ido "));

    /* Step 2 copies the two floats and waits for the halfword at 0x1a4 == 3. */
    reset(0, 100);
    WH(STEP) = 2;
    WH(0x1a4) = 2;
    run();
    low_tick();
    floats();
    assert(left("ivent "));
    reset(0, 100);
    WH(STEP) = 2;
    WH(0x1a4) = 3;
    WB(0x537b) = 6;
    run();
    low_tick();
    floats();
    XH(STEP) = 3;
    XB(0x537b) = 7;
    XB(0x537d) = 1;
    assert(left("ivent bgm(27,0) "));

    /* Step 3 waits for location byte 0, then for the leader model. */
    reset(0, 100);
    WH(STEP) = 3;
    run();
    low_tick();
    floats();
    assert(left("ivent "));
    reset(0, 100);
    WH(STEP) = 3;
    board.lv[0] = 1;
    ledar_answer = 0;
    run();
    low_tick();
    floats();
    assert(left("ivent ledar(1) "));
    reset(0, 100);
    WH(STEP) = 3;
    board.lv[0] = -1;
    run();
    low_tick();
    floats();
    XH(0x1c) = 1;
    XH(0x1a) = 3;
    XH(STEP) = 4;
    assert(left("ivent ledar(1) "));

    /* Step 4: the byte at 0x537b goes 4 -> 5; at 12 with 0x1c == 2 the tick becomes 11020. */
    reset(0, 100);
    WH(STEP) = 4;
    WB(0x537b) = 4;
    WH(0x1c) = 2;
    run();
    low_tick();
    XB(0x537b) = 5;
    assert(left("ivent "));
    reset(0, 100);
    WH(STEP) = 4;
    WB(0x537b) = 12;
    WH(0x1c) = 1;
    run();
    low_tick();
    assert(left("ivent "));
    reset(0, 100);
    WH(STEP) = 4;
    WB(0x537b) = 12;
    WH(0x1c) = 2;
    board.lv[11] = 5;
    run();
    low_tick();
    XB(0x537b) = 0;
    XB(0x537d) = 0;
    XH(STEP) = 0;
    want_board.h194 = 11020;
    want_board.lv[11] = 0;
    assert(left("ivent "));

    /* A step past the last one: only the location byte is cleared. */
    reset(0, 100);
    WH(STEP) = 5;
    board.lv[11] = 5;
    run();
    low_tick();
    want_board.lv[11] = 0;
    assert(left("ivent "));
    reset(0, 100);
    WH(STEP) = -1;
    run();
    low_tick();
    assert(left("ivent "));
}

/* Side 0, tick 101: four steps; the location byte 0 is set to 1 on every call. */
static void check_side0_101(void)
{
    reset(0, 101);
    run();
    low_tick();
    XF(0x40) = 1000.0f;
    XF(0x48) = 2000.0f;
    XH(0x1c) = 1;
    XH(0x1a) = 2;
    XH(STEP) = 1;
    want_board.lv[0] = 0;
    /* Location 0 was 1 at both route calls. */
    assert(left("ivent fune(0,1) root(0,0,3:0) root(0,1,0:1,2) ") && watched == 1);

    reset(0, 101);
    WH(STEP) = 1;
    WH(0x1c) = 2;
    run();
    low_tick();
    XH(STEP) = 2;
    want_board.lv[0] = 1;
    assert(left("ivent ledar(0) ido fune(1,1) "));
    reset(0, 101);
    WH(STEP) = 1;
    WH(0x1c) = 3;
    run();
    low_tick();
    want_board.lv[0] = 1;
    assert(left("ivent ledar(0) ido "));
    reset(0, 101);
    WH(STEP) = 1;
    WH(0x1c) = 2;
    ledar_answer = 0;
    run();
    low_tick();
    want_board.lv[0] = 1;
    assert(left("ivent ledar(0) "));

    reset(0, 101);
    WH(STEP) = 2;
    WH(0x1a4) = 3;
    run();
    low_tick();
    floats();
    XH(STEP) = 3;
    want_board.lv[0] = 1;
    assert(left("ivent "));
    reset(0, 101);
    WH(STEP) = 2;
    WH(0x1a4) = 4;
    run();
    low_tick();
    floats();
    want_board.lv[0] = 1;
    assert(left("ivent "));

    reset(0, 101);
    WH(STEP) = 3;
    run();
    low_tick();
    floats();
    want_board.lv[0] = 1;
    assert(left("ivent "));
    reset(0, 101);
    WH(STEP) = 3;
    board.lv[1] = 2;
    ledar_answer = 0;
    run();
    low_tick();
    floats();
    want_board.lv[0] = 1;
    assert(left("ivent ledar(1) "));
    reset(0, 101);
    WH(STEP) = 3;
    board.lv[1] = 2;
    run();
    low_tick();
    floats();
    XH(0x1c) = 1;
    XH(0x1a) = 0;
    XH(STEP) = 0;
    want_board.h194 = 102;
    want_board.lv[0] = 1;
    assert(left("ivent ledar(1) "));

    reset(0, 101);
    WH(STEP) = 4;
    run();
    low_tick();
    want_board.lv[0] = 1;
    assert(left("ivent "));
}

static void check_side1_simple(void)
{
    static const struct {
        int ticks;
        const char *calls;
    } rows[] = {
        { 0, "ivent kettei(1,0,0) " },
        { 100, "ivent flow(1,11,0:14) " },
        { 101, "ivent flow(1,12,0:15) " }, { 102, "ivent flow(1,13,0:0) " }, { 103, "ivent flow(1,14,0:1) " },
        { 106, "ivent flow(1,17,0:4) " },
        { 107, "ivent flow(1,18,0:5) " }, { 108, "ivent flow(1,19,0:6) " },
        { 109, "ivent flow(1,20,0:7) " }, { 110, "ivent flow(1,21,0:8) " },
        { 112, "ivent ido1(1,11) kettei(1,11,0) " }, { 113, "ivent " },
        { 200, "ivent " },
        { 201, "ivent katimake(1,12) " }, { 202, "ivent katimake(1,13) " }, { 203, "ivent katimake(1,14) " },
        { 204, "ivent " }, { 205, "ivent katimake(1,16) " }, { 206, "ivent " },
        { 207, "ivent katimake(1,18) " }, { 208, "ivent katimake(1,19) " }, { 209, "ivent katimake(1,20) " },
        { 210, "ivent katimake(1,21) " }, { 211, "ivent katimake(1,0) " },
        { 212, "ivent katimake(1,11) " }, { 213, "ivent katimake(1,11) " },
        { 301, "ivent katimake(1,12) " }, { 302, "ivent katimake(1,13) " }, { 303, "ivent katimake(1,14) " },
        { 304, "ivent " }, { 305, "ivent katimake(1,16) " }, { 306, "ivent " },
        { 307, "ivent katimake(1,18) " }, { 308, "ivent katimake(1,19) " }, { 309, "ivent katimake(1,20) " },
        { 310, "ivent katimake(1,21) " }, { 311, "ivent katimake(1,0) " },
        { 312, "ivent katimake(1,11) " }, { 313, "ivent katimake(1,11) " },
        { 12130, "ivent " },
    };
    size_t i;

    for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
        reset(1, rows[i].ticks);
        board.lv[11] = 4;
        run();
        if (rows[i].ticks == 0) {
            XB(0x537a) = 0;
            want_word = 0;
        }
        if (rows[i].ticks >= 100 && rows[i].ticks < 200) {
            low_tick();
        }
        if (rows[i].ticks == 100) {
            want_board.lv[11] = 0;
        }
        /* The byte at +201 is not written for this side. */
        assert(left(rows[i].calls));
    }

    /* Tick 106 asks for BGM 4 itself when the event call left it unrequested. */
    reset(1, 106);
    ivent_clears_bgm = 1;
    run();
    low_tick();
    XB(0x546c) = 1;
    assert(left("ivent flow(1,17,0:4) bgm(4,1) "));
    reset(1, 106);
    WB(0x546c) = 0;
    ivent_clears_bgm = 1;
    run();
    low_tick();
    XB(0x546c) = 1;
    assert(left("bgm(4,1) ivent flow(1,17,0:4) bgm(4,1) "));
}

/* Side 1, tick 104: four steps around location 15. */
static void check_side1_104(void)
{
    reset(1, 104);
    WB(0x546c) = 0;
    watch = 15;
    run();
    low_tick();
    XH(0x1c) = 1;
    XH(0x1a) = 2;
    five_bytes();
    XH(STEP) = 1;
    want_board.lv[15] = 3;
    /* No BGM request on tick 104. */
    assert(left("ivent fune(0,2) root(1,15,3:2) ") && watched == 3);

    reset(1, 104);
    WH(STEP) = 1;
    WH(0x1c) = 2;
    run();
    low_tick();
    XH(STEP) = 2;
    want_board.lv[15] = 3;
    assert(left("ivent ledar(0) ido fune(1,2) "));
    reset(1, 104);
    WH(STEP) = 1;
    WH(0x1c) = 2;
    ido_answer = 0;
    run();
    low_tick();
    want_board.lv[15] = 3;
    assert(left("ivent ledar(0) ido "));
    reset(1, 104);
    WH(STEP) = 1;
    WH(0x1c) = 0;
    run();
    low_tick();
    want_board.lv[15] = 3;
    assert(left("ivent ledar(0) ido "));

    reset(1, 104);
    WH(STEP) = 2;
    WH(0x1a4) = 3;
    run();
    low_tick();
    floats();
    XH(STEP) = 0;
    want_board.h194 = 12041;
    want_board.b199 = 16;
    want_board.lv[15] = 3;
    assert(left("ivent "));
    reset(1, 104);
    WH(STEP) = 2;
    WH(0x1a4) = 0;
    run();
    low_tick();
    floats();
    want_board.lv[15] = 3;
    assert(left("ivent "));

    /* Step 3: location 15's first route r = list[1][0] (2 here) picks list[1][r] (8) as the
     * location byte to wait for, and route record [1][r] gets its flag. */
    reset(1, 104);
    WH(STEP) = 3;
    assert(spot[15].list[1][0] == 2 && spot[15].list[1][2] == 8);
    run();
    low_tick();
    floats();
    want_board.lv[15] = 3;
    assert(left("ivent "));
    reset(1, 104);
    WH(STEP) = 3;
    board.lv[8] = 1;
    ledar_answer = 0;
    run();
    low_tick();
    floats();
    want_board.lv[15] = 3;
    assert(left("ivent ledar(1) "));
    reset(1, 104);
    WH(STEP) = 3;
    board.lv[8] = 1;
    run();
    low_tick();
    floats();
    XH(0x1c) = 1;
    XH(0x1a) = 0;
    want_work.bank.records[1][2].flag = 1;
    XH(STEP) = 4;
    want_board.lv[15] = 3;
    assert(left("ivent ledar(1) "));
    assert(XH(0x22c + (16 + 2) * 516 + 0x202) == 1);

    reset(1, 104);
    WH(STEP) = 4;
    run();
    low_tick();
    want_board.lv[15] = 3;
    assert(left("ivent "));
}

/* Side 1, tick 105: five steps around locations 16 and 17. */
static void check_side1_105(void)
{
    reset(1, 105);
    watch = 16;
    run();
    low_tick();
    XH(0x1c) = 0;
    XH(0x1a) = 2;
    five_bytes();
    XH(STEP) = 1;
    want_board.b199 = 16;
    want_board.lv[16] = 3;
    assert(left("ivent fune(0,3) root(1,16,3:3,4) root(1,17,0:3,4) bgm(35,1) ") && watched == 3);

    reset(1, 105);
    WH(STEP) = 1;
    WH(0x6c) = 0;
    run();
    low_tick();
    XH(0x1c) = 1;
    XH(STEP) = 2;
    want_board.lv[16] = 3;
    assert(left("ivent "));
    reset(1, 105);
    WH(STEP) = 1;
    WH(0x6c) = 1;
    run();
    low_tick();
    want_board.lv[16] = 3;
    assert(left("ivent "));

    reset(1, 105);
    WH(STEP) = 2;
    WH(0x1c) = 2;
    run();
    low_tick();
    XH(STEP) = 3;
    want_board.lv[16] = 3;
    assert(left("ivent ledar(0) ido fune(1,3) "));
    reset(1, 105);
    WH(STEP) = 2;
    WH(0x1c) = 2;
    ledar_answer = 0;
    run();
    low_tick();
    want_board.lv[16] = 3;
    assert(left("ivent ledar(0) "));
    reset(1, 105);
    WH(STEP) = 2;
    WH(0x1c) = 1;
    run();
    low_tick();
    want_board.lv[16] = 3;
    assert(left("ivent ledar(0) ido "));

    reset(1, 105);
    WH(STEP) = 3;
    WH(0x1a4) = 3;
    WB(0x537b) = 2;
    run();
    low_tick();
    floats();
    XH(STEP) = 4;
    XB(0x537b) = 3;
    XB(0x537d) = 1;
    want_board.b199 = 17;
    want_board.lv[16] = 3;
    assert(left("ivent "));
    reset(1, 105);
    WH(STEP) = 3;
    WH(0x1a4) = 1;
    run();
    low_tick();
    floats();
    want_board.lv[16] = 3;
    assert(left("ivent "));

    /* Step 4: location 16's first route (3) picks the list at 0x299020 [1][3], whose cell
     * names the location byte to wait for. */
    reset(1, 105);
    WH(STEP) = 4;
    assert(spot[16].list[1][0] == 3);
    data_00299020_campsystem[1][3].cell = 20;
    run();
    low_tick();
    floats();
    want_board.lv[16] = 3;
    assert(left("ivent "));
    reset(1, 105);
    WH(STEP) = 4;
    data_00299020_campsystem[1][3].cell = 20;
    board.lv[20] = 1;
    ledar_answer = 0;
    run();
    low_tick();
    floats();
    want_board.lv[16] = 3;
    assert(left("ivent ledar(1) "));
    reset(1, 105);
    WH(STEP) = 4;
    data_00299020_campsystem[1][3].cell = 20;
    board.lv[20] = 1;
    run();
    low_tick();
    floats();
    XH(0x1c) = 1;
    XH(0x1a) = 0;
    want_work.bank.records[1][3].flag = 1;
    XH(STEP) = 0;
    want_board.h194 = 106;
    want_board.lv[16] = 3;
    assert(left("ivent ledar(1) "));
    /* Another cell: the wait goes on. */
    reset(1, 105);
    WH(STEP) = 4;
    data_00299020_campsystem[1][3].cell = 21;
    board.lv[20] = 1;
    run();
    low_tick();
    floats();
    want_board.lv[16] = 3;
    assert(left("ivent "));

    reset(1, 105);
    WH(STEP) = 5;
    run();
    low_tick();
    want_board.lv[16] = 3;
    assert(left("ivent "));
}

/* One call of side 1, tick 111 at a step. */
static void at_111(int step)
{
    reset(1, 111);
    WH(STEP) = (short)step;
}

/* Side 1, tick 111: ten steps (the jump table). */
static void check_side1_111(void)
{
    int t;

    at_111(0);
    run();
    low_tick();
    XH(0x1c) = 0;
    XH(0x1a) = 2;
    five_bytes();
    XH(STEP) = 1;
    want_board.lv[0] = 3;
    assert(left("ivent fune(0,4) root(1,0,3:3) "));

    at_111(1);
    WH(0x6c) = 0;
    run();
    low_tick();
    XH(0x1c) = 1;
    XH(STEP) = 2;
    assert(left("ivent "));
    at_111(1);
    WH(0x6c) = -1;
    run();
    low_tick();
    assert(left("ivent "));

    at_111(2);
    WH(0x1c) = 2;
    WB(0x537b) = 13;
    run();
    low_tick();
    XB(0x537b) = 14;
    XB(0x537d) = 1;
    XB(0x5375) = 1;
    XH(STEP) = 3;
    assert(left("ivent "));
    at_111(2);
    WH(0x1c) = 1;
    run();
    low_tick();
    assert(left("ivent "));

    at_111(3);
    WB(0x537b) = 14;
    run();
    low_tick();
    XB(0x537b) = 0;
    XB(0x537d) = 0;
    XH(STEP) = 4;
    assert(left("ivent "));
    at_111(3);
    WB(0x537b) = 13;
    run();
    low_tick();
    assert(left("ivent "));

    /* Step 4 does not look at the halfword at 0x1c. */
    at_111(4);
    WH(0x1c) = 0;
    run();
    low_tick();
    XH(STEP) = 5;
    assert(left("ivent ledar(0) ido fune(1,4) "));
    at_111(4);
    ledar_answer = 0;
    run();
    low_tick();
    assert(left("ivent ledar(0) "));
    at_111(4);
    ido_answer = 0;
    run();
    low_tick();
    assert(left("ivent ledar(0) ido "));

    at_111(5);
    WH(0x1a4) = 3;
    run();
    low_tick();
    floats();
    want_board.b199 = 11;
    XH(STEP) = 6;
    assert(left("ivent "));
    at_111(5);
    WH(0x1a4) = 2;
    run();
    low_tick();
    floats();
    assert(left("ivent "));

    at_111(6);
    run();
    low_tick();
    floats();
    assert(left("ivent "));
    at_111(6);
    board.lv[11] = 1;
    ledar_answer = 0;
    run();
    low_tick();
    floats();
    assert(left("ivent ledar(1) "));
    at_111(6);
    board.lv[11] = 1;
    run();
    low_tick();
    floats();
    XH(0x1c) = 1;
    XH(0x1a) = 0;
    XH(STEP) = 7;
    assert(left("ivent ledar(1) "));

    /* Step 7: the focus position goes from where it was to location 11 in sixteenths. The
     * first call (counter 0) latches the start. */
    at_111(7);
    WH(0x60) = 0;
    WF(0x40) = 512.0f;
    WF(0x48) = 768.0f;
    spot[11].x = 256.0f;
    spot[11].y = 1024.0f;
    run();
    low_tick();
    XF(0x50) = 512.0f;
    XF(0x58) = 768.0f;
    XH(0x60) = 16;
    assert(left("ivent "));
    for (t = 16; t <= 240; t += 16) {
        at_111(7);
        WH(0x60) = (short)t;
        WF(0x50) = 512.0f;
        WF(0x58) = 768.0f;
        spot[11].x = 256.0f;
        spot[11].y = 1024.0f;
        run();
        low_tick();
        XF(0x40) = 512.0f - (float)t;
        XF(0x48) = 768.0f + (float)t;
        XH(0x60) = (short)(t + 16);
        /* 240 + 16 = 256 does not end it. */
        assert(left("ivent "));
    }
    /* Counter 256: exactly the location; 272 is past 256 and the step ends. */
    at_111(7);
    WH(0x60) = 256;
    WF(0x50) = 512.0f;
    WF(0x58) = 768.0f;
    spot[11].x = 256.0f;
    spot[11].y = 1024.0f;
    run();
    low_tick();
    XF(0x40) = 256.0f;
    XF(0x48) = 1024.0f;
    XF(0x50) = 256.0f;
    XF(0x58) = 1024.0f;
    XH(0x60) = 0;
    XB(0x5372) = 0;
    XB(0x5375) = 0;
    XH(STEP) = 8;
    assert(left("ivent "));

    at_111(8);
    run();
    low_tick();
    XH(0x1c) = 1;
    XH(0x1a) = 3;
    XH(STEP) = 9;
    assert(left("ivent "));

    at_111(9);
    WH(0x1c) = 2;
    run();
    low_tick();
    XH(STEP) = 0;
    want_board.h194 = 12130;
    assert(left("ivent "));
    at_111(9);
    WH(0x1c) = 1;
    run();
    low_tick();
    assert(left("ivent "));

    /* Outside the table. */
    at_111(10);
    run();
    low_tick();
    assert(left("ivent "));
    at_111(-1);
    run();
    low_tick();
    assert(left("ivent "));
}

int main(void)
{
    assert(sizeof(struct DotrCampSystemSpot) == 52 && offsetof(struct DotrCampSystemSpot, list) == 20);
    assert(offsetof(struct DotrCampSystemSpot, n) == 36 && offsetof(struct DotrCampSystemSpot, y) == 8);
    assert(offsetof(struct DotrCampSystemBoard, lv) == 3 && offsetof(struct DotrCampSystemBoard, h194) == 194);
    assert(offsetof(struct DotrCampSystemBoard, b198) == 198 && offsetof(struct DotrCampSystemBoard, b201) == 201);
    assert(sizeof(struct DotrCampSystemRecord) == 516 && offsetof(struct DotrCampSystemRecord, flag) == 0x202);
    assert(offsetof(struct DotrCampSystemBank, records) == 0x22c);
    assert(sizeof(struct DotrCampSystemList) == 76 && offsetof(struct DotrCampSystemList, cell) == 70);
    check_all_done();
    check_gates();
    check_side0_simple();
    check_side0_100();
    check_side0_101();
    check_side1_simple();
    check_side1_104();
    check_side1_105();
    check_side1_111();
    puts("camp_system: ok");
    return 0;
}

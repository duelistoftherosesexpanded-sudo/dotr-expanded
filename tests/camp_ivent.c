/* Synthetic host tests for the camp_ivent group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_ivent.h"

union DotrCampIventBoard dotr_camp_ivent_board;
union DotrCampIventWork dotr_camp_ivent_work;
short data_00298c32_campivent;
short data_00298cd0_campivent;
float data_00298d0c_campivent;
float data_00298d14_campivent;
short data_00298d6a_campivent;
float data_00298ddc_campivent;
float data_00298de4_campivent;
signed char Move_END_flg_campivent;

/* The state a call is expected to leave: copied from the fixtures before the call, then changed
 * by the test exactly as the function should have changed the real ones. */
static union DotrCampIventBoard want_board;
static union DotrCampIventWork want_work;
static signed char want_flag;

#define WB(o) (*(signed char *)(dotr_camp_ivent_work.bytes + (o)))
#define WH(o) (*(short *)(dotr_camp_ivent_work.bytes + (o)))
#define WF(o) (*(float *)(dotr_camp_ivent_work.bytes + (o)))
#define WI(o) (*(int *)(dotr_camp_ivent_work.bytes + (o)))
#define XB(o) (*(signed char *)(want_work.bytes + (o)))
#define XH(o) (*(short *)(want_work.bytes + (o)))
#define XF(o) (*(float *)(want_work.bytes + (o)))
#define XI(o) (*(int *)(want_work.bytes + (o)))
#define EVENT (*(short *)(dotr_camp_ivent_board.bytes + 0xc2))
#define XEVENT (*(short *)(want_board.bytes + 0xc2))
#define STEP 0x536c

/* What was called, one word per call, in order. */
static char seen[512];
static int inq_busy, memcard_result, ledar_result;

static void said(const char *format, ...)
{
    va_list list;
    size_t used = strlen(seen);

    va_start(list, format);
    vsnprintf(seen + used, sizeof seen - used, format, list);
    va_end(list);
    assert(strlen(seen) + 1 < sizeof seen);
}

int func_00217980_campivent(int n, int b)
{
    said("bgm(%d,%d) ", n, b);
    return 0;
}
int func_001f93f0_campivent(int index, int position, int part, void *in)
{
    said("light(%d,%d,%d,%d) ", index, position, part, in != NULL);
    return 0;
}
int SzEffect_Entry(int a, int b, int c, int d, int e)
{
    said("effect(%d,%#x,%d,%d,%d) ", a, b, c, d, e);
    return 0;
}
void Taku_Camp_SINEMA_Init(signed char mode) { said("sinema(%d) ", mode); }
int SzSound_PlaySE(int index, int value)
{
    said("se(%d,%#x) ", index, value);
    return 0;
}
int SzModel_LoadMerge(int type, int no, int force)
{
    said("merge(%d,%d,%d) ", type, no, force);
    return 0;
}
void SzEffect_SetIntensityOnly(int kind, float value) { said("intensity(%d,%g) ", kind, value); }
int SzFile_InqRequestEx(void)
{
    said("inq ");
    return inq_busy;
}
int SzData_SetMapEditFlag(int side, int flag)
{
    said("mapedit(%d,%d) ", side, flag);
    return 0;
}
void SzGame_InitMemCardProc(int kind) { said("cardinit(%d) ", kind); }
int SzGame_ActMemCardProc(void)
{
    said("card ");
    return memcard_result;
}
void SzGame_PlayMovieNo(int movie, int a, int b) { said("movie(%d,%d,%d) ", movie, a, b); }
int SzGame_SetMenuNum(int index, int value)
{
    said("menu(%d,%d) ", index, value);
    return 0;
}
void SzGame_Pop(void) { said("pop "); }
int My_LedarModel_a_set(int dir)
{
    said("ledar(%d) ", dir);
    return ledar_result;
}

/* Clear everything, then put the event and its step in place. */
static void reset(int event, int step)
{
    memset(&dotr_camp_ivent_board, 0, sizeof dotr_camp_ivent_board);
    memset(&dotr_camp_ivent_work, 0, sizeof dotr_camp_ivent_work);
    Move_END_flg_campivent = 0;
    inq_busy = 0;
    memcard_result = 0;
    ledar_result = 0;
    EVENT = (short)event;
    WH(STEP) = (short)step;
}

/* Snapshot the fixtures as the expected state; the caller then edits want_* and calls run(). */
static void expect(void)
{
    want_board = dotr_camp_ivent_board;
    want_work = dotr_camp_ivent_work;
    want_flag = Move_END_flg_campivent;
    seen[0] = 0;
}

static void run(const char *calls)
{
    Taku_Camp_Ivent();
    if (strcmp(seen, calls) != 0) {
        fprintf(stderr, "calls: got \"%s\" want \"%s\"\n", seen, calls);
    }
    assert(strcmp(seen, calls) == 0);
    assert(memcmp(&dotr_camp_ivent_board, &want_board, sizeof want_board) == 0);
    assert(memcmp(&dotr_camp_ivent_work, &want_work, sizeof want_work) == 0);
    assert(Move_END_flg_campivent == want_flag);
}

/* Event 0x2724, the opening: set-up, the waits on the fade and the camera, the hand-over. */
static void test_opening(void)
{
    reset(0x2724, 0);
    WB(0x546c) = 5;
    expect();
    XF(0x40) = 0;
    XF(0x48) = 0;
    XI(0x4c) = 0;
    XH(0x1c) = 1;
    XH(0x1a) = 2;
    XB(0x53b3) = 1;
    XB(0x62) = 0;
    XH(0x64) = 0x1000;
    XH(0x53a2) = 1;
    XH(0x53a4) = 0x1000;
    XB(0x546c) = 0;
    XH(STEP) = 1;
    run("effect(0,0x190,0,0,0) bgm(24,1) ");

    /* Step 1 waits for the halfword at 0x1ed001c to reach 2. */
    reset(0x2724, 1);
    WH(0x1c) = 1;
    expect();
    run("");
    WH(0x1c) = 2;
    expect();
    XB(0x537b) = 1;
    XB(0x537d) = 1;
    XH(STEP) = 2;
    run("");

    /* Step 2 waits for the byte at 0x1ed537b to reach 12. */
    reset(0x2724, 2);
    WB(0x537b) = 11;
    expect();
    run("");
    WB(0x537b) = 12;
    WB(0x537d) = 3;
    expect();
    XB(0x537b) = 0;
    XB(0x537d) = 0;
    XH(0x53a2) = 2;
    XH(STEP) = 3;
    run("");

    /* Step 5: below 2000 the BGM is requested once; below 0x50 the camera goes to 10. */
    reset(0x2724, 5);
    WH(0x64) = 0x7d0;
    expect();
    run("");
    WH(0x64) = 0x7cf;
    expect();
    XB(0x546c) = 1;
    run("bgm(25,0) ");
    expect();
    run("");
    WH(0x64) = 0x50;
    expect();
    run("");
    WH(0x64) = 0x4f;
    expect();
    XH(0x53a2) = 10;
    XH(STEP) = 6;
    run("");

    /* Step 6 hands over to event 0x2738 when the camera reports 11. */
    reset(0x2724, 6);
    WH(0x53a2) = 10;
    expect();
    run("");
    WH(0x53a2) = 11;
    expect();
    XEVENT = 0x2738;
    XH(STEP) = 0;
    run("");

    /* The next frame runs event 0x2738's set-up. */
    WB(0x5437) = 9;
    expect();
    XF(0x40) = 0;
    XF(0x48) = 0;
    XB(0x53b3) = 2;
    XH(0x64) = 0x1000;
    XH(0x53a2) = 0x14;
    XH(0x53a4) = 0x1000;
    XB(0x5437) = 0;
    XH(STEP) = 1;
    run("effect(0,0x191,0,0,0) sinema(2) ");

    /* Event 0x2738 step 1: two sound effects once the camera reports 0x15. */
    reset(0x2738, 1);
    WH(0x53a2) = 0x15;
    expect();
    XB(0x62) = 2;
    XH(STEP) = 2;
    run("se(0,0x22) se(0,0x23) ");
}

/* Event 0x276a step 3 picks the next event by the byte at 0x1ed53a8; then event 0x2af8. */
static void test_choice(void)
{
    reset(0x276a, 3);
    WB(0x53a6) = 3;
    expect();
    run("");

    WB(0x53a6) = 4;
    WH(STEP) = 3;
    expect();
    XB(0x53a6) = 0;
    want_board.bytes[0] = 1;
    want_board.bytes[0xc9] = 2;
    XEVENT = 0x2ee0;
    XH(STEP) = 0;
    run("");

    reset(0x276a, 3);
    WB(0x53a6) = 4;
    WB(0x53a8) = 1;
    dotr_camp_ivent_board.bytes[0] = 1;
    dotr_camp_ivent_board.bytes[0xc9] = 2;
    expect();
    XB(0x53a6) = 0;
    want_board.bytes[0] = 0;
    want_board.bytes[0xc9] = 0;
    XEVENT = 0x2af8;
    XH(STEP) = 0;
    run("");

    /* 0x2af8 step 3: cinema mode 3, the model pack and the light, once the camera says 0x17. */
    reset(0x2af8, 3);
    WH(0x53a2) = 0x16;
    expect();
    run("");
    WH(0x53a2) = 0x17;
    WB(0x537b) = 7;
    WH(0x64) = 99;
    expect();
    XH(0x53a2) = 0;
    XB(0x537b) = 0;
    XH(0x64) = 0;
    XH(STEP) = 4;
    run("sinema(3) merge(3,2,0) light(0,7,0,0) intensity(7,1) ");

    /* Step 4 waits on the file reader, then moves the focus to location 11 and starts event 100. */
    data_00298ddc_campivent = 12.5f;
    data_00298de4_campivent = -3.0f;
    WB(0x53b3) = 3;
    inq_busy = 1;
    expect();
    run("inq ");
    inq_busy = 0;
    expect();
    XB(0x53b3) = 0;
    XF(0x40) = 12.5f;
    XF(0x48) = -3.0f;
    XH(STEP) = 0;
    XEVENT = 0x64;
    run("inq bgm(4,1) ");
}

/* Event 0x2b74: the route halfwords copied from the location records, then the return to event 0. */
static void test_route_copy(void)
{
    reset(0x2b74, 0);
    data_00298d0c_campivent = 4.0f;
    data_00298d14_campivent = 8.0f;
    WH(0x52fa) = 3;
    WB(0x5483) = 1;
    expect();
    XH(0x1c) = 1;
    XH(0x1a) = 2;
    XF(0x40) = 4.0f;
    XF(0x48) = 8.0f;
    XH(0x52fa) = 0;
    XB(0x5483) = 0;
    XH(STEP) = 1;
    run("");

    reset(0x2b74, 3);
    data_00298c32_campivent = 0x111;
    data_00298cd0_campivent = 0x222;
    data_00298d6a_campivent = 0x333;
    WH(0x1c) = 1;
    expect();
    run("");
    WH(0x1c) = 2;
    expect();
    XH(0x1c5e) = 0x111;
    XH(0x1e62) = 0x222;
    XH(0x1a5a) = 0x333;
    XH(STEP) = 4;
    run("");

    reset(0x2b74, 4);
    WH(0x52fa) = 1;
    WB(0x5374) = 1;
    WB(0x5375) = 1;
    WH(0x60) = 7;
    WB(0x5372) = 1;
    WB(0x5373) = 1;
    expect();
    XEVENT = 0;
    XH(STEP) = 0;
    XB(0x5374) = 0;
    XB(0x5375) = 0;
    XH(0x60) = 0;
    XB(0x5372) = 0;
    XB(0x5373) = 0;
    run("");
}

/* Event 0x2bb0 step 3: the memory-card save; a negative result leaves for the movie. */
static void test_memcard(void)
{
    reset(0x2bb0, 3);
    WH(0x1c) = 1;
    expect();
    run("");

    WH(0x1c) = 2;
    dotr_camp_ivent_board.bytes[2] = 1;
    memcard_result = 0;
    expect();
    want_board.bytes[1] = 1;
    XB(0x543c) = 1;
    run("mapedit(0,1) cardinit(2) card ");
    /* Started already: only polled. */
    expect();
    run("card ");
    memcard_result = -1;
    expect();
    XB(0x543c) = 0;
    want_flag = 1;
    run("card movie(2,1,0) menu(10,1) pop ");

    /* The camera counter check after the step switch: above 3000 the camera state is 4, above
     * 2000 the effect 0x192 starts once. */
    reset(0x2bb0, 1);
    WH(0x64) = 0xbb9;
    expect();
    XH(0x53a2) = 4;
    XB(0x5447) = 1;
    run("effect(0,0x192,0,0,0) ");
    expect();
    run("");
    reset(0x2bb0, 1);
    WH(0x64) = 0x7d0;
    expect();
    run("");
}

/* Event 0x2f09 step 3 waits for the leader model; step 4 returns to event 0. */
static void test_leader(void)
{
    reset(0x2f09, 3);
    WH(0x530c) = 1;
    ledar_result = 1;
    expect();
    run("");
    WH(0x530c) = 0;
    ledar_result = 0;
    expect();
    run("ledar(1) ");
    ledar_result = 1;
    expect();
    XH(0x1c) = 1;
    XH(0x1a) = 0;
    XH(STEP) = 4;
    run("ledar(1) ");
}

/* An event number the function does not know clears the byte at 0x1ed53b3 only. */
static void test_unknown(void)
{
    reset(0x1234, 2);
    WB(0x53b3) = 5;
    expect();
    XB(0x53b3) = 0;
    run("");
    reset(0x2b02, 0);
    WB(0x53b3) = 5;
    expect();
    run("");
}

int main(void)
{
    test_opening();
    test_choice();
    test_route_copy();
    test_memcard();
    test_leader();
    test_unknown();
    return 0;
}

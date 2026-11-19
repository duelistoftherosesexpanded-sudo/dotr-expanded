/* Synthetic host tests for the tuto_l033 group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/tuto_l033.h"

enum {
    F_DRAW = 1, F_DISP, F_ANALOG, F_IGNORE, F_CUR, F_LP, F_ADD, F_DEL, F_MOVE, F_GOAL, F_RATE,
    F_STATUS, F_FIELD, F_POPUP, F_CLOSE, F_TURN, F_STDVIEW, F_HALFVIEW, F_FOCUS, F_FINGER,
    F_FADE, F_RESTORE, F_BKUP, F_MSG, F_FOCUS32
};

struct call {
    int fn;
    long a[5];
};

static struct call trace[128];
static int ntrace;
static int window_disp, pad_mask, fade_ret, msg_ret, standby_fail;
static struct DotrTutoL033Record rec;
/* L032's record, as bytes (the view backup area at +0x1b0, as rec.text is in L033's). */
static unsigned char rec32[0x1c0];

static void add(int fn, long a, long b, long c, long d, long e)
{
    assert(ntrace < 128);
    trace[ntrace].fn = fn;
    trace[ntrace].a[0] = a;
    trace[ntrace].a[1] = b;
    trace[ntrace].a[2] = c;
    trace[ntrace].a[3] = d;
    trace[ntrace].a[4] = e;
    ntrace++;
}

static int count(int fn)
{
    int i, n = 0;

    for (i = 0; i < ntrace; i++) {
        n += trace[i].fn == fn;
    }
    return n;
}

/* The k-th call to fn has these leading arguments. */
static void expect(int fn, int k, int n, long a, long b, long c, long d, long e)
{
    long want[5];
    int i, j;

    want[0] = a;
    want[1] = b;
    want[2] = c;
    want[3] = d;
    want[4] = e;
    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == fn && k-- == 0) {
            for (j = 0; j < n; j++) {
                assert(trace[i].a[j] == want[j]);
            }
            return;
        }
    }
    assert(0);
}

int SzDuel_GetWindowDisp(void) { return window_disp; }
void SzDuel_SetWindowDisp(int on) { add(F_DISP, on, 0, 0, 0, 0); }
void SzDuel_SetWindowDraw(int on) { add(F_DRAW, on, 0, 0, 0, 0); }
void SzPad_SetAnalogInfo(int port, int axis, int value) { add(F_ANALOG, port, axis, value, 0, 0); }
int SzPad_Check(int port, int kind, int mask)
{
    assert(port == 0 && kind == 0 && mask == 0x800);
    return pad_mask & mask;
}
void SzDuel_SetIgnoreJudge(long long on) { add(F_IGNORE, (long)on, 0, 0, 0, 0); }
int SzEffect_IsModelStandby2(int model) { return model != standby_fail; }
int SzDuel_SetPlayerCur(int side, int x, int y) { add(F_CUR, side, x, y, 0, 0); return 0; }
int SzDuel_CalcLP(int index, int operation, int value) { add(F_LP, index, operation, value, 0, 0); return 0; }
void SzDuel_AddUnit(int side, int x, int y, int card, int flag) { add(F_ADD, side, x, y, card, flag); }
int SzDuel_DelUnit(int side, int id) { add(F_DEL, side, id, 0, 0, 0); return 0; }
void *SzDuel_MoveUnit(int side, int id, int x, int y) { add(F_MOVE, side, id, x, y, 0); return NULL; }
int SzDuel_SetUnitCtlGoal(int side, int id, int goal) { add(F_GOAL, side, id, goal, 0, 0); return 0; }
int SzDuel_SetUnitCtlRate(int side, int id, int a, int b) { add(F_RATE, side, id, a, b, 0); return 0; }
int SzDuel_SetUnitStatus(int side, int id, int bit, int value) { add(F_STATUS, side, id, bit, value, 0); return 0; }
int SzDuel_SetField(int x, int y, signed char field) { add(F_FIELD, x, y, field, 0, 0); return 0; }
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value) { add(F_POPUP, operation, a, b, value, 0); }
void SzDuel_CloseHandCursor(void) { add(F_CLOSE, 0, 0, 0, 0, 0); }
int SzDuel_GetMsgName(int id) { return 0x5000 + id; }
void SzDuel_SetTurnCount(int count_) { add(F_TURN, count_, 0, 0, 0, 0); }
void TaView_ReqStdCompleteView(int a) { add(F_STDVIEW, a, 0, 0, 0, 0); }
void TaView_ReqHalfFarView(int a) { add(F_HALFVIEW, a, 0, 0, 0, 0); }
void TaTuto_FocusUnit__001745f0(int card) { add(F_FOCUS, card, 0, 0, 0, 0); }
void TaTuto_FocusUnit__00172510(int card) { add(F_FOCUS32, card, 0, 0, 0, 0); }
void func_0027f630_l032(unsigned char *r, int a, int b, int x, int y)
{
    assert(r == rec32);
    add(F_FINGER, a, b, x, y, 0);
}
int func_0027fc20_l032(int first, int last, unsigned char *r)
{
    assert(r == rec32);
    add(F_MSG, first, last, 0, 0, 0);
    return msg_ret;
}
void TaTuto_DrawFinger(struct DotrTutoL033Record *r, int a, int b, int x, int y)
{
    assert(r == &rec);
    add(F_FINGER, a, b, x, y, 0);
}
int TaTuto_ControlFade(int kind, int direction, int flag) { add(F_FADE, kind, direction, flag, 0, 0); return fade_ret; }
void TaTuto_RestoreOriginalView(void *area, int a)
{
    assert(area == rec.text || area == rec32 + 0x1b0);
    add(F_RESTORE, a, 0, 0, 0, 0);
}
void TaTuto_BkupOriginalView(void *area)
{
    assert(area == rec.text || area == rec32 + 0x1b0);
    add(F_BKUP, 0, 0, 0, 0, 0);
}
int TaTuto_OpenTutoMsg(int first, int last, struct DotrTutoL033Record *r)
{
    assert(r == &rec);
    add(F_MSG, first, last, 0, 0, 0);
    return msg_ret;
}

static void reset(int step, int wait)
{
    memset(&rec, 0, sizeof rec);
    rec.step = (unsigned short)step;
    rec.wait = (unsigned short)wait;
    ntrace = 0;
    window_disp = 0;
    pad_mask = 0;
    fade_ret = 1;
    msg_ret = 1;
    standby_fail = -1;
}

static int run(void)
{
    return L033(99, &rec);
}

static void check_prelude(void)
{
    int i;

    reset(0x3e, 0);
    window_disp = 1;
    assert(run() == 0x800 && rec.step == 0x3f);
    assert(trace[0].fn == F_DRAW && trace[0].a[0] == 1);
    for (i = 0; i < 4; i++) {
        assert(trace[1 + i].fn == F_ANALOG && trace[1 + i].a[0] == 0 && trace[1 + i].a[1] == i);
        assert(trace[1 + i].a[2] == 0x80); /* a full int, not a signed char */
    }
    reset(0x3e, 0);
    assert(run() == 0x800 && trace[0].a[0] == 0);

    /* Button 0x800 stops the script before the step runs. */
    reset(0, 0);
    pad_mask = 0x800;
    assert(run() == -2 && rec.step == 0 && ntrace == 5);
}

static void check_first_steps(void)
{
    reset(0, 0);
    assert(run() == 0 && rec.step == 2);
    expect(F_IGNORE, 0, 1, 1, 0, 0, 0, 0);
    expect(F_DISP, 0, 1, 1, 0, 0, 0, 0);

    reset(1, 0);
    assert(run() == 0 && rec.step == 2);
    reset(1, 0);
    standby_fail = 0x100;
    assert(run() == 0 && rec.step == 1);

    reset(2, 0);
    assert(run() == 0 && count(F_DISP) == 1);
    expect(F_MSG, 0, 2, 0xa48, 0xa4a, 0, 0, 0);
    msg_ret = 0;
    ntrace = 0;
    assert(run() == 0 && count(F_DISP) == 0 && rec.step == 2);

    /* Step 3: after the fade-out, the first board. */
    reset(3, 9);
    assert(run() == 0 && rec.step == 4 && rec.wait == 0);
    expect(F_FADE, 0, 3, 8, -1, 0, 0, 0);
    assert(count(F_BKUP) == 1);
    expect(F_LP, 0, 3, 0, 4, 4000, 0, 0);
    expect(F_LP, 1, 3, 1, 4, 1000, 0, 0);
    expect(F_ADD, 0, 5, 0, 3, 1, 0x9c, 0);
    expect(F_STATUS, 0, 4, 0, 1, 4, 1, 0);
    reset(3, 9);
    fade_ret = 0;
    assert(run() == 0 && rec.step == 3 && rec.wait == 9 && count(F_ADD) == 0);

    /* Step 4: waits more than 0x1e frames once model 1 is ready. */
    reset(4, 0x1e);
    assert(run() == 0 && rec.step == 4 && rec.wait == 0x1f);
    expect(F_CUR, 0, 3, 0, 3, 1, 0, 0);
    assert(run() == 0 && rec.step == 5 && rec.wait == 0);
    reset(4, 0x1e);
    standby_fail = 1;
    assert(run() == 0 && rec.wait == 0x1e);

    reset(5, 3);
    assert(run() == 0 && rec.step == 6 && rec.wait == 0 && count(F_DISP) == 2);
    expect(F_FOCUS, 0, 1, 0x9c, 0, 0, 0, 0);
    expect(F_FADE, 0, 3, 8, 1, 1, 0, 0);

    reset(6, 3);
    rec.timer = 7;
    assert(run() == 0 && rec.step == 6 && rec.wait == 0 && rec.timer == 0);
    expect(F_MSG, 0, 2, 0xa4b, 0xa4b, 0, 0, 0);

    /* Step 7: the finger for more than 0x3c frames. */
    reset(7, 3);
    rec.timer = 0x3c;
    assert(run() == 0 && rec.step == 7 && rec.timer == 0x3d);
    expect(F_FINGER, 0, 4, 0xa, 0x800, 0x12c, -0x9c, 0);
    assert(run() == 0 && rec.step == 8 && rec.timer == 0 && rec.wait == 0 && count(F_CLOSE) == 1);
}

static void check_buttons(void)
{
    static const int steps[] = {9, 10, 11, 0x11, 0x12, 0x16, 0x17, 0x18, 0x19, 0x1f, 0x13, 0x15};
    static const int masks[] = {0x40, 0x1000, 0x40, 0x1000, 0x10, 0x4000, 0x40, 0x1000, 0x40, 0x80, 0x4000, 0x20};
    unsigned int i;

    for (i = 0; i < sizeof steps / sizeof steps[0]; i++) {
        reset(steps[i], 0xc);
        assert(run() == 0 && rec.wait == 0xd && rec.step == steps[i]);
        assert(run() == masks[i] && rec.wait == 0 && rec.step == steps[i] + 1);
        assert(ntrace == 10); /* prelude only */
    }
}

static void check_boards(void)
{
    reset(0xd, 4);
    assert(run() == 0 && rec.step == 0xe && rec.wait == 0);
    expect(F_DEL, 0, 2, 0, 1, 0, 0, 0);
    expect(F_LP, 0, 3, 0, 4, 500, 0, 0);
    expect(F_ADD, 1, 5, 1, 3, 3, 0x15a, 0);
    expect(F_STATUS, 1, 4, 1, 1, 4, 1, 0);

    reset(0xe, 0x1f);
    standby_fail = 0x101;
    assert(run() == 0 && rec.wait == 0x1f);
    standby_fail = -1;
    assert(run() == 0 && rec.step == 0xf);

    reset(0x10, 0);
    assert(run() == 0);
    expect(F_MSG, 0, 2, 0xa4f, 0xa51, 0, 0, 0);
    reset(0x14, 0);
    assert(run() == 0);
    expect(F_MSG, 0, 2, 0xa52, 0xa54, 0, 0, 0);

    reset(0x1b, 0);
    assert(run() == 0 && rec.step == 0x1c);
    expect(F_DEL, 1, 2, 1, 1, 0, 0, 0);
    expect(F_MOVE, 0, 4, 0, 0, 3, 4, 0);
    expect(F_LP, 1, 3, 1, 4, 4000, 0, 0);

    /* Step 0x21: the opponent's eight units, then button 0x20. */
    reset(0x21, 5);
    assert(run() == 0x20 && rec.step == 0x22 && rec.wait == 0);
    expect(F_FADE, 0, 3, 0x10, -1, 0, 0, 0);
    assert(count(F_ADD) == 8 && count(F_STATUS) == 5);
    expect(F_ADD, 0, 5, 1, 3, 3, 3, 0);
    expect(F_ADD, 7, 5, 1, 4, 5, 0x2ab, 0);
    expect(F_STATUS, 4, 4, 1, 5, 4, 1, 0);
    reset(0x21, 5);
    fade_ret = 0;
    assert(run() == 0 && rec.step == 0x21 && count(F_ADD) == 0);

    reset(0x22, 0x1f);
    assert(run() == 0 && rec.step == 0x23 && count(F_STDVIEW) == 1);
    reset(0x22, 0x1f);
    standby_fail = 0x105;
    assert(run() == 0 && rec.step == 0x22 && rec.wait == 0x1f);

    /* Step 0x25: units 6..8 removed, five units given goal 0 and rate -1, moves. */
    reset(0x25, 0);
    assert(run() == 0 && rec.step == 0x26);
    assert(count(F_DEL) == 3 && count(F_GOAL) == 5 && count(F_RATE) == 5 && count(F_MOVE) == 6);
    expect(F_RATE, 0, 4, 1, 1, -1, 0, 0);
    expect(F_GOAL, 4, 3, 1, 5, 0, 0, 0);
    expect(F_MOVE, 5, 4, 0, 0, 0, 6, 0);

    reset(0x26, 0x1f);
    assert(run() == 0x80 && rec.step == 0x27 && count(F_HALFVIEW) == 1);
    expect(F_CUR, 0, 3, 0, 0, 6, 0, 0);

    /* Step 0x29: goals and rates of 0x80 (not -128). */
    reset(0x29, 0);
    assert(run() == 0 && rec.step == 0x2a);
    expect(F_GOAL, 0, 3, 1, 1, 0x80, 0, 0);
    expect(F_RATE, 2, 4, 1, 3, 1, 0x80, 0);
    expect(F_MOVE, 2, 4, 1, 3, 1, 6, 0);

    /* Step 0x2a: the message only after the fade. */
    reset(0x2a, 0);
    fade_ret = 0;
    assert(run() == 0 && count(F_MSG) == 0);
    fade_ret = 1;
    assert(run() == 0 && count(F_DISP) == 1);
    expect(F_MSG, 0, 2, 0xa5e, 0xa5e, 0, 0, 0);

    reset(0x2b, 0);
    assert(run() == 0x20 && rec.step == 0x2c && count(F_FIELD) == 7);
    expect(F_FIELD, 0, 3, 3, 6, 8, 0, 0);
    expect(F_FIELD, 6, 3, 4, 4, 8, 0, 0);

    reset(0x31, 0);
    assert(run() == 0x20 && rec.step == 0x32 && count(F_DEL) == 5 && count(F_FIELD) == 7);
    expect(F_FIELD, 0, 3, 3, 6, 7, 0, 0);
    expect(F_TURN, 0, 1, 0x63, 0, 0, 0, 0);

    reset(0x32, 0x1f);
    assert(run() == 0 && rec.step == 0x33);
    expect(F_CUR, 0, 3, 0, 3, 6, 0, 0);

    reset(0x36, 0);
    assert(run() == 0 && rec.step == 0x37 && count(F_ADD) == 4 && count(F_STATUS) == 4);
    expect(F_ADD, 0, 5, 0, 6, 0, 0x39, 0);
    expect(F_TURN, 0, 1, 0, 0, 0, 0, 0);

    reset(0x39, 0);
    assert(run() == 0 && rec.step == 0x3a && count(F_BKUP) == 1);
    expect(F_FOCUS, 0, 1, 0x3a, 0, 0, 0, 0);

    /* Step 0x3a: the name popup each frame of the wait. */
    reset(0x3a, 0x3d);
    assert(run() == 0 && rec.step == 0x3b);
    expect(F_POPUP, 0, 4, 0, -0x78, 0x50, 0x5000 + 0x3a, 0);

    reset(0x3c, 0);
    assert(run() == 0 && rec.step == 0x3d && count(F_MOVE) == 4);
    expect(F_RESTORE, 0, 1, 0, 0, 0, 0, 0);
}

static void check_end(void)
{
    reset(0x3d, 0);
    assert(run() == 0x800 && count(F_DISP) == 1);
    expect(F_MSG, 0, 2, 0xa6c, 0xa6e, 0, 0, 0);
    reset(0x3d, 0);
    msg_ret = 0;
    assert(run() == 0);

    reset(0x3f, 0);
    assert(run() == -1);
    msg_ret = 0;
    assert(run() == 0);

    reset(0x40, 0);
    assert(run() == -1);
    reset(0xffff, 0);
    assert(run() == -1);
}

/* L032: the halfwords at +0x162 (timer), +0x1a2 (step) and +0x1a4 (wait). */
static unsigned int half32(int offset)
{
    unsigned short v;

    memcpy(&v, rec32 + offset, sizeof v);
    return v;
}
static void set32(int offset, int value)
{
    unsigned short v = (unsigned short)value;

    memcpy(rec32 + offset, &v, sizeof v);
}
#define STEP32 half32(0x1a2)
#define WAIT32 half32(0x1a4)
#define TIMER32 half32(0x162)

static void reset32(int step, int wait)
{
    reset(0, 0);
    memset(rec32, 0, sizeof rec32);
    set32(0x1a2, step);
    set32(0x1a4, wait);
}

static int run32(int pad)
{
    return L032(pad, rec32);
}

static void check_l032_prelude(void)
{
    int i;

    reset32(0, 0);
    window_disp = 1;
    assert(run32(0) == 0 && STEP32 == 2 && ntrace == 5); /* step 0: straight to step 2 */
    assert(trace[0].fn == F_DRAW && trace[0].a[0] == 1);
    for (i = 0; i < 4; i++) {
        assert(trace[1 + i].fn == F_ANALOG && trace[1 + i].a[1] == i && trace[1 + i].a[2] == 0x80);
    }
    reset32(0, 0);
    pad_mask = 0x800;
    assert(run32(0) == -2 && STEP32 == 0 && trace[0].a[0] == 0);
}

/* Steps that wait for the counter at +0x1a4 to pass a bound, then advance and return a button
 * mask (0 for step 9). */
static void check_l032_buttons(void)
{
    static const int steps[] = {8, 9, 0xb, 0xc, 0x12, 0x13, 0x14, 0x15, 0x17, 0x19, 0x20, 0x22, 0x23,
                                0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2e, 0x2f, 0x30, 0x31,
                                0x33, 0x34, 0x36, 0x3c, 0x3d, 0x3f, 0x40, 0x46, 0x47, 0x4d, 0x4e,
                                0x54, 0x55, 0x5b, 0x5c, 0x62, 0x63, 0x69, 0x6a,
                                0x71, 0x72, 0x73, 0x74, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7d, 0x7e,
                                0x7f, 0x80, 0x82, 0x83, 0x84, 0x8b, 0x8d, 0x8e, 0x8f, 0x90, 0x98, 0x99, 0x9a};
    static const int bounds[] = {0xc, 0x1e, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
                                 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
                                 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
                                 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
                                 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc,
                                 0xc, 0xc, 0xc, 0xc, 0xc, 0x3c, 0x3c, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc};
    static const int masks[] = {0x80, 0, 0x1000, 0x40, 0x2000, 0x2000, 0x2000, 0x8000, 0x40, 0x40, 0x40, 0x1000, 0x40,
                                0x40, 0x1000, 0x40, 0x80, 0x1000, 0x40, 0x40, 0x1000, 0x40, 0x800,
                                0x40, 0x1000, 0x40, 0x1000, 0x4000, 0x1000, 0x40, 0x1000, 0x40, 0x1000, 0x40,
                                0x1000, 0x40, 0x1000, 0x40, 0x1000, 0x40, 0x1000, 0x40,
                                8, 4, 8, 4, 8, 0x1000, 0x4000, 8, 0x2000, 0x8000, 2, 1,
                                2, 1, 8, 2, 0x40, 0x4000, 0x20, 0x40, 2, 0x40, 0x100, 0x8000, 0x20};
    unsigned int i;

    assert(sizeof steps == sizeof bounds && sizeof steps == sizeof masks);
    for (i = 0; i < sizeof steps / sizeof steps[0]; i++) {
        reset32(steps[i], bounds[i]);
        assert(run32(0) == 0 && WAIT32 == (unsigned int)bounds[i] + 1 && STEP32 == (unsigned int)steps[i]);
        assert(run32(0) == masks[i] && WAIT32 == 0 && STEP32 == (unsigned int)steps[i] + 1);
        assert(ntrace == 10); /* prelude only */
    }
}

static void check_l032_steps(void)
{
    reset32(1, 0);
    assert(run32(0) == 0 && STEP32 == 2);
    reset32(1, 0);
    standby_fail = 0x100;
    assert(run32(0) == 0 && STEP32 == 1);

    reset32(2, 0);
    assert(run32(0) == 0 && count(F_DISP) == 1);
    expect(F_MSG, 0, 2, 0x9dc, 0x9df, 0, 0, 0);
    msg_ret = 0;
    ntrace = 0;
    assert(run32(0) == 0 && count(F_DISP) == 0);

    reset32(3, 0);
    assert(run32(0) == 0 && STEP32 == 4 && count(F_BKUP) == 1);
    expect(F_FADE, 0, 3, 0x10, -1, 0, 0, 0);
    expect(F_FOCUS32, 0, 1, 0x3c, 0, 0, 0, 0);
    reset32(3, 0);
    fade_ret = 0;
    assert(run32(0) == 0 && STEP32 == 3 && count(F_BKUP) == 0);

    /* Step 4: the name popup while waiting more than 0x3c frames. */
    reset32(4, 0x3c);
    assert(run32(0) == 0 && STEP32 == 4 && WAIT32 == 0x3d);
    expect(F_POPUP, 0, 4, 0, -0x78, 0x50, 0x5000 + 0x3c, 0);
    assert(run32(0) == 0 && STEP32 == 5 && WAIT32 == 0);

    reset32(6, 0);
    assert(run32(0) == 0 && STEP32 == 7);
    expect(F_RESTORE, 0, 1, 0, 0, 0, 0, 0);
    reset32(7, 0);
    assert(run32(0) == 0 && count(F_DISP) == 1);
    expect(F_MSG, 0, 2, 0x9e2, 0x9e2, 0, 0, 0);

    /* Steps 0xd/0xe: message, then the finger for more than 0x3c frames. */
    reset32(0xd, 0);
    set32(0x162, 9);
    assert(run32(0) == 0 && TIMER32 == 0);
    expect(F_MSG, 0, 2, 0x9e6, 0x9e8, 0, 0, 0);
    reset32(0xe, 5);
    set32(0x162, 0x3c);
    assert(run32(0) == 0 && TIMER32 == 0x3d && STEP32 == 0xe);
    expect(F_FINGER, 0, 4, 0xa, 0x800, -0xe8, -0x8c, 0);
    assert(run32(0) == 0 && TIMER32 == 0 && WAIT32 == 0 && STEP32 == 0xf && count(F_CLOSE) == 1);
    reset32(0x10, 0);
    assert(run32(0) == 0);
    expect(F_FINGER, 0, 4, 0xa, 0x800, 0x3c, 0x4b, 0);

    /* Step 0x1b: after more than 0x3c frames, the fade, then focus on card 0. */
    reset32(0x1b, 0x3d);
    assert(run32(0) == 0 && STEP32 == 0x1c && WAIT32 == 0);
    expect(F_FOCUS32, 0, 1, 0, 0, 0, 0, 0);
    reset32(0x1b, 0x3d);
    fade_ret = 0;
    assert(run32(0) == 0 && STEP32 == 0x1b && WAIT32 == 0x3e && count(F_DISP) == 1);

    reset32(0x1c, 0);
    assert(run32(0) == 0);
    expect(F_POPUP, 0, 4, 0, -0x78, 0x50, 0x5000, 0);

    /* Messages that end with a button. */
    reset32(0x24, 0);
    assert(run32(0) == 0x800);
    expect(F_MSG, 0, 2, 0x9fc, 0xa00, 0, 0, 0);
    reset32(0x3e, 0);
    assert(run32(0) == 0x40);
    msg_ret = 2;
    assert(run32(0) == 0); /* exactly 1 */
    reset32(0x45, 0);
    msg_ret = 2;
    assert(run32(0) == 0x40); /* any non-zero */
    msg_ret = 0;
    assert(run32(0) == 0);

    /* Step 0x38: the first board after the fade-out. */
    reset32(0x38, 4);
    assert(run32(0) == 0 && STEP32 == 0x39 && WAIT32 == 0);
    expect(F_LP, 0, 3, 0, 4, 4000, 0, 0);
    expect(F_LP, 1, 3, 1, 4, 4000, 0, 0);
    expect(F_DEL, 1, 2, 1, 1, 0, 0, 0);
    expect(F_ADD, 0, 5, 0, 3, 4, 0x9c, 0);
    expect(F_ADD, 1, 5, 1, 3, 3, 0x15, 0);
    assert(count(F_STATUS) == 2);

    reset32(0x39, 0x1f);
    assert(run32(0) == 0 && STEP32 == 0x3a);
    expect(F_CUR, 0, 3, 0, 3, 4, 0, 0);
    reset32(0x39, 0x1f);
    standby_fail = 0x101;
    assert(run32(0) == 0 && STEP32 == 0x39 && WAIT32 == 0x1f);

    reset32(0x3a, 0);
    assert(run32(0) == 0 && STEP32 == 0x3b && count(F_DISP) == 2);
    expect(F_FOCUS32, 0, 1, 0x9c, 0, 0, 0, 0);
    expect(F_FADE, 0, 3, 8, 1, 1, 0, 0);

    reset32(0x50, 0);
    assert(run32(0) == 0 && STEP32 == 0x51 && count(F_STATUS) == 4);
    expect(F_ADD, 1, 5, 1, 3, 3, 0xa9, 0);
    expect(F_STATUS, 3, 4, 1, 1, 5, 1, 0);

    reset32(0x65, 0);
    assert(run32(0) == 0 && STEP32 == 0x66);
    expect(F_LP, 1, 3, 1, 4, 0xed8, 0, 0);
    expect(F_ADD, 0, 5, 0, 3, 2, 0x9c, 0);
    reset32(0x6c, 0);
    assert(run32(0) == 0 && STEP32 == 0x6d);
    expect(F_ADD, 0, 5, 0, 3, 5, 0x272, 0);
    reset32(0x6d, 0x1f);
    assert(run32(0) == 0 && STEP32 == 0x6e);
    expect(F_CUR, 0, 3, 0, 3, 5, 0, 0);

    reset32(0x86, 0);
    assert(run32(0) == 0 && STEP32 == 0x87);
    expect(F_ADD, 0, 5, 1, 2, 2, 3, 0);
    expect(F_ADD, 1, 5, 0, 2, 6, 0x2ab, 0);

    /* Steps 0x8a and 0x91 look at the high byte of the first parameter. */
    reset32(0x8a, 0);
    assert(run32(0x6ff) == 0x10 && STEP32 == 0x8b);
    reset32(0x8a, 3);
    assert(run32(0x10500) == 0 && WAIT32 == 4 && STEP32 == 0x8a);
    assert(run32(0) == 0x4000 && WAIT32 == 0 && STEP32 == 0x8a);
    reset32(0x8a, 0);
    assert(run32(0x10600) == 0x10 && STEP32 == 0x8b); /* low halfword only */
    reset32(0x91, 0);
    assert(run32(0x200) == 0 && STEP32 == 0x92);
    reset32(0x91, 4);
    assert(run32(0x300) == 0x1000 && WAIT32 == 0 && STEP32 == 0x91);

    reset32(0x93, 0);
    assert(run32(0) == 0 && STEP32 == 0x94 && count(F_DEL) == 3 && count(F_ADD) == 4 && count(F_STATUS) == 4);
    expect(F_LP, 0, 3, 0, 4, 0x320, 0, 0);
    expect(F_ADD, 3, 5, 0, 4, 6, 0x9d, 0);
    expect(F_STATUS, 3, 4, 1, 3, 4, 1, 0);
    reset32(0x94, 0);
    assert(run32(0) == 0 && STEP32 == 0x95);
    expect(F_CUR, 0, 3, 0, 3, 6, 0, 0);
    reset32(0x95, 0x1f);
    standby_fail = 0x103;
    assert(run32(0) == 0 && STEP32 == 0x95 && WAIT32 == 0x1f);
    standby_fail = -1;
    assert(run32(0) == 0 && STEP32 == 0x96);
    reset32(0x9b, 0);
    assert(run32(0) == 0 && STEP32 == 0x9c);
}

static void check_l032_end(void)
{
    reset32(0x9c, 0);
    assert(run32(0) == 0 && count(F_DISP) == 1);
    expect(F_MSG, 0, 2, 0xa45, 0xa46, 0, 0, 0);
    reset32(0x9d, 0);
    assert(run32(0) == -1);
    expect(F_MSG, 0, 2, 0xa47, 0xa47, 0, 0, 0);
    reset32(0x9d, 0);
    fade_ret = 0;
    assert(run32(0) == 0 && count(F_MSG) == 0);
    reset32(0x9e, 0);
    assert(run32(0) == -1);
    reset32(0xffff, 0);
    assert(run32(0) == -1);
}

/* Smoke test only (docs/DECOMPILATION.md, Tests).
 * L036: step 2 opens messages 0xbc1..0xbc2 and, with the message done, stays at step 2. */
static void check_l036_smoke(void)
{
    reset(2, 0);
    msg_ret = 1;
    assert(L036(0, &rec) == 0);
    assert(count(F_ANALOG) == 4 && count(F_MSG) == 1);
    expect(F_MSG, 0, 2, 0xbc1, 0xbc2, 0, 0, 0);
    assert(rec.step == 2);
}

int main(void)
{
    check_l036_smoke();
    check_l032_prelude();
    check_l032_buttons();
    check_l032_steps();
    check_l032_end();
    check_prelude();
    check_first_steps();
    check_buttons();
    check_boards();
    check_end();
    return 0;
}

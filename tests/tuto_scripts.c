/* Synthetic host tests for the tuto_scripts group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/tuto_scripts.h"

enum {
    F_DRAW = 1, F_DISP, F_ANALOG, F_IGNORE, F_CUR, F_LP, F_POWER, F_HAND, F_DESTINY, F_ADD, F_DEL,
    F_MOVE, F_CURSE, F_STATUS, F_FIELD, F_POPUP, F_CLOSE, F_TURN, F_RANK, F_TACURSE, F_DBGINIT,
    F_FOCUS, F_DECK, F_FINGER, F_TRAP, F_FADE, F_RESTORE, F_BKUP, F_MSG
};

struct call {
    int fn;
    long a[5];
};

static struct call trace[256];
static int ntrace;
static int window_disp, pad_mask, fade_ret, msg_ret, standby_fail;
static struct DotrTutoScriptRecord rec;

static void add(int fn, long a, long b, long c, long d, long e)
{
    assert(ntrace < 256);
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
int SzDuel_CalcSummonPower(int index, int operation, int value) { add(F_POWER, index, operation, value, 0, 0); return 0; }
void SzDuel_SetHandPhase(int index, int value) { add(F_HAND, index, value, 0, 0, 0); }
void SzDuel_SetDestinyFlag(int index, signed char value) { add(F_DESTINY, index, value, 0, 0, 0); }
void SzDuel_AddUnit(int side, int x, int y, int card, int flag) { add(F_ADD, side, x, y, card, flag); }
int SzDuel_DelUnit(int side, int id) { add(F_DEL, side, id, 0, 0, 0); return 0; }
void *SzDuel_MoveUnit(int side, int id, int x, int y) { add(F_MOVE, side, id, x, y, 0); return NULL; }
int SzDuel_SetUnitCurseRest(int side, int id, signed char value) { add(F_CURSE, side, id, value, 0, 0); return 0; }
int SzDuel_SetUnitStatus(int side, int id, int bit, int value) { add(F_STATUS, side, id, bit, value, 0); return 0; }
int SzDuel_SetField(int x, int y, signed char field) { add(F_FIELD, x, y, field, 0, 0); return 0; }
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value) { add(F_POPUP, operation, a, b, value, 0); }
void SzDuel_CloseHandCursor(void) { add(F_CLOSE, 0, 0, 0, 0, 0); }
int SzDuel_GetMsgName(int id) { return 0x5000 + id; }
void SzDuel_SetTurnCount(int count_) { add(F_TURN, count_, 0, 0, 0, 0); }
int SzData_SetRankDirect(int side, int a, int b) { add(F_RANK, side, a, b, 0, 0); return 0; }
void Ta_SetUnitCurseRest(int side, int id, int value) { add(F_TACURSE, side, id, value, 0, 0); }
void TaTuto_DbgInit034(int a) { add(F_DBGINIT, a, 0, 0, 0, 0); }
void TaTuto_FocusUnit__00175ba0(int card) { add(F_FOCUS, card, 0, 0, 0, 0); }
void TaTuto_SetDeck035(int step) { add(F_DECK, step, 0, 0, 0, 0); }
void TaTuto_DrawFinger(struct DotrTutoScriptRecord *r, int a, int b, int x, int y)
{
    assert(r == &rec);
    add(F_FINGER, a, b, x, y, 0);
}
void TaTuto_DrawTrapArea(struct DotrTutoScriptRecord *r, int a, int x, int y)
{
    assert(r == &rec);
    add(F_TRAP, a, x, y, 0, 0);
}
int TaTuto_ControlFade(int kind, int direction, int flag) { add(F_FADE, kind, direction, flag, 0, 0); return fade_ret; }
void TaTuto_RestoreOriginalView(void *area, int a)
{
    assert(area == rec.text);
    add(F_RESTORE, a, 0, 0, 0, 0);
}
void TaTuto_BkupOriginalView(void *area)
{
    assert(area == rec.text);
    add(F_BKUP, 0, 0, 0, 0, 0);
}
int TaTuto_OpenTutoMsg(int first, int last, struct DotrTutoScriptRecord *r)
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

static int run34(void)
{
    return L034(99, &rec);
}

static int run35(int cursor)
{
    return L035(cursor, &rec);
}

static void check_prelude(int which)
{
    int i;

    reset(0, 0);
    window_disp = 1;
    assert((which == 34 ? run34() : run35(0)) == 0 && rec.step == 2);
    assert(trace[0].fn == F_DRAW && trace[0].a[0] == 1);
    for (i = 0; i < 4; i++) {
        assert(trace[1 + i].fn == F_ANALOG && trace[1 + i].a[0] == 0 && trace[1 + i].a[1] == i);
        assert(trace[1 + i].a[2] == 0x80); /* a full int, not a signed char */
    }
    expect(F_IGNORE, 0, 1, 1, 0, 0, 0, 0);
    assert(count(F_DISP) == (which == 34)); /* only L034 shows the window on step 0 */
    reset(0, 0);
    assert((which == 34 ? run34() : run35(0)) == 0 && trace[0].a[0] == 0);

    /* Button 0x800 stops the script before the step runs. */
    reset(0, 0);
    pad_mask = 0x800;
    assert((which == 34 ? run34() : run35(0)) == -2 && rec.step == 0 && ntrace == 5);
}

static void check_l034_messages(void)
{
    /* Step 1 waits for both leaders' models. */
    reset(1, 0);
    assert(run34() == 0 && rec.step == 2);
    reset(1, 0);
    standby_fail = 0x100;
    assert(run34() == 0 && rec.step == 1);

    /* Messages are 0x9dc above the table; kind 1 hides the window when done. */
    reset(2, 0);
    assert(run34() == 0 && rec.step == 2);
    expect(F_MSG, 0, 2, 0xa70, 0xa71, 0, 0, 0);
    expect(F_DISP, 0, 1, 0, 0, 0, 0, 0);
    reset(2, 0);
    msg_ret = 0;
    assert(run34() == 0 && count(F_DISP) == 0);
    /* Kind 2 clears both counters. */
    reset(6, 7);
    rec.timer = 9;
    assert(run34() == 0 && rec.wait == 0 && rec.timer == 0);
    expect(F_MSG, 0, 2, 0xa72, 0xa72, 0, 0, 0);
    /* Kind 3 asks for button 0x4000; kind 4 jumps to step 0x94; kind 5 hides and asks 0x20. */
    reset(0xb0, 0);
    assert(run34() == 0x4000);
    expect(F_MSG, 0, 2, 0xac4, 0xac4, 0, 0, 0);
    reset(0xfe, 0);
    assert(run34() == 0 && rec.step == 0x94);
    reset(0x114, 0);
    assert(run34() == 0x20 && count(F_DISP) == 1);
    expect(F_MSG, 0, 2, 0xb01, 0xb04, 0, 0, 0);
    reset(0x11f, 0);
    assert(run34() == 0);
    expect(F_MSG, 0, 2, 0xb0b, 0xb0b, 0, 0, 0);
}

static void check_l034_boards(void)
{
    /* Fade-out steps set up the board and advance. */
    reset(3, 5);
    assert(run34() == 0 && rec.step == 4 && rec.wait == 0);
    expect(F_FADE, 0, 3, 8, -1, 0, 0, 0);
    assert(count(F_ADD) == 10 && count(F_STATUS) == 5);
    expect(F_ADD, 0, 5, 0, 3, 4, 0x9c, 0);
    expect(F_ADD, 9, 5, 0, 1, 5, 0x2ab, 0);
    expect(F_STATUS, 4, 4, 0, 5, 4, 1, 0);
    reset(3, 5);
    fade_ret = 0;
    assert(run34() == 0 && rec.step == 3 && rec.wait == 5 && count(F_ADD) == 0);

    reset(0x11, 0);
    assert(run34() == 0 && rec.step == 0x12);
    assert(count(F_DEL) == 20);
    expect(F_DEL, 0, 2, 0, 1, 0, 0, 0);
    expect(F_DEL, 19, 2, 1, 10, 0, 0, 0);
    expect(F_DBGINIT, 0, 1, 1, 0, 0, 0, 0);
    expect(F_POWER, 1, 3, 1, 4, 4, 0, 0);
    expect(F_HAND, 0, 2, 0, 1, 0, 0, 0);

    reset(0x8f, 0);
    assert(run34() == 0x20 && rec.step == 0x90);
    assert(count(F_FIELD) == 9);
    expect(F_FIELD, 0, 3, 2, 2, 5, 0, 0);
    expect(F_FIELD, 8, 3, 4, 4, 5, 0, 0);
    expect(F_ADD, 0, 5, 0, 3, 5, 0x170, 0);

    reset(0xdd, 0);
    assert(run34() == 0 && rec.step == 0xde);
    expect(F_MOVE, 0, 4, 0, 1, 3, 4, 0);
    expect(F_CURSE, 0, 3, 0, 1, 0, 0, 0);
    reset(0xec, 0);
    assert(run34() == 0);
    expect(F_LP, 0, 3, 0, 4, 0x190, 0, 0);
    reset(0x103, 0);
    assert(run34() == 0);
    expect(F_RANK, 1, 3, 1, 0x15, 0xc, 0, 0);

    /* Fade 0x10 steps; 0x116 hides the window first; 0x58 asks for 0x8000. */
    reset(0x116, 0);
    assert(run34() == 0 && rec.step == 0x117);
    assert(trace[5].fn == F_DISP && trace[5].a[0] == 0);
    expect(F_FADE, 0, 3, 0x10, -1, 0, 0, 0);
    expect(F_ADD, 0, 5, 0, 3, 2, 0x9c, 0);
    reset(0x58, 0);
    assert(run34() == 0x8000 && rec.step == 0x59);
    reset(0x9d, 0);
    assert(run34() == 0 && count(F_BKUP) == 1);
    reset(0xa2, 0);
    assert(run34() == 0);
    expect(F_RESTORE, 0, 1, 0, 0, 0, 0, 0);
    reset(0xd7, 0);
    assert(run34() == 0);
    expect(F_TACURSE, 0, 3, 0, 1, 0xff, 0, 0);
    reset(0xf0, 0);
    assert(run34() == 0);
    expect(F_DESTINY, 0, 2, 0, 2, 0, 0, 0);
}

static void check_l034_waits(void)
{
    /* Step 4 waits for five models, then more than 0x1e frames. */
    reset(4, 0x1e);
    assert(run34() == 0 && rec.step == 4 && rec.wait == 0x1f);
    assert(run34() == 0 && rec.step == 5 && rec.wait == 0);
    reset(4, 0x1e);
    standby_fail = 5;
    assert(run34() == 0 && rec.wait == 0x1e);

    /* Cursor placement after the wait depends on the step. */
    reset(0x83, 0x1f);
    assert(run34() == 0 && rec.step == 0x84);
    expect(F_CUR, 0, 3, 0, 3, 3, 0, 0);
    reset(0x3c, 0x1f);
    assert(run34() == 0);
    expect(F_CUR, 0, 3, 0, 3, 4, 0, 0);
    reset(0x12, 0x1f);
    assert(run34() == 0);
    expect(F_CUR, 0, 3, 0, 3, 6, 0, 0);
    reset(0x12, 0x1e);
    assert(run34() == 0 && rec.step == 0x12 && count(F_CUR) == 0);

    /* Waiting steps ask for a button after more than 0xc frames. */
    reset(0x9, 0xc);
    assert(run34() == 0 && rec.wait == 0xd);
    assert(run34() == 0x80 && rec.step == 0xa && rec.wait == 0);
    reset(0xa, 0xd);
    assert(run34() == 0x1000);
    reset(0xb, 0xd);
    assert(run34() == 0x40);
    reset(0xc, 0xd);
    assert(run34() == 0x2000);
    reset(0x47, 0xd);
    assert(run34() == 0x8000);
    reset(0x4d, 0xd);
    assert(run34() == 0x4000);
    reset(0x20, 0xd);
    assert(run34() == 0x800);
    /* ... and after more than 0x3c frames on the others. */
    reset(0x88, 0x3c);
    assert(run34() == 0 && rec.wait == 0x3d);
    assert(run34() == 2 && rec.step == 0x89);
    assert((rec.wait = 0x3d, run34()) == 1);
    reset(0x109, 0x3d);
    assert(run34() == 0x10);
    reset(0x10e, 0x3d);
    assert(run34() == 0x20);
    reset(0x115, 0x3d);
    assert(run34() == 0x800);

    /* Fade-in steps: 0x91 jumps to 0x9b first; 0x33 focuses card 0x9c. */
    reset(0x91, 4);
    assert(run34() == 0 && rec.step == 0x9c && rec.wait == 0);
    expect(F_FADE, 0, 3, 8, 1, 1, 0, 0);
    expect(F_DISP, 0, 1, 1, 0, 0, 0, 0);
    reset(0x33, 0);
    fade_ret = 0;
    assert(run34() == 0 && rec.step == 0x33);
    expect(F_FOCUS, 0, 1, 0x9c, 0, 0, 0, 0);
    /* Fade 0x10 in: 0xd0 goes on to 0x92; 0x9f focuses 0x170 and keeps the window hidden. */
    reset(0xd0, 0);
    assert(run34() == 0 && rec.step == 0x92 && count(F_DISP) == 1);
    reset(0x9f, 0);
    assert(run34() == 0 && rec.step == 0xa0 && count(F_DISP) == 0);
    expect(F_FOCUS, 0, 1, 0x170, 0, 0, 0, 0);

    /* The finger is drawn for more than 0x3c frames, then the hand cursor closes. */
    reset(0x7, 3);
    rec.timer = 0x3d;
    assert(run34() == 0 && rec.step == 8 && rec.timer == 0 && rec.wait == 0 && count(F_CLOSE) == 1);
    expect(F_FINGER, 0, 4, 0xa, 0x800, -0xe8, -0x78, 0);
    reset(0x107, 0);
    assert(run34() == 0 && rec.step == 0x107 && rec.timer == 1);
    expect(F_FINGER, 0, 4, 0xa, 0x800, 0x78, 0xb4, 0);

    /* Pop-up with the name of card 0x170. */
    reset(0xa0, 0x3d);
    assert(run34() == 0 && rec.step == 0xa1);
    expect(F_POPUP, 0, 4, 0, -0x78, 0x50, 0x5170, 0);

    reset(0x8d, 0);
    assert(run34() == 0 && rec.step == 0x8e);
    reset(0x93, 0);
    assert(run34() == 0 && rec.step == 0xd1);
    reset(0x94, 0);
    assert(run34() == 0x20 && rec.step == 0x95);
    reset(0x95, 0);
    assert(run34() == 0x20 && rec.step == 0xff);
    reset(0x100, 0x1f);
    assert(run34() == 0x20 && rec.step == 0x101 && count(F_BKUP) == 1);
    reset(0x120, 0);
    assert(run34() == -1 && ntrace == 5);
}

static void check_l035(void)
{
    reset(1, 0);
    assert(run35(0) == 0 && rec.step == 0xd0);

    /* Messages: 0x9dc above the table; kinds 1..6. */
    reset(2, 0);
    assert(run35(0) == 0 && rec.step == 2);
    expect(F_MSG, 0, 2, 0xb0c, 0xb12, 0, 0, 0);
    reset(0x141, 0);
    assert(run35(0) == 0 && count(F_DISP) == 1);
    expect(F_MSG, 0, 2, 0xbc0, 0xbc0, 0, 0, 0);
    reset(0x13f, 0);
    assert(run35(0) == 0x20 && count(F_DISP) == 1);
    reset(0x10a, 5);
    assert(run35(0) == 0 && rec.wait == 0 && count(F_TRAP) == 1);
    expect(F_TRAP, 0, 3, 0x1e, 3, 5, 0, 0);
    reset(0x116, 5);
    assert(run35(0) == 0x800 && rec.wait == 0);
    expect(F_TRAP, 0, 3, 0x1e, 2, 6, 0, 0);
    reset(0x114, 0);
    assert(run35(0) == 0x8000 && count(F_TRAP) == 0);
    reset(0x12b, 0);
    assert(run35(0) == 0x800);
    reset(0x12b, 0);
    msg_ret = 0;
    assert(run35(0) == 0);

    /* Waits of more than 0xc and 6 frames, then a button. */
    reset(0xeb, 0xd);
    assert(run35(0) == 8 && rec.step == 0xec);
    reset(0x13c, 0xd);
    assert(run35(0) == 2);
    reset(3, 0xd);
    assert(run35(0) == 0x80);
    reset(5, 0xc);
    assert(run35(0) == 0 && rec.wait == 0xd);
    assert(run35(0) == 0x40 && rec.step == 6);
    reset(0xf8, 7);
    assert(run35(0) == 0x10);
    reset(0x7e, 7);
    assert(run35(0) == 0x20);

    /* Finger, fades and board set-up. */
    reset(0x10, 0);
    rec.timer = 0x3d;
    assert(run35(0) == 0 && rec.step == 0x11 && count(F_CLOSE) == 1);
    expect(F_FINGER, 0, 4, 0xa, 0x800, -0x9b, -0x55, 0);
    reset(0x5d, 3);
    assert(run35(0) == 0 && rec.step == 0x5e && rec.wait == 0);
    expect(F_DEL, 0, 2, 0, 1, 0, 0, 0);
    expect(F_POWER, 0, 3, 0, 4, 8, 0, 0);
    expect(F_DECK, 0, 1, 0x5d, 0, 0, 0, 0);
    expect(F_CUR, 0, 3, 0, 3, 6, 0, 0);
    reset(0xac, 0);
    assert(run35(0) == 0);
    expect(F_ADD, 1, 5, 0, 4, 4, 0x126, 0);
    expect(F_DECK, 0, 1, 0xac, 0, 0, 0, 0);
    reset(0x111, 0);
    assert(run35(0) == 0 && rec.step == 0x112);
    expect(F_DISP, 0, 1, 0, 0, 0, 0, 0);
    expect(F_FADE, 0, 3, 0x10, -1, 0, 0, 0);
    reset(0x5f, 0);
    assert(run35(0) == 0 && rec.step == 0x60 && count(F_DISP) == 1);

    /* Cursor checks: the low byte of the first parameter is the column, the next the row. */
    reset(0xdf, 0);
    assert(run35(5) == 0x8000 && rec.step == 0xdf);
    assert(run35(3) == 0x2000);
    assert(run35(0x704) == 0x1000);
    assert(run35(0x504) == 0x4000);
    assert(run35(0x10604) == 0 && rec.step == 0xe0);
    reset(0x120, 0);
    assert(run35(1) == 0x8000);
    assert(run35(0x500) == 0x4000);
    assert(run35(0x600) == 0x10 && rec.step == 0x121);
    expect(F_CUR, 0, 3, 0, 0, 6, 0, 0);

    reset(0x109, 0x3d);
    assert(run35(0) == 0 && rec.step == 0x10a);
    reset(0x38, 0);
    assert(run35(0) == 0 && rec.step == 0x39);
    reset(0x142, 0);
    assert(run35(0) == -1 && ntrace == 5);
}

int main(void)
{
    check_prelude(34);
    check_prelude(35);
    check_l034_messages();
    check_l034_boards();
    check_l034_waits();
    check_l035();
    return 0;
}

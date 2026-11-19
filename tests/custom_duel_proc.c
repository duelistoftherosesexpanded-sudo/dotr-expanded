/* Synthetic host tests for the custom_duel_proc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/custom_duel_proc.h"

struct DotrCustomProc dotr_custom_proc_work;
struct DotrCustomProcControl dotr_custom_proc_control;
#define W dotr_custom_proc_work
#define C dotr_custom_proc_control

/* The item grid: a made-up one, different for the two modes, with empty cells (255) and
 * the map item (7) placed so that the skipping loops are exercised. */
unsigned char SzDuel_CustomDuelMenu_customproc[2][2][5] = {
    {{0, 1, 2, 255, 3}, {4, 255, 7, 5, 6}},
    {{0, 6, 255, 255, 2}, {7, 1, 3, 4, 5}},
};

/* Four maps of 7x7 cells behind the 0xfea3 bytes the function skips. */
static unsigned char save[0xfea3 + 4 * 49];

/* The player the frame is run for. */
static int cur;
/* Answers. */
static int busy, current_deck, pad[3], deck_count[3], leader[3], duel_lp[2], duel_charge[2];
static int memcard_result, map_flag;
/* What was called, in call order, as text. */
static char trace[320];
static int trace_n;
static int pad_asks, deck_asks, map_flag_asks, field_calls, field[7][7];

static void note(const char *format, ...)
{
    va_list arguments;

    va_start(arguments, format);
    trace_n += vsnprintf(trace + trace_n, sizeof trace - (size_t)trace_n, format, arguments);
    va_end(arguments);
    assert(trace_n < (int)sizeof trace - 1);
}

int SzData_GetCurrentDeck(int side)
{
    assert(side == cur);
    deck_asks++;
    return current_deck;
}
int SzFile_InqRequestEx(void) { return busy; }
int SzText_LoadMerge(int a, int b) { note("L%d,%d ", a, b); return 0; }
int SzPad_Check(int p, int kind, int mask)
{
    assert(p == cur && (kind == 0 || kind == 2));
    pad_asks++;
    return pad[kind] & mask;
}
int SzSound_PlaySE(int a, int b)
{
    assert(a == 0);
    note("S%d ", b);
    return 0;
}
int SzData_GetDeckCount(int side, int list)
{
    assert(side == cur && list >= 0 && list < 3);
    note("K%d ", list);
    return deck_count[list];
}
int SzData_GetLeader(int side, int deck)
{
    assert(side == cur && deck >= 0 && deck < 3);
    note("R%d ", deck);
    return leader[deck];
}
void SzData_SetCurrentDeck(int side, int deck) { note("D%d,%d ", side, deck); }
void SzText_Free(int a, int b) { note("F%d,%d ", a, b); }
int SzDuel_GetLP(int side)
{
    assert(side == cur);
    return duel_lp[side];
}
int SzDuel_CalcLP(int index, int operation, int value) { note("LP%d,%d,%d ", index, operation, value); return 0; }
int SzDuel_GetSummonCharge(int index)
{
    assert(index == cur);
    return duel_charge[index];
}
void SzDuel_SetSummonCharge(int index, int value) { note("C%d,%d ", index, value); }
int SzGame_ActMemCardProc(void) { note("M "); return memcard_result; }
void *SzData_GetData(int value)
{
    assert(value == cur);
    note("G ");
    return save;
}
int SzDuel_SetField(int x, int y, int value)
{
    /* Row by row, each from column 0 to 6. */
    assert(x >= 0 && x < 7 && y >= 0 && y < 7 && field_calls == y * 7 + x);
    field_calls++;
    field[y][x] = value;
    return 0;
}
int SzGame_SetMenuNum(int index, int value) { note("N%d=%d ", index, value); return 0; }
void SzGame_Push(int a, int b) { note("P%d,%d ", a, b); }
void SzGame_Pop(void) { note("POP "); }
int SzData_GetMapEditFlag(int side)
{
    assert(side == cur);
    map_flag_asks++;
    return map_flag;
}
void SzGame_InitMemCardProc(int kind) { note("I%d ", kind); }

static void reset(void)
{
    memset(&W, 0, sizeof W);
    memset(&C, 0, sizeof C);
    memset(pad, 0, sizeof pad);
    busy = 0;
    current_deck = 0;
    memcard_result = 0;
    map_flag = 0;
    W.count = 10;
    W.maps = 4;
}

static int run(int side)
{
    int r;

    cur = side;
    trace_n = 0;
    trace[0] = 0;
    pad_asks = deck_asks = map_flag_asks = field_calls = 0;
    r = lSzDuel_ActCustomDuelProc(side);
    assert(deck_asks == 1);
    return r;
}

/* One frame at level 1-4 for `side` with the given menu byte and pads; the other player's
 * menu byte is 0x05 before. */
static int frame(int side, int menu, int pad2, int pad0)
{
    W.menu[side] = (unsigned char)menu;
    W.menu[side ^ 1] = 0x05;
    pad[2] = pad2;
    pad[0] = pad0;
    return run(side);
}

static int did(const char *expected)
{
    return strcmp(trace, expected) == 0;
}

static const char *text(const char *format, ...)
{
    static char buffer[160];
    va_list arguments;

    va_start(arguments, format);
    vsnprintf(buffer, sizeof buffer, format, arguments);
    va_end(arguments);
    return buffer;
}

static void check_layout(void)
{
    assert(offsetof(struct DotrCustomProcControl, side[0].deck) == 0x33c);
    assert(offsetof(struct DotrCustomProcControl, side[1].deck) == 0x68c);
    assert(offsetof(struct DotrCustomProcControl, reload) == 0xd32);
    assert(offsetof(struct DotrCustomProc, menu) == 2 && offsetof(struct DotrCustomProc, deck) == 4);
    assert(offsetof(struct DotrCustomProc, charge) == 6 && offsetof(struct DotrCustomProc, f8) == 8);
    assert(offsetof(struct DotrCustomProc, fa) == 0xa && offsetof(struct DotrCustomProc, fc) == 0xc);
    assert(offsetof(struct DotrCustomProc, fe) == 0xe && offsetof(struct DotrCustomProc, lp) == 0x10);
    assert(offsetof(struct DotrCustomProc, lp2) == 0x14 && offsetof(struct DotrCustomProc, list) == 0x18);
    assert(offsetof(struct DotrCustomProc, mode) == 0x88 && offsetof(struct DotrCustomProc, last) == 0x89);
    assert(offsetof(struct DotrCustomProc, count) == 0x8a && offsetof(struct DotrCustomProc, map) == 0x8c);
    assert(offsetof(struct DotrCustomProc, maps) == 0x8d && offsetof(struct DotrCustomProc, yes) == 0x8e);
    assert(offsetof(struct DotrCustomProcSave, map) == 0xfea3);
    /* The host forms of the per-player views reach the fixture's members. */
    assert(&DOTR_CUSTOM_PROC_BYTES_AT(0)->menu == &W.menu[0] && &DOTR_CUSTOM_PROC_BYTES_AT(1)->menu == &W.menu[1]);
    assert(&DOTR_CUSTOM_PROC_BYTES_AT(1)->cur == &W.cur[1] && &DOTR_CUSTOM_PROC_BYTES_AT(1)->deck == &W.deck[1]);
    assert(&DOTR_CUSTOM_PROC_BYTES_AT(1)->charge == &W.charge[1] && &DOTR_CUSTOM_PROC_BYTES_AT(1)->f8 == &W.f8[1]);
    assert(&DOTR_CUSTOM_PROC_BYTES_AT(1)->fa == &W.fa[1] && &DOTR_CUSTOM_PROC_BYTES_AT(1)->fc == &W.fc[1]);
    assert(&DOTR_CUSTOM_PROC_BYTES_AT(1)->fe == &W.fe[1]);
    assert(&DOTR_CUSTOM_PROC_WORDS_AT(0)->lp == &W.lp[0] && &DOTR_CUSTOM_PROC_WORDS_AT(1)->lp == &W.lp[1]);
    assert(&DOTR_CUSTOM_PROC_WORDS_AT(0)->lp2 == &W.lp2[0] && &DOTR_CUSTOM_PROC_WORDS_AT(1)->lp2 == &W.lp2[1]);
    assert((void *)data_00317490_customproc == (void *)&C);
}

static void check_reload(int side)
{
    int o = side ^ 1;

    reset();
    C.reload[0] = C.reload[1] = 1;
    W.mode = 1;
    assert(frame(side, 0x20, 0, 0) == 0 && did(text("L%d,1 L6,1 ", side + 5)));
    assert(C.reload[side] == 0 && C.reload[o] == 1);
    assert(run(side) == 0 && did(""));
    C.reload[side] = 1;
    W.mode = 0;
    assert(run(side) == 0 && did(text("L%d,1 ", side + 5)) && C.reload[side] == 0);
    C.reload[side] = 7;
    W.mode = 2;
    assert(run(side) == 0 && did(text("L%d,1 ", side + 5)) && C.reload[side] == 0);
    /* Not while a file request is pending. */
    C.reload[side] = 1;
    busy = 1;
    W.mode = 1;
    assert(run(side) == 0 && did("") && C.reload[side] == 1);
    busy = 0;
}

static void check_deck_item(int side)
{
    int o = side ^ 1;

    reset();
    /* Direction: 0x8000 or 4 one way, any other of 0xa000 | 12 the other way; always a sound. */
    W.deck[side] = 0; assert(frame(side, 0x10, 0x8000, 0) == 0 && did("S3 ") && W.deck[side] == 2);
    W.deck[side] = 1; assert(frame(side, 0x10, 4, 0) == 0 && did("S3 ") && W.deck[side] == 0);
    W.deck[side] = 2; assert(frame(side, 0x10, 0x2000, 0) == 0 && did("S3 ") && W.deck[side] == 0);
    W.deck[side] = 0; assert(frame(side, 0x10, 8, 0) == 0 && did("S3 ") && W.deck[side] == 1);
    /* A direction comes before the buttons; other repeat bits are not directions. */
    W.deck[side] = 1; assert(frame(side, 0x10, 8, 0xe0) == 0 && did("S3 ") && W.deck[side] == 2);
    assert(W.deck[o] == 0 && W.menu[side] == 0x10 && W.menu[o] == 0x05);
    assert(frame(side, 0x10, 0x5003, 0) == 0 && did("") && W.deck[side] == 2);

    /* Confirm: the deck must be ready (40 cards or more and a leader other than 999). */
    W.deck[side] = 2;
    current_deck = 0;
    deck_count[2] = 39; leader[2] = 5;
    assert(frame(side, 0x10, 0, 0x40) == 0 && did("K2 R2 S6 ") && C.reload[side] == 0);
    deck_count[2] = 40; leader[2] = 999;
    assert(frame(side, 0x10, 0, 0x80) == 0 && did("K2 R2 S6 "));
    deck_count[2] = 0; leader[2] = 999;
    assert(frame(side, 0x10, 0, 0x80) == 0 && did("K2 R2 S6 "));
    deck_count[2] = 40; leader[2] = 998;
    assert(frame(side, 0x10, 0, 0x80) == 0 && did(text("K2 R2 S4 D%d,2 F2,%d ", side, side)));
    assert(C.reload[side] == 1 && C.reload[o] == 0 && W.menu[side] == 0x10);
    C.reload[side] = 0;
    busy = 1;       /* keeps the reload byte for the check below */
    W.mode = 1;
    assert(frame(side, 0x10, 0, 0x40) == 0 && did(text("K2 R2 S4 D%d,2 F2,%d D1,2 F2,1 ", side, side)) && C.reload[side] == 1);
    W.mode = 2;
    assert(frame(side, 0x10, 0, 0x40) == 0 && did(text("K2 R2 S4 D%d,2 F2,%d ", side, side)));
    busy = 0;
    C.reload[side] = 0;
    /* Ready but already the current deck: nothing. */
    current_deck = 2;
    assert(frame(side, 0x10, 0, 0x40) == 0 && did("K2 R2 ") && C.reload[side] == 0);
    /* Confirm comes before cancel. */
    assert(frame(side, 0x10, 0, 0x60) == 0 && did("K2 R2 ") && W.menu[side] == 0x10);
    assert(frame(side, 0x10, 0, 0x20) == 0 && did("S5 ") && W.menu[side] == 0x00 && W.menu[o] == 0x05);
    assert(frame(side, 0x10, 0, 0x10) == 0 && did("") && W.menu[side] == 0x10);
}

/* One life-point step: from `from` with the repeat mask; returns the new value. */
static int lp_step(int side, int from, int mask, const char *sound)
{
    W.lp[side] = (short)from;
    assert(frame(side, 0x11, mask, 0xe0) == 0 && did(sound) && W.menu[side] == 0x11);
    return W.lp[side];
}

static void check_lp_item(int side)
{
    int o = side ^ 1;

    reset();
    W.lp[o] = 1234;
    assert(lp_step(side, 4000, 0x2000, "S3 ") == 4100 && lp_step(side, 4000, 8, "S3 ") == 4100);
    assert(lp_step(side, 4000, 0x8000, "S3 ") == 3900 && lp_step(side, 4000, 4, "S3 ") == 3900);
    /* Rounded to the nearest hundred first. */
    assert(lp_step(side, 4049, 0x2000, "S3 ") == 4100 && lp_step(side, 4050, 0x2000, "S3 ") == 4200);
    assert(lp_step(side, 4049, 0x8000, "S3 ") == 3900 && lp_step(side, 4050, 0x8000, "S3 ") == 4000);
    /* Kept in 1-9999; the refusal sound when nothing changed. */
    assert(lp_step(side, 100, 0x8000, "S3 ") == 1 && lp_step(side, 1, 0x8000, "S6 ") == 1);
    assert(lp_step(side, 1, 0x2000, "S3 ") == 100);
    assert(lp_step(side, 9900, 0x2000, "S3 ") == 9999 && lp_step(side, 9999, 0x2000, "S6 ") == 9999);
    assert(lp_step(side, 9999, 0x8000, "S3 ") == 9900);
    assert(W.lp[o] == 1234 && W.lp2[0] == 0 && W.lp2[1] == 0);

    /* Confirm: only when it differs from the duel's value. */
    W.lp[side] = 2500;
    duel_lp[side] = 2500;
    duel_lp[o] = 1;
    assert(frame(side, 0x11, 0, 0xc0) == 0 && did("") && W.lp2[side] == 0);
    duel_lp[side] = 2501;
    assert(frame(side, 0x11, 0, 0x40) == 0 && did(text("S4 LP%d,4,2500 ", side)));
    assert(W.lp2[side] == 2500 && W.lp2[o] == 0 && W.menu[side] == 0x11);
    /* With a mode set the other player gets the same value, but not below 4000. */
    W.mode = 1;
    W.lp2[side] = 0;
    assert(frame(side, 0x11, 0, 0x80) == 0 && did(text("S4 LP%d,4,2500 LP%d,4,4000 ", side, o)));
    assert(W.lp2[side] == 2500 && W.lp2[o] == 4000 && W.lp[o] == 1234 && W.lp[side] == 2500);
    W.mode = 2;
    W.lp[side] = 4001;
    assert(frame(side, 0x11, 0, 0x80) == 0 && did(text("S4 LP%d,4,4001 LP%d,4,4001 ", side, o)));
    assert(W.lp2[side] == 4001 && W.lp2[o] == 4001);
    W.lp[side] = 4000;
    assert(frame(side, 0x11, 0, 0x80) == 0 && did(text("S4 LP%d,4,4000 LP%d,4,4000 ", side, o)) && W.lp2[o] == 4000);
    assert(W.menu[side] == 0x11 && W.menu[o] == 0x05);

    assert(frame(side, 0x11, 0, 0x20) == 0 && did("S5 ") && W.menu[side] == 0x01 && W.menu[o] == 0x05);
}

static int charge_step(int side, int from, int mask, const char *sound)
{
    W.charge[side] = (unsigned char)from;
    assert(frame(side, 0x12, mask, 0xe0) == 0 && did(sound) && W.menu[side] == 0x12);
    return W.charge[side];
}

static void check_charge_item(int side)
{
    int o = side ^ 1;

    reset();
    W.charge[o] = 9;
    assert(charge_step(side, 2, 0x8000, "S3 ") == 1 && charge_step(side, 1, 4, "S6 ") == 1);
    assert(charge_step(side, 11, 0x2000, "S3 ") == 12 && charge_step(side, 12, 8, "S6 ") == 12);
    assert(charge_step(side, 5, 4, "S3 ") == 4 && charge_step(side, 5, 8, "S3 ") == 6);
    assert(W.charge[o] == 9);

    W.charge[side] = 2;
    duel_charge[side] = 2;
    duel_charge[o] = 0;
    assert(frame(side, 0x12, 0, 0x40) == 0 && did(""));
    duel_charge[side] = 3;
    assert(frame(side, 0x12, 0, 0x40) == 0 && did(text("S4 C%d,2 ", side)));
    /* With a mode set the other player gets it too, but not below 3. */
    W.mode = 1;
    assert(frame(side, 0x12, 0, 0x80) == 0 && did(text("S4 C%d,2 C%d,3 ", side, o)));
    W.charge[side] = 3;
    duel_charge[side] = 0;
    assert(frame(side, 0x12, 0, 0x80) == 0 && did(text("S4 C%d,3 C%d,3 ", side, o)));
    W.charge[side] = 12;
    assert(frame(side, 0x12, 0, 0x80) == 0 && did(text("S4 C%d,12 C%d,12 ", side, o)));
    assert(W.charge[o] == 9 && W.menu[side] == 0x12);

    assert(frame(side, 0x12, 0, 0x20) == 0 && did("S5 ") && W.menu[side] == 0x02 && W.menu[o] == 0x05);
}

/* The two on/off items: `edit` is the edited byte, `kept` the accepted one. */
static void check_switch_item(int side, int item, unsigned char *edit, unsigned char *kept)
{
    int o = side ^ 1;
    int menu = 0x10 | item;

    reset();
    edit[o] = 5;
    kept[o] = 6;
    /* 0x8000 turns it off, the other direction on; a sound only when it changed. */
    edit[side] = 1; assert(frame(side, menu, 0x8000, 0xe0) == 0 && did("S3 ") && edit[side] == 0);
    edit[side] = 0; assert(frame(side, menu, 0x8000, 0) == 0 && did("") && edit[side] == 0);
    edit[side] = 0; assert(frame(side, menu, 0x2000, 0) == 0 && did("S3 ") && edit[side] == 1);
    edit[side] = 1; assert(frame(side, menu, 0x2000, 0) == 0 && did("") && edit[side] == 1);
    edit[side] = 0; assert(frame(side, menu, 0xa000, 0) == 0 && did("") && edit[side] == 0);
    /* The other pair of direction bits is not used by these items. */
    edit[side] = 1; assert(frame(side, menu, 12, 0) == 0 && did("") && edit[side] == 1);

    kept[side] = 1;
    assert(frame(side, menu, 0, 0x40) == 0 && did("") && kept[side] == 1);
    kept[side] = 0;
    assert(frame(side, menu, 12, 0x80) == 0 && did("S4 ") && kept[side] == 1 && W.menu[side] == menu);
    edit[side] = 0;
    kept[side] = 1;
    assert(frame(side, menu, 0, 0xc0) == 0 && did("S4 ") && kept[side] == 0);
    assert(edit[o] == 5 && kept[o] == 6);

    assert(frame(side, menu, 0, 0x20) == 0 && did("S5 ") && W.menu[side] == item && W.menu[o] == 0x05);
}

static void check_other_items(int side)
{
    int o = side ^ 1;
    int x, y, n;

    reset();
    /* Item 5 leaves its level at once, without asking the pad. */
    assert(frame(side, 0x15, 0xffff, 0xffff) == 0 && did("") && W.menu[side] == 0x05 && pad_asks == 0);

    /* Item 6 runs the memory-card dialogue until it reports idle. */
    memcard_result = 0;
    assert(frame(side, 0x16, 0xffff, 0xffff) == 0 && did("M ") && W.menu[side] == 0x16 && pad_asks == 0);
    memcard_result = 2;
    assert(frame(side, 0x16, 0, 0) == 0 && did("M ") && W.menu[side] == 0x16);
    memcard_result = -1;
    assert(frame(side, 0x16, 0, 0) == 0 && did("M ") && W.menu[side] == 0x06 && W.menu[o] == 0x05);

    /* Item 7: the map number goes round. */
    W.maps = 4;
    W.map = 0; assert(frame(side, 0x17, 0x1000, 0xe0) == 0 && did("S3 ") && W.map == 3);
    W.map = 3; assert(frame(side, 0x17, 0x4000, 0) == 0 && did("S3 ") && W.map == 0);
    W.map = 1; assert(frame(side, 0x17, 0x4000, 0) == 0 && did("S3 ") && W.map == 2);
    W.map = 2; assert(frame(side, 0x17, 0x5000, 0) == 0 && did("S3 ") && W.map == 1);
    assert(frame(side, 0x17, 0xa00c, 0) == 0 && did("") && W.map == 1);
    /* Confirm copies map (map + 2) % maps of the player's save data to the field. */
    for (n = 0; n < 4; n++) {
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                save[0xfea3 + n * 49 + y * 7 + x] = (unsigned char)(n * 60 + y * 7 + x);
            }
        }
    }
    save[0xfea2] = 0xee;
    for (n = 0; n < 4; n++) {
        int from = (n + 2) % 4;

        W.map = (unsigned char)n;
        memset(field, 0xff, sizeof field);
        assert(frame(side, 0x17, 0, 0x40) == 0 && did("G S4 N13=0 P13,2 ") && field_calls == 49);
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                assert(field[y][x] == from * 60 + y * 7 + x);
            }
        }
        assert(W.map == n && W.menu[side] == 0x17);
    }
    assert(frame(side, 0x17, 0, 0x20) == 0 && did("S5 ") && W.menu[side] == 0x07 && field_calls == 0);

    /* Items 8-15 have no editor; levels 5-15 do nothing. */
    assert(frame(side, 0x18, 0xffff, 0xffff) == 0 && did("") && W.menu[side] == 0x18 && pad_asks == 0);
    assert(frame(side, 0x1f, 0xffff, 0xffff) == 0 && did("") && pad_asks == 0);
    assert(frame(side, 0x50, 0xffff, 0xffff) == 0 && did("") && W.menu[side] == 0x50 && pad_asks == 0);
    assert(frame(side, 0xf3, 0xffff, 0xffff) == 0 && did("") && pad_asks == 0);
    assert(W.menu[o] == 0x05);
}

static void check_wait_and_quit(int side)
{
    int o = side ^ 1;

    reset();
    /* Level 2: waiting; only cancel does anything. */
    assert(frame(side, 0x23, 0xffff, 0xffdf) == 0 && did("") && W.menu[side] == 0x23);
    assert(frame(side, 0x23, 0, 0x20) == 0 && did("S5 ") && W.menu[side] == 0x03 && W.menu[o] == 0x05);

    /* Level 3: the quit box. 0x8000 moves to yes, the other direction to no. */
    W.yes = 0; assert(frame(side, 0x32, 0, 0x8000 | 0xe0) == 0 && did("S3 ") && W.yes == 1);
    W.yes = 1; assert(frame(side, 0x32, 0, 0x8000) == 0 && did("") && W.yes == 1);
    W.yes = 1; assert(frame(side, 0x32, 0, 0x2000) == 0 && did("S3 ") && W.yes == 0);
    W.yes = 0; assert(frame(side, 0x32, 0, 0x2000) == 0 && did("") && W.yes == 0);
    /* The repeat way of asking is not used here. */
    W.yes = 0; assert(frame(side, 0x32, 0xa000, 0) == 0 && did("") && W.yes == 0);
    assert(W.menu[side] == 0x32);
    /* Confirm on yes leaves the screen; on no both players go back to browsing. */
    W.yes = 1;
    assert(frame(side, 0x32, 0, 0x40) == 0 && did("S4 POP ") && W.menu[side] == 0x32 && W.menu[o] == 0x05);
    W.yes = 0;
    W.menu[o] = 0x35;
    W.menu[side] = 0x32;
    pad[2] = 0;
    pad[0] = 0x80;
    assert(run(side) == 0 && did("S4 ") && W.menu[side] == 0x02 && W.menu[o] == 0x05);
    W.yes = 1;
    W.menu[o] = 0x35;
    W.menu[side] = 0x32;
    pad[0] = 0x20;
    assert(run(side) == 0 && did("S5 ") && W.menu[side] == 0x02 && W.menu[o] == 0x05 && W.yes == 1);
}

static int scroll(int side, int from, int pad2)
{
    W.last = (unsigned char)from;
    assert(frame(side, 0x40, pad2, 0xe0) == 0 && did("S3 ") && W.menu[side] == 0x40);
    return W.last;
}

static void check_list(int side)
{
    static const int entry[5][2] = {{0x0003, 49}, {0x0103, 54}, {0x0203, 25}, {0x05fe, 276}, {0x01ff, 306}};
    int o = side ^ 1;
    int i;

    reset();
    W.count = 10;
    /* By five with 4 / 8, by one with 0x1000 / 0x4000; the list goes round. */
    assert(scroll(side, 7, 4) == 2 && scroll(side, 2, 4) == 7 && scroll(side, 7, 8) == 2 && scroll(side, 3, 8) == 8);
    assert(scroll(side, 0, 0x1000) == 9 && scroll(side, 9, 0x4000) == 0 && scroll(side, 4, 0x4000) == 5);
    assert(scroll(side, 4, 0x1000) == 3);
    /* The step of five comes first. */
    assert(scroll(side, 4, 4 | 0x4000) == 9 && scroll(side, 4, 12) == 9 && scroll(side, 4, 0x5000) == 3);
    W.count = 7;
    assert(scroll(side, 0, 8) == 5 && scroll(side, 0, 4) == 2 && scroll(side, 6, 0x4000) == 0);

    /* Confirm: entry (last + 2) % count; class below 2 gives number + class * 5 + 46,
     * otherwise number + 22. The only frame that returns 1. */
    W.count = 10;
    C.side[0].deck = 2;
    C.side[1].deck = 1;
    for (i = 0; i < 5; i++) {
        W.last = (unsigned char)(i == 4 ? 9 : 3);
        W.list[(W.last + 2) % 10] = (unsigned short)entry[i][0];
        assert(frame(side, 0x40, 0, 0x40) == 1);
        assert(did(text("N2=0 N1=%d N3=1002 N4=1001 N28=0 N29=0 P1,1 S4 ", entry[i][1])));
        assert(W.menu[side] == 0x40 && W.menu[o] == 0x05 && W.last == (i == 4 ? 9 : 3));
    }
    W.last = 3;
    W.list[5] = 0x0003;
    W.f8[0] = 1; W.fc[1] = 9;
    assert(frame(side, 0x40, 0, 0x80) == 1 && did("N2=0 N1=49 N3=1002 N4=1001 N28=1 N29=2 P1,1 S4 "));
    W.f8[0] = 0; W.f8[1] = 2; W.fc[0] = 1; W.fc[1] = 0;
    assert(frame(side, 0x40, 0, 0x80) == 1 && did("N2=0 N1=49 N3=1002 N4=1001 N28=2 N29=1 P1,1 S4 "));
    W.f8[0] = 1; W.fc[1] = 1;
    assert(frame(side, 0x40, 0, 0xe0) == 1 && did("N2=0 N1=49 N3=1002 N4=1001 N28=3 N29=3 P1,1 S4 "));

    /* Cancel: both back to browsing; in mode 0 the other player is left waiting. */
    W.mode = 0;
    W.menu[side] = 0x43;
    W.menu[o] = 0x44;
    pad[2] = 0;
    pad[0] = 0x20;
    assert(run(side) == 0 && did("S5 ") && W.menu[side] == 0x03 && W.menu[o] == 0x24);
    W.mode = 1;
    W.menu[side] = 0x43;
    W.menu[o] = 0x44;
    assert(run(side) == 0 && did("S5 ") && W.menu[side] == 0x03 && W.menu[o] == 0x04);
    /* Nothing pressed. */
    assert(frame(side, 0x40, 0xa003, 0x10) == 0 && did("") && W.menu[side] == 0x40);
}

/* A cursor move at level 0 from cell `from`; returns the new cell, -1 when it did not move. */
static int move(int side, int from, int pad2)
{
    int mode_index = W.mode ? 1 : 0;
    int before = SzDuel_CustomDuelMenu_customproc[mode_index][from / 5][from % 5];
    int o = side ^ 1;

    W.cur[side] = (unsigned char)from;
    W.cur[o] = 3;
    W.menu[side] = (unsigned char)before;
    W.menu[o] = 0x05;
    pad[2] = pad2;
    pad[0] = 0xffff;        /* a direction comes before every button */
    assert(run(side) == 0 && W.cur[o] == 3 && W.menu[o] == 0x05);
    if (did("")) {
        assert(W.cur[side] == from && W.menu[side] == before);
        return -1;
    }
    assert(did("S3 "));
    assert(W.menu[side] == SzDuel_CustomDuelMenu_customproc[mode_index][W.cur[side] / 5][W.cur[side] % 5]);
    return W.cur[side];
}

static void check_browse(int side)
{
    int o = side ^ 1;
    int item;

    reset();
    /* Mode 0 grid: {0, 1, 2, -, 3} over {4, -, 7, 5, 6}. */
    assert(move(side, 0, 0x4000) == 1 && move(side, 1, 0x1000) == 0);
    assert(move(side, 2, 0x4000) == 4 && move(side, 4, 0x1000) == 2);          /* over the empty cell */
    assert(move(side, 4, 0x4000) == 0 && move(side, 0, 0x1000) == 4);          /* round the row */
    assert(move(side, 0, 0x8000) == 5 && move(side, 5, 0x2000) == 0);
    assert(move(side, 5, 0x8000) == 0 && move(side, 0, 0x2000) == 5);
    assert(move(side, 4, 0x8000) == 9 && move(side, 9, 0x2000) == 4);
    /* Down onto an empty cell comes back to where it was: no move, no sound. */
    assert(move(side, 1, 0x8000) == -1 && move(side, 1, 0x2000) == -1);
    assert(move(side, 8, 0x8000) == -1);
    /* Left before right before down before up. */
    assert(move(side, 0, 0xf000) == 4 && move(side, 0, 0xe000) == 1 && move(side, 0, 0xa000) == 5);
    /* The map item is skipped without the map-edit flag. */
    map_flag = 0;
    assert(move(side, 5, 0x4000) == 8 && map_flag_asks == 1);
    assert(move(side, 8, 0x1000) == 5 && map_flag_asks == 1);
    assert(move(side, 2, 0x8000) == -1 && map_flag_asks == 1);
    map_flag = 1;
    assert(move(side, 5, 0x4000) == 7 && map_flag_asks == 1);
    assert(move(side, 8, 0x1000) == 7 && move(side, 2, 0x8000) == 7 && move(side, 7, 0x2000) == 2);
    assert(move(side, 7, 0x4000) == 8 && move(side, 7, 0x1000) == 5);
    /* Any non-zero mode uses the second grid: {0, 6, -, -, 2} over {7, 1, 3, 4, 5}. */
    W.mode = 1;
    assert(move(side, 0, 0x4000) == 1 && move(side, 1, 0x4000) == 4 && move(side, 4, 0x8000) == 9);
    assert(move(side, 0, 0x8000) == 5 && move(side, 6, 0x1000) == 5);
    W.mode = 2;
    map_flag = 0;
    assert(move(side, 1, 0x4000) == 4 && move(side, 6, 0x1000) == 9 && move(side, 0, 0x8000) == -1);
    W.mode = 0;
    /* Bits below 0xf000 are not directions here. */
    W.cur[side] = 0;
    assert(frame(side, 0x00, 0x0fff, 0) == 0 && did("") && W.cur[side] == 0);

    /* Start (2048). The other player waiting: both go to the list. */
    reset();
    W.menu[side] = 0x03;
    W.menu[o] = 0x24;
    pad[0] = 2048 | 0xe0;
    assert(run(side) == 0 && did("S4 ") && W.menu[side] == 0x43 && W.menu[o] == 0x44);
    /* Not waiting, mode 0: this player waits. */
    W.menu[side] = 0x03;
    W.menu[o] = 0x04;
    assert(run(side) == 0 && did("S4 ") && W.menu[side] == 0x23 && W.menu[o] == 0x04);
    W.menu[side] = 0x03;
    W.menu[o] = 0x14;
    assert(run(side) == 0 && did("S4 ") && W.menu[side] == 0x23 && W.menu[o] == 0x14);
    /* Not waiting, a mode set: both go to the list. */
    W.mode = 1;
    W.menu[side] = 0x03;
    W.menu[o] = 0x04;
    assert(run(side) == 0 && did("S4 ") && W.menu[side] == 0x43 && W.menu[o] == 0x44);
    W.mode = 0;

    /* Confirm opens the item: 5 pushes a submenu, 6 starts the memory-card dialogue. */
    for (item = 0; item < 8; item++) {
        const char *expected = item == 5 ? "S4 N6=0 P6,1 " : item == 6 ? "S4 I1 " : "S4 ";

        assert(frame(side, item, 0, 0x40 | 0x20) == 0 && did(expected) && W.menu[side] == (0x10 | item));
        assert(frame(side, item, 0, 0x80) == 0 && did(expected) && W.menu[o] == 0x05);
    }

    /* Cancel: the quit box for both, unless the other player is past browsing. */
    W.yes = 1;
    W.menu[side] = 0x02;
    W.menu[o] = 0x07;
    pad[2] = 0;
    pad[0] = 0x20;
    assert(run(side) == 0 && did("S5 ") && W.menu[side] == 0x32 && W.menu[o] == 0x37 && W.yes == 0);
    W.yes = 1;
    W.menu[side] = 0x02;
    W.menu[o] = 0x08;
    assert(run(side) == 0 && did("S6 ") && W.menu[side] == 0x02 && W.menu[o] == 0x08 && W.yes == 1);
    W.menu[o] = 0x10;
    assert(run(side) == 0 && did("S6 ") && W.menu[side] == 0x02 && W.menu[o] == 0x10);

    /* Nothing pressed; and the level-0 values 8-15 are not items. */
    assert(frame(side, 0x04, 0, 0x10) == 0 && did("") && W.menu[side] == 0x04);
    assert(frame(side, 0x08, 0xffff, 0xffff) == 0 && did("") && W.menu[side] == 0x08 && pad_asks == 0);
    assert(frame(side, 0x0f, 0xffff, 0xffff) == 0 && did("") && pad_asks == 0);
}

int main(void)
{
    int side;

    check_layout();
    for (side = 0; side < 2; side++) {
        check_reload(side);
        check_deck_item(side);
        check_lp_item(side);
        check_charge_item(side);
        check_switch_item(side, 3, W.f8, W.fa);
        check_switch_item(side, 4, W.fc, W.fe);
        check_other_items(side);
        check_wait_and_quit(side);
        check_list(side);
        check_browse(side);
    }
    puts("custom_duel_proc: ok");
    return 0;
}

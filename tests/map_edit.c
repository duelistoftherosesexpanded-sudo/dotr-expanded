/* Synthetic host tests for the map_edit group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/map_edit.h"

struct DotrMapEditState dotr_map_edit_00a9c000;
struct DotrTintQuad data_002914d0 = { 0x11, 0x22, 0x33, 0x44 };
unsigned short data_00318116;

/* Recording stubs: every call is logged with its integer arguments in order. */
struct Call {
    char kind;
    int a[9];
};
static struct Call calls[96];
static int call_count;
static struct DotrSpriteEntry entry;
static float piece_float;
static float ratio_x;
static float ratio_y;
static float round_input;
static unsigned char tint_seen[4];

static DotrSpriteWord record(char kind, int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    struct Call *call = &calls[call_count++];
    assert(call_count <= 96);
    call->kind = kind;
    call->a[0] = a0; call->a[1] = a1; call->a[2] = a2; call->a[3] = a3; call->a[4] = a4;
    call->a[5] = a5; call->a[6] = a6; call->a[7] = a7; call->a[8] = a8;
    return 0x1000 * call_count;
}

static void expect(int n, char kind, int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    struct Call *call = &calls[n];
    assert(n < call_count && call->kind == kind);
    assert(call->a[0] == a0 && call->a[1] == a1 && call->a[2] == a2 && call->a[3] == a3 && call->a[4] == a4);
    assert(call->a[5] == a5 && call->a[6] == a6 && call->a[7] == a7 && call->a[8] == a8);
}

void SzDuel_InitMapColor2(int a, int b, int c, int d) { record('I', a, b, c, d, 0, 0, 0, 0, 0); }
int SzDuel_SetMapColor2(int x, int y, int c, struct DotrTintQuad *colour, int e, int f)
{
    memcpy(tint_seen, colour, 4);
    record('S', x, y, c, e, f, 0, 0, 0, 0);
    return 0;
}
int SzDuel_CalcMapColor(int x, int y) { record('C', x, y, 0, 0, 0, 0, 0, 0, 0); return 0; }
void SzDuel_SetWindowDefPrioZ(int value) { record('Z', value, 0, 0, 0, 0, 0, 0, 0, 0); }
static int field_mode;
int SzDuel_GetField(int x, int y) { record('G', x, y, 0, 0, 0, 0, 0, 0, 0); return field_mode ? x + 10 * y : 9; }
DotrSpriteWord lSzDuel_DrawWindow_FieldPiece_Pipe(DotrSpriteWord cursor, int a1, int field, float f, int a3, int a4)
{
    piece_float = f;
    return record('P', cursor, a1, field, a3, a4, 0, 0, 0, 0);
}
DotrSpriteWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrSpriteWord cursor, int a1)
{
    return record('W', cursor, a1, 0, 0, 0, 0, 0, 0, 0);
}
DotrSpriteWord SzMsg_LoadFont_Pipe(DotrSpriteWord cursor, int a0, int a1, int a2, int a3)
{
    return record('L', cursor, a0, a1, a2, a3, 0, 0, 0, 0);
}
DotrSpriteWord RhText_SetTextureFlush(DotrSpriteWord cursor) { return record('T', cursor, 0, 0, 0, 0, 0, 0, 0, 0); }
DotrSpriteWord lSzDuel_DrawWindow_Frame_Pipe(DotrSpriteWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8)
{
    return record('F', cursor, a1, a2, a3, x, y, a6, a7, (int)a8);
}
struct DotrSpriteEntry *SzMsg_GetInfo(int index) { record('N', index, 0, 0, 0, 0, 0, 0, 0, 0); return &entry; }
int func_001a22c0(float value) { round_input = value; return (int)value; }
void SzMsg_DrawRect_Pipe(void) {}
DotrSpriteWord SzMsg_DrawRatioX_Pipe(DotrSpriteWord cursor, int index, int x, int y, void *e, void (*function)(void),
                                     float sx, float sy)
{
    assert(e == NULL && function == SzMsg_DrawRect_Pipe);
    ratio_x = sx;
    ratio_y = sy;
    return record('R', cursor, index, x, y, 0, 0, 0, 0, 0);
}
void SzMsg_Free(int index) { record('X', index, 0, 0, 0, 0, 0, 0, 0, 0); }
DotrSpriteWord lSzDuel_DrawWindow_CommandGuide_Pipe(DotrSpriteWord cursor, int a1, int a2, int a3)
{
    return record('M', cursor, a1, a2, a3, 0, 0, 0, 0, 0);
}
DotrSpriteWord lSzDuel_DrawMapEdit_Graph_Pipe(DotrSpriteWord cursor, int a1, int a2)
{
    return record('H', cursor, a1, a2, 0, 0, 0, 0, 0, 0);
}

/* 0x1ccbb0: branch coverage. The row/column test is checked for every cell (cx, cy on the
 * cursor's row or column, both, neither); the width test at 84 and 85. */
static void check_draw(int cx, int cy, int width, float ratio)
{
    int n = 0;
    int x;
    int y;
    int c;
    DotrSpriteWord result;

    call_count = 0;
    dotr_map_edit_00a9c000.unknown_8f = (unsigned char)cx;
    dotr_map_edit_00a9c000.unknown_90 = (unsigned char)cy;
    data_00318116 = 0xc7ff;
    entry.unknown_0c = width;
    result = SzDuel_DrawMapEdit_Pipe(0x77);
    expect(n++, 'I', 0, 0, 0, 0, 0, 0, 0, 0, 0);
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            if (x == cx || y == cy) {
                expect(n++, 'S', x, y, 1, 0, 0, 0, 0, 0, 0);
            }
        }
    }
    expect(n++, 'S', cx, cy, 1, 0, 0, 0, 0, 0, 0);
    /* The copied colour with its last byte replaced by the high byte + 64, truncated to a byte. */
    assert(tint_seen[0] == 0x11 && tint_seen[1] == 0x22 && tint_seen[2] == 0x33 && tint_seen[3] == 0x07);
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            expect(n++, 'C', x, y, 0, 0, 0, 0, 0, 0, 0);
        }
    }
    expect(n++, 'Z', 0xfffff8, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(n++, 'G', cx, cy, 0, 0, 0, 0, 0, 0, 0);
    expect(n++, 'P', 0x77, 0, 9, 6, 336, 0, 0, 0, 0);
    assert(piece_float == 100.0f);
    c = 0x1000 * n;
    expect(n++, 'W', c, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(n++, 'G', cx, cy, 0, 0, 0, 0, 0, 0, 0);
    expect(n++, 'L', c + 0x1000, 1, 9 + 71, 0, 22, 0, 0, 0, 0);
    c = 0x1000 * n;
    expect(n++, 'T', c, 0, 0, 0, 0, 0, 0, 0, 0);
    c = 0x1000 * n;
    expect(n++, 'F', c, -1, 2, 3, 6, 336, 9, 18, 1);
    c = 0x1000 * n;
    expect(n++, 'F', c, 5, -1, 3, 6, 400, 9, 2, 1);
    c = 0x1000 * n;
    expect(n++, 'N', 1, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(n++, 'R', c, 1, 54 - (int)((float)(width >> 1) * ratio), 403, 0, 0, 0, 0, 0);
    assert(ratio_x == ratio && ratio_y == 1.0f && round_input == (float)(width >> 1) * ratio);
    c = 0x1000 * n;
    expect(n++, 'X', 1, 0, 0, 0, 0, 0, 0, 0, 0);
    expect(n++, 'M', c, 0, 538, 336, 0, 0, 0, 0, 0);
    c = 0x1000 * n;
    expect(n++, 'H', c, 6, 144, 0, 0, 0, 0, 0, 0);
    c = 0x1000 * n;
    expect(n++, 'X', 0, 0, 0, 0, 0, 0, 0, 0, 0);
    assert(n == call_count && result == c);
}

/* 0x1cc620 stubs. SzPad_Check answers the bits set for its second argument, masked. */
struct DotrControlDrift data_00317490_drift;
static struct DotrMapEditGrids grids;
static int pad_bits[3];
int SzPad_Check(int unknown, int zero, int mask)
{
    record('p', unknown, zero, mask, 0, 0, 0, 0, 0, 0);
    assert(zero >= 0 && zero < 3);
    return pad_bits[zero] & mask;
}
void *SzData_GetData(int value) { record('D', value, 0, 0, 0, 0, 0, 0, 0, 0); return &grids; }
int SzDuel_SetField(int x, int y, int value) { record('F', x, y, value, 0, 0, 0, 0, 0, 0); return 0; }
int SzSound_PlaySE(int index, int value) { record('E', index, value, 0, 0, 0, 0, 0, 0, 0); return 0; }
void SzGame_Pop(void) { record('O', 0, 0, 0, 0, 0, 0, 0, 0, 0); }

static void act_reset(int x, int y, int wait, int flag)
{
    memset(&data_00317490_drift, 0, sizeof(data_00317490_drift));
    memset(&dotr_map_edit_00a9c000, 0, sizeof(dotr_map_edit_00a9c000));
    memset(pad_bits, 0, sizeof(pad_bits));
    data_00317490_drift.unknown_c86 = 0x1000;
    data_00317490_drift.unknown_c88 = 0x1000;
    dotr_map_edit_00a9c000.unknown_8c = 4;
    dotr_map_edit_00a9c000.unknown_8d = 5;
    dotr_map_edit_00a9c000.unknown_8f = (unsigned char)x;
    dotr_map_edit_00a9c000.unknown_90 = (unsigned char)y;
    dotr_map_edit_00a9c000.unknown_91 = (unsigned char)wait;
    dotr_map_edit_00a9c000.unknown_92_bit_0 = (unsigned char)flag;
    call_count = 0;
    field_mode = 0;
}

/* One cursor step: pad bits in, expected x, y, flag, and whether the step sound and wait happen. */
static void act_step(int pad, int x, int y, int flag, int want_x, int want_y, int want_flag, int moved)
{
    act_reset(x, y, 0, flag);
    pad_bits[1] = pad;
    SzDuel_ActMapEdit();
    expect(0, 'p', 0, 0, 0x8c0, 0, 0, 0, 0, 0, 0);
    expect(1, 'p', 0, 0, 0x20, 0, 0, 0, 0, 0, 0);
    expect(2, 'p', 0, 2, 0xc, 0, 0, 0, 0, 0, 0);
    expect(3, 'p', 0, 1, 0xffff, 0, 0, 0, 0, 0, 0);
    assert(dotr_map_edit_00a9c000.unknown_8f == want_x && dotr_map_edit_00a9c000.unknown_90 == want_y);
    assert(dotr_map_edit_00a9c000.unknown_92_bit_0 == want_flag);
    if (moved) {
        expect(4, 'E', 0, 3, 0, 0, 0, 0, 0, 0, 0);
        assert(call_count == 5 && dotr_map_edit_00a9c000.unknown_91 == 8);
    } else {
        assert(call_count == 4 && dotr_map_edit_00a9c000.unknown_91 == 0);
    }
    assert(data_00317490_drift.restart == ((pad & 0xf000) ? 60 : 0));
}

static void check_act(void)
{
    int x, y, n;

    /* drift: restart forces the first step to 1; clamp at 0x8000 and at 0 negates each step */
    act_reset(3, 3, 2, 0);
    data_00317490_drift.restart = 1;
    data_00317490_drift.unknown_c86 = 0x7f00;
    data_00317490_drift.step_c86 = -1;
    data_00317490_drift.unknown_c88 = 0x0400;
    data_00317490_drift.step_c88 = -1;
    SzDuel_ActMapEdit();
    assert(data_00317490_drift.unknown_c86 == 0x8000 && data_00317490_drift.step_c86 == -1);
    assert(data_00317490_drift.unknown_c88 == 0 && data_00317490_drift.step_c88 == 1);
    /* the wait counts down and, while non-zero, the cursor pad is not read */
    assert(dotr_map_edit_00a9c000.unknown_91 == 1 && call_count == 3);
    expect(2, 'p', 0, 2, 0xc, 0, 0, 0, 0, 0, 0);
    /* drift inside the range: plain steps of 0x200 and 0x800, steps kept */
    act_reset(3, 3, 2, 0);
    data_00317490_drift.step_c86 = 1;
    data_00317490_drift.step_c88 = 1;
    SzDuel_ActMapEdit();
    assert(data_00317490_drift.unknown_c86 == 0x1200 && data_00317490_drift.step_c86 == 1);
    assert(data_00317490_drift.unknown_c88 == 0x1800 && data_00317490_drift.step_c88 == 1);
    /* lower clamp of the first value, upper clamp of the second */
    act_reset(3, 3, 2, 0);
    data_00317490_drift.unknown_c86 = 0x100;
    data_00317490_drift.step_c86 = -1;
    data_00317490_drift.unknown_c88 = 0x7c00;
    data_00317490_drift.step_c88 = 1;
    SzDuel_ActMapEdit();
    assert(data_00317490_drift.unknown_c86 == 0 && data_00317490_drift.step_c86 == 1);
    assert(data_00317490_drift.unknown_c88 == 0x8000 && data_00317490_drift.step_c88 == -1);

    /* any of the 0x8c0 bits: the 7 by 7 field is saved to grid (4 + 2) % 5, then sound 4 and pop */
    act_reset(3, 3, 0, 0);
    pad_bits[0] = 0x800;
    field_mode = 1;
    memset(&grids, 0xee, sizeof(grids));
    SzDuel_ActMapEdit();
    expect(0, 'p', 0, 0, 0x8c0, 0, 0, 0, 0, 0, 0);
    expect(1, 'D', 0, 0, 0, 0, 0, 0, 0, 0, 0);
    n = 2;
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            expect(n++, 'G', x, y, 0, 0, 0, 0, 0, 0, 0);
            assert(grids.grids[1][y][x] == x + 10 * y);
        }
    }
    expect(n++, 'E', 0, 4, 0, 0, 0, 0, 0, 0, 0);
    expect(n++, 'O', 0, 0, 0, 0, 0, 0, 0, 0, 0);
    assert(n == call_count);
    assert(grids.grids[0][6][6] == 0xee && grids.grids[2][0][0] == 0xee);

    /* bit 0x20: sound 5 and pop, nothing else */
    act_reset(3, 3, 0, 0);
    pad_bits[0] = 0x20;
    SzDuel_ActMapEdit();
    expect(1, 'p', 0, 0, 0x20, 0, 0, 0, 0, 0, 0);
    expect(2, 'E', 0, 5, 0, 0, 0, 0, 0, 0, 0);
    expect(3, 'O', 0, 0, 0, 0, 0, 0, 0, 0, 0);
    assert(call_count == 4);

    /* field change: bit 4 steps the field value down (9 -> 8), bit 8 up (9 -> 0), both cancel */
    for (n = 0; n < 3; n++) {
        static const int bits[3] = { 4, 8, 0xc }, want[3] = { 8, 0, 9 };
        act_reset(2, 5, 2, 0);
        pad_bits[2] = bits[n];
        SzDuel_ActMapEdit();
        expect(2, 'p', 0, 2, 0xc, 0, 0, 0, 0, 0, 0);
        expect(3, 'G', 2, 5, 0, 0, 0, 0, 0, 0, 0);
        expect(4, 'p', 0, 2, 4, 0, 0, 0, 0, 0, 0);
        expect(5, 'p', 0, 2, 8, 0, 0, 0, 0, 0, 0);
        expect(6, 'E', 0, 3, 0, 0, 0, 0, 0, 0, 0);
        expect(7, 'F', 2, 5, want[n], 0, 0, 0, 0, 0, 0);
        assert(call_count == 8);
    }

    /* cursor: no direction bit does nothing; each direction, each edge */
    act_step(0x0fff, 3, 3, 0, 3, 3, 0, 0);
    act_step(0x8000, 3, 3, 0, 2, 3, 0, 1);
    act_step(0x8000, 1, 3, 0, 0, 3, 0, 1);
    act_step(0x8000, 0, 3, 0, 0, 3, 0, 0);
    act_step(0x2000, 5, 3, 0, 6, 3, 0, 1);
    act_step(0x2000, 6, 3, 0, 6, 3, 0, 0);
    act_step(0x1000, 3, 1, 0, 3, 0, 0, 1);
    act_step(0x1000, 3, 0, 0, 3, 0, 0, 0);
    act_step(0x4000, 3, 5, 0, 3, 6, 0, 1);
    act_step(0x4000, 3, 6, 0, 3, 6, 0, 0);
    /* a diagonal moves sideways first and sets the flag; with the flag set it moves up/down */
    act_step(0x9000, 3, 3, 0, 2, 3, 1, 1);
    act_step(0x9000, 3, 3, 1, 3, 2, 0, 1);
    act_step(0x6000, 3, 3, 1, 3, 4, 0, 1);
    /* flag set but no up/down bit: the flag is cleared and the sideways step is taken */
    act_step(0x8000, 3, 3, 1, 2, 3, 0, 1);
    /* a diagonal blocked sideways moves up/down; blocked both ways clears the flag only */
    act_step(0x9000, 0, 3, 0, 0, 2, 0, 1);
    act_step(0x9000, 0, 0, 1, 0, 0, 0, 0);
}

int main(void)
{
    assert(offsetof(struct DotrMapEditState, unknown_8f) == 0x8f);
    check_draw(0, 0, 84, 1.0f);
    check_draw(3, 5, 85, 84.0f / 85);
    check_draw(6, 6, 168, 0.5f);
    check_draw(7, 2, 10, 1.0f);
    check_act();
    puts("map_edit: ok");
    return 0;
}

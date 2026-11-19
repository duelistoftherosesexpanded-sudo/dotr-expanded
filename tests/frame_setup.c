/* Synthetic host tests for the frame_setup group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/frame_setup.h"

struct DotrSetupControl data_00317490_setup;
static struct DotrSetupEntry entry;
static int step;
/* Scripted scenarios (second == 1: 0x1d9a30, second == 2: 0x1d9c40): each stub checks its row of
 * the current script and returns the row's result. */
static int second;
struct Row { int id, a, b, c, d, e, out, ret; };
static const struct Row *script;
static int script_rows;
static int slot_index;
static struct Row script_1d9a30[] = {
    {0x20a470, 3, 2, 50, 0, 0, 0, 0},      {0x20a610, 0x100, 0, 0, 0, 0, 0, 0x110},
    {0x20a870, 0x110, 0, 0, 0, 0, 0, 0x120}, {0x20a760, 0x120, 0, 0, 0, 0, 0, 0x130},
    {0x209e50, 0, 0, 0, 0, 0, 0, 1000},    {0x202e00, 1000, 2, 0, 0, 0, 0, 16},
    {0x1fd320, 0x130, 77, 1016, 0, 0, 32, 0x200},
    {0x202bf0, 3, 4, 0, 0, 0, 0, 0x64},    {0x202bf0, 3, 5, 0, 0, 0, 0, 0x65},
    {0x202bf0, 3, 6, 0, 0, 0, 0, 0x66},    {0x202b00, 0x200, 3, 1048, 0, 0, 64, 0x300},
    {0x202d60, 3, -1, 0, 0, 0, 0, 0},      {0x202e00, 1112, 1, 0, 0, 0, 0, 8},
    {0x202bf0, 3, 0, 0, 0, 0, 0, 0x60},    {0x202b00, 0x300, 3, 1120, 0, 0, 4, 0x310},
    {0x202d60, 3, 0, 0, 0, 0, 0, 0},       {0x1fd680, 0x310, 0, 21, 0, 0, 0, 0x400},
    {0x1feab0, 0, 0, 0, 0, 0, 0, 0},       {0x1fd680, 0x400, 1, 161, 0, 22, 0, 0x410},
    {0x1fd680, 0x410, 2, 162, 0, 44, 0, 0x420}, {0x20f290, 0x420, 0, 0, 0, 0, 0, 0x500},
};

static int row(int id, int a, int b, int c, int d, int e, int *out)
{
    assert(step < script_rows);
    assert(script[step].id == id && script[step].a == a && script[step].b == b && script[step].c == c);
    assert(script[step].d == d && script[step].e == e);
    if (out) *out = script[step].out;
    return script[step++].ret;
}

int RhBase_AssignRenderFrame(int index, int mode, int kind) { return row(0x20a470, index, mode, kind, 0, 0, 0); }
DotrSetupWord RhBase_EnableRenderFrame(DotrSetupWord cursor) { return row(0x20a610, cursor, 0, 0, 0, 0, 0); }
DotrSetupWord RhBase_ClearRenderFrame(DotrSetupWord cursor, int a1) { return row(0x20a870, cursor, a1, 0, 0, 0, 0); }
DotrSetupWord RhBase_DisableRenderFrame(DotrSetupWord cursor) { return row(0x20a760, cursor, 0, 0, 0, 0, 0); }

int RhBase_GetUserGsAddress(void)
{
    if (second) return row(0x209e50, 0, 0, 0, 0, 0, 0);
    assert(step++ == 0);
    return 1000;
}

int SzText_InitClutManage(int base, int size)
{
    if (second) {
        /* the running base is stored at +0xc80 before the second size query */
        if (second == 1 && step == 12) assert(data_00317490_setup.unknown_c80 == 1112);
        return row(0x202e00, base, size, 0, 0, 0, 0);
    }
    assert(step++ == 1 && base == 1000 && size == 2);
    return 16;
}

DotrSetupWord SzMsg_Init_Pipe(DotrSetupWord cursor, int a0, int a1, int *out)
{
    if (second) return row(0x1fd320, cursor, a0, a1, 0, 0, out);
    assert(step++ == 2 && cursor == 0x100 && a0 == 77 && a1 == 1016);
    *out = 32;
    return 0x200;
}

int SzText_LoadRequest(int group, int id)
{
    int i;

    if (second) {
        if (second == 1 && step == 7) for (i = 0; i < 9; i++) assert(data_00317490_setup.unknown_6a0[i] == 0);
        return row(0x202bf0, group, id, 0, 0, 0, 0);
    }
    assert(group == 3 && (step == 3 ? id == 6 : (step == 4 && id == 7)));
    /* all nine words were cleared before the first lookup */
    if (step == 3) for (i = 0; i < 9; i++) assert(data_00317490_setup.unknown_6a0[i] == 0);
    step++;
    return 0x60 + id;
}

DotrSetupWord SzText_DoRequest_Pipe(DotrSetupWord cursor, int a0, int a1, int *out)
{
    if (second == 2) {
        assert(out == 0);
        /* the slot is stored before the cursor call */
        assert(data_00317490_setup.unknown_6a0[slot_index] == 0x70);
    }
    if (second) return row(0x202b00, cursor, a0, a1, 0, 0, out);
    assert(step++ == 5 && cursor == 0x200 && a0 == 3 && a1 == 1048);
    *out = 64;
    return 0x300;
}

int SzText_FreeRequest(int a, int b)
{
    if (second) return row(0x202d60, a, b, 0, 0, 0, 0);
    assert(step++ == 6 && a == 3 && b == -1);
    return 0;
}

DotrSetupWord SzMsg_LoadFont_Pipe(DotrSetupWord cursor, int a0, int a1, int a2, int a3)
{
    if (second) {
        if (second == 1 && step == 18) assert(entry.unknown_0c == 22);
        return row(0x1fd680, cursor, a0, a1, a2, a3, 0);
    }
    assert(step++ == 7 && cursor == 0x300 && a0 == 0 && a1 == 21 && a2 == 0 && a3 == 0);
    return 0x400;
}

struct DotrSetupEntry *SzMsg_GetInfo(int index)
{
    if (second) {
        row(0x1feab0, index, 0, 0, 0, 0, 0);
        return &entry;
    }
    assert(step++ == 8 && index == 0);
    return &entry;
}

DotrSetupWord RhText_SetTextureFlush(DotrSetupWord cursor)
{
    if (second) return row(0x20f290, cursor, 0, 0, 0, 0, 0);
    assert(step++ == 9 && cursor == 0x400 && entry.unknown_0c == 22);
    return 0x500;
}

void SzMsg_SetDefPrioZ(int v)
{
    assert(step++ == 10 && v == 0xfffff8 && data_00317490_setup.unknown_c58 == 0xfffff8);
}

/* 0x1d9c40 with the slot at index empty: expects the five scripted calls when the index is 0..3 or 8,
 * none otherwise. */
static void check_1d9c40(int index)
{
    struct Row rows[5] = {
        {0x202e00, 500, 1, 0, 0, 0, 0, 8}, {0x202bf0, 3, 0, 0, 0, 0, 0, 0x70}, {0x202b00, 0x100, 3, 508, 0, 0, 0, 0x200},
        {0x202d60, 3, 0, 0, 0, 0, 0, 0},   {0x20f290, 0x200, 0, 0, 0, 0, 0, 0x300},
    };
    int small = index >= 0 && index < 4;
    int *words = (int *)((char *)&data_00317490_setup + 0x6a0);
    int i;

    if (small) {
        rows[1].b = rows[3].b = index;
    } else {
        rows[1].a = rows[2].b = rows[3].a = 9;
    }
    memset(&data_00317490_setup, 0, sizeof(data_00317490_setup));
    for (i = -1; i < 10; i++) words[i] = 5;
    words[index] = 0;
    data_00317490_setup.unknown_c80 = 500;
    script = rows;
    script_rows = small || index == 8 ? 5 : 0;
    slot_index = index;
    second = 2;
    step = 0;
    assert(lSzDuel_SetupCardTexture(0x100, index) == (script_rows ? 0x300 : 0x100) && step == script_rows);
    for (i = -1; i < 10; i++) {
        int want = 5;
        if (i == index) want = script_rows ? 0x70 : 0;
        else if (script_rows && i >= 0 && i < 4) want = 0;
        else if (small && i == 8) want = 0;
        assert(words[i] == want);
    }
    assert(data_00317490_setup.unknown_c80 == 500);
}

int main(void)
{
    int i;

    assert(offsetof(struct DotrSetupControl, unknown_6a0) == 0x6a0);
    assert(offsetof(struct DotrSetupControl, unknown_c58) == 0xc58);
    assert(offsetof(struct DotrSetupControl, unknown_c7c) == 0xc7c);
    memset(&data_00317490_setup, 0x55, sizeof(data_00317490_setup));
    /* 0x1aefd0: one fixed loop of nine and no other branch; every stored value and forwarded argument. */
    assert(SzDuel_StartWindowSessionWithPalette_Pipe(0x100, 77) == 0x500 && step == 11);
    for (i = 0; i < 9; i++) {
        assert(data_00317490_setup.unknown_6a0[i] == (i == 6 ? 0x66 : i == 7 ? 0x67 : 0));
    }
    {
        /* The word just before unknown_6a0 is left untouched. */
        unsigned int before;
        memcpy(&before, (char *)&data_00317490_setup + offsetof(struct DotrSetupControl, unknown_6a0) - 4, 4);
        assert(before == 0x55555555 && data_00317490_setup.unknown_6c4[0] == 0x55);
    }
    assert(data_00317490_setup.unknown_c7c == 1000 + 16 + 32 + 64);
    /* 0x1d9a30: the same fixed loop of nine and no other branch; every forwarded argument, stored
     * value and the result. */
    assert(offsetof(struct DotrSetupControl, unknown_c54) == 0xc54 && offsetof(struct DotrSetupControl, unknown_c80) == 0xc80);
    memset(&data_00317490_setup, 0xff, sizeof(data_00317490_setup));
    entry.unknown_0c = 0;
    second = 1;
    script = script_1d9a30;
    script_rows = sizeof(script_1d9a30) / sizeof(script_1d9a30[0]);
    step = 0;
    assert(SzDuel_StartCardSessionWithPalette_Pipe(0x100, 77) == 0x500 && step == 21);
    for (i = 0; i < 9; i++) {
        assert(data_00317490_setup.unknown_6a0[i] == (i == 0 ? 0x60 : i >= 4 && i <= 6 ? 0x60 + i : 0));
    }
    assert(data_00317490_setup.unknown_c80 == 1112 && data_00317490_setup.unknown_c7c == 1124);
    assert(data_00317490_setup.unknown_c54 == 0 && data_00317490_setup.unknown_c58 == -1);
    /* only bit 5 of the byte at +0xd64 is cleared */
    assert(((unsigned char *)&data_00317490_setup)[0xd64] == 0xdf && ((unsigned char *)&data_00317490_setup)[0xd65] == 0xff);
    /* 0x1d9a20: forwards the cursor with palette 2 (the same script, palette row changed) and returns
     * the result. */
    script_1d9a30[6].b = 2;
    step = 0;
    assert(script_1d9a30[6].id == 0x1fd320);
    assert(SzDuel_StartCardSession_Pipe(0x100) == 0x500 && step == 21);
    /* 0x1d9c40: slot already set; both ends of 0..3; 8; and the indices just outside each range. */
    memset(&data_00317490_setup, 0, sizeof(data_00317490_setup));
    data_00317490_setup.unknown_6a0[2] = 1;
    second = 2;
    script_rows = 0;
    step = 0;
    assert(lSzDuel_SetupCardTexture(0x100, 2) == 0x100 && step == 0 && data_00317490_setup.unknown_6a0[2] == 1);
    check_1d9c40(0);
    check_1d9c40(3);
    check_1d9c40(8);
    check_1d9c40(-1);
    check_1d9c40(4);
    check_1d9c40(7);
    check_1d9c40(9);
    puts("frame_setup: ok");
    return 0;
}

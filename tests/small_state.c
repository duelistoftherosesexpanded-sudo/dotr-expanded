/* Synthetic tests of the miscellaneous small state routines; every routine
 * outside the group is a stub recording its arguments. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/small_state.h"
#include "dotr/ee/draw_state.h"

struct DotrSmallTable gSzPad_pl;
int gSzDebug_dl[2];
struct DotrSmallBits data_00325cac[1];
struct DotrSmallFlagsOwner data_00325c80_owner;
int gEx3d_aPlane[1];
int gMyOT[1];
struct DotrSmallBits data_0034f6e8[1];
int data_003180e4[2];
int data_0031810c[1];
char data_01fe0000[1];

static int calls[10], fill_value, fill_count, six[7];
static int *fill_destination, *pointer_seen;
static DotrSmallWord size_seen, value_seen;

int *RhMem_FillMemory4(int *destination, int value, int count)
{
    calls[0]++;
    fill_destination = destination;
    fill_value = value;
    fill_count = count;
    return destination + count;
}

int Ex3d_Clipping(int a, int *b, int c, int d, int e, int f, int g)
{
    calls[1]++;
    six[0] = a;
    pointer_seen = b;
    six[1] = c;
    six[2] = d;
    six[3] = e;
    six[4] = f;
    six[5] = g;
    return 0x6d30;
}

void RhGeom_PushMatrix(int unknown) { assert(calls[2] == 0 && unknown == 0); calls[2]++; }
void ExClearOt(DotrPktCtx *a, DotrPktEnt *b, int c)
{
    assert(calls[2] == 1 && (int *)a == gMyOT && (intptr_t)b == 0x1e08000 && c == 12);
    calls[3]++;
}
void ExBindPrim(DotrPktCtx *a) { assert(calls[5] == 0 && (int *)a == gMyOT); calls[4]++; }
void RhGeom_PopMatrix(void) { assert(calls[4] == 1); calls[5]++; }
void func_00193808(void) { calls[6]++; }
void SD_CallEveryVSync(void) { calls[7]++; }
void SD_BGMPreLoad(void) { calls[8]++; }
static int clear_calls, init_calls, open_calls, boot_calls;
static void *open_buffers[2];

void RhMem_FillMemory(void *destination, int value, int size)
{
    if (destination == (void *)&data_00325c80_owner) { /* SzDebug_Init */
        assert(value == 0 && size == 48 && size <= (int)sizeof(data_00325c80_owner));
        memset(destination, value, (size_t)size);
        boot_calls += 0x1000;
        return;
    }
    assert(destination == &gSzPad_pl && value == 0 && size == 704 && init_calls == 0);
    assert(size <= (int)sizeof(gSzPad_pl));
    memset(destination, value, (size_t)size);
    clear_calls++;
}

void func_00192588(int unknown)
{
    assert(unknown == 0 && clear_calls == 1 && open_calls == 0);
    assert(gSzPad_pl.unknown_200[1].unknown_59 == 10);
    init_calls++;
}

void func_001927e8(int a, int b, void *buffer)
{
    assert(init_calls == 1 && a == open_calls && b == 0 && open_calls < 2);
    open_buffers[open_calls++] = buffer;
}

DotrSmallWord func_00197bc0(DotrSmallWord size) { size_seen = size; return 0x1234; }
void func_00197c18(DotrSmallWord value) { value_seen = value; calls[9]++; }

/* 0x205170: the flag byte is set through the owner view, which on the target
 * is the same storage as data_00325cac. */
static void check_flag_pair(void)
{
    unsigned char *byte = (unsigned char *)&data_00325c80_owner.unknown_2c;
    *byte = 0x04;
    assert(SzDebug_IsHide() == 1);
    *byte = 0x01;
    assert(SzDebug_IsHide() == 1);
    *byte = 0xfa;
    assert(SzDebug_IsHide() == 0);
    *byte = 0x05;
    assert(SzDebug_IsHide() == 1);
    *byte = 0x00;
    assert(SzDebug_IsHide() == 0);
    assert(offsetof(struct DotrSmallFlagsOwner, unknown_2c) == 0x2c);
}

struct DotrSmallSheetRow tex_uv_data[3];
static int sheet_calls, sheet_args[17];
void Taku_Prim_Set_SP1(int a0, short x0, short y0, short x1, short y1, int u, int v, int width, int height,
                   int c0, int c1, int c2, int c3, int unknown_20, int unknown_10, int unknown_21,
                   int unknown_22)
{
    int values[17];
    values[0] = a0; values[1] = x0; values[2] = y0; values[3] = x1; values[4] = y1;
    values[5] = u; values[6] = v; values[7] = width; values[8] = height;
    values[9] = c0; values[10] = c1; values[11] = c2; values[12] = c3;
    values[13] = unknown_20; values[14] = unknown_10; values[15] = unknown_21; values[16] = unknown_22;
    memcpy(sheet_args, values, sizeof(values));
    sheet_calls++;
}

/* Straight-line forwarders: every forwarded argument is checked once, with
 * sums that overflow 16 bits to show the narrowing. */
static void check_sheet_forwarders(void)
{
    static const int moved[17] = { 5, -32736, 220, 130, 13088, -7, 8, 32700, -9, 201, 202, 203, 204, 205, 0xfedc, 206, -300 };
    static const int sized[17] = { 5, 100, 20000, -32736, 19991, -7, 8, 32700, -9, 201, 202, 203, 204, 205, 0xfedc, 206, -300 };
    struct DotrSmallSheetItem item;

    memset(&item, 0, sizeof(item));
    memset(tex_uv_data, 0, sizeof(tex_uv_data));
    tex_uv_data[2].x = -7; tex_uv_data[2].y = 8;
    tex_uv_data[2].width = 32700; tex_uv_data[2].height = -9;
    tex_uv_data[2].unknown_10 = 0xfedc;
    item.x0 = 32700; item.y0 = 200; item.x1 = 30; item.y1 = 13068;
    item.color[0] = 201; item.color[1] = 202; item.color[2] = 203; item.color[3] = 204;
    item.unknown_20 = 205; item.unknown_21 = 206; item.unknown_22 = -300; item.row = 2;
    sheet_calls = 0;
    Taku_Obj_Set(5, &item, 100, 20);
    assert(sheet_calls == 1 && memcmp(sheet_args, moved, sizeof(moved)) == 0);
    Taku_Te_Set(5, &item, 100, 20000);
    assert(sheet_calls == 2 && memcmp(sheet_args, sized, sizeof(sized)) == 0);
}

/* 0x203a40 branch coverage: each mode, an unknown mode, and in mode 2 each reason a bit is kept
 * or dropped. */
static void check_mask_modes(void)
{
    struct DotrSmallEntry *e = &gSzPad_pl.unknown_200[1];
    int i;

    memset(e, 0, sizeof(*e));
    e->unknown_00 = 0x00f3;
    e->unknown_02 = 0x0031;
    assert(SzPad_Check(1, 0, 0xffff) == 0x00c2 && SzPad_Check(1, 0, 0x0040) == 0x0040);
    assert(SzPad_Check(1, 1, 0xffff) == 0x00f3 && SzPad_Check(1, 1, 0x0101) == 0x0001);
    assert(SzPad_Check(1, 3, 0xffff) == 0 && SzPad_Check(1, -1, 0xffff) == 0);
    e->unknown_5a.unknown_bits_0_3 = 3;
    e->unknown_5a.unknown_bits_4_7 = -1;
    for (i = 0; i < 16; i++) e->unknown_18[i] = 1;
    e->unknown_18[0] = 0;   /* multiple of 3: kept */
    e->unknown_18[2] = 6;   /* kept, but not in the mask below */
    e->unknown_18[8] = -3;  /* negative: dropped */
    e->unknown_18[9] = 4;   /* not a multiple: dropped */
    e->unknown_18[15] = 9;  /* last counter: kept */
    assert(SzPad_Check(1, 2, 0x8301) == (0x8001 | (0x00c2 & 0x8301)));
    assert(SzPad_Check(1, 2, 0xffff) == (0x8005 | 0x00c2));
    /* the mask is used as a halfword in the loop */
    assert(SzPad_Check(1, 2, 0x10004) == 0x0004);
    assert(SzPad_Check(1, 2, 0) == 0);
}

unsigned char data_002d078c = 0xc8;
char data_002d07f4[] = "%s/%s";
char data_002d07fa[] = "host";
char data_002d07ff[] = "name";
static const char *boot_format, *boot_label, *boot_first, *boot_second, *boot_name;
static char *boot_out, *boot_found;
char *SzFile_GetEnv(const char *label) { boot_calls += 1; boot_label = label; return "root"; }
int func_0019b648(char *destination, const char *format, ...)
{
    va_list args;

    boot_calls += 0x10;
    boot_out = destination;
    boot_format = format;
    va_start(args, format);
    boot_first = va_arg(args, const char *);
    boot_second = va_arg(args, const char *);
    va_end(args);
    return 0;
}
char *func_00197548(const char *label) { boot_calls += 0x100; boot_name = label; return boot_found; }

/* One case per outcome of the final test (name found / not found); every stored value and
 * forwarded argument is checked in the first. */
static void check_owner_setup(void)
{
    struct DotrSmallFlagsOwner *o = &data_00325c80_owner;
    unsigned char *byte = (unsigned char *)&o->unknown_2c;
    int i;

    memset(o, 0xff, sizeof(*o));
    boot_found = 0;
    boot_calls = 0;
    SzDebug_Init();
    assert(boot_calls == 0x1111 && boot_label == data_002d07fa && boot_format == data_002d07f4);
    assert(strcmp(boot_first, "root") == 0 && boot_second == data_002d07ff && boot_name == data_002d07ff);
    assert(boot_out != 0);
    assert(o->unknown_00[0] == 0 && o->unknown_00[1] == 0xc8 && o->unknown_00[10] == 2);
    for (i = 2; i < 10; i++) {
        assert(o->unknown_00[i] == 0);
    }
    assert(*byte == 0);
    boot_found = data_002d07ff;
    SzDebug_Init();
    assert(*byte == 2 && o->unknown_00[10] == 2);
    memset(o, 0, sizeof(*o));
}

static int pad_state[2], pad_read[2], pad_calls, pad_slots;
int func_00192bb8(int port, int slot)
{
    pad_calls++;
    pad_slots |= slot;
    return pad_state[port];
}
int func_00192b38(int port, int slot, unsigned char *out)
{
    int i;

    pad_calls += 0x10;
    pad_slots |= slot;
    for (i = 0; i < 32; i++) {
        out[i] = (unsigned char)(0x40 + port * 0x20 + i);
    }
    out[2] = 0xfe;
    out[3] = 0x7f;
    return pad_read[port];
}

static int pad_info[3], pad_modes[8], pad_lock[8], pad_req, pad_act[2], pad_log[8], pad_logged;
int func_00192f98(int port, int slot, int term, int offs)
{
    assert(port == 0 && slot == 0);
    if (term == 4) {
        return offs == -1 ? pad_info[0] : pad_modes[offs];
    }
    assert(offs == 0 && (term == 1 || term == 2));
    return pad_info[term];
}
int func_001930d0(int port, int slot, int offs, int lock)
{
    assert(port == 0 && slot == 0 && lock == 3);
    pad_log[pad_logged++] = offs;
    return pad_lock[offs];
}
int func_00192cd0(int port, int slot)
{
    assert(port == 0 && slot == 0);
    return pad_req;
}
int func_00193488(int port, int slot)
{
    assert(port == 0 && slot == 0);
    return pad_act[0];
}
int func_001934e8(int port, int slot)
{
    assert(port == 0 && slot == 0);
    return pad_act[1];
}

/* Runs SzPad_Status with entry 0 in the given phase and state; entry 1 gets state 1 (no work). */
static int pad_step(int state, int phase)
{
    struct DotrSmallEntry *e = gSzPad_pl.unknown_200;

    pad_state[0] = state;
    pad_state[1] = 1;
    pad_logged = 0;
    e[0].unknown_5b.unknown_bits_0_5 = (signed char)phase;
    SzPad_Status();
    return e[0].unknown_5b.unknown_bits_0_5;
}

static void pad_dirty(struct DotrSmallEntry *e)
{
    memset(e, 0x55, sizeof(*e));
    e->unknown_59 = 9;
}

static void pad_assert_reset(const struct DotrSmallEntry *e)
{
    int i;

    assert(e->unknown_00 == 0 && e->unknown_02 == 0 && e->unknown_04 == 0);
    for (i = 0; i < 4; i++) {
        assert(e->unknown_06[i] == 0x80);
    }
    for (i = 0; i < 12; i++) {
        assert(e->unknown_0a[i] == 0);
    }
    assert(e->unknown_16[0] == 0x55 && e->unknown_16[1] == 0x55);
    for (i = 0; i < 16; i++) {
        assert(e->unknown_18[i] == -9);
    }
    assert(e->unknown_59 == 9 && !e->unknown_5b.unknown_bit_6 && !e->unknown_5b.unknown_bit_7);
}

/* One case per branch outcome of SzPad_Status: states 0, 1, 2 and 6; every phase 0-5 and two
 * outside the table (9 and -1); each callee answer that selects a different arm; the loop
 * bounds 3, 16, 4 and 12 through sentinels; the mode search with zero, a rejected and an
 * accepted lock. */
static void check_pad_status(void)
{
    struct DotrSmallEntry *e = gSzPad_pl.unknown_200;
    struct DotrSmallEntry saved;

    memset(&gSzPad_pl, 0, sizeof(gSzPad_pl));
    pad_dirty(&e[0]);
    pad_dirty(&e[1]);
    saved = e[1];
    assert(pad_step(0, 3) == 0);
    pad_assert_reset(&e[0]);
    assert(e[0].unknown_58 == 0x55 && e[0].unknown_5a.unknown_bits_0_3 == 5);
    assert(memcmp(&saved, &e[1], sizeof(saved)) == 0);
    pad_dirty(&e[0]);
    saved = e[0];
    saved.unknown_5b.unknown_bits_0_5 = 3;
    assert(pad_step(1, 3) == 3 && memcmp(&saved, &e[0], sizeof(saved)) == 0);

    pad_info[0] = 3;
    pad_info[1] = 0;
    pad_info[2] = 7;
    assert(pad_step(2, 0) == 0);
    pad_assert_reset(&e[0]);
    assert(e[0].unknown_5a.unknown_bits_4_7 == 3 && e[0].unknown_5a.unknown_bits_0_3 == 5 && e[0].unknown_58 == 0x55);
    pad_info[1] = 4;
    pad_info[2] = 0;
    assert(pad_step(6, 0) == 1 && e[0].unknown_58 == 4);
    pad_info[2] = 7;
    pad_info[0] = 0;
    assert(pad_step(2, 0) == 5 && e[0].unknown_58 == 7 && e[0].unknown_5a.unknown_bits_4_7 == 0);

    e[0].unknown_58 = 0;
    assert(pad_step(2, 1) == 5 && pad_logged == 0);
    e[0].unknown_5a.unknown_bits_4_7 = 3;
    pad_modes[0] = 4;
    pad_modes[1] = 7;
    pad_modes[2] = 7;
    pad_lock[1] = 0;
    pad_lock[2] = 0;
    assert(pad_step(2, 1) == 5 && pad_logged == 2 && pad_log[0] == 1 && pad_log[1] == 2 && e[0].unknown_58 == 0);
    pad_lock[1] = 1;
    assert(pad_step(2, 1) == 2 && pad_logged == 1 && e[0].unknown_58 == 7);

    pad_req = 2;
    assert(pad_step(2, 2) == 2 && !e[0].unknown_5b.unknown_bit_6);
    pad_req = 1;
    assert(pad_step(2, 2) == 1 && !e[0].unknown_5b.unknown_bit_6);
    pad_req = 0;
    assert(pad_step(2, 2) == 3 && e[0].unknown_5b.unknown_bit_6 && !e[0].unknown_5b.unknown_bit_7);

    pad_act[0] = 0;
    pad_act[1] = 1;
    assert(pad_step(2, 3) == 5);
    pad_act[0] = 1;
    pad_act[1] = 0;
    assert(pad_step(2, 3) == 5);
    pad_act[1] = 1;
    assert(pad_step(2, 3) == 4);

    pad_req = 2;
    assert(pad_step(2, 4) == 4 && !e[0].unknown_5b.unknown_bit_7);
    pad_req = 1;
    assert(pad_step(2, 4) == 3 && !e[0].unknown_5b.unknown_bit_7);
    pad_req = 0;
    assert(pad_step(2, 4) == 5 && e[0].unknown_5b.unknown_bit_7 && e[0].unknown_5b.unknown_bit_6);

    saved = e[0];
    assert(pad_step(2, 5) == 5 && memcmp(&saved, &e[0], sizeof(saved)) == 0);
    assert(pad_step(2, 9) == 9);
    assert(pad_step(6, -1) == -1);
    memset(&gSzPad_pl, 0, sizeof(gSzPad_pl));
}

/* One case per branch outcome: field not 5, state 2 / 6 / other, read zero / non-zero,
 * bit 6 and bit 7 each set and clear; the copy bounds 4 and 12 are checked by sentinels. */
static void check_pad_read(void)
{
    struct DotrSmallEntry *e = gSzPad_pl.unknown_200;
    int i;

    memset(&gSzPad_pl, 0, sizeof(gSzPad_pl));
    /* field not 5 on both entries: no calls */
    pad_calls = 0;
    assert(SzPad_VSync() == 1 && pad_calls == 0);
    /* entry 0: state 2, both bits; entry 1: state 6, no bits */
    e[0].unknown_5b.unknown_bits_0_5 = 5;
    e[0].unknown_5b.unknown_bit_6 = -1;
    e[0].unknown_5b.unknown_bit_7 = -1;
    e[0].unknown_04 = 0x0001;
    e[0].unknown_16[0] = 0x99;
    e[1].unknown_5b.unknown_bits_0_5 = 5;
    pad_state[0] = 2; pad_state[1] = 6;
    pad_read[0] = 1; pad_read[1] = 1;
    pad_calls = 0; pad_slots = 0;
    assert(SzPad_VSync() == 1 && pad_calls == 0x22 && pad_slots == 0);
    for (i = 0; i < 4; i++) {
        assert(e[0].unknown_06[i] == 0x44 + i && e[1].unknown_06[i] == 0);
    }
    for (i = 0; i < 12; i++) {
        assert(e[0].unknown_0a[i] == 0x48 + i && e[1].unknown_0a[i] == 0);
    }
    assert(e[0].unknown_16[0] == 0x99);
    assert(e[0].unknown_04 == 0x0181 && e[1].unknown_04 == 0x0180);
    /* bit 6 only / bit 7 only */
    memset(e[0].unknown_06, 0, 16);
    e[0].unknown_5b.unknown_bit_7 = 0;
    e[1].unknown_5b.unknown_bit_7 = -1;
    assert(SzPad_VSync() == 1);
    assert(e[0].unknown_06[3] == 0x47 && e[0].unknown_0a[0] == 0);
    assert(e[1].unknown_06[0] == 0 && e[1].unknown_0a[0] == 0x68 && e[1].unknown_0a[11] == 0x73);
    /* other state: no read; read answering zero: nothing stored */
    memset(e[0].unknown_06, 0, 16);
    memset(e[1].unknown_06, 0, 16);
    e[0].unknown_04 = e[1].unknown_04 = 0;
    pad_state[0] = 1; pad_read[1] = 0;
    pad_calls = 0;
    assert(SzPad_VSync() == 1 && pad_calls == 0x12);
    assert(e[0].unknown_04 == 0 && e[1].unknown_04 == 0);
    assert(e[0].unknown_06[0] == 0 && e[1].unknown_0a[0] == 0);
    memset(&gSzPad_pl, 0, sizeof(gSzPad_pl));
}

int main(void)
{
    int index, slot;
    check_flag_pair();
    /* Control words. */
    SzDuel_SetCardDefPrioZ(11);
    SzDuel_SetWindowDefPrioZ(-12);
    assert(data_003180e4[0] == 11 && data_003180e4[1] == -12);
    data_0031810c[0] = 77;
    assert(SzDuel_GetUserGsAddress() == 77);

    /* The fill thunk passes the table address, zero and 176 words. */
    assert(SzPad_Term() == (int *)&gSzPad_pl + 176 && calls[0] == 1);
    assert(fill_destination == (int *)&gSzPad_pl && fill_value == 0 && fill_count == 176);

    /* Entry accessors index 92-byte entries at +0x200; the accessor
     * tests stay at index 0. */
    memset(&gSzPad_pl, 0, sizeof(gSzPad_pl));
    index = 0;
    for (slot = 0; slot < 4; slot++) {
        SzPad_SetAnalogInfo(index, slot, 0x140 + slot);
        assert(gSzPad_pl.unknown_200[0].unknown_06[slot] == 0x40 + slot);
        assert(SzPad_GetAnalogInfo(index, slot) == 0x40 + slot);
        gSzPad_pl.unknown_200[0].unknown_0a[slot] = (unsigned char)(0xf0 + slot);
        assert(SzPad_GetPressInfo(index, slot) == 0xf0 + slot);
    }
    assert(SzPad_IsAnalog(0) == 0 && SzPad_IsPress(0) == 0);
    gSzPad_pl.unknown_200[0].unknown_5b.unknown_bit_6 = -1;
    assert(SzPad_IsAnalog(0) == 1 && SzPad_IsPress(0) == 0);
    gSzPad_pl.unknown_200[0].unknown_5b.unknown_bit_6 = 0;
    gSzPad_pl.unknown_200[0].unknown_5b.unknown_bit_7 = -1;
    assert(SzPad_IsAnalog(0) == 0 && SzPad_IsPress(0) == 1);

    gSzDebug_dl[0] = 5;
    gSzDebug_dl[1] = -6;
    assert(SzDebug_GetActiveMenu() == 5 && SzDebug_GetActiveModel() == -6);
    memset(data_00325cac, 0xff, sizeof(data_00325cac));
    SzDebug_SetHideFlag(0);
    assert(data_00325cac[0].unknown_bit_0 == 0 && data_00325cac[0].unknown_bit_1 == -1);
    assert(SzDebug_IsEffect() == 1);
    data_00325cac[0].unknown_bit_2 = 0;
    assert(SzDebug_IsEffect() == 0);
    SzDebug_SetHideFlag(3); /* only the low bit is stored */
    assert(data_00325cac[0].unknown_bit_0 == -1);

    /* Argument shuffle: a constant and an address are inserted in front. */
    assert(Ex3d_ViewClip(1, 2, 3, 4, 5) == 0x6d30 && calls[1] == 1);
    assert(six[0] == 6 && pointer_seen == gEx3d_aPlane);
    assert(six[1] == 1 && six[2] == 2 && six[3] == 3 && six[4] == 4 && six[5] == 5);

    ExInitPrim();
    assert(calls[2] == 1 && calls[3] == 1);
    ExTermPrim();
    assert(calls[4] == 1 && calls[5] == 1);
    assert(ExGetMyOT() == gMyOT);

    assert(RhMemCard_Init() == 1 && calls[6] == 1);
    assert(RhMemCard_Term() == 1 && RhMemCard_Act() == 1);
    data_0034f6e8[0].unknown_bit_0 = 0;
    assert(SzSound_IsStandby() == 0);
    SzSound_VSync();
    assert(calls[7] == 0 && SzSound_PlayBGM() == 0 && calls[8] == 0);
    data_0034f6e8[0].unknown_bit_0 = -1;
    assert(SzSound_IsStandby() == 1);
    SzSound_VSync();
    assert(calls[7] == 1 && SzSound_PlayBGM() == 1 && calls[8] == 1);

    /* Size computation: a constant minus the address of a symbol. */
    SzMem_Init();
    assert(size_seen == 0x1ffeff0 - (DotrSmallWord)data_01fe0000);
    assert(value_seen == 0x1234 && calls[9] == 1);
    /* 0x2032c0: byte +0x59 and the low four bits of byte +0x5a of one entry. */
    {
        unsigned char *entry = (unsigned char *)&gSzPad_pl.unknown_200[0];
        assert(offsetof(struct DotrSmallEntry, unknown_59) == 0x59);
        assert(offsetof(struct DotrSmallEntry, unknown_5a) == 0x5a);
        memset(entry, 0xa5, 0x5c); /* the synthetic table holds one entry */
        SzPad_SetParam(0, 0x1234, 0x7c3);
        assert(entry[0x59] == 0x34 && (entry[0x5a] & 0x0f) == 3 && (entry[0x5a] & 0xf0) == (0xa5 & 0xf0));
        assert(entry[0x58] == 0xa5 && entry[0x5b] == 0xa5 && entry[0] == 0xa5);
        SzPad_SetParam(0, -1, -1);
        assert(entry[0x59] == 0xff && (entry[0x5a] & 0x0f) == 0x0f && (entry[0x5a] & 0xf0) == (0xa5 & 0xf0));
        SzPad_SetParam(0, 0, 0x80);
        assert(entry[0x59] == 0 && (entry[0x5a] & 0x0f) == 0);
    }
    /* 0x203b50: selected words receive the negated signed byte +0x59. */
    {
        struct DotrSmallEntry *e = &gSzPad_pl.unknown_200[0];
        int i;
        assert(offsetof(struct DotrSmallEntry, unknown_18) == 0x18);
        assert(offsetof(struct DotrSmallEntry, unknown_58) == 0x58);
        memset(e, 0x11, sizeof(*e));
        e->unknown_59 = -128;
        assert(SzPad_ResetCounter(0, 0x8005) == 0);
        for (i = 0; i < 16; i++) {
            assert(e->unknown_18[i] == (i == 0 || i == 2 || i == 15 ? 128 : 0x11111111));
        }
        assert(e->unknown_16[1] == 0x11 && e->unknown_58 == 0x11 && e->unknown_59 == -128);
        e->unknown_59 = 3;
        assert(SzPad_ResetCounter(0, 0xffff) == 0 && e->unknown_18[0] == -3 && e->unknown_18[15] == -3);
        assert(SzPad_ResetCounter(0, 0) == 0 && e->unknown_18[7] == -3);
    }
    /* 0x2031f0: clear, presets on both entries, then the two buffer halves. */
    {
        int k;
        assert(sizeof(struct DotrSmallTable) >= 704);
        memset(&gSzPad_pl, 0xa5, sizeof(gSzPad_pl));
        SzPad_Init();
        assert(clear_calls == 1 && init_calls == 1 && open_calls == 2);
        assert(open_buffers[0] == (void *)&gSzPad_pl);
        assert(open_buffers[1] == (void *)((char *)&gSzPad_pl + 0x100));
        for (k = 0; k < 2; k++) {
            struct DotrSmallEntry *e = &gSzPad_pl.unknown_200[k];
            assert(e->unknown_59 == 10 && e->unknown_5a.unknown_bits_0_3 == 4);
            assert(e->unknown_5a.unknown_bits_4_7 == 0 && e->unknown_58 == 0 && e->unknown_00 == 0);
        }
    }
    /* 0x203840: only entries whose six-bit field is 5 are advanced. */
    {
        struct DotrSmallEntry *e = gSzPad_pl.unknown_200;
        int i, k;
        assert(offsetof(struct DotrSmallEntry, unknown_06) == 6);
        memset(&gSzPad_pl, 0, sizeof(gSzPad_pl));
        for (k = 0; k < 2; k++) {
            e[k].unknown_00 = 0x8003;
            e[k].unknown_02 = 0x8001;
            e[k].unknown_04 = 0x1234;
            e[k].unknown_59 = -7;
            for (i = 0; i < 16; i++) {
                e[k].unknown_18[i] = 100 + i;
            }
        }
        e[0].unknown_5b.unknown_bits_0_5 = 5;
        e[0].unknown_5b.unknown_bit_6 = -1; /* outside the compared field */
        e[1].unknown_5b.unknown_bits_0_5 = 4;
        SzPad_Act();
        for (i = 0; i < 16; i++) {
            assert(e[0].unknown_18[i] == (i == 0 || i == 15 ? 101 + i : 7));
            assert(e[1].unknown_18[i] == 100 + i);
        }
        assert(e[0].unknown_00 == 0x1234 && e[0].unknown_02 == 0x8003 && e[0].unknown_04 == 0);
        assert(e[1].unknown_00 == 0x8003 && e[1].unknown_02 == 0x8001 && e[1].unknown_04 == 0x1234);
        e[1].unknown_5b.unknown_bits_0_5 = 5;
        e[0].unknown_5b.unknown_bits_0_5 = -27; /* 0x25: low six bits are not 5 as a signed field */
        SzPad_Act();
        assert(e[0].unknown_00 == 0x1234 && e[1].unknown_00 == 0x1234 && e[1].unknown_02 == 0x8003);
        assert(e[1].unknown_18[1] == 7 && e[1].unknown_18[15] == 116 && e[0].unknown_18[15] == 116);
    }
    check_sheet_forwarders();
    check_mask_modes();
    check_pad_read();
    check_pad_status();
    check_owner_setup();
    puts("small_state: ok");
    return 0;
}

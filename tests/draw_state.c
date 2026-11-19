/* Synthetic tests of the draw-state accessors and packet helpers. All
 * routines outside the group are stubs; the copy stub assumes the
 * 128-bit copy semantics of 0x20b380. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "dotr/ee/draw_state.h"
#include "dotr/ee/draw_quad_block.h"
#include "dotr/ee/block_table.h"

struct DotrDrawState gRhBase_dl;
struct DotrDrawFlag data_00325f40[1];
struct DotrDrawSelector gRhBase_rl;
struct DotrDrawView gRhVu1Mac_State;
struct DotrDrawSelectorView data_00325f20_view[1];
struct DotrDrawFrameWords data_00325f20_frame;
struct DotrDrawFrameSetup data_00325f20_setup;
struct DotrDrawFields data_00325fc0_fields[4];
long g_aqwExDitherMatrix[4] = {0x1111, 0x2222, 0x3333, 0x4444};
int data_003261a8[1];
DotrDrawWord data_003261d0[1];
int data_003261e4[1];
struct DotrDrawPair gExPrim_aaRegs[3][7];
long gExPrim_aqwAlpha[4];
struct DotrBlockEntry gSzFade_fl[2];

static int stage, last_args[4];
static void *copy_destination, *copy_source;
static int copy_count;
static void *copy16_log[4][2];
static int copy16_count[4], copy_calls;
static int chain_calls;
static int slot_indices[4], slot_count;
/* 0x2098b0 callees: each records its id (and arguments where checked). The
 * five table symbols are separate arrays here, each indexed by row * 40. */
int data_00326180[24];
char gRhSys_asd[40 * 3];
char data_00293f14[40 * 3];
char data_00293f18[40 * 3];
char data_00293f1c[40 * 3];
char data_00293f20[40 * 3];
char data_00293f30[40 * 3];
static int reset_order[32], reset_count, reset_args[32][4];
/* RhBase_FlipBuffer (0x2099d0): while flip_active is set the shared stubs also log one letter
 * each and return scripted values. */
static int flip_active, flip_n, flip_sync_result, flip_focus;
static char flip_log[64];
static int flip_vsync_result[4], flip_vsync_calls;
static unsigned flip_vsync_t1[4], flip_final_t1;
static int flip_packet[4 + 16];
static void flip_note(char c)
{
    if (flip_active) {
        assert(flip_n < 63);
        flip_log[flip_n++] = c;
        flip_log[flip_n] = 0;
    }
}
static int reset_buffer[4];
static void reset_note(int id, int a, int b, int c, int d)
{
    reset_args[reset_count][0] = a; reset_args[reset_count][1] = b;
    reset_args[reset_count][2] = c; reset_args[reset_count][3] = d;
    reset_order[reset_count++] = id;
}
void func_001869c0(int a, int b, int c) { reset_note(1, a, b, c, 0); }
void func_00186148(int a) { reset_note(2, a, 0, 0, 0); }
int func_00185578(int a, int b) { reset_note(3, a, b, 0, 0); flip_note('P'); return flip_sync_result; }
void func_001849b0(void) { reset_note(4, 0, 0, 0, 0); }
void func_001885c8(void) { reset_note(5, 0, 0, 0, 0); }
void func_00184810(int a, int b, int c, int d) { reset_note(6, a, b, c, d); }
int *lRhBase_InitGsAndVif(int a) { reset_note(7, a, 0, 0, 0); return reset_buffer; }
void func_0017bb60(int a) { reset_note(8, a, 0, 0, 0); flip_note('F'); }
void func_00186470(int a, int b) { reset_note(9, a, b, 0, 0); }
char data_00325e80[16];
static int upload_args[8], upload_last;
static unsigned long upload_x;
void func_00185890(void *p, int a, int b, int c, int d, int e, int f, int g)
{
    int v[8] = {(int)(intptr_t)p, a, b, c, d, e, f, g};
    memcpy(upload_args, v, sizeof(v));
    reset_note(10, a, b, c, d);
}
void func_001859d0(void *p, unsigned long x) { upload_x = x; upload_last = p == data_00325e80; reset_note(11, 0, 0, 0, 0); }
void SzMsg_Free(int index) { slot_indices[slot_count++] = index; }
static DotrDrawWord chain_input, chain_result;

DotrDrawWord ShSlot_Draw_Pipe(DotrDrawWord cursor)
{
    assert(cursor == chain_input);
    chain_calls++;
    return chain_result;
}

/* Each cursor routine checks the incoming cursor and returns a new one. */
static DotrDrawWord step(DotrDrawWord cursor, int id, int a, int b, int c, int d)
{
    assert(cursor == 0x1000 && stage == 0);
    stage = id;
    last_args[0] = a;
    last_args[1] = b;
    last_args[2] = c;
    last_args[3] = d;
    return 0x2000 + id;
}

DotrDrawWord RhText_SetTextureFlush(DotrDrawWord cursor) { return step(cursor, 1, 0, 0, 0, 0); }
DotrDrawWord SzDuel_StartCardSession_Pipe(DotrDrawWord cursor) { return step(cursor, 5, 0, 0, 0, 0); }
DotrDrawWord SzDuel_FinishCardSession_Pipe(DotrDrawWord cursor) { return step(cursor, 6, 0, 0, 0, 0); }
DotrDrawWord ShFace_Draw_Pipe(DotrDrawWord cursor) { return step(cursor, 7, 0, 0, 0, 0); }
static int wide_args[6];
static int wide_last[5];

static DotrDrawWord wide_step(DotrDrawWord cursor, int id, int a, int b, int c, int d, int e)
{
    assert(cursor == 0x1000 && stage == 0);
    stage = id;
    wide_last[0] = a;
    wide_last[1] = b;
    wide_last[2] = c;
    wide_last[3] = d;
    wide_last[4] = e;
    return 0x2000 + id;
}

static DotrDrawWord wide_word;
DotrDrawWord SzDuel_SetupCard_Pipe(DotrDrawWord cursor, int a0, DotrDrawWord a1) { wide_word = a1; return wide_step(cursor, 60, a0, (int)a1, 0, 0, 0); }
DotrDrawWord SzFade_Draw_Pipe(DotrDrawWord cursor, int a0, int a1, int a2) { return wide_step(cursor, 61, a0, a1, a2, 0, 0); }
DotrDrawWord SzMsg_LoadFont_Pipe(DotrDrawWord cursor, int a0, int a1, int a2, int a3) { return wide_step(cursor, 62, a0, a1, a2, a3, 0); }
DotrDrawWord SzMsg_DrawX_Pipe(DotrDrawWord cursor, int a0, int a1, int a2, int a3, int a4) { return wide_step(cursor, 63, a0, a1, a2, a3, a4); }
DotrDrawWord SzMsg_DrawGlass_Pipe(DotrDrawWord cursor, int a0, int a1, int a2, int a3) { return wide_step(cursor, 64, a0, a1, a2, a3, 0); }
DotrDrawWord SzText_Load_Pipe(DotrDrawWord cursor, int a0, int a1, int a2) { return wide_step(cursor, 65, a0, a1, a2, 0, 0); }
DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, int a0, int a1, int a2) { return wide_step(cursor, 66, a0, a1, a2, 0, 0); }
DotrDrawWord ExSortDispBlur_Pipe(DotrDrawWord cursor, int a0, int a1, int a2, int a3, int a4) { return wide_step(cursor, 67, a0, a1, a2, a3, a4); }
DotrDrawWord SzMsg_Init_Pipe(DotrDrawWord cursor, int a0, int a1, int *out) { *out = 900; return wide_step(cursor, 70, a0, a1, 0, 0, 0); }
DotrDrawWord SzText_DoRequest_Pipe(DotrDrawWord cursor, int a0, int a1, int *out) { *out = 901; return wide_step(cursor, 71, a0, a1, 0, 0, 0); }
DotrDrawWord SzModel_ShotTextureWithFunc_Pipe(DotrDrawWord cursor, int a, int b, int c, int d, int e, int f)
{
    wide_args[0] = a;
    wide_args[1] = b;
    wide_args[2] = c;
    wide_args[3] = d;
    wide_args[4] = e;
    wide_args[5] = f;
    return cursor + 0x3000;
}
signed char Move_END_flg[1];
DotrDrawWord func_001dbd30_cursor(DotrDrawWord cursor, int a) { return step(cursor, 30, a, 0, 0, 0); }
DotrDrawWord SzModel_DrawMotionBlur_Pipe(DotrDrawWord cursor, int a) { return step(cursor, 31, a, 0, 0, 0); }
DotrDrawWord TkCamp_Draw_Pipe(DotrDrawWord cursor) { return step(cursor, 32, 0, 0, 0, 0); }
DotrDrawWord lSzGame_DrawTitle_Pipe(DotrDrawWord cursor) { return step(cursor, 8, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawStageAfter_Pipe(DotrDrawWord cursor) { return step(cursor, 9, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawDialog_Pipe(DotrDrawWord cursor) { return step(cursor, 10, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawWindow_Pipe(DotrDrawWord cursor) { return step(cursor, 11, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawJoinFade_Pipe(DotrDrawWord cursor) { return step(cursor, 12, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawJoinCard_Pipe(DotrDrawWord cursor) { return step(cursor, 13, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawBuildDeck_Pipe(DotrDrawWord cursor) { return step(cursor, 14, 0, 0, 0, 0); }
DotrDrawWord SzDuel_Draw3DView_Pipe(DotrDrawWord cursor) { return step(cursor, 15, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawTrade_Pipe(DotrDrawWord cursor) { return step(cursor, 16, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawCustomDuel_Pipe(DotrDrawWord cursor) { return step(cursor, 17, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawMapEdit_Pipe(DotrDrawWord cursor) { return step(cursor, 18, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawAllCard_Pipe(DotrDrawWord cursor) { return step(cursor, 19, 0, 0, 0, 0); }
DotrDrawWord SzDuel_DrawUnit_Pipe(DotrDrawWord cursor) { return step(cursor, 20, 0, 0, 0, 0); }
DotrDrawWord SzEvent_Draw_Pipe(DotrDrawWord cursor) { return step(cursor, 21, 0, 0, 0, 0); }
DotrDrawWord RhZSort_Finish_Pipe(DotrDrawWord cursor)
{
    if (flip_active) {
        assert(cursor == 0x1000);
        flip_note('z');
        return (DotrDrawWord)flip_packet;
    }
    return step(cursor, 22, 0, 0, 0, 0);
}
DotrDrawWord RhBase_SetZBufferMode(DotrDrawWord cursor, int a) { return step(cursor, 2, a, 0, 0, 0); }
static int packet_calls, packet_args[4];
DotrDrawWord ExFrameBuffer2Texture_Pipe(DotrDrawWord cursor, int a, int b, int c, int d)
{
    if (cursor == 0) { /* direct call from ExAssignFrameBuffer: record only */
        packet_calls++;
        packet_args[0] = a; packet_args[1] = b; packet_args[2] = c; packet_args[3] = d;
        return 0;
    }
    return step(cursor, 4, a, b, c, d);
}

void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    copy_destination = destination;
    copy_source = source;
    copy_count = count;
    copy16_log[copy_calls & 3][0] = destination;
    copy16_log[copy_calls & 3][1] = source;
    copy16_count[copy_calls++ & 3] = count;
    return (char *)destination + 16 * count;
}

static int fill_calls;
static int copy_log[2][16];
static int copy_n;
int *RhMem_CopyMemory4(int *destination, int *source, int count)
{
    memcpy(copy_log[copy_n++ & 1], source, 4 * count);
    memcpy(destination, source, 4 * count);
    return destination;
}
long *RhMem_FillMemory8(long *destination, long value, int count)
{
    int i;
    fill_calls++;
    for (i = 0; i < count; i++)
        destination[i] = value;
    return destination;
}

static void check_cursor(void)
{
    RhBase_SetVifPacketArea(0x1234);
    assert(data_003261d0[0] == 0x1234 && RhBase_GetVifPacketArea() == 0x1234);
    /* Wrappers: read the cursor, call one routine, store its result. */
    data_003261d0[0] = 0x1000;
    stage = 0;
    ExFlushTexture();
    assert(stage == 1 && data_003261d0[0] == 0x2001);
    data_003261d0[0] = 0x1000;
    stage = 0;
    ExZBufferMode(-7);
    assert(stage == 2 && last_args[0] == -7 && data_003261d0[0] == 0x2002);
    data_003261d0[0] = 0x1000;
    stage = 0;
    /* 0x2085a0 runs the real 0x2085e0: a rejected index leaves the
     * cursor unchanged; an accepted one is checked in check_packets(). */
    ExDitherMatrixEz(9);
    assert(stage == 0 && data_003261d0[0] == 0x1000);
    data_003261d0[0] = 0x1000;
    stage = 0;
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzDuel_StartCardSession();
    assert(stage == 5 && data_003261d0[0] == 0x2005);
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzDuel_FinishCardSession();
    assert(stage == 6 && data_003261d0[0] == 0x2006);
    data_003261d0[0] = 0x1000;
    stage = 0;
    ShFace_Draw();
    assert(stage == 7 && data_003261d0[0] == 0x2007);
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzDuel_DrawCard(-3);
    assert(stage == 30 && last_args[0] == -3 && data_003261d0[0] == 0x201e);
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzModel_DrawMotionBlur(4);
    assert(stage == 31 && last_args[0] == 4 && data_003261d0[0] == 0x201f);
    /* Gated wrapper: runs only while the byte at 0x34f710 is zero. */
    data_003261d0[0] = 0x1000;
    stage = 0;
    Move_END_flg[0] = 1;
    TkCamp_Draw();
    assert(stage == 0 && data_003261d0[0] == 0x1000);
    Move_END_flg[0] = 0;
    TkCamp_Draw();
    assert(stage == 32 && data_003261d0[0] == 0x2020);
    {
        static void (*const wrappers[])(void) = {lSzGame_DrawTitle, SzDuel_DrawStageAfter, SzDuel_DrawDialog, SzDuel_DrawWindow, SzDuel_DrawJoinFade, SzDuel_DrawJoinCard, SzDuel_DrawBuildDeck, SzDuel_Draw3DView, SzDuel_DrawTrade, SzDuel_DrawCustomDuel, SzDuel_DrawMapEdit, SzDuel_DrawAllCard, SzDuel_DrawUnit, SzEvent_Draw, RhZSort_Finish};
        int i;
        for (i = 0; i < (int)(sizeof(wrappers) / sizeof(wrappers[0])); i++) {
            data_003261d0[0] = 0x1000;
            stage = 0;
            wrappers[i]();
            assert(stage == 8 + i && data_003261d0[0] == 0x2008 + i);
        }
    }
    {
        /* 0x207fa0 zeroes four 64-bit words, sets two bytes and returns the
         * end; 0x207f60 applies it at the cursor and returns the old cursor. */
        struct DotrDrawBlock blocks[2];
        unsigned char expected[32];
        memset(blocks, 0xff, sizeof(blocks));
        memset(expected, 0, sizeof(expected));
        expected[3] = 80;
        expected[19] = 32;
        assert(ExSetAnchor_Pipe(&blocks[0]) == &blocks[1]);
        assert(memcmp(&blocks[0], expected, sizeof(expected)) == 0);
        assert(((unsigned char *)&blocks[1])[0] == 0xff);
        memset(blocks, 0xff, sizeof(blocks));
        data_003261d0[0] = (DotrDrawWord)&blocks[0];
        assert(ExSetAnchor() == (DotrDrawWord)&blocks[0]);
        assert(data_003261d0[0] == (DotrDrawWord)&blocks[1]);
        assert(memcmp(&blocks[0], expected, sizeof(expected)) == 0);
    }
    data_003261d0[0] = 0x1000;
    stage = 0;
    ExFrameBuffer2Texture(1, -2, 3, -4);
    assert(stage == 4 && data_003261d0[0] == 0x2004);
    assert(last_args[0] == 1 && last_args[1] == -2 && last_args[2] == 3 && last_args[3] == -4);
}

static void check_state(void)
{
    struct DotrDrawState expected;
    int a = 0, b = 0;
    assert(offsetof(struct DotrDrawState, unknown_100) == 0x100);
    assert(offsetof(struct DotrDrawState, unknown_1f0) == 0x1f0);
    assert(offsetof(struct DotrDrawState, unknown_23c) == 0x23c);
    assert(offsetof(struct DotrDrawState, unknown_268) == 0x268);
    memset(&gRhBase_dl, 0x3c, sizeof(gRhBase_dl));
    gRhBase_dl.unknown_23c = 11;
    gRhBase_dl.unknown_240 = 12;
    gRhBase_dl.unknown_244 = 2.5f;
    gRhBase_dl.unknown_248 = 13;
    gRhBase_dl.unknown_260 = 14;
    gRhBase_dl.unknown_264 = 15;
    gRhBase_dl.unknown_268 = 1000;
    assert(RhBase_GetScreenSize(&a, &b) == 1 && a == 11 && b == 12);
    assert(RhBase_GetScreenCenter(&a, &b) == 1 && a == 14 && b == 15);
    assert(RhBase_GetAspectY(0) == 2.5f);
    assert(RhBase_GetAspectY(&a) == 2.5f && a == 13);
    /* The selector word, times 32, is added only when the flag bit is set. */
    gRhBase_rl.unknown_10 = 3;
    data_00325f40[0].unknown_bit_0 = 0;
    assert(RhBase_GetUserGsAddress() == 1000);
    data_00325f40[0].unknown_bit_0 = -1;
    assert(RhBase_GetUserGsAddress() == 1000 + 96);
    memcpy(&expected, &gRhBase_dl, sizeof(expected));
    expected.unknown_100[0] = expected.unknown_1f0[0] = 0x81;
    expected.unknown_100[1] = expected.unknown_1f0[1] = 0x02;
    expected.unknown_100[2] = expected.unknown_1f0[2] = 0xff;
    expected.unknown_100[3] = expected.unknown_1f0[3] = 0x40;
    expected.unknown_104 = expected.unknown_1f4 = 0;
    assert(RhBase_SetClearColorEx(0x181, 2, -1, 0x40) == 1);
    assert(memcmp(&expected, &gRhBase_dl, sizeof(expected)) == 0);
    assert(RhBase_SetVSyncCallback(-77) == 1 && data_003261e4[0] == -77);
}

static void check_tables_and_packets(void)
{
    struct { struct DotrDrawPacket header; long entries[4]; } packet;
    struct DotrDrawTag tags[6];
    char source[64];
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 7; j++) {
            gExPrim_aaRegs[i][j].unknown_00 = 0x100000000L * (i + 1) + j;
            gExPrim_aaRegs[i][j].unknown_08 = -1;
            assert(ExGetRegVal(i, j) == 0x100000000L * (i + 1) + j);
        }
        assert(ExGetRegVal(i, -1) == -1 && ExGetRegVal(i, 7) == -1);
    }
    for (i = 0; i < 4; i++) {
        gExPrim_aqwAlpha[i] = -0x200000000L - i;
        assert(ExGetAlphaVal(i) == -0x200000000L - i);
    }
    assert(ExGetAlphaVal(-1) == 0 && ExGetAlphaVal(4) == 0);

    memset(&packet, 0, sizeof(packet));
    packet.header.unknown_count = 2;
    packet.header.unknown_flags = 0x0021;
    assert(ExAssignAlpha(&packet.header, 0x7766554433221100L) == 1);
    assert(packet.entries[2] == 0x7766554433221100L && packet.entries[0] == 0 && packet.entries[3] == 0);
    assert(packet.header.unknown_flags == 0x0221 && packet.header.unknown_count == 2);

    memset(tags, 0x5a, sizeof(tags));
    assert(RhVu1Mac_ReserveChangeDrawMethod(&tags[0]) == &tags[2]);
    assert(tags[0].unknown_00 == 0x20000000 && tags[0].unknown_04 == (DotrDrawWord)&tags[2]);
    assert(tags[0].unknown_08 == 0 && tags[0].unknown_0c == 0);
    assert(RhVu1Mac_SetViewScreenMatrix(&tags[0], source) == &tags[5]);
    assert(tags[0].unknown_00 == 0x10000004 && tags[0].unknown_04 == 0 && tags[0].unknown_08 == 0);
    assert(tags[0].unknown_0c == 0x6c040044);
    assert(copy_destination == (void *)&tags[1] && copy_source == (void *)source && copy_count == 4);
    assert(RhVu1Mac_SetScreenWorldMatrix(&tags[0], source) == &tags[5] && tags[0].unknown_0c == 0x6c040056);
}

static void check_flagged_entry(void)
{
    struct Fixture {
        long before[2];
        struct DotrDrawPacket header;
        long entries[8];
        long after[2];
    } packet, expected;
    static const long values[] = {0, -1, LONG_MIN, LONG_MAX, 0x123456789abcdef0L};
    unsigned int flags;
    int count;
    assert(sizeof(long) == 8 && sizeof(packet.header) == 16);
    assert(offsetof(struct Fixture, entries) == offsetof(struct Fixture, header) + 16);
    for (flags = 0; flags <= 0xffff; flags++) {
        for (count = 0; count <= 4; count++) {
            memset(&packet, 0xa5, sizeof(packet));
            packet.header.unknown_flags = (unsigned short)flags;
            packet.header.unknown_count = (unsigned short)count;
            memcpy(&expected, &packet, sizeof(expected));
            if (flags & 0x20) {
                expected.header.unknown_flags |= 0x4000;
                expected.entries[count + 3] = values[count];
            }
            assert(ExAssignClamp(&packet.header, values[count]) == ((flags & 0x20) != 0));
            assert(memcmp(&packet, &expected, sizeof(packet)) == 0);
        }
    }
    packet.header.unknown_count = 0xffff;
    packet.header.unknown_flags = 0xffdf;
    memcpy(&expected, &packet, sizeof(expected));
    assert(ExAssignClamp(&packet.header, -1) == 0);
    assert(memcmp(&packet, &expected, sizeof(packet)) == 0);
}

static void expect_packet(const long *q, long value, long code)
{
    const unsigned char *bytes = (const unsigned char *)q;
    const int *words = (const int *)q;
    /* Halfword 0 holds 2, byte 3 holds 16; words 2 and 3 are fixed codes. */
    assert(((const unsigned short *)q)[0] == 2 && bytes[2] == 0 && bytes[3] == 16 && words[1] == 0);
    assert(words[2] == 0x11000000 && words[3] == 0x51000002);
    assert(q[2] == 0x1000000000008001 && q[3] == 14 && q[4] == value && q[5] == code);
    assert(q[6] == 0x5a5a5a5a5a5a5a5a);
}

static void check_packets(void)
{
    static const int codes[] = {48, 49, 50, 58, 57, 0};
    static const int limits[] = {0x7fffffff, 0xffffff, 0xffff, 0xffff, -7, -7};
    long q[8];
    union {
        struct DotrDrawTag tags[5];
        int words[20];
        float floats[20];
    } t;
    struct DotrDrawVector4 a, b;
    float c, d, e;
    int index, flags, x, y, z, mode;
    size_t n;

    memset(q, 0x5a, sizeof(q));
    assert(ExClampMode_Pipe((struct DotrDrawPacket *)q, 0x123456789abcL) == (struct DotrDrawPacket *)q + 3);
    expect_packet(q, 0x123456789abcL, 8);
    memset(q, 0x5a, sizeof(q));
    assert(ExDitherMode_Pipe((struct DotrDrawPacket *)q, -2) == (struct DotrDrawPacket *)q + 3);
    expect_packet(q, -2, 69);
    for (index = -1; index <= 4; index++) {
        memset(q, 0x5a, sizeof(q));
        if (index < 0 || index > 3) {
            assert(ExDitherMatrixEz_Pipe((DotrDrawWord)q, index) == (DotrDrawWord)q && q[0] == 0x5a5a5a5a5a5a5a5a);
        } else {
            assert(ExDitherMatrixEz_Pipe((DotrDrawWord)q, index) == (DotrDrawWord)(q + 6));
            expect_packet(q, g_aqwExDitherMatrix[index], 68);
        }
    }
    /* Through the cursor wrapper. */
    memset(q, 0x5a, sizeof(q));
    data_003261d0[0] = (DotrDrawWord)q;
    ExDitherMatrixEz(2);
    assert(data_003261d0[0] == (DotrDrawWord)(q + 6));
    expect_packet(q, 0x3333, 68);

    /* 0x208d40: selected word and the saved flags. */
    for (flags = 0; flags < 0x80; flags++) {
        int with;
        for (with = 0; with < 2; with++) {
            int want = (flags & 0x40) ? ((flags & 0x20) ? 628 : 232) : ((flags & 0x20) ? 420 : 48);
            if ((flags & 2) && with) want = (flags & 0x20) ? 1748 : 1408;
            memset(&t, 0x5a, sizeof(t));
            gRhVu1Mac_State.unknown_5c = with ? 9 : 0;
            assert((int *)RhVu1Mac_ChangeDrawMethod(t.tags, flags) == t.words + 8 && gRhVu1Mac_State.unknown_00 == flags);
            assert(t.words[0] == 0x10000001 && t.words[1] == 0 && t.words[2] == 0x10000000);
            assert(t.words[3] == 0x68014000 && t.words[4] == want && t.words[5] == 0 && t.words[6] == 0);
            assert(t.words[7] == 0x10000000 && t.words[8] == 0x5a5a5a5a);
        }
    }

    /* 0x2093e0 stores its scale pairs and arguments; 0x209490 reads them. */
    memset(&t, 0x5a, sizeof(t));
    assert((int *)RhVu1Mac_SetFogParameterEx(t.tags, 1.0f, 3.0f, 2.0f, 6.0f) == t.words + 8);
    assert(t.words[0] == 0x10000001 && t.words[1] == 0 && t.words[2] == 0x10000000 && t.words[3] == 0x6c014053);
    assert(t.floats[4] == 255.0f / 4.0f && t.floats[5] == 2.0f * (255.0f / 4.0f));
    assert(t.floats[6] == 255.0f / 2.0f && t.floats[7] == 3.0f * (255.0f / 2.0f) && t.words[8] == 0x5a5a5a5a);
    assert(gRhVu1Mac_State.unknown_10[0] == t.floats[6] && gRhVu1Mac_State.unknown_10[1] == t.floats[7]);
    assert(gRhVu1Mac_State.unknown_10[2] == t.floats[4] && gRhVu1Mac_State.unknown_10[3] == t.floats[5]);
    assert(gRhVu1Mac_State.unknown_20[0] == 1.0f && gRhVu1Mac_State.unknown_20[1] == 3.0f);
    assert(gRhVu1Mac_State.unknown_20[2] == 2.0f && gRhVu1Mac_State.unknown_20[3] == 6.0f);
    for (mode = 0; mode < 4; mode++) {
        memset(&t, 0x5a, sizeof(t));
        assert((int *)RhVu1Mac_SetFogOnOff(t.tags, mode & 1, mode & 2) == t.words + 8);
        assert(t.words[0] == 0x10000001 && t.words[1] == 0 && t.words[2] == 0x10000000 && t.words[3] == 0x6c014053);
        assert(t.floats[6] == ((mode & 1) ? gRhVu1Mac_State.unknown_10[0] : 0.0f));
        assert(t.floats[7] == ((mode & 1) ? gRhVu1Mac_State.unknown_10[1] : 255.0f));
        assert(t.floats[4] == ((mode & 2) ? gRhVu1Mac_State.unknown_10[2] : 0.0f));
        assert(t.floats[5] == ((mode & 2) ? gRhVu1Mac_State.unknown_10[3] : 0.0f) && t.words[8] == 0x5a5a5a5a);
    }

    /* 0x209320 */
    for (mode = 0; mode < 3; mode++) {
        memset(&t, 0x5a, sizeof(t));
        assert((int *)RhVu1Mac_SetClipParameter(t.tags, mode, 4.0f, 10.0f, 20.0f) == t.words + 16);
        assert(t.words[0] == 0x10000001 && t.words[1] == 0 && t.words[2] == 0 && t.words[3] == 0x6c014052);
        assert(t.floats[4] == 645.0f / 8.0f && t.floats[5] == -645.0f / 8.0f);
        assert(t.floats[6] == 0.01f + 10.0f && t.floats[7] == 20.0f - 0.01f);
        assert(t.words[8] == 0x10000001 && t.words[9] == 0 && t.words[10] == 0 && t.words[11] == 0x6c01405a);
        assert(t.words[12] == (mode == 1) && t.words[13] == 0 && t.words[14] == 0 && t.words[15] == 0);
        assert(t.words[16] == 0x5a5a5a5a);
    }

    /* 0x209270 copies the two vectors and three floats when asked. */
    gRhVu1Mac_State.unknown_30.x = 1.0f;
    gRhVu1Mac_State.unknown_30.unknown_w = 4.0f;
    gRhVu1Mac_State.unknown_40.y = 6.0f;
    gRhVu1Mac_State.unknown_50 = 7.0f;
    gRhVu1Mac_State.unknown_54 = 8.0f;
    gRhVu1Mac_State.unknown_58 = 9.0f;
    gRhVu1Mac_State.unknown_5c = 0x777;
    assert(RhVu1Mac_GetPointLight(0, 0, 0, 0, 0) == 0x777 && RhVu1Mac_GetPointLight(&a, &b, &c, &d, &e) == 0x777);
    assert(memcmp(&a, &gRhVu1Mac_State.unknown_30, sizeof(a)) == 0 && memcmp(&b, &gRhVu1Mac_State.unknown_40, sizeof(b)) == 0);
    assert(c == 7.0f && d == 8.0f && e == 9.0f);

    /* 0x209d80: the word at 0x3261a8, or the selector word when both flag
     * bits are set, picks the limit; unknown codes leave it untouched. */
    for (n = 0; n < sizeof(codes) / sizeof(codes[0]); n++) {
        for (mode = 0; mode < 4; mode++) {
            int use_selector = mode == 3;
            memset(data_00325f20_view, 0, sizeof(data_00325f20_view));
            data_00325f20_view[0].unknown_20.unknown_bit_0 = (mode & 1) ? -1 : 0;
            data_00325f20_view[0].unknown_20.unknown_bit_1 = (mode & 2) ? -1 : 0;
            data_00325f20_view[0].unknown_18 = use_selector ? codes[n] : 0;
            data_003261a8[0] = use_selector ? 0 : codes[n];
            x = 5;
            y = -7;
            assert(RhBase_GetZMinMax(&x, &y) == 1 && x == 0 && y == limits[n]);
        }
    }

    /* 0x20a340 packs three bytes into the value doubleword. */
    {
        unsigned char bytes[3] = {0x12, 0x34, 0xf6};
        const int *words = (const int *)q;
        memset(q, 0x5a, sizeof(q));
        assert(RhBase_SetFogColor((struct DotrDrawPacket *)q, bytes) == (struct DotrDrawPacket *)q + 3);
        assert(words[0] == 0x10000002 && words[1] == 0 && words[2] == 0 && words[3] == 0x51000002);
        assert(q[2] == 0x1000000000008001 && q[3] == 14 && q[4] == 0x1234f6 && q[5] == 61);
        assert(q[6] == 0x5a5a5a5a5a5a5a5a);
    }
    /* 0x20a9e0 clears both flag bits of the selector view. */
    memset(data_00325f20_view, 0xff, sizeof(data_00325f20_view));
    RhBase_FreeRenderFrame();
    assert(!data_00325f20_view[0].unknown_20.unknown_bit_0 && !data_00325f20_view[0].unknown_20.unknown_bit_1);
    assert(data_00325f20_view[0].unknown_20.unknown_bits_2_7 == -1 && data_00325f20_view[0].unknown_18 == -1);

    /* Three four-vertex writers that differ only in vertex offsets. */
    {
        static const int offsets[3][4] = {{32, 48, 64, 80}, {24, 40, 56, 72}, {32, 56, 80, 104}};
        void (*const writers[3])(void *, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) = {
            TAKU_FT4_XYWH, TAKU_G4_XYWH, TAKU_GT4_XYWH};
        unsigned long buffer[16], want[16];
        int w, v;
        for (w = 0; w < 3; w++) {
            memset(buffer, 0xff, sizeof(buffer));
            memcpy(want, buffer, sizeof(want));
            writers[w](buffer, 1, 2, 0x1111111, 0x1a5, -1728, -1824, 0x2222222, 0x2b6,
                       5, 6, -1, 0x3c7, 0x10000, 0x20000, 0x4444444, 0x4d8);
            for (v = 0; v < 4; v++) {
                static const int xs[4] = {1, -1728, 5, 0x10000}, ys[4] = {2, -1824, 6, 0x20000};
                static const int zs[4] = {0x1111111, 0x2222222, -1, 0x4444444}, fs[4] = {0x1a5, 0x2b6, 0x3c7, 0x4d8};
                struct DotrDrawVertex *vertex = DOTR_DRAW_VERTEX(want, offsets[w][v]);
                vertex->unknown_x = (unsigned)((xs[v] + 1728) << 4) & 0xffff;
                vertex->unknown_y = (unsigned)((ys[v] + 1824) << 4) & 0xffff;
                vertex->unknown_z = (unsigned)zs[v] & 0xffffff;
                vertex->unknown_f = (unsigned)fs[v] & 0xff;
            }
            assert(memcmp(buffer, want, sizeof(want)) == 0);
        }
    }

    /* 0x217f90: two vertices of a rectangle; the second adds the size. */
    {
        unsigned long buffer[8], want[8];
        memset(buffer, 0xff, sizeof(buffer));
        memcpy(want, buffer, sizeof(want));
        TAKU_SP_XYWH(buffer, 10, 20, 100, 50, 0x1234567, 0x180);
        DOTR_DRAW_VERTEX(want, 32)->unknown_x = (10 + 1728) << 4;
        DOTR_DRAW_VERTEX(want, 32)->unknown_y = (20 + 1824) << 4;
        DOTR_DRAW_VERTEX(want, 48)->unknown_x = (110 + 1728) << 4;
        DOTR_DRAW_VERTEX(want, 48)->unknown_y = (70 + 1824) << 4;
        DOTR_DRAW_VERTEX(want, 32)->unknown_z = 0x234567;
        DOTR_DRAW_VERTEX(want, 48)->unknown_z = 0x234567;
        DOTR_DRAW_VERTEX(want, 32)->unknown_f = 0x80;
        DOTR_DRAW_VERTEX(want, 48)->unknown_f = 0x80;
        assert(memcmp(buffer, want, sizeof(want)) == 0);
    }

    /* 0x209ef0 */
    memset(&gRhBase_dl, 0, sizeof(gRhBase_dl));
    *(unsigned short *)&gRhBase_dl.unknown_000[0x60] = 0xfe03;
    *(unsigned short *)&gRhBase_dl.unknown_108[0x48] = 0x01ff;
    *(int *)&gRhBase_dl.unknown_24c[4] = 0x1234;
    *(int *)&gRhBase_dl.unknown_24c[12] = 0x5678;
    assert(RhBase_GetBufferFormatEx(0, 0, 0, 0) == 1);
    for (mode = 0; mode < 4; mode++) {
        int low = mode & 1, argument = (mode & 2) ? 7 : 0;
        gRhBase_dl.unknown_26c = 0x100 | low;
        x = y = z = -1;
        assert(RhBase_GetBufferFormatEx(argument, &x, &y, &z) == 1 && y == 0x1234 && z == 0x5678);
        assert(x == ((low == (argument != 0)) ? 0x1ff << 5 : 0x003 << 5));
    }
}

/* 0x1d9e70 runs the real 0x20a9e0, 0x208680 and 0x2084d0 with the slot calls stubbed. */
static void check_reset_and_slots(void)
{
    unsigned long q[8], r[8];
    struct DotrDrawPacket *result;

    memset(q, 0x5a, sizeof(q));
    memset(r, 0x5a, sizeof(r));
    memset(data_00325f20_view, 0xff, sizeof(data_00325f20_view));
    slot_count = 0;
    result = SzDuel_FinishCardSessionX_Pipe((struct DotrDrawPacket *)q);
    assert(!data_00325f20_view[0].unknown_20.unknown_bit_0 && !data_00325f20_view[0].unknown_20.unknown_bit_1);
    assert(result == ExClampMode_Pipe((struct DotrDrawPacket *)r, ExGetRegVal(0, 4)) - (struct DotrDrawPacket *)r + (struct DotrDrawPacket *)q);
    assert(result != (struct DotrDrawPacket *)q && memcmp(q, r, sizeof(q)) == 0);
    assert(slot_count == 3 && slot_indices[0] == 0 && slot_indices[1] == 1 && slot_indices[2] == 2);
}

static void check_chain_insert(void)
{
    /* Slots sit on an 8-byte stride; the host struct is wider, so address them by byte offset. */
    DotrDrawSlot slots[16] = {{0, 0}};
#define SLOT(i) (*(DotrDrawSlot *)((char *)slots + (i) * 8))
    DotrDrawNode a = {0, 0}, b = {0, 0}, c = {0, 0}, d = {0, 0};
    DotrDrawChain chain = {slots, 0, 0, 3};
    unsigned char packet[32];

    memset(packet, 0xff, sizeof(packet));
    assert(ExSortAnchor_Pipe(packet, &a, &chain, 2) == packet + 16);
    assert(SLOT(2).head == &a && a.next == 0);
    for (int i = 0; i < 16; i++)
        assert(packet[i] == (i == 3 ? 96 : 0));
    assert(packet[16] == 0xff);
    ExSortAnchor_Pipe(packet, &b, &chain, 2);
    assert(SLOT(2).head == &b && b.next == &a);
    ExSortAnchor_Pipe(packet, &c, &chain, 6);
    assert(SLOT(6).head == &c && c.next == 0);
    ExSortAnchor_Pipe(packet, &d, &chain, 0xffffffffu);
    assert(SLOT(6).head == &d && d.next == &c);
    {
        /* 0x207fd0: same insert through the stored packet area, which advances by 16. */
        DotrDrawNode e = {0, 0};
        DotrDrawWord saved = RhBase_GetVifPacketArea();

        memset(packet, 0xff, sizeof(packet));
        RhBase_SetVifPacketArea((DotrDrawWord)packet);
        ExSortAnchor(&e, &chain, 2);
        assert(RhBase_GetVifPacketArea() == (DotrDrawWord)(packet + 16));
        assert(SLOT(2).head == &e && e.next == &b);
        assert(packet[3] == 96 && packet[16] == 0xff);
        RhBase_SetVifPacketArea(saved);
    }
#undef SLOT
}

/* 0x207aa0 ExSortNode_Pipe. One case per branch outcome: type 7 and count 0 / -1 leave the
 * buffer alone; no chain, a chain with the last-slot bit, and the slot bound at size - 2 (below,
 * equal, above); every flag bit of the primitive word; the optional header pairs (0x200, 0x20,
 * 0x4000 with and without 0x20, 0x8000); per vertex the first/later colour word, 1/q, the
 * texture word with register 2 (divided or not) and 3; the closing pairs and the tag fields.
 * The Z-buffer local is only partly written, so only its three written fields are compared.
 * The tag link is the low half of a host pointer. */
static unsigned long sort_buf[48];
static struct DotrSortHeader sort_header;
static struct DotrSortVertex sort_vertex[2];

static unsigned long sort_floats(float a, float b)
{
    unsigned long w;
    float f[2];

    f[0] = a;
    f[1] = b;
    memcpy(&w, f, sizeof(w));
    return w;
}

static int sort_run(int type, int count, DotrDrawChain *chain, int z)
{
    int i;
    DotrDrawWord end;

    for (i = 0; i < 48; i++) {
        sort_buf[i] = 0xa5a5a5a5a5a5a5a5UL;
    }
    sort_header.count = 0x7777;
    sort_header.type = (unsigned short)type;
    sort_header.regs = 0x1111;
    sort_header.data[0] = 0xd0;
    sort_header.data[1] = 0xd1;
    sort_header.data[2] = 0xd2;
    sort_header.data[3] = 0xd3;
    for (i = 0; i < 2; i++) {
        sort_vertex[i].q = i ? 4.0f : 2.0f;
        sort_vertex[i].xyz = 0x100 + i;
        sort_vertex[i].colour = sort_floats(3.0f + i, 99.0f);
        sort_vertex[i].uv = sort_floats(8.0f, 6.0f);
    }
    end = ExSortNode_Pipe((DotrDrawWord)sort_buf, &sort_header, count, sort_vertex, chain, z);
    return (int)((unsigned long *)end - sort_buf);
}

static void sort_assert_tag(int words, unsigned int next)
{
    struct DotrSortTag *tag = (struct DotrSortTag *)sort_buf;
    int size = words / 2 - 1;

    assert(tag->size == size && tag->unknown_02 == 0 && tag->kind == 0x10 && (unsigned int)tag->next == next);
    assert(tag->unknown_08 == 0x11000000 && tag->unknown_0c == (0x51000000 | size));
    assert(sort_buf[2] == (0x1000000000008000UL | (unsigned long)(size - 1)) && sort_buf[3] == 0xe && sort_buf[5] == 0);
    assert(sort_buf[words] == 0xa5a5a5a5a5a5a5a5UL);
}

static void check_sort_node(void)
{
    DotrDrawSlot slots[16] = {{0, 0}};
#define SLOT(i) (*(DotrDrawSlot *)((char *)slots + (i) * 8))
    DotrDrawChain chain = {slots, 0, 0, 3};
    DotrDrawNode old = {0, 0};
    struct DotrSortZBuf zbuf;
    int i;

    assert(sort_run(0x8207, 2, &chain, 1) == 0 && sort_buf[0] == 0xa5a5a5a5a5a5a5a5UL && SLOT(7).head == 0);
    assert(sort_run(0, 0, &chain, 1) == 0 && sort_buf[0] == 0xa5a5a5a5a5a5a5a5UL && SLOT(1).head == 0);
    assert(sort_run(0, -1, &chain, 1) == 0 && sort_buf[0] == 0xa5a5a5a5a5a5a5a5UL);

    /* No flags, no chain: the first vertex gives colour and position, the second position only. */
    assert(sort_run(0, 2, 0, 1) == 14);
    sort_assert_tag(14, 0);
    assert(sort_buf[4] == 0 && sort_buf[6] == 0x5000c && sort_buf[7] == 0x47);
    assert(sort_buf[8] == sort_floats(3.0f, 99.0f) && sort_buf[9] == 1 && sort_buf[10] == 0x100 && sort_buf[11] == 4);
    assert(sort_buf[12] == 0x101 && sort_buf[13] == 4);

    /* Each flag bit of the primitive word on its own (count 1; only the word and length matter). */
    assert(sort_run(0x0005, 1, 0, 0) == 12 && sort_buf[4] == 5);
    assert(sort_run(0x0010, 1, 0, 0) == 12 && sort_buf[4] == 0x008);
    assert(sort_run(0x0020, 1, 0, 0) == 18 && sort_buf[4] == 0x010);
    assert(sort_run(0x0100, 1, 0, 0) == 12 && sort_buf[4] == 0x020);
    assert(sort_run(0x0200, 1, 0, 0) == 14 && sort_buf[4] == 0x040 && sort_buf[6] == 0x5000d);
    assert(sort_buf[8] == 0xd0 && sort_buf[9] == 0x42);
    assert(sort_run(0x0400, 1, 0, 0) == 12 && sort_buf[4] == 0x080);
    assert(sort_run(0x0800, 1, 0, 0) == 12 && sort_buf[4] == 0x100);
    assert(sort_run(0x1000, 1, 0, 0) == 12 && sort_buf[4] == 0x200 && sort_buf[7] == 0x48);
    assert(sort_run(0x2000, 1, 0, 0) == 12 && sort_buf[4] == 0x400);
    /* 0x4000 without 0x20 adds nothing. */
    assert(sort_run(0x4000, 1, 0, 0) == 12 && sort_buf[4] == 0);
    sort_assert_tag(12, 0);

    /* Slot bound: below, at and above size - 2 = 6; the old head goes into the tag. */
    SLOT(2).head = &old;
    assert(sort_run(0, 1, &chain, 2) == 12 && SLOT(2).head == (DotrDrawNode *)sort_buf);
    sort_assert_tag(12, (unsigned int)(DotrDrawWord)&old);
    assert(sort_run(0, 1, &chain, 6) == 12 && SLOT(6).head == (DotrDrawNode *)sort_buf);
    sort_assert_tag(12, 0);
    SLOT(6).head = &old;
    assert(sort_run(0, 1, &chain, 7) == 12 && SLOT(6).head == (DotrDrawNode *)sort_buf && SLOT(7).head == 0);
    sort_assert_tag(12, (unsigned int)(DotrDrawWord)&old);
    assert(sort_run(0, 1, &chain, -1) == 12 && SLOT(6).head == (DotrDrawNode *)sort_buf && SLOT(7).head == 0);

    /* Texture pairs with register 3 (0x800): 1/q is stored, the texture word is not divided. */
    assert(sort_run(0x0830, 1, 0, 0) == 18 && sort_buf[4] == 0x118);
    assert(sort_buf[8] == 0xd1 && sort_buf[9] == 6 && sort_buf[10] == 0xd2 && sort_buf[11] == 0x14);
    assert(sort_buf[12] == sort_floats(3.0f, 0.5f) && sort_buf[13] == 1);
    assert(sort_buf[14] == sort_floats(8.0f, 6.0f) && sort_buf[15] == 3 && sort_buf[16] == 0x100 && sort_buf[17] == 4);
    /* Register 2 without 0x10: not divided, and the second vertex has no colour word. */
    assert(sort_run(0x0020, 2, 0, 0) == 22);
    assert(sort_buf[12] == sort_floats(3.0f, 99.0f) && sort_buf[14] == sort_floats(8.0f, 6.0f) && sort_buf[15] == 2);
    assert(sort_buf[18] == sort_floats(8.0f, 6.0f) && sort_buf[19] == 2 && sort_buf[20] == 0x101 && sort_buf[21] == 4);
    sort_assert_tag(22, 0);

    /* Everything but 0x800, context 1, last slot through bit 0x8000. */
    data_00325f40[0].unknown_bit_0 = 0;
    data_00325fc0_fields[0].low_9 = 0x123;
    data_00325fc0_fields[0].low_4 = 5;
    for (i = 0; i < 7; i++) {
        gExPrim_aaRegs[1][i].unknown_00 = 0x9000 + i;
        gExPrim_aaRegs[1][i].unknown_08 = 0x9100 + i;
        gExPrim_aaRegs[0][i].unknown_00 = 0x8000 + i;
        gExPrim_aaRegs[0][i].unknown_08 = 0x8100 + i;
    }
    SLOT(7).head = &old;
    assert(sort_run(0xf733, 2, &chain, 1) == 34 && SLOT(7).head == (DotrDrawNode *)sort_buf && SLOT(1).head == 0);
    sort_assert_tag(34, (unsigned int)(DotrDrawWord)&old);
    assert(sort_buf[4] == 0x6fb && sort_buf[6] == 0x5000d && sort_buf[7] == 0x48);
    assert(sort_buf[8] == 0xd0 && sort_buf[9] == 0x43 && sort_buf[10] == 0xd1 && sort_buf[11] == 7);
    assert(sort_buf[12] == 0xd2 && sort_buf[13] == 0x15 && sort_buf[14] == 0xd3 && sort_buf[15] == 9);
    memcpy(&zbuf, &sort_buf[16], sizeof(zbuf));
    assert(zbuf.fields.low_9 == 0x123 && zbuf.fields.low_4 == 5 && zbuf.mask == 0 && sort_buf[17] == 0x4f);
    assert(sort_buf[18] == sort_floats(3.0f, 0.5f) && sort_buf[19] == 1);
    assert(sort_buf[20] == sort_floats(4.0f, 3.0f) && sort_buf[21] == 2 && sort_buf[22] == 0x100 && sort_buf[23] == 4);
    assert(sort_buf[24] == sort_floats(4.0f, 0.25f) && sort_buf[25] == 1);
    assert(sort_buf[26] == sort_floats(2.0f, 1.5f) && sort_buf[27] == 2 && sort_buf[28] == 0x101 && sort_buf[29] == 4);
    assert(sort_buf[30] == 0x9004 && sort_buf[31] == 0x9104);
    memcpy(&zbuf, &sort_buf[32], sizeof(zbuf));
    assert(zbuf.fields.low_9 == 0x123 && zbuf.fields.low_4 == 5 && zbuf.mask == 1 && sort_buf[33] == 0x4f);
    /* Context 0 takes the other row and the lower register numbers. */
    assert(sort_run(0x4020, 1, 0, 0) == 22 && sort_buf[12] == 0xd3 && sort_buf[13] == 8);
    assert(sort_buf[20] == 0x8004 && sort_buf[21] == 0x8104);
    assert(sort_run(0x8000, 1, 0, 0) == 16 && sort_buf[9] == 0x4e && sort_buf[15] == 0x4e);
    assert(sort_header.count == 0x7777 && sort_header.regs == 0x1111);
#undef SLOT
}

/* 0x1fcde0: the packed colour buffer is a stack local passed as an int, which
 * the 64-bit host truncates, so its bytes are not checked; only the call,
 * return value and entry updates are. */
static int fade_step(int index, int b4, int b5, int b6, int bits07, int bit0, int bit7)
{
    struct DotrBlockEntry *e = &gSzFade_fl[index];
    int result;
    memset(e, 0, sizeof(*e));
    e->unknown_04 = b4;
    e->unknown_05 = b5;
    e->unknown_06 = b6;
    e->unknown_07.unknown_bits_0_5 = bits07;
    e->unknown_07.unknown_bit_7 = bit7 ? -1 : 0;
    e->unknown_08.unknown_bit_0 = bit0;
    data_003261d0[0] = 0x1000;
    stage = 0;
    result = SzFade_Disp(index);
    return result;
}

static void check_fixed_words(void)
{
    /* 0x100610 writes fixed addresses: map that page on the host (a hint, not
     * MAP_FIXED, so nothing is replaced; the test fails if it is not granted). */
    static const int sizes[2][2] = {{640, 448}, {512, 224}};
    long *page = mmap((void *)0x12000000, 4096, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    int flag, k;

    assert(page == (long *)0x12000000);
    for (k = 0; k < 2; k++) {
        for (flag = -1; flag < 2; flag++) {
            long width = sizes[k][0], height = sizes[k][1];
            long fb = (long)0x155 | (width >> 6) << 9 | (long)(0x1234 + k) << 15;
            long display = (width * 4 - 1) << 32 | (height - 1) << 44;
            memset(page, 0x5a, 4096);
            memset(&gRhBase_dl, 0, sizeof(gRhBase_dl));
            gRhBase_dl.unknown_23c = (int)width;
            gRhBase_dl.unknown_240 = (int)height;
            *(unsigned short *)&gRhBase_dl.unknown_108[0x48] = 0xff55;
            *(int *)&gRhBase_dl.unknown_24c[4] = 0x1234 + k;
            useSoftFocus(flag);
            /* Zero gives 0xff in the second byte, anything else 0x7f. */
            assert(page[0] == (flag == 0 ? 0xff23 : 0x7f23));
            assert(page[0x90 / 8] == fb && page[0x70 / 8] == fb);
            assert(page[0xa0 / 8] == (display | 0x183227c));
            assert(page[0x80 / 8] == (display | 0x183327c));
            assert(page[0xd0 / 8] == 0);
            /* Nothing else on the page is written. */
            for (width = 0; width < 512; width++) {
                if (width != 0 && width != 14 && width != 16 && width != 18 && width != 20 && width != 26) {
                    assert(page[width] == 0x5a5a5a5a5a5a5a5aL);
                }
            }
        }
    }
    munmap(page, 4096);
}

static void check_frame_packet(void)
{
    long packet[8];
    int *words = (int *)packet;
    int mode, low;
    /* Both flag bits choose the selector words and table row; any other pair,
     * the four-byte record chosen by the low bit of the state word. */
    for (mode = 0; mode < 4; mode++) {
        for (low = 0; low < 2; low++) {
            unsigned long expected;
            memset(packet, 0x5a, sizeof(packet));
            memset(&gRhBase_dl, 0, sizeof(gRhBase_dl));
            memset(&data_00325f20_frame, 0, sizeof(data_00325f20_frame));
            memset(data_00293f1c, 0, sizeof(data_00293f1c));
            gRhBase_dl.unknown_26c = 0x100 | low;
            *(unsigned short *)&gRhBase_dl.unknown_000[0x60] = 0xfe03;
            gRhBase_dl.unknown_000[0x62] = 0xca;
            gRhBase_dl.unknown_000[0x63] = 0xf1;
            *(unsigned short *)&gRhBase_dl.unknown_108[0x48] = 0x0155;
            gRhBase_dl.unknown_108[0x4a] = 0x47;
            gRhBase_dl.unknown_108[0x4b] = 0x82;
            data_00325f20_frame.unknown_00 = 2;
            data_00325f20_frame.unknown_0c = 0x123;
            data_00325f20_frame.unknown_14 = 0x31;
            data_00325f20_frame.unknown_20.unknown_bit_0 = (mode & 1) ? -1 : 0;
            data_00325f20_frame.unknown_20.unknown_bit_1 = (mode & 2) ? -1 : 0;
            *(int *)(data_00293f1c + 80) = 0x280;
            if (mode == 3) {
                expected = 0x89abcdef00000000UL | 0x31UL << 24 | 0x0aUL << 16 | 0x123;
            } else if (low) {
                expected = 0x89abcdef00000000UL | 0x02UL << 24 | 0x07UL << 16 | 0x155;
            } else {
                expected = 0x89abcdef00000000UL | 0x31UL << 24 | 0x0aUL << 16 | 0x003;
            }
            assert(RhBase_MaskFBuffer(packet, 0x89abcdefU) == packet + 6);
            assert(words[0] == 0x10000002 && words[1] == 0 && words[2] == 0x11000000);
            assert(words[3] == 0x51000002);
            assert(packet[2] == 0x1000000000008001L && packet[3] == 14);
            assert((unsigned long)packet[4] == expected && packet[5] == 76);
            assert(packet[6] == 0x5a5a5a5a5a5a5a5aL);
        }
    }
}

static void check_fade_step(void)
{
    struct DotrBlockEntry *e = &gSzFade_fl[1];
    assert(fade_step(0, 1, 1, 4, 0, 0, 0) == 1 && stage == 61);
    assert(SzFade_Disp(-1) == 0 && SzFade_Disp(2) == 0);
    /* bit 6 already set: no call */
    fade_step(1, 1, 1, 4, 0, 0, 0);
    e->unknown_07.unknown_bit_6 = -1;
    stage = 0;
    assert(SzFade_Disp(1) == 0 && stage == 0);
    /* finished with bit 7 set: bit 6 becomes set */
    assert(fade_step(1, 1, 4, 4, 0, 0, 1) == 1 && stage == 61);
    assert(wide_last[0] == 0 && wide_last[2] == 0xffffff && e->unknown_07.unknown_bit_6 != 0 && e->unknown_05 == 4);
    /* finished with bit 7 clear: unchanged */
    assert(fade_step(1, 1, 5, 4, 0, 0, 0) == 1 && e->unknown_07.unknown_bit_6 == 0 && e->unknown_05 == 5);
    /* running, bit 0 clear: advance, then clamp */
    assert(fade_step(1, 1, 2, 4, 3, 0, 0) == 1 && e->unknown_05 == 3);
    assert(fade_step(1, 5, 2, 4, 3, 0, 0) == 1 && e->unknown_05 == 4);
    /* running, bit 0 set: unchanged */
    assert(fade_step(1, 1, 2, 4, 3, 1, 0) == 1 && e->unknown_05 == 2);
}

static void check_packet_link(void)
{
    /* low (32-bit addressable) page: head buffer at 1024, entries at 2048 */
    unsigned char *page = mmap(0, 4096, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    DotrPktCtx ctx;
    DotrPktEnt *ent = (DotrPktEnt *)(page + 2048);
    char *head = (char *)(page + 1024);

    assert(page != MAP_FAILED);
    memset(&ctx, 0, sizeof ctx);
    ctx.ent = ent;
    ctx.w2 = head;
    data_003261d0[0] = 0x1234;

    /* every entry b == 7: nothing written */
    memset(page, 0, 4096);
    ctx.shift = 1;
    ent[1].b = 7; ent[1].next = (DotrDrawWord)&ent[0];
    ent[0].b = 7; ent[0].next = -1;
    ExBindPrim(&ctx);
    assert(head[3] == 0 && *(int *)(head + 4) == 0);
    assert(ent[0].next == -1 && ent[1].next == (int)(intptr_t)&ent[0]);

    /* one live entry: linked to the head, terminated with the cursor */
    memset(page, 0, 4096);
    ctx.shift = 0;
    ent[0].b = 1; ent[0].next = -1;
    ExBindPrim(&ctx);
    assert(head[3] == 32 && *(int *)(head + 4) == (int)(intptr_t)&ent[0]);
    assert(((char *)&ent[0])[3] == 32 && ent[0].next == 0x1234);

    /* two live entries: head -> ent[1] -> ent[0] -> cursor */
    memset(page, 0, 4096);
    ctx.shift = 1;
    ent[1].b = 1; ent[1].next = (DotrDrawWord)&ent[0];
    ent[0].b = 2; ent[0].next = -1;
    ExBindPrim(&ctx);
    assert(head[3] == 32 && *(int *)(head + 4) == (int)(intptr_t)&ent[1]);
    assert(((char *)&ent[1])[3] == 32 && ent[1].next == (int)(intptr_t)&ent[0]);
    assert(((char *)&ent[0])[3] == 32 && ent[0].next == 0x1234);

    /* tail byte 80 in the middle: spills 32 bytes on, next entry hangs off +16 */
    memset(page, 0, 4096);
    ctx.shift = 1;
    ent[1].b = 0x5001; ent[1].next = (DotrDrawWord)&ent[0];
    ent[0].b = 1; ent[0].next = -1;
    ExBindPrim(&ctx);
    assert(*(int *)((char *)&ent[1] + 4) == (int)(intptr_t)((char *)&ent[1] + 32));
    assert(((char *)&ent[1])[19] == 32);
    assert(*(int *)((char *)&ent[1] + 20) == (int)(intptr_t)&ent[0]);
    assert(((char *)&ent[0])[3] == 32 && ent[0].next == 0x1234);

    /* tail byte 80 on the last entry */
    memset(page, 0, 4096);
    ctx.shift = 0;
    ent[0].b = 0x5001; ent[0].next = -1;
    ExBindPrim(&ctx);
    assert(*(int *)((char *)&ent[0] + 4) == (int)(intptr_t)((char *)&ent[0] + 32));
    assert(((char *)&ent[0])[19] == 32 && *(int *)((char *)&ent[0] + 20) == 0x1234);
    munmap(page, 4096);
}

static void check_packet_context(void)
{
    /* low (32-bit addressable) page: packet cursor at 0, w1 buffer at 1024,
     * entries at 2048; the host ctx is wider than the target one so w1 is
     * seeded and the target's cursor-to-w1 handoff is not observable */
    unsigned char *page = mmap(0, 4096, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    DotrPktCtx ctx;
    DotrPktEnt *ent = (DotrPktEnt *)(page + 2048);
    unsigned short *w1 = (unsigned short *)(page + 1024);
    long *p = (long *)page;
    int i;

    assert(page != MAP_FAILED);
    memset(page, 0xaa, 4096);
    memset(&ctx, 0, sizeof ctx);
    ctx.w1 = w1;
    w1[0] = 0;
    data_003261d0[0] = (DotrDrawWord)page;
    ExClearOt(&ctx, ent, 0);
    assert(p[0] == 0 && p[1] == 0 && p[2] == 0x100000000000800eL && p[3] == 14);
    assert(copy_destination == p + 4 && copy_source == gExPrim_aaRegs && copy_count == 14);
    assert(w1[0] == 15 && ((unsigned char *)w1)[3] == 16);
    assert(*(int *)((char *)w1 + 8) == 0x11000000);
    assert(*(int *)((char *)w1 + 12) == (15 | 0x51000000));
    assert(ctx.w2 == (char *)(p + 4) + 16 * 14 && ctx.w2[3] == 16);
    assert((((long *)ctx.w2)[0] & ~0xff000000L) == 0 && ((long *)ctx.w2)[1] == 0);
    assert(data_003261d0[0] == (DotrDrawWord)(ctx.w2 + 16));
    assert(ctx.shift == 0 && ctx.ent == ent);
    assert(ent[0].next == -1 && ent[0].b == 7 && ent[0].a == 0);

    /* shift 2: four entries linked backwards, first terminated */
    memset(ent, 0xaa, 4 * sizeof *ent);
    w1[0] = 0x1234;
    data_003261d0[0] = (DotrDrawWord)page;
    ExClearOt(&ctx, ent, 2);
    assert(ctx.shift == 2 && ctx.ent == ent);
    assert(*(int *)((char *)w1 + 12) == (15 | 0x51000000));
    for (i = 0; i < 4; i++)
        assert(ent[i].b == 7 && ent[i].a == 0);
    assert(ent[0].next == -1);
    for (i = 1; i < 4; i++)
        assert(ent[i].next == (int)(intptr_t)&ent[i - 1]);
    munmap(page, 4096);
}

static void check_trailer_packet(void)
{
    unsigned short p[64] = {0};
    unsigned char *base = (unsigned char *)(p + 8);

    /* flag 0x20 clear: returns 0, no call */
    p[0] = 3; p[1] = 0xffdf;
    assert(ExAssignFrameBuffer(p, 9) == 0 && packet_calls == 0);
    /* flag set: returns 1, call args 0,0,b,end+8,end+16 */
    p[1] = 0x20;
    assert(ExAssignFrameBuffer(p, 9) == 1 && packet_calls == 1);
    assert(packet_args[0] == 0 && packet_args[1] == 9);
    assert(packet_args[2] == (int)(intptr_t)(base + 3 * 8 + 8));
    assert(packet_args[3] == (int)(intptr_t)(base + 3 * 8 + 16));
}

static void check_view_packet(void)
{
    int buf[16];
    float a[4] = {9.0f, 1.0f, 2.0f, 3.0f};
    float b[4] = {4.0f, 5.0f, 6.0f, 7.0f};
    float *w = (float *)buf;
    struct DotrDrawTag *end;

    /* null a or null b: header, zero fill, unknown_5c cleared */
    gRhVu1Mac_State.unknown_5c = 7;
    memset(buf, 0x55, sizeof(buf));
    end = RhVu1Mac_SetPointLightToVu1((struct DotrDrawTag *)buf, 0, b, 1.0f, 3.0f, 4.0f);
    assert((char *)end == (char *)buf + 48 && fill_calls == 1 && gRhVu1Mac_State.unknown_5c == 0);
    assert(buf[0] == 0x10000002 && buf[1] == 0 && buf[2] == 0x11000000 && buf[3] == 0x6c020054);
    memset(buf, 0x55, sizeof(buf));
    gRhVu1Mac_State.unknown_5c = 7;
    end = RhVu1Mac_SetPointLightToVu1((struct DotrDrawTag *)buf, a, 0, 1.0f, 3.0f, 4.0f);
    assert((char *)end == (char *)buf + 48 && fill_calls == 2 && gRhVu1Mac_State.unknown_5c == 0);
    /* both set: vectors, scaled depth terms, view copy */
    memset(buf, 0, sizeof(buf));
    end = RhVu1Mac_SetPointLightToVu1((struct DotrDrawTag *)buf, a, b, 1.0f, 3.0f, 4.0f);
    assert((char *)end == (char *)buf + 48 && fill_calls == 2);
    assert(buf[0] == 0x10000002 && buf[3] == 0x6c020054);
    assert(w[4] == 1.0f && w[5] == 2.0f && w[6] == 3.0f && w[7] == 6.0f);
    assert(w[8] == 4.0f && w[9] == 5.0f && w[10] == 6.0f && w[11] == 2.0f);
    assert(memcmp(&gRhVu1Mac_State.unknown_30, a, 16) == 0 && memcmp(&gRhVu1Mac_State.unknown_40, b, 16) == 0);
    assert(gRhVu1Mac_State.unknown_50 == 1.0f && gRhVu1Mac_State.unknown_54 == 3.0f);
    assert(gRhVu1Mac_State.unknown_58 == 4.0f && gRhVu1Mac_State.unknown_5c == 1);
}

static void check_row_reset(void)
{
    static const int order[] = {1, 2, 3, 4, 5, 6, 7, 8, 3, 9, 3};
    int k;

    *(short *)(gRhSys_asd + 80) = -5;
    *(short *)(data_00293f14 + 80) = 6;
    *(short *)(data_00293f18 + 80) = -7;
    *(int *)(data_00293f1c + 80) = 0x1234;
    *(int *)(data_00293f20 + 80) = -0x4321;
    data_00326180[17] = 111;
    data_00326180[19] = 222;
    memset(reset_buffer, 0x55, sizeof(reset_buffer));
    RhBase_ResetGsEnv(2);
    assert(reset_count == 11);
    for (k = 0; k < 11; k++)
        assert(reset_order[k] == order[k]);
    assert(reset_args[0][0] == 111 && reset_args[0][1] == 0 && reset_args[0][2] == 0);
    assert(reset_args[1][0] == 1);
    assert(reset_args[5][0] == 0 && reset_args[5][1] == -5 && reset_args[5][2] == 6 && reset_args[5][3] == -7);
    assert(data_00326180[3] == 0x1234 && data_00326180[4] == -0x4321);
    assert(reset_args[6][0] == 222);
    assert(reset_buffer[0] == 0x70000000 && reset_buffer[1] == 0 && reset_buffer[2] == 0 && reset_buffer[3] == 0);
    assert(reset_args[7][0] == 0);
    assert(reset_args[9][0] == 111 && reset_args[9][1] == 222);
}

static void check_upload(void)
{
    static const int order[] = {3, 10, 8, 11, 3};
    int k;

    /* flag set: halfword at +96; page bits 0x1ff shifted left 5 */
    data_00326180[15] = 1;
    *(unsigned short *)((char *)&gRhBase_dl + 96) = 0xfe05;
    *(unsigned short *)((char *)&gRhBase_dl + 336) = 0x0003;
    reset_count = 0;
    RhBase_CopyFrameBuffer(0xfedcba9876543210UL);
    assert(reset_count == 5);
    for (k = 0; k < 5; k++)
        assert(reset_order[k] == order[k]);
    assert(upload_args[0] == (int)(intptr_t)data_00325e80 && upload_args[1] == (short)(5 << 5));
    assert(upload_args[2] == 10 && upload_args[3] == 10 && upload_args[4] == 0 && upload_args[5] == 0);
    assert(upload_args[6] == 640 && upload_args[7] == 448);
    assert(upload_last && upload_x == (0xfedcba9876543210UL & 0xfffffffUL));
    /* flag clear (only bit 0 matters): halfword at +336; wraps to a negative short */
    data_00326180[15] = 0xfffffffe;
    *(unsigned short *)((char *)&gRhBase_dl + 336) = 0x01ff;
    RhBase_CopyFrameBuffer(1);
    assert(upload_args[1] == (short)(0x1ff << 5) && upload_x == 1);
}

static void check_row_colors(void)
{
    struct DotrDrawRowOwner owner;
    unsigned char rows[3 * 48];
    int count;

    for (count = -1; count <= 2; count++) {
        int flag;
        for (flag = 0; flag <= 1; flag++) {
            int i;
            memset(&owner, 0, sizeof(owner));
            memset(rows, 0xee, sizeof(rows));
            owner.color[0] = 1; owner.color[1] = 2; owner.color[2] = 3; owner.color[3] = 4;
            owner.rows = rows;
            owner.flag = flag ? -1 : 0;
            owner.count = count;
            stage = 0;
            assert(SzDuel_SetupCard2_Pipe(0x1000, 7, &owner) == 0x2000 + 60);
            assert(stage == 60 && wide_last[0] == 7 && wide_word == (DotrDrawWord)&owner);
            for (i = 0; i < 3; i++) {
                unsigned char *r = rows + i * 48;
                if (flag && i < count)
                    assert(r[24] == 3 && r[25] == 2 && r[26] == 1 && r[27] == 4 && r[23] == 0xee && r[28] == 0xee);
                else
                    assert(r[24] == 0xee && r[25] == 0xee && r[26] == 0xee && r[27] == 0xee);
            }
        }
    }
    stage = 0;
}

static int quad_depth_index = -1;
float SzView_GetScreenDistance(int index)
{
    quad_depth_index = index;
    return 7.5f;
}

static void check_quad_case(int mode, struct DotrDrawQuadScale *scale, float w, float h, float hw, float hh)
{
    struct DotrDrawQuadVertex v[5];
    static const int sx[4] = {-1, -1, 1, 1}, sy[4] = {-1, 1, 1, -1};
    int i;

    assert(sizeof(struct DotrDrawQuadVertex) == 48);
    memset(v, 0x33, sizeof(v));
    quad_depth_index = -1;
    assert(SzDuel_GetCardNodeUnitOnScreen(mode, 100.0f, v, scale, 200.0f) == 4 && quad_depth_index == 0);
    for (i = 0; i < 4; i++) {
        assert(v[i].x == 100.0f + sx[i] * hw && v[i].y == 200.0f + sy[i] * hh);
        assert(v[i].z == 7.5f && v[i].w == 1.0f && v[i].q == 1.0f);
        assert(v[i].color[0] == 0x80 && v[i].color[1] == 0x80 && v[i].color[2] == 0x80 && v[i].color[3] == 0x80);
        assert(v[i].u == (sx[i] > 0 ? w / 512.0f : 0.0f) && v[i].v == (sy[i] > 0 ? h / 512.0f : 0.0f));
        assert(v[i].pad_10[0] == 0x33 && v[i].pad_28[7] == 0x33);
    }
    assert(v[4].color[0] == 0x33);
}

static void check_quad(void)
{
    struct DotrDrawQuadScale scale = {0.5f, 2.0f};

    check_quad_case(0, 0, 120.0f, 180.0f, 60.0f, 90.0f);
    check_quad_case(1, 0, 256.0f, 368.0f, 128.0f, 184.0f);
    check_quad_case(0, &scale, 120.0f, 180.0f, 30.0f, 180.0f);
    check_quad_case(-3, &scale, 256.0f, 368.0f, 64.0f, 368.0f);
}

/* SzDuel_GetCardNodeOnScreen (branch coverage: both outcomes of the mode bit, the scale bit, the
 * second argument and the word at 0x317490+3156, and the loop bound through
 * the count the stub returns). ExAssignAlpha, ExAssignClamp and SzDuel_GetCardNodeUnitOnScreen
 * run for real; the other two callees are stubs. */
int data_00317490_drawwords[790];
static struct DotrDrawQuadBlock quad_block;
static struct DotrDrawQuadVertex quad_vertices[5];
static int quad_calls, quad_expected_count;

int func_001da5e0_caller(int mode, struct DotrDrawQuadVertex *out, float cx, float cy, struct DotrDrawQuadScale *scale)
{
    assert(quad_calls++ == 0);
    return SzDuel_GetCardNodeUnitOnScreen(mode, cx, out, scale, cy);
}

int Ex3d_ViewClip(int a, struct DotrDrawQuadVertex *b, int c, int d, int e)
{
    assert(quad_calls++ == 1 && a == 4 && b == quad_vertices && c == -5 && d == 0x1234 && e == 15);
    return 3;
}

static int node_mode, node_v2s_calls, node_v2s_count;

void Ex3d_NodeV2S(int a, int count, struct DotrDrawQuadVertex *v)
{
    if (node_mode) {
        assert(a == 0 && v == quad_vertices);
        node_v2s_count = count;
        ++node_v2s_calls;
        return;
    }
    assert(quad_calls++ == (quad_expected_count == 3 ? 2 : 1));
    assert(a == 0 && count == quad_expected_count && v == quad_vertices);
}

static void check_quad_block(void)
{
    int mode, scaled, second, word, i;
    long words[4];

    for (mode = 0; mode < 2; mode++)
    for (scaled = 0; scaled < 2; scaled++)
    for (second = 0; second < 2; second++)
    for (word = 0; word < 2; word++) {
        float hw = (mode ? 128.0f : 60.0f) * (scaled ? 0.5f : 1.0f);
        memset(&quad_block, 0x77, sizeof(quad_block));
        memset(quad_vertices, 0x33, sizeof(quad_vertices));
        quad_block.scale.x = 0.5f;
        quad_block.scale.y = 2.0f;
        quad_block.vertices = quad_vertices;
        quad_block.unknown_7c = 0x1234;
        quad_block.unknown_80_bits_0_11 = -5;
        quad_block.flag_12 = mode;
        quad_block.flag_14 = scaled;
        quad_block.count = 9;
        data_00317490_drawwords[789] = word ? 0x7fa1b2c3 : 0;
        quad_calls = 0;
        quad_expected_count = second ? 3 : 4;
        assert(SzDuel_GetCardNodeOnScreen(&quad_block, second, 100.0f, 200.0f) == 1);
        assert(quad_calls == (second ? 3 : 2) && quad_block.count == quad_expected_count);
        assert(quad_block.packet.unknown_count == 0 && quad_block.packet.unknown_flags == (53 | 0x200 | 0x4000));
        memcpy(words, quad_block.unknown_58, sizeof(words));
        assert(words[0] == 0x8000000044L && words[3] == (mode ? 0x5c00040000aL : 0x2d0001e000aL));
        assert(quad_block.flag_12 == -mode && quad_block.flag_14 == -scaled && quad_block.unknown_80_bits_0_11 == -5);
        assert(quad_vertices[0].x == 100.0f - hw);
        for (i = 0; i < 5; i++) {
            const unsigned char *p = quad_vertices[i].pad_10;
            if (word && i < quad_expected_count) {
                assert(p[4] == 0xc3 && p[5] == 0xb2 && p[6] == 0xa1);
            } else {
                assert(p[4] == 0x33 && p[5] == 0x33 && p[6] == 0x33);
            }
            assert(p[0] == 0x33 && p[3] == 0x33 && p[7] == 0x33);
        }
    }
}

/* SzDuel_GetCardNodeUnit (branch coverage: both outcomes of the mode test; every stored
 * field of the four vertices and the return value). */
static void check_node_unit(void)
{
    struct DotrDrawQuadVertex v[4];
    int mode, i;

    for (mode = 0; mode < 2; mode++) {
        float w = mode ? 256.0f : 120.0f, h = mode ? 368.0f : 180.0f;
        memset(v, 0x33, sizeof(v));
        assert(SzDuel_GetCardNodeUnit(mode ? -7 : 0, v) == 4);
        for (i = 0; i < 4; i++) {
            assert(v[i].x == (i < 2 ? -w : w) / 2.0f && v[i].y == (i == 0 || i == 3 ? h : -h) / 2.0f);
            assert(v[i].z == 0.0f && v[i].w == 1.0f && v[i].q == 1.0f);
            assert(v[i].color[0] == 0x80 && v[i].color[1] == 0x80 && v[i].color[2] == 0x80 && v[i].color[3] == 0x80);
            assert(v[i].u == (i < 2 ? 0.0f : w / 512.0f) && v[i].v == (i == 0 || i == 3 ? 0.0f : h / 512.0f));
            assert(v[i].pad_10[0] == 0x33 && v[i].pad_28[7] == 0x33);
        }
    }
}

/* SzDuel_GetCardNode callees: canned results, arguments recorded. SzDuel_GetCardNodeUnit,
 * ExAssignAlpha and ExAssignClamp run for real. */
struct DotrDrawQuadVector data_00291540 = { 1.0f, 2.0f, 3.0f, 4.0f };
struct DotrDrawQuadAngles data_00291550 = { 10, 11, 12, 13 }, data_00291560 = { 20, 21, 22, 23 };
struct DotrDrawQuadAngles data_00291570 = { 30, 31, 32, 33 }, data_00291580 = { 40, 41, 42, 43 };
static int node_push, node_pop, node_srt_calls, node_rot_calls, node_rev_calls;
static float *node_srt_m[2], *node_srt_scale[2], *node_srt_pos[2], *node_rot_m, *node_rev[2][2];
static int *node_srt_rot_ptr[2], node_srt_rot[2][4], node_rot_ang[4];
static void *node_world, *node_mul[2], *node_mulvec[3], *node_l2v_m, *node_view_out, *node_inner[2];
static struct DotrDrawQuadVector node_normal_in;
static int node_l2v_args[4], node_view_count;
static float node_inner_result;

void RhGeom_PushMatrix(float *matrix) { assert(matrix == NULL); ++node_push; }
void RhGeom_PopMatrix(void) { ++node_pop; }

void RhGeom_GetScaleRotXYZTransMatrix(float *m, float *scale, int *rot, float *pos)
{
    node_srt_m[node_srt_calls] = m;
    node_srt_scale[node_srt_calls] = scale;
    node_srt_rot_ptr[node_srt_calls] = rot;
    memcpy(node_srt_rot[node_srt_calls], rot, sizeof(node_srt_rot[0]));
    node_srt_pos[node_srt_calls++] = pos;
}

void RhGeom_GetRotXYZMatrix(float *m, int *ang)
{
    node_rot_m = m;
    memcpy(node_rot_ang, ang, sizeof(node_rot_ang));
    ++node_rot_calls;
}

void RhGeom_MulMatrixRev(float *m, float *other)
{
    node_rev[node_rev_calls][0] = m;
    node_rev[node_rev_calls++][1] = other;
}

void RhView_GetWorldViewMatrix(struct DotrViewMatrix *out) { node_world = out; }

void RhGeom_MulMatrix(float *m, float *other)
{
    node_mul[0] = m;
    node_mul[1] = other;
}

void RhGeom_MulMatrixVector(struct DotrViewVector *out, struct DotrViewMatrix *m, struct DotrViewVector *v)
{
    node_mulvec[0] = out;
    node_mulvec[1] = m;
    node_mulvec[2] = v;
    memcpy(&node_normal_in, v, sizeof(node_normal_in));
}

int Ex3d_NodeL2V(struct DotrViewMatrix *m, int count, struct DotrDrawQuadVertex *v, int a, int b, int c, float *average)
{
    assert(v == quad_vertices && average == NULL);
    node_l2v_m = m;
    node_l2v_args[0] = count;
    node_l2v_args[1] = a;
    node_l2v_args[2] = b;
    node_l2v_args[3] = c;
    return 3;
}

int Ex3d_GetViewVectorForNode(struct DotrDrawQuadVector *origin, int count, struct DotrDrawQuadVertex *points,
                              struct DotrDrawQuadVector *out)
{
    assert(origin == NULL && points == quad_vertices);
    node_view_count = count;
    node_view_out = out;
    return 0;
}

float RhGeom_InnerProduct(struct DotrDrawQuadVector *a, struct DotrDrawQuadVector *b)
{
    node_inner[0] = a;
    node_inner[1] = b;
    return node_inner_result;
}

/* SzDuel_GetCardNode (branch coverage: both outcomes of the clip argument, flag_13, flag_14,
 * flag_12 and the colour word, the loop bound through the count the stub returns, and the
 * return test at a positive, zero and negative inner product). */
static void check_node_block(void)
{
    static const float inner[3] = { 0.5f, 0.0f, -0.5f };
    int a, f13, f14, f12, word, i, r;
    long words[4];

    node_mode = 1;
    for (a = 0; a < 2; a++)
    for (f13 = 0; f13 < 2; f13++)
    for (f14 = 0; f14 < 2; f14++)
    for (f12 = 0; f12 < 2; f12++)
    for (word = 0; word < 2; word++)
    for (r = 0; r < 3; r++) {
        float *scale;
        memset(&quad_block, 0x77, sizeof(quad_block));
        memset(quad_vertices, 0x33, sizeof(quad_vertices));
        quad_block.turn.x = 101;
        quad_block.turn.y = 102;
        quad_block.turn.z = 103;
        quad_block.rot[0] = 201;
        quad_block.vertices = quad_vertices;
        quad_block.unknown_7c = 0x1234;
        quad_block.unknown_80_bits_0_11 = -5;
        quad_block.flag_12 = f12;
        quad_block.flag_13 = f13;
        quad_block.flag_14 = f14;
        quad_block.count = 9;
        data_00317490_drawwords[789] = word ? 0x7fa1b2c3 : 0;
        node_push = node_pop = node_srt_calls = node_rot_calls = node_rev_calls = node_v2s_calls = 0;
        node_inner_result = inner[r];
        scale = f14 ? (float *)&quad_block.scale : NULL;
        assert(SzDuel_GetCardNode(&quad_block, a ? 8 : 0) == (r == 0));
        assert(node_push == 1 && node_pop == 1 && node_srt_calls == 2 && node_v2s_calls == 1);
        assert(node_srt_scale[0] == NULL && node_srt_pos[0] == quad_block.pos);
        assert(node_srt_scale[1] == scale && node_srt_pos[1] == NULL);
        if (f13) {
            assert(node_rot_calls == 1 && node_rev_calls == 2 && node_srt_rot_ptr[0] == quad_block.rot);
            assert(node_rot_ang[0] == 20 && node_rot_ang[1] == 102 && node_rot_ang[2] == 22 && node_rot_ang[3] == 23);
            assert(node_srt_rot[1][0] == 101 && node_srt_rot[1][1] == 11 && node_srt_rot[1][2] == 103);
            assert(node_srt_rot[1][3] == 13);
            assert(node_rev[0][0] == node_rot_m && node_rev[0][1] == node_srt_m[0]);
            assert(node_rev[1][0] == node_srt_m[1] && node_rev[1][1] == node_rot_m);
        } else {
            assert(node_rot_calls == 0 && node_rev_calls == 1);
            assert(node_srt_rot[0][0] == 40 && node_srt_rot[0][1] == 102 && node_srt_rot[0][2] == 42);
            assert(node_srt_rot[0][3] == 43);
            assert(node_srt_rot[1][0] == 101 && node_srt_rot[1][1] == 31 && node_srt_rot[1][2] == 103);
            assert(node_srt_rot[1][3] == 33);
            assert(node_rev[0][0] == node_srt_m[1] && node_rev[0][1] == node_srt_m[0]);
        }
        assert(node_mul[0] == node_world && node_mul[1] == node_srt_m[1]);
        assert(node_mulvec[0] == node_mulvec[2] && node_mulvec[1] == node_world);
        assert(node_normal_in.x == 1.0f && node_normal_in.y == 2.0f && node_normal_in.z == 3.0f);
        assert(node_normal_in.w == 4.0f);
        assert(node_l2v_m == node_world && node_l2v_args[0] == 4 && node_l2v_args[1] == -5);
        assert(node_l2v_args[2] == 0x1234 && node_l2v_args[3] == (a ? 15 : -1));
        assert(node_view_count == 3 && node_v2s_count == 3 && quad_block.count == 3);
        assert(node_inner[0] == node_view_out && node_inner[1] == node_mulvec[0]);
        assert(quad_block.packet.unknown_count == 0 && quad_block.packet.unknown_flags == (53 | 0x200 | 0x4000));
        memcpy(words, quad_block.unknown_58, sizeof(words));
        assert(words[0] == 0x8000000044L && words[3] == (f12 ? 0x5c00040000aL : 0x2d0001e000aL));
        assert(quad_block.flag_12 == -f12 && quad_block.flag_13 == -f13 && quad_block.flag_14 == -f14);
        assert(quad_block.unknown_80_bits_0_11 == -5 && quad_block.turn.y == 102 && quad_block.rot[0] == 201);
        assert(quad_vertices[0].x == (f12 ? -128.0f : -60.0f));
        for (i = 0; i < 5; i++) {
            const unsigned char *p = quad_vertices[i].pad_10;
            if (word && i < 3) {
                assert(p[4] == 0xc3 && p[5] == 0xb2 && p[6] == 0xa1);
            } else {
                assert(p[4] == 0x33 && p[5] == 0x33 && p[6] == 0x33);
            }
            assert(p[0] == 0x33 && p[3] == 0x33 && p[7] == 0x33);
        }
    }
    node_mode = 0;
}

/* RhBase_GetZBufInfo (branch coverage: the four states of the two flag bits; on the
 * host bit 0 is read from data_00325f40 and bit 1 from the setup view, which
 * name the same byte on the target). */
static void check_frame_fields(void)
{
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;
    struct DotrDrawFields fields;
    int mode;

    assert(sizeof(struct DotrDrawFields) == 4);
    for (mode = 0; mode < 4; mode++) {
        memset(s, 0, sizeof(*s));
        s->unknown_0c = 0x1f0;
        s->unknown_1c = 0x21f;
        s->unknown_18 = 0x7a;
        data_00325f40[0].unknown_bit_0 = (mode & 1) ? -1 : 0;
        s->unknown_20.unknown_bit_1 = (mode & 2) ? -1 : 0;
        memset(data_00325fc0_fields, 0, sizeof(data_00325fc0_fields));
        data_00325fc0_fields[0].low_9 = 0x155;
        data_00325fc0_fields[0].unknown_bits_9_15 = 0x7f;
        data_00325fc0_fields[0].unknown_02 = 0x99;
        data_00325fc0_fields[0].low_4 = 0x6;
        data_00325fc0_fields[0].unknown_bits_4_7 = 0xf;
        memset(&fields, 0, sizeof(fields));
        fields.unknown_bits_9_15 = 0x2a;
        fields.unknown_02 = 0x44;
        fields.unknown_bits_4_7 = 0x5;
        RhBase_GetZBufInfo(&fields);
        if (mode == 3) {
            /* (0x1f0 + 0x21f) & 0x1ff and 0x7a & 0xf */
            assert(fields.low_9 == 0x00f && fields.low_4 == 0xa);
        } else {
            assert(fields.low_9 == 0x155 && fields.low_4 == 0x6);
        }
        assert(fields.unknown_bits_9_15 == 0x2a && fields.unknown_02 == 0x44 && fields.unknown_bits_4_7 == 0x5);
        assert(s->unknown_0c == 0x1f0 && data_00325fc0_fields[0].low_9 == 0x155);
    }
    data_00325f40[0].unknown_bit_0 = 0;
}

/* One 0x20aa30 call with both words filled with ones beforehand. */
static int frame_bits(int open, int w, int h, unsigned long *a, unsigned long *b)
{
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;

    memset(s, 0, sizeof(*s));
    s->unknown_00 = 2;
    s->unknown_0c = 0x155;
    s->unknown_14 = 0x53;
    s->unknown_20.unknown_bit_0 = open ? -1 : 0;
    memset(data_00293f1c, 0, sizeof(data_00293f1c));
    memset(data_00293f20, 0, sizeof(data_00293f20));
    *(int *)(data_00293f1c + 80) = w;
    *(int *)(data_00293f20 + 80) = h;
    if (a != 0) {
        *a = ~0UL;
    }
    if (b != 0) {
        *b = ~0UL;
    }
    return RhBase_GetTextureInfoForRender((struct DotrDrawBits64A *)a, (struct DotrDrawBits64B *)b);
}

static unsigned long bits_a(int w, int size_w, int size_h)
{
    return ((unsigned long)(0x155 << 5) & 0x3fff) | ((unsigned long)(w >> 6) & 0x3f) << 14
        | (unsigned long)(0x53 & 0x3f) << 20 | (unsigned long)size_w << 26 | (unsigned long)size_h << 30
        | 1UL << 34 | 1UL << 61;
}

/* RhBase_GetTextureInfoForRender (branch coverage). */
static void check_frame_bits(void)
{
    /* the bits `b` keeps (1, 10-18, 21-31, 44-63) stay ones; bit 5 and bits 6-8 = 1 */
    const unsigned long want_b = (~0UL & ~(1UL | 7UL << 2 | 7UL << 6 | 1UL << 9 | 3UL << 19 | 0xfffUL << 32))
        | 1UL << 6;
    unsigned long a, b;

    assert(sizeof(struct DotrDrawBits64A) == 8 && sizeof(struct DotrDrawBits64B) == 8);
    /* not open: nothing written */
    assert(frame_bits(0, 640, 448, &a, &b) == 0 && a == ~0UL && b == ~0UL);
    /* both null: only the answer */
    assert(frame_bits(1, 640, 448, 0, 0) == 1);
    /* each pointer alone */
    assert(frame_bits(1, 640, 448, &a, 0) == 1 && a == bits_a(640, 10, 9));
    b = 5;
    assert(frame_bits(1, 640, 448, 0, &b) == 1 && b == want_b);
    /* the two size boundaries: 320 / 321 and 224 / 225 */
    assert(frame_bits(1, 320, 224, &a, &b) == 1 && a == bits_a(320, 9, 8) && b == want_b);
    assert(frame_bits(1, 321, 224, &a, &b) == 1 && a == bits_a(321, 10, 8));
    assert(frame_bits(1, 320, 225, &a, &b) == 1 && a == bits_a(320, 9, 9));
    assert(frame_bits(1, 321, 225, &a, &b) == 1 && a == bits_a(321, 10, 9) && b == want_b);
}

/* One 0x20a470 call on index 2 with row words {first, second} at 0x293f30. */
static int frame_open(int mode, int kind, int first, int second, int base, int flags)
{
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;

    memset(s, 0x33, sizeof(*s));
    s->unknown_20.unknown_bit_0 = (flags & 1) ? -1 : 0;
    s->unknown_20.unknown_bit_1 = (flags & 2) ? -1 : 0;
    memset(data_00293f1c, 0, sizeof(data_00293f1c));
    memset(data_00293f20, 0, sizeof(data_00293f20));
    memset(data_00293f30, 0, sizeof(data_00293f30));
    *(int *)(data_00293f1c + 80) = 0x280;
    *(int *)(data_00293f20 + 80) = 0x1c0;
    ((int *)(data_00293f30 + 80))[0] = first;
    ((int *)(data_00293f30 + 80))[1] = second;
    gRhBase_dl.unknown_268 = base;
    return RhBase_AssignRenderFrame(2, mode, kind);
}

/* RhBase_AssignRenderFrame (branch coverage). */
static void check_frame_open(void)
{
    static const int modes[5][2] = { { 0, 1 }, { 1, 1 }, { 2, 0 }, { 10, 0 }, { 3, 0 } };
    static const int kinds[5][2] = { { 48, 1 }, { 49, 1 }, { 50, 0 }, { 58, 0 }, { 0, 0 } };
    static const int row[2] = { 100, 7 };
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;
    int m, k;

    /* already open: nothing stored, either state of bit 1 */
    assert(frame_open(0, 48, 100, 7, 64, 1) == 0);
    assert(s->unknown_00 == 0x33333333 && s->unknown_1c == 0x33333333);
    assert(s->unknown_20.unknown_bit_0 && !s->unknown_20.unknown_bit_1);
    assert(frame_open(0, 48, 100, 7, 64, 3) == 0);
    assert(s->unknown_14 == 0x33333333 && s->unknown_20.unknown_bit_1);
    /* every mode and kind case and the defaults */
    for (m = 0; m < 5; m++) {
        for (k = 0; k < 5; k++) {
            assert(frame_open(modes[m][0], kinds[k][0], row[0], row[1], 64 + 31, 2) == 1);
            assert(s->unknown_00 == 2 && s->unknown_04 == 0x280 && s->unknown_08 == 0x1c0);
            assert(s->unknown_14 == modes[m][0] && s->unknown_18 == kinds[k][0]);
            assert(s->unknown_0c == 2 && s->unknown_1c == row[modes[m][1]]);
            assert(s->unknown_10 == row[modes[m][1]] + row[kinds[k][1]]);
            assert(s->unknown_20.unknown_bit_0 && !s->unknown_20.unknown_bit_1);
            assert((s->unknown_20.unknown_bits_2_7 & 0x3f) == (0x33 >> 2));
        }
    }
    /* +0xc + +0x10 == 512 opens; 513 answers 0 with the words stored and the flags kept */
    assert(frame_open(10, 58, 255, 0, 2 << 5, 0) == 1);
    assert(s->unknown_0c == 2 && s->unknown_10 == 510 && s->unknown_20.unknown_bit_0);
    assert(frame_open(10, 58, 255, 0, 3 << 5, 2) == 0);
    assert(s->unknown_0c == 3 && s->unknown_10 == 510 && s->unknown_1c == 255 && s->unknown_14 == 10);
    assert(!s->unknown_20.unknown_bit_0 && s->unknown_20.unknown_bit_1);
}

/* RhBase_EnableRenderFrame (branch coverage: flag bit 0 clear / set, with bit 1 clear / set
 * beforehand; every written word is checked and the word after them is kept). */
static void check_frame_setup(void)
{
    long packet[13];
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;
    int mode, i;

    assert(offsetof(struct DotrDrawFrameSetup, unknown_14) == 0x14);
    assert(offsetof(struct DotrDrawFrameSetup, unknown_20) == 0x20);
    for (mode = 0; mode < 4; mode++) {
        memset(s, 0, sizeof(*s));
        memset(data_00293f1c, 0, sizeof(data_00293f1c));
        for (i = 0; i < 13; i++) {
            packet[i] = 0x5a5a5a5a5a5a5a5a;
        }
        s->unknown_00 = 2;
        s->unknown_04 = 0x280;
        s->unknown_08 = 0x1c0;
        s->unknown_0c = 0x123;
        s->unknown_14 = 0x31;
        s->unknown_18 = 0x22;
        s->unknown_1c = 0x40;
        s->unknown_20.unknown_bit_0 = (mode & 1) ? -1 : 0;
        s->unknown_20.unknown_bit_1 = (mode & 2) ? -1 : 0;
        s->unknown_20.unknown_bits_2_7 = 0x15;
        *(int *)(data_00293f1c + 80) = 0x280;
        if (!(mode & 1)) {
            assert(RhBase_EnableRenderFrame(packet) == packet);
            for (i = 0; i < 13; i++) {
                assert(packet[i] == 0x5a5a5a5a5a5a5a5a);
            }
            assert(!s->unknown_20.unknown_bit_0 && !s->unknown_20.unknown_bit_1 == !(mode & 2));
            continue;
        }
        assert(RhBase_EnableRenderFrame(packet) == packet + 12);
        assert(((int *)packet)[0] == 0x10000005 && ((int *)packet)[1] == 0);
        assert(((int *)packet)[2] == 0x11000000 && ((int *)packet)[3] == 0x51000005);
        assert(packet[2] == 0x1000000000008004 && packet[3] == 14);
        assert(packet[4] == (0x123L | (long)(0x280 >> 6) << 16 | 0x31L << 24) && packet[5] == 76);
        assert(packet[6] == ((long)(0x123 + 0x40) | 0x22L << 24) && packet[7] == 78);
        assert(packet[8] == 1 && packet[9] == 74);
        assert(packet[10] == (0x27fL << 16 | 0x1bfL << 48) && packet[11] == 64);
        assert(packet[12] == 0x5a5a5a5a5a5a5a5a);
        assert(s->unknown_20.unknown_bit_0 && s->unknown_20.unknown_bit_1);
        assert(s->unknown_20.unknown_bits_2_7 == 0x15 && s->unknown_1c == 0x40 && s->unknown_00 == 2);
    }
}

/* 0x20a760: flag bit 0 clear (packet untouched) and set, with the +0x26c word odd and even
 * (block at +336 or +96); every written word is checked and the word after them is kept. */
static void check_frame_close(void)
{
    long packet[13];
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;
    struct DotrDrawState *r = &gRhBase_dl;
    int mode, i;

    for (mode = 0; mode < 4; mode++) {
        long *block = (long *)((char *)r + ((mode & 2) ? 336 : 96));

        memset(s, 0, sizeof(*s));
        memset(r, 0, sizeof(*r));
        for (i = 0; i < 13; i++) {
            packet[i] = 0x5a5a5a5a5a5a5a5a;
        }
        for (i = 0; i < 4; i++) {
            ((long *)((char *)r + 96))[i] = 0x1111111100000000 + i;
            ((long *)((char *)r + 336))[i] = 0x2222222200000000 + i;
        }
        r->unknown_26c = (mode & 2) ? 0x11 : 0x10;
        r->unknown_23c = 0x280;
        r->unknown_240 = 0x1c0;
        s->unknown_20.unknown_bit_0 = (mode & 1) ? -1 : 0;
        s->unknown_20.unknown_bit_1 = -1;
        s->unknown_20.unknown_bits_2_7 = 0x15;
        if (!(mode & 1)) {
            assert(RhBase_DisableRenderFrame(packet) == packet);
            for (i = 0; i < 13; i++) {
                assert(packet[i] == 0x5a5a5a5a5a5a5a5a);
            }
            assert(!s->unknown_20.unknown_bit_0 && s->unknown_20.unknown_bit_1);
            continue;
        }
        assert(RhBase_DisableRenderFrame(packet) == packet + 12);
        assert(((int *)packet)[0] == 0x10000005 && ((int *)packet)[1] == 0);
        assert(((int *)packet)[2] == 0x11000000 && ((int *)packet)[3] == 0x51000005);
        assert(packet[2] == 0x1000000000008004 && packet[3] == 14);
        for (i = 0; i < 4; i++) {
            assert(packet[4 + i] == block[i]);
        }
        assert(packet[8] == 0 && packet[9] == 74);
        assert(packet[10] == (0x27fL << 16 | 0x1bfL << 48) && packet[11] == 64);
        assert(packet[12] == 0x5a5a5a5a5a5a5a5a);
        assert(s->unknown_20.unknown_bit_0 && !s->unknown_20.unknown_bit_1);
        assert(s->unknown_20.unknown_bits_2_7 == 0x15);
    }
    memset(r, 0, sizeof(*r));
}

struct DotrDrawColor data_00293f00 = {{0x11, 0x22, 0x33, 0x44}};
static long *clear_packet;
static int clear_args[10];
static int clear_calls;

int func_00184ff8(long *p, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
    clear_packet = p;
    clear_args[0] = a1; clear_args[1] = a2; clear_args[2] = a3; clear_args[3] = a4; clear_args[4] = a5;
    clear_args[5] = a6; clear_args[6] = a7; clear_args[7] = a8; clear_args[8] = a9; clear_args[9] = a10;
    clear_calls++;
    return 3;
}

/* 0x20a870: either flag bit clear (packet untouched, no call), then both set with a null colour
 * (the 0x293f00 record) and with a supplied one; every written word and forwarded argument. */
static void check_frame_clear(void)
{
    static const int fixed[5] = {2, 0, 0, 4095, 4095};
    struct DotrDrawColor own = {{0xa1, 0xb2, 0xc3, 0xd4}};
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;
    long packet[15];
    int mode, i;

    for (mode = 0; mode < 5; mode++) {
        const struct DotrDrawColor *want = mode == 4 ? &own : &data_00293f00;

        memset(s, 0, sizeof(*s));
        for (i = 0; i < 15; i++) {
            packet[i] = 0x5a5a5a5a5a5a5a5a;
        }
        s->unknown_20.unknown_bit_0 = (mode & 1) || mode == 4 ? -1 : 0;
        s->unknown_20.unknown_bit_1 = (mode & 2) || mode == 4 ? -1 : 0;
        clear_calls = 0;
        if (mode < 3) {
            assert(RhBase_ClearRenderFrame(packet, &own) == packet && clear_calls == 0);
            for (i = 0; i < 15; i++) {
                assert(packet[i] == 0x5a5a5a5a5a5a5a5a);
            }
            continue;
        }
        assert(RhBase_ClearRenderFrame(packet, mode == 4 ? &own : 0) == packet + 14 && clear_calls == 1);
        assert(((int *)packet)[0] == 0x10000009 && ((int *)packet)[1] == 0);
        assert(((int *)packet)[2] == 0x11000000 && ((int *)packet)[3] == 0x51000009);
        assert(packet[2] == 0x1000000000008008 && packet[3] == 14 && packet[4] == 0 && packet[5] == 74);
        assert(clear_packet == packet + 6);
        for (i = 0; i < 5; i++) {
            assert(clear_args[i] == fixed[i]);
        }
        assert(clear_args[5] == want->c[2] && clear_args[6] == want->c[1] && clear_args[7] == want->c[0]);
        assert(clear_args[8] == want->c[3] && clear_args[9] == 0);
        for (i = 6; i < 12; i++) {
            assert(packet[i] == 0x5a5a5a5a5a5a5a5a);
        }
        assert(packet[12] == 1 && packet[13] == 74 && packet[14] == 0x5a5a5a5a5a5a5a5a);
        assert(s->unknown_20.unknown_bit_0 && s->unknown_20.unknown_bit_1);
    }
}

unsigned char gSzMsg_abRGB[3][16];
unsigned char gSzMsg_abAlpha[3][16];

/* Straight-line coverage (one fixed-count loop): every stored doubleword, the copied table and
 * the return value. The stubbed RhMem_CopyMemory4 returns its destination, so the count is 6. */
static void check_load_clut(void)
{
    static long packet[24];
    unsigned char want[64];
    int i;

    for (i = 0; i < 16; i++) {
        gSzMsg_abRGB[2][i] = 0x40 + i;
        gSzMsg_abAlpha[2][i] = 0x80 + i;
        want[i * 4] = want[i * 4 + 1] = want[i * 4 + 2] = 0x40 + i;
        want[i * 4 + 3] = 0x80 + i;
    }
    memset(packet, 0x5a, sizeof packet);
    assert(SzMsg_LoadClut_Pipe((DotrDrawWord)packet, 2, 0x1234) == (DotrDrawWord)(packet + 14));
    assert(*(unsigned short *)packet == 6 && ((unsigned char *)packet)[2] == 0 && ((unsigned char *)packet)[3] == 16);
    assert(((int *)packet)[1] == 0 && ((int *)packet)[2] == 0x11000000 && ((int *)packet)[3] == 0x51000006);
    assert(packet[2] == 0x1000000000000004L && packet[3] == 14);
    assert(packet[4] == 0x0001123400000000L && packet[5] == 0x50 && packet[6] == 0 && packet[7] == 0x51);
    assert(packet[8] == 0x200000008L && packet[9] == 0x52 && packet[10] == 0 && packet[11] == 0x53);
    assert(packet[12] == 0x0800000000008004L && packet[13] == 0);
    assert(memcmp(packet + 14, want, sizeof want) == 0);
    assert(packet[22] == 0x5a5a5a5a5a5a5a5aL);
}

/* 0x209510: stubs note their call under ids 20 and up; the display record is filled by
 * func_00186410 and must carry 256 at +6 when func_00186238 sees it. */
volatile unsigned dotr_draw_t1_count_10000800;
volatile unsigned dotr_draw_t1_mode_10000810;
char data_00286980[16];
static int init_file_result, init_env_seen;
static unsigned char init_flags;
static struct DotrDrawState *init_screen_state;
void *func_00198948(void *destination, int value, int byte_count)
{
    reset_note(20, value, byte_count, destination == (void *)&gRhBase_dl, 0);
    return memset(destination, value, (size_t)byte_count);
}
int RhFile_Init(void) { reset_note(21, 0, 0, 0, 0); return init_file_result; }
void func_00186fc8(void) { reset_note(22, 0, 0, 0, 0); }
void func_00186bd0(void) { reset_note(23, 0, 0, 0, 0); }
void func_00186410(struct DotrDrawEnv *e) { memset(e, 0x11, sizeof(*e)); reset_note(24, 0, 0, 0, 0); }
void func_00186238(struct DotrDrawEnv *e)
{
    init_env_seen = e->unknown_06;
    reset_note(25, e->unknown_00[0], e->unknown_00[2], 0, 0);
}
DotrDrawWord func_00186120(int a) { reset_note(26, a, 0, 0, 0); return (DotrDrawWord)&init_flags; }
void lRhBase_InitScreenVariables(struct DotrDrawState *b, int mode, int a, int c)
{
    init_screen_state = b;
    reset_note(27, mode, a, c, init_flags);
}
int func_0017b7d0(void) { reset_note(28, 0, 0, 0, 0); return 77; }
void func_0017b770(int a, int b) { reset_note(29, a, b, 0, 0); }
int RhGeom_Init(void) { reset_note(30, 0, 0, 0, 0); return 1; }
int RhView_Init(void) { reset_note(31, 0, 0, 0, 0); return 1; }
int RhMemCard_Init(void) { reset_note(32, 0, 0, 0, 0); return 1; }
void RhZSort_Start(int a, int b) { reset_note(33, a, b, 0, 0); flip_note('Z'); }
int func_001854e0(int a)
{
    reset_note(34, a, 0, 0, 0);
    if (flip_active) {
        flip_note('V');
        dotr_draw_t1_count_10000800 = flip_vsync_t1[flip_vsync_calls & 3];
        return flip_vsync_result[flip_vsync_calls++ & 3];
    }
    return 0;
}

volatile unsigned dotr_draw_d1_chcr_10009000, dotr_draw_d1_qwc_10009020;
volatile unsigned dotr_draw_d1_tadr_10009030, dotr_draw_d_stat_1000e010;
int frameSkipCounter;
static void *flip_half_env, *flip_swap_state;
static int flip_half[3], flip_swap_frame;
static unsigned flip_tadr_at_swap;
static DotrDrawWord flip_dither_in;
void RhFile_VSyncProcess(void) { flip_note('f'); }
int RhMemCard_Act(void) { flip_note('m'); return 0; }
int SzGame_InqSoftFocus(void) { flip_note('q'); return flip_focus; }
void func_00186060(void *env, short x, short y, short half)
{
    flip_note('H');
    flip_half_env = env;
    flip_half[0] = x; flip_half[1] = y; flip_half[2] = half;
}
void func_00185480(struct DotrDrawState *state, int frame)
{
    flip_note('S');
    flip_swap_state = state;
    flip_swap_frame = frame;
    flip_tadr_at_swap = dotr_draw_d1_tadr_10009030;
    dotr_draw_t1_count_10000800 = flip_final_t1;
}
DotrDrawWord lRhBase_SetRandomDither(DotrDrawWord cursor)
{
    flip_note('D');
    flip_dither_in = cursor;
    return cursor + 64;
}
static void flip_before(void) { flip_note('a'); }
static void flip_after(void) { flip_note('b'); }

static void flip_setup(int kind, int height, int wait)
{
    memset(&gRhBase_dl, 0, sizeof(gRhBase_dl));
    memset(data_00293f14, 0, sizeof(data_00293f14));
    memset(flip_packet, 0x55, sizeof(flip_packet));
    memset(flip_vsync_result, 0, sizeof(flip_vsync_result));
    memset(flip_vsync_t1, 0, sizeof(flip_vsync_t1));
    data_00326180[2] = 1;
    *(int *)(data_00293f14 + 40) = kind;
    gRhBase_dl.unknown_240 = height;
    gRhBase_dl.unknown_26c = 4;
    gRhBase_dl.unknown_27c = 0x12345678;
    gRhBase_dl.unknown_284 = 0x100;
    gRhBase_dl.unknown_28c = 0x2000;
    gRhBase_dl.unknown_290 = 0x3000;
    frameSkipCounter = wait;
    data_003261d0[0] = 0x1000;
    dotr_draw_d1_chcr_10009000 = dotr_draw_d1_qwc_10009020 = 7;
    dotr_draw_d1_tadr_10009030 = dotr_draw_d_stat_1000e010 = 7;
    dotr_draw_t1_count_10000800 = 9;
    flip_active = 1;
    flip_n = flip_vsync_calls = flip_sync_result = flip_focus = 0;
    flip_final_t1 = 0;
    flip_log[0] = 0;
    reset_count = 0;
}

static void check_flip_buffer(void)
{
    static const struct { int kind, height; unsigned t1; int result; } ends[6] = {
        {2, 448, 540, 1}, {2, 448, 541, 2}, {2, 512, 270, 1}, {2, 512, 271, 2}, {0, 448, 640, 1}, {0, 448, 641, 2},
    };
    long *page = mmap((void *)0x12000000, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    int i;

    assert(page == (long *)0x12000000);

    /* Kind 0 (height 512, 620 counts a frame), one sync owed, no callbacks, path wait not
     * negative, half offset from the first environment (even frame count), no soft focus. */
    flip_setup(0, 448, 0);
    *(int *)&gRhBase_dl.unknown_24c[0] = 1;
    gRhBase_dl.unknown_260 = 0x10014;
    gRhBase_dl.unknown_264 = -3;
    flip_vsync_result[0] = 5;
    flip_vsync_t1[0] = 620;
    flip_final_t1 = 640;
    assert(RhBase_FlipBuffer() == 1);
    assert(strcmp(flip_log, "zfmPVHFSqPFZD") == 0);
    assert(flip_packet[0] == 0x70000000 && flip_packet[1] == 0 && flip_packet[2] == 0 && flip_packet[3] == 0);
    assert(flip_packet[4] == 0x55555555);
    assert(flip_half_env == (void *)&gRhBase_dl.unknown_000[0x60]);
    assert(flip_half[0] == 0x14 && flip_half[1] == -3 && flip_half[2] == 5);
    assert(flip_swap_state == (void *)&gRhBase_dl && flip_swap_frame == 4 && flip_tadr_at_swap == 7);
    assert(dotr_draw_d1_qwc_10009020 == 0 && dotr_draw_d1_tadr_10009030 == 0x02345678);
    assert(dotr_draw_d_stat_1000e010 == 2 && dotr_draw_d1_chcr_10009000 == 0x145);
    assert(dotr_draw_t1_count_10000800 == 0 && frameSkipCounter == 0);
    assert(gRhBase_dl.unknown_27c == 0x2000 && gRhBase_dl.unknown_280 == 0x2000);
    assert(gRhBase_dl.unknown_288 == 1 && gRhBase_dl.unknown_26c == 5);
    assert(reset_order[reset_count - 1] == 33 && reset_args[reset_count - 1][0] == 0x2000);
    assert(reset_args[reset_count - 1][1] == 0x2100);
    assert(flip_dither_in == (DotrDrawWord)flip_packet && data_003261d0[0] == (DotrDrawWord)flip_packet + 64);

    /* Kind 2 (height 448, 520 counts a frame), negative path wait (GS environment reset),
     * both callbacks, three syncs owed: non-zero results at the interlaced height ask again,
     * a count of exactly 3 * 520 goes on and one more stops the wait; odd frame count, soft
     * focus, the other packet area, and an overrun at the end. */
    flip_setup(2, 448, 2);
    *(int *)&gRhBase_dl.unknown_24c[0] = 1;
    gRhBase_dl.unknown_26c = 3;
    gRhBase_dl.unknown_288 = 1;
    gRhBase_dl.unknown_298 = flip_before;
    gRhBase_dl.unknown_294 = flip_after;
    flip_sync_result = -1;
    flip_focus = 1;
    flip_vsync_result[0] = 1; flip_vsync_result[1] = 0; flip_vsync_t1[1] = 1560;
    flip_vsync_result[2] = 0x12345; flip_vsync_result[3] = 0x1ffff; flip_vsync_t1[3] = 1561;
    flip_final_t1 = 541;
    page[0] = 0;
    assert(RhBase_FlipBuffer() == 2);
    assert(strcmp(flip_log, "zfmPPFPPaVVVVbHFSqPFZD") == 0);
    assert(flip_half_env == (void *)&gRhBase_dl.unknown_108[0x48] && flip_half[2] == -1);
    assert(page[0] == ((long)0x7f << 8 | 0x23));
    assert(gRhBase_dl.unknown_27c == 0x3000 && gRhBase_dl.unknown_280 == 0x3000);
    assert(gRhBase_dl.unknown_288 == 0 && gRhBase_dl.unknown_26c == 4 && frameSkipCounter == 0);
    assert(reset_args[reset_count - 1][0] == 0x3000 && reset_args[reset_count - 1][1] == 0x3100);

    /* Nothing owed (-1): no sync wait, no half offset. Then the result boundaries of each mode,
     * with one sync whose result 0 does not ask again. */
    flip_setup(0, 512, -1);
    assert(RhBase_FlipBuffer() == 1 && strcmp(flip_log, "zfmPFSqPFZD") == 0);
    for (i = 0; i < 6; i++) {
        flip_setup(ends[i].kind, ends[i].height, 0);
        flip_final_t1 = ends[i].t1;
        assert(RhBase_FlipBuffer() == ends[i].result && strcmp(flip_log, "zfmPVFSqPFZD") == 0);
    }
    flip_active = 0;
    reset_count = 0;
    data_003261d0[0] = 0x1000;
    munmap(page, 4096);
}

static void check_base_init(void)
{
    static const int order[25] = {20, 21, 22, 23, 4, 2, 24, 25, 6, 26, 27, 29 - 1, 29, 7, 30, 31, 32, 8, 3, 9, 3, 9, 3, 33, 34};
    int i;

    assert(sizeof(struct DotrDrawTimerMode) == 4 && sizeof(struct DotrDrawEnv) == 28);
    assert(sizeof(struct DotrDrawState) >= 672);
    /* The file layer fails: state cleared, the seventh argument stored, 0 returned. */
    memset(&gRhBase_dl, 0x3c, sizeof(gRhBase_dl));
    reset_count = 0;
    init_file_result = 0;
    dotr_draw_t1_mode_10000810 = 0x55;
    assert(RhBase_Init(1, 2, 3, 0x1000, 5, 0x600, 7) == 0);
    assert(reset_count == 2 && reset_order[0] == 20 && reset_order[1] == 21);
    assert(reset_args[0][0] == 0 && reset_args[0][1] == 672 && reset_args[0][2] == 1);
    assert(gRhBase_dl.unknown_23c == 0 && gRhBase_dl.unknown_278 == 7 && gRhBase_dl.unknown_28c == 0);
    assert(dotr_draw_t1_mode_10000810 == 0x55);

    /* Success: every call in order with its arguments, row 1 of the table. */
    memset(&gRhBase_dl, 0x3c, sizeof(gRhBase_dl));
    memset(reset_buffer, 0x3c, sizeof(reset_buffer));
    *(short *)(gRhSys_asd + 40) = 640;
    *(short *)(data_00293f14 + 40) = -448;
    *(short *)(data_00293f18 + 40) = 2;
    reset_count = 0;
    init_file_result = 1;
    init_flags = 0xbf;
    init_env_seen = 0;
    dotr_draw_t1_count_10000800 = 9;
    assert(RhBase_Init(1, 2, 3, 0x1000, 5, 0x600, 7) == 1);
    assert(reset_count == 25);
    for (i = 0; i < 25; i++) {
        assert(reset_order[i] == order[i]);
    }
    assert(reset_args[5][0] == 1 && init_env_seen == 256);
    assert(reset_args[7][0] == 0x1111 && reset_args[7][1] == 0x1111);
    assert(reset_args[8][0] == 0 && reset_args[8][1] == 640 && reset_args[8][2] == -448 && reset_args[8][3] == 2);
    assert(reset_args[9][0] == 1 && gRhBase_dl.unknown_274 == (DotrDrawWord)&init_flags);
    /* Bit 6 of the returned object is set before the screen variables are initialised. */
    assert(init_flags == 0xff && reset_args[10][3] == 0xff && init_screen_state == &gRhBase_dl);
    assert(reset_args[10][0] == 1 && reset_args[10][1] == 2 && reset_args[10][2] == 3);
    assert(dotr_draw_t1_mode_10000810 == 0x883 && dotr_draw_t1_count_10000800 == 0);
    assert(gRhBase_dl.unknown_278 == 7 && gRhBase_dl.unknown_27c == 0x1000 && gRhBase_dl.unknown_280 == 0x1000);
    assert(gRhBase_dl.unknown_284 == 0x600 && gRhBase_dl.unknown_288 == 1 && gRhBase_dl.unknown_28c == 0x1000);
    assert(gRhBase_dl.unknown_290 == 5 && gRhBase_dl.unknown_294 == 0 && gRhBase_dl.unknown_298 == 0);
    assert(reset_args[12][0] == 77 && reset_args[12][1] == 32 && reset_args[13][0] == 0x1000);
    assert(reset_buffer[0] == 0x70000000 && reset_buffer[1] == 0 && reset_buffer[2] == 0 && reset_buffer[3] == 0);
    assert(reset_args[17][0] == 0 && reset_args[18][0] == 0 && reset_args[18][1] == 0);
    assert(reset_args[19][0] == (int)(DotrDrawWord)&init_flags && reset_args[19][1] == 0x1000);
    assert(reset_args[21][0] == (int)(DotrDrawWord)&init_flags && reset_args[21][1] == (int)(DotrDrawWord)data_00286980);
    assert(reset_args[23][0] == 0x1000 && reset_args[23][1] == 0x1600 && reset_args[24][0] == 0);
}

/* 0x241c20: packet callees note their call under ids 35 and up. */
struct DotrDrawClearEnv data_00350410;
DotrDrawQuad data_0029b2c0;
static struct DotrDrawGifPacket *gsmem_packet;
static void *gsmem_base;
static long gsmem_values[4], gsmem_tag[2], gsmem_env_tag[2], gsmem_word4;
static int gsmem_regs[4], gsmem_ad;
/* 0x241e80 makes more packet calls than the note list holds: while image_mode is set
 * the packet stubs record here instead. */
static int image_mode, image_count, image_ids[320];
static long image_a[320], image_b[320];
static unsigned long image_reserved[2];
static int image_note(int id, long a, long b)
{
    if (!image_mode) {
        return 0;
    }
    assert(image_count < 320);
    image_ids[image_count] = id;
    image_a[image_count] = a;
    image_b[image_count++] = b;
    return 1;
}
int func_00184e10(void *env, short a, short w, short h, int d, int e)
{
    memcpy(gsmem_env_tag, &data_00350410.giftag, 16);
    reset_note(35, env == (void *)&data_00350410.draw[0], a, w, h);
    assert(d == 0 && e == 0);
    data_00350410.draw[4].v = 0x77;
    return 0;
}
void func_00185100(struct DotrDrawClearEnv *e)
{
    gsmem_word4 = (long)data_00350410.draw[4].v;
    reset_note(36, e == &data_00350410, 0, 0, 0);
}
void func_001872f8(struct DotrDrawGifPacket *p, DotrDrawWord128 *b)
{
    gsmem_packet = p;
    if (image_note(37, (long)(DotrDrawWord)b, 0)) return;
    gsmem_base = b;
    p->base = 0x4321;
    reset_note(37, 0, 0, 0, 0);
}
void func_00187308(struct DotrDrawGifPacket *p) { if (image_note(38, p == gsmem_packet, 0)) return; reset_note(38, p == gsmem_packet, 0, 0, 0); }
void func_00187480(struct DotrDrawGifPacket *p, int a, int b, int c) { if (image_note(39, p == gsmem_packet, a | b | c)) return; reset_note(39, p == gsmem_packet, a, b, c); }
void func_00187510(struct DotrDrawGifPacket *p, DotrDrawWord128 tag)
{
    memcpy(gsmem_tag, &tag, 16);
    if (image_note(40, gsmem_tag[0], gsmem_tag[1])) return;
    reset_note(40, p == gsmem_packet, 0, 0, 0);
}
void func_001875d0(struct DotrDrawGifPacket *p, int reg, long value)
{
    if (image_note(41, reg, value)) return;
    gsmem_regs[gsmem_ad & 3] = reg;
    gsmem_values[gsmem_ad++ & 3] = value;
    reset_note(41, p == gsmem_packet, 0, 0, 0);
}
void func_00187528(struct DotrDrawGifPacket *p) { if (image_note(42, p == gsmem_packet, 0)) return; reset_note(42, p == gsmem_packet, 0, 0, 0); }
void func_00187318(struct DotrDrawGifPacket *p) { if (image_note(43, p == gsmem_packet, 0)) return; reset_note(43, p == gsmem_packet, 0, 0, 0); }

static void check_clear_gs_mem(void)
{
    static const int order[19] = {26, 35, 8, 3, 36, 37, 38, 39, 40, 41, 41, 41, 41, 42, 43, 8, 3, 9, 3};
    int i;

    assert(sizeof(struct DotrDrawGifTag) == 16 && sizeof(struct DotrDrawClearEnv) == 128);
    memset(&data_00350410, 0x3c, sizeof(data_00350410));
    data_0029b2c0.l[0] = 0x1111222233334444L;
    data_0029b2c0.l[1] = 0x5555666677778888L;
    reset_count = 0;
    gsmem_ad = 0;
    clearGsMem(0x12, 0x34, 0x56, 0x10280, 448);
    assert(reset_count == 19 && gsmem_ad == 4);
    for (i = 0; i < 19; i++) {
        assert(reset_order[i] == order[i]);
    }
    assert(reset_args[0][0] == 2);
    /* Tag: eight loops, end of packet, one register of kind 14; set before the environment call. */
    assert(gsmem_env_tag[0] == 0x1000000000008008L && gsmem_env_tag[1] == 14);
    /* Width narrowed to 16 bits; the height is two rounded-up block counts times 64. */
    assert(reset_args[1][0] == 1 && reset_args[1][1] == 0 && reset_args[1][2] == 0x280 && reset_args[1][3] == 1344);
    assert(gsmem_word4 == 0 && reset_args[4][0] == 1);
    assert(reset_args[2][0] == 0 && reset_args[3][0] == 0 && reset_args[3][1] == 0);
    assert(gsmem_base != NULL && reset_args[6][0] == 1);
    assert(reset_args[7][0] == 1 && reset_args[7][1] == 0 && reset_args[7][2] == 0 && reset_args[7][3] == 0);
    assert(reset_args[8][0] == 1 && gsmem_tag[0] == 0x1111222233334444L && gsmem_tag[1] == 0x5555666677778888L);
    assert(gsmem_regs[0] == 0 && gsmem_values[0] == 6 && gsmem_regs[1] == 1 && gsmem_values[1] == 0x563412);
    assert(gsmem_regs[2] == 5 && gsmem_values[2] == 0 && gsmem_regs[3] == 5 && gsmem_values[3] == 0xb4002d00L);
    for (i = 9; i < 15; i++) {
        assert(reset_args[i][0] == 1);
    }
    assert(reset_args[15][0] == 0 && reset_args[16][0] == 0 && reset_args[16][1] == 0);
    assert(reset_args[17][0] == (int)(DotrDrawWord)&init_flags && reset_args[17][1] == 0x4321);
    assert(reset_args[18][0] == 0 && reset_args[18][1] == 0);
}

DotrDrawQuad data_0029b2d0;
DotrDrawQuad data_0029b2e0;
void func_00187370(struct DotrDrawGifPacket *p, int a, int b, int c) { image_note(44, p == gsmem_packet, a | b | c); }
unsigned long *func_001874f8(struct DotrDrawGifPacket *p, int count)
{
    image_note(45, p == gsmem_packet, count);
    image_reserved[0] = image_reserved[1] = 0x99;
    return image_reserved;
}
void func_001873e8(struct DotrDrawGifPacket *p, unsigned int address, int size, int a, int b, int c)
{
    assert(p == gsmem_packet && a == 0 && b == 0 && c == 0);
    assert(image_reserved[0] == (64 | (0x08000000UL << 32)) && image_reserved[1] == 0);
    image_note(46, address, size);
}

static int image_at;
static void image_expect(int id, long a, long b)
{
    assert(image_at < image_count && image_ids[image_at] == id);
    assert(image_a[image_at] == a && image_b[image_at] == b);
    image_at++;
}

/* 0x241e80 loop coverage: one call per shape (mode 0 with columns x rows blocks, mode 0 with
 * an empty outer or inner loop, mode 1), comparing the whole call sequence. */
static void image_case(unsigned int tags, unsigned int image, int mode, int width, int height)
{
    int i;
    int j;

    image_mode = 1;
    image_count = image_at = 0;
    setImageTag(tags, image, mode, width, height);
    image_mode = 0;
    image_expect(37, (long)(DotrDrawWord)((tags & 0x0fffffff) | 0x20000000), 0);
    image_expect(38, 1, 0);
    if (mode == 0) {
        image_expect(44, 1, 0);
        image_expect(40, 0x1111, 0x2222);
        image_expect(41, 80, 0x000c1b0000000000L);
        image_expect(41, 82, 0x0000001000000010L);
        image_expect(42, 1, 0);
        for (i = 0; i < width >> 4; i++) {
            for (j = 0; j < height >> 4; j++) {
                image_expect(44, 1, 0);
                image_expect(40, 0x1111, 0x2222);
                image_expect(41, 81, ((long)(i * 16) << 32) | ((long)(j * 16) << 48));
                image_expect(41, 83, 0);
                image_expect(42, 1, 0);
                image_expect(45, 1, 4);
                image_expect(46, image & 0x0fffffff, 64);
                image += 1024;
            }
        }
    }
    image_expect(39, 1, 0);
    image_expect(40, 0x3333, 0x4444);
    image_expect(41, 63, 0);
    image_expect(41, 20, 96);
    image_expect(41, 6, 0x0000000aa8031b00L);
    image_expect(41, 0, 278);
    image_expect(41, 3, 0x80008);
    image_expect(41, 5, 0x79006c00);
    image_expect(41, 3, (long)(width * 16 + 8) | ((long)(height * 16 + 8) << 16));
    image_expect(41, 5, 0x87009400L);
    image_expect(42, 1, 0);
    image_expect(43, 1, 0);
    assert(image_at == image_count);
}

float gRhVu1Mac_matDir[16], gRhVu1Mac_matCol[16];

/* 0x208f00. The copy and multiply callees are stubs, so the words after the four tag words stay
 * untouched; the checks are the stored matrix cells, the callee arguments, the tag words and
 * the returned address. */
static void check_straight_light(void)
{
    struct DotrBlockTriple light;
    struct DotrBlockVector4 dir;
    struct DotrDrawTag tags[8];
    struct DotrDrawTag *end = (struct DotrDrawTag *)((char *)(tags + 1) + 64);
    float colour_before[16], dir_before[16];
    int slot, i;

    for (i = 0; i < 16; i++) {
        gRhVu1Mac_matDir[i] = (float)(100 + i);
        gRhVu1Mac_matCol[i] = (float)(200 + i);
    }
    memcpy(dir_before, gRhVu1Mac_matDir, sizeof(dir_before));
    memcpy(colour_before, gRhVu1Mac_matCol, sizeof(colour_before));
    light.unknown_00[0].x = 9.0f; light.unknown_00[0].y = 1.0f;
    light.unknown_00[0].z = 2.0f; light.unknown_00[0].unknown_w = 3.0f;
    dir.x = 4.0f; dir.y = 5.0f; dir.z = 6.0f; dir.unknown_w = 7.0f;

    /* Slots 0-2 with a null light or a null direction: tag returned, nothing stored or called. */
    copy_calls = 0;
    reset_count = 0;
    node_mul[0] = 0;
    assert(RhVu1Mac_SetStraightLightToVu1_Ex(tags, 0, &dir, 0) == tags);
    assert(RhVu1Mac_SetStraightLightToVu1_Ex(tags, &light, 0, 2) == tags);
    assert(RhVu1Mac_SetStraightLightToVu1_Ex(0, 0, 0, 1) == 0);
    assert(copy_calls == 0 && reset_count == 0 && node_mul[0] == 0);
    assert(memcmp(dir_before, gRhVu1Mac_matDir, sizeof(dir_before)) == 0);
    assert(memcmp(colour_before, gRhVu1Mac_matCol, sizeof(colour_before)) == 0);

    /* Slots 0, 1, 2 with a null tag: colour row and direction column of the slot, no calls. */
    for (slot = 0; slot < 3; slot++) {
        assert(RhVu1Mac_SetStraightLightToVu1_Ex(0, &light, &dir, slot) == 0);
        colour_before[slot * 4] = 1.0f; colour_before[slot * 4 + 1] = 2.0f; colour_before[slot * 4 + 2] = 3.0f;
        dir_before[slot] = 4.0f; dir_before[4 + slot] = 5.0f; dir_before[8 + slot] = 6.0f;
        assert(memcmp(dir_before, gRhVu1Mac_matDir, sizeof(dir_before)) == 0);
        assert(memcmp(colour_before, gRhVu1Mac_matCol, sizeof(colour_before)) == 0);
    }
    assert(copy_calls == 0 && reset_count == 0 && node_mul[0] == 0);

    /* Slot 3 (first one past the range) with null vectors and a tag: nothing stored, the
     * product is built in an aligned local and sent after four tag words. */
    memset(tags, 0x55, sizeof(tags));
    assert(RhVu1Mac_SetStraightLightToVu1_Ex(tags, 0, 0, 3) == end);
    assert(memcmp(dir_before, gRhVu1Mac_matDir, sizeof(dir_before)) == 0);
    assert(memcmp(colour_before, gRhVu1Mac_matCol, sizeof(colour_before)) == 0);
    assert(copy_calls == 2 && reset_count == 0);
    assert(copy16_log[0][1] == (void *)gRhVu1Mac_matCol && copy16_count[0] == 4);
    assert(((size_t)copy16_log[0][0] & 15) == 0);
    assert(node_mul[0] == copy16_log[0][0] && node_mul[1] == (void *)gRhVu1Mac_matDir);
    assert(copy16_log[1][0] == (void *)(tags + 1) && copy16_log[1][1] == copy16_log[0][0] && copy16_count[1] == 4);
    assert(tags[0].unknown_00 == 0x10000004 && tags[0].unknown_04 == 0 && tags[0].unknown_08 == 0);
    assert(tags[0].unknown_0c == 0x6c04003f && tags[1].unknown_00 == 0x55555555);

    /* Negative slot with a tag: same submission, no clearing. */
    copy_calls = 0;
    assert(RhVu1Mac_SetStraightLightToVu1_Ex(tags, &light, &dir, -1) == end);
    assert(copy_calls == 2 && reset_count == 0 && gRhVu1Mac_matCol[0] == 1.0f);

    /* Slot 3 without a tag: nothing at all. Negative slot without a tag: both matrices cleared. */
    copy_calls = 0;
    assert(RhVu1Mac_SetStraightLightToVu1_Ex(0, &light, &dir, 3) == 0);
    assert(copy_calls == 0 && reset_count == 0 && gRhVu1Mac_matCol[0] == 1.0f);
    assert(RhVu1Mac_SetStraightLightToVu1_Ex(0, &light, &dir, -1) == 0);
    assert(copy_calls == 0 && reset_count == 2 && reset_order[0] == 20 && reset_order[1] == 20);
    assert(reset_args[0][0] == 0 && reset_args[0][1] == 64 && reset_args[1][1] == 64);
    for (i = 0; i < 16; i++) {
        assert(gRhVu1Mac_matDir[i] == 0.0f && gRhVu1Mac_matCol[i] == 0.0f);
    }
    reset_count = 0;
}

static void check_set_image_tag(void)
{
    data_0029b2d0.l[0] = 0x1111; data_0029b2d0.l[1] = 0x2222;
    data_0029b2e0.l[0] = 0x3333; data_0029b2e0.l[1] = 0x4444;
    image_case(0x70123400, 0x9fff0000, 0, 32, 48);
    assert(image_count == 61);
    image_case(0x00123400, 0x00100000, 0, 15, 48);
    assert(image_count == 19);
    image_case(0x00123400, 0x00100000, 0, 16, 15);
    assert(image_count == 19);
    image_case(0x00123400, 0x00100000, 1, 32, 48);
    assert(image_count == 14);
    image_case(0x00123400, 0x00100000, 0, 640, 16);
    assert(image_count == 19 + 40 * 7);
}

int main(void)
{
    check_load_clut();
    static const DotrDrawWord values[] = {0, -1, 0x12345678};
    size_t i;
    check_frame_clear();
    check_frame_close();
    check_frame_setup();
    check_frame_open();
    check_frame_fields();
    check_frame_bits();
    check_cursor();
    check_frame_packet();
    check_fixed_words();
    check_fade_step();
    /* Ten further wrappers: read the cursor, call one routine, store its
     * result; two also return a word written through a pointer. */
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzDuel_SetupCard(11, 22);
    assert(stage == 60 && wide_last[0] == 11 && wide_last[1] == 22 && data_003261d0[0] == 0x2000 + 60);
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzFade_Draw(11, 22, 33);
    assert(stage == 61 && wide_last[0] == 11 && wide_last[1] == 22 && wide_last[2] == 33 && data_003261d0[0] == 0x2000 + 61);
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzMsg_LoadFont(11, 22, 33, 44);
    assert(stage == 62 && wide_last[0] == 11 && wide_last[1] == 22 && wide_last[2] == 33 && wide_last[3] == 44 && data_003261d0[0] == 0x2000 + 62);
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzMsg_DrawX(11, 22, 33, 44, 55);
    assert(stage == 63 && wide_last[0] == 11 && wide_last[1] == 22 && wide_last[2] == 33 && wide_last[3] == 44 && wide_last[4] == 55 && data_003261d0[0] == 0x2000 + 63);
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzMsg_DrawGlass(11, 22, 33, 44);
    assert(stage == 64 && wide_last[0] == 11 && wide_last[1] == 22 && wide_last[2] == 33 && wide_last[3] == 44 && data_003261d0[0] == 0x2000 + 64);
    data_003261d0[0] = 0x1000;
    stage = 0;
    SzText_Load(11, 22, 33);
    assert(stage == 65 && wide_last[0] == 11 && wide_last[1] == 22 && wide_last[2] == 33 && data_003261d0[0] == 0x2000 + 65);
    data_003261d0[0] = 0x1000;
    stage = 0;
    ExSortPrim(11, 22, 33);
    assert(stage == 66 && wide_last[0] == 11 && wide_last[1] == 22 && wide_last[2] == 33 && data_003261d0[0] == 0x2000 + 66);
    data_003261d0[0] = 0x1000;
    stage = 0;
    ExSortDispBlur(11, 22, 33, 44, 55);
    assert(stage == 67 && wide_last[0] == 11 && wide_last[1] == 22 && wide_last[2] == 33 && wide_last[3] == 44 && wide_last[4] == 55 && data_003261d0[0] == 0x2000 + 67);
    data_003261d0[0] = 0x1000;
    stage = 0;
    assert(SzMsg_Init(11, 22) == 900 && stage == 70 && wide_last[0] == 11 && wide_last[1] == 22);
    assert(data_003261d0[0] == 0x2000 + 70);
    data_003261d0[0] = 0x1000;
    stage = 0;
    assert(SzText_DoRequest(11, 22) == 901 && stage == 71 && wide_last[0] == 11 && wide_last[1] == 22);
    assert(data_003261d0[0] == 0x2000 + 71);
    check_packets();
    data_003261d0[0] = 0x1000;
    SzModel_ShotTextureWithFunc(1, -2, 3, -4, 5, -6);
    assert(data_003261d0[0] == 0x4000 && wide_args[0] == 1 && wide_args[1] == -2 && wide_args[2] == 3);
    assert(wide_args[3] == -4 && wide_args[4] == 5 && wide_args[5] == -6);
    check_state();
    check_tables_and_packets();
    check_flagged_entry();
    check_reset_and_slots();
    check_chain_insert();
    check_packet_context();
    check_packet_link();
    check_view_packet();
    check_trailer_packet();
    check_row_reset();
    check_upload();
    check_row_colors();
    for (i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        chain_input = values[i];
        chain_result = values[2 - i];
        data_003261d0[0] = chain_input;
        chain_calls = 0;
        ShSlot_Draw();
        assert(chain_calls == 1 && data_003261d0[0] == chain_result);
    }
    check_quad();
    check_quad_block();
    check_node_unit();
    check_node_block();
    check_base_init();
    check_clear_gs_mem();
    check_set_image_tag();
    check_straight_light();
    check_flip_buffer();
    check_sort_node();
    puts("draw_state: ok");
    return 0;
}

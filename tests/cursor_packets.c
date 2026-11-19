/* Synthetic host tests for the cursor_packets group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cursor_packets.h"
#include "dotr/ee/cursor_pad_geom.h"

unsigned char gSzMsg_ml[1024];
struct DotrSlotTableWords gSzMsg_defRect[1] = {{{1, 2, 3, 4}}};
unsigned char icolDef_266[4] = {11, 12, 13, 14};

static int calls, order[16], int_args[8];
static long header_value;
static struct DotrCursorPacketHeader seen_header;
static struct DotrCursorRectPacket seen_packet;

unsigned char iColDef_210[4] = {21, 22, 23, 24};
static int slot_mode, info_index, tex_calls;
static struct DotrCursorSlotRectPacket seen_slot;
static struct DotrCursorSlotEntry slot_entry;
static struct DotrCursorSlotEntry *slot_result;

struct DotrCursorSlotEntry *SzMsg_GetInfo(int index) { info_index = index; return slot_result; }
void lSzMsg_GetTexInfo(struct DotrCursorTex *tex0, struct DotrCursorTex *tex1)
{
    assert(tex1 == tex0 + 1);
    tex0->word = 0x1111;
    tex1->word = 0x2222;
    tex_calls++;
}

struct DotrCursorPicControl data_00317490;
unsigned char data_002be850[8] = {1, 2, 3, 4, 5, 6, 7, 8};
unsigned char data_002be858[8] = {11, 12, 13, 14, 15, 16, 17, 18};
unsigned char data_002be860[8] = {21, 22, 23, 24, 25, 26, 27, 28};
int data_003180e8;
static int base_mode, base_calls;
static struct DotrBasePipePacket seen_base[2];
static struct DotrCursorPicEntry pic_entry;
static struct DotrCursorPicEntry *pic_result;
static int pic_texture[2];
static int pic_args[8];
static struct DotrCursorPacketHeader pic_header;

struct DotrCursorPicEntry *SzText_GetReadyItem(int group, int id)
{
    pic_args[0] = group;
    pic_args[1] = id;
    return pic_result;
}
int SzText_InitClutManage(int base, int size) { pic_args[2] = base; pic_args[3] = size; return 3; }
DotrDrawWord SzText_Load_Pipe(DotrDrawWord cursor, int *texture, int a1, int a2)
{
    assert(texture == pic_texture && a2 == 0);
    order[calls++] = 5;
    pic_args[4] = a1;
    return cursor + 0x10;
}
DotrDrawWord RhText_SetTextureFlush(DotrDrawWord cursor) { order[calls++] = 6; return cursor + 0x20; }
static int pad_mode;
static struct DotrCursorPadPacket seen_pad;
static struct DotrCursorPadPacket seen_hand[2];
static int hand_mode, hand_sorts, hand_tex_calls;
static int *hand_textures[2];
int ExAssignTexture(struct DotrCursorPacketHeader *packet, int *texture, int a)
{
    if (hand_mode) {
        hand_textures[hand_tex_calls++ & 1] = texture;
    }
    assert((hand_mode || (pad_mode ? texture == (int *)(intptr_t)0x4321 : texture == pic_texture)) && a == 0);
    order[calls++] = 7;
    pic_header = *packet;
    return 0;
}

long ExGetAlphaVal(int index) { order[calls++] = 1; int_args[0] = index; return 0x1122334455L; }
int ExAssignAlpha(struct DotrCursorPacketHeader *packet, long value)
{
    order[calls++] = 2;
    seen_header = *packet;
    header_value = value;
    return 1;
}
static int zmode_calls, zmode_args[2], shot_mode, shot_args[2];
static unsigned int mask_args[2];
static int mask_calls;
static struct DotrModelShotPacket seen_shot;
static struct DotrCursorTex *shot_tex[2];
DotrDrawWord ExFrameBuffer2Texture_Pipe(DotrDrawWord cursor, int a, int b, struct DotrCursorTex *c,
                                        struct DotrCursorTex *d)
{
    order[calls++] = 8;
    shot_args[0] = a;
    shot_args[1] = b;
    shot_tex[0] = c;
    shot_tex[1] = d;
    return cursor + 0x1000;
}
DotrDrawWord RhBase_MaskFBuffer(DotrDrawWord cursor, unsigned int mask)
{
    order[calls++] = 9;
    mask_args[mask_calls++ & 1] = mask;
    return cursor + 0x4000;
}
DotrDrawWord RhBase_SetZBufferMode(DotrDrawWord cursor, int a)
{
    order[calls++] = 3;
    zmode_args[zmode_calls++ & 1] = a;
    int_args[calls == 3 ? 3 : 4] = a;
    return cursor + 0x10;
}
struct DotrBlurColour data_00293ed0 = {{31, 32, 33, 34}};
static int blur_mode, blur_buffer;
static struct DotrBlurPacket seen_blur;
static struct DotrCursorPacketHeader blur_header;

int ExAssignFrameBuffer(unsigned short *p, int b)
{
    order[calls++] = 9;
    memcpy(&blur_header, p, sizeof blur_header);
    blur_buffer = b;
    return 0;
}

struct DotrBlurColour data_00293ed8 = {{61, 62, 63, 64}};
static int soft_mode, soft_args[2];
static struct DotrSoftFocusPacket seen_soft;
static struct DotrSoftFocusTex *soft_tex0;
static struct DotrCursorTex *soft_tex1;
int RhBase_GetUserGsAddress(void) { order[calls++] = 10; return 0x3456; }
DotrDrawWord func_00208870_soft(DotrDrawWord cursor, int a, int b, struct DotrSoftFocusTex *c,
                                struct DotrCursorTex *d)
{
    order[calls++] = 8;
    soft_args[0] = a;
    soft_args[1] = b;
    soft_tex0 = c;
    soft_tex1 = d;
    /* Callee-written bits below bit 34 and the second word, as the original callee fills them. */
    c->low = 0x2aaaaaaaaUL;
    c->flag = 0;
    d->word = 0x7777;
    return cursor + 0x1000;
}

static int rectg_mode;
static struct DotrCursorRectGPacket seen_rectg;
unsigned char icolDef_226[16] = {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56};
DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, void *packet, int a1, int a2)
{
    order[calls++] = 4;
    if (hand_mode) {
        assert(a1 == 0 && a2 == 0);
        memcpy(seen_hand[hand_sorts++ & 1].header, packet, sizeof(seen_pad) - 16);
        return cursor + 0x100;
    }
    if (pad_mode) {
        assert(a1 == 0 && a2 == 0);
        memcpy(seen_pad.header, packet, sizeof(seen_pad) - 16);
        return cursor + 0x100;
    }
    memcpy(&seen_packet, packet, sizeof(seen_packet));
    if (base_mode) {
        memcpy(&seen_base[base_calls++ & 1].header, packet, sizeof(seen_base[0]) - 8);
    }
    if (blur_mode) {
        memcpy(&seen_blur, packet, 56);
    }
    if (shot_mode) {
        memcpy(&seen_shot.header, packet, 64);
        assert((char *)shot_tex[0] - (char *)packet == 64 && shot_tex[1] == shot_tex[0] + 1);
    }
    if (slot_mode) {
        memcpy(&seen_slot.header, packet, sizeof(seen_slot) - 8);
    }
    if (soft_mode) {
        memcpy(&seen_soft, packet, sizeof seen_soft);
        assert((char *)soft_tex0 - (char *)packet == 64 && (char *)soft_tex1 - (char *)packet == 72);
    }
    if (rectg_mode) {
        memcpy(&seen_rectg, packet, sizeof(seen_rectg) - 8);
    }
    int_args[5] = a1;
    int_args[6] = a2;
    return cursor + 0x200;
}

/* Straight-line: every stored value, forwarded argument and the result. */
static void check_rect(void)
{
    assert(sizeof(struct DotrCursorRectPacket) == 40 && sizeof(struct DotrCursorVertex) == 8);
    calls = 0;
    int_args[5] = int_args[6] = -1;
    assert(lSzDuel_Draw_Fade_Pipe(0x1000, 10, 20, 100, 50, 0x180 + 0x7f) == 0x1220);
    assert(calls == 5 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4 && order[4] == 3);
    assert(int_args[0] == 2 && header_value == 0x1122334455L);
    assert(seen_header.unknown_00 == 3 && seen_header.unknown_02 == 6);
    assert(seen_header.unknown_04 == 0 && seen_header.unknown_08 == 1089);
    assert(int_args[3] == 0 && int_args[4] == 1 && int_args[5] == 0 && int_args[6] == 0);
    assert(seen_packet.colour[0] == 255 && seen_packet.colour[1] == 255);
    assert(seen_packet.colour[2] == 255 && seen_packet.colour[3] == 0xff);
    assert(seen_packet.unknown_14 == 1.0f);
    assert(seen_packet.vertex[0].unknown_x == (10 + 1728) << 4);
    assert(seen_packet.vertex[0].unknown_y == (20 + 1824) << 4);
    assert(seen_packet.vertex[0].unknown_z == 0xfffffd && seen_packet.vertex[0].unknown_f == 0);
    assert(seen_packet.vertex[1].unknown_x == (110 + 1728) << 4);
    assert(seen_packet.vertex[1].unknown_y == (70 + 1824) << 4);
    assert(seen_packet.vertex[1].unknown_z == 0xfffffd && seen_packet.vertex[1].unknown_f == 0);
}

/* Branches: fade byte zero / non-zero. */
static void check_join_fade(void)
{
    calls = 0;
    data_00317490.join_fade = 0;
    assert(SzDuel_DrawJoinFade_Pipe(0x1000) == 0x1000 && calls == 0);
    data_00317490.join_fade = 0x90;
    assert(SzDuel_DrawJoinFade_Pipe(0x1000) == 0x1220 && calls == 5);
    assert(seen_packet.colour[3] == 0x90);
    assert(seen_packet.vertex[0].unknown_x == 1728 << 4 && seen_packet.vertex[0].unknown_y == 1824 << 4);
    assert(seen_packet.vertex[1].unknown_x == 2368 << 4 && seen_packet.vertex[1].unknown_y == 2272 << 4);
    data_00317490.join_fade = 0;
}

/* Straight-line: every stored value, forwarded argument and the result. */
static void check_fixed_rect(void)
{
    static unsigned char colour[4] = {1, 2, 3, 4};

    calls = 0;
    int_args[5] = int_args[6] = -1;
    assert(SzFade_Draw_Pipe(0x1000, 3, colour, 0xff123456u) == 0x1200);
    assert(calls == 3 && order[0] == 1 && order[1] == 2 && order[2] == 4);
    assert(int_args[0] == 3 && header_value == 0x1122334455L);
    assert(seen_header.unknown_00 == 3 && seen_header.unknown_02 == 6);
    assert(seen_header.unknown_04 == 0 && seen_header.unknown_08 == 1089);
    assert(int_args[5] == 0 && int_args[6] == 0);
    assert(seen_packet.colour[0] == 3 && seen_packet.colour[1] == 2);
    assert(seen_packet.colour[2] == 1 && seen_packet.colour[3] == 4);
    assert(seen_packet.unknown_14 == 1.0f);
    assert(seen_packet.vertex[0].unknown_x == 1728 << 4 && seen_packet.vertex[0].unknown_y == 1824 << 4);
    assert(seen_packet.vertex[0].unknown_z == 0x123456 && seen_packet.vertex[0].unknown_f == 0);
    assert(seen_packet.vertex[1].unknown_x == 2368 << 4 && seen_packet.vertex[1].unknown_y == 2272 << 4);
    assert(seen_packet.vertex[1].unknown_z == 0x123456 && seen_packet.vertex[1].unknown_f == 0);
}

/* Branches: rect given / null, colour given / null. */
static void check_state_rect(void)
{
    static unsigned char colour[4] = {1, 2, 3, 4};
    static struct DotrSlotTableWords rect = {{100, 200, 30, 40}};
    int z = (int)0xffabcdef;

    memcpy(gSzMsg_ml + 628, &z, sizeof(z));
    calls = 0;
    int_args[5] = int_args[6] = -1;
    assert(SzMsg_DrawGlass_Pipe(0x1000, 5, 6, &rect, colour) == 0x1200);
    assert(calls == 2 && order[0] == 2 && order[1] == 4 && header_value == 0x8000000044L);
    assert(seen_header.unknown_00 == 3 && seen_header.unknown_02 == 6);
    assert(seen_header.unknown_04 == 0 && seen_header.unknown_08 == 1089);
    assert(int_args[5] == 0 && int_args[6] == 0);
    assert(seen_packet.colour[0] == 3 && seen_packet.colour[1] == 2);
    assert(seen_packet.colour[2] == 1 && seen_packet.colour[3] == 4);
    assert(seen_packet.unknown_14 == 1.0f);
    assert(seen_packet.vertex[0].unknown_x == 105 << 4 && seen_packet.vertex[0].unknown_y == 206 << 4);
    assert(seen_packet.vertex[0].unknown_z == 0xabcdef && seen_packet.vertex[0].unknown_f == 0);
    assert(seen_packet.vertex[1].unknown_x == 135 << 4 && seen_packet.vertex[1].unknown_y == 246 << 4);
    assert(seen_packet.vertex[1].unknown_z == 0xabcdef && seen_packet.vertex[1].unknown_f == 0);

    calls = 0;
    assert(SzMsg_DrawGlass_Pipe(0x1000, 5, 6, 0, 0) == 0x1200 && calls == 2);
    assert(seen_packet.colour[0] == 13 && seen_packet.colour[1] == 12);
    assert(seen_packet.colour[2] == 11 && seen_packet.colour[3] == 14);
    assert(seen_packet.vertex[0].unknown_x == 6 << 4 && seen_packet.vertex[0].unknown_y == 8 << 4);
    assert(seen_packet.vertex[1].unknown_x == 9 << 4 && seen_packet.vertex[1].unknown_y == 12 << 4);
}

/* Branches: no entry, flag clear, flag set with given and with default rect/colour. */
static void check_slot_rect(void)
{
    static unsigned char colour[4] = {1, 2, 3, 4};
    static struct DotrSlotTableWords rect = {{100, 200, 30, 40}};
    struct DotrCursorSlotUv uv;
    int z = (int)0xffabcdef;

    assert(sizeof(struct DotrCursorSlotRectPacket) == 88 && sizeof(struct DotrCursorSlotEntry) == 20);
    assert(offsetof(struct DotrCursorSlotRectPacket, tex0) == 72);
    memcpy(gSzMsg_ml + 628, &z, sizeof(z));
    slot_mode = 1;
    slot_result = 0;
    calls = tex_calls = 0;
    assert(SzMsg_DrawRect_Pipe(0x1000, 7, 5, 6, &rect, colour) == 0x1000);
    assert(info_index == 7 && calls == 0 && tex_calls == 0);

    slot_result = &slot_entry;
    slot_entry.flag = 0;
    slot_entry.unknown_004[0] = 16;
    slot_entry.unknown_004[1] = 32;
    slot_entry.unknown_004[2] = 8;
    slot_entry.unknown_004[3] = 4;
    assert(SzMsg_DrawRect_Pipe(0x1000, 7, 5, 6, &rect, colour) == 0x1000 && calls == 0 && tex_calls == 0);

    slot_entry.flag = -1;
    int_args[5] = int_args[6] = -1;
    assert(SzMsg_DrawRect_Pipe(0x1000, 7, 5, 6, &rect, colour) == 0x1200);
    assert(calls == 2 && order[0] == 2 && order[1] == 4 && tex_calls == 1 && header_value == 0x8000000044L);
    assert(seen_header.unknown_00 == 5 && seen_header.unknown_02 == (38 | 0x800));
    assert(seen_header.unknown_04 == 0 && seen_header.unknown_08 == 0x42421);
    assert(int_args[5] == 0 && int_args[6] == 0);
    assert(seen_slot.colour[0] == 3 && seen_slot.colour[1] == 2 && seen_slot.colour[2] == 1 && seen_slot.colour[3] == 4);
    assert(seen_slot.unknown_14 == 1.0f);
    assert(seen_slot.vertex[0].xyz.x == 105 << 4 && seen_slot.vertex[0].xyz.y == 206 << 4);
    assert(seen_slot.vertex[0].xyz.z == 0xabcdef && seen_slot.vertex[0].xyz.f == 0);
    assert(seen_slot.vertex[1].xyz.x == 135 << 4 && seen_slot.vertex[1].xyz.y == 246 << 4);
    assert(seen_slot.vertex[1].xyz.z == 0xabcdef && seen_slot.vertex[1].xyz.f == 0);
    memcpy(&uv, &seen_slot.vertex[0].uv, sizeof(uv));
    assert(uv.u == (16 << 4) + 8 && uv.v == (32 << 4) + 8);
    memcpy(&uv, &seen_slot.vertex[1].uv, sizeof(uv));
    assert(uv.u == (24 << 4) + 8 && uv.v == (36 << 4) + 8);
    assert(seen_slot.tex0.word == 0x1111 && seen_slot.tex1.word == 0x2222);

    calls = 0;
    assert(SzMsg_DrawRect_Pipe(0x1000, 7, 5, 6, 0, 0) == 0x1200 && calls == 2);
    assert(seen_slot.colour[0] == 23 && seen_slot.colour[1] == 22 && seen_slot.colour[2] == 21 && seen_slot.colour[3] == 24);
    assert(seen_slot.vertex[0].xyz.x == 6 << 4 && seen_slot.vertex[0].xyz.y == 8 << 4);
    assert(seen_slot.vertex[1].xyz.x == 9 << 4 && seen_slot.vertex[1].xyz.y == 12 << 4);
    slot_mode = 0;
}

/* Branches: no entry, wrong texture word, bright set and clear. */
static void check_pic_mini(void)
{
    struct DotrCursorSlotUv uv;

    assert(offsetof(struct DotrCursorPicControl, clut) == 0xc7c);
    slot_mode = 1;
    data_00317490.clut = 0x40;
    pic_entry.texture = pic_texture;
    pic_result = 0;
    calls = 0;
    assert(lSzDuel_DrawWindow_PicMini_Pipe(0x1000, 9, 10, 20, 30, 40, 0xffabcdefu, 1) == 0x1000);
    assert(pic_args[0] == 2 && pic_args[1] == 9 && calls == 0);

    pic_result = &pic_entry;
    pic_texture[0] = 0x324853;
    assert(lSzDuel_DrawWindow_PicMini_Pipe(0x1000, 9, 10, 20, 30, 40, 0xffabcdefu, 1) == 0x1000 && calls == 0);

    pic_texture[0] = 0x324852;
    int_args[5] = int_args[6] = -1;
    assert(lSzDuel_DrawWindow_PicMini_Pipe(0x1000, 9, 10, 20, 30, 40, 0xffabcdefu, 1) == 0x1230);
    assert(calls == 4 && order[0] == 5 && order[1] == 6 && order[2] == 7 && order[3] == 4);
    assert(pic_args[2] == 0x40 && pic_args[3] == 1 && pic_args[4] == 0x43);
    assert(pic_header.unknown_00 == 5 && pic_header.unknown_02 == (38 | 0x800));
    assert(pic_header.unknown_04 == 0 && pic_header.unknown_08 == 0x42421);
    assert(int_args[5] == 0 && int_args[6] == 0);
    assert(seen_slot.colour[0] == 128 && seen_slot.colour[1] == 128 && seen_slot.colour[2] == 128);
    assert(seen_slot.colour[3] == 128 && seen_slot.unknown_14 == 1.0f);
    memcpy(&uv, &seen_slot.vertex[0].uv, sizeof(uv));
    assert(uv.u == 8 && uv.v == 8);
    memcpy(&uv, &seen_slot.vertex[1].uv, sizeof(uv));
    assert(uv.u == 648 && uv.v == 520);
    assert(seen_slot.vertex[0].xyz.x == 1738 << 4 && seen_slot.vertex[0].xyz.y == 1844 << 4);
    assert(seen_slot.vertex[0].xyz.z == 0xabcdef && seen_slot.vertex[0].xyz.f == 0);
    assert(seen_slot.vertex[1].xyz.x == 1768 << 4 && seen_slot.vertex[1].xyz.y == 1884 << 4);
    assert(seen_slot.vertex[1].xyz.z == 0xabcdef && seen_slot.vertex[1].xyz.f == 0);

    calls = 0;
    assert(lSzDuel_DrawWindow_PicMini_Pipe(0x1000, 9, 10, 20, 30, 40, 5, 0) == 0x1230 && calls == 4);
    assert(seen_slot.colour[0] == 48 && seen_slot.colour[1] == 48 && seen_slot.colour[2] == 48);
    assert(seen_slot.colour[3] == 128 && seen_slot.vertex[0].xyz.z == 5);
    slot_mode = 0;
}

/* Branch coverage: no entry, wrong texture word, drawn packet. */
static void check_pic_item(void)
{
    struct DotrCursorSlotUv uv;

    slot_mode = 1;
    data_00317490.clut = 0x40;
    pic_entry.texture = pic_texture;
    pic_result = 0;
    calls = 0;
    assert(lSzDuel_DrawWindow_PicItem_Pipe(0x1000, 9, 10, 20, 30, 40, 0xffabcdefu) == 0x1000);
    assert(pic_args[0] == 1 && pic_args[1] == 9 && calls == 0);

    pic_result = &pic_entry;
    pic_texture[0] = 0x324853;
    assert(lSzDuel_DrawWindow_PicItem_Pipe(0x1000, 9, 10, 20, 30, 40, 0xffabcdefu) == 0x1000 && calls == 0);

    pic_texture[0] = 0x324852;
    int_args[5] = int_args[6] = -1;
    assert(lSzDuel_DrawWindow_PicItem_Pipe(0x1000, 9, 10, 20, 30, 40, 0xffabcdefu) == 0x1230);
    assert(calls == 4 && order[0] == 5 && order[1] == 6 && order[2] == 7 && order[3] == 4);
    assert(pic_args[2] == 0x40 && pic_args[3] == 1 && pic_args[4] == 0x43);
    assert(pic_header.unknown_00 == 5 && pic_header.unknown_02 == (38 | 0x800));
    assert(pic_header.unknown_04 == 0 && pic_header.unknown_08 == 0x42421);
    assert(int_args[5] == 0 && int_args[6] == 0);
    assert(seen_slot.colour[0] == 128 && seen_slot.colour[1] == 128 && seen_slot.colour[2] == 128);
    assert(seen_slot.colour[3] == 128 && seen_slot.unknown_14 == 1.0f);
    memcpy(&uv, &seen_slot.vertex[0].uv, sizeof(uv));
    assert(uv.u == 8 && uv.v == 8);
    memcpy(&uv, &seen_slot.vertex[1].uv, sizeof(uv));
    assert(uv.u == 2056 && uv.v == 2056);
    assert(seen_slot.vertex[0].xyz.x == 1738 << 4 && seen_slot.vertex[0].xyz.y == 1844 << 4);
    assert(seen_slot.vertex[0].xyz.z == 0xabcdef && seen_slot.vertex[0].xyz.f == 0);
    assert(seen_slot.vertex[1].xyz.x == 1768 << 4 && seen_slot.vertex[1].xyz.y == 1884 << 4);
    assert(seen_slot.vertex[1].xyz.z == 0xabcdef && seen_slot.vertex[1].xyz.f == 0);
    slot_mode = 0;
}

static void check_base_vertex(const struct DotrBasePipeVertex *v, const unsigned char *c, int x, int y)
{
    assert(v->colour[0] == c[2] && v->colour[1] == c[1] && v->colour[2] == c[0] && v->colour[3] == c[3]);
    assert(v->q == 1.0f);
    assert(v->xyz.unknown_x == (unsigned)x << 4 && v->xyz.unknown_y == (unsigned)y << 4);
    assert(v->xyz.unknown_z == 0x345678 && v->xyz.unknown_f == 0);
}

/* Straight-line coverage: one call, every stored value of both packets, arguments, return. */
static void check_base(DotrDrawWord (*draw)(DotrDrawWord), const unsigned char *c)
{
    struct DotrCursorPacketHeader header;
    int i;

    assert(sizeof(struct DotrBasePipePacket) == 88);
    data_003180e8 = 0x12345678;
    base_mode = 1;
    base_calls = 0;
    calls = 0;
    int_args[5] = int_args[6] = -1;
    assert(draw(0x1000) == 0x1400);
    assert(base_calls == 2 && calls == 2 && int_args[5] == 0 && int_args[6] == 0);
    for (i = 0; i < 2; i++) {
        const unsigned char *left = i ? c + 4 : c;
        const unsigned char *right = i ? c : c + 4;
        int x = 1728 + 320 * i;

        memcpy(&header, seen_base[i].header, sizeof(header));
        assert(header.unknown_00 == 8 && header.unknown_02 == 20);
        assert(header.unknown_04 == 0 && header.unknown_08 == 0x41414141);
        check_base_vertex(&seen_base[i].v[0], left, x, 1824);
        check_base_vertex(&seen_base[i].v[1], right, x + 320, 1824);
        check_base_vertex(&seen_base[i].v[2], left, x, 1824 + 448);
        check_base_vertex(&seen_base[i].v[3], right, x + 320, 1824 + 448);
    }
    base_mode = 0;
}

int gSzModel_pfhCache;
static int tex_flag, tex_log[16], tex_n, tex_draw_args[9];
static void (*tex_callback)(void);
static void tex_dummy(void) {}
int SzModel_GetCalcFlag(int index) { tex_log[tex_n++ & 15] = 100 + index; return tex_flag; }
int SzModel_SetCalcFlag(int index, int value) { tex_log[tex_n++ & 15] = 200 + index; tex_flag = value; return 0; }
DotrDrawWord ExDitherMatrixEz_Pipe(DotrDrawWord cursor, int index) { tex_log[tex_n++ & 15] = 300 + index; return cursor + 1; }
long ExGetRegVal(int a, int b) { tex_log[tex_n++ & 15] = 400 + a * 10 + b; return 0x77; }
DotrDrawWord ExClampMode_Pipe(DotrDrawWord cursor, long value) { tex_log[tex_n++ & 15] = 500 + (int)value; return cursor + 0x100; }
static int tex_draw(int id, int cursor, int index, int c, void *d, int e, void *f, int g)
{
    struct DotrModelShotWork *work = (struct DotrModelShotWork *)((char *)f - 8);

    tex_log[tex_n++ & 15] = id;
    assert(tex_flag == 0 && d == 0);
    tex_draw_args[0] = index;
    tex_draw_args[1] = c;
    tex_draw_args[2] = e;
    tex_draw_args[3] = g;
    memset(&work->tex0, 0, 16);
    work->tex0.unknown_0 = 21;
    work->tex0.flag = 1;
    work->tex0.mode = 5;
    work->tex1.unknown_0 = 10;
    work->tex1.flag = id == 600;
    work->tex1.mode = 7;
    return cursor + 0x1000;
}
int SzModel_DrawWithFunc_Pipe(int cursor, int index, int c, void *d, int e, void *f, int g, void (*callback)(void),
                              int zero)
{
    tex_callback = callback;
    tex_draw_args[4] = zero;
    return tex_draw(600, cursor, index, c, d, e, f, g);
}
int SzModel_Draw_Pipe(int cursor, int index, int c, void *d, int e, void *f, int g)
{
    return tex_draw(601, cursor, index, c, d, e, f, g);
}

/* One case per outcome of the callback/argument test (callback, argument only, neither), each
 * checking the call order, forwarded arguments, the restored flag, both outputs and the result. */
static void check_shot_texture_case(void (*callback)(void), int arg, int mode, int draw_id)
{
    struct DotrModelShotTex out0, out1;
    int i;

    memset(&out0, 0xff, sizeof(out0));
    memset(&out1, 0xff, sizeof(out1));
    calls = zmode_calls = tex_n = 0;
    tex_flag = 0x55;
    tex_callback = 0;
    tex_draw_args[4] = -1;
    gSzModel_pfhCache = 9;
    assert(SzModel_ShotTextureWithFunc_Pipe(0x2000, 9, mode, &out0, &out1, callback, arg)
           == 0x2000 + 1 + 0x10 + 0x1000 + 0x10 + 1 + 0x100);
    assert(gSzModel_pfhCache == 0 && tex_flag == 0x55 && tex_n == 8 && calls == 2);
    assert(tex_log[0] == 109 && tex_log[1] == 302 && tex_log[2] == 209 && tex_log[3] == draw_id);
    assert(tex_log[4] == 209 && tex_log[5] == 303 && tex_log[6] == 404 && tex_log[7] == 500 + 0x77);
    assert(zmode_args[0] == 1 && zmode_args[1] == 0);
    for (i = 0; i < 4; i++) {
        assert(tex_draw_args[i] == (i == 0 ? 9 : 0));
    }
    if (draw_id == 600) {
        assert(tex_callback == callback && tex_draw_args[4] == arg);
    } else {
        assert(tex_callback == 0 && tex_draw_args[4] == -1);
    }
    assert(out0.unknown_0 == 21 && out0.flag == 1 && out0.mode == 5);
    assert(out1.unknown_0 == 10 && out1.mode == (unsigned)(mode & 7) && out1.flag == (unsigned)(mode & 1));
}
static void check_shot_texture(void)
{
    assert(sizeof(struct DotrModelShotWork) == 88 && sizeof(struct DotrModelShotTex) == 8);
    check_shot_texture_case(tex_dummy, 0, 0xfe, 600);
    check_shot_texture_case(0, 4, 3, 600);
    check_shot_texture_case(0, 0, 13, 601);
}

/* Straight-line: every stored value, forwarded argument, the call order and the result. */
static void check_model_shot(void)
{
    static const int want[8] = {6, 8, 6, 9, 3, 4, 3, 9};
    int i;

    assert(sizeof(struct DotrModelShotPacket) == 88 && offsetof(struct DotrModelShotPacket, tex0) == 72);
    calls = zmode_calls = mask_calls = 0;
    shot_mode = 1;
    int_args[5] = int_args[6] = -1;
    assert(lSzModel_Draw_DefFunc(0x100000) == 0x100000 + 0x20 + 0x1000 + 0x20 + 0x4000 + 0x10 + 0x200 + 0x10 + 0x4000);
    shot_mode = 0;
    assert(calls == 8);
    for (i = 0; i < 8; i++) {
        assert(order[i] == want[i]);
    }
    assert(shot_args[0] == -1 && shot_args[1] == 0);
    assert(mask_args[0] == 0xff000000u && mask_args[1] == 0);
    assert(zmode_args[0] == 0 && zmode_args[1] == 1 && int_args[5] == 0 && int_args[6] == 0);
    assert(((struct DotrCursorPacketHeader *)seen_shot.header)->unknown_00 == 5);
    assert(((struct DotrCursorPacketHeader *)seen_shot.header)->unknown_02 == 38);
    assert(((struct DotrCursorPacketHeader *)seen_shot.header)->unknown_04 == 0);
    assert(((struct DotrCursorPacketHeader *)seen_shot.header)->unknown_08 == 0x42421);
    for (i = 0; i < 4; i++) {
        assert(seen_shot.colour[i] == 128);
    }
    assert(seen_shot.unknown_14 == 1.0f);
    assert(seen_shot.vertex[0].u == 0.0f && seen_shot.vertex[0].v == 0.0f);
    assert(seen_shot.vertex[0].xyz.x == 27640 && seen_shot.vertex[0].xyz.y == 29176);
    assert(seen_shot.vertex[0].xyz.z == 0xffffff && seen_shot.vertex[0].xyz.f == 0);
    assert(seen_shot.vertex[1].u == 0.625f && seen_shot.vertex[1].v == 0.875f);
    assert(seen_shot.vertex[1].xyz.x == 0x93f8 && seen_shot.vertex[1].xyz.y == 0x8df8);
    assert(seen_shot.vertex[1].xyz.z == 0xffffff && seen_shot.vertex[1].xyz.f == 0);
}

static int ot_calls, g4_args[16];
static void *g4_packet;
int *ExGetMyOT(void) { ot_calls++; return 0; }
void TAKU_G4_XYWH(void *packet, int x0, int y0, int z0, int f0, int x1, int y1, int z1, int f1,
                  int x2, int y2, int z2, int f2, int x3, int y3, int z3, int f3)
{
    int v[16];
    v[0] = x0; v[1] = y0; v[2] = z0; v[3] = f0; v[4] = x1; v[5] = y1; v[6] = z1; v[7] = f1;
    v[8] = x2; v[9] = y2; v[10] = z2; v[11] = f2; v[12] = x3; v[13] = y3; v[14] = z3; v[15] = f3;
    memcpy(g4_args, v, sizeof(v));
    g4_packet = packet;
    assert(calls == 0 && ot_calls == 1);
}

/* One call per switch arm (0, 1, 2, 3) and two for the default (4 and 0xffff): every stored
 * value, forwarded argument, the alpha index and the result. */
static void check_prim_g4(void)
{
    static const unsigned short modes[6] = {0, 1, 2, 3, 4, 0xffff};
    struct DotrCursorPacketHeader header;
    int m, v, i;

    for (m = 0; m < 6; m++) {
        calls = ot_calls = base_calls = 0;
        base_mode = 1;
        int_args[5] = int_args[6] = -1;
        assert(Taku_Prim_Set_G4(0x1000, 1, -2, 3, 4, 5, 6, 7, -8, 10, 11, 12, 13, 20, 21, 22, 23, 30, 31, 32, 33,
                                40, 41, 42, 43, modes[m], -16)
               == 0x1200);
        base_mode = 0;
        assert(ot_calls == 1 && calls == 3 && order[0] == 1 && order[1] == 2 && order[2] == 4);
        assert(int_args[0] == (modes[m] == 3 ? 2 : 0) && header_value == 0x1122334455L);
        assert(int_args[5] == 0 && int_args[6] == 0);
        memcpy(&header, seen_base[0].header, sizeof(header));
        assert(header.unknown_00 == 8 && header.unknown_02 == 20 && header.unknown_04 == 0);
        assert(header.unknown_08 == 0x41414141 && seen_header.unknown_08 == 0x41414141);
        for (v = 0; v < 4; v++) {
            for (i = 0; i < 4; i++) {
                assert(seen_base[0].v[v].colour[i] == 10 * (v + 1) + i);
            }
            assert(seen_base[0].v[v].q == 1.0f);
            assert(g4_args[v * 4] == (v == 0 ? 1 : 2 * v + 1));
            assert(g4_args[v * 4 + 1] == (v == 0 ? -2 : v == 3 ? -8 : 2 * v + 2));
            assert(g4_args[v * 4 + 2] == 0xfffff8 - 16 && g4_args[v * 4 + 3] == 0);
        }
    }
}

static void check_blur_case(unsigned char *colour, int r, int g, int b, int a)
{
    struct DotrCursorPacketHeader *header = (struct DotrCursorPacketHeader *)seen_blur.header;

    calls = 0;
    blur_mode = 1;
    memset(&seen_blur, 0x55, sizeof seen_blur);
    assert(ExSortDispBlur_Pipe(0x1000, 7, 8, 9, 0x1234567890L, colour) == 0x1200);
    blur_mode = 0;
    /* Alpha, frame buffer, then the sort call with the two pass-through arguments. */
    assert(calls == 3 && order[0] == 2 && order[1] == 9 && order[2] == 4);
    assert(header_value == 0x1234567890L && blur_buffer == 9 && int_args[5] == 7 && int_args[6] == 8);
    assert(seen_header.unknown_00 == 5 && seen_header.unknown_02 == 38);
    assert(blur_header.unknown_04 == 0 && blur_header.unknown_08 == 0x42421);
    assert(header->unknown_00 == 5 && header->unknown_02 == 38 && header->unknown_04 == 0);
    assert(header->unknown_08 == 0x42421);
    assert(seen_blur.colour[0] == r && seen_blur.colour[1] == g && seen_blur.colour[2] == b);
    assert(seen_blur.colour[3] == a && seen_blur.unknown_14 == 1.0f);
    assert(seen_blur.vertex[0].u == 0.0f && seen_blur.vertex[0].v == 0.0f);
    assert(seen_blur.vertex[0].xyz.x == 27640 && seen_blur.vertex[0].xyz.y == 29176);
    assert(seen_blur.vertex[0].xyz.z == 0xfffff7 && seen_blur.vertex[0].xyz.f == 0);
    assert(seen_blur.vertex[1].u == 0.625f && seen_blur.vertex[1].v == 0.875f);
    assert(seen_blur.vertex[1].xyz.x == 0x93f8 && seen_blur.vertex[1].xyz.y == 0x8df8);
    assert(seen_blur.vertex[1].xyz.z == 0xfffff7 && seen_blur.vertex[1].xyz.f == 0);
}

static void check_blur(void)
{
    unsigned char colour[4] = {1, 2, 3, 4};

    /* A colour pointer: bytes 2, 1, 0, 3. Null: the same bytes of the default object. */
    check_blur_case(colour, 3, 2, 1, 4);
    check_blur_case(0, 33, 32, 31, 34);
}

static void check_soft_case(unsigned char *colour, int r, int g, int b, int a)
{
    struct DotrCursorPacketHeader *header = (struct DotrCursorPacketHeader *)seen_soft.header;

    calls = 0;
    soft_mode = 1;
    memset(&seen_soft, 0x55, sizeof seen_soft);
    assert(ExSortSoftFocus_Pipe(0x1000, 7, 8, 9, 100, 200, colour) == 0x1000 + 0x20 + 0x1000 + 0x20 + 0x200);
    soft_mode = 0;
    /* Alpha, flush, GS address, frame buffer to texture, flush, then the sort call. */
    assert(calls == 6 && order[0] == 2 && order[1] == 6 && order[2] == 10 && order[3] == 8);
    assert(order[4] == 6 && order[5] == 4);
    assert(header_value == 0x8000000044L && soft_args[0] == 0x3456 && soft_args[1] == 9);
    assert(int_args[5] == 7 && int_args[6] == 8);
    assert(seen_header.unknown_00 == 5 && seen_header.unknown_02 == 38);
    assert(seen_header.unknown_04 == 0 && seen_header.unknown_08 == 0x42421);
    assert(header->unknown_00 == 5 && header->unknown_02 == 38 && header->unknown_04 == 0);
    assert(header->unknown_08 == 0x42421);
    assert(seen_soft.colour[0] == r && seen_soft.colour[1] == g && seen_soft.colour[2] == b);
    assert(seen_soft.colour[3] == a && seen_soft.unknown_14 == 1.0f);
    assert(seen_soft.vertex[0].u == 0.0f && seen_soft.vertex[0].v == 0.0f);
    assert(seen_soft.vertex[0].xyz.x == 100 + 27640 && seen_soft.vertex[0].xyz.y == 200 + 29176);
    assert(seen_soft.vertex[0].xyz.z == 0xffffff && seen_soft.vertex[0].xyz.f == 0);
    assert(seen_soft.vertex[1].u == 0.625f && seen_soft.vertex[1].v == 0.875f);
    assert(seen_soft.vertex[1].xyz.x == 100 + 0x93f8 && seen_soft.vertex[1].xyz.y == 200 + 0x8df8);
    assert(seen_soft.vertex[1].xyz.z == 0xffffff && seen_soft.vertex[1].xyz.f == 0);
    /* Bit 34 set after the callee, keeping the bits it wrote; the second word untouched. */
    assert(seen_soft.tex0.flag == 1 && seen_soft.tex0.low == 0x2aaaaaaaaUL);
    assert(seen_soft.tex1.word == 0x7777);
}

static void check_soft(void)
{
    unsigned char colour[4] = {1, 2, 3, 4};

    /* A colour pointer: bytes 2, 1, 0, 3. Null: the same bytes of the 0x293ed8 default. */
    check_soft_case(colour, 3, 2, 1, 4);
    check_soft_case(0, 63, 62, 61, 64);
}

static void check_rectg_vertex(int n, const unsigned char *c, int x, int y, int u, int v)
{
    struct DotrCursorSlotUv uv;

    assert(seen_rectg.vertex[n].colour[0] == c[2] && seen_rectg.vertex[n].colour[1] == c[1]);
    assert(seen_rectg.vertex[n].colour[2] == c[0] && seen_rectg.vertex[n].colour[3] == c[3]);
    assert(seen_rectg.vertex[n].unknown_04 == 1.0f);
    assert(seen_rectg.vertex[n].xyz.x == (unsigned)x << 4 && seen_rectg.vertex[n].xyz.y == (unsigned)y << 4);
    assert(seen_rectg.vertex[n].xyz.z == 0xabcdef && seen_rectg.vertex[n].xyz.f == 0);
    memcpy(&uv, &seen_rectg.vertex[n].uv, sizeof(uv));
    assert(uv.u == (u << 4) + 8 && uv.v == (v << 4) + 8);
}

/* SzMsg_DrawRectG_Pipe (branch coverage: no entry, flag clear, drawn with caller rect and colours,
 * drawn with both defaults). */
static void check_rectg(void)
{
    static unsigned char colour[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    static struct DotrSlotTableWords rect = {{100, 200, 30, 40}};
    struct DotrCursorPacketHeader *header = (struct DotrCursorPacketHeader *)seen_rectg.header;
    int z = (int)0xffabcdef;

    assert(sizeof(struct DotrCursorRectGPacket) == 144 && offsetof(struct DotrCursorRectGPacket, tex0) == 120);
    memcpy(gSzMsg_ml + 628, &z, sizeof(z));
    rectg_mode = 1;
    slot_result = 0;
    calls = tex_calls = 0;
    assert(SzMsg_DrawRectG_Pipe(0x1000, 9, 5, 6, &rect, colour) == 0x1000);
    assert(info_index == 9 && calls == 0 && tex_calls == 0);

    slot_result = &slot_entry;
    slot_entry.flag = 0;
    slot_entry.unknown_004[0] = 16;
    slot_entry.unknown_004[1] = 32;
    slot_entry.unknown_004[2] = 8;
    slot_entry.unknown_004[3] = 4;
    assert(SzMsg_DrawRectG_Pipe(0x1000, 9, 5, 6, &rect, colour) == 0x1000 && calls == 0 && tex_calls == 0);

    slot_entry.flag = -1;
    int_args[5] = int_args[6] = -1;
    assert(SzMsg_DrawRectG_Pipe(0x1000, 9, 5, 6, &rect, colour) == 0x1200);
    assert(calls == 2 && order[0] == 2 && order[1] == 4 && tex_calls == 1 && header_value == 0x8000000044L);
    assert(seen_header.unknown_00 == 12 && seen_header.unknown_02 == (52 | 0x800));
    assert(seen_header.unknown_04 == 0 && seen_header.unknown_08 == 0x421421421421L);
    assert(header->unknown_00 == 12 && header->unknown_02 == (52 | 0x800));
    assert(header->unknown_04 == 0 && header->unknown_08 == 0x421421421421L);
    assert(int_args[5] == 0 && int_args[6] == 0);
    check_rectg_vertex(0, colour, 105, 206, 16, 32);
    check_rectg_vertex(1, colour + 4, 135, 206, 24, 32);
    check_rectg_vertex(2, colour + 8, 105, 246, 16, 36);
    check_rectg_vertex(3, colour + 12, 135, 246, 24, 36);
    assert(seen_rectg.tex0.word == 0x1111 && seen_rectg.tex1.word == 0x2222);

    calls = 0;
    assert(SzMsg_DrawRectG_Pipe(0x1000, 9, 5, 6, 0, 0) == 0x1200 && calls == 2);
    check_rectg_vertex(0, icolDef_226, 6, 8, 16, 32);
    check_rectg_vertex(1, icolDef_226 + 4, 9, 8, 24, 32);
    check_rectg_vertex(2, icolDef_226 + 8, 6, 12, 16, 36);
    check_rectg_vertex(3, icolDef_226 + 12, 9, 12, 24, 36);
    rectg_mode = 0;
}

/* SzMsg_DrawRatioX_Pipe (0x1fe660). Branch coverage: no entry (cursor returned, callback not
 * called), and an entry whose width 100 and height 40 are scaled by 0.5 and 2.0. */
struct DotrCursorRatioRect gSzMsg_defRect_rect = {3, 4, 5, 6};
int func_001a22c0(float value) { return (int)value; }
static int ratio_calls;
static DotrDrawWord ratio_draw(DotrDrawWord cursor, int index, int x, int y, struct DotrCursorRatioRect rect, int e)
{
    ratio_calls++;
    assert(cursor == 0x700 && index == 9 && x == 11 && y == 12 && e == 13);
    assert(rect.unknown_0 == 3 && rect.unknown_4 == 4 && rect.width == 50 && rect.height == 80);
    return 0x7777;
}
static void check_ratio(void)
{
    struct DotrCursorSlotEntry ratio_entry;

    memset(&ratio_entry, 0, sizeof(ratio_entry));
    ratio_entry.unknown_004[2] = 100;
    ratio_entry.unknown_004[3] = 40;
    slot_result = 0;
    info_index = -1;
    assert(SzMsg_DrawRatioX_Pipe(0x700, 9, 11, 12, 0.5f, 2.0f, 13, ratio_draw) == 0x700);
    assert(info_index == 9 && ratio_calls == 0);
    slot_result = &ratio_entry;
    assert(SzMsg_DrawRatioX_Pipe(0x700, 9, 11, 12, 0.5f, 2.0f, 13, ratio_draw) == 0x7777);
    assert(ratio_calls == 1 && gSzMsg_defRect_rect.width == 5 && gSzMsg_defRect_rect.height == 6);
}

/* lSzDuel_DrawWindow_DualShock2_Pipe (0x1b7820). One select (side 0 / non-zero: base angle
 * and player record); everything else is straight-line: every stored value, forwarded
 * argument and the result are checked for both sides. The geometry stubs copy the corners
 * to the scratch block unrotated. */
struct DotrCursorPadIcon data_00290800[4];
struct DotrCursorDs2Quad data_002912e0;
struct DotrCursorHandRotation data_00291320 = { {1, 2, 3, 4} };
float dotr_cursor_pad_scratch_70000000[8][4];
static float *pad_matrix;
static int pad_angles[4];
static float pad_corners[8][4];
void RhGeom_GetRotXYZMatrix(float *m, int *ang)
{
    order[calls++] = 20;
    pad_matrix = m;
    memcpy(pad_angles, ang, sizeof(pad_angles));
}
void RhGeom_PushMatrix(float *matrix)
{
    order[calls++] = 21;
    assert(matrix == pad_matrix);
}
void RhGeom_MulMatrixVectorArray(float *out, float *m, float *in, int count)
{
    order[calls++] = 22;
    assert(out == dotr_cursor_pad_scratch_70000000[0] && m == 0 && count == (hand_mode ? 8 : 4));
    memcpy(pad_corners, in, (size_t)count * 16);
    memcpy(out, in, (size_t)count * 16);
}
void RhGeom_PopMatrix(void) { order[calls++] = 23; }
static int pad_vertex(int n, int u, int v, int x, int y)
{
    struct DotrCursorSlotUv uv;

    memcpy(&uv, &seen_pad.vertex[n].uv, sizeof(uv));
    return uv.u == u && uv.v == v && seen_pad.vertex[n].xyz.x == (unsigned)x && seen_pad.vertex[n].xyz.y == (unsigned)y &&
           seen_pad.vertex[n].xyz.z == 0xabcdef && seen_pad.vertex[n].xyz.f == 0;
}
static void check_pad_icon(void)
{
    static const int expect_order[9] = {2, 7, 20, 21, 22, 23, 3, 4, 3};
    int side, i, n;

    assert(offsetof(struct DotrCursorPicControl, textures) == 0x6a0 && offsetof(struct DotrCursorPicControl, z) == 0xc58);
    assert(offsetof(struct DotrCursorPicControl, side) == 0xd3c && offsetof(struct DotrCursorPicPlayer, unknown_1d4) == 0x1d4);
    assert(offsetof(struct DotrCursorPadPacket, vertex) == 32 && offsetof(struct DotrCursorPadPacket, unknown_70) == 112);
    data_00290800[1].texture = 7;
    data_00290800[1].u0 = 10;
    data_00290800[1].v0 = 20;
    data_00290800[1].u1 = 50;
    data_00290800[1].v1 = 44;
    for (i = 0; i < 4; i++) {
        data_002912e0.v[i][0] = 9.0f;
        data_002912e0.v[i][1] = 9.0f;
        data_002912e0.v[i][2] = 5.0f;
        data_002912e0.v[i][3] = 1.0f;
    }
    data_00317490.textures[7] = 0x4321;
    data_00317490.z = (int)0xffabcdef;
    data_00317490.players[0].unknown_1d4 = 0x5000;
    data_00317490.players[1].unknown_1d4 = 0xc800;
    pad_mode = 1;
    for (side = 0; side < 2; side++) {
        data_00317490.side = side;
        calls = 0;
        zmode_calls = 0;
        memset(&seen_pad, 0, sizeof(seen_pad));
        assert(lSzDuel_DrawWindow_DualShock2_Pipe(0x1000, 10, 20, 0x66) == 0x1120);
        assert(calls == 9 && zmode_args[0] == 0 && zmode_args[1] == 1);
        for (n = 0; n < 9; n++) {
            assert(order[n] == expect_order[n]);
        }
        assert(seen_header.unknown_00 == 9 && seen_header.unknown_02 == (36 | 0x800));
        assert(seen_header.unknown_04 == 0 && seen_header.unknown_08 == 0x424242421L && header_value == 0x8000000044L);
        assert(pad_angles[0] == 1 && pad_angles[1] == 2 && pad_angles[3] == 4);
        assert(pad_angles[2] == (side ? -0x800 : -0x1000));
        assert(pad_corners[0][0] == -20.0f && pad_corners[0][1] == -12.0f && pad_corners[1][0] == 20.0f && pad_corners[1][1] == -12.0f);
        assert(pad_corners[2][0] == -20.0f && pad_corners[2][1] == 12.0f && pad_corners[3][0] == 20.0f && pad_corners[3][1] == 12.0f);
        assert(pad_corners[0][2] == 5.0f && pad_corners[3][3] == 1.0f);
        assert(seen_pad.colour[0] == 128 && seen_pad.colour[1] == 128 && seen_pad.colour[2] == 128);
        assert(seen_pad.colour[3] == 0x66 && seen_pad.unknown_14 == 1.0f);
        assert(pad_vertex(0, 160, 320, 27808 - 320 - 8, 29504 - 192 - 8));
        assert(pad_vertex(1, 800, 320, 27808 + 320 - 8, 29504 - 192 - 8));
        assert(pad_vertex(2, 160, 704, 27808 - 320 - 8, 29504 + 192 - 8));
        assert(pad_vertex(3, 800, 704, 27808 + 320 - 8, 29504 + 192 - 8));
    }
    assert(data_00291320.v[2] == 3 && data_002912e0.v[0][0] == 9.0f);
    pad_mode = 0;
}

/* lSzDuel_OpenHandCursor_Pipe (0x1e4570). Branch coverage: one case per quadrant of the
 * angle (the four outcomes of the switch, including the default), one with the angle at
 * -0x10000 (remainder zero), and one with both icons' corners reversed (the negative side
 * of each absolute difference). Every stored value, forwarded argument and the result are
 * checked. Angles below -0x10000 are not exercised: there the original's instructions
 * differ from C's remainder. The geometry stubs copy the corners to the scratch block
 * unrotated. */
struct DotrCursorHandQuad data_00291600_hand;
struct DotrCursorHandRotation data_00291680_hand = {{5, 6, 7, 8}};
static int hand_vertex(int p, int n, int u, int v, int x, int y)
{
    struct DotrCursorSlotUv uv;

    memcpy(&uv, &seen_hand[p].vertex[n].uv, sizeof(uv));
    return uv.u == u && uv.v == v && seen_hand[p].vertex[n].xyz.x == (unsigned)x && seen_hand[p].vertex[n].xyz.y == (unsigned)y &&
           seen_hand[p].vertex[n].xyz.z == 0xabcdef && seen_hand[p].vertex[n].xyz.f == 0;
}
static void check_open_hand(void)
{
    static const int expect_order[9] = {20, 21, 22, 23, 2, 7, 4, 7, 4};
    static const int angles[6] = {0x1000, 0x5000, -0x7f00, 0xc800, -0x10000, 0x1000};
    static const int expect_z[6] = {0x100000, 0x500000, -0x7f0000, -0x380000, 0, 0x100000};
    static const int expect_dx[6] = {8, 8, -8, -8, 8, 8};
    static const int expect_dy[6] = {8, -8, -8, 8, 8, 8};
    int k, i, n, p, dx, dy;

    for (i = 0; i < 8; i++) {
        data_00291600_hand.v[i][0] = 9.0f;
        data_00291600_hand.v[i][1] = 9.0f;
        data_00291600_hand.v[i][2] = 5.0f;
        data_00291600_hand.v[i][3] = 1.0f;
    }
    data_00317490.textures[7] = 0x4321;
    data_00317490.textures[9] = 0x8765;
    data_00317490.z = (int)0xffabcdef;
    hand_mode = 1;
    for (k = 0; k < 6; k++) {
        data_00290800[2].texture = 7;
        data_00290800[2].u0 = k == 5 ? 50 : 10;
        data_00290800[2].v0 = k == 5 ? 44 : 20;
        data_00290800[2].u1 = k == 5 ? 10 : 50;
        data_00290800[2].v1 = k == 5 ? 20 : 44;
        data_00290800[3].texture = 9;
        data_00290800[3].u0 = k == 5 ? 36 : 4;
        data_00290800[3].v0 = k == 5 ? 70 : 6;
        data_00290800[3].u1 = k == 5 ? 4 : 36;
        data_00290800[3].v1 = k == 5 ? 6 : 70;
        calls = 0;
        hand_sorts = 0;
        hand_tex_calls = 0;
        memset(seen_hand, 0, sizeof(seen_hand));
        assert(lSzDuel_OpenHandCursor_Pipe(0x1000, angles[k], 10, 20) == 0x1200);
        assert(calls == 9 && hand_sorts == 2 && hand_tex_calls == 2);
        for (n = 0; n < 9; n++) {
            assert(order[n] == expect_order[n]);
        }
        assert(hand_textures[0] == (int *)(intptr_t)0x4321 && hand_textures[1] == (int *)(intptr_t)0x8765);
        assert(seen_header.unknown_00 == 9 && seen_header.unknown_02 == (36 | 0x800));
        assert(seen_header.unknown_04 == 0 && seen_header.unknown_08 == 0x424242421L && header_value == 0x8000000044L);
        assert(pad_angles[0] == 5 && pad_angles[1] == 6 && pad_angles[3] == 8 && pad_angles[2] == expect_z[k]);
        assert(pad_corners[0][0] == -20.0f && pad_corners[0][1] == -12.0f && pad_corners[1][0] == 20.0f && pad_corners[1][1] == -12.0f);
        assert(pad_corners[2][0] == -20.0f && pad_corners[2][1] == 12.0f && pad_corners[3][0] == 20.0f && pad_corners[3][1] == 12.0f);
        assert(pad_corners[4][0] == -20.0f && pad_corners[4][1] == -12.0f && pad_corners[5][0] == 12.0f && pad_corners[5][1] == -12.0f);
        assert(pad_corners[6][0] == -20.0f && pad_corners[6][1] == 52.0f && pad_corners[7][0] == 12.0f && pad_corners[7][1] == 52.0f);
        assert(pad_corners[0][2] == 5.0f && pad_corners[7][3] == 1.0f);
        dx = expect_dx[k];
        dy = expect_dy[k];
        for (p = 0; p < 2; p++) {
            struct DotrCursorPadIcon *t = &data_00290800[2 + p];

            assert(seen_hand[p].colour[0] == 128 && seen_hand[p].colour[1] == 128 && seen_hand[p].colour[2] == 128);
            assert(seen_hand[p].colour[3] == 128 && seen_hand[p].unknown_14 == 1.0f);
            assert(hand_vertex(p, 0, (t->u0 << 4) + dx, (t->v0 << 4) + dy, 32928 - 320, 33088 - 192));
            assert(hand_vertex(p, 1, (t->u1 << 4) - dx, (t->v0 << 4) + dy, 32928 + (p ? 192 : 320), 33088 - 192));
            assert(hand_vertex(p, 2, (t->u0 << 4) + dx, (t->v1 << 4) - dy, 32928 - 320, 33088 + (p ? 832 : 192)));
            assert(hand_vertex(p, 3, (t->u1 << 4) - dx, (t->v1 << 4) - dy, 32928 + (p ? 192 : 320), 33088 + (p ? 832 : 192)));
        }
    }
    assert(data_00291680_hand.v[2] == 7 && data_00291600_hand.v[0][0] == 9.0f);
    hand_mode = 0;
}

int main(void)
{
    check_ratio();
    check_rectg();
    check_blur();
    check_soft();
    check_prim_g4();
    check_model_shot();
    check_shot_texture();
    check_rect();
    check_fixed_rect();
    check_join_fade();
    check_state_rect();
    check_slot_rect();
    check_pic_mini();
    check_pic_item();
    check_pad_icon();
    check_open_hand();
    check_base(lSzDuel_DrawBuildDeck_Base_Pipe, data_002be850);
    check_base(lSzDuel_DrawTrade_Base_Pipe, data_002be858);
    check_base(lSzDuel_DrawCustomDuel_Base_Pipe, data_002be860);
    return 0;
}

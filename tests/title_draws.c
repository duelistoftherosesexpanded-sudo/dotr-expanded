/* Synthetic host tests for the title_draws group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/title_draws.h"

unsigned char data_00317476;
unsigned char data_00317490_bytes[3164];
unsigned char gSzDuel_aaicolRgba[16][8];
struct DotrTitleRotation data_002912d0;
int RotY;
int data_003180e8;

static int calls, order[32], disp_result, disp_index, zbuf[2], zbuf_calls, get_index, set_index[2], set_calls, draw_words[4],
    normal_calls, push_result, frame_args[3], draw_args[6], sort_args[2], view_calls, modify_calls;
static long alpha_value;
static float clip[2];
static void *draw_d;
static struct DotrCursorPacketHeader alpha_header;
static struct DotrTitleViewBlock seen_view[2];
static struct DotrCursorSlotRectPacket seen_packet;
static struct DotrTitleFlatPacket seen_flat;
static struct DotrTitleIntensity set_value[2];
static float root_arg;

int ExAssignAlpha(struct DotrCursorPacketHeader *packet, long value)
{
    order[calls++] = 1;
    alpha_header = *packet;
    alpha_value = value;
    return 0;
}
int RhView_GetCurView(struct DotrViewBlock *out)
{
    order[calls++] = 2;
    memset(out, 0x11 * ++view_calls, sizeof *out);
    return 0;
}
int RhView_PushStack(void) { order[calls++] = 3; return push_result; }
int RhBase_AssignRenderFrame(int index, int mode, int kind)
{
    order[calls++] = 4;
    frame_args[0] = index;
    frame_args[1] = mode;
    frame_args[2] = kind;
    return 0;
}
DotrDrawWord RhBase_EnableRenderFrame(DotrDrawWord p) { order[calls++] = 5; return p + 1; }
DotrDrawWord RhBase_ClearRenderFrame(DotrDrawWord p, void *color)
{
    order[calls++] = 6;
    assert(color == 0);
    return p + 2;
}
int RhView_SetClipZ(float a, float b) { order[calls++] = 7; clip[0] = a; clip[1] = b; return 0; }
int RhView_Modify(struct DotrViewBlock *block)
{
    order[calls++] = 8;
    memcpy(&seen_view[modify_calls++ & 1], block, sizeof *block);
    return 0;
}
struct DotrDrawTag *RhView_SendViewParameters(struct DotrDrawTag *tag)
{
    order[calls++] = 9;
    return (struct DotrDrawTag *)((DotrDrawWord)tag + 4);
}
DotrDrawWord SzModel_DrawWithOutCalc_Pipe(DotrDrawWord cursor, int index, int c, void *d, int e, int f, int g)
{
    order[calls++] = 10;
    draw_args[0] = index;
    draw_args[1] = c;
    draw_args[2] = e;
    draw_args[3] = f;
    draw_args[4] = g;
    draw_d = d;
    if (d != 0) {
        memcpy(draw_words, d, sizeof draw_words);
    }
    return cursor + 8;
}
int RhBase_GetTextureInfoForRender(struct DotrCursorTex *a, struct DotrCursorTex *b)
{
    order[calls++] = 11;
    assert(b == a + 1);
    a->word = 0x1111;
    b->word = 0x2222;
    return 0;
}
DotrDrawWord RhBase_DisableRenderFrame(DotrDrawWord p) { order[calls++] = 12; return p + 16; }
DotrDrawWord RhText_SetTextureFlush(DotrDrawWord cursor) { order[calls++] = 13; return cursor + 32; }
int RhView_PopStack(void) { order[calls++] = 14; return 0; }
DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, void *packet, int a1, int a2)
{
    order[calls++] = 15;
    sort_args[0] = a1;
    sort_args[1] = a2;
    if (((struct DotrCursorPacketHeader *)packet)->unknown_00 == 3) {
        memcpy(&seen_flat.header, packet, sizeof seen_flat - offsetof(struct DotrTitleFlatPacket, header));
        return cursor + 64;
    }
    memcpy(&seen_packet.header, packet, sizeof seen_packet - offsetof(struct DotrCursorSlotRectPacket, header));
    return cursor + 64;
}
void RhBase_FreeRenderFrame(void) { order[calls++] = 16; }
static int card_args[6];
DotrDrawWord lSzDuel_DrawBuildDeck_Card_Pipe(DotrDrawWord cursor, int id, int x, int y, int a4, int a5)
{
    order[calls++] = 30;
    card_args[0] = id;
    card_args[1] = x;
    card_args[2] = y;
    card_args[3] = a4;
    card_args[4] = a5;
    return cursor + 0x100;
}

/* lSzGame_DrawTitle_Rose_Pipe (branch coverage: RhView_PushStack zero and non-zero; an odd width
 * and a negative odd height show the arithmetic halving). */
static void check_title_rose(void)
{
    static const int expect[19] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 2, 8, 9, 15, 16};
    struct DotrCursorPacketHeader *header = (struct DotrCursorPacketHeader *)seen_packet.header;
    struct DotrCursorSlotUv *uv;
    unsigned char fill[4];
    int i;

    data_00317476 = 0x5a;
    calls = 0;
    push_result = 0;
    assert(lSzGame_DrawTitle_Rose_Pipe(0x1000, 3, 100, 50, 41, -7, 2.5f) == 0x1000);
    assert(calls == 3 && order[0] == 1 && order[1] == 2 && order[2] == 3);
    assert(alpha_value == 0x8000000044 && alpha_header.unknown_00 == 5);
    assert(alpha_header.unknown_02 == (38 | 0x800) && alpha_header.unknown_04 == 0);
    assert(alpha_header.unknown_08 == 0x42421);

    calls = 0;
    view_calls = 0;
    modify_calls = 0;
    push_result = 1;
    assert(lSzGame_DrawTitle_Rose_Pipe(0x1000, 3, 100, 50, 41, -7, 2.5f) == 0x1000 + 131);
    assert(calls == 19);
    for (i = 0; i < 19; i++) {
        assert(order[i] == expect[i]);
    }
    assert(frame_args[0] == 0 && frame_args[1] == 2 && frame_args[2] == 50);
    assert(clip[0] == 1.0f && clip[1] == 512.0f);
    assert(draw_args[0] == 3 && draw_args[1] == 0 && draw_args[2] == 0 && draw_args[3] == 0);
    assert(draw_args[4] == 0 && draw_d == 0 && sort_args[0] == 0 && sort_args[1] == 0);
    /* First modify: the current view with seven floats replaced; second: the fresh view. */
    memset(fill, 0x11, sizeof fill);
    for (i = 0; i < 36; i++) {
        if (i == 16 || i == 17 || i == 20 || i == 21 || i == 32) {
            assert(seen_view[0].u.values[i] == 0.0f);
        } else if (i == 18) {
            assert(seen_view[0].u.values[i] == -2.5f);
        } else if (i == 22) {
            assert(seen_view[0].u.values[i] == 1.0f);
        } else {
            assert(memcmp(&seen_view[0].u.values[i], fill, 4) == 0);
        }
    }
    memset(fill, 0x22, sizeof fill);
    for (i = 0; i < 36; i++) {
        assert(memcmp(&seen_view[1].u.values[i], fill, 4) == 0);
    }
    assert(header->unknown_00 == 5 && header->unknown_02 == (38 | 0x800));
    assert(header->unknown_04 == 0 && header->unknown_08 == 0x42421);
    assert(seen_packet.colour[0] == 128 && seen_packet.colour[1] == 128 && seen_packet.colour[2] == 128);
    assert(seen_packet.colour[3] == 0x5a && seen_packet.unknown_14 == 1.0f);
    uv = (struct DotrCursorSlotUv *)&seen_packet.vertex[0].uv;
    assert(uv->u == 0 && uv->v == 0);
    uv = (struct DotrCursorSlotUv *)&seen_packet.vertex[1].uv;
    assert(uv->u == 10240 && uv->v == 7168);
    assert(seen_packet.vertex[0].xyz.x == 34040 && seen_packet.vertex[0].xyz.y == 33624);
    assert(seen_packet.vertex[0].xyz.z == 0xfffff8 && seen_packet.vertex[0].xyz.f == 0);
    assert(seen_packet.vertex[1].xyz.x == 34680 && seen_packet.vertex[1].xyz.y == 33496);
    assert(seen_packet.vertex[1].xyz.z == 0xfffff8 && seen_packet.vertex[1].xyz.f == 0);
    assert(seen_packet.tex0.word == 0x1111 && seen_packet.tex1.word == 0x2222);
}

int SzModel_IsModelDisp(int index) { order[calls++] = 17; disp_index = index; return disp_result; }
DotrDrawWord RhBase_SetZBufferMode(DotrDrawWord cursor, int a)
{
    order[calls++] = 18;
    zbuf[zbuf_calls++ & 1] = a;
    return cursor + 128;
}
int SzModel_GetIntensity(int index, struct DotrTitleIntensity *out)
{
    order[calls++] = 19;
    get_index = index;
    out->x = 0.125f;
    out->y = 0.25f;
    out->z = 0.375f;
    return 0;
}
int SzModel_SetIntensity(int index, struct DotrTitleIntensity *in)
{
    order[calls++] = 20;
    set_index[set_calls & 1] = index;
    set_value[set_calls++ & 1] = *in;
    return 0;
}
float func_001a6c60(float value) { order[calls++] = 21; root_arg = value; return 3.0f; }
void RhGeom_NormalizeVector(struct DotrViewVector *v)
{
    order[calls++] = 22;
    normal_calls += v->x == 0.0f && v->z == 3.0f;
    v->x = 0.5f;
    v->y = 0.25f;
    v->z = -2.0f;
}

/* lSzDuel_DrawWindow_FieldPiece_Pipe (branch coverage: model not displayed; displayed with
 * RhView_PushStack zero; displayed with the push succeeding). */
static void check_field_piece(void)
{
    static const int expect[27] = {15, 17, 1, 2, 3, 4, 5, 6, 21, 22, 8, 9, 18, 19, 20, 10, 20, 18, 11, 12, 13, 14, 2, 8, 9, 15, 16};
    struct DotrCursorPacketHeader *header = (struct DotrCursorPacketHeader *)seen_flat.header;
    struct DotrCursorSlotUv *uv;
    unsigned char fill[4];
    float old;
    int word = 0x34123456;
    int i;

    gSzDuel_aaicolRgba[2][4] = 0xa4;
    gSzDuel_aaicolRgba[2][5] = 0xa5;
    gSzDuel_aaicolRgba[2][6] = 0xa6;
    data_003180e8 = 0x12abcdef;
    memcpy(data_00317490_bytes + 3160, &word, sizeof word);
    data_002912d0.word[0] = 1;
    data_002912d0.word[1] = 2;
    data_002912d0.word[2] = 3;
    data_002912d0.word[3] = 4;
    RotY = 0x700;

    calls = 0;
    disp_result = 0;
    assert(lSzDuel_DrawWindow_FieldPiece_Pipe(0x1000, 2, 5, 4.0f, 10, 20) == 0x1000 + 64);
    assert(calls == 2 && order[0] == 15 && order[1] == 17 && disp_index == 79);
    assert(sort_args[0] == 0 && sort_args[1] == 0);
    assert(header->unknown_00 == 3 && header->unknown_02 == 6 && header->unknown_04 == 0);
    assert(header->unknown_08 == 0x441);
    assert(seen_flat.colour[0] == 0xa6 && seen_flat.colour[1] == 0xa5 && seen_flat.colour[2] == 0xa4);
    assert(seen_flat.colour[3] == 128 && seen_flat.unknown_14 == 1.0f);
    assert(seen_flat.xyz[0].x == 27840 && seen_flat.xyz[0].y == 29536);
    assert(seen_flat.xyz[0].z == 0xabcdef && seen_flat.xyz[0].f == 0);
    assert(seen_flat.xyz[1].x == 29312 && seen_flat.xyz[1].y == 30528);
    assert(seen_flat.xyz[1].z == 0xabcdef && seen_flat.xyz[1].f == 0);
    assert(RotY == 0x700);

    calls = 0;
    disp_result = 1;
    push_result = 0;
    assert(lSzDuel_DrawWindow_FieldPiece_Pipe(0x1000, 2, 5, 4.0f, 10, 20) == 0x1000 + 64);
    assert(calls == 5);
    for (i = 0; i < 5; i++) {
        assert(order[i] == expect[i]);
    }
    assert(alpha_value == 0x8000000044 && alpha_header.unknown_00 == 5);
    assert(alpha_header.unknown_02 == (38 | 0x800) && alpha_header.unknown_04 == 0);
    assert(alpha_header.unknown_08 == 0x42421 && RotY == 0x700);

    calls = 0;
    view_calls = 0;
    modify_calls = 0;
    zbuf_calls = 0;
    set_calls = 0;
    normal_calls = 0;
    push_result = 1;
    assert(lSzDuel_DrawWindow_FieldPiece_Pipe(0x1000, 2, 5, 4.0f, 10, 20) == 0x1000 + 451);
    assert(calls == 27);
    for (i = 0; i < 27; i++) {
        assert(order[i] == expect[i]);
    }
    assert(frame_args[0] == 0 && frame_args[1] == 10 && frame_args[2] == 49);
    memset(fill, 0x11, sizeof fill);
    memcpy(&old, fill, sizeof old);
    assert(root_arg == old * old + old * old && normal_calls == 1);
    for (i = 0; i < 36; i++) {
        if (i == 16) {
            assert(seen_view[0].u.values[i] == -2.0f);
        } else if (i == 17) {
            assert(seen_view[0].u.values[i] == -26.0f);
        } else if (i == 18) {
            assert(seen_view[0].u.values[i] == 8.0f);
        } else if (i == 20) {
            assert(seen_view[0].u.values[i] == 0.5f);
        } else if (i == 21) {
            assert(seen_view[0].u.values[i] == 0.25f);
        } else if (i == 22) {
            assert(seen_view[0].u.values[i] == -2.0f);
        } else if (i == 32) {
            assert(seen_view[0].u.values[i] == 0.0f);
        } else {
            assert(memcmp(&seen_view[0].u.values[i], fill, 4) == 0);
        }
    }
    memset(fill, 0x22, sizeof fill);
    for (i = 0; i < 36; i++) {
        assert(memcmp(&seen_view[1].u.values[i], fill, 4) == 0);
    }
    assert(zbuf[0] == 1 && zbuf[1] == 0 && get_index == 79);
    assert(set_index[0] == 79 && set_value[0].x == 0.125f && set_value[0].y == 0.25f && set_value[0].z == 1.0f);
    assert(set_index[1] == 79 && set_value[1].x == 0.125f && set_value[1].y == 0.25f && set_value[1].z == 0.375f);
    assert(draw_args[0] == 79 && draw_args[1] == 0 && draw_args[2] == 0 && draw_args[3] == 0 && draw_args[4] == 0);
    assert(draw_words[0] == 1 && draw_words[1] == 0x700 && draw_words[2] == 3 && draw_words[3] == 4);
    assert(RotY == 0x900);
    header = (struct DotrCursorPacketHeader *)seen_packet.header;
    assert(header->unknown_00 == 5 && header->unknown_02 == (38 | 0x800));
    assert(header->unknown_04 == 0 && header->unknown_08 == 0x42421);
    assert(seen_packet.colour[0] == 255 && seen_packet.colour[1] == 255 && seen_packet.colour[2] == 255);
    assert(seen_packet.colour[3] == 128 && seen_packet.unknown_14 == 1.0f);
    uv = (struct DotrCursorSlotUv *)&seen_packet.vertex[0].uv;
    assert(uv->u == 0 && uv->v == 0);
    uv = (struct DotrCursorSlotUv *)&seen_packet.vertex[1].uv;
    assert(uv->u == 10240 && uv->v == 7168);
    assert(seen_packet.vertex[0].xyz.x == 27848 && seen_packet.vertex[0].xyz.y == 29544);
    assert(seen_packet.vertex[0].xyz.z == 0x123456 && seen_packet.vertex[0].xyz.f == 0);
    assert(seen_packet.vertex[1].xyz.x == 29288 && seen_packet.vertex[1].xyz.y == 30520);
    assert(seen_packet.vertex[1].xyz.z == 0x123456 && seen_packet.vertex[1].xyz.f == 0);
    assert(seen_packet.tex0.word == 0x1111 && seen_packet.tex1.word == 0x2222);
}

/* 0x1bd8a0: one straight path; the card is rendered at (0, 0) with the flag as fifth argument,
 * the rectangle is 304 x 54 at (x + 1728, y + 1824), the depth keeps its low 24 bits. */
static void check_ghost_card(void)
{
    static const int expect[8] = {1, 5, 6, 30, 11, 12, 13, 15};
    struct DotrCursorPacketHeader *header = (struct DotrCursorPacketHeader *)seen_packet.header;
    struct DotrCursorSlotUv *uv;
    int i;

    calls = 0;
    data_003180e8 = 0x1abcdef;
    memset(&seen_packet, 0x55, sizeof seen_packet);
    assert(lSzDuel_DrawBuildDeck_GhostCard_Pipe(0x1000, 0x2b7, 100, 50, 9, 0x63) == 0x1000 + 1 + 2 + 0x100 + 16 + 32 + 64);
    assert(calls == 8);
    for (i = 0; i < 8; i++) {
        assert(order[i] == expect[i]);
    }
    assert(card_args[0] == 0x2b7 && card_args[1] == 0 && card_args[2] == 0 && card_args[3] == 9 && card_args[4] == 0);
    assert(sort_args[0] == 0 && sort_args[1] == 0);
    assert(alpha_value == 0x8000000044 && alpha_header.unknown_00 == 5);
    assert(alpha_header.unknown_02 == (38 | 0x800) && alpha_header.unknown_04 == 0);
    assert(alpha_header.unknown_08 == 0x42421);
    assert(header->unknown_00 == 5 && header->unknown_02 == (38 | 0x800));
    assert(header->unknown_04 == 0 && header->unknown_08 == 0x42421);
    assert(seen_packet.colour[0] == 128 && seen_packet.colour[1] == 128 && seen_packet.colour[2] == 128);
    assert(seen_packet.colour[3] == 0x63 && seen_packet.unknown_14 == 1.0f);
    uv = (struct DotrCursorSlotUv *)&seen_packet.vertex[0].uv;
    assert(uv->u == 0 && uv->v == 0);
    uv = (struct DotrCursorSlotUv *)&seen_packet.vertex[1].uv;
    assert(uv->u == 4864 && uv->v == 864);
    assert(seen_packet.vertex[0].xyz.x == 1828 * 16 - 8 && seen_packet.vertex[0].xyz.y == 1874 * 16 - 8);
    assert(seen_packet.vertex[0].xyz.z == 0xabcdef && seen_packet.vertex[0].xyz.f == 0);
    assert(seen_packet.vertex[1].xyz.x == 2132 * 16 - 8 && seen_packet.vertex[1].xyz.y == 1928 * 16 - 8);
    assert(seen_packet.vertex[1].xyz.z == 0xabcdef && seen_packet.vertex[1].xyz.f == 0);
    assert(seen_packet.tex0.word == 0x1111 && seen_packet.tex1.word == 0x2222);
}

int main(void)
{
    check_title_rose();
    check_field_piece();
    check_ghost_card();
    return 0;
}

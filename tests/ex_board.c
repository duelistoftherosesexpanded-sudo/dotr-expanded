/* Synthetic host tests for the ex_board group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/ex_board.h"

struct DotrExBoardControl data_00317490;
char gSzDuel_Icon_bytes[190 * 10];
unsigned char gSzDuel_aaicolRgba[16][8];

static int alpha_index[4];
static int alpha_kind[4];
static int alpha_calls;
static int alpha_direct;
static intptr_t textures[16];
static int texture_calls;
static DotrExBoardSprite prims[16];
static int prim_kind[16];
static int prim_count;

long ExGetAlphaVal(int index)
{
    alpha_index[alpha_calls] = index;
    return 0x100 + index;
}

int ExAssignAlpha(struct DotrExBoardHeader *packet, long value)
{
    if (alpha_direct) {
        assert(value == 0x8000000044 && packet->unknown_02 == (38 | 0x800));
    } else {
        assert(value == 0x100 + alpha_index[alpha_calls]);
    }
    alpha_kind[alpha_calls++] = packet->unknown_00;
    return 0;
}

int ExAssignTexture(struct DotrExBoardHeader *packet, int *texture, int a)
{
    assert(packet->unknown_00 == 5 && a == 0);
    textures[texture_calls++] = (intptr_t)texture;
    return 0;
}

/* Host fixture for the texture pointer table at 0x317b30 (the control block's textures as
 * lSzDuel_DrawWindow_Base_Pipe reads them). */
void *data_00317b30_exboard[366];

DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, void *packet, int a1, int a2)
{
    int kind = ((struct DotrExBoardHeader *)packet)->unknown_00;

    assert(a1 == 0 && a2 == 0 && (kind == 5 || kind == 3));
    prim_kind[prim_count] = kind;
    memcpy(prims[prim_count++].header, packet, kind == 5 ? 56 : sizeof(DotrExBoardFlat));
    return cursor + 16;
}

/* lSzDuel_DrawWindow_Base_Pipe's views of the same callees. */
int func_002086d0_base(void *packet, long long alpha)
{
    return ExAssignAlpha(packet, alpha);
}

int func_00208700_base(void *packet, void *texture, int a)
{
    return ExAssignTexture(packet, texture, a);
}

DotrDrawWord func_00207650_base(DotrDrawWord cursor, DotrDrawWord packet, int a2, int a3)
{
    return ExSortPrim_Pipe(cursor, (void *)packet, a2, a3);
}

/* Tile i of a set: texture base + i, texel origin (10 i, 5 i), width 8 + i, height 4 + i. */
static void fill_tiles(int kind, int base)
{
    struct DotrExBoardTile *tile = &((struct DotrExBoardTile *)gSzDuel_Icon_bytes)[kind * 9 + 140];
    int i;

    for (i = 0; i < 9; i++) {
        tile[i].texture = base + i;
        tile[i].u0 = 10 * i;
        tile[i].v0 = 5 * i;
        tile[i].u1 = 10 * i + 8 + i;
        tile[i].v1 = 5 * i + 4 + i;
    }
}

static void reset(void)
{
    int i;

    alpha_calls = texture_calls = prim_count = 0;
    memset(prims, 0, sizeof(prims));
    for (i = 0; i < 16 * 8; i++) {
        gSzDuel_aaicolRgba[i / 8][i % 8] = i + 1;
    }
    for (i = 0; i < 40; i++) {
        data_00317490.textures[i] = 0x7000 + i;
        data_00317b30_exboard[i] = (void *)(intptr_t)(0x7000 + i);
    }
    data_00317490.z = (int)0xffabcdef;
    fill_tiles(0, 1);
    fill_tiles(1, 11);
}

static void check_sprite(int n, int row, int texture, int u0, int v0, int u1, int v1, int x0, int y0, int x1, int y1)
{
    DotrExBoardSprite *p = &prims[n];
    struct DotrExBoardHeader *header = (struct DotrExBoardHeader *)p->header;

    assert(prim_kind[n] == 5 && header->unknown_02 == 0x826 && header->unknown_04 == 0 && header->unknown_08 == 0x42421);
    assert(p->colour[0] == row * 8 + 7 && p->colour[1] == row * 8 + 6 && p->colour[2] == row * 8 + 5 && p->colour[3] == row * 8 + 8);
    assert(p->unknown_14 == 1.0f);
    assert(textures[texture] != 0);
    assert(((struct DotrExBoardUv *)&p->vertex[0].uv)->u == u0 && ((struct DotrExBoardUv *)&p->vertex[0].uv)->v == v0);
    assert(((struct DotrExBoardUv *)&p->vertex[1].uv)->u == u1 && ((struct DotrExBoardUv *)&p->vertex[1].uv)->v == v1);
    assert(p->vertex[0].xyz.x == x0 && p->vertex[0].xyz.y == y0 && p->vertex[0].xyz.z == 0xabcdef && p->vertex[0].xyz.f == 0);
    assert(p->vertex[1].xyz.x == x1 && p->vertex[1].xyz.y == y1 && p->vertex[1].xyz.z == 0xabcdef && p->vertex[1].xyz.f == 0);
}

static void check_flat(int row, int x1, int y1)
{
    DotrExBoardFlat *p = (DotrExBoardFlat *)prims[0].header;
    struct DotrExBoardHeader *header = (struct DotrExBoardHeader *)p->header;

    assert(prim_kind[0] == 3 && header->unknown_02 == 6 && header->unknown_04 == 0 && header->unknown_08 == 1089);
    assert(p->colour[0] == row * 8 + 7 && p->colour[1] == row * 8 + 6 && p->colour[2] == row * 8 + 5 && p->colour[3] == row * 8 + 8);
    assert(p->unknown_14 == 1.0f);
    assert(p->xyz[0].x == 1738 << 4 && p->xyz[0].y == 1844 << 4 && p->xyz[0].z == 0xabcdef && p->xyz[0].f == 0);
    assert(p->xyz[1].x == x1 && p->xyz[1].y == y1 && p->xyz[1].z == 0xabcdef && p->xyz[1].f == 0);
}

/* lSzDuel_DrawWindow_ExBoard_Pipe: one assertion group per branch outcome and boundary. */
static void check_ex_board(void)
{
    int i;

    /* Negative colour selects row 2; no backing; positive repeat counts; centre tile skipped. */
    reset();
    assert(lSzDuel_DrawWindow_ExBoard_Pipe(0x1000, -1, 10, 20, 2, 3, -1, 0) == 0x1000 + 8 * 16);
    assert(prim_count == 8 && texture_calls == 8 && alpha_calls == 1 && alpha_index[0] == 0 && alpha_kind[0] == 5);
    for (i = 0; i < 8; i++) {
        assert(textures[i] == 0x7000 + 1 + i + (i >= 4));
    }
    check_sprite(0, 2, 0, 0, 0, 128, 64, 27800, 29496, 27928, 29560);
    check_sprite(1, 2, 1, 160, 80, 304, 160, 27928, 29496, 28216, 29576);
    check_sprite(7, 2, 7, 1280, 640, 1536, 832, 28504, 30024, 28760, 30216);

    /* Backing at the upper alpha threshold with pixel sizes: |w| and |h| size the rectangle,
     * then the corners are subtracted for the middle tiles. */
    reset();
    assert(lSzDuel_DrawWindow_ExBoard_Pipe(0x2000, 1, 10, 20, -200, -100, 14, 1) == 0x2000 + 9 * 16);
    assert(prim_count == 9 && alpha_calls == 2 && alpha_index[1] == 0 && alpha_kind[1] == 3);
    check_flat(14, (1738 + 200) << 4, (1844 + 100) << 4);
    assert(textures[0] == 0x7000 + 11 && textures[7] == 0x7000 + 19);
    check_sprite(2, 1, 1, 160, 80, 304, 160, 27928, 29496, 30840, 29576);
    check_sprite(4, 1, 3, 480, 240, 656, 352, 27800, 29592, 27976, 30968);

    /* Backing with the second alpha value and positive counts: the rectangle spans both
     * corner tiles plus the repeated middle. */
    reset();
    assert(lSzDuel_DrawWindow_ExBoard_Pipe(0, 0, 10, 20, 5, 7, 12, 0) == 9 * 16);
    assert(alpha_calls == 2 && alpha_index[1] == 2 && alpha_kind[1] == 3);
    check_flat(12, (1738 + 63) << 4, (1844 + 63) << 4);
    check_sprite(1, 0, 0, 0, 0, 128, 64, 27800, 29496, 27928, 29560);
    reset();
    assert(lSzDuel_DrawWindow_ExBoard_Pipe(0, 0, 10, 20, 5, 7, 13, 0) == 9 * 16);
    assert(alpha_calls == 2 && alpha_index[1] == 2);

    /* Below 12 the backing gets no alpha; zero counts drop the middle column and row. */
    reset();
    assert(lSzDuel_DrawWindow_ExBoard_Pipe(0, 3, 10, 20, 0, 0, 11, 0) == 5 * 16);
    assert(prim_count == 5 && alpha_calls == 1);
    check_flat(11, (1738 + 18) << 4, (1844 + 14) << 4);
    assert(textures[0] == 0x7001 && textures[1] == 0x7003 && textures[2] == 0x7007 && textures[3] == 0x7009);
    check_sprite(1, 3, 0, 0, 0, 128, 64, 27800, 29496, 27928, 29560);

    /* back == 0 is the lowest row that draws the backing. */
    reset();
    assert(lSzDuel_DrawWindow_ExBoard_Pipe(0, 0, 10, 20, 1, 1, 0, 0) == 9 * 16);
    assert(prim_count == 9 && prim_kind[0] == 3 && alpha_calls == 1);
    check_flat(0, (1738 + 27) << 4, (1844 + 21) << 4);
}

/* lSzDuel_DrawWindow_Base_Pipe: one assertion set per loop outcome (no rows, rows without
 * columns, a 3 by 2 grid with every cell checked). */
static void check_base(void)
{
    struct DotrExBoardTile *tile = &((struct DotrExBoardTile *)gSzDuel_Icon_bytes)[189];
    int k;

    reset();
    alpha_direct = 1;
    tile->texture = 5;
    tile->u0 = 16;
    tile->v0 = 32;
    tile->u1 = 48;
    tile->v1 = 56;
    data_00317490.z = 0x123456;
    assert(lSzDuel_DrawWindow_Base_Pipe(100, 10, 20, 3, 0) == 100);
    assert(prim_count == 0 && alpha_calls == 1 && alpha_kind[0] == 5 && texture_calls == 1 && textures[0] == 0x7005);

    reset();
    data_00317490.z = 0x123456;
    assert(lSzDuel_DrawWindow_Base_Pipe(100, 10, 20, 0, 2) == 100);
    assert(prim_count == 0 && alpha_calls == 1 && texture_calls == 1);

    reset();
    data_00317490.z = 0x123456;
    assert(lSzDuel_DrawWindow_Base_Pipe(100, 10, 20, 3, 2) == 100 + 6 * 16);
    assert(prim_count == 6 && alpha_calls == 1 && texture_calls == 1 && textures[0] == 0x7005);
    for (k = 0; k < 6; k++) {
        DotrExBoardSprite *p = &prims[k];
        struct DotrExBoardHeader *h = (struct DotrExBoardHeader *)p->header;
        int x0 = 1738 + (k % 3) * 32;
        int y0 = 1844 + (k / 3) * 24;

        assert(prim_kind[k] == 5 && h->unknown_02 == (38 | 0x800) && h->unknown_04 == 0 && h->unknown_08 == 0x42421);
        assert(p->colour[0] == 128 && p->colour[1] == 128 && p->colour[2] == 128 && p->colour[3] == 128);
        assert(p->unknown_14 == 1.0f);
        assert(((struct DotrExBoardUv *)&p->vertex[0].uv)->u == 16 << 4);
        assert(((struct DotrExBoardUv *)&p->vertex[0].uv)->v == 32 << 4);
        assert(((struct DotrExBoardUv *)&p->vertex[1].uv)->u == 48 << 4);
        assert(((struct DotrExBoardUv *)&p->vertex[1].uv)->v == 56 << 4);
        assert((int)p->vertex[0].xyz.x == (x0 << 4) - 8 && (int)p->vertex[0].xyz.y == (y0 << 4) - 8);
        assert((int)p->vertex[1].xyz.x == ((x0 + 32) << 4) - 8 && (int)p->vertex[1].xyz.y == ((y0 + 24) << 4) - 8);
        assert(p->vertex[0].xyz.z == 0x123456 && p->vertex[0].xyz.f == 0);
        assert(p->vertex[1].xyz.z == 0x123456 && p->vertex[1].xyz.f == 0);
    }
    alpha_direct = 0;
}

int main(void)
{
    check_ex_board();
    check_base();
    return 0;
}

/* Synthetic host tests for the window_ex_icon group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/window_ex_icon.h"

struct DotrExIconState data_00317490_exicon;
unsigned char data_002909f4_exicon[10] __attribute__((aligned(4)));
unsigned short data_00318118_exicon;

struct sorted {
    int sprite;
    struct DotrExIconSprite s;
    struct DotrExIconSolid q;
};

static struct sorted out[8];
static int nout, alpha_calls, tex_calls;
static long long alphas[4];
static void *textures[4];
static DotrExIconWord next_cursor;

int ExAssignAlpha(void *packet, long long alpha)
{
    assert(packet != NULL && alpha_calls < 4);
    alphas[alpha_calls++] = alpha;
    return 0;
}

long long ExGetAlphaVal(int index) { return 0x7000 + index; }

int ExAssignTexture(void *packet, void *texture, int a)
{
    assert(packet != NULL && a == 0 && tex_calls < 4);
    textures[tex_calls++] = texture;
    return 0;
}

DotrExIconWord ExSortPrim_Pipe(DotrExIconWord cursor, DotrExIconWord packet, int a2, int a3)
{
    const struct DotrExIconTag *tag = (const struct DotrExIconTag *)packet;

    assert(a2 == 0 && a3 == 0 && nout < 8 && cursor == next_cursor);
    out[nout].sprite = tag->count == 5;
    if (out[nout].sprite) {
        memcpy(&out[nout].s, (const void *)packet, sizeof(out[nout].s));
    } else {
        memcpy(&out[nout].q, (const void *)packet, sizeof(out[nout].q));
    }
    nout++;
    return ++next_cursor;
}

static void reset(void)
{
    nout = 0;
    alpha_calls = 0;
    tex_calls = 0;
    next_cursor = 50;
    memset(out, 0, sizeof(out));
    data_00317490_exicon.depth = 0x123456;
    data_00317490_exicon.textures[3] = (void *)0x3333;
    data_00317490_exicon.textures[2] = (void *)0x2222;
    data_00318118_exicon = 0x4400;
}

static struct DotrExIconUv uv(unsigned long long word)
{
    struct DotrExIconUv u;

    memcpy(&u, &word, sizeof(u));
    return u;
}

static void check_rect(const struct DotrExIconXyz *a, const struct DotrExIconXyz *b, int x, int y, int w, int h)
{
    assert(a->x == (unsigned)(x * 16) && a->y == (unsigned)(y * 16));
    assert(b->x == (unsigned)((x + w) * 16) && b->y == (unsigned)((y + h) * 16));
    assert(a->z == 0x123456 && b->z == 0x123456 && a->fog == 0 && b->fog == 0);
}

static void test_image_only(void)
{
    unsigned short icon[5] = { 3, 10, 20, 50, 4 };
    struct DotrExIconUv u0, u1;
    const struct DotrExIconTag *tag;

    reset();
    /* Not flipped: u/v = coordinate * 16 + 8, clamped to 0x3fff; v1 - v0 is negative here. */
    assert(lSzDuel_DrawWindow_ExIcon_Pipe(50, icon, 7, 9, 0, 1, 0, 0, 0, 0, 1, NULL, -1, 0x55) == 51);
    assert(nout == 1 && out[0].sprite && tex_calls == 1 && textures[0] == (void *)0x3333);
    assert(alpha_calls == 1 && alphas[0] == 0x8000000044LL);
    tag = (const struct DotrExIconTag *)out[0].s.tag;
    assert(tag->count == 5 && tag->flags == (0x26 | 0x800) && tag->registers == 0x42421 && tag->reserved == 0);
    assert(out[0].s.color.r == 0x80 && out[0].s.color.g == 0x80 && out[0].s.color.b == 0x80);
    assert(out[0].s.color.a == 0x55 && out[0].s.color.q == 1.0f);
    u0 = uv(out[0].s.uv0);
    u1 = uv(out[0].s.uv1);
    assert(u0.u == 10 * 16 + 8 && u0.v == 20 * 16 + 8 && u1.u == 50 * 16 + 8 && u1.v == 4 * 16 + 8);
    check_rect(&out[0].s.xyz0, &out[0].s.xyz1, 7, 9, 40, 16);
}

static void test_flips_and_clamps(void)
{
    unsigned short icon[5] = { 2, 0, 0x400, 30, 40 };
    struct DotrExIconUv u0, u1;

    reset();
    /* Both flipped: max(coordinate * 16 - 8, 0), u/v swapped; dim intensity 0x30. */
    lSzDuel_DrawWindow_ExIcon_Pipe(50, icon, 0, 0, 0, 1, 0, 0, 1, 1, 0, NULL, -1, 0x80);
    u0 = uv(out[0].s.uv0);
    u1 = uv(out[0].s.uv1);
    assert(out[0].s.color.r == 0x30 && textures[0] == (void *)0x2222);
    assert(u0.u == 30 * 16 - 8 && u0.v == 40 * 16 - 8 && u1.u == 0 && u1.v == 0x400 * 16 - 8);

    reset();
    /* Flip x only: u from the right minus 8, v from the top plus 8 clamped at 0x3fff. */
    lSzDuel_DrawWindow_ExIcon_Pipe(50, icon, 0, 0, 0, 1, 0, 0, 1, 0, 1, NULL, -1, 0x80);
    u0 = uv(out[0].s.uv0);
    u1 = uv(out[0].s.uv1);
    assert(u0.u == 30 * 16 - 8 && u0.v == 0x3fff && u1.u == 0 && u1.v == 40 * 16 + 8);

    reset();
    /* Flip y only. */
    lSzDuel_DrawWindow_ExIcon_Pipe(50, icon, 0, 0, 0, 1, 0, 0, 0, 1, 1, NULL, -1, 0x80);
    u0 = uv(out[0].s.uv0);
    u1 = uv(out[0].s.uv1);
    assert(u0.u == 8 && u0.v == 40 * 16 - 8 && u1.u == 30 * 16 + 8 && u1.v == 0x400 * 16 - 8);
}

static void test_background_border_tint(void)
{
    unsigned short icon[5] = { 3, 10, 20, 50, 60 };
    unsigned char rgb[3] = { 1, 2, 3 };
    const struct DotrExIconTag *tag;

    reset();
    /* Shrunk to 3/4: width 30, height 30. Background, no image, tinted with rgb. */
    assert(lSzDuel_DrawWindow_ExIcon_Pipe(50, icon, 4, 6, 1, 0, 0, 1, 0, 0, 1, rgb, 5, 0x80) == 52);
    assert(nout == 2 && !out[0].sprite && !out[1].sprite && tex_calls == 0);
    tag = (const struct DotrExIconTag *)out[0].q.tag;
    assert(tag->count == 3 && tag->flags == 6 && tag->registers == 0x441);
    assert(out[0].q.color.r == 0 && out[0].q.color.g == 0 && out[0].q.color.b == 0 && out[0].q.color.a == 0x80);
    check_rect(&out[0].q.xyz0, &out[0].q.xyz1, 4, 6, 30, 30);
    assert(alpha_calls == 2 && alphas[1] == 0x7005);
    assert(out[1].q.color.r == 3 && out[1].q.color.g == 2 && out[1].q.color.b == 1 && out[1].q.color.a == 0x44);
    check_rect(&out[1].q.xyz0, &out[1].q.xyz1, 4, 6, 30, 30);

    reset();
    /* No rgb: grey 0x80 tint; blend 0 is still drawn. */
    lSzDuel_DrawWindow_ExIcon_Pipe(50, icon, 4, 6, 0, 0, 0, 0, 0, 0, 1, NULL, 0, 0x80);
    assert(nout == 1 && out[0].q.color.r == 0x80 && out[0].q.color.b == 0x80 && alphas[1] == 0x7000);
    check_rect(&out[0].q.xyz0, &out[0].q.xyz1, 4, 6, 40, 40);
}

static void test_border_recursion(void)
{
    unsigned short icon[5] = { 3, 10, 20, 50, 60 };
    unsigned short border[5] = { 2, 100, 200, 140, 260 };
    struct DotrExIconUv u0;

    reset();
    /* The border record at 0x2909f4 is drawn by a recursive call: image on, background, border
     * and blend off, the same shrink and bright, alpha 0x80, then this call's own tint. */
    memcpy(data_002909f4_exicon, border, sizeof(border));
    lSzDuel_DrawWindow_ExIcon_Pipe(50, icon, 4, 6, 0, 0, 1, 1, 0, 0, 0, NULL, 1, 0x20);
    assert(nout == 2 && out[0].sprite && !out[1].sprite && tex_calls == 1 && textures[0] == (void *)0x2222);
    assert(out[0].s.color.r == 0x30 && out[0].s.color.a == 0x80);
    u0 = uv(out[0].s.uv0);
    assert(u0.u == 100 * 16 + 8 && u0.v == 200 * 16 + 8);
    /* Border size 40 x 60 shrunk to 30 x 45; the tint uses this icon's 30 x 30. */
    check_rect(&out[0].s.xyz0, &out[0].s.xyz1, 4, 6, 30, 45);
    check_rect(&out[1].q.xyz0, &out[1].q.xyz1, 4, 6, 30, 30);
    assert(alpha_calls == 3 && alphas[2] == 0x7001);
}

int main(void)
{
    test_image_only();
    test_flips_and_clamps();
    test_background_border_tint();
    test_border_recursion();
    return 0;
}

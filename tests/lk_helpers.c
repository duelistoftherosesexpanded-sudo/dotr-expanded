/* Synthetic host tests for the lk_helpers group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/lk_helpers.h"

static int load_group;
static int load_id;

int SzText_LoadRequest(int group, int id)
{
    load_group = group;
    load_id = id;
    return 0x1234 + group;
}

static void check_counters(void)
{
    float f;
    unsigned char b;
    int i;

    f = 2.0f;
    assert(LkIncrFLOAT(&f, 4.0f, 1.0f) == 1 && f == 3.0f);
    assert(LkIncrFLOAT(&f, 4.0f, 1.0f) == 1 && f == 4.0f); /* boundary: value == limit - step */
    assert(LkIncrFLOAT(&f, 4.0f, 1.0f) == 0 && f == 4.0f);
    f = 3.5f;
    assert(LkIncrFLOAT(&f, 4.0f, 1.0f) == 0 && f == 4.0f);

    b = 12;
    assert(LkDecrBYTE(&b, 5, 4) == 1 && b == 8);
    b = 9;
    assert(LkDecrBYTE(&b, 5, 4) == 1 && b == 5); /* boundary: value == limit + step */
    b = 8;
    assert(LkDecrBYTE(&b, 5, 4) == 0 && b == 5);
    b = 200;
    assert(LkDecrBYTE(&b, 250, 250) == 0 && b == 250); /* the sum is not truncated to a byte */

    i = 12;
    assert(LkDecrINT(&i, 5, 4) == 1 && i == 8);
    i = 9;
    assert(LkDecrINT(&i, 5, 4) == 1 && i == 5);
    i = 8;
    assert(LkDecrINT(&i, 5, 4) == 0 && i == 5);

    b = 2;
    assert(LkIncrBYTE(&b, 10, 4) == 1 && b == 6);
    assert(LkIncrBYTE(&b, 10, 4) == 1 && b == 10); /* boundary: value == limit - step */
    b = 7;
    assert(LkIncrBYTE(&b, 10, 4) == 0 && b == 10);
    b = 3;
    assert(LkIncrBYTE(&b, 2, 4) == 0 && b == 2); /* the difference is not truncated to a byte */

    i = 2;
    assert(LkIncrINT(&i, 10, 4) == 1 && i == 6);
    assert(LkIncrINT(&i, 10, 4) == 1 && i == 10);
    i = 7;
    assert(LkIncrINT(&i, 10, 4) == 0 && i == 10);

    f = 6.0f;
    assert(LkDecrFLOAT(&f, 2.0f, 1.0f) == 1 && f == 5.0f);
    f = 3.0f;
    assert(LkDecrFLOAT(&f, 2.0f, 1.0f) == 1 && f == 2.0f); /* boundary: value == limit + step */
    f = 2.5f;
    assert(LkDecrFLOAT(&f, 2.0f, 1.0f) == 0 && f == 2.0f);
}

static void check_vectors(void)
{
    float a[4] = { 1.0f, 2.0f, 3.0f, 99.0f };
    float b[4] = { 4.0f, 8.0f, 16.0f, 77.0f };

    LkAddVector((struct DotrLkVector *)a, (struct DotrLkVector *)b);
    assert(a[0] == 5.0f && a[1] == 10.0f && a[2] == 19.0f && a[3] == 99.0f);
    LkSubVector((struct DotrLkVector *)a, (struct DotrLkVector *)b);
    assert(a[0] == 1.0f && a[1] == 2.0f && a[2] == 3.0f && a[3] == 99.0f);
    LkMulVector((struct DotrLkVector *)a, (struct DotrLkVector *)b);
    assert(a[0] == 4.0f && a[1] == 16.0f && a[2] == 48.0f && a[3] == 99.0f);
    assert(b[0] == 4.0f && b[1] == 8.0f && b[2] == 16.0f && b[3] == 77.0f);
}

static void check_vector_steps(void)
{
    float v[4] = { 1.0f, 3.0f, 3.5f, 99.0f };
    float a[4] = { 1.0f, 2.0f, 3.0f, 99.0f };
    float b[4] = { 5.0f, 10.0f, 19.0f, 77.0f };
    float c[4] = { 0.0f, 0.0f, 0.0f, 55.0f };

    /* x steps (1 <= 4 - 1), y sits exactly on the boundary (3 <= 3) and steps, z is clamped. */
    assert(LkIncrVec((struct DotrLkVector *)v, 4.0f, 1.0f) == 3);
    assert(v[0] == 2.0f && v[1] == 4.0f && v[2] == 4.0f && v[3] == 99.0f);
    assert(LkIncrVec((struct DotrLkVector *)v, 4.0f, 1.0f) == 1);
    assert(v[0] == 3.0f && v[1] == 4.0f && v[2] == 4.0f);
    v[0] = 9.0f, v[1] = 9.0f, v[2] = 3.0f;
    assert(LkIncrVec((struct DotrLkVector *)v, 4.0f, 1.0f) == 4);
    assert(v[0] == 4.0f && v[1] == 4.0f && v[2] == 4.0f);

    /* x is clamped (2.5 < 2 + 1), y sits on the boundary (3 >= 3) and steps, z steps. */
    v[0] = 2.5f, v[1] = 3.0f, v[2] = 8.0f;
    assert(LkDecrVec((struct DotrLkVector *)v, 2.0f, 1.0f) == 6);
    assert(v[0] == 2.0f && v[1] == 2.0f && v[2] == 7.0f && v[3] == 99.0f);
    v[0] = 5.0f, v[1] = 0.0f, v[2] = 0.0f;
    assert(LkDecrVec((struct DotrLkVector *)v, 2.0f, 1.0f) == 1);
    assert(v[0] == 4.0f && v[1] == 2.0f && v[2] == 2.0f);

    LkGetCenter((struct DotrLkVector *)a, (struct DotrLkVector *)b, (struct DotrLkVector *)c);
    assert(c[0] == 3.0f && c[1] == 6.0f && c[2] == 11.0f && c[3] == 55.0f);
    assert(a[0] == 1.0f && a[1] == 2.0f && a[2] == 3.0f && b[0] == 5.0f && b[1] == 10.0f && b[2] == 19.0f);

    LkOBJ_TransitVector((struct DotrLkVector *)a, (struct DotrLkVector *)b, 4);
    assert(a[0] == 2.0f && a[1] == 4.0f && a[2] == 7.0f && a[3] == 99.0f);
    LkOBJ_TransitVector((struct DotrLkVector *)a, (struct DotrLkVector *)b, 0);
    assert(a[0] == 5.0f && a[1] == 10.0f && a[2] == 19.0f && a[3] == 99.0f);
    assert(b[0] == 5.0f && b[1] == 10.0f && b[2] == 19.0f && b[3] == 77.0f);
}

static void uv_expect(const struct DotrLkUvPacket *packet, int ul, int ur, int vt, int vb)
{
    const int u[4] = {ul, ur, ul, ur}, v[4] = {vt, vt, vb, vb};
    unsigned short raw[2];
    int i;

    for (i = 0; i < 4; i++) {
        memcpy(raw, &packet->vertex[i], sizeof(raw));
        /* 12.4 value in the low 14 bits; the two preset top bits are kept. */
        assert(raw[0] == (0xc000 | ((u[i] << 4) & 0x3fff)));
        assert(raw[1] == (0xc000 | ((v[i] << 4) & 0x3fff)));
        assert(packet->vertex[i].unknown_04[0] == 0xff && packet->vertex[i].unknown_04[19] == 0xff);
    }
    assert(packet->flags == 0xffff && packet->unknown_00 == 0xffff);
}

static void uv_anim(struct DotrLkUvAnim *anim, int flags, int frame)
{
    memset(anim, 0, sizeof(*anim));
    anim->w = 16;
    anim->h = 32;
    anim->u0 = 8;
    anim->v0 = 4;
    anim->sheet_w = 64;
    anim->sheet_h = 96;
    anim->flags = flags;
    anim->frame = frame;
    anim->wait_max = 2;
    anim->frame_max = 6;
    anim->loop_frames = 3;
}

static void check_assign_uv2(void)
{
    struct DotrLkUvPacket packet;
    struct DotrLkUvAnim anim;

    assert(offsetof(struct DotrLkUvPacket, vertex) == 24 && sizeof(struct DotrLkUvVertex) == 24);
    assert(offsetof(struct DotrLkUvAnim, flags) == 56 && offsetof(struct DotrLkUvAnim, wait) == 72);
    assert(offsetof(struct DotrLkUvAnim, state) == 82);

    /* Row-major sheet (flags bit 1), mode 0: upper half of the cell, bit 11 set, no stepping. */
    uv_anim(&anim, 2, 5);
    memset(&packet, 0, sizeof(packet));
    LkAssignUV2(&packet, &anim, 0);
    assert(packet.flags == 0x800);
    assert(anim.u == 8 + 16 * 1 && anim.v == 4 + 32 * 1 && anim.wait == 0 && anim.frame == 5);
    memset(&packet, 0xff, sizeof(packet));
    LkAssignUV2(&packet, &anim, 0);
    uv_expect(&packet, 24, 39, 36, 51);

    /* Column-major sheet, mode 2: lower half of the cell, no stepping. */
    uv_anim(&anim, 0, 5);
    memset(&packet, 0xff, sizeof(packet));
    LkAssignUV2(&packet, &anim, 2);
    assert(anim.u == 8 + 16 * 1 && anim.v == 4 + 32 * 2 && anim.wait == 0 && anim.frame == 5);
    uv_expect(&packet, 24, 39, 83, 99);

    /* A value past 14 bits is truncated to the field. */
    uv_anim(&anim, 0, 0);
    anim.u0 = 1024;
    memset(&packet, 0xff, sizeof(packet));
    LkAssignUV2(&packet, &anim, 2);
    uv_expect(&packet, 1024, 1039, 19, 35);

    /* Mode 1: wait below or at its limit only counts. */
    uv_anim(&anim, 0, 5);
    anim.wait = 1;
    LkAssignUV2(&packet, &anim, 1);
    assert(anim.wait == 2 && anim.frame == 5 && anim.state == 0);
    /* Past the limit: wait resets and the frame advances, still inside the range. */
    LkAssignUV2(&packet, &anim, 1);
    assert(anim.wait == 0 && anim.frame == 6 && anim.state == 0);
    /* Past the last frame: state becomes 1 and the frame stays one past the end. */
    anim.wait = 2;
    LkAssignUV2(&packet, &anim, 1);
    assert(anim.wait == 0 && anim.frame == 7 && anim.state == 1);
    /* State 999 loops back instead: last - loop_frames + 1. */
    uv_anim(&anim, 0, 6);
    anim.wait = 2;
    anim.state = 999;
    LkAssignUV2(&packet, &anim, 1);
    assert(anim.wait == 0 && anim.frame == 4 && anim.state == 999);
}

static void check_polpan_uv(void)
{
    /* corner index (0: u,v  1: u+w,v  2: u,v+h  3: u+w,v+h) held by vertex 0..3 */
    static const struct { int mode; int corner[4]; } cases[5] = {
        { 0, { 0, 1, 2, 3 } }, { 1, { 2, 0, 3, 1 } }, { 10, { 1, 3, 0, 2 } },
        { 2, { 1, 0, 3, 2 } }, { -1, { 1, 0, 3, 2 } },
    };
    short rect[4] = { 0x21, 0x32, 0x40, 0x18 };
    struct DotrLkUvPacket packet;
    int i, k;

    for (i = 0; i < 5; i++) {
        memset(&packet, 0xff, sizeof(packet));
        POLPAN_UVIN_(&packet, rect, cases[i].mode);
        assert(packet.unknown_00 == 0xffff && packet.flags == 0xffff);
        for (k = 0; k < 4; k++) {
            int c = cases[i].corner[k];
            int u = rect[0] + ((c & 1) ? rect[2] : 0);
            int v = rect[1] + ((c & 2) ? rect[3] : 0);

            assert(packet.vertex[k].u == u << 4 && packet.vertex[k].v == v << 4);
            assert(packet.vertex[k].u_high == 3 && packet.vertex[k].v_high == 3);
            assert(packet.vertex[k].unknown_04[0] == 0xff && packet.vertex[k].unknown_04[19] == 0xff);
        }
    }
    /* the field keeps 14 bits of the shifted value */
    rect[0] = 0x3ff; rect[2] = 2;
    memset(&packet, 0, sizeof(packet));
    POLPAN_UVIN_(&packet, rect, 0);
    assert(packet.vertex[0].u == 0x3ff0 && packet.vertex[1].u == 0x0010 && packet.vertex[1].u_high == 0);
    assert(rect[0] == 0x3ff && rect[1] == 0x32 && rect[2] == 2 && rect[3] == 0x18);
}

int main(void)
{
    struct DotrLkCoordinate coordinate[2];
    float one = 1.0f;
    int handle;
    int i;

    assert(sizeof(struct DotrLkVector) == 12);
    assert(sizeof(struct DotrLkCoordinate) == 0x30);

    handle = 0;
    assert(LkInitTex2(&handle, 77) == 0);
    assert(handle == 0x1238 && load_group == 4 && load_id == 77);
    assert(LkInitTexBtl(&handle, 78) == 0);
    assert(handle == 0x1239 && load_group == 5 && load_id == 78);

    check_counters();
    check_vectors();
    check_vector_steps();

    memset(coordinate, 0x55, sizeof(coordinate));
    LkInitCoordinate(&coordinate[0]);
    for (i = 0; i < 4; i++) {
        assert(memcmp(&coordinate[0].rows[0][i], &one, 4) == 0);
        assert(coordinate[0].rows[1][i] == 0 && coordinate[0].rows[2][i] == 0);
        assert(coordinate[1].rows[0][i] == 0x55555555);
    }

    assert(LkCheckFlag(0x10, 4) == 1);
    assert(LkCheckFlag(0x10, 3) == 0);
    assert(LkCheckFlag((int)0x80000000u, 31) == 1);
    assert(LkCheckFlag(0, 0) == 0);

    check_assign_uv2();
    check_polpan_uv();
    puts("lk_helpers: ok");
    return 0;
}

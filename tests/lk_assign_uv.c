/* Synthetic host tests for the lk_assign_uv group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/lk_assign_uv.h"

static struct DotrLkAssignSprite sprite;

/* Expected corner per vertex: 0 left/top, 1 right/bottom, per orientation (x then y). */
static const unsigned char corners[8][4][2] = {
    { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
    { { 0, 1 }, { 0, 0 }, { 1, 1 }, { 1, 0 } },
    { { 0, 1 }, { 1, 1 }, { 0, 0 }, { 1, 0 } },
    { { 1, 0 }, { 0, 0 }, { 1, 1 }, { 0, 1 } },
    { { 1, 0 }, { 1, 1 }, { 0, 0 }, { 0, 1 } },
    { { 1, 1 }, { 0, 1 }, { 1, 0 }, { 0, 0 } },
    { { 1, 1 }, { 1, 0 }, { 0, 1 }, { 0, 0 } },
};

static int rand_values[8], rand_next, rand_calls;

/* Stub: returns the queued values in order. */
int func_0019a5c8(void)
{
    rand_calls++;
    return rand_values[rand_next++];
}

static void check_uvst(void)
{
    struct DotrLkAssignStSprite sp;
    struct DotrLkAssignAnim an;

    /* integer texels, frames along rows: 4 cells per row, frame 5 -> column 1, row 1 */
    memset(&sp, 0, sizeof(sp));
    memset(&an, 0, sizeof(an));
    sp.flags = 3;
    an.flags = 1 | 2;
    an.r.i.w = 16; an.r.i.h = 8; an.r.i.baseU = 2; an.r.i.baseV = 4; an.r.i.texW = 64; an.r.i.texH = 32;
    an.frame = 5; an.timer = 0; an.delay = 3; an.last = 9;
    LkAssignUVST(&sp, &an);
    assert(an.r.i.u == 18 && an.r.i.v == 12 && sp.flags == 0x803);
    assert(sp.v[0].tex.uv.x.val == 18 << 4 && sp.v[0].tex.uv.y.val == 12 << 4);
    assert(sp.v[1].tex.uv.x.val == 34 << 4 && sp.v[1].tex.uv.y.val == 12 << 4);
    assert(sp.v[2].tex.uv.x.val == 18 << 4 && sp.v[2].tex.uv.y.val == 20 << 4);
    assert(sp.v[3].tex.uv.x.val == 34 << 4 && sp.v[3].tex.uv.y.val == 20 << 4);
    assert(an.timer == 1 && an.frame == 5);          /* timer below the delay: no step */
    /* columns: 4 cells per column (texH / h), frame 5 -> column 1, row 1 */
    an.flags = 1;
    an.frame = 6;
    LkAssignUVST(&sp, &an);
    assert(an.r.i.u == 2 + 16 * 1 && an.r.i.v == 4 + 8 * 2);
    /* float units, rows: 1 / 0.25 = 4 per row; frame 5 -> (0.25, 0.5) */
    memset(&sp, 0, sizeof(sp));
    memset(&an, 0, sizeof(an));
    an.flags = 2;
    an.r.f.w = 0.25f; an.r.f.h = 0.5f;
    an.frame = 5; an.delay = 0; an.last = 9; an.loop = 999; an.loopLength = 3;
    LkAssignUVST(&sp, &an);
    assert(an.r.f.x == 0.25f && an.r.f.y == 0.5f && sp.flags == 0);
    assert(sp.v[0].tex.st.s == 0.25f && sp.v[0].tex.st.t == 0.5f);
    assert(sp.v[1].tex.st.s == 0.5f && sp.v[1].tex.st.t == 0.5f);
    assert(sp.v[2].tex.st.s == 0.25f && sp.v[2].tex.st.t == 1.0f);
    assert(sp.v[3].tex.st.s == 0.5f && sp.v[3].tex.st.t == 1.0f);
    assert(an.timer == 0 && an.frame == 6);          /* timer passed the delay: frame steps */
    /* float units, columns: 1 / 0.5 = 2 per column; frame 5 -> (0.5, 0.5) */
    an.flags = 0;
    an.frame = 5;
    LkAssignUVST(&sp, &an);
    assert(an.r.f.x == 0.5f && an.r.f.y == 0.5f);
    /* past the last frame: loop 999 rewinds to last - loopLength + 1, else loop is set to 1 */
    an.frame = 9; an.timer = 0;
    LkAssignUVST(&sp, &an);
    assert(an.frame == 9 - 3 + 1 && an.loop == 999);
    an.frame = 9; an.timer = 0; an.loop = 0;
    LkAssignUVST(&sp, &an);
    assert(an.frame == 10 && an.loop == 1);
    an.frame = 8; an.timer = 0; an.loop = 0;
    LkAssignUVST(&sp, &an);
    assert(an.frame == 9 && an.loop == 0);           /* reaching last itself does not wrap */
}

static void check_uvst3(void)
{
    static unsigned char packet[0x70];
    static struct DotrLkAssignState3 st;
    struct DotrLkAssignUv3 *uv;

    /* integer texels, rows, instance 3: 4 cells per row, index 6 -> column 2, row 1 */
    memset(packet, 0, sizeof(packet));
    memset(&st, 0, sizeof(st));
    st.flags = 1 | 2;
    st.uv.fixed.width = 16; st.uv.fixed.height = 8; st.uv.fixed.baseU = 1; st.uv.fixed.baseV = 2;
    st.uv.fixed.imageWidth = 64; st.uv.fixed.imageHeight = 32;
    st.frames[3].index = 6; st.delay = 2; st.end = 7; st.rewind = 4;
    LkAssignUVST3(packet, &st, 3);
    assert(st.uv.fixed.u == 33 && st.uv.fixed.v == 10 && *(unsigned short *)(packet + 2) == 0x800);
    uv = (struct DotrLkAssignUv3 *)(packet + 0x18);
    assert(uv->u == 33 << 4 && uv->v == 10 << 4);
    uv = (struct DotrLkAssignUv3 *)(packet + 0x30);
    assert(uv->u == (33 + 15) << 4 && uv->v == 10 << 4);
    uv = (struct DotrLkAssignUv3 *)(packet + 0x48);
    assert(uv->u == 33 << 4 && uv->v == (10 + 7) << 4);
    uv = (struct DotrLkAssignUv3 *)(packet + 0x60);
    assert(uv->u == (33 + 15) << 4 && uv->v == (10 + 7) << 4);
    assert(st.frames[3].timer == 1 && st.frames[3].index == 6 && st.frames[2].timer == 0);
    /* columns: imageHeight / height = 4; index 6 -> column 1, row 2 */
    st.flags = 1;
    LkAssignUVST3(packet, &st, 3);
    assert(st.uv.fixed.u == 1 + 16 && st.uv.fixed.v == 2 + 16);
    /* float, rows then columns */
    memset(packet, 0, sizeof(packet));
    memset(&st, 0, sizeof(st));
    st.flags = 2;
    st.uv.floating.width = 0.25f; st.uv.floating.height = 0.5f;
    st.frames[0].index = 5; st.delay = 5; st.end = 9;
    LkAssignUVST3(packet, &st, 0);
    assert(st.uv.floating.u == 0.25f && st.uv.floating.v == 0.5f && *(unsigned short *)(packet + 2) == 0);
    assert(*(float *)(packet + 0x18) == 0.25f && *(float *)(packet + 0x1c) == 0.5f);
    assert(*(float *)(packet + 0x30) == 0.5f && *(float *)(packet + 0x34) == 0.5f);
    assert(*(float *)(packet + 0x48) == 0.25f && *(float *)(packet + 0x4c) == 1.0f);
    assert(*(float *)(packet + 0x60) == 0.5f && *(float *)(packet + 0x64) == 1.0f);
    st.flags = 0;
    LkAssignUVST3(packet, &st, 0);
    assert(st.uv.floating.u == 0.5f && st.uv.floating.v == 0.5f);
    /* step past the end without flag 4: rewind only, no random numbers */
    st.frames[0].index = 9; st.frames[0].timer = 5; st.rewind = 4; st.frames[0].x = 7.0f;
    rand_next = rand_calls = 0;
    LkAssignUVST3(packet, &st, 0);
    assert(st.frames[0].timer == 0 && st.frames[0].index == 6 && st.frames[0].x == 7.0f && rand_calls == 0);
    /* with flag 4: restart at the base plus rand % (2 * range) - range */
    st.flags = 4; st.x = 100.0f; st.y = 50.0f; st.randomX = 10; st.randomY = 0;
    st.frames[0].index = 9; st.frames[0].timer = 5;
    rand_values[0] = 33; rand_next = rand_calls = 0;
    LkAssignUVST3(packet, &st, 0);
    assert(st.frames[0].x == 100.0f + (33 % 20 - 10) && st.frames[0].y == 50.0f && rand_calls == 1);
    assert(st.frames[0].index == 6);
    /* a negative y range: the symmetric offset, then replaced by rand % randomY */
    st.randomX = 0; st.randomY = -6;
    st.frames[0].index = 10; st.frames[0].timer = 5;
    rand_values[0] = 17; rand_values[1] = 23; rand_next = rand_calls = 0;
    LkAssignUVST3(packet, &st, 0);
    assert(st.frames[0].x == 100.0f && st.frames[0].y == 50.0f + 23 % -6 && rand_calls == 2);
    assert(st.frames[0].index == 7);
    /* index reaching end itself does not restart */
    st.frames[0].index = 8; st.frames[0].timer = 5; rand_next = rand_calls = 0;
    LkAssignUVST3(packet, &st, 0);
    assert(st.frames[0].index == 9 && rand_calls == 0);
}

int main(void)
{
    struct DotrLkAssignRect rect = { 10, 20, 30, 40 };
    int mode, i;

    for (mode = 0; mode < 8; mode++) {
        memset(&sprite, 0, sizeof(sprite));
        sprite.flags = 0x0003;
        sprite.v[0].x.hi = 2;
        assert(LkAssignUV(&sprite, &rect, mode) == 0);
        assert(sprite.flags == 0x0803);
        for (i = 0; i < 4; i++) {
            int x = corners[mode][i][0] ? 10 + 30 : 10;
            int y = corners[mode][i][1] ? 20 + 40 : 20;

            assert(sprite.v[i].x.val == x << 4 && sprite.v[i].y.val == y << 4);
        }
        /* The two top bits of each halfword are kept. */
        assert(sprite.v[0].x.hi == 2 && sprite.v[0].y.hi == 0);
    }
    /* Any other orientation only sets the flag. */
    memset(&sprite, 0, sizeof(sprite));
    assert(LkAssignUV(&sprite, &rect, 8) == 0 && sprite.flags == 0x800 && sprite.v[3].y.val == 0);
    assert(LkAssignUV(&sprite, &rect, -1) == 0 && sprite.v[0].x.val == 0);
    /* Coordinates wrap to 14 bits. */
    rect.x = 1000;
    rect.w = 100;
    LkAssignUV(&sprite, &rect, 0);
    assert(sprite.v[1].x.val == ((1100 << 4) & 0x3fff) && sprite.v[0].x.val == ((1000 << 4) & 0x3fff));
    check_uvst();
    check_uvst3();
    return 0;
}

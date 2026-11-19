/* Synthetic host tests for the battle_fakes group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/battle_fakes.h"

struct DotrFakeMirrorQuad data_00292310;
struct DotrFakeMirrorVec3 data_00292350;
short data_00292360[4];
short data_00292368[4];
short data_00292370;
short data_00292378;
int gSzBattle_bl[3];

#define RND_TRUE 0x40000000 /* 3 * (value / 2^31) = 1.5, which the conversion stub truncates to 1 */

static char trace[192];
static int trace_count, inner_calls, push_result, frame_count, pause_flag, rand_calls, rand_script[8], 
    dither_args[2], dither_calls, mask_calls, zbuf_args[4], zbuf_calls, draw_index[8], draw_calls, unit_args[2][2],
    unit_calls, disp_result, alpha_script, rgba_set_value[4], rgba_set_calls, only_calls, intensity_calls, sort_calls,
    modify_calls;
static unsigned int mask_args[2];
static float inner_result, scalar_result, only_value[4];
static DotrDrawWord next_cursor;
static struct DotrFakeUnit *unit_script[2];
static struct DotrFakeUnit units[2];
static unsigned char *rgba_seen, *rgba_set_in[4];
static struct DotrTitleIntensity intensity_set[2];
static struct DotrFakePacket sorted[5];
static char view_origin[64];

static void note(char c)
{
    trace[trace_count++] = c;
    trace[trace_count] = 0;
}
/* Every cursor-taking callee must receive the previous one's result. */
static DotrDrawWord chain(DotrDrawWord cursor, char c)
{
    note(c);
    assert(cursor == next_cursor);
    return ++next_cursor;
}

int Ex3d_GetPointsAverage(int count, struct DotrViewVector *items, struct DotrViewVector *out)
{
    note('A');
    assert(count == 4 && items[0].x == 101.0f && items[3].unknown_w == 116.0f);
    out->x = 11.0f;
    return 0;
}
int Ex3d_GetSurfaceNormal(int n, struct DotrViewVector *pts, struct DotrViewVector *out)
{
    note('N');
    assert(n == 4 && pts[1].y == 106.0f);
    out->x = 22.0f;
    return 0;
}
void *SzView_GetSP(int index)
{
    note('S');
    assert(index == 0);
    return view_origin;
}
int Ex3d_GetViewVector(struct DotrViewVector *origin, int count, struct DotrViewVector *points, struct DotrViewVector *out)
{
    note('V');
    assert((char *)origin == view_origin && count == 4 && points[2].z == 111.0f);
    out->x = 33.0f;
    return 0;
}
float RhGeom_InnerProduct(struct DotrViewVector *a, struct DotrViewVector *b)
{
    note('I');
    assert(a->x == 33.0f && b->x == 22.0f);
    inner_calls++;
    return inner_result;
}
int RhView_PushFakeMirror(struct DotrViewVector *pos, struct DotrViewVector *normal, struct DotrViewBlock *out)
{
    note('P');
    assert(pos->x == 11.0f && normal->x == 22.0f);
    memset(out, 0x5a, sizeof *out);
    return push_result;
}
int RhView_PopFakeMirror(struct DotrViewBlock *out)
{
    note('p');
    memset(out, 0x6b, sizeof *out);
    return 0;
}
int SzGame_GetFrameCount(void) { note('F'); return frame_count; }
float RhGeom_GetVectorScalor(struct DotrViewVector *v)
{
    note('L');
    assert((char *)v == view_origin);
    return scalar_result;
}
int func_0019a5c8(void) { note('r'); return rand_script[rand_calls++]; }
int SzModel_GetPauseFlag(int index) { note('U'); assert(index == 8); return pause_flag; }
DotrDrawWord ExDitherMode_Pipe(DotrDrawWord cursor, int value)
{
    dither_args[dither_calls++] = value;
    return chain(cursor, 'D');
}
int RhBase_AssignRenderFrame(int index, int mode, int kind)
{
    note('a');
    assert(index == 0 && mode == 10 && kind == 49);
    return 0;
}
DotrDrawWord RhBase_EnableRenderFrame(DotrDrawWord p) { return chain(p, 'E'); }
DotrDrawWord RhBase_ClearRenderFrame(DotrDrawWord p, void *color)
{
    assert(color == 0);
    return chain(p, 'C');
}
DotrDrawWord RhBase_DisableRenderFrame(DotrDrawWord p) { return chain(p, 'e'); }
DotrDrawWord RhText_SetTextureFlush(DotrDrawWord cursor) { return chain(cursor, 'f'); }
void RhBase_FreeRenderFrame(void) { note('R'); }
int RhView_Modify(struct DotrViewBlock *block)
{
    note('M');
    /* First the mirrored view from RhView_PushFakeMirror, then the one RhView_PopFakeMirror restored. */
    assert(*(unsigned char *)block == (modify_calls == 0 ? 0x5a : 0x6b));
    modify_calls++;
    return 0;
}
struct DotrDrawTag *RhView_SendViewParameters(struct DotrDrawTag *tag)
{
    return (struct DotrDrawTag *)chain((DotrDrawWord)tag, 'W');
}
DotrDrawWord RhBase_MaskFBuffer(DotrDrawWord cursor, unsigned int mask)
{
    mask_args[mask_calls++] = mask;
    return chain(cursor, 'K');
}
DotrDrawWord RhBase_SetZBufferMode(DotrDrawWord cursor, int a)
{
    zbuf_args[zbuf_calls++] = a;
    return chain(cursor, 'Z');
}
DotrDrawWord SzModel_DrawWithOutCalc_Pipe(DotrDrawWord cursor, int index, int c, void *d, int e, int f, int g)
{
    assert(c == 0 && d == 0 && e == 0 && f == 0 && g == 0);
    draw_index[draw_calls++] = index;
    return chain(cursor, 'd');
}
struct DotrFakeUnit *SzDuel_GetUnit(int a, int b)
{
    note('u');
    unit_args[unit_calls][0] = a;
    unit_args[unit_calls][1] = b;
    return unit_script[unit_calls++];
}
int SzModel_IsModelDisp(int index)
{
    note('i');
    assert(index == units[unit_calls - 1].unknown_46);
    return disp_result;
}
int SzModel_GetRgba(int index, unsigned char *out)
{
    note('g');
    assert(index == units[unit_calls - 1].unknown_46);
    out[0] = 1;
    out[1] = 2;
    out[2] = 3;
    out[3] = (unsigned char)alpha_script;
    rgba_seen = out;
    return 77;
}
int SzModel_SetRgba(int index, int value, unsigned char *in)
{
    note('s');
    assert(index == units[unit_calls - 1].unknown_46);
    /* The restoring call passes the bytes SzModel_GetRgba wrote. */
    assert(in == 0 || (in[0] == 1 && in[1] == 2 && in[2] == 3 && in[3] == (unsigned char)alpha_script));
    rgba_set_value[rgba_set_calls] = value;
    rgba_set_in[rgba_set_calls++] = in;
    return 0;
}
int SzModel_GetIntensityOnly(int index, float *out)
{
    note('o');
    assert(index == units[unit_calls - 1].unknown_46);
    *out = 0.75f;
    return 0;
}
int SzModel_SetIntensityOnly(int index, float value)
{
    note('O');
    assert(index == units[unit_calls - 1].unknown_46);
    only_value[only_calls++] = value;
    return 0;
}
int SzModel_GetIntensity(int index, struct DotrTitleIntensity *out)
{
    note('G');
    assert(index == 16);
    out->x = 1.0f;
    out->y = 2.0f;
    out->z = 3.0f;
    return 0;
}
int SzModel_SetIntensity(int index, struct DotrTitleIntensity *in)
{
    note('T');
    assert(index == 16);
    intensity_set[intensity_calls++] = *in;
    return 0;
}
int ExAssignAlpha(struct DotrCursorPacketHeader *packet, long value)
{
    note('x');
    assert(packet->unknown_00 == 5 && packet->unknown_02 == (38 | 0x800) && packet->unknown_04 == 0 &&
           packet->unknown_08 == 0x42421);
    assert(value == 0x8000000048);
    return 0;
}
int RhBase_GetTextureInfoForRender(struct DotrCursorTex *a, struct DotrCursorTex *b)
{
    note('t');
    assert(b == a + 1);
    a->word = -1;
    b->word = 0x2222;
    return 0;
}
DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, void *packet, int a1, int a2)
{
    assert(a1 == 0 && a2 == 0);
    memcpy(&sorted[sort_calls++], packet, sizeof sorted[0]);
    return chain(cursor, 'X');
}

/* Host forwarders: the functions call their callees through views with their own prototypes
 * (battle_fakes.h); each forwards to the stub of the real name above. Their float-to-int casts
 * are plain host conversions (no func_001a22c0 call on the host). */
int func_00206030_fakes(int count, void *a, void *b) { return Ex3d_GetPointsAverage(count, a, b); }
int func_00205e70_fakes(int count, void *a, float *b) { return Ex3d_GetSurfaceNormal(count, a, (struct DotrViewVector *)b); }
int func_002060f0_fakes(float *o, int count, float *p, float *out)
{
    return Ex3d_GetViewVector((struct DotrViewVector *)o, count, (struct DotrViewVector *)p, (struct DotrViewVector *)out);
}
DotrDrawWord func_001f9ad0_fakes(int i) { return (DotrDrawWord)SzView_GetSP(i); }
float func_0020d1a0_fakes(float *a, float *b) { return RhGeom_InnerProduct((struct DotrViewVector *)a, (struct DotrViewVector *)b); }
int func_0020e520_fakes(float *p, float *n, void *out)
{
    return RhView_PushFakeMirror((struct DotrViewVector *)p, (struct DotrViewVector *)n, out);
}
int func_001a8720_fakes(void) { return SzGame_GetFrameCount(); }
float func_0020d0e0_fakes(float *a) { return RhGeom_GetVectorScalor((struct DotrViewVector *)a); }
int func_001f3360_fakes(int i) { return SzModel_GetPauseFlag(i); }
void *func_00208530_fakes(void *c, int v) { return (void *)ExDitherMode_Pipe((DotrDrawWord)c, v); }
void *func_0020a610_fakes(void *c) { return (void *)RhBase_EnableRenderFrame((DotrDrawWord)c); }
void *func_0020a870_fakes(unsigned char *c, unsigned char *colour) { return (void *)RhBase_ClearRenderFrame((DotrDrawWord)c, colour); }
int func_0020e360_fakes(void *block) { return RhView_Modify(block); }
int func_0020ec60_fakes(int c) { return (int)(DotrDrawWord)RhView_SendViewParameters((struct DotrDrawTag *)(DotrDrawWord)c); }
void *func_00209f70_fakes(void *c, unsigned int mask) { return (void *)RhBase_MaskFBuffer((DotrDrawWord)c, mask); }
int func_0020a0b0_fakes(int c, int a) { return (int)RhBase_SetZBufferMode(c, a); }
int func_001efa80_fakes(int c, int index, int a2, int a3, int a4, int a5, int a6)
{
    return (int)SzModel_DrawWithOutCalc_Pipe(c, index, a2, (void *)(DotrDrawWord)a3, a4, a5, a6);
}
DotrDrawWord func_001df540_fakes(int a, int b) { return (DotrDrawWord)SzDuel_GetUnit(a, b); }
int func_001f1370_fakes(int i) { return SzModel_IsModelDisp(i); }
int func_001f2660_fakes(int i, void *out) { return SzModel_GetRgba(i, out); }
int func_001f26e0_fakes(int i, int v, void *in) { return SzModel_SetRgba(i, v, in); }
int func_001f1e80_fakes(int i, float *out) { return SzModel_GetIntensityOnly(i, out); }
int func_001f1f70_fakes(int i, float v) { return SzModel_SetIntensityOnly(i, v); }
int func_001f1e10_fakes(int i, float *out) { return SzModel_GetIntensity(i, (struct DotrTitleIntensity *)out); }
int func_001f1ee0_fakes(int i, float *in) { return SzModel_SetIntensity(i, (struct DotrTitleIntensity *)in); }
int func_002086d0_fakes(void *packet, long long v) { return ExAssignAlpha(packet, v); }
int func_0020aa30_fakes(void *a, void *b) { return RhBase_GetTextureInfoForRender(a, b); }
void *func_0020a760_fakes(void *c) { return (void *)RhBase_DisableRenderFrame((DotrDrawWord)c); }
int func_0020f290_fakes(int c) { return (int)RhText_SetTextureFlush(c); }
int func_0020e970_fakes(void *out) { return RhView_PopFakeMirror(out); }
void func_0020a9e0_fakes(void) { RhBase_FreeRenderFrame(); }
int func_0020a470_fakes(int index, int mode, int kind) { return RhBase_AssignRenderFrame(index, mode, kind); }
int func_00207650_fakes(int c, void *packet, int a1, int a2) { return (int)ExSortPrim_Pipe(c, packet, a1, a2); }

static void reset(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        ((float *)&data_00292310)[i] = 101.0f + (float)i;
    }
    data_00292350.x = 0.25f;
    data_00292350.y = 0.5f;
    data_00292350.z = 0.125f;
    trace_count = inner_calls = rand_calls = dither_calls = mask_calls = zbuf_calls = draw_calls = 0;
    unit_calls = rgba_set_calls = only_calls = intensity_calls = sort_calls = modify_calls = 0;
    trace[0] = 0;
    memset(rand_script, 0, sizeof rand_script);
    memset(sorted, 0, sizeof sorted);
    inner_result = 1.0f;
    push_result = 1;
    frame_count = 1;
    scalar_result = 700.0f;
    pause_flag = 0;
    disp_result = 1;
    alpha_script = 128;
    gSzBattle_bl[0] = -1;
    gSzBattle_bl[1] = -1;
    unit_script[0] = &units[0];
    unit_script[1] = &units[1];
    units[0].unknown_46 = 9;
    units[1].unknown_46 = 200;
    data_00292360[0] = 1;
    data_00292360[1] = 20;
    data_00292360[2] = 30;
    data_00292360[3] = 40;
    data_00292368[0] = 5;
    data_00292368[1] = 21;
    data_00292368[2] = 31;
    data_00292368[3] = 41;
    data_00292370 = 1;
    data_00292378 = 1;
    next_cursor = 1000;
}

/* Checks the parts every completed call has: the cursor chain, the fixed arguments and the
 * five sorted sprites built from the wobble history as it stands after the call. */
static void check_drawn(DotrDrawWord result)
{
    int i;

    assert(result == next_cursor);
    assert(dither_calls == 2 && dither_args[0] == 0 && dither_args[1] == 1);
    assert(mask_calls == 2 && mask_args[0] == 0xffffff && mask_args[1] == 0xff000000);
    assert(zbuf_calls == 4 && zbuf_args[0] == 0 && zbuf_args[1] == 1 && zbuf_args[2] == 0 && zbuf_args[3] == 1);
    assert(draw_index[0] == 55 && draw_index[draw_calls - 2] == 7 && draw_index[draw_calls - 1] == 16);
    assert(modify_calls == 2 && intensity_calls == 2);
    assert(intensity_set[0].x == 0.25f && intensity_set[0].y == 0.5f && intensity_set[0].z == 0.125f);
    assert(intensity_set[1].x == 1.0f && intensity_set[1].y == 2.0f && intensity_set[1].z == 3.0f);
    assert(sort_calls == 5);
    for (i = 0; i < 5; i++) {
        struct DotrFakePacket *p = &sorted[i];

        assert(p->colour[0] == 128 && p->colour[1] == 128 && p->colour[2] == 128 && p->colour[3] == (i < 4 ? 16 : 48));
        assert(p->unknown_14 == 1.0f);
        assert(p->vertex[0].xyz.z == 0xfffff6 && p->vertex[0].xyz.f == 0);
        assert(p->vertex[1].xyz.z == 0xfffff6 && p->vertex[1].xyz.f == 0);
        assert(((struct DotrCursorSlotUv *)&p->vertex[0].uv)->u == 10240 && ((struct DotrCursorSlotUv *)&p->vertex[0].uv)->v == 0);
        assert(((struct DotrCursorSlotUv *)&p->vertex[1].uv)->u == 0 && ((struct DotrCursorSlotUv *)&p->vertex[1].uv)->v == 7168);
        assert(p->tex0.flag == 0 && p->tex0.low == 0x3ffffffffUL && p->tex1.word == 0x2222);
        assert(p->vertex[0].xyz.y == 29080);
        if (i < 4) {
            assert(p->vertex[0].xyz.x == (unsigned short)(((data_00292360[i] + 1722) << 4) - 8));
            assert(p->vertex[1].xyz.x == (unsigned short)(((data_00292360[i] + 2374) << 4) - 8));
            assert(p->vertex[1].xyz.y == (unsigned short)(((data_00292368[i] + 2278) << 4) - 8));
        } else {
            assert(p->vertex[0].xyz.x == 27544 && p->vertex[1].xyz.x == 0x9458 && p->vertex[1].xyz.y == 0x8e58);
        }
    }
}

/* One even-frame call: sets the newest history entries and the directions, runs the function
 * and checks the shifted history, the number of random draws and the directions afterwards. */
static void check_wobble(float distance, int x0, int y0, int flip_x, int flip_y, int r0, int r1, int r2, int r3,
                         int want_x0, int want_y0, int want_rands, int want_flip_x, int want_flip_y)
{
    reset();
    frame_count = 2;
    scalar_result = distance;
    data_00292360[0] = (short)x0;
    data_00292368[0] = (short)y0;
    data_00292370 = (short)flip_x;
    data_00292378 = (short)flip_y;
    rand_script[0] = r0;
    rand_script[1] = r1;
    rand_script[2] = r2;
    rand_script[3] = r3;
    check_drawn(lSzBattle_DrawFakeMirror(1000, 55));
    assert(strncmp(trace, distance <= 600.0f ? "ANSVIPFSLUrr" : "ANSVIPFSLUrr", 12) == 0);
    assert(data_00292360[0] == want_x0 && data_00292360[1] == x0 && data_00292360[2] == 20 && data_00292360[3] == 30);
    assert(data_00292368[0] == want_y0 && data_00292368[1] == y0 && data_00292368[2] == 21 && data_00292368[3] == 31);
    assert(rand_calls == want_rands);
    assert(data_00292370 == want_flip_x && data_00292378 == want_flip_y);
}

/* lSzBattle_DrawFakeMirror (branch coverage). */
static void check_fake_mirror(void)
{
    /* The mirror faces away: inner product 0 is the boundary. */
    reset();
    inner_result = 0.0f;
    assert(lSzBattle_DrawFakeMirror(1000, 55) == 1000);
    assert(strcmp(trace, "ANSVI") == 0);
    /* RhView_PushFakeMirror refuses. */
    reset();
    push_result = 0;
    assert(lSzBattle_DrawFakeMirror(1000, 55) == 1000);
    assert(strcmp(trace, "ANSVIP") == 0);

    /* Odd frame, no unit in either slot: the history is left alone. */
    reset();
    check_drawn(lSzBattle_DrawFakeMirror(1000, 55));
    assert(strcmp(trace, "ANSVIPFDaECMWKZdKZdGTdTxtefpMWZXXXXXZRD") == 0);
    assert(draw_calls == 3 && rand_calls == 0 && unit_calls == 0);
    assert(data_00292360[0] == 1 && data_00292360[1] == 20 && data_00292368[0] == 5 && data_00292368[3] == 41);
    /* First slot names a unit that does not exist. */
    reset();
    gSzBattle_bl[0] = 0x0205;
    unit_script[0] = 0;
    check_drawn(lSzBattle_DrawFakeMirror(1000, 55));
    assert(strcmp(trace, "ANSVIPFDaECMWKZdKZudGTdTxtefpMWZXXXXXZRD") == 0);
    assert(unit_args[0][0] == 2 && unit_args[0][1] == 5);
    /* The unit's model is not displayed. */
    reset();
    gSzBattle_bl[0] = 0x0205;
    disp_result = 0;
    check_drawn(lSzBattle_DrawFakeMirror(1000, 55));
    assert(strcmp(trace, "ANSVIPFDaECMWKZdKZuidGTdTxtefpMWZXXXXXZRD") == 0);
    /* Alpha 128: drawn with its colour removed and restored, intensity untouched. */
    reset();
    gSzBattle_bl[0] = 0x0205;
    check_drawn(lSzBattle_DrawFakeMirror(1000, 55));
    assert(strcmp(trace, "ANSVIPFDaECMWKZdKZuigsdsdGTdTxtefpMWZXXXXXZRD") == 0);
    assert(draw_calls == 4 && draw_index[1] == 9);
    assert(rgba_set_calls == 2 && rgba_set_value[0] == 0 && rgba_set_in[0] == 0);
    assert(rgba_set_value[1] == 77 && rgba_set_in[1] == rgba_seen);
    /* Alpha 64: intensity 0.5 for the draw, then the value SzModel_GetIntensityOnly gave. */
    reset();
    gSzBattle_bl[0] = 0x0205;
    alpha_script = 64;
    check_drawn(lSzBattle_DrawFakeMirror(1000, 55));
    assert(strcmp(trace, "ANSVIPFDaECMWKZdKZuigsoOdOsdGTdTxtefpMWZXXXXXZRD") == 0);
    assert(only_calls == 2 && only_value[0] == 0.5f && only_value[1] == 0.75f);
    /* Both slots, alpha 255 (unsigned) and a model index above 127 (unsigned byte). */
    reset();
    gSzBattle_bl[0] = 0x0205;
    gSzBattle_bl[1] = 0x0103;
    alpha_script = 255;
    check_drawn(lSzBattle_DrawFakeMirror(1000, 55));
    assert(strcmp(trace, "ANSVIPFDaECMWKZdKZuigsoOdOsuigsoOdOsdGTdTxtefpMWZXXXXXZRD") == 0);
    assert(unit_args[1][0] == 1 && unit_args[1][1] == 3 && draw_index[1] == 9 && draw_index[2] == 200);
    assert(only_calls == 4 && only_value[0] == 255.0f / 128.0f && only_value[2] == 255.0f / 128.0f && only_value[3] == 0.75f);

    /* Even frame while model 8 is paused: no random draw, history unchanged. */
    reset();
    frame_count = 2;
    pause_flag = 1;
    check_drawn(lSzBattle_DrawFakeMirror(1000, 55));
    assert(strcmp(trace, "ANSVIPFSLUDaECMWKZdKZdGTdTxtefpMWZXXXXXZRD") == 0);
    assert(data_00292360[0] == 1 && data_00292360[1] == 20 && data_00292368[0] == 5 && data_00292368[1] == 21);

    /* Far view (limit 6). Steps: x by its direction when the first draw says so, y not. */
    check_wobble(700.0f, 1, 5, 1, -1, RND_TRUE, 0, 0, 0, 2, 5, 2, 1, -1);
    check_wobble(700.0f, 0, 0, -1, 1, RND_TRUE, RND_TRUE, 0, 0, -1, 1, 2, -1, 1);
    /* |x| above the limit turns x round, towards zero. */
    check_wobble(700.0f, 7, 5, 1, 1, 0, 0, 0, 0, 7, 5, 2, -1, 1);
    check_wobble(700.0f, -7, 5, -1, 1, 0, 0, 0, 0, -7, 5, 2, 1, 1);
    /* |x| at the limit but above half of it: a third draw decides. */
    check_wobble(700.0f, 6, 5, 1, 1, 0, 0, 0, 0, 6, 5, 3, -1, 1);
    check_wobble(700.0f, 6, 5, 1, 1, 0, 0, RND_TRUE, 0, 6, 5, 3, 1, 1);
    check_wobble(700.0f, -4, 5, -1, 1, 0, 0, 0, 0, -4, 5, 3, 1, 1);
    /* |x| at half the limit: no third draw. */
    check_wobble(700.0f, 3, 5, 1, 1, 0, 0, 0, 0, 3, 5, 2, 1, 1);
    /* y above twice the limit, or negative, turns y round. */
    check_wobble(700.0f, 1, 13, 1, 1, 0, 0, 0, 0, 1, 13, 2, 1, -1);
    check_wobble(700.0f, 1, -1, 1, -1, 0, 0, 0, 0, 1, -1, 2, 1, 1);
    /* y at twice the limit, and just above the limit: a draw decides. */
    check_wobble(700.0f, 1, 12, 1, 1, 0, 0, 0, 0, 1, 12, 3, 1, -1);
    check_wobble(700.0f, 1, 12, 1, 1, 0, 0, RND_TRUE, 0, 1, 12, 3, 1, 1);
    check_wobble(700.0f, 1, 7, 1, 1, 0, 0, 0, 0, 1, 7, 3, 1, -1);
    /* y at the limit: nothing. */
    check_wobble(700.0f, 1, 6, 1, 1, 0, 0, 0, 0, 1, 6, 2, 1, 1);
    /* Both third draws in one call. */
    check_wobble(700.0f, 6, 7, 1, 1, 0, 0, RND_TRUE, 0, 6, 7, 4, 1, -1);
    /* Distance 600 (boundary): limit 6 + 1, so x = 7 no longer turns without a draw. */
    check_wobble(600.0f, 7, 5, 1, 1, 0, 0, RND_TRUE, 0, 7, 5, 3, 1, 1);
    /* Distance 100: 6 + 6; distance 50: the addition is capped at 6. x = 6 is half the limit. */
    check_wobble(100.0f, 6, 5, 1, 1, 0, 0, 0, 0, 6, 5, 2, 1, 1);
    check_wobble(50.0f, 6, 5, 1, 1, 0, 0, 0, 0, 6, 5, 2, 1, 1);
    check_wobble(50.0f, 13, 5, 1, 1, 0, 0, 0, 0, 13, 5, 2, -1, 1);
}

int main(void)
{
    check_fake_mirror();
    return 0;
}

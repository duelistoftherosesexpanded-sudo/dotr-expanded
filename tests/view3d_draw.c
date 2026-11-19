/* Synthetic host tests for the view3d_draw group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/view3d_draw.h"

struct DotrView3dVector data_00291460;
struct DotrView3dAngles data_00291470;
struct DotrView3dVector data_00291480;
struct DotrView3dLight gSzDuel_PointLight;
struct DotrView3dControl data_00317490;

static int disp, centre_ok, ofs_ok;
static struct DotrView3dVector centre_value, cp_value, sp_value, set_cp, set_pos, set_look;
static int look_calls, radius_calls, set_cp_calls, set_pos_calls, set_look_calls, light_calls;
static int draw_calls, fade_calls, final_calls;
static int draw_index[160], draw_angle[160], draw_has_rot[160];
static float draw_y[160], fade_y[16];

int SzModel_IsModelDisp(int index) { assert(index == 29); return disp; }
static struct DotrView3dVector look_value;
void *SzView_GetLook(int index) { assert(index == 0); look_calls++; return &look_value; }

/* Stubs for SzDuel_Act3DView. */
static int pad_pop, pad_mask, pad_analog, pad_press, analog_value[4], press_value[16];
static int se_calls, play_calls, pop_calls, wrap_calls, analog_calls;
int SzModel_ReqSE(int channel, int index) { assert(channel == 2 && index == 29); return se_calls++; }
int SzPad_Check(int index, int mode, int mask)
{
    assert(index == 0);
    if (mode == 0) {
        assert(mask == 32);
        return pad_pop;
    }
    assert(mode == 1);
    return pad_mask & mask;
}
int SzSound_PlaySE(int index, int value) { assert(index == 0 && value == 5); return play_calls++; }
void SzGame_Pop(void) { pop_calls++; }
int SzPad_IsAnalog(int index) { assert(index == 0); return pad_analog; }
int SzPad_GetAnalogInfo(int index, int slot)
{
    assert(index == 0 && slot >= 0 && slot < 4);
    analog_calls++;
    return analog_value[slot] | 0x100;
}
int SzPad_IsPress(int index) { assert(index == 0); return pad_press; }
int SzPad_GetPressInfo(int index, int slot)
{
    assert(index == 0 && slot >= 0 && slot < 16);
    return press_value[slot];
}
float func_001a6c30(float a, float b)
{
    assert(b > 6.28f && b < 6.29f);
    wrap_calls++;
    while (a >= b) {
        a -= b;
    }
    return a;
}
float SzModel_GetRadius(int index) { assert(index == 29); radius_calls++; return 3.0f; }
int SzModel_GetCenter(int index, struct DotrView3dVector *out)
{
    assert(index == 29);
    *out = centre_value;
    return centre_ok;
}
void *SzView_GetCP(int index) { assert(index == 0); return &cp_value; }
int SzView_SetCP(int index, struct DotrView3dVector *in)
{
    assert(index == 0);
    set_cp = *in;
    set_cp_calls++;
    return 1;
}
int SzModel_GetCenterOfs(int index, struct DotrView3dVector *out)
{
    assert(index == 29);
    *out = centre_value;
    return ofs_ok;
}
int SzModel_GetModelNo(int index) { assert(index == 29); return 7; }
int SzDuel_Xax2New(int value) { return value + 10; }
int SzDuel_GetCamDist(int index) { return index * 2; }
int SzModel_SetPos(int index, struct DotrView3dVector *in)
{
    assert(index == 29);
    set_pos = *in;
    set_pos_calls++;
    return 1;
}
int SzView_SetLookSP(int index, struct DotrView3dVector *in)
{
    assert(index == 0);
    set_look = *in;
    set_look_calls++;
    return 1;
}
void *SzView_GetSP(int index) { assert(index == 0); return &sp_value; }
int SzView_SetPointLight(int index, int position, struct DotrView3dLight *in)
{
    assert(index == 0 && position == 4 && in == &gSzDuel_PointLight);
    light_calls++;
    return 1;
}
int SzModel_DrawWithOutCalc_Pipe(int cursor, int index, struct DotrView3dVector *c, void *d,
                                 int e, int f, int g)
{
    assert(cursor == 1000 + draw_calls + fade_calls);
    assert(e == 0 && f == 0 && g == 0);
    assert(c->x == 1.0f && c->z == 3.0f && c->w == 4.0f);
    draw_index[draw_calls] = index;
    draw_y[draw_calls] = c->y;
    draw_has_rot[draw_calls] = d != 0;
    if (d != 0) {
        struct DotrView3dAngles *rot = d;
        assert(rot->x == 5 && rot->z == 7 && rot->w == 8);
        draw_angle[draw_calls] = rot->y;
    }
    draw_calls++;
    return cursor + 1;
}
int lSzDuel_DrawFadeBoard_Pipe(int a, struct DotrView3dVector *centre, int alpha, float dx,
                               float dz)
{
    assert(a == 1000 + draw_calls + fade_calls);
    assert(alpha == 16 && dx == 1500.0f && dz == 1500.0f);
    fade_y[fade_calls++] = centre->y;
    return a + 1;
}
int SzModel_Draw_Pipe(int cursor, int index, int c, void *d, int e, int f, int g)
{
    assert(cursor == 1000 + draw_calls + fade_calls);
    assert(index == 29 && c == 0 && d == 0 && e == 0 && f == 0 && g == 0);
    final_calls++;
    return cursor + 5000;
}

static void reset(void)
{
    static const struct DotrView3dVector pos = { 1.0f, 2.0f, 3.0f, 4.0f };
    static const struct DotrView3dAngles rot = { 5, 6, 7, 8 };
    static const struct DotrView3dVector look = { 9.0f, 10.0f, 11.0f, 12.0f };

    data_00291460 = pos;
    data_00291470 = rot;
    data_00291480 = look;
    memset(&gSzDuel_PointLight, 0, sizeof(gSzDuel_PointLight));
    memset(&data_00317490, 0, sizeof(data_00317490));
    disp = centre_ok = ofs_ok = 0;
    look_calls = radius_calls = set_cp_calls = set_pos_calls = set_look_calls = 0;
    light_calls = draw_calls = fade_calls = final_calls = 0;
}

/* The draw sequence is the same for every state of model 29. */
static void check_draws(int result)
{
    int i, j, n = 0;

    assert(result == 1000 + 151 + 10 + 5000);
    assert(draw_calls == 151 && fade_calls == 10 && final_calls == 1);
    for (i = 5; i >= -5; i--, n++) {
        assert(draw_index[n] == 38 && draw_y[n] == (float)(i * 500) && !draw_has_rot[n]);
    }
    for (i = 5; i >= -5; i--) {
        for (j = 0; j < 12; j++, n++) {
            assert(draw_index[n] == 36 && draw_y[n] == (float)(i * 500));
            assert(draw_has_rot[n] && draw_angle[n] == j * 0x10000 / 12);
        }
    }
    for (j = 0; j < 6; j++, n++) {
        assert(draw_index[n] == 37 && draw_y[n] == 0.0f);
        assert(draw_has_rot[n] && draw_angle[n] == j * 0x10000 / 6);
    }
    assert(draw_index[n] == 34 && draw_y[n] == -500.0f && !draw_has_rot[n]);
    n++;
    assert(draw_index[n] == 35 && draw_y[n] == -500.0f && !draw_has_rot[n]);
    for (i = 0; i < 5; i++) {
        assert(fade_y[i] == (float)(2500 - i * 100));
        assert(fade_y[5 + i] == (float)(-2500 + i * 100));
    }
    /* The originals in .data are copied, not written. */
    assert(data_00291460.y == 2.0f && data_00291470.y == 6 && data_00291480.x == 9.0f);
}

static float follow(float centre_y, float cp_y)
{
    reset();
    disp = 1;
    centre_ok = 1;
    data_00317490.unknown_d35 = 3;
    centre_value.y = centre_y;
    cp_value.x = 20.0f;
    cp_value.y = cp_y;
    cp_value.z = 22.0f;
    cp_value.w = 23.0f;
    check_draws(SzDuel_Draw3DView_Pipe(1000));
    assert(look_calls == 1 && radius_calls == 1);
    assert(set_cp_calls == 1 && set_pos_calls == 0 && light_calls == 0);
    assert(set_cp.x == 20.0f && set_cp.z == 22.0f && set_cp.w == 23.0f);
    assert(cp_value.y == cp_y);
    assert(data_00317490.unknown_d35 == 3);
    return set_cp.y;
}

static void check_view(void)
{
    /* Model not displayed: draws only. */
    reset();
    check_draws(SzDuel_Draw3DView_Pipe(1000));
    assert(look_calls == 0 && radius_calls == 0 && set_cp_calls == 0);
    assert(set_pos_calls == 0 && data_00317490.unknown_d35 == 0);

    /* First pass, centre offset not available: only the counter moves. */
    reset();
    disp = 1;
    check_draws(SzDuel_Draw3DView_Pipe(1000));
    assert(look_calls == 1 && radius_calls == 1);
    assert(set_pos_calls == 0 && set_look_calls == 0 && light_calls == 0);
    assert(set_cp_calls == 0 && data_00317490.unknown_d35 == 1);

    /* First pass with the centre offset. */
    reset();
    disp = 1;
    ofs_ok = 1;
    centre_value.x = 1.5f;
    centre_value.y = -2.5f;
    centre_value.z = 4.0f;
    centre_value.w = 6.0f;
    sp_value.x = 30.0f;
    sp_value.y = 31.0f;
    sp_value.z = 32.0f;
    sp_value.w = 33.0f;
    check_draws(SzDuel_Draw3DView_Pipe(1000));
    assert(set_pos_calls == 1 && set_look_calls == 1 && light_calls == 1);
    assert(set_pos.x == -1.5f && set_pos.y == 2.5f && set_pos.z == -4.0f && set_pos.w == 6.0f);
    assert(set_look.x == 34.0f && set_look.y == 10.0f && set_look.z == 11.0f);
    assert(set_look.w == 12.0f);
    assert(gSzDuel_PointLight.unknown_20 == 34.0f && gSzDuel_PointLight.unknown_24 == 68.0f);
    assert(gSzDuel_PointLight.position.x == 30.0f && gSzDuel_PointLight.position.y == 31.0f);
    assert(gSzDuel_PointLight.position.z == 32.0f && gSzDuel_PointLight.position.w == 33.0f);
    assert(set_cp_calls == 0 && data_00317490.unknown_d35 == 1);

    /* Later pass, centre not available. */
    reset();
    disp = 1;
    data_00317490.unknown_d35 = 1;
    check_draws(SzDuel_Draw3DView_Pipe(1000));
    assert(set_cp_calls == 0 && set_pos_calls == 0 && data_00317490.unknown_d35 == 1);

    /* Later pass: the step is clamped to -1..1, bounds included. */
    assert(follow(10.5f, 10.0f) == 10.5f);
    assert(follow(9.5f, 10.0f) == 9.5f);
    assert(follow(11.0f, 10.0f) == 11.0f);
    assert(follow(14.0f, 10.0f) == 11.0f);
    assert(follow(9.0f, 10.0f) == 9.0f);
    assert(follow(2.0f, 10.0f) == 9.0f);
}

int main(void)
{
    check_view();
    return 0;
}

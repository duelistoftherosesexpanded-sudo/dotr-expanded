/* Synthetic host tests for the battle_stage_draw group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/battle_stage_draw.h"

struct DotrStageRecord gSzBattle_aStageParam_stage[11];
struct DotrStageCamera data_002921f0_stage;
struct DotrStageVec3 data_00292280_stage;
struct DotrStageVec3 data_00292290_stage;
int gSzBattle_nSave_stage;
signed char chRate_stage;
signed char chFlip_stage;
unsigned char data_002921e0_stage;

enum { F_WASTE = 1, F_GRASS, F_SEA, F_NORMAL, F_PLAIN };

static int blackout, paused, random_value, draw_flag[2], draw_flag_calls, se[3], se_calls, stereo[3], stereo_calls;
static int disp_mask, light_calls, intensity_index[4], nintensity;
static float intensity[4], anim_frame;
static struct DotrStageCamera monster_cam, battle_cam;
static struct DotrStageVec3 monster_amb, battle_light;
static int draws[32][4], ndraws;
static DotrStageWord next_cursor;

int SzEffect_GetBlackOutForBattle(void) { return blackout; }

int SzModel_SetDrawFlag(int flag, int on)
{
    draw_flag[0] = flag;
    draw_flag[1] = on;
    draw_flag_calls++;
    return 0;
}

void SzEffect_SetStraightLightForMonster(DotrStageWord camera)
{
    memcpy(&monster_cam, (const void *)camera, sizeof(monster_cam));
    light_calls++;
}

void SzEffect_SetStraightLightForBattle(DotrStageWord camera)
{
    memcpy(&battle_cam, (const void *)camera, sizeof(battle_cam));
    light_calls++;
}

void SzEffect_SetIntensityForMonster(DotrStageWord ambient)
{
    memcpy(&monster_amb, (const void *)ambient, sizeof(monster_amb));
}

void SzEffect_SetIntensityForBattle(DotrStageWord light)
{
    memcpy(&battle_light, (const void *)light, sizeof(battle_light));
}

int SzModel_GetPauseFlag(int flag)
{
    assert(flag == 8);
    return paused;
}

int func_0019a5c8(void) { return random_value; }

int SzSound_StereoPlaySE(int a0, int left, int right)
{
    stereo[0] = a0;
    stereo[1] = left;
    stereo[2] = right;
    stereo_calls++;
    return 0;
}

float SzModel_GetAnimFrame(int model)
{
    assert(model == 9);
    return anim_frame;
}

int func_001a23b0(float value) { return (int)value; }

int SzSound_PlaySE(int a0, int index)
{
    se[0] = a0;
    se[1] = index;
    se_calls++;
    return 0;
}

void SzEffect_SetIntensityOnly(int index, float value)
{
    assert(nintensity < 4);
    intensity_index[nintensity] = index;
    intensity[nintensity++] = value;
}

int SzModel_GetModelNo(int model) { return model * 100 + 42; }

int SzModel_IsModelDisp(int model) { return (disp_mask >> model) & 1; }

static DotrStageWord draw(int fn, DotrStageWord cursor, int model, int number)
{
    assert(cursor == next_cursor && ndraws < 32);
    draws[ndraws][0] = fn;
    draws[ndraws][1] = model;
    draws[ndraws][2] = number;
    ndraws++;
    return ++next_cursor;
}

DotrStageWord lSzBattle_DrawWasteland_Pipe(DotrStageWord cursor, int model, int number)
{
    return draw(F_WASTE, cursor, model, number);
}

DotrStageWord lSzBattle_DrawGrass_Pipe(DotrStageWord cursor, int model, int number)
{
    return draw(F_GRASS, cursor, model, number);
}

DotrStageWord lSzBattle_DrawSea_Pipe(DotrStageWord cursor, int model, int number)
{
    return draw(F_SEA, cursor, model, number);
}

DotrStageWord lSzBattle_DrawNormal_Pipe(DotrStageWord cursor, int model, int number)
{
    return draw(F_NORMAL, cursor, model, number);
}

DotrStageWord SzModel_DrawWithOutCalc_Pipe(DotrStageWord cursor, int model, int a2, int a3, int a4, int a5, int a6)
{
    assert(a2 == 0 && a3 == 0 && a4 == 0 && a5 == 0 && a6 == 0);
    return draw(F_PLAIN, cursor, model, -1);
}

static void reset(int stage)
{
    int i, k;

    gSzBattle_nSave_stage = stage;
    blackout = 0x40;
    paused = 0;
    random_value = 0x7fffffff;
    draw_flag_calls = se_calls = stereo_calls = light_calls = nintensity = ndraws = 0;
    disp_mask = 0;
    anim_frame = 0.0f;
    next_cursor = 1000;
    memset(&monster_cam, 0, sizeof(monster_cam));
    memset(&battle_cam, 0, sizeof(battle_cam));
    for (i = 0; i < 11; i++) {
        for (k = 0; k < 4; k++) {
            gSzBattle_aStageParam_stage[i].camera.position[k] = (float)(i * 10 + k);
            gSzBattle_aStageParam_stage[i].camera.rotation[k] = i * 0x1000 + k;
            gSzBattle_aStageParam_stage[i].camera.scale[k] = (float)(i + k);
        }
        gSzBattle_aStageParam_stage[i].ambient.x = (float)i;
        gSzBattle_aStageParam_stage[i].light.z = (float)(i * 2);
    }
    data_002921f0_stage.pad30[0] = 0x77;
    data_00292280_stage.x = -1.0f;
    data_00292290_stage.y = -2.0f;
}

static void test_stage7_full_blend(void)
{
    reset(7);
    chRate_stage = 0x40;
    chFlip_stage = 1;
    data_002921e0_stage = 1;
    /* t = (0x40 - 0x3c) / 4 = 1: record 10, the template's other bytes kept. */
    lSzBattle_DrawStage_Pipe(1000);
    assert(monster_cam.position[1] == 101.0f && monster_cam.rotation[2] == 0xa002 && monster_cam.scale[3] == 13.0f);
    assert(monster_cam.pad30[0] == 0x77 && battle_cam.rotation[0] == 0xa000 && light_calls == 2);
    assert(monster_amb.x == 10.0f && battle_light.z == 20.0f && battle_light.y == 0.0f);
    assert(draw_flag_calls == 1 && draw_flag[0] == 0x10 && draw_flag[1] == 1);
    /* Counter 0x41 is clamped to 0x40, the step turns to -1 and the coin picks 0x22/0x23. */
    assert(chRate_stage == 0x40 && chFlip_stage == -1 && data_002921e0_stage == 0);
    assert(stereo_calls == 1 && stereo[0] == 0 && stereo[1] == 0x22 && stereo[2] == 0x23);
    /* The other coin side and no sound once the flag is clear. */
    reset(7);
    chRate_stage = 0x40;
    chFlip_stage = 1;
    data_002921e0_stage = 1;
    random_value = 0x20000000;
    lSzBattle_DrawStage_Pipe(1000);
    assert(stereo_calls == 1 && stereo[1] == 0x24 && stereo[2] == 0x25);
    reset(7);
    chRate_stage = 0x40;
    chFlip_stage = 1;
    data_002921e0_stage = 0;
    lSzBattle_DrawStage_Pipe(1000);
    assert(stereo_calls == 0 && chFlip_stage == -1);
}

static void test_stage7_flicker(void)
{
    reset(7);
    chRate_stage = 0x3e;
    chFlip_stage = -1;
    data_002921e0_stage = 0;
    /* t = 0.5: halfway between records 7 and 10. */
    lSzBattle_DrawStage_Pipe(1000);
    assert(monster_cam.position[0] == 85.0f && monster_cam.rotation[1] == 0x8801);
    assert(monster_amb.x == 8.5f && battle_light.z == 17.0f);
    assert(chRate_stage == 0x3d && draw_flag[1] == 1);

    reset(7);
    chRate_stage = 0x10;
    chFlip_stage = -1;
    data_002921e0_stage = 0;
    random_value = 0;
    /* Below 0x3c: t = 0, record 7, draw flag off; the random 0 restarts the step at +1 and
     * a counter below 0x20 arms the sound flag. */
    lSzBattle_DrawStage_Pipe(1000);
    assert(monster_cam.position[0] == 70.0f && monster_cam.rotation[1] == 0x7001 && draw_flag[1] == 0);
    assert(chFlip_stage == 1 && chRate_stage == 0x11 && data_002921e0_stage == 1);

    reset(7);
    chRate_stage = 0;
    chFlip_stage = -1;
    data_002921e0_stage = 0;
    /* Below zero: counter and step cleared. */
    lSzBattle_DrawStage_Pipe(1000);
    assert(chRate_stage == 0 && chFlip_stage == 0 && data_002921e0_stage == 1);

    reset(7);
    chRate_stage = 0x30;
    chFlip_stage = 1;
    data_002921e0_stage = 0;
    paused = 1;
    /* Paused: nothing advances, the lights are still set. */
    lSzBattle_DrawStage_Pipe(1000);
    assert(chRate_stage == 0x30 && chFlip_stage == 1 && light_calls == 2);
}

static void test_turning_stages(void)
{
    reset(6);
    gSzBattle_aStageParam_stage[6].camera.rotation[1] = 0xff00;
    lSzBattle_DrawStage_Pipe(1000);
    assert(gSzBattle_aStageParam_stage[6].camera.rotation[1] == 0x100 && monster_cam.rotation[1] == 0x100);
    assert(battle_cam.position[0] == 60.0f && draw_flag_calls == 0);

    reset(5);
    lSzBattle_DrawStage_Pipe(1000);
    assert(gSzBattle_aStageParam_stage[5].camera.rotation[2] == 0x5202 && gSzBattle_aStageParam_stage[5].camera.rotation[1] == 0x5001);

    reset(5);
    paused = 1;
    lSzBattle_DrawStage_Pipe(1000);
    assert(gSzBattle_aStageParam_stage[5].camera.rotation[2] == 0x5002 && light_calls == 2);
}

static void test_stage8_sound_and_draws(void)
{
    static const int kinds[6] = { 1, 3, 4, 7, 2, 8 };
    static const int fns[6] = { F_WASTE, F_GRASS, F_SEA, F_NORMAL, F_PLAIN, F_PLAIN };
    int k;

    reset(8);
    anim_frame = 20.5f;
    lSzBattle_DrawStage_Pipe(1000);
    assert(se_calls == 1 && se[0] == 0 && se[1] == 0x28 && light_calls == 0);
    reset(8);
    anim_frame = 220.0f;
    lSzBattle_DrawStage_Pipe(1000);
    assert(se_calls == 1);
    reset(8);
    anim_frame = 21.0f;
    lSzBattle_DrawStage_Pipe(1000);
    assert(se_calls == 0);
    /* Black-out 0x40 gives intensity 0.5 for indexes 2 and 3. */
    assert(nintensity == 2 && intensity_index[0] == 2 && intensity_index[1] == 3 && intensity[1] == 0.5f);

    for (k = 0; k < 6; k++) {
        DotrStageWord r;

        reset(kinds[k]);
        /* Models 7, 8, 12 and 19 shown. */
        disp_mask = 1 << 7 | 1 << 8 | 1 << 12 | 1 << 19;
        r = lSzBattle_DrawStage_Pipe(1000);
        assert(r == 1004 && ndraws == 4);
        assert(draws[0][0] == fns[k] && draws[0][1] == 8);
        assert(draws[1][1] == 12 && draws[2][1] == 19 && draws[2][0] == fns[k]);
        if (fns[k] != F_PLAIN) {
            assert(draws[0][2] == 42 && draws[1][2] == 42);
        }
        assert(draws[3][0] == F_PLAIN && draws[3][1] == 7);
    }
}

int main(void)
{
    test_stage7_full_blend();
    test_stage7_flicker();
    test_turning_stages();
    test_stage8_sound_and_draws();
    return 0;
}

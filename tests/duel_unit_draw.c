/* Synthetic host tests for the duel_unit_draw group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/duel_unit_draw.h"

struct DotrUnitDrawDuel data_00317490;
struct DotrUnitDrawColour data_002914e0, data_002914e8, data_002914f0;
struct DotrUnitDrawAngles data_00291500 = {{11, 22, 33, 44}};

static int mode, black, disp[40];
static struct DotrUnitDrawUnit *units[2][6], *map_unit;
static struct DotrUnitDrawCell cell;
static float frame;
/* Two calls per SzDuel_DrawUnit_Pipe; some checks make several draws without a reset. */
static int intensity_calls, intensity_kind[8];
static float intensity_value[8];
static int map_unit_calls, map_unit_args[2], info_args[2];
static int dir_calls, dir_args[2], trim_calls, trim_args[2];
static int filter_calls, filter_index[2], filter_value[2], filter_set[2];
static struct DotrUnitDrawColour filter_colour;
static int sort_calls, sort_index[4], sort_flags[4], sort_has_v[4], sort_has_rot[4];
static float sort_pos[3];
static struct DotrUnitDrawAngles sort_rot;

int SzGame_GetMode(int a) { assert(a == 1); return mode; }
int SzEffect_GetBlackOutForMonster(void) { return black; }
void SzEffect_SetIntensityOnly(int kind, float value)
{
    assert(intensity_calls < (int)(sizeof intensity_kind / sizeof intensity_kind[0]));
    intensity_kind[intensity_calls] = kind;
    intensity_value[intensity_calls++] = value;
}
struct DotrUnitDrawUnit *SzDuel_GetMapUnit(int x, int y)
{
    map_unit_calls++;
    map_unit_args[0] = x;
    map_unit_args[1] = y;
    return map_unit;
}
struct DotrUnitDrawUnit *SzDuel_GetUnit(int a, int b) { return units[a][b]; }
struct DotrUnitDrawCell *SzDuel_GetMapInfo(int x, int y)
{
    info_args[0] = x;
    info_args[1] = y;
    return &cell;
}
int SzModel_IsModelDisp(int index) { return disp[index]; }
float SzModel_GetAnimFrame(int index) { assert(index == 7); return frame; }
int func_001a23b0(float value) { return (int)value; }
int SzModel_SetAnimDir(int index, int value)
{
    dir_calls++;
    dir_args[0] = index;
    dir_args[1] = value;
    return 0;
}
int SzDuel_TrimUnitDir1(int a, int b)
{
    trim_calls++;
    trim_args[0] = a;
    trim_args[1] = b;
    return 0;
}
int SzModel_SetFilter(int index, int value, void *in)
{
    /* the filter must bracket the submit: set before it, cleared after it */
    assert(sort_calls == filter_calls);
    filter_index[filter_calls] = index;
    filter_value[filter_calls] = value;
    filter_set[filter_calls] = in != NULL;
    if (in != NULL) {
        memcpy(&filter_colour, in, sizeof(filter_colour));
    }
    filter_calls++;
    return 0;
}
int SzModel_ExSort_Pipe(int cursor, int index, float *v, void *a, int b, int c, int d)
{
    assert(b == 0 && c == 0);
    assert(sort_calls < (int)(sizeof sort_index / sizeof sort_index[0]));
    sort_index[sort_calls] = index;
    sort_flags[sort_calls] = d;
    sort_has_v[sort_calls] = v != NULL;
    sort_has_rot[sort_calls] = a != NULL;
    if (v != NULL) {
        memcpy(sort_pos, v, sizeof(sort_pos));
    }
    if (a != NULL) {
        memcpy(&sort_rot, a, sizeof(sort_rot));
    }
    sort_calls++;
    return cursor + 10;
}

static struct DotrUnitDrawUnit unit, other_unit;

static void reset(int new_mode)
{
    memset(&data_00317490, 0, sizeof(data_00317490));
    memset(&unit, 0, sizeof(unit));
    memset(&other_unit, 0, sizeof(other_unit));
    memset(units, 0, sizeof(units));
    memset(disp, 0, sizeof(disp));
    memset(&cell, 0, sizeof(cell));
    map_unit = NULL;
    mode = new_mode;
    black = 0;
    frame = 0.0f;
    intensity_calls = map_unit_calls = dir_calls = trim_calls = filter_calls = sort_calls = 0;
    unit.model = 7;
    unit.owner = 1;
    unit.x = 3;
    unit.y = 4;
    unit.angle = 900;
    unit.pos_x = -40;
    unit.pos_z = 60;
    disp[7] = 1;
    units[1][5] = &unit;
}

static void check_intensity_and_fixed_models(void)
{
    reset(2);
    units[1][5] = NULL;
    black = 0x1c0; /* only the low byte counts */
    assert(SzDuel_DrawUnit_Pipe(100) == 100);
    assert(intensity_calls == 2 && intensity_kind[0] == 4 && intensity_kind[1] == 5);
    assert(intensity_value[0] == 1.5f && intensity_value[1] == 1.5f);
    assert(sort_calls == 0 && map_unit_calls == 0);

    reset(2);
    units[1][5] = NULL;
    disp[33] = 1;
    assert(SzDuel_DrawUnit_Pipe(100) == 110);
    assert(sort_calls == 1 && sort_index[0] == 33 && !sort_has_v[0] && !sort_has_rot[0]);
    assert(sort_flags[0] == (int)0x80fffff6);

    reset(2);
    units[1][5] = NULL;
    disp[32] = disp[33] = 1;
    assert(SzDuel_DrawUnit_Pipe(100) == 120);
    assert(sort_calls == 2 && sort_index[0] == 32 && sort_index[1] == 33);
}

static void check_unit_selection(void)
{
    /* mode 2: no position, the rotation template unchanged, no animation or filter work */
    reset(2);
    assert(SzDuel_DrawUnit_Pipe(0) == 10);
    assert(sort_calls == 1 && sort_index[0] == 7 && !sort_has_v[0] && sort_has_rot[0]);
    assert(sort_rot.v[0] == 11 && sort_rot.v[1] == 22 && sort_rot.v[2] == 33 && sort_rot.v[3] == 44);
    assert(sort_flags[0] == (int)0x80fffff6);
    assert(info_args[0] == 3 && info_args[1] == 4);
    assert(dir_calls == 0 && trim_calls == 0 && filter_calls == 0 && map_unit_calls == 0);

    reset(2);
    disp[7] = 0;
    assert(SzDuel_DrawUnit_Pipe(0) == 0);

    /* a unit in a slot other than 0 is drawn only with unknown_51 or a fade value */
    reset(2);
    unit.slot = 2;
    assert(SzDuel_DrawUnit_Pipe(0) == 0);
    unit.unknown_51 = 1;
    assert(SzDuel_DrawUnit_Pipe(0) == 10);
    unit.unknown_51 = 0;
    unit.fade = 1;
    assert(SzDuel_DrawUnit_Pipe(10) == 20);

    /* every slot of both sides is visited */
    reset(2);
    units[0][0] = &unit;
    assert(SzDuel_DrawUnit_Pipe(0) == 20);
}

static void check_position(void)
{
    reset(0);
    cell.height = 64;
    assert(SzDuel_DrawUnit_Pipe(0) == 10);
    assert(sort_has_v[0]);
    assert(sort_pos[0] == -40.0f && sort_pos[1] == 17.5f && sort_pos[2] == 60.0f);
    assert(sort_rot.v[1] == 22);
    assert(dir_calls == 0 && trim_calls == 0 && filter_calls == 0 && map_unit_calls == 0);

    reset(0);
    cell.height = -5;
    SzDuel_DrawUnit_Pipe(0);
    assert(sort_pos[1] == 0.0f);

    reset(0);
    unit.hidden = -1;
    cell.height = 64;
    SzDuel_DrawUnit_Pipe(0);
    assert(sort_has_v[0] && sort_pos[0] == 0.0f && sort_pos[1] == 0.0f && sort_pos[2] == 0.0f);

    /* flag 4 sinks the unit by 50 * (128 - fade) / 128, never raising it */
    reset(0);
    cell.height = 64;
    unit.flags = 4;
    unit.fade = 64;
    SzDuel_DrawUnit_Pipe(0);
    assert(sort_pos[1] == 17.5f - 25.0f);
    unit.fade = 128;
    SzDuel_DrawUnit_Pipe(0);
    assert(sort_pos[1] == 17.5f);
    unit.fade = 200;
    SzDuel_DrawUnit_Pipe(0);
    assert(sort_pos[1] == 17.5f);
    unit.flags = 0xfb;
    unit.fade = 64;
    SzDuel_DrawUnit_Pipe(0);
    assert(sort_pos[1] == 17.5f);
}

static void check_animation_and_trim(void)
{
    reset(1);
    data_00317490.turn = 1;
    data_00317490.side[1].x = 5;
    data_00317490.side[1].y = 6;
    assert(SzDuel_DrawUnit_Pipe(0) == 10);
    assert(map_unit_calls == 1 && map_unit_args[0] == 5 && map_unit_args[1] == 6);
    assert(dir_calls == 1 && dir_args[0] == 7 && dir_args[1] == 1);
    assert(trim_calls == 1 && trim_args[0] == 1 && trim_args[1] == 0);
    assert(sort_has_v[0] && sort_rot.v[0] == 11 && sort_rot.v[1] == 900 && sort_rot.v[2] == 33);
    assert(filter_calls == 0);

    reset(1);
    unit.unknown_49 = 1;
    frame = 1.5f;
    SzDuel_DrawUnit_Pipe(0);
    assert(dir_calls == 1 && dir_args[0] == 7 && dir_args[1] == 0);
    reset(1);
    unit.unknown_4a = 1;
    frame = 2.0f;
    SzDuel_DrawUnit_Pipe(0);
    assert(dir_calls == 0 && trim_calls == 1);
    reset(1);
    unit.unknown_4a = 1;
    frame = 0.0f;
    SzDuel_DrawUnit_Pipe(0);
    assert(dir_calls == 1 && dir_args[1] == 0);

    /* unknown_d38 set: mode 1 keeps the position but skips animation, trim and rotation */
    reset(1);
    data_00317490.unknown_d38 = 1;
    data_00317490.active = &unit;
    SzDuel_DrawUnit_Pipe(0);
    assert(dir_calls == 0 && trim_calls == 0 && filter_calls == 0);
    assert(sort_has_v[0] && sort_rot.v[1] == 22);
}

static void check_filter(void)
{
    /* the active unit is filtered, not trimmed */
    reset(1);
    data_00317490.active = &unit;
    assert(SzDuel_DrawUnit_Pipe(0) == 10);
    assert(trim_calls == 0 && filter_calls == 2);
    assert(filter_index[0] == 7 && filter_value[0] == 1 && filter_set[0]);
    assert(filter_index[1] == 7 && filter_value[1] == 1 && !filter_set[1]);

    /* another active unit: its target is filtered only when the active unit's bit is set */
    reset(1);
    data_00317490.active = &other_unit;
    data_00317490.target = &unit;
    SzDuel_DrawUnit_Pipe(0);
    assert(trim_calls == 1 && filter_calls == 0);
    other_unit.unknown_57_6 = -1;
    trim_calls = sort_calls = 0;
    SzDuel_DrawUnit_Pipe(0);
    assert(trim_calls == 0 && filter_calls == 2);
    reset(1);
    other_unit.unknown_57_6 = -1;
    data_00317490.active = &other_unit;
    SzDuel_DrawUnit_Pipe(0);
    assert(trim_calls == 1 && filter_calls == 0);
    data_00317490.target = &other_unit;
    trim_calls = sort_calls = 0;
    SzDuel_DrawUnit_Pipe(0);
    assert(trim_calls == 1 && filter_calls == 0);
}

static void check_colour(int turn, int owner, int step, int has_map_unit, int r, int g, int b)
{
    reset(1);
    data_00317490.turn = turn;
    data_00317490.step = step;
    data_00317490.unknown_c86 = 0x2345;
    data_00317490.active = &unit;
    other_unit.owner = owner;
    map_unit = has_map_unit ? &other_unit : NULL;
    SzDuel_DrawUnit_Pipe(0);
    assert(filter_calls == 2);
    assert(filter_colour.b[0] == r && filter_colour.b[1] == g && filter_colour.b[2] == b);
    assert(filter_colour.b[3] == 0x23 + 64);
}

static void check_colours(void)
{
    /* no unit under the cursor, or one of the side to move: the side's own colour */
    check_colour(0, 0, 0, 0, 0, 0, 128);
    check_colour(0, 0, 0, 1, 0, 0, 128);
    check_colour(1, 1, 0, 1, 96, 96, 96);
    /* a unit of the other side */
    check_colour(0, 1, 0, 1, 96, 96, 96);
    check_colour(1, 0, 28, 1, 0, 0, 128);
    /* step 29 and above: the third colour */
    check_colour(0, 1, 29, 1, 96, 0, 128);
    check_colour(1, 1, 30, 0, 96, 128, 0);

    /* the alpha byte wraps */
    reset(1);
    data_00317490.unknown_c86 = 0xf000;
    data_00317490.active = &unit;
    SzDuel_DrawUnit_Pipe(0);
    assert(filter_colour.b[3] == 0x30);
}

int main(void)
{
    check_intensity_and_fixed_models();
    check_unit_selection();
    check_position();
    check_animation_and_trim();
    check_filter();
    check_colours();
    return 0;
}

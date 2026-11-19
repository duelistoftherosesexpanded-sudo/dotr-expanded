/* Synthetic host tests for the duel_stage_draw group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/duel_stage_draw.h"

struct DotrStageDuel data_00317490;
struct DotrStageColour data_002910e0, data_002910e8, data_002910f0, data_002910f8, data_00291100, data_00291108;
struct DotrStageAngles data_00291110;

static int mode, black, disp[96], movable[7][7];
static struct DotrStageCell cells[7][7];
static struct DotrStageUnit *map_unit;
static int intensity_calls, alpha_calls, init_calls, init_args[3], calc_calls, pos_calls, box_calls;
static float intensity_value[3];
static unsigned char init_colour[4];
static struct { int x, y, c, e; unsigned char colour[4]; } tint[128];
static int tint_calls;
static struct { int index, g, angle, has_pos, has_angles; } draw[32];
static int draw_calls;
static struct { int index, value, set; unsigned char alpha; } filter[4];
static int filter_calls;

int SzGame_GetMode(int a) { assert(a == 1); return mode; }
int SzEffect_GetBlackOutForDuel(void) { return black; }
void SzEffect_SetIntensityOnly(int kind, float value)
{
    static const int kinds[3] = { 0, 1, 8 };
    assert(kind == kinds[intensity_calls]);
    intensity_value[intensity_calls++] = value;
}
int SzDuel_SetMapAlpha(int x, int y, int value)
{
    assert(x == alpha_calls % 7 && y == alpha_calls / 7 && value == (black & 0xff));
    alpha_calls++;
    return 0;
}
void SzDuel_InitMapColor(int a, unsigned char *b, int c)
{
    init_args[0] = a;
    init_args[1] = b != NULL;
    init_args[2] = c;
    if (b != NULL) {
        memcpy(init_colour, b, 4);
    }
    init_calls++;
}
struct DotrStageUnit *SzDuel_GetMapUnit(int x, int y)
{
    assert(x == data_00317490.side[data_00317490.turn].x && y == data_00317490.side[data_00317490.turn].y);
    return map_unit;
}
void SzDuel_SetMapColor(int a, int b, int c, unsigned char *colour, int e)
{
    tint[tint_calls].x = a;
    tint[tint_calls].y = b;
    tint[tint_calls].c = c;
    tint[tint_calls].e = e;
    memcpy(tint[tint_calls].colour, colour, 4);
    tint_calls++;
}
int SzDuel_GetMapMovable(int x, int y) { return movable[y][x]; }
int SzDuel_CalcMapColor(int x, int y)
{
    assert(x == calc_calls % 7 && y == calc_calls / 7);
    calc_calls++;
    return 0;
}
struct DotrStageCell *SzDuel_GetMapInfo(int x, int y) { return &cells[y][x]; }
int SzModel_IsModelDisp(int index) { return disp[index]; }
void SzDuel_CalcUnitPos(int x, int y, int z, float *v)
{
    v[0] = (float)x;
    v[1] = (float)y;
    v[2] = (float)z;
    pos_calls++;
}
int SzDuel_DrawBoxCursor_Pipe(int a, float *centre, unsigned char *colour, int single)
{
    assert(centre != NULL && colour == NULL && single == 1);
    box_calls++;
    return a + 1;
}
int SzModel_DrawWithOutCalc_Pipe(int cursor, int index, float *c, void *d, int e, int f, int g)
{
    assert(e == 0 && f == 0);
    draw[draw_calls].index = index;
    draw[draw_calls].g = g;
    draw[draw_calls].has_pos = c != NULL;
    draw[draw_calls].has_angles = d != NULL;
    draw[draw_calls].angle = d != NULL ? ((struct DotrStageAngles *)d)->v[1] : 0;
    draw_calls++;
    return cursor + 0x100;
}
int SzModel_SetFilter(int index, int value, void *in)
{
    filter[filter_calls].index = index;
    filter[filter_calls].value = value;
    filter[filter_calls].set = in != NULL;
    filter[filter_calls].alpha = in != NULL ? ((unsigned char *)in)[3] : 0;
    filter_calls++;
    return 0;
}

static void reset(int new_mode, int turn, int step)
{
    int x, y;

    memset(&data_00317490, 0, sizeof(data_00317490));
    memset(disp, 0, sizeof(disp));
    memset(cells, 0, sizeof(cells));
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            movable[y][x] = 255;
            cells[y][x].model = 9;
        }
    }
    memset(&data_002910f8, 0x11, 4);
    memset(&data_00291100, 0x22, 4);
    memset(&data_00291108, 0x33, 4);
    mode = new_mode;
    black = 0x140;
    map_unit = NULL;
    data_00317490.turn = turn;
    data_00317490.step = step;
    data_00317490.unknown_c86 = 0x2000; /* alpha 0x20 + 64 = 96 */
    data_00317490.side[turn].x = 3;
    data_00317490.side[turn].y = 2;
    data_00317490.side[turn ^ 1].x = 6;
    data_00317490.side[turn ^ 1].y = 6;
    intensity_calls = alpha_calls = init_calls = calc_calls = pos_calls = box_calls = 0;
    tint_calls = draw_calls = filter_calls = 0;
}

static void check_colour(const unsigned char *c, int b0, int b1, int b2, int b3)
{
    assert(c[0] == b0 && c[1] == b1 && c[2] == b2 && c[3] == b3);
}

static void check_fade_and_modes(void)
{
    /* mode 0: map colours reset, no tinting, nothing displayed */
    reset(0, 0, 40);
    assert(lSzDuel_DrawStage_Pipe(0x1000) == 0x1000);
    assert(intensity_calls == 3 && intensity_value[0] == 0.5f && intensity_value[1] == 0.5f &&
           intensity_value[2] == 0.5f);
    assert(alpha_calls == 49);
    assert(init_calls == 1 && init_args[0] == 0 && init_args[1] == 0 && init_args[2] == 0);
    assert(tint_calls == 0 && calc_calls == 0 && draw_calls == 0 && pos_calls == 0);
    /* mode 1 below step 19: reset only */
    reset(1, 0, 18);
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    assert(init_calls == 1 && tint_calls == 0 && calc_calls == 0);
    /* modes outside 0..1 (unsigned compare): no reset */
    reset(2, 0, 40);
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    assert(init_calls == 0 && tint_calls == 0);
    reset(-1, 0, 40);
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    assert(init_calls == 0);
}

static void check_models(void)
{
    /* no cursor colour: every cell of a displayed terrain model is drawn; fixed and turned models */
    reset(2, 0, 0);
    disp[1] = 1;
    disp[5] = disp[6] = 1;
    disp[77] = 1;
    cells[4][2].model = 3;
    cells[4][2].height = 5;
    cells[1][1].model = 3;
    cells[2][3].model = 3; /* the cursor cell is not skipped without a colour */
    assert(lSzDuel_DrawStage_Pipe(0) == 0x800 + 1);
    assert(pos_calls == 3 && box_calls == 1 && filter_calls == 0);
    assert(draw_calls == 8);
    assert(draw[0].index == 77 && draw[0].has_pos && !draw[0].has_angles && draw[0].g == 0);
    assert(draw[1].index == 77 && draw[2].index == 77);
    assert(draw[3].index == 1 && !draw[3].has_pos && !draw[3].has_angles);
    assert(draw[4].index == 5 && draw[4].angle == 0x6000 && draw[5].index == 5 && draw[5].angle == 0xa000);
    assert(draw[6].index == 6 && draw[6].angle == -0x2000 && draw[7].index == 6 && draw[7].angle == 0x2000);
    /* only the second turned model displayed */
    reset(2, 0, 0);
    disp[6] = 1;
    assert(lSzDuel_DrawStage_Pipe(0) == 0x200);
    assert(draw_calls == 2 && draw[0].index == 6 && draw[1].index == 6);
}

static void check_cross_and_cursor_cell(void)
{
    struct DotrStageUnit unit;
    int i, cross = 0;

    /* steps 22 and 24: the cursor's row and column in the own colour at alpha 64 */
    reset(1, 1, 22);
    memset(&unit, 0, sizeof(unit));
    unit.owner = 1;
    map_unit = &unit;
    disp[77] = 1;
    cells[2][3].model = 3;
    cells[2][3].height = 4;
    cells[2][3].alpha = 64;
    cells[5][5].model = 3;
    assert(lSzDuel_DrawStage_Pipe(0) == 0x300 + 1);
    assert(init_calls == 1 && calc_calls == 49);
    assert(tint_calls == 14);
    for (i = 0; i < 13; i++) {
        assert(tint[i].x == 3 || tint[i].y == 2);
        assert(tint[i].c == 1 && tint[i].e == 0);
        check_colour(tint[i].colour, 96, 96, 96, 64);
        cross++;
    }
    assert(cross == 13);
    assert(tint[13].x == 3 && tint[13].y == 2 && tint[13].c == 1);
    check_colour(tint[13].colour, 96, 96, 96, 96);
    /* cursor cell: plain draw, then a filtered draw at alpha * cell alpha / 128, filter cleared */
    assert(pos_calls == 2 && box_calls == 1);
    assert(draw_calls == 3);
    assert(draw[0].index == 77 && draw[0].g == 0 && draw[1].index == 77 && draw[1].g == (int)0x80fffff6);
    assert(filter_calls == 2 && filter[0].index == 77 && filter[0].value == -1 && filter[0].set &&
           filter[0].alpha == 48);
    assert(filter[1].index == 77 && filter[1].value == -1 && !filter[1].set);
    /* the model pass skips the cursor cell and draws the other cell of that model */
    assert(draw[2].index == 77 && draw[2].g == 0);

    /* step 24, turn 0, a flat cursor cell whose model is not displayed: tint only */
    reset(1, 0, 24);
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    assert(tint_calls == 14 && draw_calls == 0 && filter_calls == 0 && box_calls == 0);
    check_colour(tint[0].colour, 0, 0, 128, 64);
    check_colour(tint[13].colour, 0, 0, 128, 96);
    /* a displayed flat cursor cell: no box cursor */
    reset(1, 0, 24);
    cells[2][3].model = 8;
    disp[82] = 1;
    assert(lSzDuel_DrawStage_Pipe(0) == 0x200);
    assert(box_calls == 0 && draw_calls == 2 && filter_calls == 2);
}

static void check_owner_colours(void)
{
    struct DotrStageUnit unit;

    memset(&unit, 0, sizeof(unit));
    /* step 19 has no case of its own: only the cursor cell, in the other side's colour */
    reset(1, 0, 19);
    unit.owner = 1;
    map_unit = &unit;
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    assert(tint_calls == 1 && calc_calls == 49);
    check_colour(tint[0].colour, 96, 96, 96, 96);
    reset(1, 1, 19);
    unit.owner = 0;
    map_unit = &unit;
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    check_colour(tint[0].colour, 0, 0, 128, 96);
    /* own unit */
    reset(1, 0, 28);
    unit.owner = 0;
    map_unit = &unit;
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    check_colour(tint[0].colour, 0, 0, 128, 96);
    /* from step 29 the mark colour wins; step 33 adds nothing */
    reset(1, 0, 33);
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    assert(tint_calls == 1 && init_calls == 1);
    check_colour(tint[0].colour, 96, 0, 128, 96);
    reset(1, 1, 33);
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    check_colour(tint[0].colour, 96, 128, 0, 96);
}

static void check_reach_and_moves(void)
{
    /* step 29: reachable cells at 208 within the reach, 64 beyond it, then the marked cells */
    reset(1, 0, 29);
    data_00317490.reach = 4;
    movable[1][2] = 4;
    movable[6][0] = 5;
    movable[0][0] = 254;
    data_00317490.mark_count = 2;
    data_00317490.mark_x[0] = 1;
    data_00317490.mark_y[0] = 6;
    data_00317490.mark_x[1] = 5;
    data_00317490.mark_y[1] = 0;
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    assert(init_calls == 2 && init_args[0] == 2 && init_args[1] == 1 && init_args[2] == 0);
    check_colour(init_colour, 0x33, 0x33, 0x33, 0x33);
    assert(tint_calls == 6);
    assert(tint[0].x == 0 && tint[0].y == 0);
    check_colour(tint[0].colour, 0x22, 0x22, 0x22, 64);
    assert(tint[1].x == 2 && tint[1].y == 1);
    check_colour(tint[1].colour, 0x22, 0x22, 0x22, 208);
    assert(tint[2].x == 0 && tint[2].y == 6);
    check_colour(tint[2].colour, 0x22, 0x22, 0x22, 64);
    assert(tint[3].x == 1 && tint[3].y == 6 && tint[4].x == 5 && tint[4].y == 0);
    check_colour(tint[3].colour, 96, 0, 128, 96);
    assert(tint[5].x == 3 && tint[5].y == 2);
    check_colour(tint[5].colour, 96, 0, 128, 96);
    assert(calc_calls == 49);

    /* step 62: every reachable cell in the move colour */
    reset(1, 0, 62);
    movable[3][4] = 0;
    movable[5][6] = 254;
    assert(lSzDuel_DrawStage_Pipe(0) == 0);
    assert(init_calls == 2 && init_args[0] == 2);
    assert(tint_calls == 3);
    assert(tint[0].x == 4 && tint[0].y == 3 && tint[1].x == 6 && tint[1].y == 5);
    check_colour(tint[0].colour, 0x11, 0x11, 0x11, 0x11);
    check_colour(tint[2].colour, 96, 0, 128, 96);
}

int main(void)
{
    check_fade_and_modes();
    check_models();
    check_cross_and_cursor_cell();
    check_owner_colours();
    check_reach_and_moves();
    return 0;
}

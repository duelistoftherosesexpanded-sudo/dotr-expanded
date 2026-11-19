/* Synthetic host tests for the battle_sea group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/battle_sea.h"

struct DotrSeaPart *apLocData[4];
unsigned char abLocDataMax[4];
struct DotrSeaRot data_002922a0 = { 11, 22, 33, 44 };

static struct DotrSeaPart parts[3];
static int draw_calls, mirror_calls, set_calls, max_calls, get_calls;
static void *draw_part[4];
static struct DotrSeaRot draw_rot[4];
static float set_value[4];
static float anim_frame;
static int anim_max;

int SzModel_SetAnimFrame(int index, float value)
{
    assert(index == 7);
    set_value[set_calls++] = value;
    return 0;
}
float SzModel_GetAnimFrame(int index)
{
    assert(index == 7);
    get_calls++;
    return anim_frame;
}
int SzModel_GetAnimFrameMax(int index, int value)
{
    assert(index == 7 && value == -1);
    max_calls++;
    return anim_max;
}
int SzModel_DrawWithOutCalc_Pipe(int cursor, int index, DotrSeaWord cw, DotrSeaWord dw, int e, int f, int g)
{
    void *c = (void *)cw;
    void *d = (void *)dw;

    assert(index == 7 && e == 0 && f == 0 && g == 0);
    draw_part[draw_calls] = c;
    if (d != NULL) {
        draw_rot[draw_calls] = *(struct DotrSeaRot *)d;
    } else {
        assert(c == NULL);
    }
    draw_calls++;
    return cursor + 1;
}
int lSzBattle_DrawFakeMirror(int cursor, int index)
{
    assert(index == 15);
    mirror_calls++;
    return cursor + 100;
}

static void reset(void)
{
    draw_calls = mirror_calls = set_calls = max_calls = get_calls = 0;
    memset(draw_part, 0, sizeof draw_part);
    memset(draw_rot, 0, sizeof draw_rot);
}

/* Branch coverage: type 0, the default arm on both sides of the 5..8 range, a table with no
 * entries, and a two-entry table including the frame wrap. */
static void check_draw_sea(void)
{
    reset();
    assert(lSzBattle_DrawSea_Pipe(1000, 7, 0) == 1101);
    assert(draw_calls == 1 && draw_part[0] == NULL && mirror_calls == 1 && set_calls == 0);

    reset();
    assert(lSzBattle_DrawSea_Pipe(1000, 7, 4) == 1001);
    assert(draw_calls == 1 && mirror_calls == 0 && max_calls == 0);
    reset();
    assert(lSzBattle_DrawSea_Pipe(1000, 7, 9) == 1001);
    assert(draw_calls == 1 && mirror_calls == 0 && max_calls == 0);

    anim_frame = 8.0f;
    anim_max = 10;
    apLocData[0] = parts;
    abLocDataMax[0] = 0;
    reset();
    assert(lSzBattle_DrawSea_Pipe(1000, 7, 5) == 1000);
    assert(draw_calls == 0 && max_calls == 1 && get_calls == 1);
    assert(set_calls == 1 && set_value[0] == 8.0f);

    apLocData[3] = parts + 1;
    abLocDataMax[3] = 2;
    parts[1].rx = 90.0f;
    parts[1].ry = -45.0f;
    parts[1].frame = 1;
    parts[2].rx = 180.0f;
    parts[2].ry = 0.0f;
    parts[2].frame = 5;
    reset();
    assert(lSzBattle_DrawSea_Pipe(1000, 7, 8) == 1002);
    assert(draw_calls == 2 && mirror_calls == 0 && set_calls == 3);
    assert(draw_part[0] == &parts[1] && draw_part[1] == &parts[2]);
    assert(draw_rot[0].x == 16384 && draw_rot[0].y == -8192);
    assert(draw_rot[0].z == 33 && draw_rot[0].w == 44);
    assert(draw_rot[1].x == 32768 && draw_rot[1].y == 0);
    assert(draw_rot[1].z == 33 && draw_rot[1].w == 44);
    assert(set_value[0] == 9.0f && set_value[1] == 3.0f && set_value[2] == 8.0f);
}

int main(void)
{
    check_draw_sea();
    return 0;
}

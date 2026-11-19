/* Synthetic host tests for the battle_draws group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/battle_draws.h"

union DotrBattleDrawQuad data_002922b0 = { .f = {2.0f, 1.0f, 4.0f, 1.0f} };
union DotrBattleDrawQuad data_002922c0 = { .i = {1, 0x100, 3, 4} };
union DotrBattleDrawQuad data_002922d0 = { .i = {1, 0x300, 3, 4} };
union DotrBattleDrawQuad data_002922e0 = { .i = {1, 0x500, 3, 4} };
union DotrBattleDrawQuad data_002922f0 = { .i = {1, 0x600, 3, 4} };
union DotrBattleDrawQuad data_00292300 = { .i = {1, 0x900, 3, 4} };
int gSzBattle_bl[3];

struct draw_call {
    int cursor, index, has_pos, has_rot, rot_y;
    float x, z;
};
static struct draw_call draws[32];
static int draw_count, pos_calls, shadow_count, shadow_log[4][2], unit_count, unit_log[2][2];
static struct DotrBattleDrawUnit unit;

int SzModel_DrawWithOutCalc_Pipe(int cursor, int index, DotrBattleDrawWord c, DotrBattleDrawWord d, DotrBattleDrawWord e, int f, int g)
{
    struct draw_call *call = &draws[draw_count++];
    const union DotrBattleDrawQuad *pos = (const union DotrBattleDrawQuad *)c;
    const union DotrBattleDrawQuad *scale = (const union DotrBattleDrawQuad *)e;

    assert(draw_count <= 32 && f == 0 && g == 0);
    assert(scale->f[0] == 2.0f && scale->f[1] == 1.0f && scale->f[2] == 4.0f && scale->f[3] == 1.0f);
    call->cursor = cursor;
    call->index = index;
    call->has_pos = pos != 0;
    call->has_rot = d != 0;
    call->rot_y = d ? ((const union DotrBattleDrawQuad *)d)->i[1] : 0;
    call->x = pos ? pos->f[0] : 0.0f;
    call->z = pos ? pos->f[2] : 0.0f;
    if (pos) {
        assert(pos->f[1] == 12.0f && pos->f[3] == 14.0f);
    }
    return cursor + 1;
}
int SzModel_GetPos(int index, union DotrBattleDrawQuad *out)
{
    assert(index == 9);
    out->f[0] = 11.0f;
    out->f[1] = 12.0f;
    out->f[2] = 13.0f;
    out->f[3] = 14.0f;
    pos_calls++;
    return 0;
}
int lSzBattle_DrawFakeShadow(int cursor, int kind, int unknown, float x, float y)
{
    assert(shadow_count < 4 && unknown == -1 && x == -13.5f && y == 400.0f);
    shadow_log[shadow_count][0] = cursor;
    shadow_log[shadow_count][1] = kind;
    shadow_count++;
    return cursor + 0x100;
}
struct DotrBattleDrawUnit *SzDuel_GetUnit(int a, int b)
{
    assert(unit_count < 2);
    unit_log[unit_count][0] = a;
    unit_log[unit_count][1] = b;
    unit_count++;
    return a == 2 ? &unit : 0;
}

static void reset(void)
{
    draw_count = pos_calls = shadow_count = unit_count = 0;
    memset(draws, 0, sizeof(draws));
}
static int drew(int n, int has_rot, int rot_y, int has_pos, float x, float z)
{
    const struct draw_call *call = &draws[n];
    return call->cursor == 1000 + n && call->index == 9 && call->has_rot == has_rot && call->rot_y == rot_y &&
           call->has_pos == has_pos && call->x == x && call->z == z;
}

/* Branch coverage: every kind, each sign selection, both loop bounds, and each shadow outcome. */
static void check_normal(void)
{
    int n;

    /* Kind 1: four draws, the rotation's y stepped by 0x4000; no position, no shadows. */
    reset();
    assert(lSzBattle_DrawNormal_Pipe(1000, 9, 1) == 1004);
    assert(draw_count == 4 && pos_calls == 0 && shadow_count == 0);
    for (n = 0; n < 4; n++) {
        assert(drew(n, 1, 0x100 + n * 0x4000, 0, 0.0f, 0.0f));
    }
    assert(data_002922c0.i[1] == 0x100);

    /* Kind 3: two draws, stepped by 0x8000. */
    reset();
    assert(lSzBattle_DrawNormal_Pipe(1000, 9, 3) == 1002);
    assert(draw_count == 2 && drew(0, 1, 0x300, 0, 0.0f, 0.0f) && drew(1, 1, 0x8300, 0, 0.0f, 0.0f));

    /* Kind 5: four corners at 275, each drawn at the corner and 100 nearer on x, then on z. */
    reset();
    assert(lSzBattle_DrawNormal_Pipe(1000, 9, 5) == 1012);
    assert(draw_count == 12 && pos_calls == 1 && shadow_count == 0);
    assert(drew(0, 1, 0x500, 1, -550.0f, -1100.0f) && drew(1, 1, 0x500, 1, -350.0f, -1100.0f) &&
           drew(2, 1, 0x500, 1, -550.0f, -700.0f));
    assert(drew(3, 1, 0x500, 1, 550.0f, -1100.0f) && drew(4, 1, 0x500, 1, 350.0f, -1100.0f) &&
           drew(5, 1, 0x500, 1, 550.0f, -700.0f));
    assert(drew(6, 1, 0x500, 1, -550.0f, 1100.0f) && drew(8, 1, 0x500, 1, -550.0f, 700.0f));
    assert(drew(9, 1, 0x500, 1, 550.0f, 1100.0f) && drew(10, 1, 0x500, 1, 350.0f, 1100.0f) &&
           drew(11, 1, 0x500, 1, 550.0f, 700.0f));

    /* Kind 6: four corners at 425; the corner with the rotation, then three steps of 100
     * along x and along z without one. */
    reset();
    assert(lSzBattle_DrawNormal_Pipe(1000, 9, 6) == 1028);
    assert(draw_count == 28 && pos_calls == 1 && shadow_count == 0);
    assert(drew(0, 1, 0x600, 1, -850.0f, -1700.0f));
    assert(drew(1, 0, 0, 1, -650.0f, -1700.0f) && drew(2, 0, 0, 1, -850.0f, -1300.0f));
    assert(drew(3, 0, 0, 1, -450.0f, -1700.0f) && drew(4, 0, 0, 1, -850.0f, -900.0f));
    assert(drew(5, 0, 0, 1, -250.0f, -1700.0f) && drew(6, 0, 0, 1, -850.0f, -500.0f));
    assert(drew(7, 1, 0x600, 1, 850.0f, -1700.0f) && drew(8, 0, 0, 1, 650.0f, -1700.0f));
    assert(drew(14, 1, 0x600, 1, -850.0f, 1700.0f) && drew(16, 0, 0, 1, -850.0f, 1300.0f));
    assert(drew(21, 1, 0x600, 1, 850.0f, 1700.0f) && drew(26, 0, 0, 1, 250.0f, 1700.0f) &&
           drew(27, 0, 0, 1, 850.0f, 500.0f));

    /* Any other non-zero kind: one draw with the default rotation, no shadows. */
    reset();
    assert(lSzBattle_DrawNormal_Pipe(1000, 9, 2) == 1001);
    assert(draw_count == 1 && drew(0, 1, 0x900, 0, 0.0f, 0.0f) && shadow_count == 0 && unit_count == 0);

    /* Kind 0: the default draw, shadow 7, then one shadow per non-negative word whose unit
     * exists: a negative word is skipped without a lookup, a missing unit draws nothing. */
    reset();
    unit.unknown_46 = 0xc8;
    gSzBattle_bl[0] = -1;
    gSzBattle_bl[1] = 0x0305;
    gSzBattle_bl[2] = 0x0207;
    assert(lSzBattle_DrawNormal_Pipe(1000, 9, 0) == 1001 + 0x100);
    assert(draw_count == 1 && drew(0, 1, 0x900, 0, 0.0f, 0.0f));
    assert(shadow_count == 1 && shadow_log[0][0] == 1001 && shadow_log[0][1] == 7);
    assert(unit_count == 1 && unit_log[0][0] == 3 && unit_log[0][1] == 5);

    reset();
    gSzBattle_bl[0] = 0x0207;
    gSzBattle_bl[1] = 0x02ff;
    assert(lSzBattle_DrawNormal_Pipe(1000, 9, 0) == 1001 + 0x300);
    assert(shadow_count == 3 && shadow_log[1][0] == 1001 + 0x100 && shadow_log[1][1] == 0xc8 &&
           shadow_log[2][0] == 1001 + 0x200 && shadow_log[2][1] == 0xc8);
    assert(unit_count == 2 && unit_log[0][0] == 2 && unit_log[0][1] == 7 && unit_log[1][0] == 2 && unit_log[1][1] == 0xff);
}

int main(void)
{
    check_normal();
    return 0;
}

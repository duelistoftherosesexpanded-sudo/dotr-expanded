/* Synthetic host tests for the unit_request group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/unit_request.h"

static int mode_value, mode_calls, standby_value, standby_calls, standby_index;
static int alpha_calls, alpha_index, alpha_value, scale_calls, scale_index;
static struct DotrStateVector scale_seen;

int SzGame_GetMode(int a)
{
    assert(a == 1);
    mode_calls++;
    return mode_value;
}

int SzModel_IsModelStandby(int index)
{
    standby_calls++;
    standby_index = index;
    return standby_value;
}

int SzModel_SetAlpha(int index, int value)
{
    alpha_calls++;
    alpha_index = index;
    alpha_value = value;
    return 1;
}

int SzModel_SetScale(int index, struct DotrStateVector *in)
{
    scale_calls++;
    scale_index = index;
    scale_seen = *in;
    return 1;
}

static struct DotrStateItem item;
static struct DotrStateItemObject object;
static struct DotrStateCell cell;

/* An item whose model part returns at once (state byte +0x45 is 6). */
static struct DotrStateItem *reset(void)
{
    memset(&item, 0, sizeof(item));
    memset(&object, 0, sizeof(object));
    memset(&cell, 0, sizeof(cell));
    item.cardModel = &object;
    item.slot = 6;
    item.modelId = 9;
    item.unknown_4c[1] = 100;
    cell.mapAlpha = 64;
    mode_value = 1;
    mode_calls = standby_calls = alpha_calls = scale_calls = 0;
    standby_value = 1;
    return &item;
}

static void run(void)
{
    lSzDuel_DoUnitRequest(&cell, (DotrStateWord)&item);
    assert(mode_calls == 1);
}

static void check_rejections(void)
{
    struct DotrStateItem *u = reset();
    lSzDuel_DoUnitRequest(0, (DotrStateWord)u);
    assert(mode_calls == 1 && u->unknown_4c[1] == 100);
    lSzDuel_DoUnitRequest(&cell, 0);
    assert(mode_calls == 2 && u->unknown_4c[1] == 100);
    /* Bit 7 of +0x57 skips the fades but not the model part. */
    u = reset();
    u->status.unknown_bit_7 = -1;
    u->slot = 5;
    u->unknown_4f = 1;
    u->ctlRateM = 77;
    standby_value = 0;
    run();
    assert(u->unknown_4c[1] == 100 && object.unknown_40.unknown_0 == 0);
    assert(standby_calls == 1 && alpha_calls == 1 && alpha_index == 9 && alpha_value == 77);
}

static void check_brightness(void)
{
    static const unsigned char down[3][2] = {{100, 96}, {68, 64}, {66, 64}};
    static const unsigned char up[3][2] = {{100, 104}, {124, 128}, {126, 128}};
    struct DotrStateItem *u;
    int i;
    for (i = 0; i < 3; i++) {
        u = reset();
        u->status.unknown_bit_0 = -1;
        u->unknown_4c[1] = down[i][0];
        run();
        assert(u->unknown_4c[1] == down[i][1]);
        u = reset();
        u->unknown_4c[1] = up[i][0];
        run();
        assert(u->unknown_4c[1] == up[i][1]);
    }
    u = reset();
    u->modelStatusFlags.unknown_bit_1 = -1;
    run();
    assert(u->unknown_4c[1] == 100);
}

/* The two 32-step fades (+0x54 by bits 4-5, +0x55 by bits 6-7 of +0x56). */
static void check_angle_fades(void)
{
    static const int cases[7][4] = {
        /* direction, start, end, direction afterwards */
        {1, 255, 255, 0}, {1, 100, 132, 1}, {1, 230, 255, 1},
        {-1, 0, 0, 0}, {-1, 100, 68, -1}, {-1, 20, 0, -1}, {0, 100, 100, 0},
    };
    struct DotrStateItem *u;
    int i;
    for (i = 0; i < 7; i++) {
        u = reset();
        u->unknown_56.unknown_bits_4_5 = cases[i][0];
        u->openRate = cases[i][1];
        u->unknown_55 = 7;
        run();
        assert(u->openRate == cases[i][2] && u->unknown_56.unknown_bits_4_5 == cases[i][3]);
        assert(u->unknown_55 == 7 && u->unknown_56.unknown_bits_6_7 == 0);
        u = reset();
        u->unknown_56.unknown_bits_6_7 = cases[i][0];
        u->unknown_55 = cases[i][1];
        u->openRate = 7;
        run();
        assert(u->unknown_55 == cases[i][2] && u->unknown_56.unknown_bits_6_7 == cases[i][3]);
        assert(u->openRate == 7 && u->unknown_56.unknown_bits_4_5 == 0);
    }
}

/* The two 8-step fades towards a limit: +0x52 (limit +0x50, flags +0x4e,
 * bits 0-1) in the first part and +0x53 (limit +0x51, flags +0x4f, bits 2-3)
 * in the model part. */
static void check_limit_fades(void)
{
    static const int cases[9][6] = {
        /* direction, start, limit, mode, end, end flags (-1: untouched) */
        {1, 40, 40, 1, 40, 1}, {1, 40, 40, 0, 40, 0}, {1, 10, 40, 1, 18, -1}, {1, 36, 40, 1, 40, -1},
        {-1, 0, 40, 1, 0, 1}, {-1, 0, 40, 2, 0, 0}, {-1, 20, 40, 1, 12, -1}, {-1, 5, 40, 1, 0, -1},
        {0, 20, 40, 1, 20, -1},
    };
    struct DotrStateItem *u;
    int i, done;
    for (i = 0; i < 9; i++) {
        done = cases[i][5] >= 0;
        u = reset();
        u->cardModel = 0; /* no object: the fade still runs */
        u->unknown_56.unknown_bits_0_1 = cases[i][0];
        u->ctlRateC = cases[i][1];
        u->ctlGoalC = cases[i][2];
        u->ctlModeC = 0x40;
        mode_value = cases[i][3];
        run();
        assert(u->ctlRateC == cases[i][4]);
        assert(u->ctlModeC == (done ? cases[i][5] : 0x40));
        assert(u->unknown_56.unknown_bits_0_1 == (done ? 0 : cases[i][0]));
        assert(standby_calls == 0);

        u = reset();
        u->slot = 0;
        u->unknown_56.unknown_bits_2_3 = cases[i][0];
        u->ctlRateM = cases[i][1];
        u->ctlGoalM = cases[i][2];
        u->unknown_4f = 0x40;
        mode_value = cases[i][3];
        run();
        assert(standby_calls == 1 && standby_index == 9);
        assert(u->ctlRateM == cases[i][4]);
        assert(u->unknown_4f == (done ? cases[i][5] : 0x40));
        assert(u->unknown_56.unknown_bits_2_3 == (done ? 0 : cases[i][0]));
        assert(alpha_calls == (done && cases[i][5] == 1) && scale_calls == 0);
    }
}

static void check_object(void)
{
    struct DotrUnitRequestObject *o = (struct DotrUnitRequestObject *)&object;
    struct DotrStateItem *u = reset();
    /* Fixed level 128 with bit 1 of +0x58; side 0 adds half a turn. */
    u->modelStatusFlags.unknown_bit_1 = -1;
    u->unknown_55 = 255;
    u->openRate = 255;
    u->ctlRateC = 160;
    run();
    assert(o->unknown_14 == 0x4000 && o->unknown_18 == 0x8000);
    assert(o->unknown_40.unknown_0 == 100 && o->unknown_40.unknown_1 == 100 && o->unknown_40.unknown_2 == 100);
    assert(o->unknown_40.unknown_3 == 0 && o->unknown_30.x == 0.0f);
    /* The cell's level otherwise (after the brightness step); other side. */
    u = reset();
    u->team = 1;
    u->status.unknown_bit_0 = -1;
    u->unknown_55 = 255;
    u->ctlRateC = 160;
    u->ctlModeC = 1;
    run();
    assert(o->unknown_14 == -0x4000 && o->unknown_18 == 0);
    assert(o->unknown_40.unknown_0 == 48 && o->unknown_40.unknown_1 == 48 && o->unknown_40.unknown_2 == 48);
    assert(o->unknown_40.unknown_3 == 160 && o->unknown_30.x == 0.0f);
    u = reset();
    u->ctlRateC = 160;
    u->ctlModeC = 2;
    run();
    assert(o->unknown_40.unknown_3 == 0);
    assert(o->unknown_30.x == 0.125f && o->unknown_30.y == 0.125f && o->unknown_30.z == 0.125f);
}

static void check_model_part(void)
{
    struct DotrStateItem *u = reset();
    /* State 6 and above, or bit 0 of +0x58: nothing is asked of the model. */
    u->unknown_4f = 3;
    run();
    assert(standby_calls == 0 && alpha_calls == 0 && scale_calls == 0);
    u = reset();
    u->slot = 5;
    u->modelStatusFlags.unknown_bit_0 = -1;
    u->unknown_4f = 3;
    run();
    assert(standby_calls == 0 && alpha_calls == 0 && scale_calls == 0);
    /* The fade needs a model on standby and state 0 or bit 4 of +0x57. */
    u = reset();
    u->slot = 0;
    u->unknown_56.unknown_bits_2_3 = 1;
    u->ctlGoalM = 40;
    standby_value = 0;
    run();
    assert(standby_calls == 1 && u->ctlRateM == 0);
    u = reset();
    u->slot = 3;
    u->unknown_56.unknown_bits_2_3 = 1;
    u->ctlGoalM = 40;
    run();
    assert(standby_calls == 1 && u->ctlRateM == 0);
    u->status.unknown_bit_4 = -1;
    mode_calls = 0;
    run();
    assert(u->ctlRateM == 8);
    /* Scale: +0x53 / 128, at most 1. */
    u = reset();
    u->slot = 5;
    u->unknown_4f = 2;
    u->ctlRateM = 64;
    run();
    assert(alpha_calls == 0 && scale_calls == 1 && scale_index == 9);
    assert(scale_seen.x == 0.5f && scale_seen.y == 0.5f && scale_seen.z == 0.5f);
    u->ctlRateM = 128;
    mode_calls = 0;
    run();
    assert(scale_calls == 2 && scale_seen.x == 1.0f);
    u->ctlRateM = 200;
    u->unknown_4f = 3;
    mode_calls = 0;
    run();
    assert(scale_calls == 3 && scale_seen.x == 1.0f && scale_seen.z == 1.0f);
    assert(alpha_calls == 1 && alpha_value == 200);
}

int main(void)
{
    check_rejections();
    check_brightness();
    check_angle_fades();
    check_limit_fades();
    check_object();
    check_model_part();
    return 0;
}

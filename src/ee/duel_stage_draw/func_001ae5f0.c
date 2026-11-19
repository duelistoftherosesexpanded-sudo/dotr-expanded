#include "dotr/ee/duel_stage_draw.h"

/* 0x1ae5f0 lSzDuel_DrawStage_Pipe (name from the Japanese build's symbols). Fades the stage by
 * the duel black-out level, tints the map cells for the current step of the player's turn and
 * submits the terrain models; the cell under the cursor is drawn a second time through a colour
 * filter. Returns the advanced packet cursor. */
int lSzDuel_DrawStage_Pipe(int cursor)
{
    struct DotrStageDuel *duel = &data_00317490;
    struct DotrStageSide *side = &data_00317490.side[data_00317490.turn];
    int mode = SzGame_GetMode(1);
    unsigned char alpha = ((data_00317490.unknown_c86 >> 8) & 0xff) + 64;
    struct DotrStageColour own = data_002910e0;
    struct DotrStageColour other;
    struct DotrStageColour mark;
    unsigned char *colour;
    int kmodel;
    float pos[4];
    unsigned int black;
    int j;
    struct DotrStageCell *cell;
    float level;
    int reach;
    int n;
    int x;
    int model;
    struct DotrStageCell *kcell;
    int y;
    struct DotrStageUnit *unit;
    int i;
    int ky;
    int kx;
    int k;

    own.b[0] = data_00317490.turn == 1 ? 96 : 0;
    own.b[1] = duel->turn == 1 ? 96 : 0;
    own.b[2] = duel->turn == 1 ? 96 : 128;
    own.b[3] = alpha;
    other = data_002910e8;
    other.b[0] = duel->turn == 0 ? 96 : 0;
    other.b[1] = duel->turn == 0 ? 96 : 0;
    other.b[2] = duel->turn == 0 ? 96 : 128;
    other.b[3] = alpha;
    mark = data_002910f0;
    mark.b[0] = duel->turn == 0 ? 96 : 96;
    mark.b[1] = duel->turn == 0 ? 0 : 128;
    mark.b[2] = duel->turn == 0 ? 128 : 0;
    mark.b[3] = alpha;
    colour = 0;
    black = SzEffect_GetBlackOutForDuel() & 0xff;
    level = (float)black / 128.0f;
    SzEffect_SetIntensityOnly(0, level);
    SzEffect_SetIntensityOnly(1, level);
    SzEffect_SetIntensityOnly(8, level);
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            SzDuel_SetMapAlpha(x, y, black & 0xff);
        }
    }
    if ((unsigned int)mode < 2) {
        SzDuel_InitMapColor(0, 0, 0);
    }
    if (mode == 1 && duel->step >= 19) {
        struct DotrStageColour move = data_002910f8;
        struct DotrStageColour range = data_00291100;
        struct DotrStageColour base = data_00291108;

        unit = SzDuel_GetMapUnit(side->x, side->y);
        if (unit && unit->owner != duel->turn) {
            colour = other.b;
        } else {
            colour = own.b;
        }
        if (duel->step >= 29) {
            colour = mark.b;
        }
        switch (duel->step) {
        case 22:
        case 24:
            own.b[3] = 64;
            for (y = 0; y < 7; y++) {
                for (x = 0; x < 7; x++) {
                    if (side->x == x || side->y == y) {
                        SzDuel_SetMapColor(x, y, 1, own.b, 0);
                    }
                }
            }
            own.b[3] = alpha;
            break;
        case 29:
            SzDuel_InitMapColor(2, base.b, 0);
            for (y = 0; y < 7; y++) {
                for (x = 0; x < 7; x++) {
                    reach = SzDuel_GetMapMovable(x, y);
                    if (reach < 255) {
                        range.b[3] = reach <= duel->reach ? 208 : 64;
                        SzDuel_SetMapColor(x, y, 1, range.b, 0);
                    }
                }
            }
            for (i = 0; i < duel->mark_count; i++) {
                SzDuel_SetMapColor(duel->mark_x[i], duel->mark_y[i], 1, mark.b, 0);
            }
            break;
        case 33:
            break;
        case 62:
            SzDuel_InitMapColor(2, base.b, 0);
            for (y = 0; y < 7; y++) {
                for (x = 0; x < 7; x++) {
                    if (SzDuel_GetMapMovable(x, y) < 255) {
                        SzDuel_SetMapColor(x, y, 1, move.b, 0);
                    }
                }
            }
            break;
        }
        SzDuel_SetMapColor(side->x, side->y, 1, colour, 0);
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                SzDuel_CalcMapColor(x, y);
            }
        }
    }
    cell = SzDuel_GetMapInfo(side->x, side->y);
    model = cell->model + 74;
    if (SzModel_IsModelDisp(model)) {
        if (colour) {
            SzDuel_CalcUnitPos(side->x, side->y, cell->height, pos);
            if (cell->height > 0) {
                cursor = SzDuel_DrawBoxCursor_Pipe(cursor, pos, 0, 1);
            }
            cursor = SzModel_DrawWithOutCalc_Pipe(cursor, model, pos, 0, 0, 0, 0);
            colour[3] = (alpha * cell->alpha) >> 7;
            SzModel_SetFilter(model, -1, colour);
            cursor = SzModel_DrawWithOutCalc_Pipe(cursor, model, pos, 0, 0, 0, 0x80fffff6);
            SzModel_SetFilter(model, -1, 0);
            colour[3] = alpha;
        }
    }
    for (k = 0; k < 10; k++) {
        kmodel = k + 74;
        if (SzModel_IsModelDisp(kmodel)) {
            for (ky = 0; ky < 7; ky++) {
                for (kx = 0; kx < 7; kx++) {
                    kcell = SzDuel_GetMapInfo(kx, ky);
                    if (kcell->model == k) {
                        if (colour && side->x == kx && side->y == ky) {
                            continue;
                        }
                        SzDuel_CalcUnitPos(kx, ky, kcell->height, pos);
                        if (kcell->height > 0) {
                            cursor = SzDuel_DrawBoxCursor_Pipe(cursor, pos, 0, 1);
                        }
                        cursor = SzModel_DrawWithOutCalc_Pipe(cursor, kmodel, pos, 0, 0, 0, 0);
                    }
                }
            }
        }
    }
    for (n = 0; n < 3; n++) {
        if (SzModel_IsModelDisp(n)) {
            cursor = SzModel_DrawWithOutCalc_Pipe(cursor, n, 0, 0, 0, 0, 0);
        }
    }
    for (n = 0; n < 2; n++) {
        struct DotrStageAngles turn = data_00291110;
        j = n + 5;
        turn.v[1] = n ? 0 : 0x8000;
        if (SzModel_IsModelDisp(j)) {
            turn.v[1] -= 0x2000;
            cursor = SzModel_DrawWithOutCalc_Pipe(cursor, j, 0, &turn, 0, 0, 0);
            turn.v[1] += 0x4000;
            cursor = SzModel_DrawWithOutCalc_Pipe(cursor, j, 0, &turn, 0, 0, 0);
        }
    }
    return cursor;
}


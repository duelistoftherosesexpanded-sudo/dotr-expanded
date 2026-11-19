/* Evidence: docs/modules/duel.md. Names and signatures are provisional. */
#include "dotr/ee/duel_unit_draw.h"

/* SzDuel_DrawUnit_Pipe (0x1ce430): sets effect intensities 4 and 5 from the monster black-out
 * level, then submits the model of each of the 2 x 6 duel units and the two models 32 and 33
 * through SzModel_ExSort_Pipe. In modes 0 and 1 a unit is placed from its record and the cell
 * height; in mode 1 it also steers the animation direction, trims the facing of units that are
 * not highlighted and applies a colour filter to the active unit (and its target). The name is
 * the Japanese build's; local names and the declaration order are this project's. */
int SzDuel_DrawUnit_Pipe(int cursor)
{
    struct DotrUnitDrawDuel *duel = &data_00317490;
    struct DotrUnitDrawSide *side = &data_00317490.side[data_00317490.turn];
    int mode = SzGame_GetMode(1);
    unsigned char alpha = ((data_00317490.unknown_c86 >> 8) & 0xff) + 64;
    struct DotrUnitDrawColour own = data_002914e0;
    struct DotrUnitDrawColour other;
    struct DotrUnitDrawColour mark;
    struct DotrUnitDrawColour *colour;
    int i;
    float *p;
    float pos[4];
    int j;
    float level;
    struct DotrUnitDrawCell *cell;
    struct DotrUnitDrawUnit *unit;
    int lit;
    int model;
    int m;
    int k;

    own.b[0] = data_00317490.turn == 0 ? 0 : 96;
    own.b[1] = duel->turn == 0 ? 0 : 96;
    own.b[2] = duel->turn == 0 ? 128 : 96;
    own.b[3] = alpha;
    other = data_002914e8;
    other.b[0] = duel->turn == 0 ? 96 : 0;
    other.b[1] = duel->turn == 0 ? 96 : 0;
    other.b[2] = duel->turn == 0 ? 96 : 128;
    other.b[3] = alpha;
    mark = data_002914f0;
    mark.b[0] = duel->turn == 0 ? 96 : 96;
    mark.b[1] = duel->turn == 0 ? 0 : 128;
    mark.b[2] = duel->turn == 0 ? 128 : 0;
    mark.b[3] = alpha;
    colour = 0;
    p = 0;
    level = (float)(unsigned int)(SzEffect_GetBlackOutForMonster() & 0xff) / 128.0f;
    SzEffect_SetIntensityOnly(4, level);
    SzEffect_SetIntensityOnly(5, level);
    if (mode == 1) {
        struct DotrUnitDrawUnit *at = SzDuel_GetMapUnit(side->x, side->y);

        if (at && at->owner != duel->turn) {
            colour = &other;
        } else {
            colour = &own;
        }
        if (duel->step >= 29) {
            colour = &mark;
        }
    }
    if ((unsigned int)mode < 2) {
        p = pos;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 6; j++) {
            struct DotrUnitDrawAngles rot;

            unit = SzDuel_GetUnit(i, j);
            rot = data_00291500;
            lit = 0;
            if (unit) {
                model = unit->model;
                if (SzModel_IsModelDisp(model)) {
                    if (unit->slot == 0 || unit->unknown_51 != 0 || unit->fade != 0) {
                        cell = SzDuel_GetMapInfo(unit->x, unit->y);
                        if (mode == 1 && duel->unknown_d38 == 0) {
                            if (unit->unknown_49 != 0 || unit->unknown_4a != 0) {
                                if ((unsigned int)func_001a23b0(SzModel_GetAnimFrame(model)) < 2) {
                                    SzModel_SetAnimDir(model, 0);
                                }
                            } else {
                                SzModel_SetAnimDir(model, 1);
                            }
                            if (duel->active) {
                                if (duel->active == unit) {
                                    lit = 1;
                                } else if (duel->active->unknown_57_6 && duel->target && duel->target == unit) {
                                    lit = 1;
                                }
                            }
                            if (!lit) {
                                SzDuel_TrimUnitDir1(unit->owner, unit->slot);
                            }
                            rot.v[1] = unit->angle;
                        }
                        if (p) {
                            if (unit->hidden) {
                                p[2] = 0.0f;
                                p[1] = 0.0f;
                                p[0] = 0.0f;
                            } else {
                                int h = cell->height > 0 ? cell->height : 0;

                                p[0] = (float)unit->pos_x;
                                p[1] = (float)(h * 35) / 128.0f;
                                p[2] = (float)unit->pos_z;
                                if (unit->flags & 4) {
                                    int n = 128 - unit->fade > 0 ? 128 - unit->fade : 0;

                                    p[1] -= 50.0f * (float)(unsigned int)n / 128.0f;
                                }
                            }
                        }
                        if (lit) {
                            SzModel_SetFilter(model, 1, colour);
                        }
                        cursor = SzModel_ExSort_Pipe(cursor, model, p, &rot, 0, 0, 0x80fffff6);
                        if (lit) {
                            SzModel_SetFilter(model, 1, 0);
                        }
                    }
                }
            }
        }
    }
    for (k = 0; k < 2; k++) {
        m = k + 32;
        if (SzModel_IsModelDisp(m)) {
            cursor = SzModel_ExSort_Pipe(cursor, m, 0, 0, 0, 0, 0x80fffff6);
        }
    }
    return cursor;
}

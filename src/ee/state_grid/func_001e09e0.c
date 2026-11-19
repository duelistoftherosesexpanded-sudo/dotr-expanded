#include "dotr/ee/state_grid.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

struct DotrStateEntry8 *SzDuel_GetUnitConstEffect2(struct DotrStateItem *unit, int eff)
{
    int ok = 1;
    struct DotrStateCell *square;
    int id;

    if (unit == 0) {
        return 0;
    }
    if (unit->constInfo == 0) {
        return 0;
    }
    if (eff < 0 || eff >= 5) {
        return 0;
    }
    square = cell(unit->locCol, unit->locRow);
    if (square == 0 || square->unit != (DotrStateWord)unit) {
        ok = 0;
    }
    if (ok && unit->slot == 0) {
        id = unit->cardNo;
        if (id >= 854) {
            return 0;
        }
        if (id == 671) {
            return 0;
        }
        return &EffIdx_L[id][eff];
    }
    id = unit->constInfo->effectId;
    if (id == 0xffff) {
        return 0;
    }
    if (unit->slot < 6) {
        return &EffIdx_M[id][eff];
    }
    return &EffIdx_S[id];
}

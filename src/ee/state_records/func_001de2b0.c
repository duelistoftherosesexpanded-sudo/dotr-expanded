#include "dotr/ee/state_compare.h"

int SzDuel_GetPlayMotion(int a0, int a1, int a2, int a3, int a4, int a5, int a6)
{
    struct DotrStateCompareReference *first = SzDuel_GetUnit(a0, a1);
    struct DotrStateCompareReference *second = SzDuel_GetUnit(a2, a3);
    int result = 0;
    int x, y, z;
    int base_x, base_y, base_z;
    if (first != 0 && second != 0) {
        base_x = Ta_GetParamOfs(first, a4, a5, 1);
        base_y = Ta_GetParamOfs(second, a4, a5, 1);
        base_z = Ta_GetParamOfs(second, a4, a5, 0);
        x = base_x + SzDuel_GetUnitAP2(first, 0);
        y = base_y + SzDuel_GetUnitAP2(second, 0);
        z = base_z + SzDuel_GetUnitDP2(second, 0);
        x = ((x > 0 ? x : 0) < 9999) ? (x > 0 ? x : 0) : 9999;
        y = ((y > 0 ? y : 0) < 9999) ? (y > 0 ? y : 0) : 9999;
        z = ((z > 0 ? z : 0) < 9999) ? (z > 0 ? z : 0) : 9999;
        result = 4;
        if (second->unknown_57.unknown_bit_3) {
            if (z >= x) {
                result = 6;
            }
        } else if (y == x) {
            if (a6 == 0 && a0 == SzDuel_GetTurnOwner()) {
                result = 3;
            }
        } else if (x < y) {
            result = 3;
        }
    }
    return result;
}

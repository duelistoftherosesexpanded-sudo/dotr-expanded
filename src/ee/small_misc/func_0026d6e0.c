#include "dotr/ee/small_misc.h"
int Ta_GetUnitGravityLoc(int a, int b, int c, int d)
{
    int r;
    int y;
    int x;
    int t;

    r = SzDuel_GetUnitLoc(c >> 8, c & 0xff);
    y = r & 0xff;
    if (a < y) {
        x = y - 1;
        t = r >> 8;
        if (SzDuel_GetMapUnitID(x, t) < 0) {
            if (d == 0) {
                return (t << 8) | x;
            }
            return Ta_GetUnitGravityLocSearchOnZ(a, b, y, t);
        }
        return Ta_GetUnitGravityLocSearchOnZ(a, b, y, t);
    } else if (a > y) {
        x = y + 1;
        t = r >> 8;
        if (SzDuel_GetMapUnitID(x, t) < 0) {
            if (d == 0) {
                return (t << 8) | x;
            }
            return Ta_GetUnitGravityLocSearchOnZ(a, b, y, t);
        }
        return Ta_GetUnitGravityLocSearchOnZ(a, b, y, t);
    }
    return Ta_GetUnitGravityLocSearchOnZ(a, b, y, r >> 8);
}

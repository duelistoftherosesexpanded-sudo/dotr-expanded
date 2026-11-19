#include "dotr/ee/small_misc.h"

int Ta_Do_CompMove(int team, int unitArrayPos, int col, int row, struct DotrMiscWords4 *out)
{
    if (SzDuel_GetField(col, row) == 9) {
        if (Ta_IsVirus(team, unitArrayPos, out) == 1) {
            return 0x10001;
        }
    }
    return Ta_Do_MoveEfc(team, unitArrayPos, col, row, out);
}

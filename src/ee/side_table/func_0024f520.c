#include "dotr/ee/small_misc.h"

int TaUnit_AddUnitEfc(int side, int pos, int x, int y, struct DotrMiscWords4 *out)
{
    int me = func_00250360_pair(side, pos);
    int kind = SzDuel_GetUnitKind(side, pos);
    int a;
    int b;
    int c;

    a = b = 0;
    if (!func_001df540_word(side, pos) || pos == 0) {
        return 0;
    }
    if (SzDuel_GetField(x, y) == 9 && Ta_IsVirus(side, pos, out) == 1) {
        return 1;
    }
    if (Ta_IsLDRange(me == 0 ? 1 : 0, x, y)) {
        if (Ta_IsLDAbl(16, me == 0 ? 1 : 0) && Ta_IsLD_DestObj(me == 0 ? 1 : 0, kind & 0x1f) && kind >> 5 == 0) {
            a = ConvUnitIDtoFlagList(a, side, pos);
        }
        if (Ta_IsLDAbl(15, me == 0 ? 1 : 0) && Ta_IsLD_CurseObj(me == 0 ? 1 : 0, kind & 0x1f) && kind >> 5 == 0) {
            b = ConvUnitIDtoFlagList(0, side, pos);
        }
        if (a > 0 || b > 0) {
            out->unknown_00[0] = 113;
            out->unknown_00[1] = a;
            out->unknown_00[2] = b;
            out->unknown_00[3] = 0x80000000;
            return 1;
        }
        c = 0;
        if (Ta_IsLDAbl(13, me == 0 ? 1 : 0) && !SzDuel_GetUnitStatus(side, pos, 4)) {
            c = ConvUnitIDtoFlagList(0, side, pos);
        }
        if (c > 0) {
            out->unknown_00[0] = 113;
            out->unknown_00[1] = 0;
            out->unknown_00[2] = 0;
            out->unknown_00[3] = c | 0x80000000;
            return 1;
        }
    }
    return 0;
}

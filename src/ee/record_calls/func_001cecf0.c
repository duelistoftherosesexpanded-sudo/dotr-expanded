#include "dotr/ee/record_calls.h"

int lSzDuel_GetControl(int side, int other)
{
    struct DotrCallRecord *base = data_00317490_callrecs;
    int result;
    struct DotrCallRecord *rec = &data_00317490_callrecs[side];
    struct DotrCallRecord *use;
    int r;

    result = 0;
    switch (rec->kind) {
    case 100:
        result = (unsigned short)SzPad_Check(0, other, 0xffff);
        break;
    case 101:
        result = (unsigned short)SzPad_Check(1, other, 0xffff);
        break;
    default:
        if (rec->kind < 100) {
            use = rec;
            if (TaTuto_IsTuto(side) != 0) {
                use = base;
            }
            if (SzView_InqRequest(-1, -1) == 0) {
                r = g_aLogFunc[rec->kind]((unsigned short)((rec->hi << 8) | rec->lo), use);
                if (r < 0) {
                    if (Rensyu_flg != 0) {
                        if (r == -1) {
                            Taku_Camp_Go_Rensyu_Duel(1);
                        } else {
                            Rensyu_flg = 0;
                            Rensyu_END = 1;
                            SzGame_Pop();
                        }
                    } else {
                        SzGame_Pop();
                    }
                } else {
                    result = (unsigned short)r;
                }
            }
        }
        break;
    }
    return result;
}

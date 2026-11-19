#include "dotr/ee/action_reaction.h"

int TaLogic_GetActionReaction(int side1, int pos1, int side2, int pos2, unsigned char n, int last)
{
    struct jp_7b52 out;
    unsigned int *p;
    int r;

    out.eNO = -1;
    /* MATCHING DEVICE: the pointer is re-pointed for every store; the original
     * writes dwFrom through a register and the other two members directly. */
    p = &out.dwFrom;
    *p = -1;
    p = &out.dwTo;
    *p = -1;
    p = &out.dwPar;
    *p = -1;
    switch (n) {
    case 4:
        r = Ta_IsTrapBInDecideOperation2(side1, pos1, 1, last);
        if ((r & 0xffff) != 1) {
            break;
        }
        if (r >> 16 == 1) {
            return 2;
        }
        return 5;
    case 5:
        r = Ta_IsTrapBInDecideOperation2(side1, pos1, 0, last);
        if ((r & 0xffff) != 1) {
            break;
        }
        return 5;
    case 0:
        r = Ta_IsTrapBInDecideOperation2(side1, pos1, 0, last);
        if ((r & 0xffff) == 1) {
            return 5;
        }
        r = SzDuel_GetUnitLoc(side2, pos2);
        if (r < 0) {
            break;
        }
        if ((Chk_TrapA(side1, pos1, r & 0xff, r >> 8, &out, ((last & 0xff) + 1) << 16) & 0xff) != 1) {
            break;
        }
        switch (out.eNO) {
        case 113:
            return 1;
        case 44:
            return 2;
        case 45:
        case 46:
            return 1;
        }
        break;
    case 6:
        if (Ta_SearchUncTypeNaturalEffectUserNum(14) > 0) {
            return 5;
        }
        r = Ta_IsActiveTrapB(Ta_IsUnitControl(side1, pos1) == 0 ? 1 : 0, 14);
        if (r < 0) {
            break;
        }
        if ((last & 0xff) != 1 && SzDuel_GetUnitStatus(r >> 8, r & 0xff, 4) == 0) {
            break;
        }
        return 5;
    }
    return 0;
}

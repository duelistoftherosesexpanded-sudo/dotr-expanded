#include "dotr/ee/small_misc.h"
int TaLogic_GetMaxApUnit3(unsigned short *words, int key)
{
    int i;
    int best;
    int result;
    int v;
    unsigned short *p;
    int r;
    best = -1;
    result = best;
    i = 1;
    if (TaLogic_GetUnitCtlLstLen(words) > 1) {
        p = words + 1;
        do {
            v = SzDuel_GetUnitLoc(*p >> 8, *p & 0xff);
            if (SzDuel_GetUnitStatus(*p >> 8, *p & 0xff, 4)
                || (key == func_00250360_pair(*p >> 8, *p & 0xff)
                    && SzDuel_GetUnitCurseRest(*p >> 8, *p & 0xff) == 0
                    && SzDuel_GetUnitSpellRest(*p >> 8, *p & 0xff) == 0)) {
                r = TaUtil_GetUnitAP(*p >> 8, *p & 0xff, v & 0xff, v >> 8);
                if (best < r) {
                    result = *p;
                    best = r;
                }
            }
            p++;
            i++;
        } while (i < TaLogic_GetUnitCtlLstLen(words));
    }
    return result;
}

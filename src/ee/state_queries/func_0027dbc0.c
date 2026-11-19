#include "dotr/ee/state_queries.h"
int TaLogic_GetGraveEntryAfterScreen(int side, int target, int mode)
{
    unsigned char count = 0;
    int i;

    if ((mode & 0xff) == 31) {
        return target & 0xff;
    }
    if ((mode & 0xff) == 29) {
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (i == (target & 0xff)) {
                return count & 0xff;
            }
            if ((TaHand_GetCardKind(SzDuel_InqGrave(side, i)) >> 5) == 0) {
                count++;
            }
        }
    } else if ((mode & 0xff) == 30) {
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (i == (target & 0xff)) {
                return count & 0xff;
            }
            if ((TaHand_GetCardKind(SzDuel_InqGrave(side, i)) >> 5) != 0) {
                count++;
            }
        }
    }
    return target & 0xff;
}

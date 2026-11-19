#include "dotr/ee/state_queries.h"
int TaLogic_GetGraveCardCount(int side, int flags, int id)
{
    int i;
    unsigned char count = 0;

    if (flags & 1) {
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (id == SzDuel_InqGrave(side, i)) {
                count++;
            }
        }
    }
    if (flags >> 1) {
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (id == SzDuel_InqGrave(side == 0 ? 1 : 0, i)) {
                count++;
            }
        }
    }
    return count & 0xff;
}

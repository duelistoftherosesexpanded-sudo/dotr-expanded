#include "dotr/ee/state_queries.h"

int TaLogic_GetMaxAPGraveCard(int side, int mask)
{
    int s0;
    int s1;
    int s2;
    int i;
    int sc;
    s2 = -1;
    s1 = s2;
    s0 = s2;
    if (mask & 1) {
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if ((TaHand_GetCardKind(SzDuel_InqGrave(side, i)) >> 5) == 0) {
                sc = (unsigned)TaHand_GetCardAPDP(SzDuel_InqGrave(side, i)) >> 16;
                if (s0 < sc) {
                    s0 = sc;
                    s1 = i;
                    s2 = side;
                }
            }
        }
    }
    if (mask >> 1) {
        for (i = 0; i < SzDuel_GetGraveCount((side == 0 ? 1 : 0)); i++) {
            if ((TaHand_GetCardKind(SzDuel_InqGrave((side == 0 ? 1 : 0), i)) >> 5) == 0) {
                sc = (unsigned)TaHand_GetCardAPDP(SzDuel_InqGrave((side == 0 ? 1 : 0), i)) >> 16;
                if (s0 < sc) {
                    s0 = sc;
                    s1 = i;
                    s2 = (side == 0 ? 1 : 0);
                }
            }
        }
    }
    return (s2 << 24) | (s1 << 16) | s0;
}

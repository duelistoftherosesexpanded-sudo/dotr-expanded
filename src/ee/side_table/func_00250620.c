#include "dotr/ee/side_table.h"

int Ta_GetRecycleRequestLvl(int side, int deckCost)
{
    int i;
    int id;
    int maxRank;

    maxRank = 0;
    for (i = 0; i < 2; i++) {
        if ((id = SzData_GetLeader(side, i)) != 999 && maxRank < SzData_GetRank(side, id)) {
            maxRank = SzData_GetRank(side, id);
        }
    }
    if (func_0019a5c8() % 100 + (maxRank - 1) * 2 >= 90) {
        return deckCost + (func_0019a5c8() % 10 + 1);
    }
    return deckCost - func_0019a5c8() % 12 + 1;
}

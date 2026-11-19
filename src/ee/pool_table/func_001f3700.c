#include "dotr/ee/pool_table.h"

void SzEffect_Init(void)
{
    int i;
    for (i = 0; i < 3; i++) {
        int *p = gSzEffect_aEffFuncTbl[i].unknown_list;
        int *c = &gSzEffect_aEffFuncTbl[i].unknown_count;
        *c = 0;
        while (*p++ != 0) {
            (*c)++;
        }
    }
    RhMem_FillMemory(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    SzEffect_Clear();
    SzEffect_SetBlackOutForMonster(128);
    SzEffect_SetBlackOutForDuel(128);
    SzEffect_SetBlackOutForBattle(128);
}

#include "dotr/ee/state_grid.h"

static inline struct DotrStateEntry20 *entry(int id)
{
    if (id == 671) {
        id = 999;
    }
    if (id < 0 || id >= 854) {
        return 0;
    }
    return &gSzDuel_CardConstInfo[id];
}

void SzDuel_SetupDuel(void)
{
    int i;
    int offset;
    SzEffect_LoadModule(0, 620, 0);
    SzModel_LoadMerge(6, 0, 0);
    SzModel_LoadMerge(0, 0, 0);
    for (i = 0, offset = 0; i < 2; i++, offset += 100) {
        struct DotrStateEntry20 *e = entry(SzData_GetLeader(i, SzData_GetCurrentDeck(i)));
        if (e != 0) {
            SzModel_Load(i + 5, -1, (*(unsigned char *)e & 0x1f) + offset, 1);
        }
    }
}

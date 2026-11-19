#include "dotr/ee/object_table.h"
#include "dotr/ee/state_grid.h"

int SzModel_BattleSeq2(int side, int flag, int unknown)
{
    struct DotrObject *p;
    int packed = SzBattle_InqEntry(side);
    int index;
    SzBattle_InqEntry(side ^ 1);
    index = SzDuel_GetUnitModelID(packed >> 8, packed & 0xff);
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    SzModel_SetAnimIntr(index, 5);
    if (flag != 0) {
        if (p->pCameraMotion2 != 0) {
            SzView_ReqCamera(0, 1, (unknown ? 1 : 0) + 8, 0, index, p->pCameraMotion2);
        }
    }
    return 1;
}

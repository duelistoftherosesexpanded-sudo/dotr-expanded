#include "dotr/ee/state_grid.h"
#include "dotr/ee/object_table.h"

void SzBattle_Entry(int side, int packed)
{
    int *triple = gSzBattle_bl;
    int x = packed >> 8;
    int y = packed & 0xff;
    struct DotrStateItem *item = SzDuel_GetUnit(x, y);
    union DotrStateBlock16 first;
    struct DotrObjectVector4 second;
    int k;
    first = data_002921c0[0];
    first.unknown_words[1] = side ? 0 : 0x8000;
    if (item != 0 && item->constInfo != 0) {
        SzDuel_SetBusyFlag(x, y, 1);
        k = item->modelId;
        SzSound_TransferSEGrp(side + 2, SzModel_GetModelNo(k), 0);
        SzModel_SetRot(k, (struct DotrObjectVector4 *)&first);
        SzModel_GetPos(k, &second);
        second.x = 0.0f;
        second.z = side ? -80.0f : 80.0f;
        SzModel_SetPos(k, &second);
        SzModel_SetDrawFlag(k, 1);
        SzModel_SetDeform(k, 0);
        SzModel_SetDeformRatio(k, 0);
        SzModel_SetAnimDirect(k, 0);
        SzModel_SetAnimDir(k, 1);
        triple[side] = packed;
    }
}

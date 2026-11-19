#include "dotr/ee/cell_drops.h"

void SzBattle_Return(int index, int mode)
{
    struct DotrDropWords q;
    struct DotrDropVector v;
    struct DotrDropVector at;
    int word;
    int x;
    int y;
    struct DotrDropBlock *block;
    struct DotrDropWords *to;
    int object;
    int *table;

    table = gSzBattle_bl;
    word = gSzBattle_bl[index];
    x = word >> 8;
    y = word & 0xff;
    block = func_001df540_drop(x, y);
    *(DotrDropWord128 *)(to = &q) = *(DotrDropWord128 *)&data_002921d0[0];
    q.words[1] = x != 0 ? 0 : 0x8000;
    if (block) {
        object = block->modelId;
        SzEffect_ClearRefID2(word, 1);
        SzDuel_SetBusyFlag(x, y, 0);
        func_001f25d0_drop(object, 0);
        func_001f2430_drop(object, &q);
        func_001f1fd0_drop(object, &v);
        func_001e10f0_drop(block->locCol, block->locRow, 0, &at);
        v.x = at.x;
        v.z = at.z;
        func_001f22b0_drop(object, &v);
        SzModel_SetDeform(object, 1);
        SzModel_SetDeformRatio(object, 32);
        if (mode) {
            SzDuel_SetUnitCtlRateM(x, y, 0, 0);
            SzDuel_SetUnitCtlGoalM(x, y, 0);
        } else {
            SzModel_SetAnimDirect(object, 0);
            SzModel_SetAnimDir(object, 1);
        }
        func_001f26e0_drop(object, 0, 0);
        func_001f2860_drop(object, 1, 0);
        SzModel_CancelMotionBlur(object);
        SzSound_StopSE(index + 2, -1);
        table[index] = -1;
    }
}

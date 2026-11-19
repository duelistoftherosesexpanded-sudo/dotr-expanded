#include "dotr/ee/menu.h"
#include "dotr/ee/movie_operations.h"

void Taku_Camp_Set_Go_Duel(void)
{
    short duelistToVs = SelectedBattleArena[0];

    SzGame_SetMenuNum(1, duelistToVs);
    SzGame_SetMenuNum(2, data_00299e10[0]);
    if (data_00299e10[0] == 0) {
        SzGame_SetMenuNum(3, 1100);
        SzGame_SetMenuNum(4, duelistToVs + 1000);
    } else {
        SzGame_SetMenuNum(3, duelistToVs + 1000);
        SzGame_SetMenuNum(4, 1100);
    }
    SzGame_Push(1, 0);
}

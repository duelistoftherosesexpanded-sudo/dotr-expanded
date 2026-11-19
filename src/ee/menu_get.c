#include "dotr/ee/menu.h"

/* Evidence: docs/modules/game-modes.md */
int SzGame_GetMenuNum(int index)
{
    struct DotrMenuStatePrefix *state = &dotr_ee_state_00317250;
    if (index < 0 || index >= 37)
        return -1;
    return state->menu_nums[index];
}

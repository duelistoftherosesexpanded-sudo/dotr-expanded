#include "dotr/ee/menu.h"

/* Evidence: docs/modules/game-modes.md */
int SzGame_SetMenuNum(int index, int value)
{
    struct DotrMenuStatePrefix *state = &dotr_ee_state_00317250;
    if (index < 0 || index >= 37)
        return 0;
    state->menu_nums[index] = value;
    return 1;
}

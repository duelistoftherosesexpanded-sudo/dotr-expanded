#include "dotr/ee/small_misc.h"

void Taku_Camp_Go_Help(void)
{
    HELP_flg[0] = 1;
    *(struct DotrMiscLargeWords *)0x1e301e0 = *(struct DotrMiscLargeWords *)SzData_GetData(0);
    SzGame_Push(1, 0);
}

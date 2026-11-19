#include "dotr/ee/small_misc.h"

void TkCamp_Term(void)
{
    int first;
    int second;
    int third;
    int value;
    int left;
    int right;
    int side;

    if ((signed char)HELP_flg[0] != 0) {
        *(struct DotrMiscLargeWords *)0x1e301e0 = *(struct DotrMiscLargeWords *)SzData_GetData(0);
        first = SzData_GetOption(0, 0);
        second = SzData_GetOption(0, 1);
        third = SzData_GetOption(0, 2);
        switch (data_01ed546b) {
        case 0:
            value = 32;
            left = 17;
            right = 18;
            break;
        case 1:
            value = 33;
            left = 19;
            right = 20;
            break;
        case 2:
            value = 34;
            left = 21;
            right = 22;
            break;
        case 3:
            value = 35;
            left = 23;
            right = 24;
            break;
        default:
            value = 36;
            left = 25;
            right = 26;
            break;
        }
        SzGame_SetMenuNum(1, 0);
        SzGame_SetMenuNum(2, 0);
        SzGame_SetMenuNum(3, value);
        SzGame_SetMenuNum(4, value);
        SzData_SetupFromConstDeck(0, left);
        SzData_SetupFromConstDeck(1, right);
        for (side = 0; side < 2; side++) {
            SzData_SetOption(side, 0, first);
            SzData_SetOption(side, 1, second);
            SzData_SetOption(side, 2, third);
        }
    }
    SzDuel_CloseMsgPopup();
}

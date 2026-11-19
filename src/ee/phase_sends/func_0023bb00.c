#include "dotr/ee/phase_sends.h"

/* Picks (value, first, second) by the byte at 0x34f700 (0..4; any other value
 * leaves them unset, as in the original), sends them, then advances the byte
 * while it is below 3; on reaching 3 it goes back to 0 and the byte at
 * 0x34f708 is set. The argument selects the last call. */
void Taku_Camp_Go_Rensyu_Duel(int flag)
{
    int value;
    int first;
    int second;

    switch (Rensyu_flg) {
    case 0:
        value = 32;
        first = 17;
        second = 18;
        break;
    case 1:
        value = 33;
        first = 19;
        second = 20;
        break;
    case 2:
        value = 34;
        first = 21;
        second = 22;
        break;
    case 3:
        value = 35;
        first = 23;
        second = 24;
        break;
    case 4:
        value = 36;
        first = 25;
        second = 26;
        break;
    }
    SzGame_SetMenuNum(1, 0);
    SzGame_SetMenuNum(2, 0);
    SzGame_SetMenuNum(3, value);
    SzGame_SetMenuNum(4, value);
    SzData_SetupFromConstDeck(0, first);
    SzData_SetupFromConstDeck(1, second);
    if (Rensyu_flg < 3) {
        Rensyu_flg++;
        if (Rensyu_flg > 2) {
            Rensyu_flg = 0;
            Rensyu_END = 1;
        }
    }
    if (flag != 0) {
        SzGame_Goto(1, 0);
    } else {
        SzGame_Push(1, 0);
    }
}

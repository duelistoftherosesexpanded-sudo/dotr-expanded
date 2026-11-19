#include "dotr/ee/kind_calls.h"

/* Dispatches on the kind byte for `index`: high nibble 1 selects by the low nibble (0-4 and 7
 * with c + 14 and d + 11, or d - 6 for 7; 5 and 6 with the arguments unchanged through
 * lSzDuel_DrawCustomDuel_Menu_Top_Pipe; 8-15 no call), any other high nibble goes to
 * lSzDuel_DrawCustomDuel_Menu_Top_Pipe. One jump table at 0x2beb20. */
int lSzDuel_DrawCustomDuel_Menu_Pipe(int a, int index, int c, int d)
{
    unsigned char kind = DOTR_KIND_BYTE(index);

    if ((kind & 0xf0) == 0x10) {
        switch (kind & 0xf) {
        case 0:
            a = lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe(a, index, c + 14, d + 11);
            break;
        case 1:
            a = lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe(a, index, c + 14, d + 11);
            break;
        case 2:
            a = lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe(a, index, c + 14, d + 11);
            break;
        case 3:
            a = lSzDuel_DrawCustomDuel_Menu_LeaderAbility_Pipe(a, index, c + 14, d + 11);
            break;
        case 4:
            a = lSzDuel_DrawCustomDuel_Menu_CardDisp_Pipe(a, index, c + 14, d + 11);
            break;
        case 5:
        case 6:
            a = lSzDuel_DrawCustomDuel_Menu_Top_Pipe(a, index, c, d);
            break;
        case 7:
            a = lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe(a, index, c + 14, d - 6);
            break;
        }
    } else {
        a = lSzDuel_DrawCustomDuel_Menu_Top_Pipe(a, index, c, d);
    }
    return a;
}

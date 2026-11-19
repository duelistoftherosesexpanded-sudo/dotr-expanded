#include "dotr/ee/small_misc.h"

/* Calls lSzDuel_ActCustomDuelProc(0); when that returns zero and the byte at 0xa9c088
 * is zero, also calls lSzDuel_ActCustomDuelProc(1). The switch form is what reproduces
 * the original branch layout. */
void SzDuel_ActCustomDuel(void)
{
    if (lSzDuel_ActCustomDuelProc(0) == 0) {
        switch (customDuelOpponentId[0]) {
        case 0:
            lSzDuel_ActCustomDuelProc(1);
            return;
        }
    }
}

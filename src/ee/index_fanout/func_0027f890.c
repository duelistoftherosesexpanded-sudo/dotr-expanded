#include "dotr/ee/index_fanout.h"

int TaTuto_ControlFade(int step, int dir)
{
    unsigned char done = 0;

    if (dir > 0) {
        if ((unsigned char)SzEffect_GetBlackOutForDuel() < 128) {
            SzEffect_SetBlackOutForDuel((unsigned char)(step + (unsigned char)SzEffect_GetBlackOutForDuel()));
        } else {
            done++;
        }
        if ((unsigned char)SzEffect_GetBlackOutForMonster() < 128) {
            SzEffect_SetBlackOutForMonster((unsigned char)(step + (unsigned char)SzEffect_GetBlackOutForMonster()));
        } else {
            done++;
        }
    } else if (dir < 0) {
        if ((unsigned char)SzEffect_GetBlackOutForDuel() > 0) {
            if ((unsigned char)SzEffect_GetBlackOutForDuel() - step < 0) {
                SzEffect_SetBlackOutForDuel(0);
            } else {
                SzEffect_SetBlackOutForDuel((unsigned char)((unsigned char)SzEffect_GetBlackOutForDuel() - step));
            }
        } else {
            done++;
        }
        if ((unsigned char)SzEffect_GetBlackOutForMonster() > 0) {
            if ((unsigned char)SzEffect_GetBlackOutForMonster() - step < 0) {
                SzEffect_SetBlackOutForMonster(0);
            } else {
                SzEffect_SetBlackOutForMonster((unsigned char)((unsigned char)SzEffect_GetBlackOutForMonster() - step));
            }
        } else {
            done++;
        }
    }
    if (done == 2) {
        return 1;
    }
    return 0;
}

#include "dotr/ee/kind_dispatch.h"

void SzGame_ModeTerm(int kind)
{
    signed char *view = (signed char *)&gSzGame_gl_digits;

    switch (kind) {
    case 1:
        SzDuel_Term();
        if (view[0x21f] == 9) {
            SzDuel_InitCustomDuel(-1);
        }
        break;
    case 5:
        SzBattle_Term();
        break;
    case 6:
        SzDuel_TermBuildDeck();
        if (view[0x21f] == 9) {
            SzDuel_InitCustomDuel(-1);
        }
        break;
    case 7:
        SzDuel_Term3DView();
        break;
    case 8:
        SzDuel_TermTrade();
        break;
    case 9:
        SzDuel_TermCustomDuel();
        break;
    case 36:
        lSzGame_TermMemCheck();
        break;
    case 10:
        lSzGame_TermTitle();
        break;
    case 11:
        TkCamp_Term();
        ShFace_Term();
        break;
    case 12:
        SzEvent_Term();
        break;
    case 13:
        SzDuel_TermMapEdit();
        break;
    case 14:
        ShFace_Term();
        break;
    case 15:
        ShSlot_Term();
        if (view[0x21f] == 9) {
            SzDuel_InitCustomDuel(-1);
        }
        break;
    case 19:
        SzBattle_Term();
        SzView_TermCamera();
        break;
    }
}

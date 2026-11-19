#include "dotr/ee/boot_sequence.h"

void Main_LoopProc(void)
{
    SzPad_Status();
    SzSound_VSync();
    SzFile_DoRequest();
    SzGame_ModeAct();
    SzView_DoRequest(-1, -1);
    SzDuel_DoUnitRequest();
    Main_Draw();
    SzGame_SetFrameSpeed(RhBase_FlipBuffer());
    if (SzGame_GetStabilizer() != 0) {
        SzGame_SetFrameSpeed(1);
    } else {
        SzGame_SetStabilizer(1);
    }
    Main_VSync();
    SzGame_AddFrameCount();
    SzPad_Act();
}

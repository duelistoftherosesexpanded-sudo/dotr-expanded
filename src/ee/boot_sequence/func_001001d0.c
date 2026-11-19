#include "dotr/ee/boot_sequence.h"

int Main_Init(void)
{
    int result;
    int ready;
    int opened;

    SzFile_Init();
    opened = RhBase_Init(0, 10, 0x31, 0x600000, 0x780000, 0x180000, 0x4000000);
    if (opened == 0) {
        result = 0;
    } else {
        RhBase_SetClearColorEx(0, 0, 0, 0);
        SzGame_ClearGsMem(10, 0, 0, 0, 0x400, 0x400);
        SzText_Init();
        SzPad_Init();
        SzSound_Init();
        SzModel_Init();
        SzView_Init();
        SzFade_Init();
        SzGame_Init();
        SzEffect_Init();
        SzDebug_Init();
        SzPad_SetParam(0, 12, 1);
        SzPad_SetParam(1, 12, 1);
        ready = SzView_Create(0);
        if (ready == 0) {
            result = 0;
        } else {
            SzMsg_Initialize();
            SzData_Initialize();
            SzDuel_Initialize();
            SzBattle_Initialize();
            opened = lMain_InitModel2();
            if (opened == 0) {
                result = 0;
            } else {
                SzGame_SetMode(0x24, 0);
                SzFade_Entry(1, 0, 1, 0, 0x5a, 0);
                SzFade_SetKillFlag(1);
                SzFade_SetPauseFlag(1, 1);
                lMain_LoadModel();
                result = 1;
            }
        }
    }
    return result;
}

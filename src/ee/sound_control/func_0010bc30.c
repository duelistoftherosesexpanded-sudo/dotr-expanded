#include "dotr/ee/sound_control.h"

void SD_KickSEGroupReadSequence(int a, char *name)
{
    g_SDValue[0]->unknown_20b4 = a;
    func_0019b648(g_SDValue[0]->unknown_2070, data_002b90b0, name);
    SD_AddReadToEeReq(g_SDValue[0]->unknown_2070, (int)(DotrSoundControlWord)(g_SDBuffer[0] + 0x101800), 0, 0x80800);
    g_SDValue[0]->unknown_0000 |= 0x4000;
    g_SDValue[0]->unknown_20b0 = 30;
}

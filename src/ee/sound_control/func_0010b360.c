#include "dotr/ee/sound_control.h"

void SD_vInitializeAutoDMA(void)
{
    if (!(g_SDValue[0]->unknown_0000 & 0x40)) {
        SDRPC_SendCommand(0x5000);
        g_SDValue[0]->unknown_0000 |= 0x40;
        g_SDValue[0]->unknown_01e8 = -1;
    }
}

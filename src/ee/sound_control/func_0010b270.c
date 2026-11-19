#include "dotr/ee/sound_control.h"

int SD_iStopAutoDMA(void)
{
    if (!(g_SDValue[0]->unknown_0004 & 2)) {
        return 0;
    }
    if (g_SDValue[0]->unknown_0000 & 0x100) {
        g_SDValue[0]->unknown_0000 &= ~0x100;
        g_SDValue[0]->unknown_0000 &= ~0x200;
        g_SDValue[0]->unknown_0018 = 0;
        g_SDValue[0]->unknown_01c4 = 0;
        g_SDValue[0]->unknown_01c0 = 0;
        SDRPC_SendCommand(0x1021);
        g_SDValue[0]->unknown_01d4 = 0;
        g_SDValue[0]->unknown_01d8 = 0;
        g_SDValue[0]->unknown_2254 = -1;
    }
    return 1;
}

#include "dotr/ee/sound_control.h"

int SD_iStartAutoDMA(void)
{
    g_SDValue[0]->unknown_0000 |= 0x100;
    g_SDValue[0]->unknown_01fc &= ~4;
    g_SDValue[0]->unknown_01d4 = 0;
    g_SDValue[0]->unknown_2254 = g_SDValue[0]->unknown_01dc;
    SDRPC_SendCommand(0x1020, g_SDValue[0]->unknown_0014, 0x40000, 1);
    return 1;
}

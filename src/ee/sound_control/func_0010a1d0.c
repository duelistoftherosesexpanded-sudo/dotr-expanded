#include "dotr/ee/sound_control.h"

void SD_SEPlay2(int a, int b, int c)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return;
    }
    if (g_SDValue[0]->unknown_0004 & 4) {
        if (g_SDValue[0]->unknown_0000 & 1) {
            if ((int)SD_iGetSDStatus() & 0x80) {
                SDRPC_SendCommand(0x5210, a, b, c, 0);
            }
        }
    }
}

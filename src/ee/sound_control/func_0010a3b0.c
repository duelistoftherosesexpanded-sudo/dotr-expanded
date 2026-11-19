#include "dotr/ee/sound_control.h"

void SD_SEStop(int a, int b)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return;
    }
    if (g_SDValue[0]->unknown_0004 & 4) {
        if (g_SDValue[0]->unknown_0000 & 1) {
            if ((int)SD_iGetSDStatus() & 0x80) {
                SDRPC_SendCommand(0x5201, a, b);
            }
        }
    }
}

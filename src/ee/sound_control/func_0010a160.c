#include "dotr/ee/sound_control.h"

void SD_BGMSetSourceLevel(int a, int b)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return;
    }
    if (g_SDValue[0]->unknown_0004 & 2) {
        if (g_SDValue[0]->unknown_0000 & 1) {
            if (b < 0) {
                b = -b;
            }
            SDRPC_SendCommand(0x5005, a, b);
        }
    }
}

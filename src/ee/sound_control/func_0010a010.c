#include "dotr/ee/sound_control.h"

void SD_BGMStop(void)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return;
    }
    if (g_SDValue[0]->unknown_0004 & 2) {
        if (g_SDValue[0]->unknown_0000 & 1) {
            SD_iStopAutoDMA();
        }
        g_SDValue[0]->unknown_0000 &= ~0x200;
    }
}

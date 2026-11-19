#include "dotr/ee/sound_control.h"

void SD_BGMFadeOut(short value)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return;
    }
    if (g_SDValue[0]->unknown_0004 & 2) {
        SDRPC_SendCommand(0x5003, value);
    }
}

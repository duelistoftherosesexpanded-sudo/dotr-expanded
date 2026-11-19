#include "dotr/ee/sound_control.h"

void SD_SetOutputType(int value)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return;
    }
    SDRPC_SendCommand(0xf003, value);
    g_SDValue[0]->unknown_2a6c = value;
}

#include "dotr/ee/sound_control.h"

void SD_StopAll(void)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return;
    }
    SD_BGMStop();
    SD_SEStopAll();
}

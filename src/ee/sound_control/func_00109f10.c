#include "dotr/ee/sound_control.h"

int SD_IsLoadingNow(void)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return 0;
    }
    return g_SDValue[0]->unknown_0000 & 0x2000 ? 1 : 0;
}

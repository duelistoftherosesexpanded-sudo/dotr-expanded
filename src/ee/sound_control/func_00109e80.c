#include "dotr/ee/sound_control.h"

int SD_BGMPreLoad(int unknown_000, int flag)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return 0;
    }
    if (!(g_SDValue[0]->unknown_0004 & 2)) {
        return 1;
    }
    if (g_SDValue[0]->unknown_0000 & 1) {
        if (flag) {
            SD_iPlayBGMByAutoDMA(unknown_000);
        } else {
            SD_iPreparePreBlockOnIOP(unknown_000);
        }
    }
    return 1;
}

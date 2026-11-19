#include "dotr/ee/sound_control.h"

int SD_iTransferSEGrpToIop(int a, void *data)
{
    if (!g_SDValue[0]) {
        return 0;
    }
    if (!(g_SDValue[0]->unknown_0004 & 4)) {
        return 0;
    }
    if (!(g_SDValue[0]->unknown_0000 & 1)) {
        return 0;
    }
    return SD_AddToXferQueue(a, data);
}

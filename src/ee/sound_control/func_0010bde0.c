#include "dotr/ee/sound_control.h"

int SD_DoesExistAnyCDReq(void)
{
    if (g_SDValue[0]->read_count) {
        return g_SDValue[0]->read_count;
    }
    if (g_SDValue[0]->unknown_171c) {
        return g_SDValue[0]->unknown_171c;
    }
    return 0;
}

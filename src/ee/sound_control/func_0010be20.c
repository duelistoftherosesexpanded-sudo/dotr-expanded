#include "dotr/ee/sound_control.h"

int SD_DoesExistAnySIFReq(void)
{
    if (g_SDValue[0]->request_count) {
        return g_SDValue[0]->request_count;
    }
    if (g_SDValue[0]->unknown_182c) {
        return g_SDValue[0]->unknown_182c;
    }
    return 0;
}

#include "dotr/ee/sound_queue.h"

void SD_AddEeToIopReq(int unknown_000, int unknown_004, int unknown_008)
{
    struct DotrSoundQueueRequest *request =
        &g_SDValue[0]->requests[g_SDValue[0]->request_write];
    request->unknown_000 = unknown_000;
    request->unknown_004 = unknown_004;
    request->unknown_008 = unknown_008;
    g_SDValue[0]->request_write = (g_SDValue[0]->request_write + 1) % 16;
    g_SDValue[0]->request_count++;
}

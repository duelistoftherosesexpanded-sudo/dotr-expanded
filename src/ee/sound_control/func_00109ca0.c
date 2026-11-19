#include "dotr/ee/sound_control.h"

int SD_Init(struct DotrSoundQueueManager *block)
{
    g_SDValue[0] = block;
    g_SDBuffer[0] = (char *)(block + 1);
    func_00198948(g_SDValue[0], 0, sizeof(struct DotrSoundQueueManager));
    g_SDValue[0]->unknown_01dc = -1;
    g_SDValue[0]->unknown_01f4 = -1;
    g_SDValue[0]->unknown_2248 = -1;
    g_SDValue[0]->unknown_2254 = -1;
    g_SDValue[0]->unknown_224c[0] = -1;
    g_SDValue[0]->unknown_2258 = -1;
    g_SDValue[0]->unknown_224c[1] = -1;
    g_SDValue[0]->unknown_225c = -1;
    SD_ClearXferQueue();
    SD_InitializeDevice();
    return 1;
}

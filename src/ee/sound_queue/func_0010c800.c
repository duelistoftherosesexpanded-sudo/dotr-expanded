#include "dotr/ee/sound_queue.h"

int SDRPC_StartTransferToIop(int unknown_0, int unknown_1, int unknown_2)
{
    func_0017bb60(0);
    g_SDValue[0]->unknown_0044[0] = unknown_0;
    g_SDValue[0]->unknown_0044[1] = unknown_1;
    g_SDValue[0]->unknown_0044[2] = unknown_2;
    g_SDValue[0]->unknown_0044[3] = 0;
    g_SDValue[0]->unknown_0064 = func_0017bca0(g_SDValue[0]->unknown_0044, 1);
    return g_SDValue[0]->unknown_0064;
}

#include "dotr/ee/sound_control.h"

void SD_CallEveryVSync(void)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return;
    }
    if (g_SDValue[0]->unknown_0000 & 1) {
        if (SDRPC_SendCommand(0x5002) == 0) {
            func_00198790(g_SDValue[0]->unknown_0054, g_SDValue[0]->unknown_0180, 16);
            g_SDValue[0]->unknown_01ec = g_SDValue[0]->unknown_0054[3];
            if (g_SDValue[0]->unknown_0054[2]) {
                g_SDValue[0]->unknown_0018 = g_SDValue[0]->unknown_0054[2];
            }
        }
        SD_vDoFsCommandQueue();
        SD_vAutoDMASequentialWatchDog();
        SD_vTransferSEGrpToIopProc();
        SD_vRunAmbienceOnIopProc();
    }
}

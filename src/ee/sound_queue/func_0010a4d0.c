#include "dotr/ee/sound_queue.h"

void SD_vRunAmbienceOnIopProc(void)
{
    int i;

    if (!g_SDValue[0]) {
        return;
    }
    if (!(g_SDValue[0]->unknown_0004 & 8)) {
        return;
    }
    for (i = 0; i < 2; i++) {
        switch (g_SDValue[0]->ambience_step[i]) {
        case 1:
            if (SD_DoesExistAnyCDReq()) {
                return;
            }
            if (*(unsigned char *)&g_SDValue[0]->unknown_0054[1]) {
                return;
            }
            if (g_SDValue[0]->unknown_204c) {
                return;
            }
            func_0019b648(g_SDValue[0]->ambience_name, data_002b9090, g_SDValue[0]->ambience_file[i]);
            if (!RhFile_ExistFile(g_SDValue[0]->ambience_name)) {
                g_SDValue[0]->ambience_step[i] = 0;
                break;
            }
            SD_AddReadToEeReq(g_SDValue[0]->ambience_name, (int)(DotrSoundQueueWord)(g_SDBuffer[0] + (i << 12) + 0x101800), 0, 0x1000);
            g_SDValue[0]->ambience_step[i]++;
            return;
        case 2:
            if (SD_DoesExistAnyCDReq()) {
                return;
            }
            g_SDValue[0]->ambience_step[i]++;
            return;
        case 3:
            if (SD_DoesExistAnySIFReq()) {
                return;
            }
            SD_AddEeToIopReq((int)(DotrSoundQueueWord)(g_SDBuffer[0] + (i << 12) + 0x101800), g_SDValue[0]->unknown_0014 + 0xc0800, 0x1000);
            g_SDValue[0]->ambience_step[i]++;
            return;
        case 4:
            if (SD_DoesExistAnySIFReq()) {
                return;
            }
            SDRPC_SendCommand(0x5401, g_SDValue[0]->ambience_slot[i], g_SDValue[0]->unknown_0014 + 0xc0800);
            g_SDValue[0]->ambience_step[i]++;
            return;
        case 5:
            if (*(unsigned char *)&g_SDValue[0]->unknown_0054[1] & 8) {
                return;
            }
            g_SDValue[0]->ambience_step[i] = 0;
            (&g_SDValue[0]->unknown_2258)[g_SDValue[0]->ambience_slot[i]] = g_SDValue[0]->ambience_file[i];
            return;
        default:
            g_SDValue[0]->ambience_step[i] = 0;
            break;
        }
    }
    if (!(g_SDValue[0]->unknown_0000 & 0x200000)) {
        g_SDValue[0]->unknown_0000 &= ~0x8000;
        /* MATCHING DEVICE: the explicit return in the last conditional block makes the
         * compiler fill branch delay slots throughout the function with useful instructions
         * instead of nops, and drop the originals of instructions it copies into them. */
        return;
    }
}

#include "dotr/ee/sound_control.h"

void SD_vTransferSEGrpToIopProc(void)
{
    int number;
    char text[28];

    if (!g_SDValue[0]) {
        return;
    }
    if (!(g_SDValue[0]->unknown_0004 & 4)) {
        return;
    }
    switch (g_SDValue[0]->unknown_20b0) {
    case 1:
        if (SDFS_IsUsingNow()) {
            return;
        }
        SD_AddEeToIopReq((int)(DotrSoundControlWord)(g_SDBuffer[0] + 0x101800),
                         g_SDValue[0]->unknown_0014 + 0x40000, 0x80800);
        g_SDValue[0]->unknown_20b0++;
        break;
    case 2:
        if (SD_DoesExistAnySIFReq()) {
            return;
        }
        SDRPC_SendCommand(0x5100, g_SDValue[0]->unknown_20b4, g_SDValue[0]->unknown_0014 + 0x40000);
        g_SDValue[0]->unknown_20b0++;
        break;
    case 3:
        if (((unsigned char *)&g_SDValue[0]->unknown_0054[1])[2]) {
            return;
        }
        g_SDValue[0]->unknown_20b0 = 0;
        g_SDValue[0]->unknown_0000 &= ~0x4000;
        break;
    case 20:
        if (SD_IsAnyJobOnCD()) {
            return;
        }
        g_SDValue[0]->unknown_20b0 = 1;
        break;
    case 30:
        if (SD_IsAnyJobOnCD()) {
            return;
        }
        g_SDValue[0]->unknown_20b0++;
        break;
    case 31:
        if (SDFS_IsUsingNow()) {
            return;
        }
        g_SDValue[0]->unknown_20b8 = *(int *)(g_SDBuffer[0] + 0x101804) + 0x800;
        g_SDValue[0]->unknown_20bc = 0;
        g_SDValue[0]->unknown_20c0 = g_SDValue[0]->unknown_20b8 <= 0x80800 ? g_SDValue[0]->unknown_20b8 : 0x80800;
        SD_AddEeToIopReq((int)(DotrSoundControlWord)(g_SDBuffer[0] + 0x101800),
                         g_SDValue[0]->unknown_0014 + 0x40000, g_SDValue[0]->unknown_20c0);
        g_SDValue[0]->unknown_20b8 -= g_SDValue[0]->unknown_20c0;
        g_SDValue[0]->unknown_20bc += g_SDValue[0]->unknown_20c0;
        g_SDValue[0]->unknown_20b0++;
        break;
    case 32:
        if (SD_DoesExistAnySIFReq()) {
            return;
        }
        SDRPC_SendCommand(0x5102, g_SDValue[0]->unknown_20b4, g_SDValue[0]->unknown_0014 + 0x40000,
                          g_SDValue[0]->unknown_20c0, 0);
        if (g_SDValue[0]->unknown_20b8 > 0) {
            g_SDValue[0]->unknown_20b0++;
        } else {
            g_SDValue[0]->unknown_20b0 = 3;
        }
        break;
    case 33:
        if (((unsigned char *)&g_SDValue[0]->unknown_0054[1])[2]) {
            return;
        }
        g_SDValue[0]->unknown_20c0 = g_SDValue[0]->unknown_20b8 <= 0x80800 ? g_SDValue[0]->unknown_20b8 : 0x80800;
        SD_AddReadToEeReq(g_SDValue[0]->unknown_2070, (int)(DotrSoundControlWord)(g_SDBuffer[0] + 0x101800),
                          g_SDValue[0]->unknown_20bc, g_SDValue[0]->unknown_20c0);
        g_SDValue[0]->unknown_20b0++;
        break;
    case 34:
        if (SD_IsAnyJobOnCD()) {
            return;
        }
        g_SDValue[0]->unknown_20b0++;
        break;
    case 35:
        if (SDFS_IsUsingNow()) {
            return;
        }
        SD_AddEeToIopReq((int)(DotrSoundControlWord)(g_SDBuffer[0] + 0x101800),
                         g_SDValue[0]->unknown_0014 + 0x40000, g_SDValue[0]->unknown_20c0);
        g_SDValue[0]->unknown_20b8 -= g_SDValue[0]->unknown_20c0;
        g_SDValue[0]->unknown_20bc += g_SDValue[0]->unknown_20c0;
        g_SDValue[0]->unknown_20b0++;
        break;
    case 36:
        if (SD_DoesExistAnySIFReq()) {
            return;
        }
        SDRPC_SendCommand(0x5102, g_SDValue[0]->unknown_20b4, g_SDValue[0]->unknown_0014 + 0x40000,
                          g_SDValue[0]->unknown_20c0,
                          g_SDValue[0]->unknown_20bc - g_SDValue[0]->unknown_20c0 - 0x800);
        if (g_SDValue[0]->unknown_20b8 > 0) {
            g_SDValue[0]->unknown_20b0 = 33;
        } else {
            g_SDValue[0]->unknown_20b0 = 3;
        }
        break;
    default:
        g_SDValue[0]->unknown_20b0 = 0;
        if (SD_FetchFromXferQueue(&number, text)) {
            SD_KickSEGroupReadSequence(number, text);
        }
        break;
    }
}

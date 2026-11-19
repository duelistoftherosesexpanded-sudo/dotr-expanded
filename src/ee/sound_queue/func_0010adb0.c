#include "dotr/ee/sound_control.h"


void SD_vAutoDMASequentialWatchDog(void)
{
    int flags;

    if (!(g_SDValue[0]->unknown_0004 & 2)) {
        return;
    }
    flags = g_SDValue[0]->unknown_0000;
    if (!(flags & 0x10)) {
        return;
    }
    if (!(flags & 0x20)) {
        return;
    }
    if (!(flags & 0x40)) {
        return;
    }
    if (!g_SDValue[0]->unknown_01ec) {
        SD_iStopAutoDMA();
    }
    switch (g_SDValue[0]->unknown_01e4) {
    case 100:
        if (!SD_BGMGetVolume()) {
            SD_iStopAutoDMA();
            g_SDValue[0]->unknown_01c8 = 0;
            g_SDValue[0]->unknown_01cc = 0;
            g_SDValue[0]->unknown_01d0 = 0;
            g_SDValue[0]->unknown_01e4++;
        }
        break;
    case 101:
    case 102:
    case 103:
        g_SDValue[0]->unknown_01e4++;
        break;
    case 104:
        if (g_SDValue[0]->unknown_2248 == 9127) {
            g_SDValue[0]->unknown_0000 &= ~0x100;
            g_SDValue[0]->unknown_0000 &= ~0x2000;
            g_SDValue[0]->unknown_0000 &= ~0x200;
            g_SDValue[0]->unknown_01dc = 9127;
            g_SDValue[0]->unknown_01e4 = 0;
        } else {
            g_SDValue[0]->unknown_01e4 = 1;
            g_SDValue[0]->unknown_01dc = g_SDValue[0]->unknown_2248;
        }
        break;
    case 1:
        if (g_SDValue[0]->unknown_01fc & 2) {
            _vLoadBGMFileToEE(g_SDValue[0]->unknown_01dc, 0, 0x90000);
        } else {
            _vLoadBGMFileToEE(g_SDValue[0]->unknown_01dc, 0, 0x100000);
        }
        g_SDValue[0]->unknown_01e4++;
        return;
    case 2:
        if (SD_DoesExistAnyCDReq()) {
            return;
        }
        g_SDValue[0]->unknown_0018 = 0;
        g_SDValue[0]->unknown_01c4 = 0;
        g_SDValue[0]->unknown_01c0 = 0;
        g_SDValue[0]->unknown_01d8 = 0;
        _vDecodeLPCMThenSendToIOP(0);
        g_SDValue[0]->unknown_01e4++;
        return;
    case 3:
        if (SD_DoesExistAnySIFReq()) {
            return;
        }
        _vDecodeLPCMThenSendToIOP(0);
        g_SDValue[0]->unknown_01e4++;
        return;
    case 4:
        if (SD_DoesExistAnySIFReq()) {
            break;
        }
        if (g_SDValue[0]->unknown_01fc & 2) {
            g_SDValue[0]->unknown_01fc &= ~2;
            g_SDValue[0]->unknown_01e4++;
        } else {
            g_SDValue[0]->unknown_01fc = 4;
            g_SDValue[0]->unknown_01e4 = 0;
            g_SDValue[0]->unknown_0000 &= ~0x2000;
        }
        break;
    case 5:
        _vLoadBGMFileToEE(g_SDValue[0]->unknown_01dc, 0x90000, 0x70000);
        SD_iStopAutoDMA();
        SD_iStartAutoDMA();
        g_SDValue[0]->unknown_01e4++;
        return;
    case 6:
        if (SD_DoesExistAnyCDReq()) {
            break;
        }
        g_SDValue[0]->unknown_01e4 = 0;
        g_SDValue[0]->unknown_0000 &= ~0x2000;
        break;
    default:
        g_SDValue[0]->unknown_01e4 = 0;
        break;
    }
    if (g_SDValue[0]->unknown_0000 & 0x100) {
        if ((g_SDValue[0]->unknown_01c4 & 0x1000000) != (g_SDValue[0]->unknown_0018 & 0x1000000)) {
            g_SDValue[0]->unknown_01c4 = g_SDValue[0]->unknown_0018;
            g_SDValue[0]->unknown_01c0 = g_SDValue[0]->unknown_01c4;
            g_SDValue[0]->unknown_01d8 = (g_SDValue[0]->unknown_01c0 & 0x1000000) ? 0 : 1;
            _vDecodeLPCMThenSendToIOP(0);
            if (g_SDValue[0]->unknown_0000 & 0x100) {
                if (g_SDValue[0]->unknown_01c8 == 0 || g_SDValue[0]->unknown_01c8 == 0xa0000) {
                    _vLoadBGMFileToEE(g_SDValue[0]->unknown_01dc, g_SDValue[0]->unknown_01cc, 0x80000);
                }
            }
            g_SDValue[0]->unknown_01c0 = 0;
            /* MATCHING DEVICE: the explicit return in the last conditional block makes the
             * compiler fill branch delay slots throughout the function with useful instructions
             * instead of nops, and drop the originals of instructions it copies into them. */
            return;
        }
    }
}

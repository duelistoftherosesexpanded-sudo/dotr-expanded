#include "dotr/ee/sound_control.h"

/* Search that 0x10b490 contains inline twice; the original helper form is unknown. */
static inline int dotr_sound_control_find_item(int id)
{
    int i;
    for (i = 0; i < 256; i++) {
        if (g_SDValue[0]->items[i].unknown_004 == id) {
            return i;
        }
    }
    return -1;
}
/* The body of 0x10b3c0, which 0x10b490 contains inline with its result unused. */
static inline int dotr_sound_control_start(int id)
{
    if (!(g_SDValue[0]->unknown_0004 & 2)) {
        return 0;
    }
    SD_BGMFadeOut(256);
    g_SDValue[0]->unknown_2248 = id;
    if (dotr_sound_control_find_item(id) < 0 && id != 9127) {
        return 0;
    }
    g_SDValue[0]->unknown_01fc = 1;
    g_SDValue[0]->unknown_01e4 = 100;
    g_SDValue[0]->unknown_0000 |= 0x2000;
    return 1;
}
int SD_iPlayBGMByAutoDMA(int id)
{
    if (!(g_SDValue[0]->unknown_0004 & 2)) {
        return 0;
    }
    if (dotr_sound_control_find_item(id) < 0 && id != 9127) {
        return 0;
    }
    g_SDValue[0]->unknown_2248 = id;
    if (g_SDValue[0]->unknown_01dc == id && (g_SDValue[0]->unknown_0000 & 0x100)) {
        if (g_SDValue[0]->unknown_01ec < 32767) {
            SDRPC_SendCommand(0x5004, 256);
        }
        if (g_SDValue[0]->unknown_0000 & 0x2000) {
            g_SDValue[0]->unknown_0000 &= ~0x2000;
        }
        return 1;
    }
    if (g_SDValue[0]->unknown_01fc == 4 && g_SDValue[0]->unknown_01dc == id) {
        g_SDValue[0]->unknown_0000 |= 0x100;
        g_SDValue[0]->unknown_01fc &= ~4;
        g_SDValue[0]->unknown_01d4 = 0;
        g_SDValue[0]->unknown_2254 = g_SDValue[0]->unknown_01dc;
        SDRPC_SendCommand(0x1020, g_SDValue[0]->unknown_0014, 0x40000, 1);
        return 1;
    }
    dotr_sound_control_start(id);
    g_SDValue[0]->unknown_01fc |= 2;
    return 1;
}

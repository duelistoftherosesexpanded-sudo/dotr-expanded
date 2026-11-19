#include "dotr/ee/sound_control.h"

/* Search that 0x10b3c0 contains inline; the original helper form is unknown. */
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
int SD_iPreparePreBlockOnIOP(int id)
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

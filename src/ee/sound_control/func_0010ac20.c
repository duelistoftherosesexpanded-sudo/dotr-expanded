#include "dotr/ee/sound_control.h"

/* Search contained inline, taking the manager as in the observed code; the
 * original helper form is unknown. */
static inline int dotr_sound_control_find_item_in(struct DotrSoundQueueManager *manager, int id)
{
    int i;
    for (i = 0; i < 256; i++) {
        if (manager->items[i].unknown_004 == id) {
            return i;
        }
    }
    return -1;
}

/* Both position comparisons are written position-first; with the item word
 * first the compiler swaps the two scratch registers of the second one. */
void _vDecodeLPCMThenSendToIOP(int unused)
{
    int index;
    (void)unused;
    func_0017bb60(0);
    if (g_SDValue[0]->unknown_01dc == 9127) {
        SD_BGMFadeOut(256);
        g_SDValue[0]->unknown_2248 = 9127;
        return;
    }
    index = dotr_sound_control_find_item_in(g_SDValue[0], g_SDValue[0]->unknown_01dc);
    if (index == -1) {
        return;
    }
    g_SDValue[0]->unknown_01c8 = _iDecodeLPCM(g_SDValue[0]->unknown_01c8);
    g_SDValue[0]->unknown_01d4 += 0x4000;
    if (g_SDValue[0]->items[index].unknown_00c) {
        if (g_SDValue[0]->unknown_01d4 > g_SDValue[0]->items[index].unknown_00c) {
            g_SDValue[0]->unknown_01d4 -= g_SDValue[0]->items[index].unknown_00c;
            g_SDValue[0]->unknown_01d4 += g_SDValue[0]->items[index].unknown_008;
        }
    } else if (g_SDValue[0]->unknown_01d4 > (g_SDValue[0]->items[index].unknown_000 >> 2)) {
        SD_iStopAutoDMA();
        return;
    }
    SD_AddEeToIopReq((int)(DotrSoundControlWord)(g_SDBuffer[0] + 0x181800),
                  g_SDValue[0]->unknown_0014 + (g_SDValue[0]->unknown_01d8 << 17), 0x20000);
    g_SDValue[0]->unknown_01d8 = 1 - g_SDValue[0]->unknown_01d8;
}

#include "dotr/ee/slot_reel.h"

/* Local names are assumptions (the Japanese build's debug information has none for this
 * function); i counts the reels and j the entries, which is what fixes the loop registers. */
int TaSlot_SetReelData(int Count, unsigned short *wGraveLst, struct jp_9a20 *pSlotOrder)
{
    unsigned short wBuf[120];
    int i;
    int j;
    int Num;

    for (i = 0; i < 120; i++) {
        wBuf[i] = 999;
    }
    Num = 0;
    if (Count < 16) {
        if (Count < 9) {
            /* Pad a short grave list to 8 entries with card 820. */
            for (i = Count; i < 8; i++) {
                wGraveLst[i] = 820;
            }
        }
        for (i = 0; wGraveLst[i] != 999; i++) {
            Num++;
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 21; j++) {
                pSlotOrder->wReelBuf[i][j] = wGraveLst[j];
            }
        }
    } else {
        Num = 16;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < Count; j++) {
                wBuf[j] = wGraveLst[j];
            }
            for (j = 0; j < 16; j++) {
                pSlotOrder->wReelBuf[i][j] = wBuf[j];
            }
        }
    }
    return Num;
}

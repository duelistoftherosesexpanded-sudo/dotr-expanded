#include "dotr/ee/card_save.h"

/* 0x23fbe0. The data views carry the _cardsave suffix. */

void TakuLoad_Name_Set(int side) {
    extern volatile int playerNameOffset_cardsave;
    extern int data_002d19e0_cardsave;
    extern unsigned short StringBlob_cardsave[];
    short i;
    unsigned short *dest;
    int offset;
    unsigned char *src;
    unsigned short v;
    int off2;

    src = (unsigned char *)&data_00299e10_cardsave;
    if (side == 0) {
        i = 0;
        offset = 0;
        while (i < 12) {
            if (i == 0) {
                dest = (unsigned short *)((unsigned char *)&StringBlob_cardsave[playerNameOffset_cardsave] + offset);
                *dest = *(unsigned short *)(src + 0x24);
            }
            v = *(unsigned short *)(src + 0x24);
            dest = (unsigned short *)((unsigned char *)&StringBlob_cardsave[playerNameOffset_cardsave] + offset + 4);
            offset += 2;
            *dest = v;
            src += 2;
            i++;
        }
    } else {
        i = 0;
        off2 = 0;
        while (i < 12) {
            v = *(unsigned short *)(src + 0x24);
            dest = (unsigned short *)((unsigned char *)&StringBlob_cardsave[data_002d19e0_cardsave] + off2);
            off2 += 2;
            *dest = v;
            src += 2;
            i++;
        }
    }
}

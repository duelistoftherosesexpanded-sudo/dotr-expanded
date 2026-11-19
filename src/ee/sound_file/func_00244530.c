#include "dotr/ee/sound_file.h"

/* Under the stream's semaphore (+0x40): clears +0x44, rewrites DMA channel 4's control
 * word with 5 and saves that channel's address, tag address, count and control words in
 * +0x1c..+0x28; waits until bits 4-7 of 0x10002010 are clear; clears DMA channel 3's
 * control word and saves its address, count and control words in +0x2c..+0x34 and the
 * words at 0x10002020 and 0x10002010 in +0x38 and +0x3c. Each control write has the
 * enable bit 0x10000 raised around it between func_001843e0 and func_00184428. Returns 1. */
int viBufStopDMA(struct DotrSoundStream *p)
{
    func_0017b920(p->unknown_40);
    p->unknown_44 = 0;
    func_001843e0();
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) | 0x10000;
    DOTR_SOUND_REG(0x1000b400) = 5;
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) & ~0x10000;
    func_00184428();
    p->unknown_1c[0] = DOTR_SOUND_REG(0x1000b410);
    p->unknown_1c[1] = DOTR_SOUND_REG(0x1000b430);
    p->unknown_1c[2] = DOTR_SOUND_REG(0x1000b420);
    p->unknown_1c[3] = DOTR_SOUND_REG(0x1000b400);
    while (DOTR_SOUND_REG(0x10002010) & 0xf0) {
    }
    func_001843e0();
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) | 0x10000;
    DOTR_SOUND_REG(0x1000b000) = 0;
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) & ~0x10000;
    func_00184428();
    p->unknown_1c[4] = DOTR_SOUND_REG(0x1000b010);
    p->unknown_1c[5] = DOTR_SOUND_REG(0x1000b020);
    p->unknown_1c[6] = DOTR_SOUND_REG(0x1000b000);
    p->unknown_1c[7] = DOTR_SOUND_REG(0x10002020);
    p->unknown_1c[8] = DOTR_SOUND_REG(0x10002010);
    func_0017b900(p->unknown_40);
    return 1;
}

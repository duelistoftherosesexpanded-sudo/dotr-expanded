#include "dotr/ee/sound_file.h"

/* Rewrites DMA channel 4's control word with 5 (enable bit raised around the write),
 * clears that channel's count, address and tag address, and hands the stream's
 * semaphore id (+0x40) to func_0017b8f0. Returns 1. */
int viBufDelete(struct DotrSoundStream *p)
{
    func_001843e0();
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) | 0x10000;
    DOTR_SOUND_REG(0x1000b400) = 5;
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) & ~0x10000;
    func_00184428();
    DOTR_SOUND_REG(0x1000b420) = 0;
    DOTR_SOUND_REG(0x1000b410) = 0;
    DOTR_SOUND_REG(0x1000b430) = 0;
    func_0017b8f0(p->unknown_40);
    return 1;
}

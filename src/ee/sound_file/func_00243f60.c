#include "dotr/ee/sound_file.h"

/* Resets the stream counters (+0x0c, +0x10, +0x14, +0x58, +0x5c = 0; +0x44 = 1), marks
 * every 24-byte slot free (-1, -1, 0, 0), writes one 64-bit DMA tag per 2048-byte block
 * (address in the upper word, 0x30000080 below) and a closing tag that points back at the
 * tag list (0x20000000), then programs DMA channel 4 (count 0, address, tag address,
 * control 5) with the channel's enable bit raised around the control write. Returns 1. */
int viBufReset(struct DotrSoundStream *p)
{
    int i;

    p->unknown_0c = 0;
    p->unknown_10 = 0;
    p->unknown_14 = 0;
    p->unknown_44 = 1;
    p->unknown_58 = 0;
    p->unknown_5c = 0;
    for (i = 0; i < p->unknown_54; i++) {
        DOTR_SOUND_PTR(struct DotrSoundSlot, p->unknown_50)[i].unknown_00 = -1;
        DOTR_SOUND_PTR(struct DotrSoundSlot, p->unknown_50)[i].unknown_08 = -1;
        DOTR_SOUND_PTR(struct DotrSoundSlot, p->unknown_50)[i].unknown_10 = 0;
        DOTR_SOUND_PTR(struct DotrSoundSlot, p->unknown_50)[i].unknown_14 = 0;
    }
    for (i = 0; i < p->unknown_08; i++) {
        DOTR_SOUND_PTR(unsigned long, p->unknown_04)[i * 2] =
            (((unsigned long)(unsigned)(p->unknown_00 + i * 2048) & 0x0fffffff) << 32) | 0x30000080;
    }
    DOTR_SOUND_PTR(unsigned long, p->unknown_04)[i * 2] =
        (((unsigned long)p->unknown_04 & 0x0fffffff) << 32) | 0x20000000;
    DOTR_SOUND_REG(0x1000b420) = 0;
    DOTR_SOUND_REG(0x1000b410) = (unsigned long)p->unknown_00 & 0x0fffffff;
    DOTR_SOUND_REG(0x1000b430) = (unsigned long)p->unknown_04 & 0x0fffffff;
    func_001843e0();
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) | 0x10000;
    DOTR_SOUND_REG(0x1000b400) = 5;
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) & ~0x10000;
    func_00184428();
    return 1;
}

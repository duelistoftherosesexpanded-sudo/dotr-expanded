#include "dotr/ee/sound_file.h"

/* Fills the stream words (+0, +4 with the address moved to 0x2xxxxxxx, +8,
 * +0x18 = c << 11, +0x50, +0x54), stores the result of func_0017b8e0 on a
 * local record with two words set to 1 at +0x40, calls viBufReset and
 * clears the 64-bit total. Returns 1. */
int viBufCreate(struct DotrSoundStream *p, int a, unsigned b, int c, int d, int e)
{
    struct DotrSoundSema sema;
    p->unknown_00 = a;
    p->unknown_04 = (b & 0x0fffffff) | 0x20000000;
    p->unknown_08 = c;
    p->unknown_18 = c << 11;
    p->unknown_50 = d;
    p->unknown_54 = e;
    sema.unknown_8 = 1;
    sema.unknown_4 = 1;
    p->unknown_40 = func_0017b8e0(&sema);
    viBufReset(p);
    p->unknown_48 = 0;
    return 1;
}

#include "dotr/ee/sound_file.h"

/* Between func_0017b920 and func_0017b900 on the word at +0x40, adds n to
 * the word at +0x14 and to the doubleword at +0x48. */
void viBufEndPut(struct DotrSoundStream *p, int n)
{
    func_0017b920(p->unknown_40);
    p->unknown_14 += n;
    p->unknown_48 += n;
    func_0017b900(p->unknown_40);
}

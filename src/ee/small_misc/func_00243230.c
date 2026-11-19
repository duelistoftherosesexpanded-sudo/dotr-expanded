#include "dotr/ee/small_misc.h"

/* Byte-identical to its sibling (0x243230 / 0x243290): calls changeInputVolume(0x7fff),
 * then 0x190810 with seven arguments, then sets the first word to 2. */
void audioDecResume(struct DotrMiscStream *p)
{
    changeInputVolume(0x7fff);
    func_00190810_args7(1, 0x80e0, 0, 19, p->unknown_44, (p->unknown_48 >> 10) << 10, p->unknown_44 + p->unknown_50);
    p->unknown_00 = 2;
}

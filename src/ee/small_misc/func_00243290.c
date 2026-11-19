#include "dotr/ee/small_misc.h"

/* The same bytes as audioDecResume (0x243230): calls changeInputVolume(0x7fff), then
 * func_00190810 with seven arguments, then sets the first word to 2. */
void audioDecStart(struct DotrMiscStream *p)
{
    changeInputVolume(0x7fff);
    func_00190810_args7(1, 0x80e0, 0, 19, p->unknown_44, (p->unknown_48 >> 10) << 10, p->unknown_44 + p->unknown_50);
    p->unknown_00 = 2;
}

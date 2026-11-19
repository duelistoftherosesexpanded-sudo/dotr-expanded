#include "dotr/ee/small_misc.h"

/* Calls viBufReset on the span at +0x48, voBufReset on the object at
 * 0x350600 and decBs0(p), waits until the word at 0x35060c reads
 * zero (re-read every iteration, hence the volatile view) and sets the
 * word at +0xa8 to 3. */
void videoDecMain(struct DotrMiscPlayer *p)
{
    func_00243f60_span(&p->unknown_48);
    voBufReset(&voBuf);
    decBs0(p);
    while (data_0035060c_volatile[0] != 0) {
    }
    p->unknown_a8 = 3;
}

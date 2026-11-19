#include "dotr/ee/small_misc.h"

/* Between func_0017b920 and func_0017b900 on the word at +0x40 (WaitSema / SignalSema, as in
 * viBufEndPut), rounds the word at +0x14 up to a multiple of 2048. */
void viBufFlush(struct DotrMiscSpan *span)
{
    func_0017b920(span->unknown_40);
    span->unknown_14 = ((span->unknown_14 + 2047) >> 11) << 11;
    func_0017b900(span->unknown_40);
}

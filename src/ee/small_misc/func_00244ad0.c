#include "dotr/ee/small_misc.h"

/* Bracketed by 0x17b920/0x17b900 on the word at +0x40: the word at +0x14
 * plus the word at +0x10 times 2048. */
int viBufCount(struct DotrMiscSpan *span)
{
    int result;
    func_0017b920(span->unknown_40);
    result = span->unknown_14 + (span->unknown_10 << 11);
    func_0017b900(span->unknown_40);
    return result;
}

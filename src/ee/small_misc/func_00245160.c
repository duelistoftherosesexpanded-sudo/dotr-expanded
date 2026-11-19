#include "dotr/ee/small_misc.h"

/* Packs two doubleword arguments, the fourth argument less the object word
 * at +0x48 and the fifth argument into a stack record for viBufPutTs (0x244cb0). */
void videoDecPutTs(struct DotrMiscOrigin *object, long a, long b, int c, int d)
{
    struct DotrMiscRequest request;
    request.unknown_00 = a;
    request.unknown_08 = b;
    request.unknown_10 = c - object->unknown_48;
    request.unknown_14 = d;
    viBufPutTs(data_003506c8, &request);
}

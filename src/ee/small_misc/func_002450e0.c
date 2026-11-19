#include "dotr/ee/small_misc.h"

int videoDecEndPut(struct DotrMiscObject *p)
{
    return viBufEndPut(&p->unknown_48);
}

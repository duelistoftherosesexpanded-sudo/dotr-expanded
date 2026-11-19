#include "dotr/ee/small_misc.h"

int videoDecBeginPut(struct DotrMiscObject *p)
{
    return viBufBeginPut(&p->unknown_48);
}

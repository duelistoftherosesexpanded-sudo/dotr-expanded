#include "dotr/ee/small_misc.h"

int videoDecDelete(struct DotrMiscObject *p)
{
    viBufDelete(&p->unknown_48);
    func_0018ed10(p);
    return 1;
}

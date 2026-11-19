#include "dotr/ee/small_misc.h"

int audioDecDelete(struct DotrMiscObject *p)
{
    func_00182a28(p->unknown_44);
    func_00182a28(p->unknown_58);
    changeMasterVolume(0);
    return 1;
}

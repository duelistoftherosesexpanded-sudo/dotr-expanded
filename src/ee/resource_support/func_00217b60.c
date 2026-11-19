#include "dotr/ee/resource_support.h"

int SzMem_StrDup(const char *name)
{
    int copy;
    if (!name) {
        return 0;
    }
    copy = func_00197990(16, func_0019bef0(name) + 1);
    if (!copy) {
        return 0;
    }
    func_0019bdd8((char *)(DotrResourceSubmitWord)copy, name);
    return copy;
}

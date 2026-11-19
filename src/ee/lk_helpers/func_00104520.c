#include "dotr/ee/lk_helpers.h"

int LkCheckFlag(int flags, int bit)
{
    return !(flags & (1 << bit)) ? 0 : 1;
}

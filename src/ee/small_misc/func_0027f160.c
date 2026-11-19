#include "dotr/ee/small_misc.h"

int Ta_LkCheckFlag(int a,int b)
{
    return (a&(1<<b))?1:0;
}

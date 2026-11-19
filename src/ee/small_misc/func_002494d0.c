#include "dotr/ee/small_misc.h"

int TaEnc_GetRandomElf(void)
{
    switch (func_0019a5c8() % 6) {
    case 0:
        return 35;
    case 1:
        return 79;
    case 2:
        return 44;
    case 3:
        return 92;
    case 4:
        return 379;
    case 5:
        break;
    }
    return 394;
}

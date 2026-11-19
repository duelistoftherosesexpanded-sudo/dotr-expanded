#include "dotr/ee/small_misc.h"

int Ta_GetEOwnerSide(int side, int mode)
{
    switch (mode) {
    case 1: return side;
    case 2: return side == 0 ? 1 : 0;
    default: return -1;
    }
}

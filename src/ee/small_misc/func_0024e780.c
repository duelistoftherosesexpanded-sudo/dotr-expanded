#include "dotr/ee/small_misc.h"

int Ta_GetUnitNum(int side, unsigned char kind)
{
    int count;
    int lo;
    int i;
    int hi;
    count = 0;
    switch (kind) {
    case 0:
        lo = 0;
        hi = 11;
        break;
    case 1:
        lo = 0;
        hi = 6;
        break;
    case 2:
        lo = 6;
        hi = 11;
        break;
    case 3:
        lo = 1;
        hi = 11;
        break;
    case 4:
        lo = 1;
        hi = 6;
        break;
    default:
        return -1;
    }
    for (i = lo; i < hi; i++) {
        if (func_001df540_word(side, i)) {
            count++;
        }
    }
    return count;
}

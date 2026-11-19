#include "dotr/ee/low_leaves.h"
int Ta_GetLDChgLand(int Race)
{
    int r;
    switch (Race) {
    case 1:
    case 7:
        r = 5;
        break;
    case 9:
        r = 0;
        break;
    case 10:
        r = 1;
        break;
    case 16:
        r = 4;
        break;
    case 20:
        r = 9;
        break;
    default:
        r = 7;
        break;
    }
    return r;
}

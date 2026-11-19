#include "dotr/ee/unsigned_convert.h"

float _f_ulltof(unsigned long x)
{
    float r;
    if ((long)x > 0) {
        return func_001a1f18_uconv(x);
    }
    r = func_001a1f18_uconv((x >> 1) | (x & 1));
    return r + r;
}

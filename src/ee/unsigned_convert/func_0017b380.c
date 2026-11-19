#include "dotr/ee/unsigned_convert.h"

double _d_utod(unsigned long x)
{
    double r;
    x <<= 32;
    x >>= 32;
    if ((long)x > 0) {
        return func_001a1e70_uconv(x);
    }
    r = func_001a1e70_uconv((x >> 1) | (x & 1));
    return func_001a2858_uconv(r, r);
}

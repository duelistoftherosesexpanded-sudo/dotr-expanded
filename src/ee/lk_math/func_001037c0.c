#include "dotr/ee/lk_math.h"

int LkSetAngleW2S(struct DotrLkObject *obj, void *p1, void *p2)
{
#ifdef __MWERKS__
    long pad; /* MATCHING DEVICE: frame filler, unused */
#endif
    struct DotrLkScreen a;
    struct DotrLkScreen b;
#ifdef __MWERKS__
    int pad2; /* MATCHING DEVICE: frame filler, unused */
#endif
    float w;
    func_00205440_lk(p1, obj->matrix, &a, &w);
    func_00205440_lk(p2, obj->matrix, &b, &w);
    return func_001a22c0(func_001a6c18((float)((a.x - b.x) >> 4), (float)((a.y - b.y) >> 4)) / 6.2831855f * 65536.0f) % 0x10000;
}

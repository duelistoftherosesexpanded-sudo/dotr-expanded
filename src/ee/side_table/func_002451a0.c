#include "dotr/ee/small_misc.h"

int videoDecFlush(struct DotrMiscPlayer *p)
{
    int a;
    int c;
    struct DotrMiscFloatWord f = data_0029b300[0];
    int b;
    int d;

    func_002440f0_five(&p->unknown_48, &a, &b, &c, &d);
    if (b + d < 4)
        return 0;
    func_002441f0_count(data_003506c8, cpy2area((char *)(DotrMiscAddress)((a & 0x0fffffff) | 0x20000000), b,
                                                     (char *)(DotrMiscAddress)((c & 0x0fffffff) | 0x20000000), d,
                                                     (char *)&f, 4, 0, 0));
    viBufFlush(&p->unknown_48);
    if (p->unknown_a8 == 0)
        p->unknown_a8 = 2;
    return 1;
}

#include "dotr/ee/g_trans.h"

int GRotTransPers_3(struct DotrGVec *v0, struct DotrGVec *v1, struct DotrGVec *v2, int a, int b, int c)
{
    struct DotrGVec v[3];
    v[0] = *v0;
    v[1] = *v1;
    v[2] = *v2;
    return func_002058d0_g(v, data_0030ff00_g, a, b, c);
}

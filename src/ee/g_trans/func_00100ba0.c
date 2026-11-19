#include "dotr/ee/g_trans.h"

int GRotTransPers_4(struct DotrGVec *v0, struct DotrGVec *v1, struct DotrGVec *v2, struct DotrGVec *v3, int a, int b, int c)
{
    struct DotrGVec v[4];
    v[0] = *v0;
    v[1] = *v1;
    v[2] = *v2;
    v[3] = *v3;
    return func_002058d0_g(v, data_0030ff00_g, a, b, c);
}

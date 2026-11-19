#include "dotr/ee/g_trans.h"

float GCalc_Trans(int a, int b, int n)
{
    return 1.0f - func_001a6c48_g(func_001a6b70_g(6.2831855f * ((float)(((b - a) << 16) / (b * 5)) / 65536.0f)) / func_001a6b70_g(1.2566179f), (float)n);
}

#include "dotr/ee/small_misc.h"

int SzEffect_Entry(int a,int b,int c,int d,int e)
{
    return SzEffect_EntryCallback(a,b,c,d,e,0,0);
}

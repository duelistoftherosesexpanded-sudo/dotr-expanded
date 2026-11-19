#include "dotr/ee/tick_bytes.h"

void ShFx_initPartsTimer(unsigned char *p, int a, int b)
{
    p[2] = a;
    p[0] = a;
    p[1] = b;
}

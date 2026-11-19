#include "dotr/ee/tick_bytes.h"

int ShFx_GetTempIndex(unsigned char *p, int stride, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        if (p[i * stride] == 0) {
            return i;
        }
    }
    return -1;
}

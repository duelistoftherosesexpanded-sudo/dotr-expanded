#include "dotr/ee/tick_bytes.h"

int ShFx_isTimer(unsigned char *p)
{
    if (p[0] == 0) {
        return 0;
    }
    if (p[1] != 0) {
        p[1]--;
        return 0;
    }
    p[0]--;
    return 1;
}

#include "dotr/ee/lk_frames.h"

int LkIncrSHORT(short *value, short limit, short step)
{
    if (*value <= limit - step) {
        *value = *value + step;
        return 1;
    }
    *value = limit;
    return 0;
}

#include "dotr/ee/lk_helpers.h"

int LkIncrFLOAT(float *value, float limit, float step)
{
    float current;

    current = *value;
    if (current <= limit - step) {
        *value = current + step;
        return 1;
    }
    *value = limit;
    return 0;
}

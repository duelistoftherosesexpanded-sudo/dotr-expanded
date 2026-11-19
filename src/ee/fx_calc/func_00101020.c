#include "dotr/ee/fx_calc.h"

float GCalc_Increase(float value, float limit)
{
    float ratio;

    ratio = 1.0f;
    if (value < limit) {
        ratio = value / limit;
    }
    return ratio;
}

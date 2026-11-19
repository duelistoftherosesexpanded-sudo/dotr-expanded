#include "dotr/ee/fx_calc.h"

float GCalc_Decrease(float value, float base, float limit)
{
    float ratio;

    ratio = 1.0f;
    if (!(value <= limit)) {
        ratio = (value - base) / (limit - base);
    }
    return ratio;
}

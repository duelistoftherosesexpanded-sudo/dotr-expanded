#include "dotr/ee/fx_calc.h"

int GWithin_LimitsI(int value, int low, int high)
{
    if (value < low) {
        value = low;
    } else if (high < value) {
        value = high;
    }
    return value;
}

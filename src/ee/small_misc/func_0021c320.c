#include "dotr/ee/small_misc.h"

float func(float a, float b, float c, float d, float e)
{
    return 3.0f * ((e * (b - a)) / d + (d * (c - b)) / e);
}

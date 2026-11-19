#include "dotr/ee/fx_calc.h"

float ShFx_GetDrawSize(float size, float depth)
{
    return (size / (depth / 256.0f)) * (SzView_GetScreenDistance(0) / 800.0f);
}

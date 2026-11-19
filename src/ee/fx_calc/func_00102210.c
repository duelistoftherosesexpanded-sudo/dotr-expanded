#include "dotr/ee/fx_calc.h"

void ShFx_GetTexWidth(unsigned int kind, float *width, float *height)
{
    switch (kind) {
    case 0:
    case 4:
        *width = 128.0f;
        *height = 64.0f;
        break;
    case 1:
        *width = 64.0f;
        *height = 128.0f;
        break;
    case 2:
    case 5:
    case 8:
        *width = *height = 128.0f;
        break;
    case 3:
        *width = 256.0f;
        *height = 64.0f;
        break;
    case 6:
        *width = 32.0f;
        *height = 128.0f;
        break;
    case 7:
        *width = 128.0f;
        *height = 64.0f;
        break;
    case 9:
        *width = *height = 64.0f;
        break;
    default:
        *width = *height = 0.0f;
        break;
    }
}

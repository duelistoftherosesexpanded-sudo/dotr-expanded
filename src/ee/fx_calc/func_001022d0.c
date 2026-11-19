#include "dotr/ee/fx_calc.h"

int ShFx_GetAnmID(unsigned int kind, int frame)
{
    switch (kind) {
    case 0:
    case 1:
    case 3:
        return frame & 7;
    case 4:
        return frame & 1;
    case 2:
        return frame & 15;
    case 5:
        return frame & 3;
    case 6:
    case 7:
    case 8:
    case 9:
        break;
    }
    return 0;
}

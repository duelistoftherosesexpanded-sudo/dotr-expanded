#include "dotr/ee/draw_state.h"

long ExGetAlphaVal(int index)
{
    if (index < 0 || index >= 4) {
        return 0;
    }
    return gExPrim_aqwAlpha[index];
}

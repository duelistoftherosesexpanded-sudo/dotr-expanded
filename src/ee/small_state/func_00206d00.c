#include "dotr/ee/small_state.h"

int Ex3d_ViewClip(int a, int b, int c, int d, int e)
{
    return Ex3d_Clipping(6, gEx3d_aPlane, a, b, c, d, e);
}

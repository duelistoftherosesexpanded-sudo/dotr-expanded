#include "dotr/ee/draw_state.h"

void SzModel_ShotTextureWithFunc(int modelId, int b, int c, int d, int e, int f)
{
    RhBase_SetVifPacketArea(SzModel_ShotTextureWithFunc_Pipe(RhBase_GetVifPacketArea(), modelId, b, c, d, e, f));
}

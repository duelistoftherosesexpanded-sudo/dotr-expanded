#include "dotr/ee/draw_state.h"

void SzModel_DrawMotionBlur(int a)
{
    RhBase_SetVifPacketArea(SzModel_DrawMotionBlur_Pipe(RhBase_GetVifPacketArea(), a));
}

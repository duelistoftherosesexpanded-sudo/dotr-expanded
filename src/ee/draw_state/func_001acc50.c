#include "dotr/ee/draw_state.h"

void lSzGame_DrawTitle(void)
{
    RhBase_SetVifPacketArea(lSzGame_DrawTitle_Pipe(RhBase_GetVifPacketArea()));
}

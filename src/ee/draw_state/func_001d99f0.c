#include "dotr/ee/draw_state.h"

void SzDuel_StartCardSession(void)
{
    RhBase_SetVifPacketArea(SzDuel_StartCardSession_Pipe(RhBase_GetVifPacketArea()));
}

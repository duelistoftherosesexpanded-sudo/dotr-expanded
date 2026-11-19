#include "dotr/ee/draw_state.h"

void SzDuel_FinishCardSession(void)
{
    RhBase_SetVifPacketArea(SzDuel_FinishCardSession_Pipe(RhBase_GetVifPacketArea()));
}

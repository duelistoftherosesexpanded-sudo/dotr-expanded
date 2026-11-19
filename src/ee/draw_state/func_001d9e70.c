#include "dotr/ee/draw_state.h"

struct DotrDrawPacket *SzDuel_FinishCardSessionX_Pipe(struct DotrDrawPacket *packet)
{
    RhBase_FreeRenderFrame();
    packet = ExClampMode_Pipe(packet, ExGetRegVal(0, 4));
    SzMsg_Free(0);
    SzMsg_Free(1);
    SzMsg_Free(2);
    return packet;
}

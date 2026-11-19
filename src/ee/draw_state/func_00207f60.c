#include "dotr/ee/draw_state.h"

DotrDrawWord ExSetAnchor(void)
{
    DotrDrawWord cursor = RhBase_GetVifPacketArea();
    RhBase_SetVifPacketArea((DotrDrawWord)ExSetAnchor_Pipe((struct DotrDrawBlock *)cursor));
    return cursor;
}

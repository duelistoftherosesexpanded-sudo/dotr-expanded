#include "dotr/ee/draw_state.h"

/* Calls RhBase_FreeRenderFrame, advances the cursor through RhBase_SetZBufferMode(cursor, 0)
 * and ExClampMode_Pipe with the value of ExGetRegVal(0, 4), then calls
 * SzMsg_Free for 0, 1 and 2. Returns the advanced cursor. */
DotrDrawWord SzDuel_FinishCardSession_Pipe(DotrDrawWord cursor)
{
    RhBase_FreeRenderFrame();
    cursor = RhBase_SetZBufferMode(cursor, 0);
    cursor = (DotrDrawWord)ExClampMode_Pipe((struct DotrDrawPacket *)cursor, ExGetRegVal(0, 4));
    SzMsg_Free(0);
    SzMsg_Free(1);
    SzMsg_Free(2);
    return cursor;
}

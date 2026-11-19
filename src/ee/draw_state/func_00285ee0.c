#include "dotr/ee/draw_state.h"

/* MATCHING DEVICE: `#pragma schedule off` around the function leaves every call and return
 * delay slot empty, as the target has them; the original build flags are not known. */
#ifdef __MWERKS__
#pragma schedule off
#endif
void ShSlot_Draw(void)
{
    DotrDrawWord cursor = RhBase_GetVifPacketArea();
    cursor = ShSlot_Draw_Pipe(cursor);
    RhBase_SetVifPacketArea(cursor);
}
#ifdef __MWERKS__
#pragma schedule on
#endif

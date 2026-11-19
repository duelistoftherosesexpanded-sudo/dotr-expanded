#include "dotr/ee/draw_state.h"

void TkCamp_Draw(void)
{
    if (Move_END_flg[0] == 0) {
        RhBase_SetVifPacketArea(TkCamp_Draw_Pipe(RhBase_GetVifPacketArea()));
    }
}

#include "dotr/ee/small_state.h"
#include "dotr/ee/draw_state.h"

void ExInitPrim(void)
{
    RhGeom_PushMatrix(0);
    ExClearOt((DotrPktCtx *)gMyOT, (DotrPktEnt *)0x1e08000, 12);
}

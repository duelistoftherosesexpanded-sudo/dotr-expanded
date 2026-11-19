#include "dotr/ee/small_state.h"
#include "dotr/ee/draw_state.h"

void ExTermPrim(void)
{
    ExBindPrim((DotrPktCtx *)gMyOT);
    RhGeom_PopMatrix();
}

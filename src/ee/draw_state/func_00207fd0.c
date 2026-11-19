#include "dotr/ee/draw_state.h"

void ExSortAnchor(DotrDrawNode *node, DotrDrawChain *chain, unsigned int index)
{
    RhBase_SetVifPacketArea((DotrDrawWord)ExSortAnchor_Pipe((unsigned char *)RhBase_GetVifPacketArea(), node, chain, index));
}

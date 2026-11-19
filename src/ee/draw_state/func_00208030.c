#include "dotr/ee/draw_state.h"

void *ExSortAnchor_Pipe(unsigned char *packet, DotrDrawNode *node, DotrDrawChain *chain, unsigned int index)
{
    unsigned int last = (1 << chain->shift) - 2;
    if (last < index) { index = last; }
    node->next = (*(DotrDrawSlot *)((char *)chain->slots + index * 8)).head;
    (*(DotrDrawSlot *)((char *)chain->slots + index * 8)).head = node;
    ((long long *)packet)[1] = 0;
    ((long long *)packet)[0] = 0;
    packet[3] = 96;
    return packet + 16;
}

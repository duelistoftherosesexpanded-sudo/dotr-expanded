#include "dotr/ee/draw_state.h"

struct DotrDrawBlock *ExSetAnchor_Pipe(struct DotrDrawBlock *block)
{
    block->unknown_words[3] = 0;
    block->unknown_words[2] = 0;
    block->unknown_words[1] = 0;
    block->unknown_words[0] = 0;
    ((unsigned char *)block)[3] = 80;
    ((unsigned char *)block)[19] = 32;
    return block + 1;
}

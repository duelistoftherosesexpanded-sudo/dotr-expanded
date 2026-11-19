#include "dotr/ee/packed_item.h"

/* MATCHING DEVICE: the comparison goes through this inline helper with a byte result, which
 * keeps the original's XOR operand order. */
static inline unsigned char same_word(int stored)
{
    int actual = SzBattle_InqEntry(0);
    unsigned char result = actual == stored;
    return result;
}

int SzEffect_InqDuelResult(void)
{
    int stored = data_0031be9c[0];
    return SzEffect_GetPlayMotion(same_word(stored) ? 0 : 1) == 4 ? 1 : 0;
}

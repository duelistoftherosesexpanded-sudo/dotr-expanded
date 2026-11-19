#include "dotr/ee/state_grid.h"

void SzBattle_Initialize(void)
{
    struct DotrStateTriple *p = (struct DotrStateTriple *)gSzBattle_bl;
    int i;
    RhMem_FillMemory((struct DotrStateTriple *)gSzBattle_bl, 0, 12);
    for (i = 0; i < 2; i++) {
        p->unknown_words[i] = -1;
    }
    p->unknown_08 = 0;
}

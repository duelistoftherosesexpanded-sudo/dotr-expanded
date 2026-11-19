#include "dotr/ee/small_state.h"

int *SzPad_Term(void)
{
    return RhMem_FillMemory4((int *)&gSzPad_pl, 0, 176);
}

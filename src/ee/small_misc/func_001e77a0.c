#include "dotr/ee/small_misc.h"

void SzBattle_Term(void)
{
    int i;
    for (i = 0; i < 2; i++) {
        SzBattle_Return(i, 0);
    }
}

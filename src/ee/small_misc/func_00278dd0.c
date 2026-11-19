#include "dotr/ee/small_misc.h"

void TaLogic_CopyArray(unsigned char *source, unsigned char *destination)
{
    int i;
    for (i = 0; i < 5; i++) {
        destination[i] = source[i];
    }
}

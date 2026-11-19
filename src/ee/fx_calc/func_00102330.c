#include "dotr/ee/fx_calc.h"

/* The output pointer is walked through a local copy declared before the
 * counter; walking the parameter itself allocates the saved registers
 * differently from the original. */
void ShFx_MakeRnd(unsigned char *out, int count)
{
    unsigned char *p = out;
    int i;

    for (i = 0; i < count; i++) {
        *p = func_0019a5c8() >> 4;
        p++;
    }
}

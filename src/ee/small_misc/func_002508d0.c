#include "dotr/ee/small_misc.h"

/* Clamp to 0..9999. `> 9999` selects the observed scratch register; the
 * equivalent `>= 10000` does not. */
int GetParCap(int stat)
{
    if (stat < 0) {
        return 0;
    }
    return stat > 9999 ? 9999 : stat;
}

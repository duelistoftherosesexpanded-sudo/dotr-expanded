#include "dotr/ee/small_misc.h"

/* compMode 0 never, 1 when the two sides are equal, 2 when `sideA` is the
 * 0/1 complement of `sideB` (opposite sides), 3 always. */
int Ta_SideMatch(int sideA, int sideB, int compMode)
{
    switch (compMode) {
    case 0:
        return 0;
    case 1:
        if (sideA == sideB) {
            return 1;
        }
        return 0;
    case 2:
        if (sideA == (sideB == 0 ? 1 : 0)) {
            return 1;
        }
        return 0;
    case 3:
        return 1;
    }
    return 0;
}

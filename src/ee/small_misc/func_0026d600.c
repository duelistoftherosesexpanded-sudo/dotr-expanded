#include "dotr/ee/small_misc.h"

/* Picks a value from Ta_GetUnitGravityLoc (variant 0, else variant 1 if variant 0 is already
 * in the 22-word table); -1 on failure. */
int Ta_RequestGravityReserve(int a, int b, int c, int *table)
{
    int v = Ta_GetUnitGravityLoc(a, b, c, 0);
    int i;
    int j;
    if (v >= 0) {
        for (i = 0; i < 22; i++) {
            if (v == table[i]) {
                v = Ta_GetUnitGravityLoc(a, b, c, 1);
                if (v >= 0) {
                    for (j = 0; j < 22; j++) {
                        if (v == table[j]) {
                            return -1;
                        }
                    }
                    return v;
                }
                return -1;
            }
        }
        return v;
    }
    return -1;
}

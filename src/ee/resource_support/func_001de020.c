#include "dotr/ee/resource_support.h"

/* Nested conditional expressions retain the branch after the final mapping;
 * an if/else chain with the same results drops it. */
/* Magic and power-up (1, 2) give 1, the two trap kinds (3, 4) give 2, ritual (5) gives 3;
 * monsters (0) and anything else pass through. */
int SzDuel_Kind2Type(int kind)
{
    return kind == 1 ? 1
        : kind == 2 ? 1
        : kind == 3 ? 2
        : kind == 4 ? 2
        : kind == 5 ? 3
        : kind;
}

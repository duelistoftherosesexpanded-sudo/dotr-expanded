#include "dotr/ee/small_misc.h"

/* Tests whether objKind is acceptable for the kind (low 5 bits) reported by
 * SzDuel_GetUnitConstKind(side, 0): kind 8 wants 2, 11 wants 16, 17 wants 5 or 6, 19 wants 0. */
int Ta_IsLD_CurseObj(int side, int objKind)
{
    switch (SzDuel_GetUnitConstKind(side, 0) & 0x1f) {
    case 8:
        if (objKind == 2) return 1;
        return 0;
    case 11:
        if (objKind == 16) return 1;
        return 0;
    case 17:
        return (unsigned)(objKind - 5) < 2 ? 1 : 0;
    case 19:
        return objKind == 0 ? 1 : 0;
    }
    return 0;
}

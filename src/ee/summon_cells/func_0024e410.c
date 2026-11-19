#include "dotr/ee/summon_cells.h"

/* MATCHING DEVICE: unsigned locals, side 0 before side 1, no default cases, the nested
 * checks of the two diagonal cases and the repeated trailing returns. */
int Ta_GetSommonPiecePreference(int side, int direction)
{
    unsigned int unitLoc;
    unsigned int n;
    unsigned int m;

    unitLoc = SzDuel_GetUnitLoc(side, 0);
    switch (side) {
    case 0:
        switch (direction) {
        case 0: /* right */
            return (((unitLoc >> 8) - 1) << 8) | (unitLoc & 0xff);
        case 1: /* left */
            n = (unitLoc >> 8) + 1;
            if (n < 7) {
                return (n << 8) | (unitLoc & 0xff);
            }
            return -1;
        case 2: /* down */
            n = (unitLoc & 0xff) + 1;
            if (n < 7) {
                return ((unitLoc >> 8) << 8) | n;
            }
            return -1;
        case 3: /* up */
            return ((unitLoc >> 8) << 8) | ((unitLoc & 0xff) - 1);
        case 4: /* down-right */
            n = (unitLoc & 0xff) + 1;
            if (n < 7) {
                return (((unitLoc >> 8) - 1) << 8) | n;
            }
            return -1;
        case 5: /* up-right */
            return (((unitLoc >> 8) - 1) << 8) | ((unitLoc & 0xff) - 1);
        case 6: /* down-left */
            n = (unitLoc & 0xff) + 1;
            if (n < 7) {
                m = (unitLoc >> 8) + 1;
                if (m < 7) {
                    return (m << 8) | n;
                }
            }
            return -1;
        case 7: /* up-left */
            n = (unitLoc >> 8) + 1;
            if (n < 7) {
                return (n << 8) | ((unitLoc & 0xff) - 1);
            }
            return -1;
        }
        return -1;
    case 1:
        switch (direction) {
        case 0: /* right */
            n = (unitLoc >> 8) + 1;
            if (n < 7) {
                return (n << 8) | (unitLoc & 0xff);
            }
            return -1;
        case 1: /* left */
            return (((unitLoc >> 8) - 1) << 8) | (unitLoc & 0xff);
        case 2: /* down */
            return ((unitLoc >> 8) << 8) | ((unitLoc & 0xff) - 1);
        case 3: /* up */
            n = (unitLoc & 0xff) + 1;
            if (n < 7) {
                return ((unitLoc >> 8) << 8) | n;
            }
            return -1;
        case 4: /* down-right */
            n = (unitLoc >> 8) + 1;
            if (n < 7) {
                return (n << 8) | ((unitLoc & 0xff) - 1);
            }
            return -1;
        case 5: /* up-right */
            n = (unitLoc & 0xff) + 1;
            if (n < 7) {
                m = (unitLoc >> 8) + 1;
                if (m < 7) {
                    return (m << 8) | n;
                }
            }
            return -1;
        case 6: /* down-left */
            return (((unitLoc >> 8) - 1) << 8) | ((unitLoc & 0xff) - 1);
        case 7: /* up-left */
            n = (unitLoc & 0xff) + 1;
            if (n < 7) {
                return (((unitLoc >> 8) - 1) << 8) | n;
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

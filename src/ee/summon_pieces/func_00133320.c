#include "dotr/ee/summon_pieces.h"

/* TaLogic008_GetSummonPiece: -1 for a null block. When the top three bits of the block byte
 * +0x3e are 0 and `packed` is not negative: among the eight preferred places of `side`
 * (fields other than 8, and other than 9 with Ta_IsVirus_Chk3 == 1, no unit), the one with
 * the lowest route cost below 255 (TaLogic_GetInvadeRoot2 with mode 4 when TaLogic_IsUnitWin4
 * answers above 0, else TaLogic_GetRoot2 with 4) whose list is non-empty, else -1. Otherwise
 * the free place (no unit id) nearest to TaLogic008_GetNextInvadePoint's place (two-axis
 * distance below 14), else -1. */
int TaLogic008_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *block, int packed)
{
    int bestCell;
    int bestCost;
    struct DotrFreeNibbles path[12];
    int i;
    struct DotrFreeNibbles *p;

    (void)kind; /* not read */
    if (block == 0) {
        return -1;
    }
    p = path;
    i = 0;
    do {
        p->low = p->high = 15;
        p++;
        i++;
    } while (i < 12);

    if ((unsigned char)(block->kind >> 5) == 0 && packed >= 0) {
        int cell;
        int y;
        int x;
        int index;
        int targetY;
        int targetX;
        int target;
        int cost;

        bestCost = 255;
        bestCell = 0xFFFF;
        target = SzDuel_GetUnitLoc(packed >> 8, packed & 0xFF);
        index = 0;
        targetY = target >> 8;
        targetX = target & 0xFF;
        do {
            cell = Ta_GetSommonPiecePreference(side, index);
            if (cell >= 0) {
                x = cell & 0xFF;
                y = cell >> 8;
                if (SzDuel_GetField(x, y) != 8 &&
                    (SzDuel_GetField(x, y) != 9 || func_0024fe40_free(block, x, y) != 1) &&
                    SzDuel_GetMapUnit(x, y) == 0) {
                    if (func_00278510_free(block, packed, TaLogic_GetWarnHideLvl__00134ae0(side), targetX, targetY, 0) > 0) {
                        cost = func_0027a130_piece(block, path, x, y, targetX, targetY, packed, 0, 4,
                                                   TaLogic_GetWarnHideLvl__00134ae0(side) & 0xFFFF);
                    } else {
                        cost = func_002792c0_free(block, path, x, y, targetX, targetY, packed, 4);
                    }
                    if (func_0027afc0_free(path) > 0 && cost < bestCost) {
                        bestCost = cost;
                        bestCell = cell;
                    }
                }
            }
            index++;
        } while (index < 8);
        if (bestCell != 0xFFFF) {
            return bestCell;
        } else {
            bestCell = -1;
            return bestCell;
        }
    } else {
        int maxY;
        int maxX;
        int y;
        int x;
        int index;
        int bestDistance;
        int cell;
        int target;
        int targetY;
        int distance;
        int minY;
        int minX;

        bestDistance = 14;
        packed = -1;
        index = 0;
        do {
            cell = Ta_GetSommonPiecePreference(side, index);
            if (cell >= 0) {
                x = cell & 0xFF;
                y = cell >> 8;
                if (SzDuel_GetField(x, y) != 8 &&
                    (SzDuel_GetField(x, y) != 9 || func_0024fe40_free(block, x, y) != 1) &&
                    SzDuel_GetMapUnitID(x, y) < 0) {
                    target = func_00131f80_piece(block, x, y);
                    targetY = target >> 8;
                    minY = y < targetY ? y : targetY;
                    if (targetY < y) {
                        maxY = y;
                    } else {
                        maxY = targetY;
                    }
                    minX = x < (target & 0xFF) ? x : (target & 0xFF);
                    if ((target & 0xFF) < x) {
                        maxX = x;
                    } else {
                        maxX = target & 0xFF;
                    }
                    distance = (maxX - minX) + (maxY - minY);
                    if (distance < bestDistance && distance >= 0) {
                        packed = cell;
                        bestDistance = distance;
                    }
                }
            }
            index++;
        } while (index < 8);
        packed = bestDistance < 14 ? packed : -1;
    }
    return packed;
}

#include "dotr/ee/logic010_summon.h"

/* Picks the place for a summoned piece among the eight preferred places of `side`.
 * Block mode 0 (top three bits of the byte at +0x3e): with a unit position in `packed`, the
 * place with the lowest route cost that leaves a non-empty list; otherwise, or when none
 * qualifies, the free place nearest (two-axis distance below 14) to TaLogic010_GetEmptyLine's
 * place. Any other mode: the last free place in rows 0-7. Returns -1 when nothing fits.
 * The second stage keeps its result in the parameter `packed`. */
int TaLogic010_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *block, int packed)
{
    int bestPosition;
    int bestScore;
    struct DotrFreeNibbles path[12];
    int transformedY;
    int transformedX;
    int value;
    unsigned char mode;

    (void)kind; /* not read */
    if (block == 0) {
        return -1;
    }
    {
    int initIndex;
    struct DotrFreeNibbles *p;
    p = path;
    initIndex = 0;
    do {
        p->low = p->high = 15;
        p++;
        initIndex++;
    } while (initIndex < 12);
    }
    mode = *((unsigned char *)block + 0x3E) >> 5;
    if (mode == 0) {
        if (packed >= 0) {
            int candidate;
            int candidateY;
            int candidateX;
            int index;
            int score;
            bestScore = 255;
            bestPosition = 65535;
            value = SzDuel_GetUnitLoc(packed >> 8, packed & 255);
            index = 0;
            transformedY = value >> 8;
            transformedX = value & 255;
            do {
                candidate = Ta_GetSommonPiecePreference(side, index);
                if (candidate >= 0) {
                    candidateX = candidate & 255;
                    candidateY = candidate >> 8;
                    if (SzDuel_GetField(candidateX, candidateY) != 8 &&
                        (SzDuel_GetField(candidateX, candidateY) != 9 ||
                         func_0024fe40_free(block, candidateX, candidateY) != 1) &&
                        SzDuel_GetMapUnit(candidateX, candidateY) == 0) {
                        if (func_00278510_free(block, packed, TaLogic_GetWarnHideLvl__0013da70(side),
                                               transformedX, transformedY, 0) > 0) {
                            score = func_0027a130_free(block, path,
                                                       candidateX, candidateY,
                                                       transformedX, transformedY,
                                                       packed, 0, 2,
                                                       TaLogic_GetWarnHideLvl__0013da70(side));
                        } else {
                            score = func_002792c0_free(block, path,
                                                       candidateX, candidateY,
                                                       transformedX, transformedY,
                                                       packed, 2);
                        }
                        if (func_0027afc0_free(path) > 0 && score < bestScore) {
                            bestScore = score;
                            bestPosition = candidate;
                        }
                    }
                }
                index++;
            } while (index < 8);
            if (bestPosition != 65535) {
                return bestPosition;
            }
        }
        {
        int maxY;
        int maxX;
        int y;
        int x;
        int i;
        int bestDistance;
        int position;
        int minY;
        int minX;
        bestDistance = 14;
        packed = 0;
        i = 0;
        do {
            position = Ta_GetSommonPiecePreference(side, i);
            if (position >= 0) {
                x = position & 255;
                y = position >> 8;
                if (SzDuel_GetField(x, y) != 8 &&
                    (SzDuel_GetField(x, y) != 9 || func_0024fe40_free(block, x, y) != 1) &&
                    SzDuel_GetMapUnitID(x, y) < 0) {
                    value = TaLogic010_GetEmptyLine(side);
                    if (y < (value >> 8)) {
                        minY = y;
                    } else {
                        minY = value >> 8;
                    }
                    if (y > (value >> 8)) {
                        maxY = y;
                    } else {
                        maxY = value >> 8;
                    }
                    if (x < (value & 255)) {
                        minX = x;
                    } else {
                        minX = value & 255;
                    }
                    if (x > (value & 255)) {
                        maxX = x;
                    } else {
                        maxX = value & 255;
                    }
                    value = (maxX - minX) + (maxY - minY);
                    if (value < bestDistance && value >= 0) {
                        packed = position;
                        bestDistance = value;
                    }
                }
            }
            i++;
        } while (i < 8);
        if (bestDistance < 14) {
            return packed;
        }
        packed = -1;
        return packed;
        }
    }
    {
    int y;
    int x;
    int i;
    int best;
    best = -1;
    i = 0;
    do {
        packed = Ta_GetSommonPiecePreference(side, i);
        if (packed >= 0) {
            x = packed & 255;
            y = packed >> 8;
            if (SzDuel_GetField(x, y) != 8 &&
                (SzDuel_GetField(x, y) != 9 || func_0024fe40_free(block, x, y) != 1) &&
                SzDuel_GetMapUnitID(x, y) < 0 && y < 8) {
                best = packed;
            }
        }
        i++;
    } while (i < 8);
    if (best >= 0) {
        return best;
    }
    best = -1;
    return best;
    }
}

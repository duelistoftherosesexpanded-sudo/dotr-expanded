#include "dotr/ee/logic012_line.h"

/* TaLogic012_CommandLineUnit (0x142f50): chooses the command for the unit at order->position.
 * The unit handle is kept in an integer local (DotrLineWord). */

static inline int min_00142f50(int a, int b)
{
    if (a < b) {
        return a;
    }
    return b;
}

static inline int max_00142f50(int a, int b)
{
    if (a < b) {
        return b;
    }
    return a;
}

void TaLogic012_CommandLineUnit(struct DotrLineOrder *arg0)
{
    struct DotrLineOrder *st = arg0;
    int otherPosition;
    int position;
    int side;
    int bestScore;
    int other;
    int bestId;
    int result;
    unsigned short *walk;
    int lowY;
    int y;
    int count;
    int lowX;
    int x;
    int otherY;
    int otherX;
    int i;
    DotrLineWord owner;
    unsigned short *list;
    int candidateX;
    int secondCount;
    struct DotrLineCell *entry;
    int candidateY;
    int maximum;
    struct DotrLineNibbles *tile;
    int candidatePosition;
    struct DotrLineCell entries[22];
    unsigned short candidates[7];
    struct DotrLineNibbles tiles[12];

    if (SzDuel_GetUnit(st->position >> 8, st->position & 0xFF) != 0) {
        tile = tiles;
        i = 0;
        do {
            tile->lo = tile->hi = 15;
            i++;
            tile++;
        } while (i < 12);
        side = Ta_GetCurrentSide(st->position >> 8, st->position & 0xFF);
        other = (side == 0 ? 1 : 0) << 8;
        position = SzDuel_GetUnitLoc(st->position >> 8, st->position & 0xFF);
        otherX = other >> 8;
        otherY = other & 0xFF;
        otherPosition = SzDuel_GetUnitLoc(otherX, otherY);
        owner = (DotrLineWord)SzDuel_GetUnit(st->position >> 8, st->position & 0xFF);
        bestId = -1;
        bestScore = 7;
        entry = entries;
        i = 0;
        do {
            i++;
            entry->id = 0xFFFF;
            entry->kind = 0xFF;
            entry++;
        } while (i < 22);
        if (TaLogic_IsRootOpen(st->position >> 8, st->position & 0xFF, x = position & 0xFF, y = position >> 8, otherX, otherY,
                               otherPosition & 0xFF, otherPosition >> 8, 2) > 0) {
            st->unknown_04 = 0x2FF;
            st->flags.word = (other << 16) | st->flags.half;
        } else {
            list = candidates;
            i = 0;
            {
                unsigned short *fill = list;
                do {
                    i++;
                    *fill = 0xFFFF;
                    fill++;
                } while (i < 7);
            }
            if (CheckLandAgreement(SzDuel_GetField(x, y), (struct jp_9a533 *)owner) != 1) {
                st->unknown_04 = 0x6FF;
                result = TaLogic012_GetAgreeLineZ((struct jp_9a533 *)owner);
                if (result >= 0) {
                    st->flags.word = st->flags.half | (((result << 8) | (position & 0xFF)) << 16);
                } else {
                    TaLogic012_CommandBtlUnit(st);
                    return;
                }
                return;
            } else {
                count = TaLogic012_GetLineUnitLst(candidates, y, side, 2) & 0xFF;
                i = 0;
                walk = list;
                goto test1;
            loop1:
                candidatePosition = SzDuel_GetUnitLoc(*walk >> 8, *walk & 0xFF);
                candidateX = candidatePosition & 0xFF;
                if (TaLogic_IsUnitWin4((struct jp_9a533 *)owner, *walk, TaLogic_GetWarnHideLvl__001461c0(side),
                                       candidateX, candidateX, 0) >= 0) {
                    result = TaLogic_GetInvadeRoot2((struct jp_9a533 *)owner, (struct jp_9b2bd *)tiles, x, y, candidateX, candidatePosition >> 8,
                                                    *walk, 0, 5, (unsigned short)TaLogic_GetWarnHideLvl__001461c0(side));
                    if (result > 0 && bestScore < result) {
                        bestId = *walk;
                        bestScore = result;
                    }
                } else {
                    candidateY = candidatePosition >> 8;
                    lowY = min_00142f50(y, candidateY);
                    candidateY = max_00142f50(candidateY, y);
                    lowX = min_00142f50(x, candidateX);
                    candidateX = max_00142f50(candidateX, x);
                    if ((candidateX - lowX) + (candidateY - lowY) < 3) {
                        st->unknown_04 = 0x3FF;
                        st->flags.word = (position << 16) | st->flags.half;
                        return;
                    }
                }
                walk++;
                i++;
            test1:
                if (i < count) {
                    goto loop1;
                }
                if (bestId >= 0) {
                    st->unknown_04 = 0x2FF;
                    st->flags.word = (bestId << 16) | st->flags.half;
                }
                secondCount = TaLogic012_GetLineUnitLst(candidates, GetCap(6, y - 1), side, 2) & 0xFF;
                maximum = -1;
                i = 0;
                goto test2;
            loop2:
                candidateX = SzDuel_GetUnitLoc(*list >> 8, *list & 0xFF) & 0xFF;
                result = TaLogic_IsUnitWin4((struct jp_9a533 *)owner, *list, TaLogic_GetWarnHideLvl__001461c0(side),
                                            candidateX, candidateX, 0);
                if (result >= 0 && maximum < result) {
                    maximum = result;
                }
                list++;
                i++;
            test2:
                if (i < secondCount) {
                    goto loop2;
                }
                if ((TaLogic012_GetLineUnitLst(candidates, y, side, 1) & 0xFF) > 1) {
                    result = TaLogic012_GetEmptyLine((struct jp_9a533 *)owner);
                    if (result >= 0) {
                        st->unknown_04 = 0x6FF;
                        st->flags.word = st->flags.half | (((result << 8) | (position & 0xFF)) << 16);
                    } else {
                        TaLogic012_CommandBtlUnit(st);
                    }
                } else {
                    switch ((st->unknown_04 >> 8) & 0xFF) {
                    case 16:
                        if (x > 0) {
                            st->flags.word = (y << 24) | st->flags.half;
                        } else {
                            st->unknown_04 = 0xFFF;
                            st->flags.word = (((y << 8) | 6) << 16) | st->flags.half;
                        }
                        break;
                    case 15:
                        if (x < 6) {
                            st->flags.word = (((y << 8) | 6) << 16) | st->flags.half;
                        } else {
                            st->unknown_04 = 0x10FF;
                            st->flags.word = (y << 24) | st->flags.half;
                        }
                        break;
                    default:
                        if (func_0019a5c8() % 2 == 0) {
                            st->unknown_04 = 0x10FF;
                            st->flags.word = (y << 24) | st->flags.half;
                        } else {
                            st->unknown_04 = 0xFFF;
                            st->flags.word = (((y << 8) | 6) << 16) | st->flags.half;
                        }
                        break;
                    }
                }
            }
        }
    }
}


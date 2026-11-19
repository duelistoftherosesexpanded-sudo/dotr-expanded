#include "dotr/ee/search_range.h"

/* 0x24ca40. SzDuel_GetUnitCardNo is declared without a prototype inside the body. */
#ifndef __MWERKS__
/* Host build only: this C mixes signed and unsigned comparisons and calls
 * SzDuel_GetUnitCardNo without a prototype. */
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

int SearchUnitByRange(int col, int row, int arg2, int compMode, int searchMode, int range) {
    unsigned int score;
    unsigned int hi;
    unsigned short lo;
    unsigned int bestA;
    unsigned int bestB;
    int idx;
    int side;
    int card;
    int v;
    int y;
    int x;
    int id;
    int best2;
    int bestSide;
    unsigned int res;
    int i;
    unsigned short *p;
    int idxB;
    int sideB;
    int idxC;
    int sideC;
    unsigned short list[24];

    p = list;
    best2 = 0x270F;
    bestSide = -1;
    res = 0;
    bestA = 0;
    bestB = 0x27100000;
    for (i = 0; i < 22; i++) {
        list[i] = 0xFFFF;
    }
    TaUtil_GetUnitLst(arg2, compMode, (short *)list);
    switch (range) {
    case 22: /* mountain cells */
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(list); i++) {
            if ((*p & 0xFF) > 0) {
                id = SzDuel_GetUnitLoc(*p >> 8, *p & 0xFF);
                if (SzDuel_GetField(id & 0xFF, id >> 8) == 2) {
                    v = SzDuel_GetUnitKind(*p >> 8, *p & 0xFF);
                    switch (searchMode & 0xFFFF) {
                    case 0x1D:
                        if ((v >> 5) == 0 && Ta_SideMatch(arg2, Ta_GetCurrentSide(*p >> 8, *p & 0xFF), compMode) == 1) {
                            res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        }
                        break;
                    case 0x1F:
                    case 0x34:
                        res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        break;
                    }
                }
            }
            p++;
        }
        break;
    case 24: /* sea cells */
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(list); i++) {
            if ((*p & 0xFF) > 0) {
                id = SzDuel_GetUnitLoc(*p >> 8, *p & 0xFF);
                if (SzDuel_GetField(id & 0xFF, id >> 8) == 4) {
                    v = SzDuel_GetUnitKind(*p >> 8, *p & 0xFF);
                    switch (searchMode & 0xFFFF) {
                    case 0x1D:
                        if ((v >> 5) == 0 && Ta_SideMatch(arg2, Ta_GetCurrentSide(*p >> 8, *p & 0xFF), compMode) == 1) {
                            res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        }
                        break;
                    case 0x1F:
                    case 0x34:
                        res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        break;
                    }
                }
            }
            p++;
        }
        break;
    case 23: /* meadow cells */
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(list); i++) {
            if ((*p & 0xFF) > 0) {
                id = SzDuel_GetUnitLoc(*p >> 8, *p & 0xFF);
                if (SzDuel_GetField(id & 0xFF, id >> 8) == 3) {
                    v = SzDuel_GetUnitKind(*p >> 8, *p & 0xFF);
                    switch (searchMode & 0xFFFF) {
                    case 0x1D:
                        if ((v >> 5) == 0 && Ta_SideMatch(arg2, Ta_GetCurrentSide(*p >> 8, *p & 0xFF), compMode) == 1) {
                            res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        }
                        break;
                    case 0x1F:
                    case 0x34:
                        res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        break;
                    }
                }
            }
            p++;
        }
        break;
    case 20: /* forest cells */
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(list); i++) {
            if ((*p & 0xFF) > 0) {
                id = SzDuel_GetUnitLoc(*p >> 8, *p & 0xFF);
                if (SzDuel_GetField(id & 0xFF, id >> 8) == 0) {
                    v = SzDuel_GetUnitKind(*p >> 8, *p & 0xFF);
                    switch (searchMode & 0xFFFF) {
                    case 0x1D:
                        if ((v >> 5) == 0 && Ta_SideMatch(arg2, Ta_GetCurrentSide(*p >> 8, *p & 0xFF), compMode) == 1) {
                            res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        }
                        break;
                    case 0x1F:
                    case 0x34:
                        res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        break;
                    }
                }
            }
            p++;
        }
        break;
    case 29: /* crush cells */
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(list); i++) {
            if ((*p & 0xFF) > 0) {
                id = SzDuel_GetUnitLoc(*p >> 8, *p & 0xFF);
                if (SzDuel_GetField(id & 0xFF, id >> 8) == 9) {
                    v = SzDuel_GetUnitKind(*p >> 8, *p & 0xFF);
                    switch (searchMode & 0xFFFF) {
                    case 0x1D:
                        if ((v >> 5) == 0 && Ta_SideMatch(arg2, Ta_GetCurrentSide(*p >> 8, *p & 0xFF), compMode) == 1) {
                            res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        }
                        break;
                    case 0x1F:
                    case 0x34:
                        res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        break;
                    }
                }
            }
            p++;
        }
        break;
    case 18: /* same column or row */
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(list); i++) {
            if ((*p & 0xFF) > 0) {
                id = SzDuel_GetUnitLoc(*p >> 8, *p & 0xFF);
                if ((id & 0xFF) == col || (id >> 8) == row) {
                    v = SzDuel_GetUnitKind(*p >> 8, *p & 0xFF);
                    switch (searchMode & 0xFFFF) {
                    case 29:
                        if ((v >> 5) == 0 && Ta_SideMatch(arg2, Ta_GetCurrentSide(*p >> 8, *p & 0xFF), compMode) == 1) {
                            res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        }
                        break;
                    case 30:
                        if ((v >> 5) != 0 && Ta_SideMatch(arg2, Ta_GetCurrentSide(*p >> 8, *p & 0xFF), compMode) == 1) {
                            res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        }
                        break;
                    case 31:
                        res = ConvUnitIDtoFlagList(res, *p >> 8, *p & 0xFF);
                        break;
                    }
                }
            }
            p++;
        }
        break;
    default: /* any other range is a board (Manhattan) distance */
        for (i = 0; i < TaLogic_GetUnitCtlLstLen(list); i++) {
            id = SzDuel_GetUnitLoc(*p >> 8, *p & 0xFF);
            if (((((id & 0xFF) < col) ? col : (id & 0xFF)) - ((col < (id & 0xFF)) ? col : (id & 0xFF)) + ((((id >> 8) < row) ? row : (id >> 8)) - ((row < (id >> 8)) ? row : (id >> 8))) <= range)) {
                y = id >> 8;
                x = id & 0xFF;
                v = SzDuel_GetUnitKind(*p >> 8, *p & 0xFF);
                card = *p;
                switch (searchMode & 0xFFFF) {
                case 0x1F:
                case 0x2D:
                case 0x34:
                case 0x3D:
                    res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    break;
                case 0x1E:
                    if ((v >> 5) != 0) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 0x1D:
                    if ((v >> 5) == 0) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 0:
                    if ((v >> 5) == 0 && (v & 0x1F) == 0) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 1:
                    if ((v >> 5) == 0 && (v & 0x1F) == 1) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 2:
                    if ((v >> 5) == 0 && (v & 0x1F) == 2) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 3:
                    if ((v >> 5) == 0 && (v & 0x1F) == 3) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 4:
                    if ((v >> 5) == 0 && (v & 0x1F) == 4) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 5:
                    if ((v >> 5) == 0 && (v & 0x1F) == 5) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 6:
                    if ((v >> 5) == 0 && (v & 0x1F) == 6) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 7:
                    if ((v >> 5) == 0 && (v & 0x1F) == 7) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 8:
                    if ((v >> 5) == 0 && (v & 0x1F) == 8) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 9:
                    if ((v >> 5) == 0 && (v & 0x1F) == 9) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 10:
                    if ((v >> 5) == 0 && (v & 0x1F) == 10) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 11:
                    if ((v >> 5) == 0 && (v & 0x1F) == 11) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 12:
                    if ((v >> 5) == 0 && (v & 0x1F) == 12) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 13:
                    if ((v >> 5) == 0 && (v & 0x1F) == 13) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 14:
                    if ((v >> 5) == 0 && (v & 0x1F) == 14) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 15:
                    if ((v >> 5) == 0 && (v & 0x1F) == 15) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 16:
                    if ((v >> 5) == 0 && (v & 0x1F) == 16) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 17:
                    if ((v >> 5) == 0 && (v & 0x1F) == 17) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 18:
                    if ((v >> 5) == 0 && (v & 0x1F) == 18) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 19:
                    if ((v >> 5) == 0 && (v & 0x1F) == 19) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 0x3B:
                    if ((v >> 5) == 0 && (v & 0x1F) == 20) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 0x14:
                    side = card >> 8;
                    if ((v >> 5) == 0) {
                        idx = card & 0xFF;
                        if (SzDuel_GetUnitAttr(side, idx) == 0) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x15:
                    side = card >> 8;
                    if ((v >> 5) == 0) {
                        idx = card & 0xFF;
                        if (SzDuel_GetUnitAttr(side, idx) == 1) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x16:
                    side = card >> 8;
                    if ((v >> 5) == 0) {
                        idx = card & 0xFF;
                        if (SzDuel_GetUnitAttr(side, idx) == 2) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x18:
                    side = card >> 8;
                    if ((v >> 5) == 0) {
                        idx = card & 0xFF;
                        if (SzDuel_GetUnitAttr(side, idx) == 4) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x17:
                    side = card >> 8;
                    if ((v >> 5) == 0) {
                        idx = card & 0xFF;
                        if (SzDuel_GetUnitAttr(side, idx) == 3) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x19:
                    side = card >> 8;
                    if ((v >> 5) == 0) {
                        idx = card & 0xFF;
                        if (SzDuel_GetUnitAttr(side, idx) == 5) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x3C:
                    side = card >> 8;
                    idx = card & 0xFF;
                    if (SzDuel_GetUnitStatus(side, idx, 4) == 0) {
                        res = ConvUnitIDtoFlagList(res, side, idx);
                    }
                    break;
                case 0x35:
                    side = card >> 8;
                    idx = card & 0xFF;
                    if (SzDuel_GetUnitStatus(side, idx, 3) == 0) {
                        res = ConvUnitIDtoFlagList(res, side, idx);
                    }
                    break;
                case 0x36:
                    if ((v >> 5) == 4 || (v >> 5) == 3) {
                        side = card >> 8;
                        idx = card & 0xFF;
                        if (SzDuel_GetUnitStatus(side, idx, 4) != 0) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x21:
                    sideB = card >> 8;
                    if ((v >> 5) == 0) {
                        idxB = card & 0xFF;
                        id = SzDuel_GetUnitLoc(sideB, idxB);
                        score = (TaUtil_GetUnitAP(sideB, idxB, id & 0xFF, id >> 8) << 16) | TaUtil_GetUnitDP(sideB, idxB, id & 0xFF, id >> 8);
                        if ((bestA >> 16) < (score >> 16)) {
                            best2 = SzDuel_GetUnitSummonOrder(sideB, idxB);
                            bestA = score;
                            bestSide = sideB;
                            res = ConvUnitIDtoFlagList(0, sideB, idxB);
                        } else if ((bestA >> 16) == (score >> 16)) {
                            lo = bestA;
                            if (lo == (score & 0xFFFF)) {
                                if (best2 == SzDuel_GetUnitSummonOrder(sideB, idxB)) {
                                    if (bestSide == compMode) {
                                        bestA = score;
                                        bestSide = sideB;
                                        res = ConvUnitIDtoFlagList(0, sideB, idxB);
                                    }
                                } else if (SzDuel_GetUnitSummonOrder(sideB, idxB) < best2) {
                                    best2 = SzDuel_GetUnitSummonOrder(sideB, idxB);
                                    bestA = score;
                                    bestSide = sideB;
                                    res = ConvUnitIDtoFlagList(0, sideB, idxB);
                                }
                            } else if (lo < (score & 0xFFFF)) {
                                best2 = SzDuel_GetUnitSummonOrder(sideB, idxB);
                                bestA = score;
                                bestSide = sideB;
                                res = ConvUnitIDtoFlagList(0, sideB, idxB);
                            }
                        }
                    }
                    break;
                case 0x20:
                    sideC = card >> 8;
                    if ((v >> 5) == 0) {
                        idxC = card & 0xFF;
                        id = SzDuel_GetUnitLoc(sideC, idxC);
                        score = (TaUtil_GetUnitAP(sideC, idxC, id & 0xFF, id >> 8) << 16) | TaUtil_GetUnitDP(sideC, idxC, id & 0xFF, id >> 8);
                        hi = score >> 16;
                        if (hi < (bestB >> 16)) {
                            best2 = SzDuel_GetUnitSummonOrder(sideC, idxC);
                            bestB = score;
                            bestSide = sideC;
                            res = ConvUnitIDtoFlagList(0, sideC, idxC);
                        } else {
                            if (bestB == hi) {
                            if ((bestB & 0xFFFF) == (score & 0xFFFF)) {
                                if (best2 == SzDuel_GetUnitSummonOrder(sideC, idxC)) {
                                    if (bestSide == compMode) {
                                        bestB = score;
                                        bestSide = sideC;
                                        res = ConvUnitIDtoFlagList(0, sideC, idxC);
                                    }
                                } else if (SzDuel_GetUnitSummonOrder(sideC, idxC) < best2) {
                                    best2 = SzDuel_GetUnitSummonOrder(sideC, idxC);
                                    bestB = score;
                                    bestSide = sideC;
                                    res = ConvUnitIDtoFlagList(0, sideC, idxC);
                                }
                            } else if ((bestB & 0xFFFF) > (score & 0xFFFF)) {
                                best2 = SzDuel_GetUnitSummonOrder(sideC, idxC);
                                bestB = score;
                                bestSide = sideC;
                                res = ConvUnitIDtoFlagList(0, sideC, idxC);
                            }
                        }
                        }
                    }
                    break;
                case 0x25:
                    if ((v >> 5) == 0) {
                        side = card >> 8;
                        idx = card & 0xFF;
                        if (TaUtil_GetUnitAP(side, idx, x, y) <= (searchMode >> 16 & 0xFFFF)) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x24:
                    if ((v >> 5) == 0) {
                        side = card >> 8;
                        idx = card & 0xFF;
                        if (TaUtil_GetUnitAP(side, idx, x, y) >= (searchMode >> 16 & 0xFFFF)) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x3A:
                    if ((v >> 5) == 0) {
                        side = card >> 8;
                        idx = card & 0xFF;
                        if (TaUtil_GetUnitDP(side, idx, x, y) <= (searchMode >> 16 & 0xFFFF)) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x39:
                    if ((v >> 5) == 0) {
                        side = card >> 8;
                        idx = card & 0xFF;
                        if (TaUtil_GetUnitDP(side, idx, x, y) >= (searchMode >> 16 & 0xFFFF)) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x37:
                    if ((v >> 5) == 0 && (v & 0x1F) == 0) {
                        side = card >> 8;
                        idx = card & 0xFF;
                        if (TaUtil_GetUnitAP(side, idx, x, y) <= (searchMode >> 16 & 0xFFFF)) {
                            res = ConvUnitIDtoFlagList(res, side, idx);
                        }
                    }
                    break;
                case 0x22:
                    if ((card & 0xFF) == 0) {
                        res = ConvUnitIDtoFlagList(res, card >> 8, card & 0xFF);
                    }
                    break;
                case 0x1C: {
                    extern int SzDuel_GetUnitCardNo();

                    side = card >> 8;
                    idx = card & 0xFF;
                    if ((searchMode >> 16) == SzDuel_GetUnitCardNo(side, idx)) {
                        res = ConvUnitIDtoFlagList(res, side, idx);
                    }
                    break;
                }
                }
            }
            p++;
        }
        break;
    }
    return res;
}

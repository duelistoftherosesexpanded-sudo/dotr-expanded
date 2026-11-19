#include "dotr/ee/driver_tree.h"

/* 0x27c770. TaLogic_GetRoot2 is reached through a view with its prototype (driver_tree.h) and the
 * variable information is a DotrTreeWord (int on the target); one callee is declared without a
 * prototype inside the function body. */
#ifndef __MWERKS__
/* Host build only: the code calls a view without a prototype and keeps an unused local. */
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

int TaLogic_GetLandChange(DotrTreeWord unit, int col, int row, int targetFieldType, int arg4, int radius) {
    extern int func_002792c0_tree();
    int x;
    int cy;
    unsigned char x1;
    int i;
    int bestCount;
    DotrTreeNibbles * entry;
    int bestCost;
    unsigned char y0;
    int cost;
    unsigned char y1;
    int cx;
    unsigned char minCount;
    unsigned char x0;
    int best;
    unsigned char range;
    unsigned char kind;
    DotrTreeNibbles entries[12];
    int count;
    int y;

    entry = entries;
    best = -1;
    bestCount = 0;
    i = 0;
    do {
        entry->low = entry->high = 15;
        entry++;
        i++;
    } while (i < 12);
    minCount = arg4;
    bestCost = 14;
    range = radius;
    kind = targetFieldType;
    for (i = 0; i < 7; i++) {
        for (x = 0; x < 7; x++) {
            count = 0;
            y0 = GetCap(0, i - range);
            y1 = i + range >= 7 ? 6 : i + range;
            x0 = GetCap(0, x - range);
            x1 = x + range >= 7 ? 6 : x + range;
            for (cy = y0; cy <= y1; cy++) {
                for (cx = x0; cx <= x1; cx++) {
                    if (((cy < i ? i : cy) - (i < cy ? i : cy)) + ((cx < x ? x : cx) - (x < cx ? x : cx)) <= range &&
                        kind != SzDuel_GetField(cy, cx) && SzDuel_GetField(cy, cx) != 8) { /* 8: labyrinth */
                        count++;
                    }
                }
            }
            if (count >= bestCount && minCount < count) {
                cost = func_002792c0_tree(unit, entries, col, row, i, x, 0, 30);
                if (cost < bestCost) {
                    bestCost = cost;
                    bestCount = count;
                    best = (x << 8) | i;
                }
            }
        }
    }
    return best;
}

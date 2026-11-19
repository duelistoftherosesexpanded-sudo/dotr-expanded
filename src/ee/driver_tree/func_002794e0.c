#include "dotr/ee/driver_tree.h"

/* 0x2794e0. The callees are reached through views with their prototypes (driver_tree.h), and the
 * address-carrying int parameters and the parent cast use DotrTreeWord (int on the target); one
 * callee is declared without a prototype inside the function body. */
#ifndef __MWERKS__
/* Host build only: the code calls a view without a prototype. */
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

unsigned char *TaLogic_GetRootTree2(unsigned char *node, DotrTreeWord parent, DotrTreeWord card, int x, int y,
                  int targetX, int targetY, unsigned char limit, signed char *found, DotrTreeWord ownerId,
                  unsigned char depth, unsigned short *pool, unsigned short *poolEnd, DotrTreeWord ignoreList) {
    int count;
    int minY;
    unsigned int maxX;
    unsigned int maxY;
    unsigned int distanceLimit;
    int nextDepth;
    int range;
    int iy;
    int ix;
    extern int func_002798c0_tree();

    range = func_002464f0_tree(card, x, y);
    if (card == 0) {
        return NULL;
    }
    if (node == NULL) {
        if ((int)depth > (int)limit) {
            return NULL;
        }
        if ((node = func_0027b150_tree(pool, poolEnd)) == NULL) {
            return NULL;
        }
        func_0027af80_tree(node, parent, x, y);
        if (x == targetX && y == targetY) {
            *found = 1;
            return node;
        }
    }
    range &= 0xF;
    count = 0;
    ix = GetCap(0, x - range);
    if ((unsigned int)(x + range) >= 7U) {
        maxX = 6;
    } else {
        maxX = x + range;
    }
    minY = GetCap(0, y - range);
    maxY = y + range;
    if (maxY >= 7U) {
        maxY = 6;
    }
    if (ix <= (int)maxX) {
        distanceLimit = range + 1;
        nextDepth = depth + 1;
        while (1) {
            iy = minY;
            if (iy <= (int)maxY) {
                while (1) {
                    if (ix >= 0 && iy >= 0 && ix < 7 && iy < 7 &&
                        (unsigned int)(((ix < x ? x : ix) - (x < ix ? x : ix)) +
                              ((iy < y ? y : iy) - (y < iy ? y : iy))) < distanceLimit &&
                        (((ix < x ? x : ix) - (x < ix ? x : ix)) +
                         ((iy < y ? y : iy) - (y < iy ? y : iy))) != 0 &&
                        func_002798c0_tree(card, x, y, ix, iy, ownerId) == 1 &&
                        func_0027b010_tree(node, ix, iy, x, y, depth) == 0) {
                        ((DotrTreeNode2 *)node)->children[count] =
                            TaLogic_GetRootTree2(((DotrTreeNode2 *)node)->children[count],
                                         (DotrTreeWord)node, card, ix, iy, targetX, targetY,
                                         limit, found, ownerId, (unsigned char)nextDepth,
                                         pool, poolEnd, ignoreList);
                        count++;
                    }
                    iy++;
                    if (iy > (int)maxY) {
                        break;
                    }
                }
            }
            ix++;
            if (ix > (int)maxX) {
                break;
            }
        }
    }
    return node;
}

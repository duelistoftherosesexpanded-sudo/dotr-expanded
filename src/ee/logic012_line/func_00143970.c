#include "dotr/ee/logic012_line.h"

/* TaLogic012_GetEmptyLine (0x143970): the first line y of 0..6 on which no map unit is
 * controlled by the handle's controller and whose field at x 0 agrees with the unit, else -1.
 * An inline helper counts the line's units. MATCHING DEVICE: each line first runs an empty
 * seven-step loop, as the original code does. */

static inline unsigned char count_00143970(int line, int key)
{
    int i;
    unsigned char count;
    int card;

    count = 0;
    i = 0;
    do {
        card = SzDuel_GetMapUnitID(i, line);
        if (card >= 0) {
            if (key == Ta_IsUnitControl(card >> 8, card & 0xFF)) {
                count++;
            }
        }
        i++;
    } while (i < 7);
    return count;
}

int TaLogic012_GetEmptyLine(struct jp_9a533 *item)
{
    int row;
    int delay;
    int category;
    int n;

    category = Ta_IsUnitControl2(item);
    row = 0;
    do {
        delay = 0;
        do {
            delay++;
        } while (delay < 7);
        if ((n = count_00143970(row, category)) == 0) {
            if (CheckLandAgreement(SzDuel_GetField(0, row), item) == 1) {
                return row;
            }
        }
        row++;
    } while (row < 7);
    return -1;
}

#include "dotr/ee/state_records.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Operation: 0 add, 1 subtract, 2 multiply, 3 divide, 4 set. Operation 3 divides
 * without a zero check; the original has none either. */
int SzDuel_CalcSummonPower(int side, int operation, int amount)
{
    struct DotrStateRecord *p = &data_00317490_records[side];
    int currentPower = p->unknown_341;
    int r;

    switch (operation) {
    case 0:
        r = currentPower + amount;
        break;
    case 1:
        r = currentPower - amount;
        break;
    case 2:
        r = currentPower * amount;
        break;
    case 3:
        r = currentPower / amount;
        break;
    case 4:
        r = amount;
        break;
    default:
        return 0;
    }
    p->unknown_341 = MIN(MAX(r, 0), 12);
    return 1;
}

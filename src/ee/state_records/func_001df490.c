#include "dotr/ee/state_records.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Operation: 0 add, 1 subtract, 2 multiply, 3 divide, 4 set. Operation 3 divides
 * without a zero check; the original has none either. */
int SzDuel_CalcLP(int side, int operation, int amount)
{
    struct DotrStateRecord *p = &data_00317490_records[side];
    int LP = p->unknown_33a;
    int r;

    switch (operation) {
    case 0:
        r = LP + amount;
        break;
    case 1:
        r = LP - amount;
        break;
    case 2:
        r = LP * amount;
        break;
    case 3:
        r = LP / amount;
        break;
    case 4:
        r = amount;
        break;
    default:
        return 0;
    }
    p->unknown_33a = MIN(MAX(r, 0), 9999);
    return 1;
}

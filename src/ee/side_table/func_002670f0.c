#include "dotr/ee/state_queries.h"

int Ta_GetLPRevByNaturalEffect(int lpChangeType, int lifeAmount)
{
    int i;
    unsigned value = lifeAmount;
    struct DotrStateModifier *e;
    unsigned head;
    unsigned word;

    if (lifeAmount == 0) {
        return 0;
    }
    for (i = 0; i < 11; i++) {
        if (SzDuel_GetUnitStatus(0, i, 4) && SzDuel_GetUnitStatus(0, i, 3) && SzDuel_GetUnitStatus(0, i, 5)) {
            e = (struct DotrStateModifier *)Ta_GetUnitEffect(0, i, 2);
            if (e) {
                head = e->head;
                if (head >> 8 == 59 && (head & 0x3f) == (unsigned)lpChangeType) {
                    word = e->u.word;
                    switch (word >> 16) {
                    case 0:
                        value += e->u.half.amount;
                        break;
                    case 1:
                        value -= e->u.half.amount;
                        break;
                    case 2:
                        value += value * (e->u.half.amount - 1);
                        break;
                    case 3:
                        if ((word & 0xffff) != 0) {
                            value -= value / (word & 0xffff);
                        }
                        break;
                    }
                }
            }
        }
    }
    for (i = 1; i < 11; i++) {
        if (SzDuel_GetUnitStatus(1, i, 4) && SzDuel_GetUnitStatus(1, i, 3) && SzDuel_GetUnitStatus(1, i, 5)) {
            e = (struct DotrStateModifier *)Ta_GetUnitEffect(1, i, 2);
            if (e) {
                head = e->head;
                if (head >> 8 == 59 && (head & 0x3f) == (unsigned)lpChangeType) {
                    word = e->u.word;
                    switch (word >> 16) {
                    case 0:
                        value += e->u.half.amount;
                        break;
                    case 1:
                        value -= e->u.half.amount;
                        break;
                    case 2:
                        value += value * (e->u.half.amount - 1);
                        break;
                    case 3:
                        if ((word & 0xffff) != 0) {
                            value -= value / (word & 0xffff);
                        }
                        break;
                    }
                }
            }
        }
    }
    return GetParCap(value);
}

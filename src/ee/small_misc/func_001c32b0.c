#include "dotr/ee/small_misc.h"

int lSzDuel_TradeDo(void)
{
    struct DotrMiscControlCounts *s = &data_00317490_counts;
    int total = 0;
    int i;

    for (i = 0; i < 854; i++) {
        unsigned char *pa = &s->first[i];
        unsigned char x = *pa;
        if (x != 0) {
            unsigned char *pb = &s->second[i];
            unsigned char y = *pb;
            if (y != 0) {
                if ((x & 0xff) > (y & 0xff)) {
                    *pa = x - y;
                    s->second[i] = 0;
                } else {
                    *pb = y - x;
                    s->first[i] = 0;
                }
            }
        }
    }
    lSzDuel_TradeDoSide(0);
    lSzDuel_TradeDoSide(1);
    for (i = 0; i < 854; i++) {
        total += s->first[i];
        total += s->second[i];
    }
    return total;
}

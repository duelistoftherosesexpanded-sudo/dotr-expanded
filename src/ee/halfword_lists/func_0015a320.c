#include "dotr/ee/halfword_lists.h"

int TaLogic018_GetZoneSize(unsigned short *p)
{
    int i;
    unsigned char n = 0;
    for (i = 0; i < 49; i++) {
        if (*p != 0xFFFF) {
            n++;
        }
        p++;
    }
    return n;
}

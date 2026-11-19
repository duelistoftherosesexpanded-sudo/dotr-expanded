#include "dotr/ee/halfword_lists.h"

void TaLogic018_GetOutVirusZone(int a, int b, unsigned short *out)
{
    int i;
    unsigned short *p;
    for (i = 0, p = out; i < 49; i++) {
        *p++ = 0xFFFF;
    }
    TaLogic018_GetZoneSide(a, b, out);
    TaLogic018_GetZoneSize(out);
}

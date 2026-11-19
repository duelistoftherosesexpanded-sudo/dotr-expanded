#include "dotr/ee/low_leaves.h"

int TaLogic018_IsZonePast(unsigned int value, unsigned short *list)
{
    int index;

    index = 0;
    do {
        if (value == *list) {
            return 1;
        }
        index = index + 1;
        list = list + 1;
    } while (index < 0x31);
    return 0;
}

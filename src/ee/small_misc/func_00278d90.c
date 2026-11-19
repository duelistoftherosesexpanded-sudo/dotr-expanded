#include "dotr/ee/small_misc.h"

int TaLogic_GetArrayLength(unsigned char *array)
{
    int count;
    int i;
    for (i = 0, count = 0; i < 5; i++) {
        if (array[i] == 255) {
            return count;
        }
        count++;
    }
    return count;
}

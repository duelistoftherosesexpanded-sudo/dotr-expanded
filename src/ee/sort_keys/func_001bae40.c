#include "dotr/ee/sort_keys.h"

int lSzDuel_NumberCmp(unsigned short *a, unsigned short *b)
{
    int key_a = *a;
    int key_b = *b;

    return key_b < key_a ? 1 : key_a < key_b ? -1 : 0;
}

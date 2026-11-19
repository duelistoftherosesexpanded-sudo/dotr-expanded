#include "dotr/ee/resource_support.h"

int SzDuel_Type2Attr(int value, int fallback)
{
    /* Explicit branch returns reproduce the final a1-to-v0 delay-slot copy. */
    if (value > 0) {
        return value + 5;
    } else {
        return fallback;
    }
}

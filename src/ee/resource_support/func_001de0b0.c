#include "dotr/ee/resource_support.h"

/* The mapping of SzDuel_Kind2Type (0x1de020) followed by the selection of SzDuel_Type2Attr
 * (0x1de090), both contained inline; whether the original used shared helpers is unknown. */
int SzDuel_Kind2Attr(int type, int fallback)
{
    type = type == 1 ? 1
        : type == 2 ? 1
        : type == 3 ? 2
        : type == 4 ? 2
        : type == 5 ? 3
        : type;
    return type > 0 ? type + 5 : fallback;
}

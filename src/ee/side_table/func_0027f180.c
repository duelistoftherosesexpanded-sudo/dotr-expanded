#include "dotr/ee/side_table.h"

int Ta_LkPrim_DrawCard(struct DotrSideFadeOwner *owner, unsigned char alpha, struct DotrSidePoint *point)
{
    int r;
    int i;

    if (point) {
        r = func_001da2b0_owner(owner, 0, point->x, point->y);
    } else {
        r = func_001d9f00_owner(owner, 1);
    }
    for (i = 0; i < owner->count; i++) {
        owner->rows[i].color = 0xffffff;
        owner->rows[i].alpha = alpha;
    }
    if (r) {
        func_001da7a0_owner(owner->unknown_90, owner);
    } else {
        func_001da7a0_owner(0, owner);
    }
    func_001dbcf0_owner(owner);
    return 0;
}

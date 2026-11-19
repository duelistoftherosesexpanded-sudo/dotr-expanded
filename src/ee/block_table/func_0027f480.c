#include "dotr/ee/block_table.h"

void TaTuto_InitTutoData(DotrSlotSet *set)
{
    DotrSlotVec *p0;
    DotrSlotVec *p1;
    int i;

    p0 = (DotrSlotVec *)SzView_GetCP(0);
    p1 = (DotrSlotVec *)SzView_GetLook(0);
    if (set != 0) {
        set->h418 = 0;
        set->h420 = 0;
        for (i = 0; i < 22; i++) {
            set->entries[i].a = 0xffff;
            set->entries[i].b = 255;
            set->entries[i].d = -1;
            set->entries[i].c = 0xffff;
            set->entries[i].kind = 0;
        }
        for (i = 0; i < 32; i++) {
            set->ids[i] = 0xffff;
        }
        set->mode = 0;
        set->v0.x = p0->x;
        set->v0.y = p0->y;
        set->v0.z = p0->z;
        set->v0.w = p0->w;
        set->v1.x = p1->x;
        set->v1.y = p1->y;
        set->v1.z = p1->z;
        set->v1.w = p1->w;
        set->ids[1] = 0;
    }
}

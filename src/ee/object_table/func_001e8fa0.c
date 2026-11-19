#include "dotr/ee/object_table.h"

int SzModel_BindTextureFile(int index)
{
    struct DotrObjPackView *pModel;
    struct DotrObjPackDesc *desc;
    struct DotrObjPackEntry *e;
    struct DotrObjPackTable *t;
    int count;
    int i;
    if (index < 0 || index >= 84) {
        return 0;
    }
    pModel = (struct DotrObjPackView *)gSzModel_ahModel[index];
    if (pModel == 0) {
        return 0;
    }
    if (pModel->base == 0) {
        return 1;
    }
    desc = pModel->desc;
    if (desc->unknown_2c == 0) {
        return 1;
    }
    t = (struct DotrObjPackTable *)(pModel->base + desc->unknown_28);
    count = t->count;
    e = t->entries;
    for (i = 0; i < count; i++) {
        if (e->a) {
            e->a = pModel->base + e->a;
            RhText_BindAddress((void *)e->a);
            if (e->c) {
                e->c = pModel->base + e->c;
            }
            e->d = 0;
        }
        e++;
    }
    return 1;
}

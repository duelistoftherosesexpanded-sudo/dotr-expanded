#include "dotr/ee/small_misc.h"
int lNjaReloc_AdjustMotion(DotrMiscFixupHead *p, int delta, int count, int flag)
{
    int kind;
    if (flag) {
        ((DotrMiscAddress *)p)[-1] = delta + ((DotrMiscAddress *)p)[-1];
    }
    kind = p->kind & 0x3f;
    if (kind == 3) {
        if (p->data) {
            p->data = delta + p->data;
            lNjaReloc_AdjustMData2((struct DotrMiscFixup16 *)p->data, delta, count, flag);
        }
    } else if (kind == 7) {
        if (p->data) {
            p->data = delta + p->data;
            lNjaReloc_AdjustMData3((struct DotrMiscFixup24 *)p->data, delta, count, flag);
        }
    }
    return 1;
}

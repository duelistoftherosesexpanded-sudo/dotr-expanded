#include "dotr/ee/object_table.h"

/* Raises the object's level byte +0x200 to the low byte of value and passes
 * each halfword between the old and new level, in every listed node that has
 * data, through SzColor_CLUT2SEP with 1.0f and 6. Returns 0 without changes for a
 * rejected index or object, a set sign of +0x1f2, a missing +0x138 or +0x13c,
 * a zero header +0x2c, set bit 3 of +0x20c, clear bit 1 of +0x20d or a lower
 * level; otherwise 1. */
int SzModel_SetStoneRatio(int index, int value)
{
    struct DotrObject *p;
    struct DotrObjectLink *base;
    struct DotrObjectShadeHeader *header;
    int count;
    int i;
    unsigned short *words;
    int left;
    struct DotrObjectShadeSlot *slot;
    int level;
    struct DotrObjectShadeNode *node;
    int *list;
    struct DotrObjectShadeInfo *info;

    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    if (p->modelId < 0) {
        return 0;
    }
    base = p->modelBaseData;
    if (base == 0) {
        return 0;
    }
    if (p->drawFlag.unknown_bit_3) {
        return 0;
    }
    if (base == 0) {
        return 0;
    }
    header = *(struct DotrObjectShadeHeader **)p->unknown_13c;
    if (header == 0) {
        return 0;
    }
    if (header->unknown_2c == 0) {
        return 0;
    }
    if (!p->stateFlags.unknown_bit_1) {
        return 0;
    }
    level = value & 0xff;
    if (level < p->stoneRatio) {
        return 0;
    }
    list = (int *)((char *)base + header->unknown_28);
    count = *list;
    slot = (struct DotrObjectShadeSlot *)(list + 4);
    for (i = 0; i < count; i++, slot++) {
        node = slot->node;
        if (node != 0 && (info = &node->unknown_10, node->unknown_10.unknown_0c != 0)) {
            words = (unsigned short *)((char *)node + info->unknown_08 + 96) + p->stoneRatio;
            left = level - p->stoneRatio + 1;
            while (left-- != 0) {
                *words = func_00241a00_shade(1.0f, *words, 6);
                words++;
            }
        }
    }
    p->stoneRatio = value;
    return 1;
}

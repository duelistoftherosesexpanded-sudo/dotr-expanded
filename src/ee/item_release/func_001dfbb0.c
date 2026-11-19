#include "dotr/ee/item_release.h"

static inline struct DotrStateCell *cell(int x, int y)
{
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return 0;
    }
    return &gSzDuel_aaMapInfo[y][x];
}

static inline struct DotrReleaseSlot *slot(int index)
{
    if (index < 0 || index >= 6) {
        return 0;
    }
    return &gSzDuel_aCardControl_release[index];
}

static inline void release(struct DotrReleaseObject *object)
{
    if (object != 0) {
        SzDuel_ClearDrawCard((struct DotrOwnerDrawBlock *)object);
        object->unknown_90 = 0;
    }
}

/* The cell word is cleared without testing the lookup: out-of-range item bytes store
 * through a null pointer in the original as well. */
int SzDuel_DelUnit(int side, int unitArrayPos)
{
    struct DotrStateItem *item = dotr_state_item_checked(side, unitArrayPos);
    struct DotrReleaseObject *object;
    struct DotrReleaseSlot *s;

    if (item == 0) {
        return 0;
    }
    if (side < 2) {
        if (item->slot < 6) {
            SzModel_Preset(item->modelId, -1, 999);
        }
        object = (struct DotrReleaseObject *)item->cardModel;
        if (object != 0) {
            release(object);
        }
        cell(item->locCol, item->locRow)->unit = 0;
    } else if (unitArrayPos > 0) {
        s = slot(unitArrayPos);
        if (s != 0) {
            object = s->object;
            if (object != 0) {
                release(object);
            }
            SzDuel_ClearCardControl((struct DotrMiscSlot160 *)s);
        }
    }
    SzDuel_InitCardInfo((struct DotrStateBlock *)item, -1);
    return 1;
}

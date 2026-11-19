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

/* Same body as SzDuel_DelUnit. */
static inline int give_back(int a, int b)
{
    struct DotrStateItem *item = dotr_state_item_checked(a, b);
    struct DotrReleaseObject *object;
    struct DotrReleaseSlot *s;

    if (item == 0) {
        return 0;
    }
    if (a < 2) {
        if (item->slot < 6) {
            SzModel_Preset(item->modelId, -1, 999);
        }
        object = (struct DotrReleaseObject *)item->cardModel;
        if (object != 0) {
            release(object);
        }
        cell(item->locCol, item->locRow)->unit = 0;
    } else if (b > 0) {
        s = slot(b);
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

/* Moves a checked item to cell (x, y): whatever item the target cell holds is given back
 * first (SzDuel_DelUnit's body, inlined), then the item's cell bytes, both cell words and
 * its object's position are rewritten. */
struct DotrStateItem *SzDuel_MoveUnit(int a, int b, int x, int y)
{
    struct DotrStateItem *item = dotr_state_item_checked(a, b);
    struct DotrStateCell *to;
    struct DotrStateCell *from;
    struct DotrStateItem *other;
    struct DotrStateVector v;

    if (item == 0) {
        return 0;
    }
    if (item->locCol == x && item->locRow == y) {
        return 0;
    }
    to = cell(x, y);
    from = cell(item->locCol, item->locRow);
    if (to == 0 || from == 0) {
        return 0;
    }
    other = (struct DotrStateItem *)to->unit;
    if (other != 0) {
        give_back(other->team, other->slot);
    }
    item->locCol = x;
    item->locRow = y;
    *(short *)&item->unknown_34[0] = 0;
    *(short *)&item->unknown_34[2] = 0;
    from->unit = 0;
    to->unit = (DotrStateWord)item;
    SzModel_GetPos(item->modelId, &v);
    v.x = (x - 3) * 50;
    v.z = (y - 3) * 50;
    SzModel_SetPos(item->modelId, &v);
    if (item->cardModel != 0) {
        v.y = 0;
        item->cardModel->unknown_00 = v;
    }
    return item;
}

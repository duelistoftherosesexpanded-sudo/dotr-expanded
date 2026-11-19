#include "dotr/ee/small_misc.h"

int ShFace_Request(struct DotrMiscSlotRequest *req)
{
    int i;
    struct DotrMiscSlot52 *slot;

    if (req->id == 0xff) {
        for (i = 0; i < 8; i++) {
            if (gSh_Face_slots[i].active == 0) {
                break;
            }
        }
        if (i == 8) {
            return 0;
        }
        req->id = i;
        gSh_Face_slots[req->id].active = 1;
        gSh_Face_slots[req->id].key = 0xff;
    }
    slot = &gSh_Face_slots[req->id];
    if (slot->active == 0) {
        return 0;
    }
    if (slot->key != req->key) {
        for (i = 0; i < 16; i++) {
            gSh_Face_slots[req->id].unknown_1b[i] = 0;
            gSh_Face_slots[req->id].unknown_0b[i] = 0;
        }
    }
    slot->unknown_04 = req->unknown_01;
    slot->key = req->key;
    slot->unknown_06 = req->unknown_03;
    slot->unknown_2c = req->unknown_04;
    slot->unknown_2e = req->unknown_06;
    return 1;
}

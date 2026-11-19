#include "dotr/ee/shslot.h"

/* ShSlot_HitCardRequest (0x280920): sets the hit card (0x35117c) to 0xffff, then from the
 * step at 0x35117a (0..7) finds the next card to award and returns after advancing the
 * step: steps 0..2 take reel 0..2's centre symbol when it is not 0xffff and differs from
 * the centre symbols of the reels before it; steps 3..7 take, for each line still flashing
 * (line timer non-zero), the line's stop card from the reel table, or for 0xfffe a card
 * from TaSlot_GetFeverGain(reel 0's symbol at (position / 0x70 + offset[line]) % count,
 * line). With nothing left the step ends at 8 and the hit card stays 0xffff.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
void ShSlot_HitCardRequest(void)
{
    unsigned short id;
    int slot;
    int base;

    nextSlotDrop_shslot = 0xFFFF;
    while (nextSlotDropNumber_shslot < 8) {
        id = data_00351240_shslot[nextSlotDropNumber_shslot].center;
        switch (nextSlotDropNumber_shslot) {
        case 0:
            if (id != 0xFFFF) {
                nextSlotDrop_shslot = data_00351240_shslot[nextSlotDropNumber_shslot].center;
                nextSlotDropNumber_shslot++;
                return;
            }
            break;
        case 1:
            if (id != 0xFFFF && id != SquareSlotCardID_shslot) {
                nextSlotDrop_shslot = data_00351240_shslot[nextSlotDropNumber_shslot].center;
                nextSlotDropNumber_shslot++;
                return;
            }
            break;
        case 2:
            if (id != 0xFFFF && id != SquareSlotCardID_shslot && id != data_00351292_shslot) {
                nextSlotDrop_shslot = data_00351240_shslot[nextSlotDropNumber_shslot].center;
                nextSlotDropNumber_shslot++;
                return;
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            slot = nextSlotDropNumber_shslot - 3;
            if (data_00351210_shslot.line_timer[slot]) {
                if (data_00351230_shslot->extra[slot] == 0xFFFE) {
                    base = SquareSlotReelImageOffset_shslot / 0x70;
                    nextSlotDrop_shslot = TaSlot_GetFeverGain(data_00351240_shslot[0].symbols[(base + SlotTable_shslot[slot]) % data_00351230_shslot->count], slot);
                } else {
                    nextSlotDrop_shslot = data_00351230_shslot->extra[nextSlotDropNumber_shslot - 3];
                }
                nextSlotDropNumber_shslot++;
                return;
            }
            break;
        }
        nextSlotDropNumber_shslot++;
    }
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

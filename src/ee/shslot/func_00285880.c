#include "dotr/ee/shslot.h"

/* ShSlot_Init (0x285880): stores demo at 0x351244 and points the reel table at 0x1ed0000.
 * In demo mode it first fills that table: 16 symbols, each (random >> 4) % 3 + 5, the
 * middle one 0x334, all five line stops 0xfffe, flags 2. Line 0's stop is always set to
 * 0xfffe; the lines in play (0x351241) are those whose stop is not 0xffff, the winning
 * mask is cleared, game phase 0, reel length count * 0x70, frame 0, flash state and
 * timers cleared, each reel reset (position, velocity 0, center 0xffff, phase 0) and given
 * its strip: reel 0 the first; reels 1 and 2 their own (flags bit 0) or a copy of reel 0's
 * shuffled with count * 2 random swaps. Outside demo mode it ends with ShSlot_Start.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
void ShSlot_Init(int demo)
{
    int i;
    int j;
    int a;
    int b;
    unsigned short temp;
    struct DotrShSlotDeck *deck;
    int k;

    data_00351244_shslot = demo;
    data_00351230_shslot = DOTR_SHSLOT_DEMO_DECK;
    if (data_00351244_shslot) {
        deck = data_00351230_shslot;
        deck->count = 0x10;
        deck->extra[0] = deck->extra[1] = deck->extra[2] = deck->extra[3] = deck->extra[4] = 0xFFFE;
        for (k = 0; k < deck->count; k++) {
            deck->cards[0][k] = (func_0019a5c8() >> 4) % 3 + 5;
        }
        deck->cards[0][deck->count >> 1] = 0x334;
        deck->flags = 2;
    }
    data_00351230_shslot->extra[0] = 0xFFFE;
    data_00351241_shslot = CanStillGet3InARow_shslot = 0;
    for (i = 0; i < 5; i++) {
        if (data_00351230_shslot->extra[i] != 0xFFFF) {
            data_00351241_shslot |= (unsigned char)(1 << i);
        }
    }
    data_00351240_shslot[0].state = 0;
    data_00351248_shslot = data_00351230_shslot->count * 0x70;
    data_00351246_shslot = 0;
    data_00351210_shslot.active = 0;
    for (j = 0; j < 3; j++) {
        data_00351210_shslot.cell_timer[j][0] = data_00351210_shslot.cell_timer[j][1] = data_00351210_shslot.cell_timer[j][2] = 0;
    }
    MiddleStraightLineHighlight_shslot = TopStraightLineHighlight_shslot = BottomStraightLineHighlight_shslot = TopLeftDiagonalHighlight_shslot = BottomLeftDiagonalLineHighlight_shslot = 0;
    for (j = 0; j < 3; j++) {
        ((struct DotrShSlotReelRow *)&data_00351240_shslot[j])->position = 0;
        ((struct DotrShSlotReelRow *)&data_00351240_shslot[j])->velocity = 0;
        ((struct DotrShSlotReelRow *)&data_00351240_shslot[j])->center = 0xFFFF;
        data_00351240_shslot[j].enabled = 0;
        switch (j) {
        case 0:
            for (i = 0; i < data_00351230_shslot->count; i++) {
                ((struct DotrShSlotReelRow *)&(&data_00351240_shslot[0].state)[i * 2])->cards[0] = data_00351230_shslot->cards[0][i];
            }
            break;
        case 1:
        case 2:
            if (data_00351230_shslot->flags & 1) {
                for (i = 0; i < data_00351230_shslot->count; i++) {
                    ((struct DotrShSlotReelRow *)&(&data_00351240_shslot[j].state)[i * 2])->cards[0] = data_00351230_shslot->cards[j][i];
                }
            } else {
                for (i = 0; i < data_00351230_shslot->count; i++) {
                    ((struct DotrShSlotReelRow *)&(&data_00351240_shslot[j].state)[i * 2])->cards[0] = ((struct DotrShSlotReelRow *)&(&data_00351240_shslot[0].state)[i * 2])->cards[0];
                }
                /* Shuffle the copy: randomises the order the monsters appear in on this reel. */
                for (i = 0; i < data_00351230_shslot->count * 2; i++) {
                    a = (func_0019a5c8() >> 4) % data_00351230_shslot->count;
                    b = (func_0019a5c8() >> 4) % data_00351230_shslot->count;
                    temp = ((struct DotrShSlotReelRow *)&(&data_00351240_shslot[j].state)[a * 2])->cards[0];
                    ((struct DotrShSlotReelRow *)&(&data_00351240_shslot[j].state)[a * 2])->cards[0] = ((struct DotrShSlotReelRow *)&(&data_00351240_shslot[j].state)[b * 2])->cards[0];
                    ((struct DotrShSlotReelRow *)&(&data_00351240_shslot[j].state)[b * 2])->cards[0] = temp;
                }
            }
            break;
        }
    }
    if (!data_00351244_shslot) {
        ShSlot_Start();
    }
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

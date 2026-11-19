#include "dotr/ee/shslot.h"

/* ShSlot_Act (0x285f20): in demo mode pad 0x800 closes the popup and re-runs
 * ShSlot_Init(1), pad 0x100 pops the game state. Then by game phase (reel 0's state):
 * 0 waits for pad 0x4000 or an analogue stick value above 0xf0 (sound 0x20, ShSlot_Start);
 * 1 stops spinning reels 0/1/2 on pads 0x80/0x10/0x20 (sound 0x21, flash timer
 * (0x10, 0, 0xff), radius 40, speed 1, phase 3, countdown (0x70 - position % 112) /
 * velocity + 5, plus (0x70 / velocity) * ((random >> 4) % 3) with deck flags bit 1) and
 * moves to 2 with sound 0x1e stopped once all three are stopped; 2, once no flash runs,
 * goes to 3 (hit phase 0, wait 0x10) when no line is left, else starts the flash of the
 * lowest winning line (its line timer and its three cells set to 0x10) and drops its bit;
 * 3 steps the hit phase (0: wait, then phase 1, step 0, wait 8; 2: pad 0xf0 back to 1;
 * 6: wait, then game phase 4); 4 pops the game state outside demo mode.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md); the two accessors before it are compiled at the file's
 * default level. */
static inline struct DotrShSlotFlash *shslot_flash_at(int index)
{
    return &data_00351180_shslot[index];
}

static inline struct DotrShSlotReel *shslot_reel_at(int index)
{
    return &data_00351240_shslot[index];
}

#ifdef __MWERKS__
#pragma optimization_level 0
#endif
void ShSlot_Act(void)
{
    int i;
    int remainder1;
    int remainder2;
    int remainder3;

    if (data_00351244_shslot) {
        if (SzPad_Check(0, 0, 0x800) != 0) {
            SzDuel_CloseMsgPopup();
            ShSlot_Init(1);
        }
        if (SzPad_Check(0, 0, 0x100) != 0) {
            SzGame_Pop();
        }
    }
    switch (data_00351240_shslot[0].state) {
    case 0:
        if (SzPad_Check(0, 0, 0x4000) != 0 ||
            (SzPad_IsAnalog(0) != 0 && (SzPad_GetAnalogInfo(0, 3) & 0xFF) > 0xF0)) {
            SzSound_PlaySE(0, 0x20);
            ShSlot_Start();
        }
        break;
    case 1:
        if (data_00351240_shslot[0].enabled == 2 && SzPad_Check(0, 0, 0x80) != 0) {
            SzSound_PlaySE(0, 0x21);
            ShFx_initTimer(shslot_flash_at(0), 0x10, 0, 0xFF);
            shslot_flash_at(0)->radius = 40.0f;
            shslot_flash_at(0)->speed = 1.0f;
            shslot_reel_at(0)->enabled = 3;
            remainder1 = shslot_reel_at(0)->position % 112;
            shslot_reel_at(0)->countdown = (unsigned char)((0x70 - remainder1) / shslot_reel_at(0)->velocity + 5);
            if (data_00351230_shslot->flags & 2) {
                shslot_reel_at(0)->countdown +=
                    (unsigned char)((0x70 / shslot_reel_at(0)->velocity) * ((func_0019a5c8() >> 4) % 3));
            }
        }
        if (data_00351240_shslot[1].enabled == 2 && SzPad_Check(0, 0, 0x10) != 0) {
            SzSound_PlaySE(0, 0x21);
            ShFx_initTimer(shslot_flash_at(1), 0x10, 0, 0xFF);
            shslot_flash_at(1)->radius = 40.0f;
            shslot_flash_at(1)->speed = 1.0f;
            shslot_reel_at(1)->enabled = 3;
            remainder2 = shslot_reel_at(1)->position % 112;
            shslot_reel_at(1)->countdown = (unsigned char)((0x70 - remainder2) / shslot_reel_at(1)->velocity + 5);
            if (data_00351230_shslot->flags & 2) {
                shslot_reel_at(1)->countdown +=
                    (unsigned char)((0x70 / shslot_reel_at(1)->velocity) * ((func_0019a5c8() >> 4) % 3));
            }
        }
        if (data_00351240_shslot[2].enabled == 2 && SzPad_Check(0, 0, 0x20) != 0) {
            SzSound_PlaySE(0, 0x21);
            ShFx_initTimer(shslot_flash_at(2), 0x10, 0, 0xFF);
            shslot_flash_at(2)->radius = 40.0f;
            shslot_flash_at(2)->speed = 1.0f;
            shslot_reel_at(2)->enabled = 3;
            remainder3 = shslot_reel_at(2)->position % 112;
            shslot_reel_at(2)->countdown = (unsigned char)((0x70 - remainder3) / shslot_reel_at(2)->velocity + 5);
            if (data_00351230_shslot->flags & 2) {
                shslot_reel_at(2)->countdown +=
                    (unsigned char)((0x70 / shslot_reel_at(2)->velocity) * ((func_0019a5c8() >> 4) % 3));
            }
        }
        if (data_00351240_shslot[0].enabled == 4 && data_00351240_shslot[1].enabled == 4 && data_00351240_shslot[2].enabled == 4) {
            data_00351240_shslot[0].state = 2;
            SzSound_StopSE(0, 0x1E);
        }
        break;
    case 2:
        if (ShSlot_IsFlashStop() != 0) {
            if ((CanStillGet3InARow_shslot & 0x1F) == 0) {
                data_00351240_shslot[0].state = 3;
                data_00351178_shslot = 0;
                VisualDelayTimer_shslot = 0x10;
            } else {
                for (i = 0; i < 5; i++) {
                    if (CanStillGet3InARow_shslot & (1 << i)) {
                        switch (i) {
                        case 0:
                            data_00351210_shslot.active = 1;
                            MiddleStraightLineHighlight_shslot = data_00351210_shslot.cell_timer[0][1] = data_00351210_shslot.cell_timer[1][1] = data_00351210_shslot.cell_timer[2][1] = 0x10;
                            break;
                        case 1:
                            data_00351210_shslot.active = 1;
                            TopStraightLineHighlight_shslot = data_00351210_shslot.cell_timer[0][2] = data_00351210_shslot.cell_timer[1][2] = data_00351210_shslot.cell_timer[2][2] = 0x10;
                            break;
                        case 2:
                            data_00351210_shslot.active = 1;
                            BottomStraightLineHighlight_shslot = data_00351210_shslot.cell_timer[0][0] = data_00351210_shslot.cell_timer[1][0] = data_00351210_shslot.cell_timer[2][0] = 0x10;
                            break;
                        case 3:
                            data_00351210_shslot.active = 1;
                            TopLeftDiagonalHighlight_shslot = data_00351210_shslot.cell_timer[0][2] = data_00351210_shslot.cell_timer[1][1] = data_00351210_shslot.cell_timer[2][0] = 0x10;
                            break;
                        case 4:
                            data_00351210_shslot.active = 1;
                            BottomLeftDiagonalLineHighlight_shslot = data_00351210_shslot.cell_timer[0][0] = data_00351210_shslot.cell_timer[1][1] = data_00351210_shslot.cell_timer[2][2] = 0x10;
                            break;
                        }
                        CanStillGet3InARow_shslot &= (unsigned char)~(1 << i);
                        i = 5;
                    }
                }
            }
        }
        break;
    case 3:
        switch (data_00351178_shslot) {
        case 0:
            if (VisualDelayTimer_shslot) {
                VisualDelayTimer_shslot--;
            } else {
                data_00351178_shslot = 1;
                nextSlotDropNumber_shslot = 0;
                VisualDelayTimer_shslot = 8;
            }
            break;
        case 1:
            break;
        case 2:
            if (SzPad_Check(0, 0, 0xF0) != 0) {
                data_00351178_shslot = 1;
                VisualDelayTimer_shslot = 8;
            }
            break;
        case 6:
            if (VisualDelayTimer_shslot) {
                VisualDelayTimer_shslot--;
            } else {
                data_00351240_shslot[0].state = 4;
            }
            break;
        }
        break;
    case 4:
        if (!data_00351244_shslot) {
            SzGame_Pop();
        }
        break;
    }
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

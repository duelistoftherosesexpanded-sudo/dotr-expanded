#include "dotr/ee/shslot.h"

/* ShSlot_Draw_Pipe (0x284d30): moves each reel by its phase: 1 speeds up by 1 per frame to
 * 32 then becomes 2; 2 adds the velocity to position (wrapping at the reel length
 * 0x351248) and to angle (wrapping at 351); 3 brakes over its countdown (at 5 snaps
 * position to the next 112 step and sets velocity 16, 4..3 velocity -12, 2..1 velocity 4),
 * and at 0 stops (velocity 0, phase 4) and calls ShSlot_CheckLine. Then, with a non-zero
 * cursor, draws parts 0, the eye, the three reels, (while flashing: the flash boxes,
 * parts 2, and counts the line and cell timers above 1 down), parts 1, in game phase 3
 * with hit phase 1 the next hit request (a card: sound 25 and popup 3 after the reel
 * steps, else sound 7 and popup 2, hit phase 2; none: close the popup, hit phase 6 and
 * wait 8), and the three flash objects. Counts the frame and returns the cursor.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
DotrShSlotWord ShSlot_Draw_Pipe(DotrShSlotWord cursor)
{
    int i;
    int j;
    int speed;
    int brakingSpeed;
    int segment;

    for (i = 0; i < 3; i++) {
        switch (data_00351240_shslot[i].enabled) {
        case 0:
        case 4:
            break;
        case 1:
            if (data_00351240_shslot[i].velocity < 32) {
                data_00351240_shslot[i].velocity++;
            } else {
                data_00351240_shslot[i].enabled = 2;
            }
            /* fall through */
        case 2:
            speed = data_00351240_shslot[i].velocity;
            data_00351240_shslot[i].position += (short)speed;
            if (data_00351240_shslot[i].position > data_00351248_shslot) {
                data_00351240_shslot[i].position -= data_00351248_shslot;
            } else if (data_00351240_shslot[i].position < 0) {
                data_00351240_shslot[i].position += data_00351248_shslot;
            }
            data_00351240_shslot[i].angle += (short)speed;
            if (data_00351240_shslot[i].angle > 351) {
                data_00351240_shslot[i].angle -= 351;
            } else if (data_00351240_shslot[i].angle < 0) {
                data_00351240_shslot[i].angle += 351;
            }
            break;
        case 3:
            if (data_00351240_shslot[i].countdown < 6) {
                switch (data_00351240_shslot[i].countdown) {
                case 5:
                    segment = data_00351240_shslot[i].position / 112;
                    data_00351240_shslot[i].position = (short)((segment + 1) * 112);
                    data_00351240_shslot[i].velocity = 16;
                    break;
                case 4:
                case 3:
                    data_00351240_shslot[i].velocity = -12;
                    break;
                case 2:
                case 1:
                    data_00351240_shslot[i].velocity = 4;
                    break;
                }
            }
            brakingSpeed = data_00351240_shslot[i].velocity;
            data_00351240_shslot[i].position += (short)brakingSpeed;
            if (data_00351240_shslot[i].position > data_00351248_shslot) {
                data_00351240_shslot[i].position -= data_00351248_shslot;
            } else if (data_00351240_shslot[i].position < 0) {
                data_00351240_shslot[i].position += data_00351248_shslot;
            }
            data_00351240_shslot[i].angle += (short)brakingSpeed;
            if (data_00351240_shslot[i].angle > 351) {
                data_00351240_shslot[i].angle -= 351;
            } else if (data_00351240_shslot[i].angle < 0) {
                data_00351240_shslot[i].angle += 351;
            }
            data_00351240_shslot[i].countdown--;
            if (!data_00351240_shslot[i].countdown) {
                data_00351240_shslot[i].velocity = 0;
                data_00351240_shslot[i].enabled = 4;
                ShSlot_CheckLine(i);
            }
            break;
        }
    }
    data_00351228_shslot = cursor;
    if (data_00351228_shslot != 0) {
        ShSlot_DrawParts(0);
        ShSlot_DrawEye();
        for (j = 0; j < 3; j++) {
            ShSlot_DrawReel(j);
        }
        if (data_00351210_shslot.active != 0) {
            ShSlot_FlashBox();
            ShSlot_DrawParts(2);
            for (i = 0; i < 5; i++) {
                if (data_00351210_shslot.line_timer[i] > 1) {
                    data_00351210_shslot.line_timer[i]--;
                }
            }
            for (j = 0; j < 3; j++) {
                for (i = 0; i < 3; i++) {
                    if (data_00351210_shslot.cell_timer[j][i] > 1) {
                        data_00351210_shslot.cell_timer[j][i]--;
                    }
                }
            }
        }
        ShSlot_DrawParts(1);
        if (data_00351240_shslot[0].state == 3) {
            switch (data_00351178_shslot) {
            case 1:
                ShSlot_HitCardRequest();
                if (nextSlotDrop_shslot != 0xFFFF) {
                    if (nextSlotDropNumber_shslot > 3) {
                        SzSound_PlaySE(0, 25);
                        SzDuel_OpenMsgPopup(3, 0, 0, nextSlotDrop_shslot);
                        data_00351178_shslot = 2;
                        VisualDelayTimer_shslot = 0;
                    } else {
                        SzSound_PlaySE(0, 7);
                        SzDuel_OpenMsgPopup(2, 0, 0, nextSlotDrop_shslot);
                        data_00351178_shslot = 2;
                        VisualDelayTimer_shslot = 0;
                    }
                } else {
                    SzDuel_CloseMsgPopup();
                    data_00351178_shslot = 6;
                    VisualDelayTimer_shslot = 8;
                }
                break;
            }
        }
        ShSlot_Flear(data_00351180_shslot, 3);
    }
    data_00351246_shslot++;
    return data_00351228_shslot;
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

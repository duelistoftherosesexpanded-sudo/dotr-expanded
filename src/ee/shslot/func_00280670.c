#include "dotr/ee/shslot.h"

/* ShSlot_Start (0x280670): sets the three reels' phase to 1 (speeding up) and the game
 * phase (reel 0's state) to 1, plays sound effect 0x1e, then starts the three flash
 * objects: timer (0x28, 0, 0xff), flags 0x80, 9 divisions, radius 40, speed 0, colours
 * 0x80/0x10/0x40 rim and 0xf0 centre and alpha, at x = 0x78e + reel * 0x70, y = 2249.
 * The winning-lines mask starts as the lines in play; the frame counter is cleared.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
void ShSlot_Start(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        data_00351240_shslot[i].enabled = 1;
    }
    data_00351240_shslot[0].state = 1;
    SzSound_PlaySE(0, 0x1E);
    for (i = 0; i < 3; i++) {
        ShFx_initTimer(&data_00351180_shslot[i], 0x28, 0, 0xFF);
        data_00351180_shslot[i].flags = 0x80;
        data_00351180_shslot[i].divisions = 9;
        data_00351180_shslot[i].radius = 40.0f;
        data_00351180_shslot[i].speed = 0.0f;
        data_00351180_shslot[i].center_r = data_00351180_shslot[i].center_g = data_00351180_shslot[i].center_b = data_00351180_shslot[i].alpha = 0xF0;
        data_00351180_shslot[i].r = 0x40;
        data_00351180_shslot[i].g = 0x10;
        data_00351180_shslot[i].b = 0x80;
        data_00351180_shslot[i].x = (float) (i * 0x70 + 0x78E);
        data_00351180_shslot[i].y = 2249.0f;
    }
    CanStillGet3InARow_shslot = data_00351241_shslot;
    data_00351246_shslot = 0;
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

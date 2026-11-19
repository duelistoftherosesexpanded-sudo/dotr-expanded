/* Synthetic host tests for the camp_ivent_ga group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/camp_ivent_ga.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

union DotrIventGaBoard dotr_camp_ivent_ga_board;
union DotrIventGaWork dotr_camp_ivent_ga_work;
DotrIventGaTex tex_uv_data[0x400];
DotrIventGaObj data_0029abf0_iventga;
DotrIventGaObj4 data_0029ac20_iventga;
DotrIventGaObj4 data_0029acc0_iventga;
DotrIventGaObj5 data_0029ad60_iventga;
DotrIventGaObj data_0029ae20_iventga;
DotrIventGaObj data_0029ae50_iventga;
DotrIventGaObj data_0029ae80_iventga;
DotrIventGaPair data_0029aea8_iventga;

int Taku_G4_Set(int cursor, void *src, short x, short y) { return cursor + 1; }
int Taku_GT4_Set(int cursor, void *src, short x, short y, short dx, short dy) { return cursor + 1; }
int Taku_Obj_Set(int cursor, void *src, short dx, short dy) { return cursor + 1; }

int main(void)
{
    /* Event 0x2b72 with every work-area flag clear: the four frame texture entries from 0x94 on
     * get the event's frame size 0x280 x 0x180. */
    CutSceneNumber_iventga = 0x2B72;
    Taku_Camp_Ivent_Ga(0x1000);
    assert(tex_uv_data[0x95].f[2] == 0x280 && tex_uv_data[0x95].f[3] == 0x180);
    return 0;
}

#include "dotr/ee/shslot.h"

/* ShSlot_FlashBox (0x2831a0): loads texture (9, 1), then for every cell whose timer at
 * 0x351210 is non-zero draws its frame as four 0x39-unit corner sprites around
 * (0x760 + reel * 0x70, 0x840 - row * 0x70), each mirrored by swapping the vertex pairs,
 * grey level 0x2b8740[timer] with half of it as alpha; each corner is sent twice, with
 * alpha modes 0x44 and 0x49, through ExSortPrim_Pipe on the cursor at 0x351228.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
void ShSlot_FlashBox(void)
{
    struct DotrShSlotTexPacket packet;
    int y;
    int x;
    int k;
    DotrShSlotWord handle;
    int j;
    int i;
    int size;

    handle = SzText_LoadRequest(9, 1);
    size = RhBase_GetUserGsAddress();
    size += SzText_InitClutManage(size, 1);
    data_00351228_shslot = SzText_DoRequest_Pipe(data_00351228_shslot, 9, size, 0);
    data_00351228_shslot = RhText_SetTextureFlush(data_00351228_shslot);
    SzText_FreeRequest(9, -1);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (data_00351210_shslot.cell_timer[i][j]) {
                x = i * 0x70 + 0x760;
                y = 0x840 - j * 0x70;
                ((struct DotrShSlotTag *)&packet.header[0])->flags = 0x24;
                ((struct DotrShSlotTag *)&packet.header[0])->count = 9;
                ((struct DotrShSlotTag *)&packet.header[0])->registers = 0x424242421LL;
                ((struct DotrShSlotTag *)&packet.header[0])->reserved = 0;
                packet.color.r = packet.color.g = packet.color.b = gSh_Lamp_shslot[data_00351210_shslot.cell_timer[i][j]];
                packet.color.a = packet.color.r >> 1;
                packet.color.q = 1.0f;
                packet.v0.xyz.z = packet.v1.xyz.z = packet.v2.xyz.z = packet.v3.xyz.z = 0xFFFFF8;
                packet.v0.xyz.f = packet.v1.xyz.f = packet.v2.xyz.f = packet.v3.xyz.f = 0;
                ((struct DotrShSlotTag *)&packet.header[0])->flags |= 0x800;
                ((struct DotrShSlotUv *)&packet.v0.texture)->u = 0x440;
                ((struct DotrShSlotUv *)&packet.v0.texture)->v = 0x390;
                ((struct DotrShSlotUv *)&packet.v1.texture)->u = 0x7D0;
                ((struct DotrShSlotUv *)&packet.v1.texture)->v = 0x390;
                ((struct DotrShSlotUv *)&packet.v2.texture)->u = 0x440;
                ((struct DotrShSlotUv *)&packet.v2.texture)->v = 0x720;
                ((struct DotrShSlotUv *)&packet.v3.texture)->u = 0x7D0;
                ((struct DotrShSlotUv *)&packet.v3.texture)->v = 0x720;

                for (k = 0; k < 4; k++) {
                    switch (k) {
                    case 0:
                        packet.v2.xyz.x = packet.v0.xyz.x = (x - 9) * 16;
                        packet.v1.xyz.y = packet.v0.xyz.y = (y - 9) * 16;
                        packet.v3.xyz.x = packet.v1.xyz.x = packet.v0.xyz.x + 0x390;
                        packet.v3.xyz.y = packet.v2.xyz.y = packet.v0.xyz.y + 0x390;
                        break;
                    case 1:
                        packet.v3.xyz.x = packet.v1.xyz.x = (x + 0x30) * 16;
                        packet.v1.xyz.y = packet.v0.xyz.y = (y - 9) * 16;
                        packet.v2.xyz.x = packet.v0.xyz.x = packet.v1.xyz.x + 0x390;
                        packet.v3.xyz.y = packet.v2.xyz.y = packet.v0.xyz.y + 0x390;
                        break;
                    case 2:
                        packet.v2.xyz.x = packet.v0.xyz.x = (x - 9) * 16;
                        packet.v3.xyz.y = packet.v2.xyz.y = (y + 0x30) * 16;
                        packet.v3.xyz.x = packet.v1.xyz.x = packet.v0.xyz.x + 0x390;
                        packet.v1.xyz.y = packet.v0.xyz.y = packet.v2.xyz.y + 0x390;
                        break;
                    case 3:
                        packet.v3.xyz.x = packet.v1.xyz.x = (x + 0x30) * 16;
                        packet.v3.xyz.y = packet.v2.xyz.y = (y + 0x30) * 16;
                        packet.v2.xyz.x = packet.v0.xyz.x = packet.v1.xyz.x + 0x390;
                        packet.v1.xyz.y = packet.v0.xyz.y = packet.v2.xyz.y + 0x390;
                        break;
                    }
                    ExAssignClamp(&packet.header[0], 0x1C4391F044ALL);
                    ExAssignTexture(&packet.header[0], (void *)handle, 1);
                    ExAssignAlpha(&packet.header[0], 0x44);
                    data_00351228_shslot = ExSortPrim_Pipe(data_00351228_shslot, &packet.header[0], 0, -1);
                    ExAssignAlpha(&packet.header[0], 0x49);
                    data_00351228_shslot = ExSortPrim_Pipe(data_00351228_shslot, &packet.header[0], 0, -1);
                }
            }
        }
    }
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

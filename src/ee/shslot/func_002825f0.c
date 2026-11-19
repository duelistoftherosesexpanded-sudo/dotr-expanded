#include "dotr/ee/shslot.h"

/* ShSlot_DrawEye (0x2825f0): with pulse = table 0x2b8e40[frame & 0xf], draws for every
 * reel not yet stopped (phase != 4) a 9-register sprite from texture (3, 7) at
 * x = 0x77e + reel * 0x70 + pulse / 4, y = 0x8b8 + pulse / 4, size 0x20 - pulse / 2, with a
 * per-reel texture rectangle (alpha mode 0x44); then from texture (9, 0) a fixed
 * 0x22 x 0x22 sprite at (0x77d + reel * 0x70, 0x8b7) coloured 0x60/0x80/0x80 with alpha
 * pulse * 3 (mode 0x48). Packets go through ExSortPrim_Pipe on the cursor at 0x351228.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
void ShSlot_DrawEye(void)
{
    struct DotrShSlotPulse pulse;
    struct DotrShSlotEyePacket packet __attribute__((aligned(8)));
    int i;
    DotrShSlotWord handle;
    int x;
    int y;
    int width;
    int size;

    pulse = data_002b8e40_shslot;
    handle = SzText_LoadRequest(3, 7);
    size = RhBase_GetUserGsAddress();
    size += SzText_InitClutManage(size, 1);
    data_00351228_shslot = SzText_DoRequest_Pipe(data_00351228_shslot, 3, size, 0);
    data_00351228_shslot = RhText_SetTextureFlush(data_00351228_shslot);
    SzText_FreeRequest(3, -1);
    ((struct DotrShSlotTag *)&packet.header[0])->flags = 0x24;
    ((struct DotrShSlotTag *)&packet.header[0])->count = 9;
    ((struct DotrShSlotTag *)&packet.header[0])->registers = 0x424242421LL;
    ((struct DotrShSlotTag *)&packet.header[0])->reserved = 0;
    packet.color.r = packet.color.g = packet.color.b = 0x80;
    packet.color.a = 0x80;
    packet.color.q = 1.0f;
    packet.v0.xyz.z = packet.v1.xyz.z = packet.v2.xyz.z = packet.v3.xyz.z = 0xFFFFF8;
    packet.v0.xyz.f = packet.v1.xyz.f = packet.v2.xyz.f = packet.v3.xyz.f = 0;
    y = (pulse.values[data_00351246_shslot & 0xF] >> 2) + 0x8B8;
    x = (pulse.values[data_00351246_shslot & 0xF] >> 2) + 0x77E;
    width = 0x20 - (pulse.values[data_00351246_shslot & 0xF] >> 1);
    for (i = 0; i < 3; i++) {
        if (data_00351240_shslot[i].enabled != 4) {
            packet.v2.xyz.x = packet.v0.xyz.x = (x + i * 0x70) << 4;
            packet.v1.xyz.y = packet.v0.xyz.y = y << 4;
            packet.v3.xyz.x = packet.v1.xyz.x = packet.v0.xyz.x + (width << 4);
            packet.v3.xyz.y = packet.v2.xyz.y = packet.v0.xyz.y + (width << 4);
            ((struct DotrShSlotTag *)&packet.header[0])->flags |= 0x800;
            switch (i) {
            case 0:
                ((struct DotrShSlotUv *)&packet.v0.texture)->u = 0x160;
                ((struct DotrShSlotUv *)&packet.v0.texture)->v = 0xB00;
                ((struct DotrShSlotUv *)&packet.v1.texture)->u = 0x2C0;
                ((struct DotrShSlotUv *)&packet.v1.texture)->v = 0xB00;
                ((struct DotrShSlotUv *)&packet.v2.texture)->u = 0x160;
                ((struct DotrShSlotUv *)&packet.v2.texture)->v = 0xC60;
                ((struct DotrShSlotUv *)&packet.v3.texture)->u = 0x2B0;
                ((struct DotrShSlotUv *)&packet.v3.texture)->v = 0xC60;
                break;
            case 1:
                ((struct DotrShSlotUv *)&packet.v0.texture)->u = 0;
                ((struct DotrShSlotUv *)&packet.v0.texture)->v = 0xB00;
                ((struct DotrShSlotUv *)&packet.v1.texture)->u = 0x160;
                ((struct DotrShSlotUv *)&packet.v1.texture)->v = 0xB00;
                ((struct DotrShSlotUv *)&packet.v2.texture)->u = 0;
                ((struct DotrShSlotUv *)&packet.v2.texture)->v = 0xC60;
                ((struct DotrShSlotUv *)&packet.v3.texture)->u = 0x160;
                ((struct DotrShSlotUv *)&packet.v3.texture)->v = 0xC60;
                break;
            case 2:
                ((struct DotrShSlotUv *)&packet.v0.texture)->u = 0x420;
                ((struct DotrShSlotUv *)&packet.v0.texture)->v = 0xB00;
                ((struct DotrShSlotUv *)&packet.v1.texture)->u = 0x580;
                ((struct DotrShSlotUv *)&packet.v1.texture)->v = 0xB00;
                ((struct DotrShSlotUv *)&packet.v2.texture)->u = 0x420;
                ((struct DotrShSlotUv *)&packet.v2.texture)->v = 0xC60;
                ((struct DotrShSlotUv *)&packet.v3.texture)->u = 0x580;
                ((struct DotrShSlotUv *)&packet.v3.texture)->v = 0xC60;
                break;
            }
            ExAssignTexture(&packet.header[0], (void *)handle, 0);
            ExAssignAlpha(&packet.header[0], 0x44);
            data_00351228_shslot = ExSortPrim_Pipe(data_00351228_shslot, &packet.header[0], 0, -1);
        }
    }

    handle = SzText_LoadRequest(9, 0);
    size = RhBase_GetUserGsAddress();
    size += SzText_InitClutManage(size, 1);
    data_00351228_shslot = SzText_DoRequest_Pipe(data_00351228_shslot, 9, size, 0);
    data_00351228_shslot = RhText_SetTextureFlush(data_00351228_shslot);
    SzText_FreeRequest(9, -1);
    ((struct DotrShSlotTag *)&packet.header[0])->flags = 0x24;
    ((struct DotrShSlotTag *)&packet.header[0])->count = 9;
    ((struct DotrShSlotTag *)&packet.header[0])->registers = 0x424242421LL;
    ((struct DotrShSlotTag *)&packet.header[0])->reserved = 0;
    packet.color.r = 0x60;
    packet.color.g = packet.color.b = 0x80;
    packet.color.a = pulse.values[data_00351246_shslot & 0xF] * 3;
    packet.color.q = 1.0f;
    packet.v0.xyz.z = packet.v1.xyz.z = packet.v2.xyz.z = packet.v3.xyz.z = 0xFFFFF8;
    packet.v0.xyz.f = packet.v1.xyz.f = packet.v2.xyz.f = packet.v3.xyz.f = 0;
    y = 0x8B7;
    x = 0x77D;
    for (i = 0; i < 3; i++) {
        if (data_00351240_shslot[i].enabled != 4) {
            packet.v2.xyz.x = packet.v0.xyz.x = (x + i * 0x70) << 4;
            packet.v1.xyz.y = packet.v0.xyz.y = y << 4;
            packet.v3.xyz.x = packet.v1.xyz.x = packet.v0.xyz.x + 0x220;
            packet.v3.xyz.y = packet.v2.xyz.y = packet.v0.xyz.y + 0x220;
            ((struct DotrShSlotTag *)&packet.header[0])->flags |= 0x800;
            ((struct DotrShSlotUv *)&packet.v0.texture)->u = 0x160;
            ((struct DotrShSlotUv *)&packet.v0.texture)->v = 0x600;
            ((struct DotrShSlotUv *)&packet.v1.texture)->u = 0x2E0;
            ((struct DotrShSlotUv *)&packet.v1.texture)->v = 0x600;
            ((struct DotrShSlotUv *)&packet.v2.texture)->u = 0x160;
            ((struct DotrShSlotUv *)&packet.v2.texture)->v = 0x780;
            ((struct DotrShSlotUv *)&packet.v3.texture)->u = 0x2E0;
            ((struct DotrShSlotUv *)&packet.v3.texture)->v = 0x780;
            ExAssignTexture(&packet.header[0], (void *)handle, 0);
            ExAssignAlpha(&packet.header[0], 0x48);
            data_00351228_shslot = ExSortPrim_Pipe(data_00351228_shslot, &packet.header[0], 0, -1);
        }
    }
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

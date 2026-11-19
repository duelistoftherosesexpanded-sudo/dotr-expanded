#include "dotr/ee/shslot.h"

/* ShSlot_DrawReel (0x283880): draws reel `reel` at x = 0x75c + reel * 0x70: the scrolling
 * background from texture (9, 0) in three pieces (scroll = angle % 176, the halves of
 * the texture swapped once angle reaches 176); then the four visible symbols
 * (symbols[(position / 112 + i) % count], images from SzText_GetReadyItem(1, card)
 * loaded with SzText_Load_Pipe) 0x70 apart, clipped to the window rows 0x30..400 with the
 * texture v range scaled to match; then four Gouraud shading bands (0x80 to 0x20, 0x20 to
 * 0, 0 to 0x20, 0x20 to 0x80; heights 0x40, 0x70, 0x70, 0x40) over the window.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
void ShSlot_DrawReel(int reel)
{

    int i;
    int u0;
    int u1;
    int scroll;
    int y;
    int shadeY;
    int shadeHeight;
    int top;
    int bottom;
    struct DotrShSlotReelWork work;
    int position;
    int x;
    int imageIndex;
    int height;
    int size;
    int backgroundX;
    int backgroundY;

    work.background = (int *) SzText_LoadRequest(9, 0);
    position = RhBase_GetUserGsAddress();
    position += SzText_InitClutManage(position, 1);
    data_00351228_shslot = SzText_DoRequest_Pipe(data_00351228_shslot, 9, position, 0);
    data_00351228_shslot = RhText_SetTextureFlush(data_00351228_shslot);
    SzText_FreeRequest(9, -1);
    scroll = data_00351240_shslot[reel].angle % 176;

    ((struct DotrShSlotTag *) &work.textured.header[0])->flags = 0x24;
    ((struct DotrShSlotTag *) &work.textured.header[0])->count = 9;
    ((struct DotrShSlotTag *) &work.textured.header[0])->registers = 0x424242421LL;
    ((struct DotrShSlotTag *) &work.textured.header[0])->reserved = 0;
    work.textured.color.r = work.textured.color.g = work.textured.color.b = 0x80;
    work.textured.color.a = 0x80;
    work.textured.color.q = 1.0f;
    work.textured.v0.xyz.z = work.textured.v1.xyz.z = work.textured.v2.xyz.z = work.textured.v3.xyz.z = 0xFFFFF8;
    work.textured.v0.xyz.f = work.textured.v1.xyz.f = work.textured.v2.xyz.f = work.textured.v3.xyz.f = 0;

    backgroundY = 0x750;
    backgroundX = reel * 0x70 + 0x75C;
    for (i = 0; i < 3; i++) {
        switch (i) {
        case 0:
            work.textured.v2.xyz.x = work.textured.v0.xyz.x = backgroundX << 4;
            work.textured.v1.xyz.y = work.textured.v0.xyz.y = (backgroundY + scroll) << 4;
            work.textured.v3.xyz.x = work.textured.v1.xyz.x = work.textured.v0.xyz.x + 0x680;
            work.textured.v3.xyz.y = work.textured.v2.xyz.y = work.textured.v0.xyz.y + 0xB00;
            ((struct DotrShSlotTag *) &work.textured.header[0])->flags |= 0x800;
            if (data_00351240_shslot[reel].angle < 176) {
                u0 = 0x30;
                u1 = 0x98;
            } else {
                u0 = 0x98;
                u1 = 0x100;
            }
            ((struct DotrShSlotTag *) &work.textured.header[0])->flags |= 0x800;
            ((struct DotrShSlotUv *) &work.textured.v0.texture)->u = (unsigned short) (u0 << 4);
            ((struct DotrShSlotUv *) &work.textured.v0.texture)->v = 0x500;
            ((struct DotrShSlotUv *) &work.textured.v1.texture)->u = (unsigned short) (u1 << 4);
            ((struct DotrShSlotUv *) &work.textured.v1.texture)->v = 0x500;
            ((struct DotrShSlotUv *) &work.textured.v2.texture)->u = (unsigned short) (u0 << 4);
            ((struct DotrShSlotUv *) &work.textured.v2.texture)->v = 0x1000;
            ((struct DotrShSlotUv *) &work.textured.v3.texture)->u = (unsigned short) (u1 << 4);
            ((struct DotrShSlotUv *) &work.textured.v3.texture)->v = 0x1000;
            break;
        case 1:
            work.textured.v2.xyz.x = work.textured.v0.xyz.x = backgroundX << 4;
            work.textured.v1.xyz.y = work.textured.v0.xyz.y = backgroundY << 4;
            work.textured.v3.xyz.x = work.textured.v1.xyz.x = work.textured.v0.xyz.x + 0x680;
            work.textured.v3.xyz.y = work.textured.v2.xyz.y = work.textured.v0.xyz.y + (scroll << 4);
            if (data_00351240_shslot[reel].angle < 176) {
                u0 = 0x98;
                u1 = 0x100;
            } else {
                u0 = 0x30;
                u1 = 0x98;
            }
            ((struct DotrShSlotTag *) &work.textured.header[0])->flags |= 0x800;
            ((struct DotrShSlotUv *) &work.textured.v0.texture)->u = (unsigned short) (u0 << 4);
            ((struct DotrShSlotUv *) &work.textured.v0.texture)->v = (unsigned short) ((0x100 - scroll) * 16);
            ((struct DotrShSlotUv *) &work.textured.v1.texture)->u = (unsigned short) (u1 << 4);
            ((struct DotrShSlotUv *) &work.textured.v1.texture)->v = (unsigned short) ((0x100 - scroll) * 16);
            ((struct DotrShSlotUv *) &work.textured.v2.texture)->u = (unsigned short) (u0 << 4);
            ((struct DotrShSlotUv *) &work.textured.v2.texture)->v = 0x1000;
            ((struct DotrShSlotUv *) &work.textured.v3.texture)->u = (unsigned short) (u1 << 4);
            ((struct DotrShSlotUv *) &work.textured.v3.texture)->v = 0x1000;
            break;
        case 2:
            work.textured.v2.xyz.x = work.textured.v0.xyz.x = backgroundX << 4;
            work.textured.v1.xyz.y = work.textured.v0.xyz.y = (backgroundY + 0xB0 + scroll) << 4;
            work.textured.v3.xyz.x = work.textured.v1.xyz.x = work.textured.v0.xyz.x + 0x680;
            work.textured.v3.xyz.y = work.textured.v2.xyz.y = work.textured.v0.xyz.y + ((0xB0 - scroll) << 4);
            ((struct DotrShSlotTag *) &work.textured.header[0])->flags |= 0x800;
            ((struct DotrShSlotUv *) &work.textured.v0.texture)->u = (unsigned short) (u0 << 4);
            ((struct DotrShSlotUv *) &work.textured.v0.texture)->v = 0x500;
            ((struct DotrShSlotUv *) &work.textured.v1.texture)->u = (unsigned short) (u1 << 4);
            ((struct DotrShSlotUv *) &work.textured.v1.texture)->v = 0x500;
            ((struct DotrShSlotUv *) &work.textured.v2.texture)->u = (unsigned short) (u0 << 4);
            ((struct DotrShSlotUv *) &work.textured.v2.texture)->v = (unsigned short) ((0x100 - scroll) * 16);
            ((struct DotrShSlotUv *) &work.textured.v3.texture)->u = (unsigned short) (u1 << 4);
            ((struct DotrShSlotUv *) &work.textured.v3.texture)->v = (unsigned short) ((0x100 - scroll) * 16);
            break;
        }
        ExAssignClamp(&work.textured.header[0], ((long long) u0 << 4) | 0xA | ((long long) u1 << 14) | 0x50000000 | ((unsigned long long) 0x3FC << 32));
        ExAssignTexture(&work.textured.header[0], work.background, 1);
        ExAssignAlpha(&work.textured.header[0], 0x44);
        data_00351228_shslot = ExSortPrim_Pipe(data_00351228_shslot, &work.textured.header[0], 0, -1);
    }

    imageIndex = data_00351240_shslot[reel].position / 112;
    scroll = data_00351240_shslot[reel].position % 112;
    position = RhBase_GetUserGsAddress();
    position += SzText_InitClutManage(position, 1);
    for (i = 0; i < 4; i++) {
        work.images[i] = SzText_GetReadyItem(1, data_00351240_shslot[reel].symbols[(imageIndex + i) % *data_00351230_shslot_bytes]);
        if (work.images[i] != 0) {
            data_00351228_shslot = SzText_Load_Pipe(data_00351228_shslot, *(int *) work.images[i], position, &size);
            position += size;
        }
    }
    data_00351228_shslot = RhText_SetTextureFlush(data_00351228_shslot);
    for (i = 0; i < 4; i++) {
        if (work.images[i] != 0) {
            ((struct DotrShSlotTag *) &work.textured.header[0])->flags = 0x24;
            ((struct DotrShSlotTag *) &work.textured.header[0])->count = 9;
            ((struct DotrShSlotTag *) &work.textured.header[0])->registers = 0x424242421LL;
            ((struct DotrShSlotTag *) &work.textured.header[0])->reserved = 0;
            work.textured.color.r = work.textured.color.g = work.textured.color.b = work.textured.color.a = 0x80;
            work.textured.color.q = 1.0f;
            work.textured.v0.xyz.z = work.textured.v1.xyz.z = work.textured.v2.xyz.z = work.textured.v3.xyz.z = 0xFFFFF8;
            work.textured.v0.xyz.f = work.textured.v1.xyz.f = work.textured.v2.xyz.f = work.textured.v3.xyz.f = 0;

            x = reel * 0x70 + 0xA0;
            work.textured.v2.xyz.x = work.textured.v0.xyz.x = (x + 0x6C0) << 4;
            work.textured.v3.xyz.x = work.textured.v1.xyz.x = work.textured.v0.xyz.x + 0x600;
            y = scroll + (0x120 - i * 0x70);
            if (y < 0x30) {
                top = ((0x30 - y) << 7) / 96;
                height = 0x60 - (0x30 - y);
                y = 0x30;
            } else {
                height = 0x60;
                top = 0;
            }
            if (y + 0x60 > 400) {
                bottom = ((400 - y) << 7) / 96;
                height = 400 - y;
            } else {
                bottom = 0x80;
            }
            work.textured.v1.xyz.y = work.textured.v0.xyz.y = (y + 0x720) << 4;
            work.textured.v3.xyz.y = work.textured.v2.xyz.y = work.textured.v0.xyz.y + (height << 4);
            ((struct DotrShSlotTag *) &work.textured.header[0])->flags |= 0x800;
            ((struct DotrShSlotUv *) &work.textured.v0.texture)->u = 0;
            ((struct DotrShSlotUv *) &work.textured.v0.texture)->v = (unsigned short) (top << 4);
            ((struct DotrShSlotUv *) &work.textured.v1.texture)->u = 0x800;
            ((struct DotrShSlotUv *) &work.textured.v1.texture)->v = (unsigned short) (top << 4);
            ((struct DotrShSlotUv *) &work.textured.v2.texture)->u = 0;
            ((struct DotrShSlotUv *) &work.textured.v2.texture)->v = (unsigned short) (bottom << 4);
            ((struct DotrShSlotUv *) &work.textured.v3.texture)->u = 0x800;
            ((struct DotrShSlotUv *) &work.textured.v3.texture)->v = (unsigned short) (bottom << 4);
            ExAssignClamp(&work.textured.header[0], ((long long) top << 24) | 0x20000A | ((long long) bottom << 34));
            ExAssignTexture(&work.textured.header[0], *(void **) work.images[i], 1);
            data_00351228_shslot = ExSortPrim_Pipe(data_00351228_shslot, &work.textured.header[0], 0, -1);
        }
    }
    ((struct DotrShSlotTag *) &work.gradient.header[0])->flags = 0x14;
    ((struct DotrShSlotTag *) &work.gradient.header[0])->count = 8;
    ((struct DotrShSlotTag *) &work.gradient.header[0])->registers = 0x41414141LL;
    ((struct DotrShSlotTag *) &work.gradient.header[0])->reserved = 0;
    x = reel * 0x70 + 0x9C;
    for (i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            work.gradient.v0.color.r = work.gradient.v0.color.g = work.gradient.v0.color.b =
                work.gradient.v1.color.r = work.gradient.v1.color.g = work.gradient.v1.color.b = 0x80;
            work.gradient.v2.color.r = work.gradient.v2.color.g = work.gradient.v2.color.b =
                work.gradient.v3.color.r = work.gradient.v3.color.g = work.gradient.v3.color.b = 0x20;
            shadeY = 0x30;
            shadeHeight = 0x40;
            break;
        case 1:
            work.gradient.v0.color.r = work.gradient.v0.color.g = work.gradient.v0.color.b =
                work.gradient.v1.color.r = work.gradient.v1.color.g = work.gradient.v1.color.b = 0x20;
            work.gradient.v2.color.r = work.gradient.v2.color.g = work.gradient.v2.color.b =
                work.gradient.v3.color.r = work.gradient.v3.color.g = work.gradient.v3.color.b = 0;
            shadeY += 0x40;
            shadeHeight = 0x70;
            break;
        case 2:
            work.gradient.v0.color.r = work.gradient.v0.color.g = work.gradient.v0.color.b =
                work.gradient.v1.color.r = work.gradient.v1.color.g = work.gradient.v1.color.b = 0;
            work.gradient.v2.color.r = work.gradient.v2.color.g = work.gradient.v2.color.b =
                work.gradient.v3.color.r = work.gradient.v3.color.g = work.gradient.v3.color.b = 0x20;
            shadeY += 0x70;
            shadeHeight = 0x70;
            break;
        case 3:
            work.gradient.v0.color.r = work.gradient.v0.color.g = work.gradient.v0.color.b =
                work.gradient.v1.color.r = work.gradient.v1.color.g = work.gradient.v1.color.b = 0x20;
            work.gradient.v2.color.r = work.gradient.v2.color.g = work.gradient.v2.color.b =
                work.gradient.v3.color.r = work.gradient.v3.color.g = work.gradient.v3.color.b = 0x80;
            shadeY += 0x70;
            shadeHeight = 0x40;
            break;
        }
        work.gradient.v0.color.a = work.gradient.v1.color.a = work.gradient.v2.color.a = work.gradient.v3.color.a = 0x80;
        work.gradient.v0.color.q = work.gradient.v1.color.q = work.gradient.v2.color.q = work.gradient.v3.color.q = 1.0f;
        work.gradient.v0.xyz.z = work.gradient.v1.xyz.z = work.gradient.v2.xyz.z = work.gradient.v3.xyz.z = 0xFFFFF8;
        work.gradient.v0.xyz.f = work.gradient.v1.xyz.f = work.gradient.v2.xyz.f = work.gradient.v3.xyz.f = 0;
        work.gradient.v2.xyz.x = work.gradient.v0.xyz.x = (x + 0x6C0) << 4;
        work.gradient.v1.xyz.y = work.gradient.v0.xyz.y = (shadeY + 0x720) << 4;
        work.gradient.v3.xyz.x = work.gradient.v1.xyz.x = work.gradient.v0.xyz.x + 0x680;
        work.gradient.v3.xyz.y = work.gradient.v2.xyz.y = work.gradient.v0.xyz.y + (shadeHeight << 4);
        ExAssignAlpha(&work.gradient.header[0], 0x42 | ((unsigned long long) 0x80 << 32));
        data_00351228_shslot = ExSortPrim_Pipe(data_00351228_shslot, &work.gradient.header[0], 0, -1);
    }
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

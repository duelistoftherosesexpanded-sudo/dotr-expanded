#include "dotr/ee/shslot_parts.h"

/* 0x2818a0. Compiled with `#pragma optimization_level 0`, set directly before the function and
 * reset after it. Data views carry the _parts suffix; the cursor and handles are DotrShPartsWord.
 * Some declarations are inside the body. */
#ifndef __MWERKS__
/* Host build only: this C calls two functions without a prototype and switches over
 * part kinds without a default. */
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif

#ifdef __MWERKS__
#pragma optimization_level 0
#endif
void ShSlot_DrawParts(int arg0) {
    extern int SzText_InitClutManage();
    extern int ExSortPrim_Pipe();
    extern DotrShPartsItems data_002b87a0_parts;
    extern DotrShPartsPulse data_002b8e30_parts;
    extern struct { unsigned char pad[4]; unsigned char lv[12]; } data_00351210_parts;
    extern unsigned char gSh_Lamp_parts[];
    int i;
    int bit;
    int skip;
    int tmp;
    DotrShPartsItems items;
    DotrShPartsPulse pulse;
    void *handles[2];
    DotrShPartsPacket packet __attribute__((aligned(8)));

    items = data_002b87a0_parts;
    pulse = data_002b8e30_parts;
    switch (arg0) {
    case 0:
        i = 0;
        break;
    case 1:
        i = 0x16;
        break;
    case 2:
        i = 0x35;
        break;
    }
    handles[0] = (void *)SzText_LoadRequest(9, 0);
    handles[1] = (void *)SzText_LoadRequest(9, 1);
    tmp = RhBase_GetUserGsAddress();
    tmp += SzText_InitClutManage(tmp, 1);
    data_00351228_parts = SzText_DoRequest_Pipe(data_00351228_parts, 9, tmp, NULL);
    data_00351228_parts = RhText_SetTextureFlush(data_00351228_parts);
    SzText_FreeRequest(9, -1);
    while (items.items[i].x != -1) {
        skip = 0;
        ((DotrShPartsTag *)&packet.header[0])->flags = 0x24;
        ((DotrShPartsTag *)&packet.header[0])->count = 9;
        ((DotrShPartsTag *)&packet.header[0])->registers = 0x424242421LL;
        ((DotrShPartsTag *)&packet.header[0])->reserved = 0;
        packet.color.r = packet.color.g = packet.color.b = packet.color.a = 0x80;
        packet.color.q = 1.0f;
        packet.v0.xyz.z = packet.v1.xyz.z = packet.v2.xyz.z = packet.v3.xyz.z = 0xFFFFF8;
        packet.v0.xyz.f = packet.v1.xyz.f = packet.v2.xyz.f = packet.v3.xyz.f = 0;
        packet.v2.xyz.x = packet.v0.xyz.x = (items.items[i].x + 0x6C0) << 4;
        packet.v1.xyz.y = packet.v0.xyz.y = (items.items[i].y + 0x720) << 4;
        packet.v3.xyz.x = packet.v1.xyz.x = packet.v0.xyz.x + (items.items[i].w << 4);
        packet.v3.xyz.y = packet.v2.xyz.y = packet.v0.xyz.y + (items.items[i].h << 4);
        ((DotrShPartsTag *)&packet.header[0])->flags |= 0x800;
        ((DotrShPartsUv *)&packet.v0.texture)->u = items.items[i].u << 4;
        ((DotrShPartsUv *)&packet.v0.texture)->v = items.items[i].v << 4;
        ((DotrShPartsUv *)&packet.v1.texture)->u = items.items[i].uw << 4;
        ((DotrShPartsUv *)&packet.v1.texture)->v = items.items[i].v << 4;
        ((DotrShPartsUv *)&packet.v2.texture)->u = items.items[i].u << 4;
        ((DotrShPartsUv *)&packet.v2.texture)->v = items.items[i].vh << 4;
        ((DotrShPartsUv *)&packet.v3.texture)->u = items.items[i].uw << 4;
        ((DotrShPartsUv *)&packet.v3.texture)->v = items.items[i].vh << 4;
        switch (items.items[i].kind) {
        case 1:
            ExAssignClamp(&packet.header[0], DOTR_SHPARTS_CLAMP(2, 2, items.items[i].u, items.items[i].uw, items.items[i].v,
                                                          items.items[i].vh - 1));
            break;
        case 2:
            ExAssignClamp(&packet.header[0], 0x100030800FFLL);
            break;
        case 3:
            ExAssignClamp(&packet.header[0], 0x10C030800FFLL);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            if (data_00351240_parts[0].state == 1) {
                bit = items.items[i].kind - 4;
                if (CanStillGet3InARow_parts & (1 << bit)) {
                    packet.color.r = packet.color.g = packet.color.b = pulse.values[(data_00351246_parts >> 1) & 0xF] + 0x40;
                    packet.color.a = pulse.values[(data_00351246_parts >> 1) & 0xF] + 4;
                    break;
                }
            }
            skip = 1;
            break;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            bit = items.items[i].kind - 9;
            if (!(data_00351241_parts & (1 << bit))) {
                skip = 1;
            }
            break;
        case 14:
            ExAssignClamp(&packet.header[0], 0x3F0003FF);
            break;
        case 15:
            ExAssignClamp(&packet.header[0], 0x1001F0001FFLL);
            break;
        case 16:
            packet.v0.xyz.x = (items.items[i].x + 0x6C0) << 4;
            packet.v0.xyz.y = (items.items[i].y + 0x720 + items.items[i].h) << 4;
            packet.v1.xyz.x = (items.items[i].x + 0x6C0) << 4;
            packet.v1.xyz.y = (items.items[i].y + 0x720) << 4;
            packet.v3.xyz.x = (items.items[i].x + 0x6C0 + items.items[i].w) << 4;
            packet.v3.xyz.y = (items.items[i].y + 0x720) << 4;
            packet.v2.xyz.x = (items.items[i].x + 0x6C0 + items.items[i].w) << 4;
            packet.v2.xyz.y = (items.items[i].y + 0x720 + items.items[i].h) << 4;
            break;
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            tmp = items.items[i].kind - 0x11;
            bit = tmp;
            if (data_00351210_parts.lv[bit]) {
                packet.color.r = packet.color.g = packet.color.b = gSh_Lamp_parts[data_00351210_parts.lv[bit]];
                packet.color.a = packet.color.r >> 1;
                ExAssignTexture(&packet.header[0], handles[items.items[i].tex], 1);
                ExAssignClamp(&packet.header[0], DOTR_SHPARTS_CLAMP(2, 2, items.items[i].u, items.items[i].uw,
                                                              items.items[i].v, items.items[i].vh));
                ExAssignAlpha(&packet.header[0], 0x44);
                data_00351228_parts = ExSortPrim_Pipe(data_00351228_parts, &packet.header[0], 0, -1);
                ExAssignAlpha(&packet.header[0], 0x49);
                data_00351228_parts = ExSortPrim_Pipe(data_00351228_parts, &packet.header[0], 0, -1);
            }
            skip = 1;
            break;
        }
        if (!skip) {
            ExAssignTexture(&packet.header[0], handles[items.items[i].tex], 1);
            ExAssignAlpha(&packet.header[0], 0x44);
            data_00351228_parts = ExSortPrim_Pipe(data_00351228_parts, &packet.header[0], 0, -1);
        }
        i++;
    }
}

#ifdef __MWERKS__
#pragma optimization_level reset
#endif

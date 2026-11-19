#include "dotr/ee/battle_fakes.h"

/* 0x1e6650. Callees are called through views with their prototypes (header); the data and two
 * further callees are declared inside the function body. */
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */
#ifndef __MWERKS__
/* Host build only: the code passes addresses as 32-bit words, which the target compiler accepts. */
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#define DOTR_FAKE_MIRROR_ABS(x) ((x) >= 0 ? (x) : -(x))
#define DOTR_FAKE_MIRROR_RANDOM(n) ((int)((n) * (func_0019a5c8() / 2147483648.0f)))

int lSzBattle_DrawFakeMirror(int arg0, int arg1) {
    extern struct DotrFakeMirrorQuad data_00292310;
    extern struct DotrFakeMirrorVec3 data_00292350;
    extern short data_00292360[4];
    extern short data_00292368[4];
    extern short data_00292370;
    extern short data_00292378;
    extern int func_0020a470_fakes(int, int, int);
    extern int func_00207650_fakes(int, void *, int, int);
    struct DotrFakeMirrorQuad quad;
    float center[4];
    float normal[4];
    float view[4];
    unsigned char screen[0x90];
    float previous[4];
    struct DotrFakeMirrorVec3 light __attribute__((aligned(16)));
    struct DotrFakeMirrorPacket packet;
    float saved;
    unsigned char color[4];
    unsigned char *card;
    int flag;
    int id;
    int j;
    int state;
    int *slot;
    int out;
    float scale;

    slot = (int *)&gSzBattle_bl;
    out = arg0;
    quad = data_00292310;
    light = data_00292350;
    func_00206030_fakes(4, &quad, center);
    func_00205e70_fakes(4, &quad, normal);
    func_002060f0_fakes((float *)func_001f9ad0_fakes(0), 4, quad.points[0], view);
    if (!(func_0020d1a0_fakes(view, normal) <= 0.0f) && func_0020e520_fakes(center, normal, screen) != 0) {
        if ((func_001a8720_fakes() & 1) == 0) {
            int range;
            int n;
            int dx;
            int dy;
            short *x;
            short *y;
            float distance;

            range = 6;
            distance = func_0020d0e0_fakes((float *)func_001f9ad0_fakes(0));
            if (distance <= 600.0f) {
                n = (int)(600.0f / distance);
                range += (n < 6) ? n : 6;
            }
            if (func_001f3360_fakes(8) == 0) {
                for (j = 2; j >= 0; j--) {
                    data_00292360[j + 1] = data_00292360[j];
                    data_00292368[j + 1] = data_00292368[j];
                }
                if (DOTR_FAKE_MIRROR_RANDOM(3.0f) != 0) {
                    dx = data_00292370;
                } else {
                    dx = 0;
                }
                data_00292360[0] += dx;
                if (DOTR_FAKE_MIRROR_RANDOM(3.0f) != 0) {
                    dy = data_00292378;
                } else {
                    dy = 0;
                }
                data_00292368[0] += dy;
                if (DOTR_FAKE_MIRROR_ABS(data_00292360[0]) > range ||
                    (DOTR_FAKE_MIRROR_ABS(data_00292360[0]) > range >> 1 && DOTR_FAKE_MIRROR_RANDOM(3.0f) == 0)) {
                    data_00292370 = (data_00292360[0] > 0) ? -1 : 1;
                }
                if (data_00292368[0] > range * 2 || data_00292368[0] < 0 ||
                    (data_00292368[0] > range && DOTR_FAKE_MIRROR_RANDOM(3.0f) == 0)) {
                    data_00292378 = (data_00292368[0] > range) ? -1 : 1;
                }
            }
        }
        arg0 = (int)func_00208530_fakes((void *)arg0, 0);
        func_0020a470_fakes(0, 10, 0x31);
        out = (int)func_0020a610_fakes((void *)arg0);
        out = (int)func_0020a870_fakes((unsigned char *)out, NULL);
        func_0020e360_fakes(screen);
        out = func_0020ec60_fakes(out);
        out = (int)func_00209f70_fakes((void *)out, 0xFFFFFF);
        out = func_0020a0b0_fakes(out, 0);
        out = func_001efa80_fakes(out, arg1, 0, 0, 0, 0, 0);
        out = (int)func_00209f70_fakes((void *)out, 0xFF000000);
        out = func_0020a0b0_fakes(out, 1);
        {
        int i;
        for (i = 0; i < 2; i++, slot++) {
            if (*slot < 0) {
                continue;
            }
            card = (unsigned char *)func_001df540_fakes(*slot >> 8, *slot & 0xFF);
            flag = 0;
            saved = 1.0f;
            if (card == NULL) {
                continue;
            }
            id = card[0x46];
            if (func_001f1370_fakes(id) == 0) {
                continue;
            }
            state = func_001f2660_fakes(id, color);
            func_001f26e0_fakes(id, 0, NULL);
            if (color[3] != 0x80) {
                flag = 1;
            }
            if (flag) {
                scale = color[3] / 128.0f;
                (void)scale; /* a use right after the assignment keeps scale at its own
                              * statement, as in the original */
                func_001f1e80_fakes(id, &saved);
                func_001f1f70_fakes(id, scale);
            }
            out = func_001efa80_fakes(out, id, 0, 0, 0, 0, 0);
            if (flag) {
                func_001f1f70_fakes(id, saved);
            }
            func_001f26e0_fakes(id, state, color);
        }
        }
        out = func_001efa80_fakes(out, 7, 0, 0, 0, 0, 0);
        func_001f1e10_fakes(0x10, previous);
        func_001f1ee0_fakes(0x10, &light.x);
        out = func_001efa80_fakes(out, 0x10, 0, 0, 0, 0, 0);
        func_001f1ee0_fakes(0x10, previous);
        ((struct DotrFakeMirrorTag *)packet.tag)->flags = 0x26;
        ((struct DotrFakeMirrorTag *)packet.tag)->count = 5;
        ((struct DotrFakeMirrorTag *)packet.tag)->registers = 0x42421;
        ((struct DotrFakeMirrorTag *)packet.tag)->padding = 0;
        ((struct DotrFakeMirrorTag *)packet.tag)->flags |= 0x800;
        func_002086d0_fakes(&packet, 0x8000000048LL);
        packet.r = 0x80;
        packet.g = 0x80;
        packet.b = 0x80;
        packet.a = 0x10;
        packet.q = 1.0f;
        packet.xyz0.z = 0xFFFFF6;
        packet.xyz0.fog = 0;
        packet.xyz1.z = 0xFFFFF6;
        packet.xyz1.fog = 0;
        ((struct DotrFakeMirrorUv *)&packet.uv0)->u = 0x2800;
        ((struct DotrFakeMirrorUv *)&packet.uv0)->v = 0;
        ((struct DotrFakeMirrorUv *)&packet.uv1)->u = 0;
        ((struct DotrFakeMirrorUv *)&packet.uv1)->v = 0x1C00;
        func_0020aa30_fakes(&packet.tex0, &packet.tex1);
        packet.tex0.tcc = 0;
        out = (int)func_0020a760_fakes((void *)out);
        out = func_0020f290_fakes(out);
        func_0020e970_fakes(screen);
        func_0020e360_fakes(screen);
        out = func_0020ec60_fakes(out);
        out = func_0020a0b0_fakes(out, 0);
        packet.xyz0.x = ((data_00292360[0] + 0x6BA) << 4) - 8;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = ((data_00292360[0] + 0x946) << 4) - 8;
        packet.xyz1.y = ((data_00292368[0] + 0x8E6) << 4) - 8;
        out = func_00207650_fakes(out, &packet, 0, 0);
        packet.xyz0.x = ((data_00292360[1] + 0x6BA) << 4) - 8;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = ((data_00292360[1] + 0x946) << 4) - 8;
        packet.xyz1.y = ((data_00292368[1] + 0x8E6) << 4) - 8;
        out = func_00207650_fakes(out, &packet, 0, 0);
        packet.xyz0.x = ((data_00292360[2] + 0x6BA) << 4) - 8;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = ((data_00292360[2] + 0x946) << 4) - 8;
        packet.xyz1.y = ((data_00292368[2] + 0x8E6) << 4) - 8;
        out = func_00207650_fakes(out, &packet, 0, 0);
        packet.xyz0.x = ((data_00292360[3] + 0x6BA) << 4) - 8;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = ((data_00292360[3] + 0x946) << 4) - 8;
        packet.xyz1.y = ((data_00292368[3] + 0x8E6) << 4) - 8;
        out = func_00207650_fakes(out, &packet, 0, 0);
        packet.a = 0x30;
        packet.xyz0.x = 0x6B98;
        packet.xyz0.y = 0x7198;
        packet.xyz1.x = 0x9458;
        packet.xyz1.y = 0x8E58;
        out = func_00207650_fakes(out, &packet, 0, 0);
        out = func_0020a0b0_fakes(out, 1);
        func_0020a9e0_fakes();
        out = (int)func_00208530_fakes((void *)out, 1);
    }
    return out;
}

#include "dotr/ee/taku_quads.h"

/* 0x219180. Address words are DotrQuadWord; some declarations are inside the body. */
#ifndef __MWERKS__
/* Host build only: the two unused arguments are not read. */
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

void Taku_Prim_Set_GT4(void *arg0, short x0, short y0, short x1, short y1,
                       short x2, short y2, short x3, short y3,
                       short u0, short v0, short u1, short v1,
                       short u2, short v2, short u3, short v3,
                       unsigned char r0, unsigned char g0, unsigned char b0, unsigned char a0,
                       unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1,
                       unsigned char r2, unsigned char g2, unsigned char b2, unsigned char a2,
                       unsigned char r3, unsigned char g3, unsigned char b3, unsigned char a3,
                       unsigned char unused0, unsigned short texture, unsigned char unused1, short z) {
    extern void *tex_data[];
    extern int SzDuel_TrimUnitDir2();
    void **textures;
    DotrQuadGt4Packet packet;
    float width;
    float height;
    int depth;

    textures = tex_data;
    ExGetMyOT();
    dotr_taku_quads_init_gt4(&packet);
    packet.vertex[0].r = r0;
    packet.vertex[0].g = g0;
    packet.vertex[0].b = b0;
    packet.vertex[0].a = a0;
    packet.vertex[0].q = 1.0f;
    packet.vertex[1].r = r1;
    packet.vertex[1].g = g1;
    packet.vertex[1].b = b1;
    packet.vertex[1].a = a1;
    packet.vertex[1].q = 1.0f;
    packet.vertex[2].r = r2;
    packet.vertex[2].g = g2;
    packet.vertex[2].b = b2;
    packet.vertex[2].a = a2;
    packet.vertex[2].q = 1.0f;
    packet.vertex[3].r = r3;
    packet.vertex[3].g = g3;
    packet.vertex[3].b = b3;
    packet.vertex[3].a = a3;
    packet.vertex[3].q = 1.0f;
    depth = z + 0xFFFFF8;
    TAKU_GT4_XYWH(&packet, x0, y0, depth, 0, x1, y1, depth,
                  0, x2, y2, depth, 0, x3, y3, depth, 0);
    if ((u1 - (short)u0) < 0x101) {
        width = 256.0f;
    } else {
        width = 512.0f;
    }
    if ((v2 - v0) < 0x101) {
        height = 256.0f;
    } else {
        height = 512.0f;
    }
    packet.vertex[0].s = (0.5f + (float)u0) / width;
    packet.vertex[0].t = (0.5f + (float)v0) / height;
    packet.vertex[1].s = (0.5f + (float)u1) / width;
    packet.vertex[1].t = (0.5f + (float)v1) / height;
    packet.vertex[2].s = (0.5f + (float)u2) / width;
    packet.vertex[2].t = (0.5f + (float)v2) / height;
    packet.vertex[3].s = (0.5f + (float)u3) / width;
    packet.vertex[3].t = (0.5f + (float)v3) / height;
    ExAssignTexture(&packet, textures[texture], 0);
    ExAssignAlpha(&packet, ExGetAlphaVal(0));
    ExSortPrim_Pipe((DotrQuadWord)arg0, (DotrQuadWord)&packet, 0, 0);
}

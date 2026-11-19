#include "dotr/ee/taku_quads.h"

/* 0x218340. Address words are DotrQuadWord; some declarations are inside the body. */
#ifndef __MWERKS__
/* Host build only: mode and flag are not read. */
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

void *Taku_Prim_Set_FT4(void *arg0, short x0, short y0, short x1, short y1,
                        short x2, short y2, short x3, short y3,
                        short u0, short v0, short u1, short v1,
                        short u2, short v2, short u3, short v3,
                        unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned char mode,
                        unsigned short texture, unsigned char flag, short depth) {
    extern void TAKU_FT4_XYWH(void *, short, short, int, unsigned char, short, short, int, unsigned char,
                              short, short, int, unsigned char, short, short, int, unsigned char);
    extern void *tex_data[];
    DotrQuadFt4Packet packet __attribute__((aligned(8)));
    int z;

    ExGetMyOT();
    ((DotrQuadFt4Header *)packet.tag)->flags = 0x24;
    ((DotrQuadFt4Header *)packet.tag)->count = 9;
    ((DotrQuadFt4Header *)packet.tag)->registers = 0x0000000424242421ULL;
    ((DotrQuadFt4Header *)packet.tag)->pad = 0;
    packet.r = r;
    packet.g = g;
    packet.b = b;
    packet.a = a;
    packet.q = 1.0f;
    z = depth + 0xFFFFF8;
    TAKU_FT4_XYWH(packet.tag, x0, y0, z, 0, x1, y1, z, 0,
                 x2, y2, z, 0, x3, y3, z, 0);
    ((DotrQuadFt4Header *)packet.tag)->flags |= 0x800;
    ((DotrQuadFt4Vertex *)packet.vertices)->u = (u0 * 16) + 8;
    ((DotrQuadFt4Vertex *)packet.vertices)->v = (v0 * 16) + 8;
    ((DotrQuadFt4Vertex *)(packet.vertices + 0x10))->u = (u1 * 16) + 8;
    ((DotrQuadFt4Vertex *)(packet.vertices + 0x10))->v = (v1 * 16) + 8;
    ((DotrQuadFt4Vertex *)(packet.vertices + 0x20))->u = (u2 * 16) + 8;
    ((DotrQuadFt4Vertex *)(packet.vertices + 0x20))->v = (v2 * 16) + 8;
    ((DotrQuadFt4Vertex *)(packet.vertices + 0x30))->u = (u3 * 16) + 8;
    ((DotrQuadFt4Vertex *)(packet.vertices + 0x30))->v = (v3 * 16) + 8;
    ExAssignTexture(packet.tag, tex_data[texture], 0);
    ExAssignAlpha(packet.tag, 0x0000008000000088ULL);
    return (void *)ExSortPrim_Pipe((DotrQuadWord)arg0, (DotrQuadWord) packet.tag, 0, 0);
}

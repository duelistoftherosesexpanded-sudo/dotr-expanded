#include "dotr/ee/prim_sp1.h"

/* 0x217d70. Address words are DotrSp1Word; some declarations are inside the body. */
#ifndef __MWERKS__
/* Host build only: mode and flag are not read. */
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

void Taku_Prim_Set_SP1(DotrSp1Word arg0, short x0, short y0, short x1, short y1,
                       short u, short v, short width, short height,
                       unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned char mode,
                       unsigned short texture, unsigned char flag, short depth) {
    extern void *tex_data[];
    extern void TAKU_SP_XYWH(void *, short, short, short, short, int, int);
    DotrSp1Packet packet __attribute__((aligned(8)));

    ExGetMyOT();
    ((DotrSp1Header *)packet.tag)->flags = 0x26;
    ((DotrSp1Header *)packet.tag)->count = 5;
    ((DotrSp1Header *)packet.tag)->registers = 0x42421;
    ((DotrSp1Header *)packet.tag)->pad = 0;
    packet.r = r;
    packet.g = g;
    packet.b = b;
    packet.a = a;
    packet.q = 1.0f;
    TAKU_SP_XYWH(packet.tag, x0, y0, (short)(x1 - x0), (short)(y1 - y0), depth + 0xFFFFF8, 0);
    ((DotrSp1Header *)packet.tag)->flags |= 0x800;
    ((DotrSp1Vertex *)packet.vertices)->u = (u * 16) + 8;
    ((DotrSp1Vertex *)packet.vertices)->v = (v * 16) + 8;
    ((DotrSp1Vertex *)(packet.vertices + 0x10))->u = ((u + width) * 16) + 8;
    ((DotrSp1Vertex *)(packet.vertices + 0x10))->v = ((v + height) * 16) + 8;
    ExAssignTexture(packet.tag, tex_data[texture], 0);
    ExAssignAlpha(packet.tag, 0x0000008000000044ULL);
    ExSortPrim_Pipe(arg0, (DotrSp1Word)packet.tag, 0, 0);
}

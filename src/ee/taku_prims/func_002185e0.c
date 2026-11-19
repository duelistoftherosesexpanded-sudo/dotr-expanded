#include "dotr/ee/taku_prims.h"

DotrPrimWord Taku_Prim_Set_FT4_ST(DotrPrimWord cursor, int x0, int y0, int x1, int y1, int x2,
                                  int y2, int x3, short y3, short s0, short t0, short s1,
                                  short t1, short s2, short t2, short s3, short t3,
                                  unsigned char r, unsigned char g, unsigned char b,
                                  unsigned char a, int unknown_68, unsigned short texture,
                                  int unknown_78, short z)
{
    DotrPrimStPacket packet;
    int depth;

    (void)unknown_68;
    (void)unknown_78;
    ExGetMyOT();
    ((struct DotrPrimHeader *)packet.header)->unknown_02 = 36;
    ((struct DotrPrimHeader *)packet.header)->unknown_00 = 9;
    ((struct DotrPrimHeader *)packet.header)->unknown_08 = 0x424242421;
    ((struct DotrPrimHeader *)packet.header)->unknown_04 = 0;
    packet.colour[0] = r;
    packet.colour[1] = g;
    packet.colour[2] = b;
    packet.colour[3] = a;
    packet.q = 1.0f;
    depth = z + 0xfffff8;
    TAKU_FT4_XYWH((struct DotrPrimHeader *)packet.header, x0, y0, depth, 0, x1, y1, depth, 0,
                  x2, y2, depth, 0, x3, y3, depth, 0);
    packet.v[0].s = (0.5f + (float)s0) / 256.0f;
    packet.v[0].t = (0.5f + (float)t0) / 256.0f;
    packet.v[1].s = (0.5f + (float)s1) / 256.0f;
    packet.v[1].t = (0.5f + (float)t1) / 256.0f;
    packet.v[2].s = (0.5f + (float)s2) / 256.0f;
    packet.v[2].t = (0.5f + (float)t2) / 256.0f;
    packet.v[3].s = (0.5f + (float)s3) / 256.0f;
    packet.v[3].t = (0.5f + (float)t3) / 256.0f;
    ExAssignTexture((struct DotrPrimHeader *)&packet, tex_data[texture], 0);
    ExAssignAlpha((struct DotrPrimHeader *)&packet, 0x8000000088);
    return ExSortPrim_Pipe(cursor, &packet, 0, 0);
}

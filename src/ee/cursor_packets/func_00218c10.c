#include "dotr/ee/cursor_packets.h"

/* Builds a four-vertex shaded packet: header (8, 20, 0, 0x41414141), per vertex four colour
 * bytes and 1.0, positions (x0, y0), (x1, y1), (x2, y2), (x3, y3) set by TAKU_G4_XYWH with the
 * depth z + 0xfffff8 and a zero last byte. Mode 3 assigns ExGetAlphaVal(2), every other mode
 * ExGetAlphaVal(0) (the original keeps one arm per mode 0 to 3 and a default). ExGetMyOT is
 * called first and its result dropped. Returns the result of ExSortPrim_Pipe. */
DotrDrawWord Taku_Prim_Set_G4(DotrDrawWord cursor, int x0, int y0, int x1, int y1, int x2, int y2, int x3,
                              short y3, unsigned char r0, unsigned char g0, unsigned char b0,
                              unsigned char a0, unsigned char r1, unsigned char g1, unsigned char b1,
                              unsigned char a1, unsigned char r2, unsigned char g2, unsigned char b2,
                              unsigned char a2, unsigned char r3, unsigned char g3, unsigned char b3,
                              unsigned char a3, unsigned short mode, short z)
{
    struct DotrBasePipePacket packet;
    int depth;

    ExGetMyOT();
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_02 = 20;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_00 = 8;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_08 = 0x41414141;
    ((struct DotrCursorPacketHeader *)packet.header)->unknown_04 = 0;
    packet.v[0].colour[0] = r0;
    packet.v[0].colour[1] = g0;
    packet.v[0].colour[2] = b0;
    packet.v[0].colour[3] = a0;
    packet.v[0].q = 1.0f;
    packet.v[1].colour[0] = r1;
    packet.v[1].colour[1] = g1;
    packet.v[1].colour[2] = b1;
    packet.v[1].colour[3] = a1;
    packet.v[1].q = 1.0f;
    packet.v[2].colour[0] = r2;
    packet.v[2].colour[1] = g2;
    packet.v[2].colour[2] = b2;
    packet.v[2].colour[3] = a2;
    packet.v[2].q = 1.0f;
    packet.v[3].colour[0] = r3;
    packet.v[3].colour[1] = g3;
    packet.v[3].colour[2] = b3;
    packet.v[3].colour[3] = a3;
    packet.v[3].q = 1.0f;
    depth = z + 0xfffff8;
    TAKU_G4_XYWH((struct DotrCursorPacketHeader *)packet.header, x0, y0, depth, 0, x1, y1, depth, 0, x2, y2, depth, 0,
                 x3, y3, depth, 0);
    /* MATCHING DEVICE: after the TAKU_G4_XYWH call the header address is spelled from the member
     * before it; with `packet.header` the compiler keeps the address in a saved register. */
    switch (mode) {
    case 0:
        ExAssignAlpha((struct DotrCursorPacketHeader *)(&packet.unknown_m8 + 1), ExGetAlphaVal(0));
        break;
    case 1:
        ExAssignAlpha((struct DotrCursorPacketHeader *)(&packet.unknown_m8 + 1), ExGetAlphaVal(0));
        break;
    case 2:
        ExAssignAlpha((struct DotrCursorPacketHeader *)(&packet.unknown_m8 + 1), ExGetAlphaVal(0));
        break;
    case 3:
        ExAssignAlpha((struct DotrCursorPacketHeader *)(&packet.unknown_m8 + 1), ExGetAlphaVal(2));
        break;
    default:
        ExAssignAlpha((struct DotrCursorPacketHeader *)(&packet.unknown_m8 + 1), ExGetAlphaVal(0));
        break;
    }
    return ExSortPrim_Pipe(cursor, &packet.unknown_m8 + 1, 0, 0);
}

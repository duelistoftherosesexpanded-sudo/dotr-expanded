#include "dotr/ee/scratch_quads.h"

/* Draws the four side faces of the box cursor around `centre`: half-width 24 in x and z,
 * top edge at centre y - 35 and bottom edge at y = -35. The first scratchpad vertex gets
 * colour 0,0,0,255 or, when `colour` is given, its bytes 2,1,0,3, and 1.0 at +0x1c. The
 * local packet header is (0, 5, 0, 0) when `single` is set and (0, 2, 0, 0) otherwise;
 * alpha 0x8000000048 is assigned when the colour's fourth byte is not 255. Each face goes
 * through Ex3d_NodeL2V, Ex3d_NodeV2S and ExSortNode_Pipe, the cursor being chained; the
 * last result is returned.
 * The scratchpad is addressed as a literal here (one `lui $at` per store), unlike
 * SzDuel_DrawCursor_Pipe; the extents are locals because the original keeps -35 in a
 * saved float register across the calls. */
#define v DOTR_SCRATCH_VERTICES

/* One side face: four corners at centre x/z plus or minus dx, top at centre y - dy and
 * bottom at -dy, sent through Ex3d_NodeL2V and Ex3d_NodeV2S. */
#define BOX_FACE(x0, z0, x1, z1) \
    v[0].position[0] = centre[0] x0 dx; \
    v[0].position[1] = centre[1] - dy; \
    v[0].position[2] = centre[2] z0 dx; \
    v[1].position[0] = centre[0] x0 dx; \
    v[1].position[1] = -dy; \
    v[1].position[2] = centre[2] z0 dx; \
    v[2].position[0] = centre[0] x1 dx; \
    v[2].position[1] = -dy; \
    v[2].position[2] = centre[2] z1 dx; \
    v[3].position[0] = centre[0] x1 dx; \
    v[3].position[1] = centre[1] - dy; \
    v[3].position[2] = centre[2] z1 dx; \
    count = func_002070f0_quad(0, 4, v, 170, 0x70000000, 0, 0); \
    Ex3d_NodeV2S(0, count, v)

int SzDuel_DrawBoxCursor_Pipe(int a, float *centre, unsigned char *colour, int single)
{
    long packet[6];
    int count;
    float dx = 24.0f;
    float dy = 35.0f;

    v[0].colour[0] = 0;
    v[0].colour[1] = 0;
    v[0].colour[2] = 0;
    v[0].colour[3] = 255;
    if (colour != 0) {
        v[0].colour[0] = colour[2];
        v[0].colour[1] = colour[1];
        v[0].colour[2] = colour[0];
        v[0].colour[3] = colour[3];
    }
    v[0].unknown_1c = 1.0f;
    if (single != 0) {
        ((struct DotrScratchPacket *)packet)->unknown_02 = 5;
        ((struct DotrScratchPacket *)packet)->unknown_00 = 0;
        ((struct DotrScratchPacket *)packet)->unknown_08 = 0;
        ((struct DotrScratchPacket *)packet)->unknown_04 = 0;
    } else {
        ((struct DotrScratchPacket *)packet)->unknown_02 = 2;
        ((struct DotrScratchPacket *)packet)->unknown_00 = 0;
        ((struct DotrScratchPacket *)packet)->unknown_08 = 0;
        ((struct DotrScratchPacket *)packet)->unknown_04 = 0;
    }
    if (v[0].colour[3] != 255) {
        func_002086d0_quad((struct DotrScratchPacket *)packet, 0x8000000048L);
    }
    BOX_FACE(-, -, +, -);
    a = func_00207aa0_quad(a, (struct DotrScratchPacket *)packet, count, v, 0, 0);
    BOX_FACE(-, +, +, +);
    a = func_00207aa0_quad(a, (struct DotrScratchPacket *)packet, count, v, 0, 0);
    BOX_FACE(-, -, -, +);
    a = func_00207aa0_quad(a, (struct DotrScratchPacket *)packet, count, v, 0, 0);
    BOX_FACE(+, -, +, +);
    return func_00207aa0_quad(a, (struct DotrScratchPacket *)packet, count, v, 0, 0);
}

#undef v

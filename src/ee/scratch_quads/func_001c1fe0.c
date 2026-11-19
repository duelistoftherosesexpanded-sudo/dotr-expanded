#include "dotr/ee/scratch_quads.h"

/* Writes a closed rectangle (five points, the last repeating the first) of half-extents dx
 * and dz around `centre` in the x/z plane to the scratchpad vertices, with colour
 * (128, 128, 128, alpha) and 1.0 at +0x1c of the first vertex only, then passes them through
 * Ex3d_NodeL2V, Ex3d_NodeV2S and, with a local packet header, ExSortNode_Pipe. The fifth
 * argument of Ex3d_NodeL2V is the scratchpad base 0x70000000 as the original passes it. */
void lSzDuel_DrawFadeBoard_Pipe(int a, float *centre, int alpha, float dx, float dz)
{
    long packet[6];
    int count;

    DOTR_SCRATCH_VERTICES[0].colour[0] = 128;
    DOTR_SCRATCH_VERTICES[0].colour[1] = 128;
    DOTR_SCRATCH_VERTICES[0].colour[2] = 128;
    DOTR_SCRATCH_VERTICES[0].colour[3] = alpha;
    DOTR_SCRATCH_VERTICES[0].unknown_1c = 1.0f;
    DOTR_SCRATCH_VERTICES[0].position[0] = centre[0] - dx;
    DOTR_SCRATCH_VERTICES[0].position[1] = centre[1];
    DOTR_SCRATCH_VERTICES[0].position[2] = centre[2] - dz;
    DOTR_SCRATCH_VERTICES[1].position[0] = centre[0] + dx;
    DOTR_SCRATCH_VERTICES[1].position[1] = centre[1];
    DOTR_SCRATCH_VERTICES[1].position[2] = centre[2] - dz;
    DOTR_SCRATCH_VERTICES[2].position[0] = centre[0] + dx;
    DOTR_SCRATCH_VERTICES[2].position[1] = centre[1];
    DOTR_SCRATCH_VERTICES[2].position[2] = centre[2] + dz;
    DOTR_SCRATCH_VERTICES[3].position[0] = centre[0] - dx;
    DOTR_SCRATCH_VERTICES[3].position[1] = centre[1];
    DOTR_SCRATCH_VERTICES[3].position[2] = centre[2] + dz;
    DOTR_SCRATCH_VERTICES[4].position[0] = centre[0] - dx;
    DOTR_SCRATCH_VERTICES[4].position[1] = centre[1];
    DOTR_SCRATCH_VERTICES[4].position[2] = centre[2] - dz;
    count = func_002070f0_quad(0, 5, DOTR_SCRATCH_VERTICES, 170, 0x70000000, 0, 0);
    Ex3d_NodeV2S(0, count, DOTR_SCRATCH_VERTICES);
    ((struct DotrScratchPacket *)packet)->unknown_02 = 5;
    ((struct DotrScratchPacket *)packet)->unknown_00 = 0;
    ((struct DotrScratchPacket *)packet)->unknown_08 = 0;
    ((struct DotrScratchPacket *)packet)->unknown_04 = 0;
    func_002086d0_quad((struct DotrScratchPacket *)packet, ExGetAlphaVal(2));
    func_00207aa0_quad(a, (struct DotrScratchPacket *)packet, count, DOTR_SCRATCH_VERTICES, 0, 0);
}

#include "dotr/ee/scratch_quads.h"

/* Draws the cursor outline: opens an anchor on `a`, then up to three times writes a closed
 * rectangle (five points) of half-extents dx and dz around `centre` to the scratchpad
 * vertices, with colour 128,128,128,128 or, when `colour` is given, its bytes 2,1,0,3, sends
 * it through Ex3d_NodeL2V, Ex3d_NodeV2S and ExSortNode_Pipe with a local packet header (kind
 * 5 when `single` is set, 0x402 otherwise; alpha assigned when the colour's fourth byte is
 * below 255), shrinking both extents by 0.2 per pass. `single` stops after the first pass.
 * Finally sorts the anchor into the caller's node at the sample depth of `centre`.
 * The scratchpad base is a pointer local: the original keeps 0x70000000 in a saved register
 * and forms the vertex address from it. Parameter order (floats after `centre`) follows the
 * order in which the original saves its arguments. */
void SzDuel_DrawCursor_Pipe(int a, float *centre, float dx, float dz, int alpha, unsigned char *colour, int single)
{
    long packet[6];
    int node;
    struct DotrScratchVertex *v;
    int i;
    int *ot;
    char *base;
    int count;

    base = DOTR_SCRATCH_BASE;
    i = 2;
    node = a;
    v = DOTR_SCRATCH_VERTICES;
    a = ExSetAnchor_Pipe(a);
    do {
        v[0].colour[0] = 128;
        v[0].colour[1] = 128;
        v[0].colour[2] = 128;
        v[0].colour[3] = 128;
        if (colour != 0) {
            v[0].colour[0] = colour[2];
            v[0].colour[1] = colour[1];
            v[0].colour[2] = colour[0];
            v[0].colour[3] = colour[3];
        }
        v[0].unknown_1c = 1.0f;
        v[0].position[0] = centre[0] - dx;
        v[0].position[1] = centre[1];
        v[0].position[2] = centre[2] - dz;
        v[1].position[0] = centre[0] + dx;
        v[1].position[1] = centre[1];
        v[1].position[2] = centre[2] - dz;
        v[2].position[0] = centre[0] + dx;
        v[2].position[1] = centre[1];
        v[2].position[2] = centre[2] + dz;
        v[3].position[0] = centre[0] - dx;
        v[3].position[1] = centre[1];
        v[3].position[2] = centre[2] + dz;
        v[4].position[0] = centre[0] - dx;
        v[4].position[1] = centre[1];
        v[4].position[2] = centre[2] - dz;
        count = func_002070f0_quad(0, 5, v, 170, DOTR_SCRATCH_BASE_ARG(base), 0, 0);
        Ex3d_NodeV2S(0, count, v);
        if (single != 0) {
            ((struct DotrScratchPacket *)packet)->unknown_02 = 5;
            ((struct DotrScratchPacket *)packet)->unknown_00 = 0;
            ((struct DotrScratchPacket *)packet)->unknown_08 = 0;
            ((struct DotrScratchPacket *)packet)->unknown_04 = 0;
        } else {
            ((struct DotrScratchPacket *)packet)->unknown_02 = 0x402;
            ((struct DotrScratchPacket *)packet)->unknown_00 = 0;
            ((struct DotrScratchPacket *)packet)->unknown_08 = 0;
            ((struct DotrScratchPacket *)packet)->unknown_04 = 0;
        }
        if (v[0].colour[3] < 255) {
            func_002086d0_quad((struct DotrScratchPacket *)packet, ExGetAlphaVal(alpha));
        }
        a = func_00207aa0_quad(a, (struct DotrScratchPacket *)packet, count, v, 0, 0);
        if (single != 0) {
            break;
        }
        if (i-- <= 0) {
            break;
        }
        dx -= 0.2f;
        dz -= 0.2f;
    } while (1);
    ot = ExGetMyOT();
    ExSortAnchor_Pipe(a, node, ot, Ex3d_GetSampleZ(centre, 0.0f));
}

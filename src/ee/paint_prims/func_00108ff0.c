#include "dotr/ee/paint_prims.h"

/* 0x108ff0 PAINT_PIRM_GT4 (name from the Japanese build's symbols): the same code as
 * PAINT_PIRM_G4 for a textured packet. Returns 0. */
int PAINT_PIRM_GT4(void *source, struct DotrPaintWidePacket *packet, int arg2, int arg3)
{
    float depth[4];

    if (Ex3d_RotTransPersAuto(source, 0x70000000, arg2, packet, depth) != 0) {
        ExSortPrim(packet, arg3, func_001a23b0(depth[0]));
    }
    return 0;
}

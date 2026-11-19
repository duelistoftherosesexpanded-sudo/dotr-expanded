#include "dotr/ee/paint_prims.h"

/* 0x108f90 PAINT_PIRM_G4 (name from the Japanese build's symbols): transform, and when that
 * returns non-zero sort the packet at the depth func_001a23b0(result[0]). Returns 0. */
int PAINT_PIRM_G4(void *source, struct DotrPaintWidePacket *packet, int arg2, int arg3)
{
    float result[4];

    if (Ex3d_RotTransPersAuto(source, 0x70000000, arg2, packet, result) != 0) {
        ExSortPrim(packet, arg3, func_001a23b0(result[0]));
    }
    return 0;
}

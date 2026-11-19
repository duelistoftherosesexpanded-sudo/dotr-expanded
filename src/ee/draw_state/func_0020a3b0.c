#include "dotr/ee/draw_state.h"

/* Reads a 640x448 32-bit region of the GS frame buffer back into main
 * memory at `x`. func_00185890 builds a store-image packet at data_00325e80
 * (BITBLTBUF/TRXPOS/TRXREG with TRXDIR 1, local to host), with the source
 * page taken from one of two halfwords of the draw state selected by a flag
 * bit; after a cache flush (func_0017bb60(0)), func_001859d0 runs the
 * transfer. GS-path syncs (func_00185578) come before and after. */
void RhBase_CopyFrameBuffer(unsigned long x)
{
    char *b = (char *)&gRhBase_dl;
    unsigned short *p;
    func_00185578(0, 0);
    if (data_00326180[15] & 1) p = (unsigned short *)(b + 96); else p = (unsigned short *)(b + 336);
    func_00185890(data_00325e80, (short)((*p & 0x1ff) << 5), 10, 10, 0, 0, 640, 448);
    func_0017bb60(0);
    func_001859d0(data_00325e80, x & 0xfffffff);
    func_00185578(0, 0);
}

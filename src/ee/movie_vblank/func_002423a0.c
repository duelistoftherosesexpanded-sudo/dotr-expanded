#include "dotr/ee/movie_vblank.h"

/* 0x2423a0. C with an inline hardware block (the sync, ei pair before the return, which has no
 * C spelling in the pinned compiler). The data views carry the _vblank suffix. The host build
 * omits the block. */

int handler_endimage(void) {
    if (isFrameEnd_vblank != 0) {
        voBufDecCount(&data_00350600_vblank);
        isFrameEnd_vblank = 0;
    }
#ifdef __MWERKS__
    asm("sync"); asm("ei");
#endif
    return 0;
}

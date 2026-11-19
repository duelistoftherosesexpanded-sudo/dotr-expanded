#include "dotr/ee/movie_vblank.h"

/* 0x242190. C with an inline hardware block (the sync, ei pair before each return, which has no
 * C spelling in the pinned compiler). Some callees are declared inside the body; the data views
 * carry the _vblank suffix and the CSR is addressed through DOTR_VBLANK_CSR. The host build omits
 * the block. */
#ifndef __MWERKS__
/* Host build only: the code turns addresses into 32-bit words and back. */
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#endif

int vblankHandler(int val) {
    extern volatile int vblankCount_vblank;
    extern int data_00350360_vblank;
    extern unsigned char data_00350420_vblank[];
    extern unsigned char data_00350510_vblank[];
    void *dmaGif;
    int *tag;

    dmaGif = (void *)func_00186120(2);
    (&isCountVblank_vblank)[6] = (*(volatile unsigned long long *)DOTR_VBLANK_CSR >> 13) & 1;
    if (isCountVblank_vblank) {
        vblankCount_vblank++;
        data_00350360_vblank = func_00185578(1, 0);
        if (!data_00350360_vblank) {
            tag = (int *)voBufGetTag(data_00350600_vblank);
            if (!tag) {
                frd_vblank++;
#ifdef __MWERKS__
                asm("sync"); asm("ei");
#endif
                return 0;
            }
            func_00186060(((&isCountVblank_vblank)[6] & 1) ? (unsigned char *)(((unsigned int)data_00350510_vblank & 0x0FFFFFFF) | 0x20000000)
                                                : (unsigned char *)(((unsigned int)data_00350420_vblank & 0x0FFFFFFF) | 0x20000000),
                               0x800, 0x800, (&isCountVblank_vblank)[6] ^ 1);
            if ((&isCountVblank_vblank)[6] == 0 && *tag == 2) {
                func_00185480(data_003503c0_vblank, 0);
                func_00185578(0, 0);
                func_00186470(dmaGif, (unsigned char *)tag + 0x40);
                *tag = 1;
            } else if ((&isCountVblank_vblank)[6] == 1 && *tag == 1) {
                func_00185480(data_003503c0_vblank, 1);
                func_00185578(0, 0);
                func_00186470(dmaGif, (unsigned char *)tag + 0x26740);
                *tag = 0;
                isFrameEnd_vblank = 1;
            }
        }
    }
#ifdef __MWERKS__
    asm("sync"); asm("ei");
#endif
    return 0;
}

#include "dotr/ee/draw_state.h"
void ExClearOt(DotrPktCtx *ctx, DotrPktEnt *ent, int shift)
{
    long *p;
    unsigned short *w;
    char *r;
    int n;

    p = (long *)RhBase_GetVifPacketArea();
    n = 1 << shift;
    p[1] = 0;
    p[0] = 0;
    *(int *)((char *)ctx + 4) = (DotrDrawWord)p;
    p[2] = 0x100000000000800eL;
    p[3] = 14;
    r = (char *)RhMem_CopyMemory16(p + 4, gExPrim_aaRegs, 14);
    w = ctx->w1;
    *w = 15;
    ((char *)ctx->w1)[3] = 16;
    *(int *)((char *)ctx->w1 + 8) = 0x11000000;
    *(int *)((char *)ctx->w1 + 12) = ctx->w1[0] | 0x51000000;
    ((long *)r)[1] = 0;
    ((long *)r)[0] = 0;
    ctx->w2 = r;
    ctx->w2[3] = 16;
    RhBase_SetVifPacketArea((DotrDrawWord)(r + 16));
    ctx->shift = shift;
    ctx->ent = ent;
    ent += n - 1;
    if (--n != 0) {
        do {
            ent->next = (DotrDrawWord)(ent - 1);
            ent->b = 7;
            ent->a = 0;
            ent--;
        } while (--n != 0);
    }
    ent->next = -1;
    ent->b = 7;
    ent->a = 0;
}

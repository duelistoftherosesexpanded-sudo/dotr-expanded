#include "dotr/ee/draw_state.h"
/* Chain every packet entry whose b != 7 into the context packet list. */
void ExBindPrim(DotrPktCtx *ctx)
{
    char *last;
    DotrPktEnt *cur;
    int nxt;

    last = 0;
    cur = ctx->ent + ((1 << ctx->shift) - 1);
    do {
        nxt = cur->next;
        if (cur->b != 7) {
            if (last != 0) {
                if (((unsigned char *)last)[3] == 80) {
                    *(int *)(last + 4) = (DotrDrawWord)(last + 32);
                    last += 16;
                }
                last[3] = 32;
                *(int *)(last + 4) = (DotrDrawWord)cur;
            } else {
                ctx->w2[3] = 32;
                *(int *)(ctx->w2 + 4) = (DotrDrawWord)cur;
            }
            last = (char *)cur;
        }
        cur = (DotrPktEnt *)(DotrDrawWord)nxt;
    } while (nxt != -1);
    if (last != 0) {
        if (((unsigned char *)last)[3] == 80) {
            *(int *)(last + 4) = (DotrDrawWord)(last + 32);
            last += 16;
        }
        last[3] = 32;
        *(int *)(last + 4) = RhBase_GetVifPacketArea();
    }
}

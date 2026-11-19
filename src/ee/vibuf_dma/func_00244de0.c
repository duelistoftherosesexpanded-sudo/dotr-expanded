#include "dotr/ee/vibuf_dma.h"

/* 0x244de0. Compiled with `#pragma opt_dead_assignments off` around it. The DMA and IPU registers are accessed through DOTR_VIBUF_UREG; WaitSema and SignalSema are
 * func_0017b920 and func_0017b900. */

#ifdef __MWERKS__
#pragma opt_dead_assignments off
#endif
int viBufGetTs(void *arg0, void *arg1)
{
    DotrViBufTs *f;
    int count;
    unsigned int ipubp;
    unsigned int d4madr;
    int i;
    int bp;
    int datasize;
    unsigned int fp;
    unsigned int ifc;
    DotrViBufTimeStamp *ts;
    int isEnd;
    int wt;
    int stop;
    f = arg0;
    ts = arg1;

    d4madr = DOTR_VIBUF_UREG(0x1000B410);
    isEnd = 0;
    ipubp = DOTR_VIBUF_UREG(0x10002020);
    bp = f->ipubp & 0x7F;
    fp = (ipubp >> 16) & 3;
    ifc = (ipubp >> 8) & 0xF;
    datasize = f->n << 11;
    func_0017b920(f->sema);
    stop = d4madr - ((fp + ifc) << 4);
    ts->pts = -1;
    ts->dts = -1;
    count = f->count_ts;
    stop = (stop + (bp >> 3) + datasize - f->data) % datasize;
    wt = f->wt_ts;
    for (i = 0; i < count && !isEnd; i++) {
        int index;
        index = (wt - count + f->n_ts + i) % f->n_ts;
        if ((stop + datasize - f->ts[index].pos) % datasize < f->ts[index].len) {
            ts->pts = f->ts[index].pts;
            ts->dts = f->ts[index].dts;
            f->ts[index].pts = -1;
            f->ts[index].dts = -1;
            isEnd = 1;
            f->count_ts -= (f->count_ts <= 0) ? f->count_ts : 1;
        }
    }
    func_0017b900(f->sema);
    return 1;
}
#ifdef __MWERKS__
#pragma opt_dead_assignments reset
#endif

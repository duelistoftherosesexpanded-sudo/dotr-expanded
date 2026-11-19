#include "dotr/ee/vibuf_dma.h"

int viBufAddDMA(struct DotrViBufDma *p)
{
    int ctrl;
    int restart;
    int addr;
    int pos;
    int n;
    int wr;
    int count;
    int i;
    unsigned prev;

    restart = 0;
    func_0017b920(p->sema);
    if (p->isActive == 0) {
        ErrMessage(DOTR_VIBUF_ADDR(int, data_0030f890));
        return 0;
    }
    func_001843e0();
    DOTR_VIBUF_REG(0x1000f590) = DOTR_VIBUF_REG(0x1000f520) | 0x10000;
    DOTR_VIBUF_REG(0x1000b400) = 5;
    DOTR_VIBUF_REG(0x1000f590) = DOTR_VIBUF_REG(0x1000f520) & ~0x10000;
    func_00184428();
    ctrl = DOTR_VIBUF_REG(0x1000b400);
    addr = DOTR_VIBUF_REG(0x1000b410);
    if ((unsigned long)addr == ((unsigned long)DOTR_VIBUF_ADDR(unsigned, p->tag + (p->n + 1)) & 0x0fffffff)) {
        pos = 0;
    } else {
        pos = (unsigned)(addr - DOTR_VIBUF_ADDR(int, p->data)) >> 11;
    }
    n = (pos + p->n - p->dmaStart) % p->n;
    p->dmaStart = (p->dmaStart + n) % p->n;
    p->dmaN -= n;
    wr = (p->dmaStart + p->dmaN) % p->n;
    count = p->readBytes >> 11;
    p->readBytes = p->readBytes % 2048;
    if (count > 0) {
        restart = 1;
        prev = (p->dmaStart + p->dmaN - 1 + p->n) % p->n;
        *(unsigned long *)(p->tag + prev) =
            ((unsigned long)(DOTR_VIBUF_ADDR(unsigned, p->data) + (prev << 11)) << 32) | 0x30000080;
    }
    for (i = 0; i < count; i++) {
        *(unsigned long *)(p->tag + wr) = ((unsigned long)(DOTR_VIBUF_ADDR(unsigned, p->data) + (wr << 11)) << 32) | ((unsigned long)(unsigned)(i == count - 1 ? 0 : 3) << 28) | 0x80;
        wr = (wr + 1) % p->n;
    }
    p->dmaN += count;
    if (p->dmaN != 0) {
        if (restart) {
            ctrl = ((unsigned long)ctrl & 0x0fffffff) | 0x30000000;
        }
        func_001843e0();
        DOTR_VIBUF_REG(0x1000f590) = DOTR_VIBUF_REG(0x1000f520) | 0x10000;
        DOTR_VIBUF_REG(0x1000b400) = ctrl | 0x100;
        DOTR_VIBUF_REG(0x1000f590) = DOTR_VIBUF_REG(0x1000f520) & ~0x10000;
        func_00184428();
    }
    func_0017b900(p->sema);
    return 1;
}

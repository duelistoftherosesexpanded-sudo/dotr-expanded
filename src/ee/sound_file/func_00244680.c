#include "dotr/ee/sound_file.h"

#define VIBUF_ELM_SIZE 2048
#define DmaAddr(v) ((unsigned)(v) & 0x0fffffff)
#define DataAddr(f, i) ((f)->data + VIBUF_ELM_SIZE * (i))
#define TagAddr(f, i) ((f)->tag + 16 * (i))
/* MATCHING DEVICE: the region test has to be a macro; as an inline function the compiler
 * materialises its result instead of branching on the two comparisons. */
#define IsInRegion(i, start, len, n) (0 <= ((i) + (n) - (start)) % (n) && ((i) + (n) - (start)) % (n) < (len))

static inline int getFIFOindex(struct DotrSoundViBuf *f, unsigned addr)
{
    if (addr == DmaAddr(TagAddr(f, f->n + 1))) {
        return 0;
    } else {
        return (addr - f->data) / VIBUF_ELM_SIZE;
    }
}

static inline void setD3_CHCR(unsigned val)
{
    func_001843e0();
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) | 0x10000;
    DOTR_SOUND_REG(0x1000b000) = val;
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) & ~0x10000;
    func_00184428();
}

static inline void setD4_CHCR(unsigned val)
{
    func_001843e0();
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) | 0x10000;
    DOTR_SOUND_REG(0x1000b400) = val;
    DOTR_SOUND_REG(0x1000f590) = DOTR_SOUND_REG(0x1000f520) & ~0x10000;
    func_00184428();
}

/* Restarts the stream's DMA after a stop, following the Sony MPEG sample's routine (local
 * names as there). Backs the saved channel-4 address up by the words still held in the IPU
 * (FIFO counts in the saved bit-position word) and adds them to the count. If that address
 * falls before the data area it wraps to the end, restarting at tag 0; otherwise, if it moved
 * into another element, the transfer restarts at the saved element's tag. In both cases the
 * control word gets tag id 0 (end) or 3 and the DMA start index and count are extended when
 * the restart element lies outside the pending region. Then restores channel 3 when its saved
 * address and count are set, clears the IPU input to the saved bit position when elements are
 * pending, writes channel 4's address, tag address and count, restarts it when elements are
 * pending, restores the IPU control word and marks the buffer active. Returns 1. */
int viBufRestartDMA(struct DotrSoundStream *p)
{
    struct DotrSoundViBuf *f = (struct DotrSoundViBuf *)p;
    int bp = f->ipubp & 0x7f;
    int fp = (f->ipubp >> 16) & 3;
    int ifc = (f->ipubp >> 8) & 0xf;
    unsigned d4madr_next = f->d4madr - ((fp + ifc) << 4);
    unsigned d4qwc_next = f->d4qwc + (fp + ifc);
    unsigned d4chcr_next = f->d4chcr | 0x100;
    unsigned d3chcr_next;
    unsigned d4tadr_next = f->d4tadr;
    int index;
    int index_next;
    int id;

    func_0017b920(f->sema);
    if (d4madr_next < f->data) {
        d4qwc_next = (DataAddr(f, 0) - d4madr_next) >> 4;
        d4madr_next += f->n * 2048;
        d4tadr_next = DmaAddr(TagAddr(f, 0));
        id = (f->d4madr == DataAddr(f, 0) || f->d4madr == DataAddr(f, f->n)) ? 0 : 3;
        d4chcr_next = (f->d4chcr & 0x0fffffff) | (id << 28) | 0x100;
        if (!IsInRegion(0, f->dmaStart, f->dmaN, f->n)) {
            f->dmaStart = f->n - 1;
            f->dmaN++;
        }
    } else if ((index = getFIFOindex(f, f->d4madr)) != (index_next = getFIFOindex(f, d4madr_next))) {
        d4tadr_next = DmaAddr(TagAddr(f, index));
        d4qwc_next = (DataAddr(f, index) - d4madr_next) >> 4;
        id = (f->data + (f->d4madr - f->data) % (f->n * 2048) == DataAddr(f, (f->dmaStart + f->dmaN) % f->n)) ? 0 : 3;
        d4chcr_next = (f->d4chcr & 0x0fffffff) | (id << 28) | 0x100;
        if (!IsInRegion(index_next, f->dmaStart, f->dmaN, f->n)) {
            f->dmaStart = index_next;
            f->dmaN++;
        }
    }
    if (f->d3madr && f->d3qwc) {
        DOTR_SOUND_REG(0x1000b010) = f->d3madr;
        DOTR_SOUND_REG(0x1000b020) = f->d3qwc;
        d3chcr_next = f->d3chcr | 0x100;
        setD3_CHCR(d3chcr_next);
    }
    if (f->dmaN) {
        while (DOTR_SOUND_REG(0x10002010) < 0) {
        }
        DOTR_SOUND_REG(0x10002000) = bp;
        while (DOTR_SOUND_REG(0x10002010) < 0) {
        }
    }
    DOTR_SOUND_REG(0x1000b410) = d4madr_next;
    DOTR_SOUND_REG(0x1000b430) = d4tadr_next;
    DOTR_SOUND_REG(0x1000b420) = d4qwc_next;
    if (f->dmaN) {
        setD4_CHCR(d4chcr_next);
    }
    DOTR_SOUND_REG(0x10002010) = f->ipuctrl;
    f->isActive = 1;
    func_0017b900(f->sema);
    return 1;
}




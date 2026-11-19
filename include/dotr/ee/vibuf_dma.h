#ifndef DOTR_EE_VIBUF_DMA_H
#define DOTR_EE_VIBUF_DMA_H

/* Video stream buffer routine that appends newly read 2048-byte blocks to the DMA channel 4 tag
 * chain and restarts the channel.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* One 16-byte DMA tag / data element, addressed by quadword pointer arithmetic. */
#ifdef __MWERKS__
typedef __int128 DotrViBufQuad;
#else
__extension__ typedef __int128 DotrViBufQuad __attribute__((aligned(4), may_alias));
#endif

/* Stream record view (same storage as sound_file.h DotrSoundStream / DotrSoundViBuf, with
 * pointer members for the data and tag areas): data +0, tag +4, element count +8, DMA start
 * index and count +0xc/+0x10, bytes read but not yet queued +0x14, semaphore +0x40, active
 * flag +0x44. Member names follow Sony's MPEG streaming sample, not this build's
 * symbols. On the host the pointers move the later offsets. */
struct DotrViBufDma {
    DotrViBufQuad *data;
    DotrViBufQuad *tag;
    int n;
    int dmaStart;
    int dmaN;
    int readBytes;
    int unknown_18;
    int unknown_1c[9];
    int sema;
    int isActive;
};

/* Area addresses are compared with and stored into 32-bit DMA words. The target converts
 * the pointer directly; the host goes through uintptr_t (truncating to the low 32 bits). */
#ifdef __MWERKS__
#define DOTR_VIBUF_ADDR(type, ptr) ((type)(ptr))
#else
#include <stdint.h>
#define DOTR_VIBUF_ADDR(type, ptr) ((type)(uintptr_t)(ptr))
#endif

/* DMA channel 4 (0x1000b400 control, 0x1000b410 address) and the DMA enable registers
 * (0x1000f520 read, 0x1000f590 written). The host maps them by (addr & 0xffff) >> 4 into a
 * test-supplied word array (as sound_file.h does); not a claim about an original object. */
#ifdef __MWERKS__
#define DOTR_VIBUF_REG(addr) (*(volatile int *)(addr))
#else
extern volatile int dotr_vibuf_host_regs[];
#define DOTR_VIBUF_REG(addr) (dotr_vibuf_host_regs[((addr) & 0xffff) >> 4])
#endif

/* 0x30f890: message text passed to ErrMessage when the stream is inactive (by address). */
extern char data_0030f890[];
/* ErrMessage (0x242cf0, small_misc); this caller passes the text address as an int. */
extern void ErrMessage(int a);
/* Library-block callees: wait on / signal the semaphore id (as declared in
 * sound_file.h), and enter / leave the DMA-register critical section. */
extern void func_0017b920(int sema);
extern void func_0017b900(int sema);
extern void func_001843e0(void);
extern void func_00184428(void);

/* 0x244250 viBufAddDMA: under the semaphore, returns 0 after ErrMessage when inactive.
 * Otherwise stops channel 4 (control 5), reads its control and address; the element the
 * address is in (0 when it is the end tag after the last element) gives how many elements
 * were consumed, which advance dmaStart and shrink dmaN. readBytes / 2048 new elements
 * become tags: the previous last tag turns into REF (0x30000080 with its data address), new
 * tags are REF except the last, REFE (0x80 with id 0). readBytes keeps the remainder. When
 * dmaN is not 0 the channel is restarted with control | 0x100 (tag id forced to 3 when new
 * elements were added). Returns 1.
 * MATCHING DEVICE: the tag store as one expression with the inline id choice; the address
 * conversions as written. */
int viBufAddDMA(struct DotrViBufDma *p);

/* Time-stamp views: one 24-byte entry (pts, dts, stream position and length)
 * and the same stream record as DotrViBufDma with the area addresses as 32-bit words, the IPU
 * bit pointer at +0x38, the semaphore at +0x40 and the time-stamp ring (entries, size, count,
 * write index) at +0x50. Member names follow Sony's MPEG streaming sample. On the host the
 * pointer moves the later offsets. Assumed: these meanings, from use. */
typedef struct DotrViBufTimeStamp {
    long long pts;
    long long dts;
    int pos;
    int len;
} DotrViBufTimeStamp;

typedef struct DotrViBufTs {
    unsigned int data;
    unsigned int tag;
    int n;
    int dmaStart;
    int dmaN;
    int readBytes;
    int buffSize;
    unsigned int d4madr;
    unsigned int d4tadr;
    unsigned int d4qwc;
    unsigned int d4chcr;
    unsigned int d3madr;
    unsigned int d3qwc;
    unsigned int d3chcr;
    unsigned int ipubp;
    unsigned int ipuctrl;
    int sema;
    int isActive;
    long long totalcount;
    DotrViBufTimeStamp *ts;
    int n_ts;
    int count_ts;
    int wt_ts;
} DotrViBufTs;

/* DMA channel 4 address (0x1000b410) and the IPU bit pointer (0x10002020) read as unsigned
 * words; the host maps them like DOTR_VIBUF_REG. */
#ifdef __MWERKS__
#define DOTR_VIBUF_UREG(addr) (*(volatile unsigned int *)(addr))
#else
#define DOTR_VIBUF_UREG(addr) (*(volatile unsigned int *)&dotr_vibuf_host_regs[((addr) & 0xffff) >> 4])
#endif

/* 0x244de0 viBufGetTs(stream, out): under the semaphore, sets out's pts and dts to -1, works out
 * the stream position the IPU is decoding (channel 4 address less the FIFO contents, plus the bit
 * pointer's bytes, relative to the data area, modulo n * 2048) and searches the last count_ts
 * time stamps (oldest first) for the one whose [pos, pos + len) holds it: copies its pts and dts
 * to out, marks it used (-1) and decrements count_ts (not below 0). Returns 1. */
int viBufGetTs(void *arg0, void *arg1);

#endif

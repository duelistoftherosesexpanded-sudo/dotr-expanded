#ifndef DOTR_EE_ITEM_WALKS_H
#define DOTR_EE_ITEM_WALKS_H

/* The routine 0x213410 that walks variable-length word records, hands each to 0x2134d0 or
 * 0x2138d0 by a flag bit, and takes the next record from the scratchpad word at 0x700000fc,
 * and the polygon sort lRhModelCpu_SortPolygon (0x213d00).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* The scratchpad at 0x70000000 as 0x213410 reads it: only the word at +0xfc,
 * which the two callees are assumed to leave pointing past the record they
 * consumed. The routine keeps the base in a saved register, so the target
 * build uses a literal address; the host build points the name at an object
 * the test defines. */
struct DotrWalkPad {
    int unknown_00[63];
    unsigned *next;
};
#ifdef __MWERKS__
#define DOTR_WALK_PAD ((struct DotrWalkPad *)0x70000000)
#else
extern struct DotrWalkPad dotr_walk_pad;
#define DOTR_WALK_PAD (&dotr_walk_pad)
#endif

/* Callees (rh_model_cpu); that they take and return the first argument as a
 * running value is assumed from how 0x213410 threads it. */
extern int lRhModelCpu_CalcIndepStrip(int cursor, unsigned *item);
extern int lRhModelCpu_CalcSharedStrip(int cursor, unsigned *item);
int lRhModelCpu_TraceStaticBlock(int cursor, unsigned *item, int size);

/* 0x213d00 lRhModelCpu_SortPolygon (original name): heap sort, ascending by the unsigned
 * word at +0x7c, of the 128-byte records from first up to end, moving records with
 * RhMem_CopyMemory16(destination, source, 8) and holding one record in the scratchpad at
 * 0x70000120. Assumed: the record layout beyond its size and the key word; the scratchpad
 * view (only +0x120 is used here); void result. Record addresses for the copies are formed
 * with integer arithmetic (`(k << 7) + address - 128`), which is what the target's operand
 * order shows; DotrWalkAddr is the address as an integer. */
#ifdef __MWERKS__
typedef int DotrWalkAddr;
#else
typedef __INTPTR_TYPE__ DotrWalkAddr;
#endif
struct DotrWalkPolygon {
    int unknown_00[31];
    unsigned int key;
};
struct DotrWalkSortPad {
    unsigned char unknown_000[0x120];
    struct DotrWalkPolygon temp;
};
#ifdef __MWERKS__
#define DOTR_WALK_SORT_PAD ((struct DotrWalkSortPad *)0x70000000)
#else
extern struct DotrWalkSortPad dotr_walk_sort_pad;
#define DOTR_WALK_SORT_PAD (&dotr_walk_sort_pad)
#endif
extern void *RhMem_CopyMemory16(void *destination, void *source, int count);
void lRhModelCpu_SortPolygon(struct DotrWalkPolygon *first, struct DotrWalkPolygon *end);

#endif

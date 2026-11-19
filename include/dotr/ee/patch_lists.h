#ifndef DOTR_EE_PATCH_LISTS_H
#define DOTR_EE_PATCH_LISTS_H

/* Functions that walk an offset-addressed list of 16-byte items inside a loaded block and copy
 * two quadwords per tagged word into one of two destinations (0x20b380).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* Callee, with the declaration draw_state.h and view_state.h use. */
extern void *RhMem_CopyMemory16(void *destination, void *source, int count);

/* Views for 0x211f70; only the fields it reads are evidenced, the names are
 * descriptions of use. The block header holds, from +0x10, a word at +0x28
 * (offset of the list from the block start), a word at +0x2c tested for zero,
 * and words at +0x48 / +0x4c (offset of the selection record, and a word
 * tested for zero). */
struct DotrPatchSub {
    int unknown_00[10];
    int list_offset;
    int has_list;
    int unknown_30[6];
    int pick_offset;
    int has_pick;
};
struct DotrPatchHead {
    unsigned char unknown_00[16];
    struct DotrPatchSub sub;
};
/* Selection record: words from +0x84 indexed by the fourth argument and a
 * word at +0x184. The array bound is the distance between the two. */
struct DotrPatchPick {
    unsigned char unknown_000[132];
    int choice[64];
    int key;
};
/* The source of each copy is 0x30 bytes into the object an item points to. */
struct DotrPatchObject {
    unsigned char unknown_00[16];
    unsigned char rows[4][16];
};
/* 16 bytes on the EE (the pointer is one word there). */
struct DotrPatchItem {
    struct DotrPatchObject *object;
    int unknown_04[3];
};
/* List header; the items follow it directly. */
struct DotrPatchList {
    int count;
    int runs_offset;
    int unknown_08[2];
};
struct DotrPatchRun {
    int count;
    int words_offset;
};
/* One tagged word: a 28-bit destination offset under a 4-bit tag. */
union DotrPatchWord {
    unsigned int raw;
    struct {
        unsigned int offset : 28;
        unsigned int kind : 4;
    } bits;
};

/* 0x211f70. Does nothing when the header word at +0x3c is zero. Otherwise,
 * for each of the list's `count` items i with its run (count, offset of its
 * words from the list): for every word, tag 2 copies two quadwords
 * (0x20b380 with count 2) from item object + 0x30 to scratchpadAddr1 + offset, tag 4
 * to scratchpadAddr2 + offset, any other tag nothing. The item is the i-th one,
 * except that when a selection record exists (both header words non-zero),
 * its key is not negative, animationIndex is not negative, i equals the key and
 * choice[animationIndex] is within 0..count-1, the item at choice[animationIndex] is used.
 * Assumed: the pointer types, int animationIndex, no result. */
void lRhModel_SetupTexture(struct DotrPatchHead *modelFile, unsigned char *scratchpadAddr1, unsigned char *scratchpadAddr2, int animationIndex);

#endif

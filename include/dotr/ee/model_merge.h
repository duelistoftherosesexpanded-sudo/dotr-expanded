#ifndef DOTR_EE_MODEL_MERGE_H
#define DOTR_EE_MODEL_MERGE_H

/* Read-request callback of the SzModel loader that rebinds merged model data into existing
 * model slots.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* View of the read request record handed to the callback (resource_submit.h and
 * texture_callbacks.h have other views): the loaded data at +0x14 and the user word at +0x1c,
 * here used directly as a merge-group index (no packing assumed). On the host the pointer
 * moves the later offsets. */
struct DotrMergeRecord {
    char unknown_00[0x14];
    char *data;
    int unknown_18;
    int user;
};

/* Eight one-bit fields of the byte at object +0x20c (object_table.h DotrObject.drawFlag).
 * Only bit 0 (first declared) and bit 3 are written here; meanings unknown. */
struct DotrMergeBits {
    signed char bit_0 : 1;
    signed char bit_1 : 1;
    signed char bit_2 : 1;
    signed char bit_3 : 1;
    signed char bit_4 : 1;
    signed char bit_5 : 1;
    signed char bit_6 : 1;
    signed char bit_7 : 1;
};

/* Model object view: +0x1f3 a signed kind byte indexing the ignore table, +0x1f4 a
 * halfword compared with it, +0x20c the bit byte (object_table.h type/modelNo/drawFlag). */
struct DotrMergeObject {
    unsigned char unknown_000[0x1f3];
    signed char kind;
    short number;
    unsigned char unknown_1f6[0x16];
    struct DotrMergeBits bits;
};

/* Four 39-entry tables indexed by the user word, copied whole onto the stack by the
 * callback (MATCHING DEVICE: the copies). Assumed: 0x292ca0 the first model slot of each
 * group, 0x292d40 the slot count, 0x292de0 the number of slots that may be rebound,
 * 0x292e80 per-group lists of byte offsets into the loaded data. */
struct DotrMergeTable {
    int v[39];
};
struct DotrMergeOffsets {
    int *v[39];
};
extern struct DotrMergeTable data_00292ca0_merge;
extern struct DotrMergeTable data_00292d40_merge;
extern struct DotrMergeTable data_00292de0_merge;
extern struct DotrMergeOffsets data_00292e80_merge;
/* 0x2cf910: halfword limit per object kind (Japanese-build neighbour name suggests
 * gSzModel_awIgnore; not verified). */
extern unsigned short gSzModel_awIgnore_merge[];
/* 0x31abb0: model object pointer table (gSzModel_ahModel in the Japanese build). */
extern struct DotrMergeObject *gSzModel_ahModel_merge[];

/* lSzModel_Clean (0x1e7d00, object_table); result unused here. */
int lSzModel_Clean(int modelId);
/* lSzModel_BindModelByAddr (0x1f0500, object_table, which types the address as a bind
 * header); result unused here. */
int lSzModel_BindModelByAddr(int modelId, void *addr);

/* 0x1e84d0 lSzModel_LoadMergeCB: for each existing model slot of group `user` clears bit 3
 * of +0x20c. When the read completed it also, for each slot j whose number is below the
 * kind's limit, j below the group's rebind limit, and whose data word at the j-th offset is
 * 0x304852, clears bit 0, cleans the slot and binds it to that data address.
 * MATCHING DEVICE: the four whole-table stack copies, declared first; every use indexed
 * again rather than hoisted. */
void lSzModel_LoadMergeCB(struct DotrMergeRecord *req, int ok);

#endif

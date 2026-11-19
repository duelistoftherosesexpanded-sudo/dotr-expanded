#ifndef DOTR_EE_OBJECT_FREES_H
#define DOTR_EE_OBJECT_FREES_H

/* Functions that release the resources hanging off one object of the 84-pointer table at
 * 0x31abb0 (first member 0x1f08d0).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* Head word pair at the address an object keeps at +0x138: the flag word at
 * +4 is tested whole and with 0x80000 / 0x100000 / 0x200000. */
struct DotrFreeHead {
    int unknown_00;
    int unknown_04;
};

/* Descriptor at object +0x13c: byte offset from the head at +0x28, used only
 * when the word at +0x2c is not 0. */
struct DotrFreeDesc {
    unsigned char unknown_00[0x1c];
    int unknown_1c;
    unsigned char unknown_20[8];
    int unknown_28;
    int unknown_2c;
};

/* Table found at head + desc +0x28: a count, then 16-byte entries from +0x10
 * whose first and third words are released when not null. */
struct DotrFreeEntry {
    void *unknown_0;
    int unknown_4;
    void *unknown_8;
    int unknown_c;
};
struct DotrFreeList {
    int count;
    int unknown_04[3];
    struct DotrFreeEntry entries[1];
};

struct DotrFreeBits {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bit_2 : 1;
    signed char unknown_bit_3 : 1;
    signed char unknown_bit_4_7 : 4;
};

/* This group's view of the objects object_table.h calls DotrObject. Address
 * words are pointers here, so host offsets after +0x138 differ from the
 * target's (stated in the comments). */
struct DotrFreeObject {
    unsigned char unknown_000[0x138];
    struct DotrFreeHead *modelBaseData;
    struct DotrFreeDesc *unknown_13c;
    void *mainEffectTexture;
    void *effectTextureB[8];
    int OverlayM;
    int OverlayB;
    void *pCameraMotion;
    void *pCameraMotion2;
    void *ptrBlock;
    void *AnimationDataPointers[8];
    unsigned char unknown_198[0x54];
    int effectCallback;
    unsigned char unknown_1f0[3];
    signed char type;
    unsigned char unknown_1f4[0x18];
    struct DotrFreeBits drawFlag;
};
#ifdef __MWERKS__
typedef char DotrFreeObjectBitsAt20c[(sizeof(struct DotrFreeObject) == 0x210) ? 1 : -1];
#endif

/* The 84 object pointers at 0x31abb0 and the 32 entries of 0x70 bytes that
 * follow at 0x31ad00 (signed 27-bit field at +0x68), as object_table.h has
 * them under other names. */
struct DotrFreeEntry112 {
    unsigned char unknown_00[0x68];
    signed int unknown_68_bits_0_26 : 27;
    signed int unknown_68_bits_27_31 : 5;
    unsigned char unknown_6c[4];
};
extern struct DotrFreeObject *gSzModel_ahModel_free[84];
extern struct DotrFreeEntry112 data_0031ad00_free[32];

/* Callees defined in other groups. */
extern void SzMem_Free(void *pointer);
extern int lSzModel_FreeMotion(int modelId, int slot);
extern int SzView_FreeRequest(int a, int b);
extern int SzEffect_Destroy(int index);

/* 0x1f08d0. Answers 0 for an index outside 0..83 or a null table entry, 1 at
 * once for an object whose +0x138 is null, and 0 for one with bit 3 of +0x20c
 * set. Otherwise: when the head's +4 word is not 0, clears +0x178[0..7],
 * +0x140 and +0x144[0..7] without releasing; when it is 0, releases the
 * non-null first and third words of each list entry (only if desc +0x2c is
 * not 0), calls lSzModel_FreeMotion(index, 0..7), and releases and clears +0x140 and
 * each non-null +0x144[i]. Then clears +0x164 / +0x168, calls
 * SzView_FreeRequest(0, 1), and for +0x16c / +0x170 / +0x174 clears the word,
 * releasing it first unless the head's +4 has 0x80000 / 0x100000 / 0x200000.
 * Calls SzEffect_Destroy(+0x1ec) when that is not negative, releases the head when
 * bit 0 of +0x20c is set, clears +0x138, sets the 27-bit field of every
 * entry 1..31 of 0x31ad00 that equals index to -1 and answers 1.
 * Assumed: int index and result. */
int lSzModel_FreeModel(int modelId);

/* SzModel_DrawWithFunc_Pipe (model_func_draw.h, 4564 bytes): nine arguments as 0x1ee4b0 passes them;
 * the last one goes on the stack as a zero. Result assumed to be the cursor. */
extern int SzModel_DrawWithFunc_Pipe(int cursor, int index, int translationRelated, void *rotationRelated, int scaleRelated,
                                     int f, int zbufferPtr, void (*callback)(void), int zero);
/* 0x1ee570: only its address is used here. */
extern void lSzModel_Draw_DefFunc(void);
/* 0x1ee4b0. Answers cursor unchanged when index is outside 0..83, the object is
 * absent, or its +0x138 or +0x13c word is 0. Otherwise forwards all seven
 * arguments to SzModel_DrawWithFunc_Pipe with a callback and a ninth zero: the
 * callback is lSzModel_Draw_DefFunc when the +0x1f3 byte is 4, 5 or 6 and the
 * header's +0x1c word is not 0, else null. Arguments 3 to 7 are untouched pass-throughs; their types follow the
 * declaration object_table.h has for its callers. */
int SzModel_Draw_Pipe(int cursor, int modelId, int c, void *d, int e, int f, int g);

#endif

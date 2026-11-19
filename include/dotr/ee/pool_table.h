#ifndef DOTR_EE_POOL_TABLE_H
#define DOTR_EE_POOL_TABLE_H

/* Offset view of the 852-byte object at EE 0x31bb50: 32 eight-byte entries,
 * 24 twenty-four-byte entries and four words. Field meanings are unknown.
 * packed_item.h declares an incompatible prefix view of the same storage
 * under a different symbol. Evidence: docs/modules/effects.md.
 * Pointer members make host sizes differ from the target. */
struct DotrPoolEntry8 {
    unsigned short unknown_bits_0_2 : 3;
    unsigned short unknown_bits_3_5 : 3;
    unsigned short unknown_bits_6_15 : 10;
    unsigned char unknown_02[2];
    void *unknown_04;
};

struct DotrPoolEntry24 {
    unsigned short unknown_00_bits_0_2 : 3;
    unsigned short unknown_00_bits_3_15 : 13;
    unsigned short unknown_02_bits_0_2 : 3;
    unsigned short unknown_02_bits_3_12 : 10;
    signed short unknown_02_bit_13 : 1;
    signed char unknown_02_bit_14 : 1;
    signed char unknown_02_bit_15 : 1; /* signed: 0x1f4900 reloads the byte with lb */
    unsigned short unknown_04; /* +4 and +6: passed to 0x1f56d0 by 0x1f4cb0 */
    unsigned short unknown_06;
    void (*unknown_08)(int);   /* +8: called by 0x1f4cb0 with the word at +0xc when set */
    int unknown_0c;
    int unknown_10;
    int unknown_14;
};

struct DotrPool {
    struct DotrPoolEntry8 unknown_000[32];
    struct DotrPoolEntry24 unknown_100[24];
    int unknown_340;
    int unknown_344;
    int unknown_348;
    int unknown_34c;
    int unknown_350;
};

#ifdef __MWERKS__
typedef char DotrPoolMustBe354[(sizeof(struct DotrPool) == 0x354) ? 1 : -1];
#endif

/* Three records at EE 0x292ff0: a zero-terminated word list and its count. */
struct DotrPoolList {
    int *unknown_list;
    int unknown_04;
    int unknown_count;
};

extern struct DotrPool gSzEffect_el_pool;
extern struct DotrPoolList gSzEffect_aEffFuncTbl[];
/* One block pointer at EE 0x31beb0, after the pool object; the block is
 * 2,240 bytes (560 words). Only index 0 is accepted. */
extern int *gSzView_hView[];

void SzEffect_Init(void);
void SzEffect_Clear(void);
void SzEffect_TrimModule(void);
struct DotrPoolEntry8 *SzEffect_GetModuleInfo(int a, int b);
/* 0x1f3f20: for each 24-byte entry whose +2 selector is 0 and whose state is
 * 1, find the 8-byte entry with selector 0 and the same 10-bit id (the body
 * of 0x1f3ea0 repeated in place); when that entry is in state 2 and its
 * pointer is set, walk at most 64 four-byte records from the pointer up to a
 * tag of 15 and send SzSound_PlaySE(1, value) for each record whose 12-bit time is
 * at least (13-bit field of the entry minus SzGame_GetFrameSpeed()), as a halfword, and
 * below that field. The record layout is assumed from this function alone;
 * the two callees are declared as in object_table.h. */
struct DotrPoolCue {
    unsigned short tag : 4;
    unsigned short time : 12;
    unsigned short value;
};
extern int func_001a8760_int(void);
extern int func_00217710_cue(int index, int value);
void SzEffect_ReqSE(void);
int SzEffect_Sleep(int index, int flag);
int SzEffect_Sleep2(int a, int b, int flag);
int SzEffect_IsUse(int index);
int SzView_Create(int viewIndex);
int SzView_Destroy(int viewId);
int SzEffect_IsUse2(int a, int b);

/* Dependencies, defined in other groups. */
extern void SzMem_Free(void *pointer);
extern void RhMem_FillMemory(void *destination, int value, int size);
extern void SzEffect_SetBlackOutForMonster(int value);
extern void SzEffect_SetBlackOutForDuel(int blackoutAmount);
extern void SzEffect_SetBlackOutForBattle(int blackOutValue);
extern void *SzMem_Alloc(int byte_count);
extern int *RhMem_FillMemory4(int *destination, int value, int count);
extern void lSzView_InitVars(int index);

/* 0x1f4cb0: 0 for an index outside 0..23. Otherwise, when the entry's +2
 * selector is 1 or 2, calls SzEffect_ClearRefID2(halfword +6, selector) and
 * SzEffect_ClearRefID2(halfword +4, selector); clears the entry's state; calls the
 * function at +8, when set, with the word at +0xc; then finds the 8-byte
 * entry with the same selector and 10-bit id (the body of 0x1f3ea0 in place)
 * and, when that entry is in state 2 and SzEffect_IsUse2(selector, id) now answers
 * 0, puts it in state 3. Answers 1.
 * Assumed: the callback's signature (one int, result unused); 0x1f56d0
 * (defined in packed_item) declared as there. */
extern int SzEffect_ClearRefID2(int packed, int value);
int SzEffect_Destroy(int index);
/* 0x1f5090. For each of the 24 entries whose selector (+2 bits 0-2) equals a
 * (any when a < 0) and whose 10-bit id equals b (any when b < 0): an entry
 * in state 2 is given to SzEffect_Destroy(i), an entry in state 1 gets bit 14 of the
 * halfword at +2 set and counts as 1, any other state as 0. Answers the OR
 * of those. The bit is written through a signed byte at +3, hence the
 * one-bit signed char member. Assumed: int arguments and result. */
int SzEffect_Erase2(int a, int b);

/* 0x1a22c0 (library code): float to int, as scaled_sprite.h has it. */
extern int func_001a22c0(float value);
/* 0x1fafb0. For index 0 with a block and a non-null out: out[0] = -convert(32768 *
 * block float +8 / pi), out[1] = 16384 - convert(32768 * block float +4 / pi),
 * out[2] = out[3] = 0, answer 1; otherwise 0 and nothing written. Assumed: int
 * result and four ints out (block_table.h calls it through func_001fafb0_angles;
 * small_misc.h's caller ignores the result); the block words are read as floats
 * through a cast. */
int SzView_GetLookVector(int index, int *out);

/* RhGeom_GetRotXYZMatrix (small_misc.h): builds a matrix from four angle words. */
extern void RhGeom_GetRotXYZMatrix(float *m, int *ang);
/* 0x1fb0b0. With a non-null m, index 0 and a block: computes the four words SzView_GetLookVector
 * computes into a local and hands them to RhGeom_GetRotXYZMatrix(m, local), answering 1; otherwise
 * 0 with no call. The target has no call to SzView_GetLookVector here and no test of the local's address, so
 * the source carries an inline copy without that test (an assumption about the original source, not
 * a recovered helper). */
int SzView_GetLookMatrix(int index, float *m);

/* 0x293060: sixteen bytes copied whole (lq/sq) into a local of SzView_GetLookMatrixWithRoll and
 * used as four angle words. Assumed: a four-int structure object; the target may
 * equally be the initialiser of a local array, which compiles the same. */
struct DotrPoolAngles {
    int v[4];
};
extern struct DotrPoolAngles data_00293060;
/* RhGeom_MulMatrix (view_state.h / small_misc.h name matrix structures): two pointers to
 * sixteen floats, declared here as float pointers. */
extern void RhGeom_MulMatrix(float *m, float *other);
/* 0x1fb1d0. Copies data_00293060 to a local and stores minus the block's word +64
 * (0 when the index is not 0 or there is no block) as its third word, before any
 * test. Then like SzView_GetLookMatrix: 0 for a null m, a bad index or no block;
 * otherwise RhGeom_GetRotXYZMatrix(m, angles), RhGeom_GetRotXYZMatrix(local, the copied
 * words), RhGeom_MulMatrix(m, local) and 1. Both helper bodies are inline copies (assumption,
 * as at SzView_GetLookMatrix). */
int SzView_GetLookMatrixWithRoll(int index, float *m);

/* 0x1f3e40: load callback for the eight-byte pool entry picked by the record's word at
 * 0x1c: state 1 becomes 3, and the record's word at 0x14 is stored at +4 when the load
 * completed. The function reads the state through a byte, so the entry is viewed with
 * byte bit-fields (a cast of the DotrPoolEntry8 entry, not another symbol); the record is
 * assumed to be the resource submit record, declared here by its offsets. */
struct DotrPoolEntryState {
    unsigned char state : 3;
    unsigned char rest : 5;
    unsigned char unknown_01[3];
    int unknown_04;
};
struct DotrPoolLoadRecord {
    int unknown_00[5];
    int unknown_14;
    int unknown_18;
    int unknown_1c;
};
void lSzEffect_LoadSECtlCB(struct DotrPoolLoadRecord *record, int completed);

/* 0x1f3cb0: load callback for an overlay request made for the eight-byte pool entry
 * picked by the record's word at 0x1c. The record is viewed by its offsets as in
 * resource_submit.h (31-bit size at +8, one-bit flag in the top bit of +0xc), with the
 * word at 0x14 as the loaded address; that header cannot be included here
 * (RhMem_FillMemory is declared differently). 0x19c100 and 0x19b648 are library
 * routines used as a three-byte comparison and a formatter; the three strings at
 * 0x2cfbc6, 0x2cfbcb and 0x2cfbcf are not described here. Assumed: every signature here. */
struct DotrPoolOverlayRecord {
    int unknown_00[2];
    signed int byte_size : 31;
    signed int flag_008 : 1;
    signed int byte_offset : 31;
    signed int flag_00c : 1;
    int unknown_10;
    char *unknown_14;
    int unknown_18;
    int unknown_1c;
};
extern char data_002cfbc6[];
extern char data_002cfbcb[];
extern char data_002cfbcf[];
extern int func_0019c100(const char *a, const char *b, int count);
extern int func_0019b648(char *destination, const char *format, ...);
extern char *SzFile_GetEnv(const char *label);
extern void *SzFile_ReadRequestParam(int fileType, int priority, char *name, int unknown_014, int byte_size, int byte_offset,
                                     int unknown_010, void (*callback)(struct DotrPoolLoadRecord *record, int completed),
                                     int unknown_01c);
void lSzEffect_LoadOverlayCB(struct DotrPoolOverlayRecord *record, int completed);

/* 0x1f4900 SzEffect_EntryCallback (name from the Japanese build's symbols).
 * Starts effect `id` of table `type` (0-2) in the first free 24-byte entry
 * and answers its index, or -1. Assumed: the parameter meanings beyond what
 * the stores show (arg -> +0x14, target -> +4, from -> +6, callback -> +8,
 * callback_arg -> +0xc); that the words of DotrPoolList.unknown_list are the
 * effect functions, taking (work address, arg) and answering a status where 8
 * means "finished at once"; that SzEffect_LoadModule (0x1f3910, effect_module)
 * answers the 8-byte entry of the loaded module; the overlay window at
 * 0x1e00000 whose word +0x1c is its end address. The work and module
 * addresses are integers in the image; the host build widens them and
 * resolves the function word through a test hook. */
#ifdef __MWERKS__
typedef int DotrPoolAddr;
#define DOTR_POOL_OVERLAY ((int *)0x1e00000)
#define DOTR_POOL_EFFECT_FUNCTION(word) ((int (*)(int, int))(word))
#else
typedef __INTPTR_TYPE__ DotrPoolAddr;
extern int dotr_pool_overlay[8];
#define DOTR_POOL_OVERLAY (dotr_pool_overlay)
extern int (*dotr_pool_effect_function(int word))(DotrPoolAddr, int);
#define DOTR_POOL_EFFECT_FUNCTION(word) (dotr_pool_effect_function(word))
#endif
extern int SzDuel_GetUnitModelID(int side, int pos);
extern int SzModel_GetOverlayB(int modelId);
extern int SzModel_GetOverlayM(int modelId);
extern struct DotrPoolEntry8 *SzEffect_LoadModule(int a, int b, int c);
extern void *RhMem_CopyMemory16(void *destination, void *source, int count);
extern void func_0017bb90(int unknown);
extern void mwOverlayInit(void *address, int initialized_size);
extern void SzEffect_SetTarget(int effectTarget);
extern void SzEffect_SetFrom(int effectSource);
extern void RhGeom_PushMatrix(float *matrix);
extern void RhGeom_PopMatrix(void);
int SzEffect_EntryCallback(int moduleType, int id, int arg, int target, int from, void (*callback)(int), int callback_arg);

/* 0x1f40d0 SzEffect_Draw: after SzGame_GetMode(1) (answer unused) and RhGeom_PushMatrix(0),
 * runs every 24-byte entry in state 1 whose selector equals type (any when type < 0), then
 * RhGeom_PopMatrix(). Per entry: the module address is the B (type 1) or M (type 2)
 * overlay of the unit model at `from`, else that of the 8-byte entry with the same
 * selector and id, which must be in state 2 (entry skipped otherwise); the module is
 * copied into the overlay window unless the pool's +0x340/+0x344 already name this
 * selector and id; target and from are set; the effect function is called with the
 * work address (0x1ed0000 for selector 1, else 0x1e10000 + 0x8000 * i) and -1, -2
 * (bit 13) or -3 (bit 14); its answer goes to +0x10 and the 13-bit counter is
 * advanced. For selector 1 the answer then drives the target unit's model animation
 * and camera requests (see docs/modules/effects.md); otherwise an answer of 8
 * puts a bit-13 entry in state 2 and destroys any other.
 * Assumed: the callee prototypes below as object_table.h, block_table.h and
 * cell_forwards.h have them; SzView_ReqCamera (0x1fa8a0) takes its fourth
 * argument as an unsigned short here (the caller masks it at each call); func_0019a5c8
 * (library code, unnamed) answers the next pseudo-random int. */
extern int SzGame_GetMode(int a);
extern int func_0019a5c8(void);
extern int SzEffect_GetPlayMotion(int a);
extern int SzModel_GetAnim(int modelId);
extern int SzModel_GetAnimParam(int modelId, int paramIndex);
extern int SzModel_GetAnimDir(int modelId);
extern float SzModel_GetAnimFrame(int modelId);
extern int SzModel_GetAnimFrameMax(int modelId, int animIndex);
extern int SzView_FreeRequest(int a, int b);
extern void SzView_ReqCamera(int a, int b, int c, unsigned short d, int index, void *data);
extern int SzModel_SetStoneFlag(int modelId, int value);
extern int SzModel_SetMotionDelay(int modelId, int motionDelay);
extern int SzModel_SetAnimFrame(int modelId, float animFrame);
extern int SzModel_SetAnimSpeed(int index, float value);
extern int SzModel_SetAnimIntrReverse(int index);
extern int SzModel_SetAnimIntr(int modelId, int animIndex);
extern int SzModel_SetAnimIntrParam(int modelId, int animIndex, int unknown);
void SzEffect_Draw(int type);

#endif

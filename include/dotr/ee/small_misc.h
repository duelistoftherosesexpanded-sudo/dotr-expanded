#ifndef DOTR_EE_SMALL_MISC_H
#define DOTR_EE_SMALL_MISC_H

/* Miscellaneous small routines from across the EE program (0x1038d0-0x286900), grouped for
 * convenience only. Views are offset hypotheses with unknown meanings.
 * Evidence: the functions' entries in docs/modules/. */
struct DotrMiscLarge {
    unsigned char unknown_00000[0x50000];
    int unknown_50000;
    int unknown_50004;
    int unknown_50008;
};

/* One object view shared by several routines that take a pointer; whether
 * they operate on the same original type is not established. */
struct DotrMiscObject {
    unsigned char unknown_00[8];
    int unknown_08;
    int unknown_0c;
    int unknown_10;
    unsigned char unknown_14[0x30];
    int unknown_44;
    int unknown_48;
    unsigned char unknown_4c[8];
    int unknown_54;
    int unknown_58;
    unsigned char unknown_5c[0x4c];
    int unknown_a8;
};

/* 0x245960 reloads the counter after testing it, which a volatile member
 * reproduces; whether the original was volatile is unknown. */
struct DotrMiscCounter {
    unsigned char unknown_00[12];
    volatile int unknown_0c;
};

struct DotrMiscPair {
    int unknown_00;
    int unknown_04;
};

struct DotrMiscEntry52 {
    int unknown_00;
    unsigned char unknown_04[44];
    int unknown_30; /* word written and tested by 0x246340 */
};

struct DotrMiscEntry7 {
    unsigned char unknown_bytes[7];
};

/* Object read by 0x1dbd30 and a three-float vector. */
struct DotrMiscSource {
    unsigned char unknown_00[72];
    unsigned char unknown_48[48];
    int unknown_78;
    unsigned char unknown_7c[6];
    short unknown_82_bits_0_11 : 12;
    short unknown_82_bits_12_15 : 4;
};

struct DotrMiscVector3 {
    float x, y, z;
};

/* DotrMiscTag: the four words written by 0x26eea0. DotrMiscHigh: the five fields
 * written by 0x228940 at 0x1ed01a4, an address above the main executable's data. */
struct DotrMiscTag {
    int unknown_00;
    int unknown_04;
    int unknown_08;
    int unknown_0c;
};

struct DotrMiscHigh {
    short unknown_00;
    short unknown_02;
    unsigned char unknown_04[8];
    short unknown_0c;
    short unknown_0e;
    unsigned char unknown_10;
};

extern int gRhZSort_zsl[];
extern struct DotrMiscHigh data_01ed01a4;
/* Further object views (0x317490 is the control storage). */
struct DotrMiscControlWords {
    unsigned char unknown_000[0x6a0];
    int unknown_6a0[8];
};

struct DotrMiscNode {
    unsigned char unknown_00[0x90];
    int unknown_90;
};

struct DotrMiscEntry160 {
    unsigned char unknown_bytes[160];
};

/* View of one gSzDuel_aCardControl element as 0x1cdd90 uses it: a 26-bit field in the
 * word at +0x9c and six one-bit fields in the byte at +0x9f (signed char so the
 * compiler uses byte accesses; signedness is unobserved, only zero is stored). */
struct DotrMiscSlot160 {
    int unknown_00;
    unsigned char unknown_04[0x94];
    short unknown_98;
    short unknown_9a;
    unsigned unknown_9c_bits_0_25 : 26;
    signed char unknown_9f_bit_2 : 1;
    signed char unknown_9f_bit_3 : 1;
    signed char unknown_9f_bit_4 : 1;
    signed char unknown_9f_bit_5 : 1;
    signed char unknown_9f_bit_6 : 1;
    signed char unknown_9f_bit_7 : 1;
};

/* Prefix view assumed from 0x243230/0x243290; whether it is the same original
 * type as DotrMiscObject is not established. */
struct DotrMiscStream {
    int unknown_00;
    unsigned char unknown_04[0x40];
    int unknown_44;
    int unknown_48;
    int unknown_4c;
    int unknown_50;
};

struct DotrMiscEntry12 {
    unsigned char unknown_00[7];
    signed char unknown_07_bits_0_5 : 6;
    signed char unknown_07_bit_6 : 1;
    signed char unknown_07_bit_7 : 1;
    unsigned char unknown_08[4];
};

struct DotrMiscVector4 {
    float x, y, z;
    float unknown_w;
};

struct DotrMiscTriple {
    struct DotrMiscVector4 unknown_00;
    struct DotrMiscVector4 unknown_10;
    struct DotrMiscVector4 unknown_20;
};

struct DotrMiscEntry16 {
    unsigned short unknown_00;
    unsigned char unknown_02[14];
};

/* 65440 bytes; word members because 0x23f5a0 copies one through the pointer
 * at 0x34fae8 two words at a time. */
struct DotrMiscLargeEntry {
    int unknown_words[16360];
};

/* Same address as the control view in state_records.h, under a distinct symbol. */
extern struct DotrMiscControlWords data_00317490_words;
extern struct DotrMiscEntry160 gSzDuel_aCardControl[];
/* 32 elements inferred from the 0x1cda10 loop; they end where gSzDuel_aCardControl starts. */
extern struct DotrMiscEntry160 gSzDuel_aCardRequest[32];
extern int gSzView_pCamera[];
extern struct DotrMiscEntry12 gSzFade_fl[];
extern struct DotrMiscLargeEntry gSzData_All[];
extern int gSh_FaceMode[];
extern int gSh_pbVif1[];
extern int isCountVblank[];
extern int isWithAudio[];
extern struct DotrMiscObject audioDec;
extern int frd[];
extern char data_0030f3ae[];
extern char data_0030f8e9[];
extern int data_003506c8[];
extern struct DotrMiscEntry52 gSh_Face[];
extern struct DotrMiscEntry7 data_0029d586[];
extern unsigned char EnchantId[];

void readBufCreate(struct DotrMiscLarge *p);
void readBufDelete(void);
void endDisplay(void);
void switchThread(void);
int isAudioOK(void);
void defMain(void);
void ErrMessage(int a);
int audioDecIsPreset(struct DotrMiscObject *p);
int videoDecSetStream(void);
int videoDecBeginPut(struct DotrMiscObject *p);
int videoDecEndPut(struct DotrMiscObject *p);
void videoDecReset(struct DotrMiscObject *p);
void videoDecAbort(struct DotrMiscObject *p);
int videoDecGetState(struct DotrMiscObject *p);
int mpegError(void *unused, struct DotrMiscPair *pair);
int mpegNodata(void);
int mpegStopDMA(void);
int mpegRestartDMA(void);
void voBufDelete(void);
void voBufReset(struct DotrMiscObject *p);
int voBufIsFull(struct DotrMiscObject *p);
void voBufDecCount(struct DotrMiscCounter *p);
void ShFace_Close(unsigned char *p);
void ShFace_Term(void);
int Ta_GetToonAgreement(int cardNo);
/* Maps the 0x19a5c8 result modulo 6 to one of six constants; no arguments are
 * assumed (none is read). */
int TaEnc_GetRandomElf(void);
int TaEnc_IsActReflexEnc(int index);
int TaEnc_GetEncID(int cardNumber);

void lSzGame_TermMemCheck(void);
void lSzGame_TermTitle(void);
int SzDuel_StartWindowSession_Pipe(int a);
int lSzDuel_DrawWindow_Board_Pipe(int a,int b,int c,int d,int e,int f,int g);
int SzDuel_DrawWindow_Digit_Pipe(int a,int b,int c,int d,int e,int f);
void SzDuel_TermBuildDeck(void);
void lSzDuel_SortBag(void);
void SzDuel_TermTrade(void);
void SzDuel_TermCustomDuel(void);
void SzDuel_TermMapEdit(void);
int SzDuel_DrawCard_Pipe(int a,struct DotrMiscSource *b);
void SzEvent_Term(void);
void SzModel_SetDrawCache(int v);
int SzEffect_Entry(int a,int b,int c,int d,int e);
int SzView_Init(void);
unsigned char*SzMsg_Initialize(void);
void SzMsg_SetDefPrioZ(int v);
int SzFile_InqRequest(int a);
int RhFile_Term(void);
void RhFile_VSyncProcess(void);
int RhGeom_Init(void);
int RhGeom_Term(void);
int RhGeom_ResetMatrixStack(void);
void GetAnyCoordinateInit(void);
void SzData_SetRoseFlag(int val);
void Taku_Vector_Set(struct DotrMiscVector3 *a, struct DotrMiscVector3 *b);
int Taku_Get_Duel5_flg(void);
void Taku_Set_Duel5_flg(void);
int Taku_Get_WinCount(void);
/* Called with the 22-bit flag list by Ta_Eff_000 (0x2560c0); the argument is unused. Parameter
 * name assumed. */
int TaUtil_GetObjSideNum(unsigned int dwObj);
int Ta_IsUnitControl(void);
int TaView_ReqFireView(int a,int b,unsigned char c,unsigned char d);
int TaHand_GetCardSummonCost(int cardNo);
void TaLogic_InitIgnoreLst(void);
int Ta_LkCheckFlag(int a,int b);
void TaTuto_Init032(void);
void ShSlot_Term(void);

/* Data and dependency declarations (RhMem_FillMemory is defined in memory_ops,
 * SzFile_InqRequestX in resource_helpers; the other prototypes are assumed from the call sites). */
extern int SzDuel_StartWindowSessionWithPalette_Pipe(int,int);
extern int lSzDuel_DrawWindow_ExBoard_Pipe(int,int,int,int,int,int,int,int);
extern int SzDuel_DrawWindow_ExDigit_Pipe(int,int,int,int,int,int,int,int);
extern int data_00317b68[];
extern void func_00199bc0(int,int,int,int (*)(void));
extern int lSzDuel_SortFunc1(void);
extern int gSzModel_pfhCache[];
extern int SzEffect_EntryCallback(int,int,int,int,int,int,int);
extern unsigned char gSzMsg_ml[];
extern unsigned char*RhMem_FillMemory(unsigned char*,int,int);
extern int data_0031c2e4[];
extern int SzFile_InqRequestX(int,int);
extern int data_0032a220[];
extern int require_coordinate_no[];

/* Eighty-byte entries at EE 0x32a780 counted by the word at 0x32a728; only
 * the first two words are observed. */
struct DotrMiscEntry80 {
    int unknown_00;
    int unknown_04;
    unsigned char unknown_08[8];
    float unknown_10[16];
};

extern struct DotrMiscEntry80 require_coordinate[];
/* A 51 x 41 halfword table at EE 0x2d0970; the low 12 bits are returned. */
extern unsigned short gSzData_ConstDeck[51][41];

int BookingGetAnyCoordinate(int value);

/* 0x212e60: looks for the entry whose first word is id among the counted
 * entries. State word -1 answers 0. State 1 is cleared and the entry's 64
 * bytes at +0x10 are replaced through a local copy of the 64 bytes at
 * 0x32a730 handed to RhGeom_MulMatrix(local, entry + 0x10). With the state then
 * 0 those 64 bytes are copied to out. Answers 1 when an entry was found, 0
 * when the count is 0, id is not positive, out is null or nothing matched.
 * Assumed: int id, the 64 bytes as sixteen floats (RhGeom_MulMatrix's type; the
 * object at 0x32a730 is view_state.h's, left incomplete here), unsigned
 * index, func_00198790 as sound_file.h declares it. */
struct DotrMiscMatrix;
struct DotrViewMatrix;
extern struct DotrViewMatrix InvertView;
extern void *func_00198790(void *destination, const void *source, int byte_count);
int GetAnyCoordinate(int id, struct DotrMiscMatrix *out);

/* 1,024 twelve-byte entries at EE 0x3229a0: a word, an unobserved word, a
 * halfword of flag bits with a signed 13-bit field, and a signed halfword. */
struct DotrMiscEntry12Bits {
    signed short unknown_bit_0 : 1;
    signed short unknown_bit_1 : 1;
    signed short unknown_bit_2 : 1;
    signed short unknown_bits_3_15 : 13;
};

struct DotrMiscScanEntry {
    int unknown_00;
    int unknown_04;
    struct DotrMiscEntry12Bits unknown_08;
    short unknown_0a;
};

extern struct DotrMiscScanEntry gSzText_aItem[];

/* Three halfwords at EE 0x322990: a base, a limit and a running offset. */
struct DotrMiscAccumulator {
    unsigned short unknown_00;
    unsigned short unknown_02;
    unsigned short unknown_04;
};

extern struct DotrMiscAccumulator gSzText_tl[];

int SzText_SleepRequest(int a, int b, int flag);
int SzText_FreeRequest(int textureFlags, int textureId);
int SzText_InitClutManage(int base, int size);
int SzText_GetClutGsAddr(int count);

/* Pure helpers on caller-supplied bytes; see docs/modules/other.md. */
struct DotrMiscNibbles {
    unsigned char col : 4; /* 15: no goal */
    unsigned char row : 4;
};

struct DotrMiscWordAt8 {
    unsigned char unknown_00[8];
    unsigned goalLoc1;
};

int TaLogic_GetArrayLength(unsigned char *array);
void TaLogic_SetRootTreeDat(unsigned char *currentTree, int parentTree, int col, int row);
int TaLogic_GetMinGoalCount(struct DotrMiscNibbles *pathfinderBuffer);
void TaLogic_InitRogicControl(struct DotrMiscWordAt8 *unit, unsigned char goalId);

/* An address held in a word: 32 bits on the target. */
#ifdef __MWERKS__
typedef int DotrMiscAddress;
#else
#include <stdint.h>
typedef intptr_t DotrMiscAddress;
/* Fills the 16-float matrix m from three int phases at ang[0..2]; elements 3, 7, 11, 12-14 are
 * 0, 15 is 1. */
void RhGeom_GetRotXYZMatrix(float *m, int *ang);

#endif

/* Relocation walkers 0x214190/0x214210: only the first words of each element
 * are read; the second array gives the 16- or 24-byte target stride (on the
 * host the elements are larger because the words are pointer-sized). */
struct DotrMiscFixup16 {
    DotrMiscAddress unknown_00[2];
    DotrMiscAddress unknown_08[2];
};
struct DotrMiscFixup24 {
    DotrMiscAddress unknown_00[3];
    DotrMiscAddress unknown_0c[3];
};
int lNjaReloc_AdjustMData2(struct DotrMiscFixup16 *p, int delta, int count, int flag);
int lNjaReloc_AdjustMData3(struct DotrMiscFixup24 *p, int delta, int count, int flag);

/* 0x20f250: a header that holds its own address at +0x0c, a byte offset at
 * +0x10 to 48-byte entries and a signed count at +0x58; each entry's word at
 * +0x04 is rebased by the header address. Other bytes are unobserved and the
 * address words are wider on the host. */
struct DotrMiscPackedEntry48 {
    int unknown_00;
    DotrMiscAddress unknown_04;
    unsigned char unknown_08[0x28];
};
struct DotrMiscPackedHeader {
    unsigned char unknown_00[0x0c];
    DotrMiscAddress unknown_0c;
    int unknown_10;
    unsigned char unknown_14[0x44];
    int unknown_58;
};
void RhText_BindAddress(struct DotrMiscPackedHeader *header);

/* 0x213f10: 128-byte records; a record whose word at +0x04 equals the previous
 * kept value is blanked. The constant 0x10000000 is not interpreted. */
struct DotrMiscPacket128 {
    unsigned unknown_00;
    unsigned unknown_04;
    unsigned unknown_08;
    unsigned unknown_0c;
    unsigned char unknown_10[0x70];
};
void lRhModelCpu_CleanupPacket(struct DotrMiscPacket128 *packet, struct DotrMiscPacket128 *end);

/* 0x212fd0: calls lRhModelCpu_SetupGlobal(flag), writes four words and four doublewords
 * (0x10000002, 0, 0x11000000, 0x51000002; 0x1000000000008001, 14, 115, 0) at
 * head, then has lRhModelCpu_TraceDynamicBlock fill the 128-byte records that follow. With no
 * end it answers head; otherwise lRhModelCpu_SortPolygon and lRhModelCpu_CleanupPacket run over the
 * records and the end is the answer. The constants are not interpreted.
 * Assumed: argument and result types (void * result because both head and end
 * are returned), the two ints passed through, the callee prototypes. */
struct DotrMiscPacketHead {
    unsigned words[4];
    unsigned long tags[4];
};
extern int lRhModelCpu_SetupGlobal(int flag);
extern struct DotrMiscPacket128 *lRhModelCpu_TraceDynamicBlock(struct DotrMiscPacket128 *packet, int a, int b);
extern void lRhModelCpu_SortPolygon(struct DotrMiscPacket128 *packet, struct DotrMiscPacket128 *end);
void *RhModelCpu_DrawAlpha(struct DotrMiscPacketHead *head, int a, int b, int flag);

/* Ring accounting in the three words after the 0x50000-byte area:
 * +0x50000 write offset, +0x50004 used bytes, +0x50008 capacity. */
int readBufBeginPut(struct DotrMiscLarge *p, char **out);
int readBufEndPut(struct DotrMiscLarge *p, int count);
int readBufBeginGet(struct DotrMiscLarge *p, char **out);
int readBufEndGet(struct DotrMiscLarge *p, int count);

/* Five-word queue header: two base addresses, an index, a count and a
 * capacity. Element strides are 0x195000 and 0x4ce40 bytes. */
struct DotrMiscQueue {
    DotrMiscAddress unknown_00;
    DotrMiscAddress unknown_04;
    int unknown_08;
    int unknown_0c;
    int unknown_10;
};

void voBufCreate(struct DotrMiscQueue *queue, DotrMiscAddress a, DotrMiscAddress b, int capacity);
DotrMiscAddress voBufGetData(struct DotrMiscQueue *queue);
DotrMiscAddress voBufGetTag(struct DotrMiscQueue *queue);
int Ta_GetEOwnerSide(int side, int mode);

/* Float table view used by the signed 16-bit phase interpolation routine;
 * its original declaration and complete object boundary are unknown. */
extern float data_00294050[];
float RhMath_CosI(int phase);
float RhMath_SinI(int phase);
/* 0x20b730: the angle in radians is scaled to 65536 units per turn, rounded by adding 0.5 and
 * converted by 0x1a22c0 (library block; assumed float to int). The name comes
 * from the Japanese build's symbol table; the parameter name is assumed. */
extern int func_001a22c0(float value);
float RhMath_Sin(float rad);
/* RhMath_Cos (0x20b620): RhMath_Sin with the phase advanced by a quarter turn (16384)
 * before the conversion. Compiled warm, so the conversion is written as a cast. */
float RhMath_Cos(float rad);
/* Weighted mix of two 16-bit angles along the shorter arc; int parameters assumed (both are
 * masked to 16 bits). Compiled warm, so the float-to-int conversion is written as a cast. */
int RhGeom_RotationInterpolate(float first_rate, float second_rate, int first, int second);

/* Pointer pairs addressed by the table at EE 0x29d488: eight-byte rows of
 * four halfwords and of eight bytes. */
struct DotrMiscRows {
    unsigned char *unknown_00;
    unsigned char *unknown_04;
};

struct DotrMiscHalfRow {
    short unknown_values[4];
};

struct DotrMiscByteRow {
    signed char unknown_values[8];
};

extern struct DotrMiscRows *gpShFaceIndex[];

/* The set is an unsigned char parameter: ShFace_DrawOne loads it after the row argument,
 * which the pinned compiler does only for a narrow parameter type. */
void ShAnime_GetPartsRect(unsigned char set, int row, struct DotrMiscHalfRow *out);
void ShAnime_GetPatInfo(int set, int row, struct DotrMiscByteRow *out);
unsigned ConvUnitIDtoFlagList(unsigned flagList, int teamId, int unitArrayPos);
int Ta_GetAgreeOwnerIDon22bitFlagLst(int ownerId, unsigned flagList);

/* Prefix view of the object read by 0x250370 (the 92-byte unit record). */
struct DotrMiscFlagObject {
    unsigned char unknown_00[0x44];
    unsigned char team;
    unsigned char unknown_45[0x12];
    struct DotrMiscFlagBits {
        signed char unknown_bit_0 : 1;
        signed char unknown_bit_1 : 1;
        signed char unknown_bits_2_7 : 6;
    } status;
};

int Ta_IsUnitControl2(struct DotrMiscFlagObject *unit);
int SzData_GetConstDeck(int deck, int slot);
extern unsigned char gSzData_bRoseFlag[];
extern signed char data_00299eda[];
extern short winCount[];
extern int Ta_GetCurrentSide(void);
extern int TaView_ReqFocusCamera(int,int,int,int);
extern int TaHand_GetCardSummonCost2(int);
extern int ExSortNode_Pipe(int a, void *b, int c, int d, int e, int f);

int SzDuel_FinishWindowSession_Pipe(int a);
void RhZSort_Start(int vifPacketArea, int b);
float func(float a, float b, float c, float d, float e);
void Taku_Fune_init(int a, int b);
void Swap(int *array, int indexA, int indexB);
/* Sorts words[low..high] by descending SzDuel_GetUnitSummonOrder(word >> 8, word & 0xff); int
 * words assumed. */
void SortByOrder(int *ownerIds, int low, int high);
/* Collects positions 1..10 of both sides passing 0x24fda0 == 1 and 0x1e22b0 == 9; four-word
 * output assumed. */
int Ta_IsVirusAll(int *out);
int Ta_IsVirus_Chk(int team, int unitArrayPos);
int Ta_Do_HandDestroyEfc(struct DotrMiscTag *p, int a, int b);
int Ta_GetLDRange(int side);
void TaLogic_CopyArray(unsigned char *source, unsigned char *destination);

/* Dependencies. SzMsg_Free (0x1feaf0) is defined in slot_table. SzDuel_GetUnit (0x1df540) is
 * defined in state_records, where it returns an item pointer; this caller
 * forwards the result as a word, so a separately named word-typed symbol is
 * used for the same address. */
extern void SzMsg_Free(int unknown);
extern int func_001df540_word(int a, int b);
extern int Ta_IsVirus_Chk2(int unit);

int SzDuel_GetWindowTexture(int index);
void SzDuel_Term3DView(void);
int SzDuel_FreeCardRequest(struct DotrMiscNode *p);
void SzDuel_InitCardRequest(void);
void SzDuel_InitCardControl(void);
int SzDuel_ClearCardControl(struct DotrMiscSlot160 *p);
struct DotrMiscEntry160 *SzDuel_GetCardControl(int index);
void SzBattle_Term(void);
void SzView_TermCamera(void);
int SzFade_Destroy(int index);
void SzFade_DispAll(void);
int RhBase_Term(void);
struct DotrMiscLargeEntry *SzData_GetData(int side);
void Taku_Coord_Init(struct DotrMiscTriple *m);
int audioDecDelete(struct DotrMiscObject *p);
int videoDecDelete(struct DotrMiscObject *p);
void ShFace_Init(int value);
int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
int TaLogic_IsUnitAssign(unsigned short ownerId, struct DotrMiscEntry16 *p);
int TaLogic_GetActionUnitFromOwnerID(struct DotrMiscEntry16 *actionUnits, int ownerId);
int TaLogic_GetUnitNum(int side, int countSpells);
int TaLogic_IsUseHandInCombo(unsigned char handSlot, unsigned char *combo);
void Ta_LkInitCoordinate(struct DotrMiscTriple *m);
void Ta_LkAddVector(struct DotrMiscVector3 *a, struct DotrMiscVector3 *b);
void Ta_LkSubVector(struct DotrMiscVector3 *a, struct DotrMiscVector3 *b);

/* Dependencies. SzDuel_GetUnitMonsterCount and SzDuel_GetUnitSpellCount are defined in
 * state_grid; RhView_Term in view_state; RhMemCard_Term in small_state; SzMem_Free (0x217b50)
 * in resource_support (declared here under a word-typed name). SzView_SetPointLight
 * (0x1f9940) is defined in block_table with other parameter and return types and is declared
 * here, under its own name, as void with three ints, as index_fanout.h also declares it. */
extern void SzModel_Preset(int modelId, int modelType, int modelNumber);
extern void func_001f9940_three(int a, int b, int c);
extern void SzDuel_ClearDrawCard(struct DotrMiscNode *p);
extern void SzBattle_Return(int a, int b);
extern void func_00217b50_word(int value);
extern int SzFade_Disp(int index);
extern int RhView_Term(void);
extern int RhMemCard_Term(void);
extern void func_00182a28(int value);
extern void changeMasterVolume(int value);
extern void viBufDelete(int *p);
extern void func_0018ed10(struct DotrMiscObject *p);
extern int SzDuel_GetUnitSpellCount(int side);
extern int SzDuel_GetUnitMonsterCount(int side);

/* Prefix view of the object read by 0x202e30: two selector halfwords. */
/* Extended for 0x202fb0: the image offset and size words, a 64-bit word laid out like the GS
 * TEX0 register (only tbp0 and cbp are written) and the packet count. Member names other
 * than the two selectors are descriptive guesses. */
struct DotrMiscFormat {
    int offset;
    int size;
    unsigned char unknown_08[0x18];
    unsigned long tbp0 : 14;
    unsigned long unknown_tex0_14 : 23;
    unsigned long cbp : 14;
    unsigned long unknown_tex0_51 : 13;
    unsigned char unknown_28[0x18];
    unsigned short unknown_40;
    unsigned short unknown_42;
    int unknown_44;
    int count;
};

int SzText_GetClutSize(struct DotrMiscFormat *imageHeader);

/* 48-byte transfer packet written by 0x202fb0; the 64-bit word at 0x20 is laid out like the
 * GS BITBLTBUF register and only its destination base (bits 32-45) is written. */
struct DotrMiscTextPacket {
    unsigned char unknown_00[0x20];
    unsigned long unknown_sbp : 32;
    unsigned long dbp : 14;
    unsigned long unknown_46 : 18;
    unsigned char unknown_28[8];
};
/* Defined in memory_ops with DotrWord128 pointers; void pointers here, as in draw_state.h. */
extern void *RhMem_CopyMemory16(void *destination, void *source, int count);
/* 0x202fb0: copies the packets of texture file `tex` (header at +16) to `dst`, gives the
 * file's palette a slot from the gSzText_tl allocator, restarting the
 * allocator at `gs` when it is full, and writes the texture and palette addresses into the
 * header word and each copied packet (32 blocks apart). `*used`, when given, receives the
 * blocks consumed. Returns the copy routine's result, or `dst` when `tex` is null.
 * Pointer types and member names assumed. */
void *SzText_Load_Pipe(void *dst, char *tex, int gs, int *used);
int Ta_CalcCtrlRate(unsigned short value, int amount, int direction);
int GetCap(int limit, int value);
int GetParCap(int stat);
int TaUtil_IsVarInfoLeader(struct DotrMiscFlagObject *unit);
int TaLogic_IsStatusMatch(int actualStatus, int targetStatus);

/* 0x1e2100 is defined in state_grid and 0x26fbb0 is declared by
 * state_queries; both declarations are repeated identically here. */
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int TaHand_GetCardAPDP(int id);

/* Four-byte entry scanned by 0x277c60; 22 entries, ended early by 0xffff. */
struct DotrMiscLevelEntry {
    unsigned short unknown_00;
    unsigned char unknown_02;
    unsigned char unknown_03;
};

/* 24-byte records addressed from the literal EE address 0x1ed0000 by
 * 0x2788d0; their contents are unknown and are not accessed. */
struct DotrMiscRecord24 {
    unsigned char unknown_00[24];
};
#define DOTR_MISC_RECORDS_01ED0000 ((struct DotrMiscRecord24 *)0x1ed0000)

/* Two halfwords and a word compared by 0x279170. */
struct DotrMiscRule {
    unsigned short unknown_00;
    unsigned short unknown_02;
    unsigned unknown_04;
};
/* 32 stack bytes passed to 0x26fd30 by 0x278c80; the size is the frame gap. */
struct DotrMiscScratch32 {
    int unknown_00[8];
};
/* 0x1e13c0 and 0x1e22b0 are defined in state_grid, whose header cannot be
 * included with this one; the declarations are repeated identically. */
extern int SzDuel_GetUnitSummonOrder(int side, int pos);
extern int SzDuel_GetField(int x, int y);
/* Dependency prototypes assumed from the call sites; argument meanings are unknown. */
extern void TaHand_GetNextHand(int a, struct DotrMiscScratch32 *out);
extern void TaLogic_InitTree(struct DotrMiscRecord24 *record, unsigned short d, int e, int handIndex);
int Ta_GetLocDistanceFromCenterOnRangeXY(int packed, int step, int mode);
int Ta_GetMinimumOrderUnitFromFlagList(int flagList);
int Ta_GetMaximumOrderUnitFromFlagList(int word);
int TaLogic_IsKeepEnchant(struct DotrMiscRule *rule, int cardId, int fusionResult);
/* Three words; 0x24e880 rebuilds the second and third. */
struct DotrMiscBitWords {
    int unknown_00;
    int unknown_04;
    int unknown_08;
};
/* Four words written by 0x24fd10. */
struct DotrMiscWords4 {
    int unknown_00[4];
};
/* Dependency prototypes assumed from the call sites; argument meanings are unknown. */
extern int Ta_GetHandEnchant(int cardNo, int equipNo);
extern int TaEnc_IsEncDo(int a);
extern int Fusion(int cardA, int cardB);
extern int TaUtil_DoOpen(int side1, int pos2, int c);
extern int TaLogic_GetRoot2(int item, int c, int d, int e, int f, int g, int h, unsigned char i);
/* g is a pointer: 0x27b520 passes the address of a stack halfword through 0x279850. */
extern int TaLogic_IsPassRoot2(int item, int c, int d, int e, int f, unsigned short *g);
extern int TaLogic_IsRunPassRoot2(int item, int c, int d, int e, int f, int g, int h);
void Ta_SetEffectObjByField(struct DotrMiscBitWords *out, int *source);
int Ta_IsVirus(int team, int unitArrayPos, struct DotrMiscWords4 *out);
extern int Ta_Do_MoveEfc(int a, int b, int c, int d, struct DotrMiscWords4 *out);
int Ta_Do_CompMove(int team, int unitArrayPos, int col, int row, struct DotrMiscWords4 *out);
struct DotrMiscRecord24 *TaLogic_GetFusionTree2(int a, int unused, int c, int d, int e,
                                       unsigned short *used, unsigned short *count);
int TaLogic_GetHandFusion(unsigned short cardNo, unsigned short equipNo);
int TaLogic_GetRoot(int a, int b, int c, int d, int e, int f, int g, int h, unsigned char i);
/* 0x2792c0 viewed with the second argument as a pointer to 12 nibble pairs
 * and the seventh as a halfword; the existing TaLogic_GetRoot2 takes ints. */
extern int func_002792c0_nibbles(int item, struct DotrMiscNibbles *bytes, int c, int d, int e, int f,
                                 unsigned short g, unsigned char h);
/* Arguments are assumed from the callers only; 0x27b290 is the one callee. */
int TaLogic_IsRootOpen(int side, int pos, int c, int d, int e, int f, int g, int h, unsigned char maxDistance);
int TaLogic_IsRootOpen2(int unit, int unitCol, int unitRow, int enemyUnitSide, int enemyUnitArrayPos, int enemyUnitCol,
                        int enemyUnitRow, unsigned char maxDistance);
int TaLogic_IsPassRoot(int side, int pos, int col, int row, int destCol, int destRow, unsigned short *atkTargetPtr);
int TaLogic_IsRunPassRoot(int a, int b, int c, int d, int e, int f, int g, int h);
/* Library routine; its result is reduced modulo a count by 0x24ebd0. */
extern int func_0019a5c8(void);
int Ta_GetRandomUnit(int flagList);
int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
void TaLogic_AddEmptyCombo(unsigned char *combo, int newCard);
int TaLogic_IsAdjustUnitLst(struct DotrMiscLevelEntry *entries, int unitOwnerId, int level);
int TaLogic_IsJudgeUnitDanger(int side, int pos, int canSeeFaceDown);
/* Assumed prototype; receives the word-typed result of 0x1df540. */
extern int TaLogic_IsUnitWin2(int sourceUnit, int b, int col, int row);
int TaLogic_IsUnitWin(int unit1OwnerId, int unit2, int col, int row);
struct DotrMiscRecord24 *TaLogic_TreeAloc(unsigned short *allocedBytes, unsigned short *allocedBlocks);
unsigned TaLogic_GetEnchantQue2(int unused, unsigned short *id);

/* TaLogic_IsUnitWin3 (0x278420) is defined in state_queries with an item-pointer first
 * argument; TaLogic_IsUnitWin4 (0x278510) forwards it as a word. */
extern int TaLogic_IsUnitWin3(int unit, int base, int x, int y);
int TaLogic_IsUnitWin4(int unit1, int ownerId, int warnHideLvl, int col, int row, unsigned char canSeeFaceDown);
/* Entry at +8 of the two items compared by 0x2787f0, and the byte at +0x3e. */
struct DotrMiscPairEntry {
    unsigned type;
    unsigned short data;
};
struct DotrMiscPairItem {
    unsigned char unknown_00[8];
    struct DotrMiscPairEntry cardEffects;
    unsigned char unknown_10[0x2e];
    unsigned char kind;
};
int TaLogic_GetActOfs(struct DotrMiscPairItem *unit1, struct DotrMiscPairItem *unit2);
/* It reads at least four stack arguments. The fifteen arguments are what
 * 0x2770e0 passes; the prototype is assumed from that call. */
extern int TaLogic_GetNearUnitPickUpTree(int a, int zero, int b, int c, int d, int e, int f, int g,
                         int h, int zero2, int k, int l, int i, int j, int m);
int TaLogic_GetNearUnitL2(int routeTree, int unit, int col, int row, int side, int currentDistance, int canSeeFaceDown,
                          unsigned short warnHideLvl, int ownerIdArray, int numberOfValidTargets, int allocedBytes,
                          int allocedBlocks, int ignoreList);
/* Byte-wide bit view of the 12-byte entry returned by 0x2008a0, as accessed
 * by SzText_LoadRequest (entry_table.h and DotrMiscScanEntry use halfword fields). */
struct DotrMiscByteFlagEntry {
    int unknown_00;
    int unknown_04;
    struct DotrMiscByteFlagBits {
        signed char unknown_bit_0 : 1;
        signed char unknown_bit_1 : 1;
        signed char unknown_bits_2_7 : 6;
    } unknown_08;
    unsigned char unknown_09[3];
};
extern struct DotrMiscByteFlagEntry *func_002008a0_bytes(int group, int id);
int SzText_LoadRequest(int group, int id);
int Ta_SideMatch(int sideA, int sideB, int compMode);
/* Ta_GetCurrentSide (0x24de70) with its two arguments, defined in state_queries; the
 * `Ta_GetCurrentSide(void)` declaration above serves the tail thunk Ta_IsUnitControl
 * (0x250360). Ta_GetAPPlusDP (0x24f2f0) is defined in state_queries. */
extern int func_0024de70_pair(int a, int b);
extern int Ta_GetAPPlusDP(int side, int pos);
int Ta_GetMinimumOrderUnitFromFlagList2(int word, int side);
int Ta_GetMinimumParamUnitFromFlagList(int flagList);
int Ta_GetExzodiaPartsCardNo(int limbId);
/* Halfword table indexed like the kind bytes at 0x29d1f0. */
extern unsigned short gAnmPat_Bakura[];
int TaEnc_GetEncScore(int cardID);
/* Record written by 0x276ec0; only the named offsets are observed. */
struct DotrMiscTaggedPair {
    unsigned short unitOwnerId;
    unsigned char taskType;
    unsigned char unknown_03;
    unsigned short loc;
    unsigned char unknown_06[10];
    unsigned char atkDefPosition; /* 0 attack, 1 defence, 2 no change */
};
/* SzDuel_GetUnitLoc (0x1e1090) is defined in state_grid; the declaration is repeated. */
extern int SzDuel_GetUnitLoc(int side, int pos);
void TaLogic_GetMinimumTask_DoNone(unsigned short *unit, int unused, struct DotrMiscTaggedPair *task);
/* Assumed prototype: 0x27ac70 sets eight argument registers. */
extern int TaLogic_DoGetShortRoot(int a, int *word, unsigned char *byte, int b, int c, int d, int e, int f);
int TaLogic_GetShortRoot(int pathfindingBuffer, int tree, int col, int row);
/* 56-byte records addressed from the same literal as the 24-byte records. */
struct DotrMiscRecord56 {
    unsigned char unknown_00[56];
};
#define DOTR_MISC_RECORDS56_01ED0000 ((struct DotrMiscRecord56 *)0x1ed0000)
struct DotrMiscRecord56 *TaLogic_RootTreeAloc(unsigned short *allocedBytes, unsigned short *allocedBlocks);
int LkCheckData(unsigned short *words);
int TaLogic_GetUnitCtlLstLen(unsigned short *units);
/* Words read by 0x244ad0 and 0x244b20; 0x17b900 and 0x17b920 are library
 * routines bracketing the access. */
struct DotrMiscSpan {
    unsigned char unknown_00[0x10];
    int unknown_10;
    int unknown_14;
    unsigned char unknown_18[0x28];
    int unknown_40;
};
extern void func_0017b900(int unknown);
extern void func_0017b920(int unknown);
int viBufCount(struct DotrMiscSpan *span);
void viBufFlush(struct DotrMiscSpan *span);
/* Dependency prototypes of four small callers, assumed from the call sites. */
extern void func_00190810(int a, int b, int c, int d);
extern int Ta_IsActiveTrapB(int side, int trapEffect);
void changeInputVolume(int value);
/* 0x190810 called with seven registers (probably variadic; unverified). */
extern void func_00190810_args7(int a, int b, int c, int d, int e, int f, int g);
void audioDecResume(struct DotrMiscStream *p);
void audioDecStart(struct DotrMiscStream *p);
int Ta_DoUncTrapB(int *out);
/* 0x245160: the object word at +0x48 and the 24-byte stack record. The two
 * doubleword members are stored from the full argument registers. */
struct DotrMiscOrigin {
    unsigned char unknown_00[0x48];
    int unknown_48;
};
struct DotrMiscRequest {
    long unknown_00;
    long unknown_08;
    int unknown_10;
    int unknown_14;
};
extern void viBufPutTs(int *queue, struct DotrMiscRequest *request);
void videoDecPutTs(struct DotrMiscOrigin *object, long a, long b, int c, int d);
/* 0x27f110: 16-byte members at +0x10 and +0x20 and a 64-byte member at
 * +0x30; float elements are assumed. 0x20bc00 is a VU0 macro routine. */
struct DotrMiscMatrix {
    float unknown_00[16];
};
struct DotrMiscFrame {
    float unknown_00[4];
    float unknown_10[4];
    float unknown_20[4];
    struct DotrMiscMatrix unknown_30;
};
extern void RhView_GetWorldScreenMatrix(struct DotrMiscMatrix *m);
extern void RhGeom_GetScaleRotXYZTransMatrix(struct DotrMiscMatrix *out, struct DotrMiscFrame *frame, float *a, float *b);
extern void RhGeom_MulMatrix(struct DotrMiscMatrix *m, struct DotrMiscMatrix *other);
void Ta_LkSetCoordinate(struct DotrMiscFrame *frame);
/* 0x1038d0: same body and callees as Ta_LkSetCoordinate, with the same frame view. */
void LkSetCoordinate(struct DotrMiscFrame *frame);
void Taku_Coord_Set(struct DotrMiscFrame *frame);

/* View of the control storage at 0x317490: words at +0x6d8/+0x6dc, byte at +0xd5c. */
struct DotrMiscControlTail {
    unsigned char unknown_000[0x6d8];
    int unknown_6d8;
    int unknown_6dc;
    unsigned char unknown_6e0[0x67c];
    unsigned char unknown_d5c;
};
extern struct DotrMiscControlTail data_00317490_tail;
/* Only its address is used, as a callback like lSzDuel_SortFunc1 (assumed int (void)). */
extern int lSzDuel_SortFunc2(void);
void lSzDuel_SortTrade(int flag);

/* Word-aligned view of a 65440-byte block, so the copy is done in words. */
struct DotrMiscLargeWords {
    int unknown_words[16360];
};
/* Only the first byte is used (read signed by 0x23b920, set by 0x23bc80); declared as an
 * array because the original object's type and extent are unknown. */
extern unsigned char HELP_flg[];
extern void SzGame_Push(int arg0, int arg1);
void Taku_Camp_Go_Help(void);

/* 7x7 cell scans over SzDuel_GetMapUnitID (state_grid.h has the same shape). */
extern int SzDuel_GetMapUnitID(int x, int y);
int Ta_SearchMoveTemp(void);
int Ta_GetStackMapPiece(void);

/* Rebase header: word before it, address word +0, halfword kind at +8 (placeholder names). */
typedef struct DotrMiscFixupHead {
    DotrMiscAddress data;
    DotrMiscAddress unknown_04;
    unsigned short kind;
} DotrMiscFixupHead;
int lNjaReloc_AdjustMotion(DotrMiscFixupHead *p, int delta, int count, int flag);

/* 7x7 scan; the setter is defined in state_grid, whose header cannot be
 * included with this one, so the declaration is repeated identically. */
extern int SzDuel_SetField(int x, int y, int value);
void Ta_InitMazePiece(void);

/* Byte-list view: count at +0xd4, entries from +6 (assumed from the loop). */
typedef struct DotrMiscByteList {
    unsigned char unknown_00[6];
    unsigned char entries[0xce];
    unsigned char count;
} DotrMiscByteList;
extern int SzEffect_IsUse(int index);
int Ta_IsFinishEffect(DotrMiscByteList *list);

/* Eight fixed (index, value) calls; the return type is an assumption (void). */
extern int SzDuel_AddGrave(int side, int cardNo);
void TaTuto_Init036(void);

/* Copies four floats from each of two callees; func_001faf60_floats is a float
 * view of SzView_GetLook (struct DotrBlock * in block_table.h). */
extern void *SzView_GetCP(int index);
extern float *func_001faf60_floats(int index);
void TaTuto_BkupOriginalView(float *out);

/* Dependency prototypes assumed from the call sites (the first three are library routines). */
extern void func_0017b790(int unknown);
extern void func_00199b50(char *text, int value);
extern void func_0018e200(void);
extern int viBufBeginPut(void *p);
extern int viBufEndPut(void *p);
extern void viBufAddDMA(int *p);
extern void viBufStopDMA(int *p);
extern void viBufRestartDMA(int *p);

/* The fade level is a halfword at a fixed address
 * (not host-addressable); the ring view covers the fields used by 0x243750. */
#define DOTR_MISC_FADE_LEVEL (*(short *)0x1ed547c)
struct DotrMiscRing { unsigned char unknown_00[0x44]; int unknown_44; int unknown_48; int unknown_4c; };
extern unsigned int FusionDat[];
int lSzDuel_GetFontIndex(char character);
int My_LedarModel_a_set(int dir);
void iopGetArea(int *a, int *b, int *c, int *d, struct DotrMiscRing *ring, int position);
int Fusion(int cardA, int cardB);
int Ta_IsLD_CurseObj(int side, int objKind);
int TaLogic_IsPickUp(int ownerId);
extern int SzDuel_GetUnitConstKind(int side, int pos);
extern int SzDuel_GetUnitKind(int side, int pos);

/* Rebase header layout assumed from the accesses of NjaReloc_Adjust_M3DMOT (field names are
 * placeholders): magic 0x1a544f4d at +0, count at +0x10, flag bit 7 of the byte at +0x13,
 * offsets at +0x14/+0x18. */
typedef struct DotrMiscRebaseHead {
    int magic;
    unsigned char unknown_04[12];
    unsigned short count;
    unsigned char unknown_12;
    unsigned char unknown_13 : 7;
    unsigned char flags : 1;
    DotrMiscAddress first;
    DotrMiscAddress second;
} DotrMiscRebaseHead;
int NjaReloc_Adjust_M3DMOT(DotrMiscRebaseHead *head);

/* Job request and queue views (placeholder names), fields assumed from 0x25ed20 only. */
typedef struct DotrMiscJob {
    int unknown_00;
    int first;
    int second;
    int value;
} DotrMiscJob;
typedef struct DotrMiscJobQueue {
    unsigned char unknown_00[4];
    unsigned short count;
    unsigned char slot;
} DotrMiscJobQueue;
/* Callee prototypes assumed from the call sites; SzDuel_CalcSummonPower and
 * SzDuel_CloseMsgPopup are defined in state_records / state_grid, whose headers cannot
 * be included here, so their declarations are repeated identically. */
extern int SzDuel_CalcSummonPower(int side, int operation, int amount);
extern void SzDuel_CloseMsgPopup(void);
extern void InitEffectTemp(DotrMiscJobQueue *q);
extern void TaEffect_LoadModule(DotrMiscJobQueue *q, int id);
extern int TaEffect_IsLoadModule(DotrMiscJobQueue *q);
extern int Ta_Do_Effect(DotrMiscJob *req, DotrMiscJobQueue *q);
int Ta_Eff_034(DotrMiscJob *in, DotrMiscJobQueue *q, int mode);

/* Averages count 16-byte vectors into out (0 when count is 0). */
int Ex3d_GetPointsAverage(int count, struct DotrMiscVector4 *items, struct DotrMiscVector4 *out);

/* 0x255c30 views a job object as 24 ushort ids at +0x82;
 * SzEffect_GetModuleInfo (pool_table) is redeclared here as returning a byte whose low three
 * bits are a state. Do not include pool_table.h with this header. */
struct DotrJobPool {
    unsigned char state : 3;
    unsigned char rest : 5;
};
struct DotrJobIds {
    unsigned char unknown_00[0x82];
    unsigned short ids[24];
};
extern struct DotrJobPool *SzEffect_GetModuleInfo(int a, int b);

/* 0x2761d0 views 22 sixteen-byte slots; the first 16 bytes move as four floats. */
struct DotrSlotF4 {
    float a;
    float b;
    float c;
    float d;
};
struct DotrSlot16 {
    unsigned short id;
    unsigned short tag;
    unsigned short aux;
    unsigned short pad06;
    int word_08;
    unsigned char flag_lo : 2;
    unsigned char flag_rest : 6;
    unsigned char pad0d[3];
};

int TaEffect_IsLoadModule2(struct DotrJobIds *job, int id);
void TaLogic_DoSortTaskList(struct DotrSlot16 *taskList);

/* Segment/plane intersection (float views, plane = nx,ny,nz,d) and a grid step. */
void Ex3d_IntersectP(float *plane, float *p1, float *p2, float *out);
int Ta_GetUnitGravityLocSearchOnZ(int unused, int target, int y, int x);

/* Ta_GetUnitNum counts nonzero func_001df540_word(side, i) results over a kind-selected range. */
int Ta_GetUnitNum(int side, unsigned char kind);
/* Flag word read at +8 by Ta_IsFinishDestEfc; bits 21..0 map to Ta_DelUnit(i / 11, i % 11) calls
 * (assumed signature, same as state_queries.h; do not include both headers). */
struct DotrMiscFlagWord {
    int unknown_00;
    int unknown_04;
    unsigned unknown_08;
};
extern int Ta_DelUnit(int side, int pos);
int Ta_IsFinishDestEfc(struct DotrMiscFlagWord *flags, DotrMiscByteList *list);
int Ta_IsLD_DestObj(int side, int objKind);
/* dst moves by (src - dst) / n through the vector add/subtract helpers; n == 0 copies src. */
void Ta_LkOBJ_TransitVector(struct DotrMiscVector3 *dst, struct DotrMiscVector3 *src, int n);

/* Writes (kind << 8 | i) then (partner << 8 | i) halfwords to out for i in 1..10 whose word is nonzero
 * and Ta_SideMatch(kind, pair, mode) == 1; partner is 1 - kind for kind 0/1. Up to 20 entries. */
void TaUtil_GetUnitLst(int sideToCheck, int compMode, unsigned short *units);

/* Fills the 16-float matrix m from three int phases at ang[0..2]; elements 3, 7, 11, 12-14 are
 * 0, 15 is 1. */
void RhGeom_GetRotXYZMatrix(float *m, int *ang);

/* 0x250360 is a tail thunk to 0x24de70 that forwards a0/a1; this view symbol keeps the two
 * arguments. TaUtil_GetUnitAP is defined in state_queries. */
extern int func_00250360_pair(int a, int b);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);

/* words[0] is a count (TaLogic_GetUnitCtlLstLen); entries words[1..] are (hi << 8 | lo)
 * halfwords. Among entries accepted by SzDuel_GetUnitStatus(hi, lo, 4) the one with the
 * strictly greatest TaUtil_GetUnitAP score wins (first on ties); returns that halfword, or
 * -1 when the count is below 2 or nothing is accepted. */
int TaLogic_GetMaxApUnit(unsigned short *ownerIdArray);
/* Same scan; an entry is also accepted when key == func_00250360_pair(hi, lo). */
int TaLogic_GetMaxApUnit2(unsigned short *ownerIdArray, int team);
/* Same scan; the key alternative also needs SzDuel_GetUnitCurseRest(hi, lo) == 0 and
 * SzDuel_GetUnitSpellRest(hi, lo) == 0 (both defined in state_grid; two-int views assumed). */
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
int TaLogic_GetMaxApUnit3(unsigned short *words, int key);

/* Second view of the eight 52-byte entries at 0x350fd0 (struct DotrMiscEntry52 is the
 * first): fields written by 0x245be0. The request layout is assumed from its accesses. */
struct DotrMiscSlot52 {
    int active;
    unsigned char unknown_04;
    unsigned char key;
    unsigned char unknown_06;
    unsigned char unknown_07[4];
    unsigned char unknown_0b[16];
    unsigned char unknown_1b[16];
    unsigned char unknown_2b;
    unsigned short unknown_2c;
    unsigned short unknown_2e;
    int unknown_30;
};
struct DotrMiscSlotRequest {
    unsigned char id; /* 0xff asks for the first inactive entry */
    unsigned char unknown_01;
    unsigned char key;
    unsigned char unknown_03;
    unsigned short unknown_04;
    unsigned short unknown_06;
};
#ifdef __MWERKS__
extern struct DotrMiscSlot52 gSh_Face_slots[];
#else
#define gSh_Face_slots ((struct DotrMiscSlot52 *)gSh_Face)
#endif
int ShFace_Request(struct DotrMiscSlotRequest *req);

/* Declared locally; the functions are defined in state_queries and state_grid. */
extern int Ta_SearchUncTypeNaturalEffectUserNumWithSide(int kind, int mode);
extern int SzDuel_GetUnitLeaderAbility(int side, int pos, int abil, int extraDataOut);

/* Returns 0 when Ta_SearchUncTypeNaturalEffectUserNumWithSide(88, side == 0) is positive
 * (88: leader abilities denied); otherwise forwards to
 * SzDuel_GetUnitLeaderAbility(side, 0, ability, 0). */
int Ta_IsLDAbl(int ability, int side);

/* c packs a unit as (high << 8 | low) for SzDuel_GetUnitLoc, whose result gives the cell's row
 * (high byte) and y (low byte). When a differs from y, the neighbour x = y - 1 (a < y) or y + 1
 * (a > y) in that row is probed with SzDuel_GetMapUnitID; if it is free (negative) and d == 0
 * it is returned packed as (row << 8) | x. Every other case returns
 * Ta_GetUnitGravityLocSearchOnZ(a, b, y, row). */
int Ta_GetUnitGravityLoc(int a, int b, int c, int d);
/* Chooses a candidate from Ta_GetUnitGravityLoc (variant 0, else variant 1 when variant 0
 * is already among the 22 words of table); returns -1 when no usable candidate exists. */
int Ta_RequestGravityReserve(int a, int b, int c, int *table);

/* Callee prototypes assumed from the argument registers (0 is the first argument). */
extern void SzView_GetLookVector(int a, int b);
extern void SzView_GetLookMatrix(int a, int b);
extern void SzView_GetLookMatrixWithRoll(int a, int b);
extern void RhGeom_GetRotVectorMatrix(float *m, float *axis, int phase);

/* Selects float block 0 through func_001faf60_floats, calls SzView_GetLookVector(0, arg),
 * then returns the first float. */
float Ex3d_GetLookVector(int arg);
/* Pure forwarders: SzView_GetLookMatrix(0, arg) and SzView_GetLookMatrixWithRoll(0, arg). */
void Ex3d_GetLookMatrix(int arg);
void Ex3d_GetLookMatrixWithRoll(int arg);
/* Build a rotation in a local matrix (RhGeom_GetRotXYZMatrix from ang,
 * RhGeom_GetRotVectorMatrix from axis/phase), then hand it to RhGeom_MulMatrix(out, &local). */
void RhGeom_RotXYZMatrix(struct DotrMiscMatrix *out, int *ang);
void RhGeom_RotVectorMatrix(struct DotrMiscMatrix *out, float *axis, int phase);
/* Per-entry routine RhGeom_MulMatrixRev (0x20bcf0, VU0 assembly in asm/ee/vu0); the argument
 * meaning is unknown. */
extern void RhGeom_MulMatrixRev(void *entry, int arg);
/* Calls RhGeom_MulMatrixRev(base + i * 112, arg) for i in [0, count). */
void lRhModel_ChangeL2VtoL2S(unsigned char *base, int arg, int count);
/* Callee at 0x1b3cc0 reads six stack arguments; all fourteen are passed as
 * words here and its return value is not used. */
extern void lSzDuel_DrawWindow_ExIcon_Pipe(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k,
                          int l, int m, int n);
/* Argument types assumed int: none is read except the three stack words. */
void lSzDuel_DrawWindow_Icon_Pipe(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k);
/* The elements of gSzDuel_aCardRequest are viewed as DotrMiscNode (word at +0x90). */
struct DotrMiscNode *SzDuel_GetCardRequest(void);
/* Only the three read words are named; the rest of the object is unknown. */
struct DotrMiscClose {
    int unknown_00;
    unsigned char unknown_04[0x28];
    int unknown_2c;
    int unknown_30;
};
/* No arguments set by the caller here; return value unused. */
extern void func_00192058(void);
/* Same declaration as resource_process.h. */
extern int func_0017fd20(int descriptor);
int strFileClose(struct DotrMiscClose *p);
/* One argument (0 or 1) and a tested int result, from this caller alone. */
extern int lSzDuel_ActCustomDuelProc(int side);
/* Byte at 0xa9c088, declared as an array; the original object's extent is unknown. */
extern unsigned char customDuelOpponentId[16];
void SzDuel_ActCustomDuel(void);
/* Byte at +0 and link at +0x34 are the only members read. */
struct DotrMiscChain {
    unsigned char loc; /* (row << 4) | col */
    unsigned char unknown_01[0x33];
    struct DotrMiscChain *parent;
};
/* depth is masked to a byte on entry; the result is not masked. */
int TaLogic_GetNextPiece(struct DotrMiscChain *tree, unsigned char depth);
/* Four int cell coordinates and a byte depth, read from the body alone (not
 * from its callers); the result is 0 or 1. */
int TaLogic_IsRootPast2(struct DotrMiscChain *tree, int col, int row, int col2, int row2, unsigned char remainingSteps);
/* Halfword at +0 packs a cell (row << 8 | column). +8 is read as a halfword or,
 * for mode 1, as the upper half of the word there. GetMoveRev (0x2464f0,
 * perm_efc) is viewed with three int arguments; the result is unused. */
struct DotrMiscCellPair {
    unsigned short unknown_00;
    unsigned short unknown_02[3];
    union {
        unsigned short half;
        unsigned int word;
    } unknown_08;
};
extern int func_002464f0_rule(int item, int x, int y);
int TaLogic_IsAtkNowToGoal(struct DotrMiscCellPair *assignedUnit, unsigned char useSecondGoal);
/* Three int arguments and an int result, from Match_AtkEfcObj alone. */
extern int Do_Match_AtkEfcObj(int a, int b, int c);
/* Six register arguments, all assumed int. */
int Match_AtkEfcObj(int compMode, int searchType, int b, int c, int d, int e);
/* Only the stored members are named. */
struct DotrMiscRect {
    unsigned char unknown_00[0x10];
    float unknown_10;
    float unknown_14;
    int unknown_18;
    int unknown_1c;
    float unknown_20;
    float unknown_24;
    int unknown_28;
    int unknown_2c;
    unsigned char unknown_30[0x68];
    short unknown_98;
    short unknown_9a;
};
/* SzDuel_DoCardControl0 (card_control) returns int; this caller ignores the result, so the
 * view here is void. */
extern void SzDuel_DoCardControl0(struct DotrMiscRect *p);
/* e and f arrive in t1 and t2 and are stored as halfwords. */
int lSzDuel_SetCardCtl(struct DotrMiscRect *p, int a, int b, int c, int d, short e, short f);
/* View of the control storage at 0x317490: four bytes at +0xcb2. */
struct DotrMiscControlCb2 {
    unsigned char unknown_000[0xcb2];
    unsigned char unknown_cb2[4];
};
extern struct DotrMiscControlCb2 data_00317490_cb2;
/* Same declaration as state_grid.h (the callee is in item_release). */
extern int SzDuel_DelUnit(int side, int unitArrayPos);
void SzDuel_Term(void);
/* Text and words used by func_00242580; element types and extents unknown
 * (declared as arrays). */
extern char data_0030f354[];
extern int data_003503b8[4];
extern int videoDec[4];
extern int readBuf[4];
extern int infile[4];
extern int controller_val[4];
extern void initAll(int a);
extern void readMpeg(int *a, int b, int *c);
void termAll(void);
int func_00242580(int a);
/* Callees of movie_play (0x1851e8 and 0x17bb60 lie in the SDK library
 * block): argument counts as passed, results unused. data_003503c0 is only
 * passed by address; its extent is unknown. */
extern void clearGsMem(int, int, int, int, int);
extern void func_001851e8(int *, int, int, int, int, int, int);
extern void func_0017bb60(int);
extern int data_003503c0[16];
/* Stores b to isWithAudio[0] and c to isCountVblank[28], calls
 * clearGsMem(0, 0, 0, 640, 448), 0x1851e8(data_003503c0, 0, 640, 224, 0, 0, 1)
 * and 0x17bb60(0), stores nine constant words to isCountVblank[10..26] (even
 * indices) and one to readBuf[0], then calls func_00242580(a); result unused. */
void movie_play(int a, int b, int c);
/* Each takes the object pointer; int results returned unchanged. */
extern int Ta_Eff_Hand_Fusion(int *p);
extern int Ta_Eff_Hand_Enc(int *p);
extern int Ta_Eff_Hand_Fail(int *p);
int Ta_ManageHandEffect(int *handResult);
/* Table at 0x299f90 passed by address; contents unknown. */
extern int data_00299f90[4];
/* Result tested for zero; out receives two 16-bit values, w is unused here. */
extern int Ex3d_RotTransPers(void *a, int *b, unsigned short *out, int *w);
void Taku_3D_2D_Set(int *x, int *y, void *a);
/* Writes three bytes through out; six arguments from SzColor_CLUT2SEP alone. */
extern void SzColor_RGB2SEP(unsigned char *out, int r, int g, int b, int e, int f);
/* color is returned whole when bits 0-14 are all zero; only those bits are tested. */
int SzColor_CLUT2SEP(int color, int e);
/* One int argument and an int result compared with the caller's argument
 * (draw_state.h has the same declaration). */
extern int func_001854e0(int a);
/* Uses frd[0] and isCountVblank[0] and [2]. */
void startDisplay(int a);
/* Mask builder Ta_ConvBitLengthToNumericLength (0x24c870, ta_values; uses soft-float helpers). */
extern unsigned Ta_ConvBitLengthToNumericLength(int bitLength);
unsigned ConvUnitIDtoFlagList_OFF(unsigned flagList, int side, int pos);
/* Word at +0 is not read; twelve links at +4. */
struct DotrMiscTree {
    int unknown_00;
    struct DotrMiscTree *children[12];
};
int TaLogic_IsConnectRoot(struct DotrMiscTree *tree, unsigned char currentDistance, unsigned char targetDistance);
/* Defined in the side_table group. */
unsigned TaLogic_RemoveHideUnit(unsigned flagList);
/* 24-byte entries at 0x31bc50: a 3-bit and a 10-bit field in the halfword at +2. */
struct DotrMiscEntry24 {
    unsigned short unknown_00;
    unsigned short kind : 3;
    unsigned short id : 10;
    unsigned char unknown_04[20];
};
extern struct DotrMiscEntry24 data_0031bc50[24];
/* One index argument; int result ORed by the caller. */
extern int SzEffect_Destroy(int index);
int SzEffect_Destroy2(int a, int b);
/* Object holding a DotrMiscSpan at +0x48 and a state word at +0xa8. */
struct DotrMiscPlayer {
    unsigned char unknown_00[0x48];
    struct DotrMiscSpan unknown_48;
    unsigned char unknown_8c[0xa8 - 0x48 - sizeof(struct DotrMiscSpan)];
    int unknown_a8;
};
extern int func_0018ee68(struct DotrMiscPlayer *p);
/* Span view of the function sound_file.h declares on DotrSoundStream. */
extern void func_00243f60_span(struct DotrMiscSpan *p);
extern void decBs0(struct DotrMiscPlayer *p);
extern struct DotrMiscObject voBuf;
/* Word +0xc of the object above, re-read in a wait loop. */
extern volatile int data_0035060c_volatile[4];
/* Both defined in the side_table group. */
int videoDecIsFlushed(struct DotrMiscPlayer *p);
void videoDecMain(struct DotrMiscPlayer *p);
/* Four int arguments from Ta_SwapUnit alone; result unused. */
extern void SzDuel_MoveUnit(int a, int b, int c, int d);
/* Defined in the side_table group. */
void Ta_SwapUnit(int a, int b, int c, int d);
/* Words and halfwords of the storage at 0x34fb00 used by TakuSave_card_format/6d0. */
struct DotrMiscLink {
    int unknown_00;
    int unknown_04;
    int unknown_08;
    int unknown_0c;
    int unknown_10;
    int unknown_14;
    int unknown_18;
    int unknown_1c;
    int unknown_20;
    int unknown_24;
    unsigned char unknown_28[4];
    short unknown_2c;
    short unknown_2e;
    short unknown_30;
    short unknown_32;
    short unknown_34;
    short unknown_36;
    short unknown_38;
    short unknown_3a;
    short unknown_3c;
    short unknown_3e;
    short unknown_40;
    short unknown_42;
    short unknown_44; /* 0x2406c0 indexes +0x44 / +0x46 by the word at +8 */
    short unknown_46;
    int unknown_48; /* 0x23f5a0 indexes +0x48 / +0x4c by the word at +8 */
    int unknown_4c;
    short unknown_50;
    short unknown_52;
    short unknown_54;
    short unknown_56;
    unsigned char unknown_58[4];
    int unknown_5c;
    int unknown_60;
    int unknown_64;
    int unknown_68;
    unsigned char unknown_6c[0x14];
    unsigned char unknown_80[0x200];
    short unknown_280;
    short unknown_282;
    int unknown_284;
    int unknown_288;
    int unknown_28c;
    int unknown_290;
    short unknown_294;
    short unknown_296;
    unsigned char unknown_298[0x10];
    short unknown_2a8;
    short unknown_2aa;
    short unknown_2ac;
    short unknown_2ae;
    short unknown_2b0;
    short unknown_2b2;
    short unknown_2b4;
    short unknown_2b6;
    short unknown_2b8;
    unsigned char unknown_2ba[4];
    short unknown_2be;
    /* 0x840 bytes in all: the length 0x23c170 passes to 0x195108. */
    unsigned char unknown_2c0[0x580];
};
extern struct DotrMiscLink data_0034fb00;
/* 0x23c770 / 0x23c8b0: step dispatchers on unknown_30. The callees are in
 * link_step; one int argument and an int result are assumed from the
 * call sites (0x23f130, 0x23c170); the other three are called with
 * no argument set up. */
extern int TakuCard_inti(int a);
extern int TakuLoad_card_kansi(int a);
extern int TakuLoad_card_filename_check(void);
extern int TakuLoad_card_file_open(void);
extern int TakuCustom_card_file_open(void);
/* TakuCustom_card_kansi (link_step): one int argument and an int result are assumed from 0x23cc50. */
extern int TakuCustom_card_kansi(int a);
int TkMc_Custom_Main(int mode);
/* 0x254b20: three words in, a bit word of three picked positions out (or
 * -1). SearchUnitByRange (search_range) is viewed with six int arguments and a bit-word
 * result, as this call site uses it. The three-word local is written and re-read
 * but never used after the loop. */
extern int SearchUnitByRange(int col, int row, int side, int compMode, int searchMode, int range);
int Ta_GetScrificeOrder(int *sacrifices, int side);
/* 0x23c3f0: both callees are passed the constant 1. */
extern int TakuSave_card_kansi(int a);
extern int TakuSave_card_filename_check(int a);
int TkMc_Check_Main(int mode);
int TkMc_Load_Main(int mode);
int TkMc_Trade_Load_Main(int mode);
/* Second symbol for the same storage: the target code keeps its address in
 * a register for the later accesses. One object on the host. */
#ifdef __MWERKS__
extern struct DotrMiscLink data_0034fb00_view;
#else
#define data_0034fb00_view data_0034fb00
#endif
extern int func_001948a8(int a, int b);
extern int func_00194e90(int a, int b);
extern int func_00194248(int a, int *b, int *c);
int TakuSave_card_format(void);
int TakuSave_card_unformat(void);

/* videoDecFlush (defined in the side_table group). It calls viBufBeginPut (0x2440f0) with a
 * span and four output words and viBufEndPut (0x2441f0) with a count, so those two get view
 * prototypes distinct from the one-argument externs above; cpy2area (0x245630) is declared as
 * in sound_file.h. MATCHING DEVICE: the 4-byte value at 0x29b300 is copied to the stack as a
 * one-float struct, which gives the original's load and store; its content is not
 * interpreted. */
struct DotrMiscFloatWord {
    float value;
};
extern struct DotrMiscFloatWord data_0029b300[4];
extern int func_002440f0_five(struct DotrMiscSpan *span, int *a, int *b, int *c, int *d);
extern void func_002441f0_count(void *p, int n);
extern int cpy2area(char *first, int first_size, char *second, int second_size, char *a, int a_size, char *b, int b_size);
int videoDecFlush(struct DotrMiscPlayer *p);

/* 0x1b6ea0 formats a decimal number into a 16-byte stack buffer and forwards
 * it to SzDuel_DrawWindow_Font_Pipe (window_font). The eight-argument callee prototype, the
 * word type of the first argument and the returned word are assumed from
 * this caller and 0x1c3d50's call. */
extern DotrMiscAddress SzDuel_DrawWindow_Font_Pipe(DotrMiscAddress cursor, int x, int y, int a, int flags, int n, const char *text, int c);
DotrMiscAddress SzDuel_DrawWindow_FontDigit_Pipe(DotrMiscAddress cursor, int x, int y, int a, int flags, int width, int value);

/* 0x242c20 termAll (defined in the side_table group). It passes one argument to the empty
 * routines readBufDelete (0x241ac0) and voBufDelete (0x2457c0), hence the two view prototypes. The
 * six 0x17bxxx/0x17cxxx callees are library routines; argument counts are those loaded by this
 * caller, types assumed. Words at 0x350730/0x350734, 0x350778 and 0x350780 have unknown extents. */
extern void func_00241ac0_word(int a);
extern void func_002457c0_object(struct DotrMiscObject *p);
extern void func_0017b730(int a);
extern void func_0017b6f0(int a);
extern void func_0017c490(int a);
extern void func_0017b610(int a, int b);
extern void func_0017c3c0(int a);
extern void func_0017b5e0(int a, int b);
extern int videoDecTh[4];
extern int defaultTh[4];
extern int data_00350730[4];

/* View of the control storage at 0x317490 for 0x1c32b0: two addresses at
 * +0x6e0 and +0x6e4, each used as 854 unsigned bytes (pointers here, so the
 * host view is wider). The extent 854 comes from the loop bounds only;
 * lSzDuel_TradeDoSide (0x1c33a0, side_table) is declared here with an int argument. */
struct DotrMiscControlCounts {
    unsigned char unknown_000[0x6e0];
    unsigned char *first;
    unsigned char *second;
};
extern struct DotrMiscControlCounts data_00317490_counts;
extern void lSzDuel_TradeDoSide(int side);
int lSzDuel_TradeDo(void);

/* videoDecCreate (defined in the side_table group) sets up a player: 0x18eb08(p, a, b), five
 * callbacks registered through 0x18ef20(p, kind, function, 0) for kinds 0, 1, 2, 3 and 5, then
 * videoDecReset(p) and 0x243ee0 on the span at +0x48 with the last five arguments. The callback type
 * is assumed to be int (void); mpegError (0x245520) and mpegTS (0x2455e0) take two arguments
 * and are converted to it. MATCHING DEVICE: both conversions go through an intermediate
 * `void (*)(void)` cast; the one on mpegTS is needed to reproduce the original. 0x243ee0 is
 * declared here on the span view (sound_file.h declares it on DotrSoundStream). */
typedef int (*DotrMiscPlayerCallback)(void);
extern void func_0018eb08(struct DotrMiscPlayer *p, int a, int b);
extern void func_0018ef20(struct DotrMiscPlayer *p, int kind, DotrMiscPlayerCallback function, int d);
/* 0x2455e0 copies the first two 64-bit words that viBufGetTs (vibuf_dma)
 * writes into a 32-byte local to out[1] and out[2]. The 64-bit element type,
 * the local's four-word extent (from the frame size) and the callee
 * prototype are assumed. */
extern void viBufGetTs(void *p, long *out);
int mpegTS(void *unused, long *out);
extern void func_002450f0_player(struct DotrMiscPlayer *p);
extern int func_00243ee0_span(struct DotrMiscSpan *p, int a, unsigned b, int c, int d, int e);
int videoDecCreate(struct DotrMiscPlayer *p, int a, int b, int c, unsigned d, int e, int f, int g);

/* Views of the four words at 0x32f780 used by 0x213fc0: output cursor, end
 * and first of an 8-byte key/value entry list, and a flag. The entry layout,
 * the names and the RhMath_HeapSortStruct (sort-like) prototype are assumed. The target
 * code reads two words through the symbol and the rest through a register,
 * hence the second symbol; one object on the host. */
struct DotrMiscSortEntry {
    int key;
    int value;
};
struct DotrMiscSortList {
    int *out;
    struct DotrMiscSortEntry *end;
    int sorted;
    struct DotrMiscSortEntry *first;
};
extern struct DotrMiscSortList gRhZSort_zsl_list;
#ifdef __MWERKS__
extern struct DotrMiscSortList gRhZSort_zsl_list_view;
#else
#define gRhZSort_zsl_list_view gRhZSort_zsl_list
#endif
extern void RhMath_HeapSortStruct(void *base, int count, int size, int (*compare)(int *, int *));
int lRhZSort_ZCompare(int *a, int *b);
int *RhZSort_Finish_Pipe(int *cursor);

/* Same shape as Ta_Eff_034 with id 533; SzDuel_SetHandPhase is defined in
 * state_records and declared here as a callee (two int arguments). */
extern void SzDuel_SetHandPhase(int side, int value);
int Ta_Eff_033(DotrMiscJob *in, DotrMiscJobQueue *q, int mode);

/* ShFace_Draw_Pipe (defined in the side_table group). The `_word` names are int-cursor views of
 * SzText_DoRequest_Pipe (0x202b00) and RhText_SetTextureFlush (draw_state.h keeps word-typed
 * externs); RhBase_GetUserGsAddress (0x209e50) is defined in draw_state and ShFace_DrawOne
 * (0x2460f0) is declared below. Prototypes and the int cursor are assumed. */
extern int RhBase_GetUserGsAddress(void);
extern int func_00202b00_word(int cursor, int a0, int a1, int *out);
extern int func_0020f290_word(int cursor);
/* ShFace_DrawOne (0x2460f0). Bytes of the entry as it uses them (offsets from the entry): +4
 * set, +5 pattern row, +6 mask of running kinds, +11.. one countdown and +27.. one frame number
 * per animated part. The eight-byte records filled by ShAnime_GetFixInfo (0x245a30) and
 * ShAnime_GetAnmDat (0x245ae0) and the prototypes of those two and of ShFace_Prim (0x245db0),
 * all in sh_face_parts, are assumed from the call sites; a fixed part's record is read again as
 * DotrMiscFaceFixAnm when bit 7 of its flags is set. */
struct DotrMiscFaceFix {
    unsigned char unknown_00;
    unsigned char flags;
    unsigned char row;
    unsigned char unknown_03;
    unsigned short x;
    unsigned short y;
};
struct DotrMiscFaceFixAnm {
    unsigned char unknown_00;
    unsigned char flags;
    unsigned short kind;
    unsigned short count;
    unsigned short unknown_06;
};
struct DotrMiscFaceAnm {
    unsigned char unknown_00;
    unsigned char delay;
    unsigned char row;
    unsigned char unknown_03;
    unsigned short x;
    unsigned short y;
};
extern void ShAnime_GetFixInfo(int set, int row, unsigned char index, struct DotrMiscFaceFix *out);
extern void ShAnime_GetAnmDat(int set, int row, unsigned char index, int frame, struct DotrMiscFaceAnm *out);
extern void ShFace_Prim(struct DotrMiscEntry52 *entry, struct DotrMiscHalfRow *rect, int x, int y);
void ShFace_DrawOne(struct DotrMiscEntry52 *entry);
int ShFace_Draw_Pipe(int cursor);

/* 0x202b00 walks the same 1024 entries as 0x202d60. `_word` is an
 * int-cursor view of SzText_Load_Pipe (declared above) whose fourth argument is
 * taken as a count output (draw_state.h keeps a word-typed extern);
 * parameter meanings are assumed. */
extern int func_00202fb0_word(int cursor, int a0, int a1, int *count);
int SzText_DoRequest_Pipe(int cursor, int kind, int pos, int *out);

/* 0x276120 claims the first free slot (key 0xffff) of a 22-entry table of
 * 16-byte slots. Field names, the two-bit mode field at +0x0c and the
 * 16-bit id parameter are assumed from this function's stores alone. */
typedef struct DotrMiscSlot16 {
    unsigned short key;
    unsigned short value;
    unsigned short link;
    unsigned short pad_06;
    int ref;
    unsigned char mode : 2;
    unsigned char rest : 6;
    unsigned char pad_0d[3];
} DotrMiscSlot16;
int TaLogic_DoUnitAssign(unsigned short ownerId, DotrMiscSlot16 *table);

/* 0x2500c0 moves the two per-cell levels read by 0x1e1d10 and 0x1e1cb0
 * towards 128 (dir > 0) or 0 through the state_grid setters, after
 * SzEffect_SetBusyFlag2(side << 8 | pos, 0). flags bit 0 selects the 0x1e1d10 level and
 * bit 1 the 0x1e1cb0 level; a non-zero pos whose 0x1e0700 kind (>> 5) is
 * non-zero keeps only bit 1. No result is used; void and all parameter
 * meanings are assumed. The callees are defined in state_grid and
 * packed_item; their prototypes are repeated here because those headers
 * cannot be included with this one. */
extern int SzEffect_SetBusyFlag2(int packed, int flag);
extern int SzDuel_GetUnitCtlRateC(int side, int pos);
extern int SzDuel_GetUnitCtlRateM(int side, int pos);
extern int SzDuel_SetUnitCtlModeM(int side, int pos, int value);
extern int SzDuel_SetUnitCtlGoalM(int side, int pos, int value);
extern int SzDuel_SetUnitCtlRateM(int side, int pos, int pair, int value);
extern int SzDuel_SetUnitCtlModeC(int a, int b, int value);
extern int SzDuel_SetUnitCtlGoalC(int side, int pos, int value);
extern int SzDuel_SetUnitCtlRateC(int a, int b, int pair, int value);
void Ta_UnitFade(int side, int pos, int dir, int value, int flags);

/* TaUnit_AddUnitEfc (defined in the side_table group) builds a four-word record {113, a, b, 0x80000000}
 * or {113, 0, 0, c | 0x80000000} from position words made by 0x24c780, or forwards to 0x24fd10.
 * 0x271a40, 0x2720f0 and 0x272050 prototypes follow their definitions; parameter meanings
 * (side, pos, x, y) are assumed. */
extern int Ta_IsLDRange(int side, int col, int row);
int TaUnit_AddUnitEfc(int side, int pos, int x, int y, struct DotrMiscWords4 *out);

/* 0x24e220 asks Ta_GetSommonPiecePreference (summon_cells; (a, i) -> packed y << 8 | x or
 * negative) for eight indices and returns the first
 * position whose cell is free; a nonzero flag also rejects kind 8 cells. */
extern int Ta_GetSommonPiecePreference(int a, int direction);
int Ta_GetSummonablePiece(int a, unsigned char flag);
/* 0x24e300 is 0x24e220 with a count: it returns the idx-th (from 0) accepted
 * position instead of the first. Only the low byte of avoidLabyrinth is tested; the int
 * parameter follows the declaration its callers use. */
int Ta_GetSummonablePieceWithOrder(int side, int idx, int avoidLabyrinth);

/* 0x1349a0 returns the first cell (y << 8 | x; odd y, x 1-5) whose 0x1e22b0
 * kind is not 8; failing that 6 when some occupied cell in columns 0-2 has an
 * owner (0x250360 view) other than side and cells (6,0) and (5,0) are not
 * kind 8; else -1. The meaning of side and the axis names are assumed. */
int TaLogic008_GetWallEmpty(int side);

/* Ta_AllUnitFade2 (defined in the side_table group) walks 22 positions, 0-10 on side 0 and 11-21 on
 * side 1. With dir negative it applies 0x2500c0 to every position whose 0x1e1a90 value is
 * positive and records it in the word at owner + 0xcc through 0x24c780; otherwise it applies
 * 0x2500c0 to the positions whose bit (21 - i) is set in that word. The first and third
 * parameters are unused; the owner layout is not known. */
extern int SzDuel_GetUnitCtlGoalM(int a, int b);
void Ta_AllUnitFade2(int a, unsigned char *owner, int c, int dir, int value, int flags);

/* Ta_AllUnitFade (defined in the side_table group) is the 0x25fc30 walk with a second word at mask +
 * 8: a position whose bit is set there only gets SzEffect_SetBusyFlag2(pos, flag) (side 1 as (pos - 11) |
 * 0x100). The others take the 0x25fc30 path with flags 1, and with dir negative the 0x1e1a30
 * value must be positive as well. Third parameter unused; both layouts are unknown. */
extern int SzDuel_GetUnitCtlGoalC(int a, int b);
void Ta_AllUnitFade(unsigned char *mask, unsigned char *owner, int c, int dir, int flag, int value);

/* 0x122680 classifies the 0x1e1090 word of (a, b), asked twice: low byte and
 * the rest shifted down by 8 select a class 0-7 from eight range tests in
 * order, else -1. What the two parts and the classes mean is not known. */
int TaLogic005_GetUnitOnCycleRoot(int a, int b);

/* TaLogic002_IsActiveMode (defined in the side_table group) fills 22 halfwords with 0xffff, lets
 * TaUtil_GetUnitLst(kind, 1, buf) write positions, and answers 1 when any position's 0x1e1030 id is 402,
 * 403 or 422; otherwise 0 when id 848 or 798 appears among those positions, the five 0x1df0c0
 * words (low 12 bits) or the 0x1de9e0 list; else 1. The state_grid/state_records prototypes are
 * repeated here; kind is the 0x1de540 value. */
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_InqHand(int side, int position);
extern int SzDuel_GetDeckCount(int side);
extern int SzDuel_InqDeck(int side, int position);
extern int SzDuel_GetTurnOwner(void);
int TaLogic002_IsActiveMode(void);

/* View of the control storage at 0x317490 for 0x1c2300: per side, a list of
 * 854 halfwords (+0x6d8), 854 count bytes (+0x6e0, the pointers 0x1c32b0
 * uses), a first-row/length halfword pair (+0xc8a), a mode byte (+0xcc0) and
 * a row-count byte (+0xd5e). Pointers here, so the host view is wider; the
 * names are placeholders. */
struct DotrMiscControlPair {
    unsigned short top;
    unsigned short count;
};
struct DotrMiscControlLists {
    unsigned char unknown_000[0x6d4];
    void *table;
    short *lists[2];
    unsigned char *counts[2];
    unsigned char unknown_6e8[0x5a2];
    struct DotrMiscControlPair pairs[2];
    short unknown_c92;
    short unknown_c94;
    short unknown_c96;
    short unknown_c98;
    unsigned char unknown_c9a[0x1c];
    unsigned char unknown_cb6[6];
    unsigned char unknown_cbc[2];
    unsigned char unknown_cbe[2];
    unsigned char modes[2];
    unsigned char unknown_cc2[0x9a];
    unsigned char side;         /* 0xd5c: the side lSzDuel_ReBuildBag asks about */
    unsigned char unknown_d5d;
    unsigned char rows[2];
};
extern struct DotrMiscControlLists data_00317490_lists;

/* Declared as in side_table.h; defined in the side_table group. */
extern int SzData_GetBagCount(int side, int cardNo, int kind);
/* 0x1c2300: rebuild side `flag`'s list. Entry i becomes i when, by the side's
 * mode byte, SzData_GetBagCount(flag, i, 3) (mode 0), the side's own count byte (mode
 * 1) or the other side's count byte (mode 2) is above zero, else 999; the
 * pair's length counts the kept entries (any other mode leaves the list and
 * a length of 0). When the length is below first row + row count the first
 * row becomes length - 5, not below 0. Ends with lSzDuel_SortTrade(flag). Only 0 and
 * 1 are meaningful flags: the mode byte is indexed by the flag itself. */
void lSzDuel_SetupTrade(int flag);

/* 0x1c21a0 SzDuel_InitTrade (name from the Japanese build's symbol table). The trade
 * work block at the literal 0x1ed0000, as the function lays it out: per side 854
 * halfwords, then per side 854 count bytes (854 is the card count; it bounds both
 * clearing loops). The host build gives the block an object. The table (+0x6d4) and
 * the fields at +0xc92..+0xcbf of the control view above are only cleared or preset
 * here; their names are positions. data_003180e8 is the word at +0xc58 of the same
 * storage, which the function addresses on its own. */
struct DotrTradeWork {
    short lists[2][854];
    unsigned char counts[2][854];
};
#ifdef __MWERKS__
#define DOTR_TRADE_WORK ((struct DotrTradeWork *)0x1ed0000)
#else
extern struct DotrTradeWork dotr_trade_work_01ed0000;
#define DOTR_TRADE_WORK (&dotr_trade_work_01ed0000)
#endif
/* An address SzDuel_InitTrade keeps in an unsigned integer local and stores as a signed
 * word (32 bits on the target; the host build widens both so the test can point them at
 * an object). */
#ifdef __MWERKS__
typedef unsigned int DotrTradeAddr;
typedef int DotrTradeWord;
#define DOTR_TRADE_COUNTS1 0x1ed10ae /* &DOTR_TRADE_WORK->counts[1][0] */
#else
typedef unsigned long DotrTradeAddr;
typedef long DotrTradeWord;
#define DOTR_TRADE_COUNTS1 ((DotrTradeAddr)DOTR_TRADE_WORK->counts[1])
#endif
/* 0x1b8720 lSzDuel_ReBuildBag (name from the Japanese build's symbol table): rebuilds the
 * first list for the side byte at +0xd5c. Entry i becomes i when SzData_GetBagCount(side,
 * i, -1) is above zero, else 999; 854 is the card count and bounds the loop. The first
 * pair's count is the number kept and its top is limited to 0..max(count - 5, 0); ends
 * with lSzDuel_SortBag(). The clamp is spelled with two macros because the code
 * evaluates max(top, 0) twice. */
void lSzDuel_ReBuildBag(void);
extern int data_003180e8;
extern unsigned char gSzDuel_NameIndex1[];
extern unsigned char gSzDuel_NameIndex0[];
void SzDuel_InitTrade(void);

/* View of a 65440-byte entry at 0x32f790 for 0x1dde80: five 7 by 7 byte
 * grids at +0xfea3 (they end at +0xff98). 46 such grids are read at
 * 0x2cee5c; that count comes from the range test only. */
struct DotrMiscLargeGrids {
    unsigned char unknown_0000[0xfea3];
    unsigned char grids[5][7][7];
    unsigned char unknown_ff98[8];
};
extern unsigned char gSzDuel_MapData[46][7][7];

/* 0x1dde80: pass one 7 by 7 byte grid to SzDuel_SetField(x, y, byte), row by row:
 * grid `index` at 0x2cee5c for 0-45, grid index - 46 of the SzData_GetData(0)
 * entry for 46-50 and for a negative index (which then reads before the
 * entry's grids), grid index - 51 of the SzData_GetData(1) entry for 51-55;
 * nothing from 56 on. */
void SzDuel_SetupMap(int index);

/* Ta_Eff_101 is 0x25ee40's shape with id 601: mode >= 0 also calls TaUtil_DoOpen(second >> 8,
 * second & 0xFF, 1); the queued request copies the caller's second and value words and is only
 * made while SzDuel_GetUnitOpenRate(second >> 8, second & 0xFF) answers 0xFF. Assumed: the job
 * views above. */
extern int SzDuel_GetUnitOpenRate(int side, int pos);
int Ta_Eff_101(DotrMiscJob *in, DotrMiscJobQueue *q, int mode);

/* For each set position of `word` (0x24e910 count, 0x24ec50 position), the
 * word is rebuilt by 0x24c7d0 when Ta_IsLDRangeAvility(high, low, 11) or else
 * Ta_IsResistAll(high, low) answers and 0x24de70 of the position differs from `side`.
 * Assumed: int arguments and results; Ta_IsResistAll (state_queries) is passed two ints. */
extern int Ta_IsLDRangeAvility(int side, int pos, int ability);
extern int Ta_IsResistAll(int side, int pos);
int Ta_RemoveObjFromFlagLst(int flagList, int side);

/* 0x272210 with effects: each position's side is also looked up through
 * SzDuel_GetUnitLoc(side_of_position, 0). On the 0x271e40 branch SzDuel_OpenMsgPopup(0, -120, 80,
 * 270) and Ta_SetRangeVisual(low byte, high byte, 0, Ta_IsLDAbl(9, side_of_position), 1,
 * 1, 11) run before the rebuild; on the 0x267700 branch SzDuel_OpenMsgPopup(0, 0, 80,
 * SzDuel_GetMsgName(83)) does. Assumed: int arguments and results, as in the callees' definitions. */
extern void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);
extern void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int visualType);
extern int SzDuel_GetMsgName(int index);
int Ta_RemoveObjFromFlagLst2(int word, int side);

/* Job step with id 603 (shape of 0x25ed20): mode >= 0 resets; otherwise a
 * `switch` on q->count: 0 asks 0x255b50 and, on 1, queues a request whose
 * `second` is in->second; any other count asks SzEffect_IsUse(q->slot) and, on 0,
 * calls Ta_DelUnit2(in->second >> 8 (logical), in->second & 0xff) and 0x1e4530.
 * Assumed: the job views of 0x25ed20; 0x24f430 as defined in state_queries. */
extern int Ta_DelUnit2(int a, int b);
int Ta_Eff_Hand_Dest(DotrMiscJob *in, DotrMiscJobQueue *q, int mode);
/* 0x263790: same shape as 0x271630 with request id 534. The queued request carries the
 * unsigned difference between in->value and SzDuel_GetSummonPower(in->second) and which side was
 * larger. Callees as defined in state_records (its header is not included here). */
extern void SzDuel_SetWindowDraw(int value);
extern int SzDuel_GetWindowDisp(void);
extern int SzDuel_GetSummonPower(int side);
int Ta_Eff_065(DotrMiscJob *in, DotrMiscJobQueue *q, int mode);

/* 0x23b920: when the flag byte at 0x34f6f8 is set, copies the 65440-byte block again
 * (as 0x23bc80), reads three side-0 values, picks three constants by the byte at
 * 0x1ed546b and replays them through the callees; always ends with 0x1e4530.
 * Assumed: the flag is read signed; callee prototypes as defined in side_table,
 * src/ee/menu_set.c and phase_sends. */
extern signed char data_01ed546b;
extern int SzData_GetOption(int side, int which);
extern int SzGame_SetMenuNum(int index, int value);
extern void SzData_SetupFromConstDeck(int side, int deckIndex);
extern int SzData_SetOption(int side, int which, int flag);
void TkCamp_Term(void);

/* 0x21c350. Solves a tridiagonal system of n equations (n at most 64, the size of
 * the local work array) by forward elimination and back substitution: lower[i],
 * diagonal[i], upper[i] are row i's three coefficients, right[i] its right-hand side,
 * and solution[] receives the answer. No pivoting and no zero-pivot test. Names
 * are assumed from the arithmetic; n < 1 still writes solution[0]. */
void tri_diag(float *lower, float *diagonal, float *upper, float *right, float *solution, int n);

/* One character rectangle of gSzDuel_Font (0x290b70; Japanese build data name),
 * 37 per font, ten bytes each. Only the words at +2 and +6 are read here and
 * their difference is used as a width: left/right are assumed names, the
 * other members are unknown. gSzDuel_abFontSize (0x2be950) holds a cell
 * width and a line height per font. */
struct DotrFontRect {
    unsigned short unknown_00;
    unsigned short left;
    unsigned short unknown_04;
    unsigned short right;
    unsigned short unknown_08;
};
extern struct DotrFontRect gSzDuel_Font[];
extern unsigned char gSzDuel_abFontSize[][2];
/* Parameter names are assumed from use; the Japanese build has no debug
 * information for this function. Calls lSzDuel_GetFontIndex (0x1b7660). */
void lSzDuel_GetFontSize(int font, int flags, int n, char *str, int *w, int *h);

/* 0x1c5d70 SzDuel_InitCustomDuel. The control block at 0x317490 as this function writes it
 * (cast from data_00317490_lists): per side a life-point halfword, a deck byte and a
 * byte set to 3, and two bytes at +0xd32. */
struct DotrCustomDuelSide {
    unsigned char unknown_000[0x33a];
    unsigned short lp;
    unsigned char deck;
    unsigned char unknown_33d[5];
    unsigned char n;
    unsigned char unknown_343[0xd];
};
struct DotrCustomDuelControl {
    struct DotrCustomDuelSide side[2];
    unsigned char unknown_6a0[0x692];
    unsigned char a;
    unsigned char b;
};
/* The custom-duel settings at the literal address 0xa9c000 (map_edit.h and
 * state_records.h have other views of the block). The function addresses it as a
 * literal: with a linked symbol the compiler leaves one branch delay slot empty. */
struct DotrCustomDuel {
    unsigned char f0[2];
    unsigned char f2[2];
    unsigned char deck[2];
    unsigned char f6[2];
    unsigned char f8[2];
    unsigned char fa[2];
    unsigned char fc[2];
    unsigned char fe[2];
    unsigned short lp[2];
    unsigned short lp2[2];
    unsigned short list[56];
    unsigned char mode;
    unsigned char last;
    unsigned char count;
    unsigned char f8b;
    unsigned char f8c;
    unsigned char f8d;
};
#ifdef __MWERKS__
#define DOTR_CUSTOM_DUEL ((struct DotrCustomDuel *)0xa9c000)
#else
extern struct DotrCustomDuel dotr_custom_duel_00a9c000;
#define DOTR_CUSTOM_DUEL (&dotr_custom_duel_00a9c000)
#endif
/* Bit 5 of the word at +0xff9c of a save block (side_table.h: DotrSideBits). */
struct DotrCustomDuelFlags {
    unsigned char unknown_0000[0xff9c];
    unsigned int b0 : 5;
    unsigned int b5 : 1;
};
/* Prototypes as defined in memory_ops, entry_table and side_table; SzText_LoadMerge
 * (0x2009e0, text_merge) as movie_operations.h has it. */
extern unsigned char *RhMem_CopyMemory(unsigned char *destination, unsigned char *source, int count);
extern int SzText_Free(int group, int id);
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetMapEditFlag(int side);
extern int SzText_LoadMerge(int a, int b);
/* A mode of 0 or more is stored at 0xa9c088. By the stored mode: 1 copies save block
 * SzData_GetData(0) over SzData_GetData(1) (0xffa0 bytes), clears its bit 5 and picks
 * deck 22; 2, 3, 4 call SzData_SetupFromConstDeck(1, 50 / 49 / 48) and pick deck 25 /
 * 24 / 23; otherwise deck 101. Frees texts (0, 1), (0, 0), (1, -1). For a mode of 0 or
 * more both sides get 4000 life points and 3, side 0 deck 100 and side 1 the picked
 * deck, the settings are reset (decks from SzData_GetCurrentDeck) and the list is
 * rebuilt: 24 entries i | 0x200, then 5 entries i when SzData_GetMapEditFlag(0), then
 * 5 entries i | 0x100 when SzData_GetMapEditFlag(1). For a negative mode only the two
 * decks and the stored life points are put back. Ends with SzText_LoadMerge(4, 1) and
 * (7, 1) and clears the two bytes at +0xd32. Assumed: int mode, no result. */
void SzDuel_InitCustomDuel(int mode);

#endif

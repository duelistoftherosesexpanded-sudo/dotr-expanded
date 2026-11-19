#ifndef DOTR_EE_OBJECT_TABLE_H
#define DOTR_EE_OBJECT_TABLE_H

/* Offset views of the objects addressed by the 84-word table at EE 0x31abb0.
 * Not recovered declarations or object bounds; only the named offsets are
 * observed. Evidence: docs/modules/models.md. The pointer member
 * makes host offsets after +0x138 differ from the target. */
/* An object word that holds an address: 32 bits on the target. */
#ifdef __MWERKS__
typedef int DotrObjectWord;
#else
#include <stdint.h>
typedef intptr_t DotrObjectWord;
#endif

struct DotrObjectVector3 {
    float x, y, z;
};

struct DotrObjectVector4 {
    float x, y, z;
    float unknown_w;
};

struct DotrObjectLink {
    int unknown_00;
    int unknown_04;
};

struct DotrObjectPair {
    signed char unknown_bits_0_5 : 6;
    signed char unknown_bits_6_7 : 2;
};

struct DotrObjectBits {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bit_2 : 1;
    signed char unknown_bit_3 : 1;
    signed char unknown_bit_4 : 1;
    signed char unknown_bit_5 : 1;
    signed char unknown_bit_6 : 1;
    signed char unknown_bit_7 : 1;
};

struct DotrObjectLowPair {
    unsigned char unknown_bits_0_1 : 2;
    unsigned char unknown_bits_2_3 : 2;
    unsigned char unknown_bits_4_7 : 4;
};

struct DotrObjectQuad {
    unsigned char unknown_0;
    unsigned char unknown_1;
    unsigned char unknown_2;
    unsigned char unknown_3;
};

struct DotrObject {
    struct DotrObjectVector4 pos;
    struct DotrObjectVector4 rot;
    struct DotrObjectVector4 rotC;
    struct DotrObjectVector4 scale;
    unsigned char coordMatrix[0x40];
    struct DotrObjectVector4 center;
    unsigned char unknown_090[0x10];
    struct DotrObjectVector4 boneRefWorldPos[8];
    struct DotrObjectVector3 intensity;
    struct DotrObjectQuad rgba;
    struct DotrObjectQuad filters;
    int GNode;
    struct DotrObjectLink *modelBaseData;
    unsigned char unknown_13c[8];
    unsigned char effectTextureB[0x20];
    int OverlayM;
    int OverlayB;
    int pCameraMotion;
    int pCameraMotion2;
    unsigned char ptrBlock[4];
    int animationDataPointers[8];
    /* Indexed by slot 0-3 in the original; slots 2 and 3 overlap the
     * halfwords and words that follow. */
    struct DotrObjectVector3 unknown_198[2];
    unsigned short boneRefIds[8];
    unsigned animationFrameCountArray[8];
    signed char unknown_1e0[4];
    float radius;
    unsigned char unknown_1e8[8];
    short coordIndex;
    signed char modelId;
    signed char type;
    short modelNo;
    unsigned char cameraAxisIndex;
    struct DotrObjectPair animDir;
    struct DotrObjectLowPair alphaMode;
    unsigned char boneRefSlotCursor;
    unsigned char unknown_1fa;
    unsigned char blendTickTotal;
    unsigned char blendTickCurrent;
    unsigned char motionDelay;
    unsigned char refBoneNum;
    unsigned char deformRatio;
    unsigned char stoneRatio;
    unsigned char unknown_201[9];
    unsigned short maxOT;
    struct DotrObjectBits drawFlag;
    struct DotrObjectBits stateFlags;
};

extern struct DotrObject *gSzModel_ahModel[];

/* One 112-byte entry of the 32-entry table at EE 0x31ad00, which directly
 * follows the pointer table. Only the signed 27-bit field at +0x68 is observed. */
struct DotrObjectEntry112 {
    unsigned char unknown_00[0x68];
    signed int unknown_68_bits_0_26 : 27;
    signed int unknown_68_bits_27_31 : 5;
    unsigned char unknown_6c[4];
};
typedef char DotrObjectEntry112MustBe70[(sizeof(struct DotrObjectEntry112) == 0x70) ? 1 : -1];

extern struct DotrObjectEntry112 gSzModel_aBlur[];
extern int gSzEvent_nSave[];
extern int gSzModel_nBlurMode[];
extern int gSzModel_fPacking[];
extern char data_002cf9d0[];
extern char data_002cf9d5[];

void SzEvent_Init(int value);
int SzModel_Init(void);
int SzModel_Destroy(int modelId);
void SzModel_CancelMotionBlur(int value);
int SzModel_DrawWithOutCalc_Pipe(int cursor, int modelId, int c, void *d, int e, int f, int g);
int lSzModel_GetGNode(int modelId, int gNodeId, int *maxDepth);
int lSzModel_Clean(int modelId);
int SzModel_Preset(int modelId, int modelType, int modelNumber);
int lSzModel_LoadModel(int modelId, char *filePath);
/* 0x1f0060: assumed parameters, forwarded in this order to SzFile_ReadRequestParam after a zero priority. */
int lSzModel_LoadModelPack(int modelId, char *filePath, int unknown_014, int byte_size, int byte_offset, int unknown_010);
int lSzModel_FreeMotion(int modelId, int slot);
int SzModel_BattleSeq2(int side, int flag, int unknown);
int SzModel_SetAnimIntrParam(int modelId, int animIndex, int unknown);
int SzModel_SetAnimDirect(int index, int slot);
/* SzModel_Motion: the result is assumed to be an unsigned char (the zero
 * returns are 128-bit clears); the frame speed is converted as a signed int
 * (func_001a8760_int view). */
unsigned char SzModel_Motion(int modelId);
int SzModel_GetCenter(int modelId, struct DotrObjectVector4 *xyzCoords);
/* SzModel_GetCenterOfs: the word at 0x13c is read as a pointer to floats and the
 * lSzModel_TraceGNode result as a pointer to floats; both are assumptions. */
int SzModel_GetCenterOfs(int modelId, struct DotrObjectVector4 *centerArray);
int SzModel_SetRefID(int modelId, int value, int flag);
int SzModel_SetRefID_RefNo(int modelId, int value, int slot);
int SzModel_GetRef(int modelId, int slot, struct DotrObjectVector4 *out);

float SzModel_GetRadius(int modelId);
void *SzModel_GetEffTextureB(int modelId);
int SzModel_GetOverlayM(int modelId);
/* 0x1f3910 with its result used; state_grid declares the void view. */
extern int func_001f3910_result(int a, int b, int c);
int SzModel_LoadOverlayB(int modelId);
int SzModel_GetOverlayB(int modelId);
int SzModel_GetCamera(int modelId, int kind);
int SzModel_IsModelReady(int index);
int SzModel_IsModelLoading(int modelId);
int SzModel_IsModelStandby(int index);
int SzModel_IsModelDisp(int modelId);
int SzModel_GetAnim(int modelId);
int SzModel_GetAnimParam(int modelId, int paramIndex);
int SzModel_SetAnim(int modelId, int animIndex);
int SzModel_SetAnimParam(int index, int value, int slot);
int SzModel_SetAnimIntr(int modelId, int animIndex);
/* 0x1f16b0 tail-jumps to 0x1f16c0 with a second argument of 6. Two int parameters and an
 * int result are assumed for the callee. */
extern int SzModel_SetAnimIntrReverseParam(int index, int frames);
int SzModel_SetAnimIntrReverse(int index);
int SzModel_SetMotionDelay(int modelId, int motionDelay);
int SzModel_GetAnimDir(int modelId);
int SzModel_SetAnimDir(int modelId, int animDir);
int SzModel_SetAnimSpeed(int index, float value);
float SzModel_GetAnimFrame(int modelId);
int SzModel_SetAnimFrame(int modelId, float animFrame);
int SzModel_GetIntensity(int modelId, struct DotrObjectVector3 *intensity);
int SzModel_GetIntensityOnly(int index, float *out);
int SzModel_SetIntensity(int modelId, struct DotrObjectVector3 *intensity);
int SzModel_SetIntensityOnly(int index, float value);
int SzModel_GetPos(int modelId, struct DotrObjectVector4 *xyzCoords);

int SzModel_SetPos(int modelId, struct DotrObjectVector4 *pos);
int SzModel_SetRot(int index, struct DotrObjectVector4 *in);
int SzModel_SetRotC(int index, struct DotrObjectVector4 *in);
int SzModel_GetScale(int modelId, struct DotrObjectVector4 *out);
int SzModel_SetScale(int index, struct DotrObjectVector4 *in);
int SzModel_GetRgba(int modelId, struct DotrObjectQuad *out);
int SzModel_SetRgba(int index, int value, struct DotrObjectQuad *in);
int SzModel_GetAlpha(int index);
int SzModel_SetAlpha(int index, int value);
int SzModel_GetModelNo(int modelId);
int SzModel_GetModelType(int modelId);
int SzModel_SetDeform(int modelId, int value);
int SzModel_GetDeform(int modelId);
int SzModel_SetDeformRatio(int index, int deformToSet);
int SzModel_GetRefNum(int index);
int SzModel_InqRefID(int modelId, int refIdIndex);
int SzModel_SetMaxOT(int index, int value);
int SzModel_GetMaxOT(int index);
int SzModel_SetDrawFlag(int index, int value);
int SzModel_GetDrawFlag(int index);
int SzModel_SetCalcFlag(int modelId, int flagValue);
int SzModel_GetCalcFlag(int modelId);
int SzModel_SetFogFlag(int index, int value);
int SzModel_SetPauseFlag(int modelId, int value);
int SzModel_GetPauseFlag(int modelId);
int SzModel_SetStoneFlag(int modelId, int value);
void SzEffect_SetIntensity(int targetModelType, struct DotrObjectVector3 *intensity);
void SzEffect_SetIntensityOnly(int kind, float value);
int SzModel_SetAnimSpeedParam(int modelId, int paramIndex, float speed);
float SzModel_GetAnimFrameParam(int modelId, int animIndex);
int SzModel_SetAnimFrameParam(int modelId, int animIndex, float requestedFrame);
int SzModel_GetAnimFrameMax(int modelId, int animIndex);
int SzModel_SetFilter(int index, int value, struct DotrObjectQuad *in);
int SzModel_GetStoneFlag(int index);
int SzModel_GetStoneRatio(int index);
int SzModel_GetRot(int modelId, struct DotrObjectVector4 *rotationArray);
int SzModel_GetRotC(int index, struct DotrObjectVector4 *out);

/* Callee prototypes assumed from the call sites. */
extern void SzView_SetStraightLight(int unknown, int index, int value);
extern char *func_00197548(const char *label);
extern int lSzModel_FreeModel(int modelId);
extern void SzMem_Free(void *pointer);
extern int SzModel_Draw_Pipe(int cursor, int modelId, int c, void *d, int e, int f, int g);
/* The result holds an address (SzModel_GetCenterOfs reads floats through it);
 * node_walk.h declares this address with pointer types. */
extern DotrObjectWord lSzModel_TraceGNode(int targetNodeId, int base, int node, int *nodeCounter, int depth, int *maxDepth, int visit);
extern void SzView_ReqCamera(int a, int b, int c, int d, int index, int unknown);

/* 0x1ac610 initializes the object table flags and a few bytes of the storage at
 * 0x317250. The movie_controls.h prefix of that storage is a different C view,
 * so this one has its own symbol (gSzGame_gl_init) and is not assumed to be
 * the original object. data_003173b0 is assumed to be an int array. */
struct DotrInitBits2 {
    signed char lo : 2;
    signed char rest : 6;
};
struct DotrInitBits1 {
    signed char bit0 : 1;
    signed char bit1 : 1;
    signed char rest : 6;
};
struct DotrInitView {
    int word_000;
    int word_004;
    int word_008;
    unsigned char unknown_00c[0x218];
    unsigned char byte_224;
    unsigned char byte_225;
    unsigned char byte_226;
    unsigned char byte_227;
    unsigned char unknown_228[4];
    struct DotrInitBits2 bits_22c;
    unsigned char unknown_22d[2];
    struct DotrInitBits1 bits_22f;
};
extern struct DotrInitView gSzGame_gl_init;
extern int data_003173b0[16];
void lSzGame_InitTitle(int mode);

/* Packed-resource view used by SzModel_BindTextureFile: an object's +0x138 base word and
 * descriptor, whose table (count, 16-byte entries) is rebased to the base. */
struct DotrObjPackEntry { DotrObjectWord a; DotrObjectWord b; DotrObjectWord c; DotrObjectWord d; };
struct DotrObjPackDesc { unsigned char unknown_00[0x28]; DotrObjectWord unknown_28; DotrObjectWord unknown_2c; };
struct DotrObjPackTable { int count; int pad[3]; struct DotrObjPackEntry entries[1]; };
struct DotrObjPackView { unsigned char unknown_00[0x138]; DotrObjectWord base; struct DotrObjPackDesc *desc; };
/* Declared void* here: small_misc.h declares it with the packed header type
 * but conflicts with this header on SzModel_Preset. */
extern void RhText_BindAddress(void *header);
int SzModel_BindTextureFile(int index);

/* Resource-slot view of the gSzModel_ahModel objects used by lSzModel_LoadMotionCB:
 * eight rebase-head pointers at +0x178 and eight values at +0x1c0. A separate
 * symbol, not assumed to be the original object. small_misc.h cannot be
 * included alongside (redeclaration conflicts), so the head type is local. */
struct DotrSlotHead {
    unsigned char unknown_00[20];
    int first;
};
struct DotrResourceSlots {
    unsigned char unknown_000[376];
    struct DotrSlotHead *heads[8];
    int unknown_198[10];
    int values[8];
};
extern struct DotrResourceSlots *gSzModel_ahModel_slots[];
extern int NjaReloc_Adjust_M3DMOT(struct DotrSlotHead *head);
struct DotrResourceSubmitRecord;
/* Callback of lSzModel_LoadMotion (address passed to SzFile_ReadRequest). */
void lSzModel_LoadMotionCB(struct DotrResourceSubmitRecord *record, int completed);
/* Submit resource `name` for object type/slot; 1 if queued, 0 if rejected. */
int lSzModel_LoadMotion(int modelId, int slot, char *filePath);
/* Argument type and void return assumed: the only caller here passes 6 and
 * ignores v0. */
extern void SzGame_InitMemCardProc(int unknown);
/* No arguments read, no return value set. */
void lSzGame_InitMemCheck(void);

/* Views read by SzModel_SetStoneRatio alone (assumed layouts): the header behind
 * object +0x13c, the 16-byte slots after the count word at +0x138 plus the
 * header's +0x28, and each slot's node with its four words at +0x10. */
struct DotrObjectShadeHeader {
    unsigned char unknown_00[0x1c];
    int unknown_1c;
    unsigned char unknown_20[8];
    int unknown_28;
    int unknown_2c;
};

struct DotrObjectShadeNode {
    unsigned char unknown_00[0x10];
    struct DotrObjectShadeInfo {
        int unknown_00;
        int unknown_04;
        int unknown_08;
        int unknown_0c;
    } unknown_10;
};

struct DotrObjectShadeSlot {
    struct DotrObjectShadeNode *node;
    int unknown_04[3];
};

/* Three-argument view of SzColor_CLUT2SEP as SzModel_SetStoneRatio calls it: a float in
 * $f12, the halfword and 6. The result is stored as a halfword. */
extern int func_00241a00_shade(float scale, unsigned short color, int e);
/* value's low byte is the level compared; the byte stored is value's low byte too. */
int SzModel_SetStoneRatio(int index, int value);

/* Four-byte cue read by SzModel_ReqSE alone (assumed layout): a 4-bit kind
 * and 12-bit time in the first halfword, then a code. */
struct DotrObjectCue {
    unsigned short kind : 4;
    unsigned short time : 12;
    unsigned short code;
};
/* Views for SzModel_ReqSE: SzGame_GetFrameSpeed's result is converted as a signed
 * word without masking; SzSound_PlaySE is called with (channel, code) and its
 * result ignored (state_grid.h declares it void). */
extern int func_001a8760_int(void);
extern int func_00217710_cue(int index, int value);
extern int SzEffect_GetCount(int effectIndex);
/* Both parameters assumed int; channel is compared with 2 and 3 only. */
int SzModel_ReqSE(int channel, int modelId);

/* Second view of the 112-byte entries at EE 0x31ad00, for SzModel_ReqMotionBlur,
 * which fills one: four 16-byte rows copied from the object by 0x20b380,
 * two vectors and two bytes, two byte quads, two halfwords and, in the word
 * at +0x68, the signed 27-bit index, a 2-bit mode at bits 27-28 and a
 * 1-bit flag at bit 31 (the last two are accessed as bytes at +0x6b).
 * Field meanings are unknown. Host bit-field layout may differ. */
struct DotrObjectCopy112 {
    struct DotrObjectVector4 unknown_00[4];
    struct DotrObjectVector3 unknown_40[2];
    signed char unknown_58[2];
    struct DotrObjectQuad unknown_5a;
    struct DotrObjectQuad unknown_5e;
    unsigned short unknown_62;
    unsigned short unknown_64;
    signed int index : 27;
    unsigned char mode : 2;
    unsigned char unknown_bits : 2;
    signed char flag : 1;
    unsigned char unknown_6c[4];
};
#ifdef __MWERKS__
typedef char DotrObjectCopy112MustBe70[(sizeof(struct DotrObjectCopy112) == 0x70) ? 1 : -1];
#endif
extern struct DotrObjectCopy112 data_0031ad00_copy[];
extern void *RhMem_CopyMemory16(void *destination, void *source, int count);
/* 0x1edcb0: 0 unless object `modelId` exists with a +0x1f3 kind of 4 or 5 and
 * one of entries 1-31 has a negative index field. The first such entry gets
 * the object's two +0x198 vectors and +0x1e0 bytes, its first four rows
 * (0x20b380 with count 4), quad a at +0x5a and quad b at +0x5e (each
 * defaulting to the object's +0x12c quad; the a default also stores 0x80 at
 * +0x61, which quad b or the b default's final 0 then overwrites), the mode
 * (the object's low two +0x1f8 bits when negative), wFrame at +0x62, zero at
 * +0x64, the flag bit and the index; the result is 1. Parameter types are assumed from use. */
int SzModel_ReqMotionBlur(int modelId, int wFrame, int mode, struct DotrObjectQuad *a, struct DotrObjectQuad *b, int flag);

/* Declared as in pool_table.h; both are defined in other groups. */
extern void *SzMem_Alloc(int byte_count);
extern int *RhMem_FillMemory4(int *destination, int value, int count);
/* 0x1e7b20: 0 for an index outside 0-83 or when SzMem_Alloc(0x210) answers
 * null. Otherwise store the new object in the table, clear its 132 words
 * (0x20b400), set the starting values (ones in +0x30..+0x3c, 0.8, 0.8, 1.0
 * at +0x120, four 0x80 bytes at +0x12c, -1 at +0x1e1, +0x1ec, +0x1f2 and
 * +0x1f4, the two-bit fields at +0x1f7 and +0x1f8 to 1, the +0x20c bits to
 * 1, 1, 0, 0, 0, 0, 1, 1), call lSzModel_Clean(index) and answer 1. The target's
 * allocation size and word count equal this view's size there. */
int SzModel_Create(int modelId);

/* For every index 0..83: when targetModelType is negative, or equals SzModel_GetModelType(index),
 * calls SzModel_SetPauseFlag(index, value). Two int arguments, no result assumed. */
void SzEffect_SetModelPauseForType(int targetModelType, int value);
/* For indices 20..29 and then 30..31: a 0x1f3690 answer in 0..254 is raised by
 * one (at most 255) and passed, masked to a byte, to 0x1f34a0. No arguments
 * read; no result assumed. */
void SzEffect_StoneProc(void);
/* 0x26cd80: only the sign of the argument is used. */
void Ta109_BlackOutForExz(int direction);

/* 0x22a020. The 16-byte object at 0x29abb0 is copied to a local with one quadword move;
 * four words are assumed, only the second is rewritten (from the short at 0x1ed53f4). The
 * signed byte at 0x1ed53b3 is read through a base of exactly 0x1ed0000 kept in a register,
 * which the compiler emits for an integer-constant pointer; the host build uses a
 * test-supplied object instead. Nothing else about that range is claimed. */
struct DotrObjectVector16 {
    int unknown_words[4];
};
struct DotrObjectBank {
    char unknown_0000[0x53b3];
    signed char unknown_53b3;
};
extern struct DotrObjectVector16 data_0029abb0;
extern short data_01ed53f4;
#ifdef __MWERKS__
#define DOTR_OBJECT_BANK ((struct DotrObjectBank *)0x1ed0000)
#else
extern struct DotrObjectBank dotr_object_bank_01ed0000;
#define DOTR_OBJECT_BANK (&dotr_object_bank_01ed0000)
#endif
int Taku_Camp_STON(int cursor);

/* Texture-load view of the gSzModel_ahModel objects used by SzModel_LoadEffectTextureFile and its
 * callback lSzModel_LoadEffectTextureFileCB: a flag word at +0x138, one loaded-header word at
 * +0x140, eight more at +0x144, a kind byte at +0x1f3 and a number at +0x1f4.
 * A separate symbol, not assumed to be the original object; field names are
 * descriptive only. */
struct DotrLoadObject {
    unsigned char unknown_000[312];
    int modelBaseData;
    int unknown_13c;
    int main_part;
    int parts[8];
    unsigned char unknown_164[143];
    signed char kind;
    short number;
};
extern struct DotrLoadObject *gSzModel_ahModel_load[84];
/* Table of directory-name strings indexed by the kind byte (element type assumed). */
extern char *gSzModel_apchBase[];
/* "DBDIR", "%s/%s/TEX/M%%03d.rh2" and "%s/%s/TEX/B%%03d_%%d.rh2" in the image. */
extern char data_002cfa17[];
extern char data_002cfb4f[];
extern char data_002cfb64[];
extern char *SzFile_GetEnv(const char *label);
/* SDK-block formatter (sprintf-like; signature as in the other groups). */
extern int func_0019b648(char *destination, const char *format, ...);
/* 0x1ea0f0: submits the object's nine texture files; 0 for a bad index or
 * missing object, otherwise 1. */
int SzModel_LoadEffectTextureFile(int modelId);
/* Callback of SzModel_LoadEffectTextureFile: stores the loaded address through the word
 * pointer carried in unknown_01c and packs that header. */
void lSzModel_LoadEffectTextureFileCB(struct DotrResourceSubmitRecord *record, int completed);

/* SzView_PauseRequest (block_table.h has the defining group's declaration). */
extern int SzView_PauseRequest(int a, int b, int flag);
/* 0x1e1360, declared as this group calls it. */
extern int SzDuel_GetUnitModelID(int side, int pos);
/* 0x1f7260. Calls 0x1f7350 with model types 4, 5, 2, 3 in that order and the given
 * pauseFlagValue, then SzView_PauseRequest(-1, -1, pauseFlagValue). One int argument; no
 * result assumed (the last call's answer is left in v0). */
void SzEffect_SetPauseFor3DBattle(int pauseFlagValue);
/* 0x1f72c0. Splits cell into row (cell >> 8, arithmetic) and column (low byte);
 * answers 0 unless row is 0..2 and column 0..10, then 0 when SzDuel_GetUnitModelID(row, column)
 * is negative, otherwise SzModel_SetPauseFlag(that index, value). Two int arguments assumed. */
int SzEffect_SetModelPause2(int cell, int value);

/* 0x1e8e90. Views limited to what it touches: a record with an address word at
 * +0x14 and a word at +0x1c (object index in the low seven bits, bit 7 selecting
 * the second attachment), and the object's link pointer, two attachment words and
 * two halfwords. The object view is a cast of gSzModel_ahModel's entry, not another
 * symbol. */
struct DotrObjectAttachMessage {
    int unknown_00[5];
    DotrObjectWord unknown_14;
    int unknown_18;
    int unknown_1c;
};
struct DotrObjectAttachView {
    unsigned char unknown_000[0x138];
    struct DotrObjectLink *modelBaseData;
    unsigned char unknown_13c[0x30];
    DotrObjectWord pCameraMotion;
    DotrObjectWord pCameraMotion2;
    unsigned char unknown_174[0x8e];
    short cameraFrameCurrent;
    short cameraFrameTotal;
};
/* With no object for the index: nothing. With an object that has no link: the
 * record's address is released through 0x217b50 and `on` is treated as 0. With
 * on == 0 the selected attachment word is cleared. Otherwise the address is stored
 * there and one bit of the link's word +4 is cleared (0x100000 for the second,
 * 0x80000 for the first); the first also stores the halfword at +12 of the
 * attached data, minus one, to +0x204 and +0x202. Assumed: void result, int flag. */
void lSzModel_LoadCameraCB(struct DotrObjectAttachMessage *p, int on);

/* 0x1ed460: effect callback; for an id in 0-83 whose object exists, sets the word at 0x1ec
 * to -1. object_starts.h declares the same function without parameters because it only
 * takes its address; the int parameter is what the body reads. */
void lSzModel_EffectCB(int modelId);

/* Bind view of the gSzModel_ahModel objects used by lSzModel_BindModelByAddr
 * (0x1f0500): the packed model file at +0x138 and the pointers into it at +0x13c..+0x174, the
 * motion heads at +0x178 and their values at +0x1c0. Member names are this project's. The
 * 0x8000-byte blocks are stepped as 0x1000 eight-byte elements (what the code shows: the
 * advance is +8 then +0x7ff8); what the elements are is unknown. */
struct DotrBindHeader { int unknown_00; unsigned int flags; int unknown_08; int motion_offset; };
struct DotrBindSized { int unknown_00; int unknown_04; int size; };
struct DotrBindPair { int a; int b; };
struct DotrBindMotion { unsigned char unknown_00[20]; int *first; };
struct DotrBindObject {
    unsigned char unknown_000[0x138];
    struct DotrBindHeader *base;
    char *desc;
    struct DotrBindSized *text;
    struct DotrBindSized *texts[8];
    char *block_164;
    char *block_168;
    struct DotrBindSized *sized_16c;
    struct DotrBindSized *sized_170;
    char *block_174;
    struct DotrBindMotion *heads[8];
    int unknown_198[10];
    int values[8];
};
/* Declared void* here: packed_rebase.h declares it with DotrRebaseHeader. */
extern int RhModel_BindAddress(void *header);
/* lSzModel_LoadCB (model_load.h); argument and void return assumed from this caller. */
extern void lSzModel_LoadCB(int index);
/* Stores addr as object `modelId`'s model file, binds it and walks the blocks behind its motion
 * offset by the header's flag bits; 0 when the slot is empty, else 1. */
int lSzModel_BindModelByAddr(int modelId, struct DotrBindHeader *addr);

/* "%s/%s/%s.rh0" in the image (follows "DBDIR"). */
extern char data_002cfa1d[];
/* 0x1f0390, callback of lSzModel_LoadModelPack: when the read did not complete, or the data does
 * not start with the word 0x304852, requests the single model file "<DBDIR>/<kind>/<kind>.rh0"
 * instead (callback lSzModel_LoadModelCB); otherwise cleans the object and binds the data. The
 * record is declared in resource_submit.h; parameter types assumed from the callback type. */
struct DotrResourceSubmitRecord;
void lSzModel_LoadModelPackCB(struct DotrResourceSubmitRecord *record, int completed);

/* SzModel_DrawMotionBlur_Pipe (0x1edfd0): walks entries 31..1 of the 112-byte table through the
 * copy view, with entry 0 as the place the object's own values are kept during the draw. The
 * +0x62 / +0x64 halfwords are read unsigned there (total and elapsed frames of the fade).
 * Assumed: the cursor is an int as for SzModel_Draw_Pipe; SzModel_ExSort_Pipe as side_table.h
 * declares it; SzModel_SetDrawCache as small_misc.h declares it. */
struct DotrSideVector;
extern void SzModel_SetDrawCache(int v);
extern int SzModel_ExSort_Pipe(int cursor, int index, struct DotrSideVector *v, int a, int b, int c, int d);
int SzModel_DrawMotionBlur_Pipe(int cursor, int index);

/* 0x1e8780 SzModel_Load (name from the Japanese build's symbol table): loads model file
 * `modelNo` of class `modelType` into object `modelId` (0..83). Waits with SzFile_DoRequestS while
 * the object's busy bit is set, stores the class and number when they are not negative
 * (a negative number means the stored one), and returns 0 when the number is outside
 * 0..gSzModel_awIgnore[class] - 1. With `packed` and gSzModel_fPacking set and the label
 * data_002cf9d5 present: classes 4..6 read one megabyte per monster from a pack file (the
 * monster id comes from SzDuel_Xax2New, 737 maps to 671, and ids of 683 or more, the
 * monster count, are refused; the destination is a fixed megabyte slot per class from the
 * two triples below); class 1 reads entry 39 + n or 60 + n of anOfs_MODEL for numbers
 * 0..20 and 100..120. Everything else formats a file name from the class directory and
 * loads it through lSzModel_LoadModelPack (packed) or lSzModel_LoadModel. Returns 1 when
 * the request was made. The callers' headers declare it void; the result is not used
 * there. Array bounds below are not known. */
struct DotrObjectPackTriple {
    int v[3];
};
extern unsigned short gSzModel_awIgnore[];
extern int anOfs_MODEL[];
extern struct DotrObjectPackTriple data_00292f20; /* first object index of classes 4..6 */
extern struct DotrObjectPackTriple data_00292f30; /* first destination address of classes 4..6 */
extern char data_002cf9dd[];
extern char data_002cf9f0[];
extern char data_002cf9f5[];
extern char data_002cfa0a[];
extern int SzFile_DoRequestS(void);
extern int SzDuel_Xax2New(int index);
int SzModel_Load(int modelId, int modelType, int modelNo, int packed);

/* 0x1e7f70 SzModel_LoadMerge (name from the Japanese build): loads the shared model pack of
 * one of seven types. Nine model groups (class, count, first object index; copied from three
 * nine-entry tables) are selected by the type's mask in aShareData; the type's own group takes
 * its model numbers from a per-`no` word table. Unless `force` is set, the pack is requested
 * only when a selected object is standing by or a wanted number differs from the loaded one.
 * Assumed: int parameters and result, the element types from the access widths, the row
 * lengths of the word tables from the index scaling, the table copies as structure copies. */
struct DotrEntryTableEntry;
struct DotrModelShare {
    int addr;            /* destination address of the pack */
    unsigned short ofs;  /* first anOfs_MODEL entry of the type */
    unsigned short mask; /* groups to check and preset, one bit each */
};
struct DotrModelMergeWords {
    short *v[9];
};
struct DotrModelMergeInts {
    int v[9];
};
extern struct DotrModelShare aShareData[7]; /* aShareData$76 in the Japanese build */
extern struct DotrModelMergeWords data_00292be0; /* template; entries 0..6 are replaced */
extern struct DotrModelMergeInts data_00292c10;  /* class of each group */
extern struct DotrModelMergeInts data_00292c40;  /* objects in each group */
extern struct DotrModelMergeInts data_00292c70;  /* first object index of each group */
extern short gSzDuel_DuelData[][5];
extern short gSzDuel_RingData[][1];
extern short gSzDuel_StageData[][12];
extern short gSzDuel_EventData[][40];
extern short gSzDuel_FieldData[][10];
/* Views of SzModel_LoadMerge (they link by address): the five word tables as bytes (0x2cebc6 as
 * halfwords), the first-entry halfwords of aShareData from +4 (0x292ba4), anOfs_MODEL and its next
 * entry as two arrays (0x292a50/0x292a54), 0x1fecb0 SzFile_ReadRequestParam with integer name and
 * callback words and an int result, the flag byte at +0x20c of a model object as a bit-field
 * structure (bit 2 pending, bit 3 active), and integer addresses passed as words. On the host the
 * table names are the fixtures above and the address words are widened. Assumed: these types. */
#ifdef __MWERKS__
typedef int DotrMergeAddr;
extern unsigned char gSzDuel_DuelData_merge[];
extern short gSzDuel_RingData_merge[];
extern unsigned char gSzDuel_StageData_merge[];
extern unsigned char gSzDuel_EventData_merge[];
extern unsigned char gSzDuel_FieldData_merge[];
extern unsigned short data_00292ba4_merge[];
extern int anOfs_MODEL_merge[];
extern int data_00292a54_merge[];
#else
typedef long DotrMergeAddr;
#define gSzDuel_DuelData_merge ((unsigned char *)gSzDuel_DuelData)
#define gSzDuel_RingData_merge ((short *)gSzDuel_RingData)
#define gSzDuel_StageData_merge ((unsigned char *)gSzDuel_StageData)
#define gSzDuel_EventData_merge ((unsigned char *)gSzDuel_EventData)
#define gSzDuel_FieldData_merge ((unsigned char *)gSzDuel_FieldData)
#define data_00292ba4_merge (&aShareData[0].ofs)
#define anOfs_MODEL_merge anOfs_MODEL
#define data_00292a54_merge (anOfs_MODEL + 1)
#endif
/* MATCHING DEVICE: the flag bits are signed one-bit fields on the target (unsigned
 * fields do not match); the host uses unsigned fields so that storing 1
 * keeps its value. */
#ifdef __MWERKS__
typedef signed char DotrMergeBit;
#else
typedef unsigned char DotrMergeBit;
#endif
struct DotrModelMergeFlags {
    DotrMergeBit unused0 : 2;
    DotrMergeBit pending : 1;
    DotrMergeBit active : 1;
    DotrMergeBit unused4 : 4;
};
/* The flag byte at +0x20c of a model object, reached by its offset on the target; on the
 * host the field itself (host pointers move the offset). */
#ifdef __MWERKS__
#define DOTR_MERGE_FLAGS(obj) ((struct DotrModelMergeFlags *)((unsigned char *)(obj) + 0x20C))
#else
#define DOTR_MERGE_FLAGS(obj) ((struct DotrModelMergeFlags *)&(obj)->drawFlag)
#endif
extern int func_001fecb0_merge(int type, int priority, DotrMergeAddr name, int addr, int size, int offset, int size2,
                               DotrMergeAddr callback, int index);
extern struct DotrEntryTableEntry *SzText_GetItem(int group, int id);
extern int SzText_Free(int group, int id);
extern int SzDuel_DelUnit(int side, int unitArrayPos);
void lSzModel_LoadMergeCB(struct DotrResourceSubmitRecord *record, int completed);
int SzModel_LoadMerge(int modelType, int no, int force);

/* 0x1e90b0: submits two files per entry of the object's pack table (count and
 * entries at base + desc->unknown_28): the name built from "%s/%s/%%s" at
 * 0x2cfa30 with ".rh2" (0x2cfa3a) and ".anm" (0x2cfa3f) appended. Each request
 * carries the low 26 bits of the table's first entry address with the loop
 * counter in the top six bits; fields a, c and d of the entry are cleared.
 * Returns 0 for a bad index or empty slot, otherwise 1. The object's number is
 * passed where the format has %s, as the code shows; element types of
 * the strings and the callback parameter types are assumed. */
extern char data_002cfa30[];
extern char data_002cfa3a[];
extern char data_002cfa3f[];
/* SDK-block string append (strcat-like; signature as in resource_boot.h). */
extern char *func_0019b9d0(char *destination, const char *source);
void lSzModel_LoadTextureFileCB(struct DotrResourceSubmitRecord *record, int completed);
void lSzModel_LoadTexAnimFileCB(struct DotrResourceSubmitRecord *record, int completed);
int SzModel_LoadTextureFile(int modelId);

#endif

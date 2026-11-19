#ifndef DOTR_EE_MODEL_ACT_H
#define DOTR_EE_MODEL_ACT_H

/* SzModel per-frame model update (mode and debug viewer handling) and the reverse animation
 * interpolation setter, both on the model table at 0x31abb0.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/models.md. */

/* Field meanings and parameter types in the types, data declarations and callee prototypes
 * below are assumed. */

#ifndef NULL
#define NULL 0
#endif

struct DotrModelActVector4 {
    float x, y, z, w;
};
struct DotrModelActSlotInner {
    unsigned char pad00[4];
    int f4;
};
struct DotrModelActSlot {
    unsigned char pad00[0x14];
    struct DotrModelActSlotInner *f14;
};
/* The model table at 0x31abb0 holds 0x54 pointers to these. */
struct DotrModelActObject {
    unsigned char pad000[0x30];
    struct DotrModelActVector4 v;      /* 0x030 */
    unsigned char pad040[0x138];
    struct DotrModelActSlot *slots[18]; /* 0x178 */
    int vals[8];                       /* 0x1C0 */
    signed char lv[4];                 /* 0x1E0 */
};
extern struct DotrModelActObject *gSzModel_ahModel_modelact[];

/* Debug viewer controls for one object slot: the pad drives animation, camera, light and fog settings
   depending on the viewer page (SzDebug_GetActiveMenu, 0x205150). */

struct DotrModelActDbgFlags {
    unsigned char pad[4];
    int flags;
};

struct DotrModelActDigits4 {
    int d0, d1, d2, d3;
};

struct DotrModelActDigits3 {
    int d0, d1, d2;
};

struct DotrModelActDbgObject {
    float pos[3];                       /* 0x000 */
    unsigned char pad00C[0x14];
    int rot[3];                       /* 0x020 */
    unsigned char pad02C[4];
    struct DotrModelActVector4 v;                          /* 0x030 */
    unsigned char pad040[0xE0];
    float light[3];                     /* 0x120 */
    unsigned char col0[4];                       /* 0x12C */
    unsigned char col1[4];                       /* 0x130 */
    unsigned char pad134[4];
    struct DotrModelActDbgFlags *model;         /* 0x138 */
    unsigned char pad13C[8];
    int parts[8];                     /* 0x144 */
    unsigned char pad164[8];
    void *anim;                       /* 0x16C */
    unsigned char pad170[8];
    struct DotrModelActSlot *slots[8];       /* 0x178 */
    float speed;                        /* 0x198 */
    float frame;                        /* 0x19C */
    unsigned char pad1A0[0x20];
    unsigned int vals[8];                      /* 0x1C0 */
    signed char lv[4];                         /* 0x1E0 */
    unsigned char pad1E4[8];
    int sound;                        /* 0x1EC */
    unsigned char pad1F0[2];
    signed char page;                          /* 0x1F2 */
    signed char kind;                          /* 0x1F3 */
    unsigned char pad1F4[2];
    unsigned short num : 14;                     /* 0x1F6 */
    short loop : 2;
    unsigned char fog0 : 2;                      /* 0x1F8 */
    unsigned char fog1 : 2;
    unsigned char pad1F8 : 4;
    unsigned char pad1F9[5];
    unsigned char count;                         /* 0x1FE */
    unsigned char pad1FF[3];
    unsigned short f202;                         /* 0x202 */
    unsigned char pad204[2];
    unsigned short f206;                         /* 0x206 */
    unsigned short f208;                         /* 0x208 */
};

extern unsigned char iColF[4];
extern unsigned char iColT[4];
extern unsigned short wFrame;
extern int nCameraCurrent;
extern int nEffectCurrent;
extern struct DotrModelActDigits4 data_00292f90;
extern struct DotrModelActDigits4 data_00292fa0;
extern struct DotrModelActDigits3 data_00292fb0;
extern struct DotrModelActDigits3 data_00292fc0;
extern char data_002cfa17[];
extern char data_002cfb7d[];
extern int gSzModel_fRefLock_word;
extern int gSzModel_nRefLock_word;
extern unsigned short wAlphaMode;
extern int nBlurUse;
extern int nBlurCur;
extern int data_0031bb48;

#define DOTR_MODEL_ACT_PAD(m, b) SzPad_Check(0, m, b)

/* Callees (func_0019b648 is in the library block, unnamed; assumed a formatted print into a
 * buffer). */
int SzPad_Check(int arg0, int arg1, int arg2);
int func_0019b648(char *str, const char *fmt, ...);
int SzGame_GetFrameCount(void);
long long SzGame_GetMode(int arg0);
int SzDuel_SetUnitStatus(int side, int pos, int arg2, int arg3);
void lSzModel_LoadCameraCB(void *arg0, int arg1);
int SzModel_BattleSeq(int arg0, int arg1, int arg2);
int SzModel_SetAnim(int modelId, int animIndex);
int SzModel_SetAnimIntr(int modelId, int animIndex);
int SzModel_SetAnimDirect(int arg0, int arg1);
int SzModel_SetAnimDir(int modelId, long long animDir);
int SzModel_SetAnimSpeed(int arg0, float value);
int SzModel_SetIntensity(int modelId, float *intensity);
int SzModel_SetScale(int idx, void *src);
int SzModel_SetRgba(int arg0, int arg1, void *arg2);
int SzModel_SetFilter(int arg0, int arg1, void *arg2);
void SzEffect_Clear(void);
int SzEffect_EntryCallback(int moduleType, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);
int SzEffect_Destroy(int arg0);
int SzEffect_Destroy2(int arg0, int arg1);
int SzView_FreeRequest(int arg0, int arg1);
int SzView_ReqCamera(int owner, int target, int type, int count, int value, void *data);
int SzFile_InqRequest(int arg0);
int SzFile_DoRequestS(void);
unsigned char *SzFile_GetEnv(int unused0);
int SzText_AddFile(int group, int id, char *name, int srcImageAddress);
int SzText_FreeRequest(int textureFlags, int textureId);
int SzDebug_GetActiveMenu(void);
int SzDebug_GetActiveModel(void);
int SzDebug_IsHide(void);
void SzMem_Free(int unused0);

/* 0x1eaa60: per-frame update of model arg0 (0..0x53); 0 for an index out of range or an empty
 * slot, else 1. */
int SzModel_Act(int modelId);

/* 0x1f16c0: SzModel_SetAnimIntrReverseParam's two 12-byte vectors at slot words 8 and 11. */
struct DotrModelActVector3 {
    float x, y, z;
};
int SzModel_SetAnimIntrReverseParam(int modelId, int arg1);

#endif

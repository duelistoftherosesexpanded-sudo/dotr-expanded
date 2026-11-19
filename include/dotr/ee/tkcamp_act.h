#ifndef DOTR_EE_TKCAMP_ACT_H
#define DOTR_EE_TKCAMP_ACT_H

/* The campaign map's per-frame controller TkCamp_Act, which runs the map cursor, the spot menus and
 * the camp mode steps in the camp work area at 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* An address kept in an integer (the camp state's address, SzView_GetSP's result): a 32-bit
 * EE word on the target, pointer-sized on the host. */
#ifdef __MWERKS__
typedef int DotrTkCampWord;
#else
#include <stdint.h>
typedef intptr_t DotrTkCampWord;
#endif

/* A 16-byte quadword copied whole (lq/sq) and a four-float vector. Assumed: these views. */
#ifdef __MWERKS__
typedef __int128 DotrTkCampI128;
#else
__extension__ typedef __int128 DotrTkCampI128;
#endif
struct DotrTkCampQ {
    DotrTkCampI128 q;
};
struct DotrTkCampVec4 {
    float x;
    float y;
    float z;
    float w;
};

/* The camp state at 0x299e10 (camp_system.h: struct DotrCampSystemBoard). The function keeps
 * its address in an integer and reads bytes +0x0, +0xe, +0x13, +0xc6, +0xc9 and the halfword
 * +0xc2 by offset; the event number +0xc2 is also read as the separate symbol 0x299ed2.
 * The work area at the literal address 0x1ed0000 (lui/ori constants, not a relocated symbol)
 * is reached by its absolute addresses (up to 0x1ed5494).
 * MATCHING DEVICE: under the target compiler the two symbols are separate externs and AT(a)
 * is the literal address itself; on the host the two
 * symbols name the same fixture and AT(a) indexes a 0x5500-byte fixture of the work area.
 * Assumed: these types. */
#ifdef __MWERKS__
extern signed char data_00299e10_tkcamp;
extern short CutSceneNumber_tkcamp;
#define DOTR_TKCAMP_AT(a) (a)
#else
union DotrTkCampBoard {
    signed char bytes[0x100];
    short align;
};
extern union DotrTkCampBoard dotr_tkcamp_board;
#define data_00299e10_tkcamp (dotr_tkcamp_board.bytes[0])
#define CutSceneNumber_tkcamp (*(short *)(dotr_tkcamp_board.bytes + 0xc2))
union DotrTkCampWork {
    unsigned char bytes[0x5500];
    DotrTkCampI128 align;
};
extern union DotrTkCampWork dotr_tkcamp_work;
#define DOTR_TKCAMP_AT(a) (dotr_tkcamp_work.bytes + ((a) - 0x01ED0000))
#endif

/* Data: the map spot records at 0x298260 (19 halfwords each;
 * +0/+1 the spot position, +0..+3 words compared as a box), the two quadwords 0x29b000 /
 * 0x29b010 (model 0x1e position and rotation), and four debug format strings at 0x30f1f6,
 * 0x30f213, 0x30f227 and 0x30f244 (passed by address only). Assumed: these types. */
extern short data_00298260_tkcamp[];
extern struct DotrTkCampQ data_0029b000_tkcamp;
extern struct DotrTkCampQ data_0029b010_tkcamp;
extern signed char data_0030f1f6_tkcamp;
extern signed char data_0030f213_tkcamp;
extern signed char data_0030f227_tkcamp;
extern signed char data_0030f244_tkcamp;

/* Callees as views (they link by address; the trailing comments name them). 0x195070 is abs() in
 * the library block, called as a real call; 0x199b50 is a library printf-like routine; 0x1e3ff0
 * SzDuel_OpenOptionPopup is called with three words (no halfword narrowing at the call). Assumed:
 * these types and results. */
extern int func_00195070_tkcamp(int value);                                   /* abs */
extern void func_00199b50_tkcamp(const signed char *fmt, ...);                /* printf-like */
extern int func_001a8320_tkcamp(int a, int b);                                /* SzGame_SetMenuNum */
extern void func_001a8870_tkcamp(int a, int b);                               /* SzGame_Push */
extern void func_001a8920_tkcamp(void);                                       /* SzGame_Pop */
extern void func_001aafa0_tkcamp(int a);                                      /* SzGame_InitMemCardProc */
extern int func_001ab1d0_tkcamp(void);                                        /* SzGame_ActMemCardProc */
extern int func_001de1a0_tkcamp(int a);                                       /* SzDuel_New2Xax */
extern void func_001e3ff0_tkcamp(int a, int b, int c);                        /* SzDuel_OpenOptionPopup */
extern int func_001e40a0_tkcamp(int a);                                       /* SzDuel_ActOptionPopup */
extern void func_001e42b0_tkcamp(void);                                       /* SzDuel_CloseOptionPopup */
extern void func_001e42c0_tkcamp(int a, int b, int c, int d);                 /* SzDuel_OpenMsgPopup */
extern int func_001e4430_tkcamp(int a);                                       /* SzDuel_ActMsgPopup */
extern void func_001e4530_tkcamp(void);                                       /* SzDuel_CloseMsgPopup */
extern int func_001e8780_tkcamp(int a, int b, int c, int d);                  /* SzModel_Load */
extern int func_001f12a0_tkcamp(int a);                                       /* SzModel_IsModelLoading */
extern int func_001f22b0_tkcamp(int index, void *src);                        /* SzModel_SetPos */
extern int func_001f2430_tkcamp(int index, void *src);                        /* SzModel_SetRot */
extern int func_001f5240_tkcamp(int a, int b);                                /* SzEffect_IsUse2 */
extern DotrTkCampWord func_001f9ad0_tkcamp(int a);                            /* SzView_GetSP */
extern int func_001f9fe0_tkcamp(int a, void *b, void *c);                     /* SzView_SetCamera */
extern int func_00203a40_tkcamp(int pad, int kind, int mask);                 /* SzPad_Check */
extern int func_00203bc0_tkcamp(int pad);                                     /* SzPad_IsAnalog */
extern unsigned char func_00203c40_tkcamp(int pad, int axis);                 /* SzPad_GetAnalogInfo */
extern void func_00205cb0_tkcamp(void *out, float x, float y);                /* Ex3d_GetPointOnScreen */
extern void func_00206430_tkcamp(void *a, void *b, void *c, void *d);         /* Ex3d_IntersectP */
extern void func_00214990_tkcamp(int a, int b);                               /* SzData_SetupFromConstDeck */
extern int func_002158b0_tkcamp(int a);                                       /* SzData_GetCurrentDeck */
extern int func_002159d0_tkcamp(int a, int b);                                /* SzData_GetLeader */
extern int func_00217710_tkcamp(int a, int b);                                /* SzSound_PlaySE */
extern void func_00224a40_tkcamp(void);                                       /* Taku_Camp_Ivent_Name_Set */
extern void func_00226600_tkcamp(void);                                       /* Taku_Camp_System */
extern int func_0022d140_tkcamp(void);                                        /* Taku_Camp_BUN_HANTEI */
extern void func_0023b1f0_tkcamp(int a);                                      /* TkCamp_Act2 */
extern void func_0023bb00_tkcamp(int a);                                      /* Taku_Camp_Go_Rensyu_Duel */
extern void func_0023bc80_tkcamp(void);                                       /* Taku_Camp_Go_Help */
extern void func_00245d80_tkcamp(unsigned char *face);                        /* ShFace_Close */

/* 0x237730 TkCamp_Act (name from the Japanese build's symbol table): one frame of the camp
 * map. Assumed: no parameters and no result. */
void TkCamp_Act(void);

#endif

#ifndef DOTR_EE_BOOT_SEQUENCE_H
#define DOTR_EE_BOOT_SEQUENCE_H

/* Functions at 0x1001d0-0x100850 that call the start-up and shutdown entries of other groups in a
 * fixed order.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/main.md. */

/* Callees, declared here with the argument counts these callers pass. Results
 * and parameter types are assumed int unless a caller shows otherwise; other
 * group headers hold different views of some of them. */
extern int SzFile_Init(void);
extern int RhBase_Init(int, int, int, int, int, int, int);
extern int RhBase_SetClearColorEx(int, int, int, int);
extern int SzGame_ClearGsMem(int, int, int, int, int, int);
extern int SzText_Init(void);
extern int SzPad_Init(void);
extern int SzSound_Init(void);
extern int SzModel_Init(void);
extern int SzView_Init(void);
extern int SzFade_Init(void);
extern int SzGame_Init(void);
extern int SzEffect_Init(void);
extern int SzDebug_Init(void);
extern int SzPad_SetParam(int, int, int);
extern int SzView_Create(int);
extern int SzMsg_Initialize(void);
extern int SzData_Initialize(void);
extern int SzDuel_Initialize(void);
extern int SzBattle_Initialize(void);
extern int lMain_InitModel2(void);
extern int SzGame_SetMode(int, int);
extern int SzFade_Entry(int, int, int, int, int, int);
extern int SzFade_SetKillFlag(int);
extern int SzFade_SetPauseFlag(int, int);
extern int lMain_LoadModel(void);
/* Main_Loop's callees: RhBase_SetVSyncCallback (0x209f60, through the `_entry` view) receives
 * the address of SzEffect_StoneProc (0x1f73c0, viewed here without arguments; its own header
 * gives it an int one); SzGame_GetMode (0x1aaf60) takes one int. */
extern void SzEffect_StoneProc(void);
extern int func_00209f60_entry(void (*entry)(void));
extern int SzGame_GetMode(int);
extern int SzView_Destroy(int);
extern int SzModel_Destroy(int);
extern int RhBase_Term(void);
extern int *SzPad_Term(void);
extern int SzPad_Status(void);
extern int SzSound_VSync(void);
extern int SzFile_DoRequest(void);
extern int SzGame_ModeAct(void);
extern int SzView_DoRequest(int, int);
extern int SzDuel_DoUnitRequest(void);
extern int Main_Draw(void);
extern int RhBase_FlipBuffer(void);
extern int SzGame_SetFrameSpeed(int);
extern int SzGame_GetStabilizer(void);
extern int SzGame_SetStabilizer(int);
extern int SzGame_AddFrameCount(void);
extern int SzPad_Act(void);
extern int func_0019a5c8(void);
extern int SzPad_VSync(void);
extern int SzGame_VSync(void);

/* Start-up: returns 1 when every step succeeded, 0 at the first failed one. */
int Main_Init(void);
/* Destroys view 0 and models 0..83, calls RhBase_Term and returns SzPad_Term's
 * pointer. */
int *Main_Term(void);
/* One frame of the main loop: pad status, sound, file and game-mode processing,
 * view and unit requests, drawing, then the buffer flip, whose result becomes the
 * frame speed (forced to 1 when SzGame_GetStabilizer is non-zero; otherwise the
 * stabilizer is set), Main_VSync, the frame count and SzPad_Act. */
void Main_LoopProc(void);
/* Passes SzEffect_StoneProc (0x1f73c0) to RhBase_SetVSyncCallback, then loops forever:
 * lMain_LoadModel when SzGame_GetMode(1) is 1, then Main_LoopProc. Never returns. */
void Main_Loop(void);
/* Three calls, then 0 as a 64-bit result (the code clears the full register). */
long Main_VSync(void);

#endif

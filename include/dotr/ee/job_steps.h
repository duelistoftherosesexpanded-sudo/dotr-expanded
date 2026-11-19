#ifndef DOTR_EE_JOB_STEPS_H
#define DOTR_EE_JOB_STEPS_H

/* Functions taking (request, queue, mode) that build a 999-tagged four-word local, as
 * Ta_Eff_034 in small_misc does; mode >= 0 starts the step, mode < 0 polls it.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Request, queue and local views (placeholder names); offsets observed in 0x2755d0, member
 * names and widths assumed. The queue view differs from small_misc's DotrMiscJobQueue. */
typedef struct DotrJobStepRequest {
    int unknown_00;
    unsigned short first;
    unsigned short unknown_06;
    unsigned short second;
    unsigned short unknown_0a;
    unsigned int flags;
} DotrJobStepRequest;
typedef struct DotrJobStepQueue {
    unsigned short first;
    unsigned short second;
    unsigned char unknown_04[0xfe];
    unsigned short count;
} DotrJobStepQueue;
typedef struct DotrJobStepLocal {
    int unknown_00;
    int first;
    int second;
    int value;
} DotrJobStepLocal;

/* Callees are defined in other groups (state_grid, preset_words, state_queries, cell_tints,
 * small_misc); argument types here are assumed. Ta_Eff_BTL_002 is this group's function. */
extern void SzDuel_CloseMsgPopup(void);
extern void InitEffectTemp(DotrJobStepQueue *q);
extern int Ta_GetCurrentSide(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int Ta_IsLDAbl(int ability, int side);
extern void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int visualType);
extern void Ta_UnitFade(int side, int pos, int dir, int value, int flags);
extern int Ta_DelUnit(int side, int pos);
int Ta_Eff_BTL_002(DotrJobStepRequest *in, DotrJobStepQueue *q, int mode);

#endif

#ifndef DOTR_EE_VIEW_ACT_CAMERA_H
#define DOTR_EE_VIEW_ACT_CAMERA_H

/* SzView_ActCamera at 0x1fcab0 (debug pad control of the requested camera frame and of a unit
 * model's animation).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/camera.md. */

/* Globals with the types the code shows (a word tested and passed on, a 16-bit
 * frame count); other groups declare the same names as arrays. */
extern int gSzView_pCamera;
extern unsigned short gSzView_wFrame;
/* Callees: argument lists as this caller passes them (assumed types). */
extern int SzPad_Check(int unknown, int zero, int mask);
extern void SzView_ReqCamera(int a, int b, int c, int d, int index, int data);
extern int SzBattle_InqEntry(int index);
extern int SzDuel_GetUnitModelID(int side, int pos);
extern int SzModel_SetAnimIntr(int modelId, int animIndex);

/* 0x1fcab0: does nothing while gSzView_pCamera is zero; otherwise reads the pad
 * (see docs/modules/camera.md). No arguments or result are observed. */
void SzView_ActCamera(void);

#endif

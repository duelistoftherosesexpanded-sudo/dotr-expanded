#ifndef DOTR_EE_VIEW_ACT_H
#define DOTR_EE_VIEW_ACT_H

/* SzView_Act (0x1f7790): debug pad editing of the active view's lights, fog ranges and radius, then
 * pad-driven camera rolls.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/camera.md. */

/* One straight light as the debug editor touches it: three rotation floats at +4 and two
 * integer offsets at +0x10/+0x14, 0x30 bytes apart. Assumed: the meaning of +0x10/+0x14
 * (stepped by 0x100 with the pad). */
struct DotrViewActLight {
    int unk0;
    float rotation[3];
    int x;
    int y;
    unsigned char pad18[0x18];
};

/* Ambient colour bytes (r, g, b at +0..+2). */
struct DotrViewActColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

/* A fog range: near at +0, far at +4. */
struct DotrViewActRange {
    float near;
    float far;
};

/* The view block (gSzView_hView[index]) as this function reads it: radius at +0x44,
 * lights at +0x60 (0x90 bytes per model type, three lights each), two range tables at
 * +0x7e0 and +0x830 (8 bytes per type), colours at +0x880, the three per-type enable
 * masks at +0x8a8/+0x8ac/+0x8ae and the camera mode at +0x8b0. Assumed field meanings
 * from their use here. */
struct DotrViewActState {
    unsigned char pad0[0x44];
    float radius;
    unsigned char pad48[0x860];
    unsigned short lightFlags;
    unsigned short pad8AA;
    unsigned short fogFlags;
    unsigned short secondFlags;
    int mode;
};

/* The 16-byte default light direction at 0x293050, copied whole (lq/sq). */
#ifdef __MWERKS__
typedef __int128 DotrViewActQuad;
#else
__extension__ typedef __int128 DotrViewActQuad;
#endif
struct DotrViewActReset {
    DotrViewActQuad value;
};

/* gSzView_hView (0x31beb0, name from the Japanese build's symbols) holds the view blocks'
 * addresses as words, as block_table.h's views of it do; a host pointer needs a wider word. */
#ifdef __MWERKS__
typedef int DotrViewActWord;
#else
typedef long DotrViewActWord;
#endif
extern DotrViewActWord gSzView_hView_act[];
extern struct DotrViewActReset data_00293050_act;
/* 0x31c040: the light index being edited (nCurLight, a function-local static in the
 * Japanese build), stepped modulo 3. */
extern int nCurLight_act;

/* Callees, defined in small_state, movie_controls, object_table and block_table. Assumed:
 * SzGame_GetMode's 64-bit result (kept as long long; only its comparisons with 0, 5 and 12 are
 * used here),
 * SzPad_Check (port, kind, mask) non-zero on a matching button, SzPad_GetPressInfo a pressure
 * reading. */
extern int SzDebug_GetActiveMenu(void);
extern long long SzGame_GetMode(int a);
extern int SzDebug_GetActiveModel(void);
extern int SzModel_GetModelType(int modelId);
extern int SzPad_Check(int port, int kind, int mask);
extern int SzView_SetStraightLightDirection(int index, int position, int part, struct DotrViewActReset *in);
extern int SzView_SetStraightLightColor(int index, int position, int part, void *in);
extern int SzView_GetMode(int index);
extern int SzView_SetMode(int index, int value);
extern void *SzView_InqRequest(int a, int b);
extern int SzView_SetViewCamera(int index, int model, float *target, int mode, float weight);
extern int SzPad_IsPress(int port);
extern unsigned int SzPad_GetPressInfo(int port, int button);
extern int SzView_RollCamera(int index, int nested, int dir, float yaw, float pitch, float dist);
extern int SzDebug_IsHide(void);

/* 0x1f7790 SzView_Act (name from the Japanese build's symbols): returns 0 for an index other
 * than 0 or an empty view slot, else 1. */
int SzView_Act(int index);

#endif

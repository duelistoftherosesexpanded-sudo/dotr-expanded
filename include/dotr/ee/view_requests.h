#ifndef DOTR_EE_VIEW_REQUESTS_H
#define DOTR_EE_VIEW_REQUESTS_H

/* SzView camera request runner and vector conversion from the view module, called through the view
 * table at 0x31beb0.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/camera.md. */

/* Types of SzView_DoRequest, assumed from their use: a
 * camera's polar, rotation, position and target vectors, then a roll word and a field-of-view
 * float; a 48-byte camera request slot with two one-bit flags (active, paused), its type (0-9),
 * owner and target camera, frame count and timer, and two values (an int, a pointer or a vector)
 * to interpolate between. */
struct DotrViewReqVector4 {
    float x, y, z, w;
};
struct DotrViewReqCamera {
    struct DotrViewReqVector4 polar;
    struct DotrViewReqVector4 rotation;
    struct DotrViewReqVector4 position;
    struct DotrViewReqVector4 target;
    int roll;
    float fov;
};
union DotrViewReqValue {
    int i;
    void *p;
    struct DotrViewReqVector4 v;
};
struct DotrViewReqSlot {
    signed char active : 1;
    signed char type : 7;
    signed char flag : 1;
    signed char other : 7;
    signed char owner;
    signed char target;
    unsigned short count;
    unsigned short timer;
    unsigned char pad08[8];
    union DotrViewReqValue from;
    union DotrViewReqValue to;
};

#ifndef NULL
#define NULL 0
#endif

/* The view (camera) table at 0x31beb0 (gSzView_hView), a pointer view so that the host can hold
 * addresses; each element is cast to the camera type. */
extern void *gSzView_hView_viewreq[];
/* Default camera position and target (0x293020 / 0x293030). */
extern struct DotrViewReqVector4 gSzView_pntSPDef;
extern struct DotrViewReqVector4 gSzView_pntCPDef;

/* Callees, all defined in block_table. */
int lSzView_CalcRel(int arg0);
int lSzView_CalcLook(int arg0);
int SzView_SetLookSP(int index, float *src);
int SzView_SetLookCP(int index, float *src);
int SzView_SetViewCamera(int index, int otherIndex, float *target, int mode, float weight);
int SzView_SetInterpCameraMotion(int index, void *motion, int duration, int frame, int parent, int mirror);

/* 0x1fa220: advances every active, unpaused request of owner / target (negative: any). */
void SzView_DoRequest(int owner, int target);
/* 0x1fc890: float vector to int vector, the middle two components as angles. */
void SzView_V2IV(float *src, int *dst);

#endif

#ifndef DOTR_EE_VIEW_REQUEST_H
#define DOTR_EE_VIEW_REQUEST_H

/* SzView_ReqCamera: queues a timed camera change (field of view, roll, direction, position, polar
 * angles, a track) for view 0 in the first free of eight request slots.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/camera.md. */

struct DotrViewReqVec {
    float x;
    float y;
    float z;
    float w;
};

/* The view block as this function reads it: polar angles at +0, direction at +0x20, position
 * at +0x30, roll at +0x40, field of view at +0x44. Assumed: the meaning of each field (taken
 * from the request types that copy them). */
struct DotrViewReqCamera {
    struct DotrViewReqVec polar;
    struct DotrViewReqVec rotation;
    struct DotrViewReqVec direction;
    struct DotrViewReqVec position;
    int roll;
    float fov;
};

/* A request's start and end value: an integer, a track pointer or a vector. */
union DotrViewReqValue {
    int i;
    void *p;
    struct DotrViewReqVec v;
};

/* One of the eight 0x30-byte request slots at 0x31bec0 (as SzView_InqRequest and
 * SzView_PauseRequest in block_table read them): active bit and type in the first byte, a flag
 * bit and a second field in the next, owner and target bytes, the frame count and the timer,
 * then the start and end values at +0x10 and +0x20. Assumed: field meanings. On the host the
 * slot is wider (pointer alignment); only the fixture uses it there. The active bit is a signed
 * one-bit field on the target (it reads back as -1); the host declares it unsigned,
 * because storing the constant 1 into a signed one-bit field is a host compiler error here.
 * Only its zero test is read. */
#ifdef __MWERKS__
typedef signed char DotrViewReqActiveBit;
#else
typedef unsigned char DotrViewReqActiveBit;
#endif
struct DotrViewReqSlot {
    DotrViewReqActiveBit active : 1;
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

/* A camera track (types 8 and 9): its frame count at +0xc. */
struct DotrViewReqTrack {
    unsigned char pad00[0xC];
    unsigned short frames;
};

/* gSzView_hView (0x31beb0, name from the Japanese build's symbols): the view blocks' addresses
 * as words, as view_act.h's and block_table.h's views have them; a host pointer needs a wider
 * word. */
#ifdef __MWERKS__
typedef int DotrViewReqWord;
#else
typedef long DotrViewReqWord;
#endif
extern DotrViewReqWord gSzView_hView_req[];
extern struct DotrViewReqSlot gSzView_CameraReq_req[];

/* 0x1fa8a0 SzView_ReqCamera (name from the Japanese build's symbols). Assumed: parameter
 * meanings (view, target, request type 0..9, frame count, integer end value, vector or track). */
int SzView_ReqCamera(int owner, int target, int type, int count, int value, void *data);

#endif

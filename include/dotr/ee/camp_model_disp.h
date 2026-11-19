#ifndef DOTR_EE_CAMP_MODEL_DISP_H
#define DOTR_EE_CAMP_MODEL_DISP_H

/* Draws a model into a render frame and sorts that frame as a sprite on the campaign map.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

#include "dotr/ee/battle_fakes.h"

/* 0x21dff0 Taku_Camp_Teki_Model_Disp (name from the Japanese build's symbol table). Renders
 * model `modelId` into render frame (0, 10, 49) through a view placed `dist` in front of the
 * model's centre height, looking along +z, then sorts that frame as a sprite of w x h at
 * (x - 320, y - 224) with the given alpha: the texture's middle strip, w * 448 / h texels wide,
 * 448 high. The packet is the 88-byte sprite packet of cursor_packets.h. Returns the cursor
 * unchanged when RhView_PushStack refuses. Assumed: the view block as four rows, an eye and a
 * direction vector and a roll at +0x80 (view_state.h has it as nine 16-byte words); the
 * callees' prototypes, taken from this call site (other headers declare the same addresses with
 * other types, hence the view names). */
struct DotrCampModelView {
    float m[16];
    float eye[4];
    float dir[4];
    float unknown_60[8];
    float roll;
    float unknown_84[3];
};
struct DotrCampModelVector {
    float x, y, z, w;
};
extern int func_0020def0_campmodel(struct DotrCampModelView *out);   /* RhView_GetCurView */
extern int func_0020e460_campmodel(void);                             /* RhView_PushStack */
extern int func_0020e4f0_campmodel(void);                             /* RhView_PopStack */
extern int func_0020e360_campmodel(struct DotrCampModelView *view);   /* RhView_Modify */
extern int func_0020dfd0_campmodel(float a, float b);                 /* RhView_SetClipZ */
extern int func_001f2050_campmodel(int index, struct DotrCampModelVector *out);   /* SzModel_GetCenter */
extern int func_001f30b0_campmodel(int index, int value);             /* SzModel_SetDrawFlag */
extern DotrDrawWord func_001ee4b0_campmodel(DotrDrawWord cursor, int index, int c, int d, int e, int f, int g);   /* SzModel_Draw_Pipe */
extern int func_001ed4a0_campmodel(int index);                        /* SzModel_Motion */
DotrDrawWord Taku_Camp_Teki_Model_Disp(DotrDrawWord cursor, float dist, int modelId, int x, int y, int w, int h, int alpha);

#endif

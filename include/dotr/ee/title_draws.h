#ifndef DOTR_EE_TITLE_DRAWS_H
#define DOTR_EE_TITLE_DRAWS_H

/* Title-screen draw routines (lSzGame_DrawTitle_*) that render a model into the off-screen frame
 * and sort it as one textured rectangle; callees in draw_state, view_state, object_table and
 * cursor_packets.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/cursor_packets.h"
#include "dotr/ee/view_state.h"

/* 0x1ad670 lSzGame_DrawTitle_Rose_Pipe (original name). Assumed: the 144-byte view block as
 * nine 128-bit words (view_state.h) overlaid with the floats this function stores; the
 * 88-byte packet local of SzMsg_DrawRect_Pipe (8 bytes before the header, the two texture
 * words after the vertices filled by RhBase_GetTextureInfoForRender, which draw_state.h
 * declares with its own bit-field types); the render-frame functions with the cursor as
 * DotrDrawWord (draw_state.h has long pointers); SzModel_DrawWithOutCalc_Pipe as in
 * object_table.h with the cursor as DotrDrawWord; the byte at 0x317476 as the alpha. */
struct DotrTitleViewBlock {
    union {
        DotrViewWord128 words[9];
        float values[36];
    } u;
};
extern unsigned char data_00317476;
int RhBase_AssignRenderFrame(int index, int mode, int kind);
DotrDrawWord RhBase_EnableRenderFrame(DotrDrawWord p);
DotrDrawWord RhBase_ClearRenderFrame(DotrDrawWord p, void *color);
DotrDrawWord RhBase_DisableRenderFrame(DotrDrawWord p);
int RhBase_GetTextureInfoForRender(struct DotrCursorTex *a, struct DotrCursorTex *b);
void RhBase_FreeRenderFrame(void);
DotrDrawWord SzModel_DrawWithOutCalc_Pipe(DotrDrawWord cursor, int modelId, int c, void *d, int e, int f, int g);
DotrDrawWord lSzGame_DrawTitle_Rose_Pipe(DotrDrawWord cursor, int index, int x, int y, int width, int height, float z);

/* 0x1b3790 lSzDuel_DrawWindow_FieldPiece_Pipe (original name). Assumed: the flat packet as
 * the 8 bytes before the header, the header, colour, 1.0 and two position words;
 * gSzDuel_aaicolRgba (original name, 128 bytes) as rows of eight bytes; the 16 bytes at
 * 0x2912d0 as four words passed to SzModel_DrawWithOutCalc_Pipe, the second taken from
 * the word at 0x31ab80; the intensity as three floats (DotrObjectVector3 in
 * object_table.h); the control block at 0x317490 read only at +3160, the same word as
 * data_003180e8; the remaining callees with the cursor as DotrDrawWord. */
struct DotrTitleIntensity {
    float x, y, z;
};
struct DotrTitleFlatPacket {
    long unknown_m8;
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrCursorSlotXyz xyz[2];
};
struct DotrTitleRotation {
    int word[4];
};
extern unsigned char data_00317490_bytes[];
extern unsigned char gSzDuel_aaicolRgba[][8];
extern struct DotrTitleRotation data_002912d0;
extern int RotY;
extern int SzModel_IsModelDisp(int modelId);
extern int SzModel_GetIntensity(int modelId, struct DotrTitleIntensity *intensity);
extern int SzModel_SetIntensity(int modelId, struct DotrTitleIntensity *intensity);
extern float func_001a6c60(float value);
DotrDrawWord lSzDuel_DrawWindow_FieldPiece_Pipe(DotrDrawWord cursor, int colour, int field, float f, int x, int y);

/* 0x1bd8a0 lSzDuel_DrawBuildDeck_GhostCard_Pipe (original name). The packet and render-frame
 * calls of lSzGame_DrawTitle_Rose_Pipe, with the build-deck card drawer as the renderer.
 * lSzDuel_DrawBuildDeck_Card_Pipe (0x1bce20, build_deck_card): ASSUMED six arguments (cursor,
 * id, x, y and two words) from this call, which passes (cursor, id, 0, 0, flag, 0); the roles
 * of id and flag are assumed from their positions. alpha is stored as the fourth colour
 * byte; the depth word 0x3180e8 is data_003180e8 as cursor_packets.h declares it. */
extern DotrDrawWord lSzDuel_DrawBuildDeck_Card_Pipe(DotrDrawWord cursor, int id, int x, int y, int a4, int a5);
DotrDrawWord lSzDuel_DrawBuildDeck_GhostCard_Pipe(DotrDrawWord cursor, int id, int x, int y, int flag, int alpha);

#endif

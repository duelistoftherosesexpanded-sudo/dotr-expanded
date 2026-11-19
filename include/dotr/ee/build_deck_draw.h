#ifndef DOTR_EE_BUILD_DECK_DRAW_H
#define DOTR_EE_BUILD_DECK_DRAW_H

/* The deck editor's per-frame screen drawer that composes the build-deck panes, detail views and
 * mode captions through the window pipe.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The control-block view (DotrCaptionControl on data_00317490_caption, extended there with the
 * members this drawer reads) and the caption drawers it calls (lSzDuel_DrawBuildDeck_Title_Pipe
 * and the seven mode captions, defined in dialog_captions) come from that group's header. */
#include "dotr/ee/dialog_captions.h"

/* Callees as 0x1baea0 calls them, all taking and returning the pipe cursor as DotrCaptionWord.
 * Argument counts are from the call sites; meanings are assumed. Declared in other groups with
 * other cursor types: RhBase_AssignRenderFrame, RhBase_EnableRenderFrame,
 * RhBase_ClearRenderFrame, RhBase_DisableRenderFrame, RhBase_MaskFBuffer and
 * RhBase_FreeRenderFrame (draw_state), SzDuel_StartWindowSessionWithPalette_Pipe (frame_setup),
 * lSzDuel_DrawBuildDeck_Base_Pipe (cursor_packets), lSzDuel_DrawBuildDeck_Ghost_Pipe
 * (blend_slots), SzData_GetBagCount and SzData_GetLookFlag (side_table), SzMsg_Free (slot_table),
 * and the Library, Detail, Bag, Deck, BagCursor and DeckCursor panes (deck_detail, window_panes,
 * deck_list_pipe, scaled_sprite). Assumed: AssignRenderFrame(index, mode, kind); the clear colour
 * pointer (0 here); the window session palette (2); the frame-buffer mask; Detail's eight
 * arguments as (cursor, card id, x, then five flags/positions passed as constants or the mode
 * tests); the cursor panes' last argument as the list cursor byte. */
extern int RhBase_AssignRenderFrame(int index, int mode, int kind);
extern DotrCaptionWord RhBase_EnableRenderFrame(DotrCaptionWord cursor);
extern DotrCaptionWord RhBase_ClearRenderFrame(DotrCaptionWord cursor, void *color);
extern DotrCaptionWord RhBase_DisableRenderFrame(DotrCaptionWord cursor);
extern DotrCaptionWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrCaptionWord cursor, int a1);
extern DotrCaptionWord RhBase_MaskFBuffer(DotrCaptionWord cursor, unsigned int mask);
extern DotrCaptionWord lSzDuel_DrawBuildDeck_Base_Pipe(DotrCaptionWord cursor);
extern DotrCaptionWord lSzDuel_DrawBuildDeck_Library_Pipe(DotrCaptionWord cursor, int x, int y);
extern int SzData_GetBagCount(int side, int cardNo, int kind);
extern int SzData_GetLookFlag(int side, int id);
extern DotrCaptionWord lSzDuel_DrawBuildDeck_Detail_Pipe(DotrCaptionWord cursor, int card, int x, int a, int b, int c, int d,
                                                         int e);
extern DotrCaptionWord lSzDuel_DrawBuildDeck_Bag_Pipe(DotrCaptionWord cursor, int x, int y);
extern DotrCaptionWord lSzDuel_DrawBuildDeck_Deck_Pipe(DotrCaptionWord cursor, int x, int y);
extern DotrCaptionWord lSzDuel_DrawBuildDeck_Ghost_Pipe(DotrCaptionWord cursor, int x, int y);
extern DotrCaptionWord lSzDuel_DrawBuildDeck_BagCursor_Pipe(DotrCaptionWord cursor, int x, int y, int cur);
extern DotrCaptionWord lSzDuel_DrawBuildDeck_DeckCursor_Pipe(DotrCaptionWord cursor, int x, int y, int cur);
extern void SzMsg_Free(int index);
extern void RhBase_FreeRenderFrame(void);

/* 0x1baea0 SzDuel_DrawBuildDeck_Pipe: draws one frame of the deck editor and returns the
 * cursor (see docs/modules/duel.md). */
DotrCaptionWord SzDuel_DrawBuildDeck_Pipe(DotrCaptionWord cursor);

#endif

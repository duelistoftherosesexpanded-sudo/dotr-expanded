#include "dotr/ee/card_scene.h"

/* Card pass of the duel scene. Returns the cursor unchanged unless SzGame_GetMode(1) is 0
 * or 1 and SzModel_GetDrawFlag(0) is set. Otherwise: sets an anchor, starts the card session,
 * draws the card requests, then with the Z buffer mode 1 both leader bases (unit position
 * of side 0 and 1 with y cleared, alpha 128, flag 1) and mode 0 again. When the control
 * block has a moving card with its active bit set, its position is the camera point
 * (state 29 and a pending view request) or the current player's cell, with y 1.0; kind 0
 * draws a leader base (alpha 64, flag 0), otherwise a copy of the card's block with the
 * alpha scaled by 64/128 and that position is set up (with the card, or 0 when
 * SzDuel_GetCardNode gives 0) and drawn. Ends with the hand cards, the session finish and
 * ExSortAnchor_Pipe on the entry cursor with ExGetMyOT() and 0xffffff. */
DotrCardSceneWord SzDuel_DrawAllCard_Pipe(DotrCardSceneWord cursor)
{
    struct DotrCardSceneVector pos;
    struct DotrCardSceneBlock block;
    struct DotrCardSceneControl *control = &data_00317490;
    struct DotrCardScenePlayer *player = &data_00317490.player[turnOwner[0]];
    DotrCardSceneWord anchor;
    struct DotrCardSceneItem *item;
    int request;
    int mode;

    mode = SzGame_GetMode(1);
    anchor = cursor;
    if (mode != 0 && mode != 1) {
        return cursor;
    }
    if (SzModel_GetDrawFlag(0) == 0) {
        return cursor;
    }
    cursor = ExSetAnchor_Pipe(cursor);
    cursor = SzDuel_StartCardSession_Pipe(cursor);
    cursor = SzDuel_DrawCardRequest_Pipe(cursor);
    cursor = RhBase_SetZBufferMode(cursor, 1);
    SzDuel_GetUnitPos(0, 0, &pos);
    pos.y = 0.0f;
    cursor = SzDuel_DrawLeaderBase_Pipe(cursor, 0, &pos, 128, 1);
    SzDuel_GetUnitPos(1, 0, &pos);
    pos.y = 0.0f;
    cursor = SzDuel_DrawLeaderBase_Pipe(cursor, 1, &pos, 128, 1);
    cursor = RhBase_SetZBufferMode(cursor, 0);
    if (control->item != 0) {
        if (control->item->active) {
            request = 0;
            if (control->state == 29 && SzView_InqRequest(-1, -1) != 0) {
                request = 1;
            }
            if (request) {
                pos = *SzView_GetCP(0);
            } else {
                SzDuel_CalcUnitPos(player->x, player->y, 0, &pos);
            }
            pos.y = 1.0f;
            item = control->item;
            if (item->kind == 0) {
                cursor = RhBase_SetZBufferMode(cursor, 1);
                cursor = SzDuel_DrawLeaderBase_Pipe(cursor, control->item->leader, &pos, 64, 0);
                cursor = RhBase_SetZBufferMode(cursor, 0);
            } else if (item->block != 0) {
                block = *item->block;
                block.color[3] = (block.color[3] << 6) >> 7;
                block.pos = pos;
                cursor = SzDuel_SetupCard2_Pipe(cursor, SzDuel_GetCardNode(&block, 1) ? item : 0, &block);
                cursor = RhBase_SetZBufferMode(cursor, 1);
                cursor = SzDuel_DrawCard_Pipe(cursor, &block);
                cursor = RhBase_SetZBufferMode(cursor, 0);
            }
        }
    }
    cursor = SzDuel_DrawHandCard_Pipe(cursor);
    cursor = SzDuel_FinishCardSession_Pipe(cursor);
    return ExSortAnchor_Pipe(cursor, anchor, ExGetMyOT(), 0xffffff);
}

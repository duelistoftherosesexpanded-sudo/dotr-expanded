#include "dotr/ee/build_deck_draw.h"

DotrCaptionWord SzDuel_DrawBuildDeck_Pipe(DotrCaptionWord cursor)
{
    struct DotrCaptionControl *c = &data_00317490_caption;
    int side = c->side;
    int show;
    int id;

    RhBase_AssignRenderFrame(5, 2, 50);
    cursor = RhBase_EnableRenderFrame(cursor);
    cursor = RhBase_ClearRenderFrame(cursor, 0);
    cursor = RhBase_DisableRenderFrame(cursor);
    cursor = SzDuel_StartWindowSessionWithPalette_Pipe(cursor, 2);
    cursor = RhBase_MaskFBuffer(cursor, 0xff000000);
    cursor = lSzDuel_DrawBuildDeck_Base_Pipe(cursor);
    cursor = RhBase_MaskFBuffer(cursor, 0);
    cursor = lSzDuel_DrawBuildDeck_Title_Pipe(cursor, c->slide, 0);
    if (c->slide > 320) {
        cursor = lSzDuel_DrawBuildDeck_Library_Pipe(cursor, c->slide, 0);
    }
    if (c->slide > 0) {
        if (c->mode == 0) {
            show = 0;
            id = (c->grid_top + c->grid_row) * 10 + c->grid_col;
            if (SzData_GetBagCount(side, id, -1) > 0) {
                show = 1;
            }
            if (SzData_GetLookFlag(side, id)) {
                show = 1;
            }
            if (show) {
                cursor = lSzDuel_DrawBuildDeck_Detail_Pipe(cursor, id, c->slide, 0, 0, 0, 0, 1);
            }
        } else {
            cursor = lSzDuel_DrawBuildDeck_Detail_Pipe(cursor, c->list_cards[c->list_cursor + c->list_top], c->slide, 0, 0,
                                                       c->mode == 1 ? 1 : 0, 0, 1);
        }
    }
    if (c->slide > -320 && c->slide < 640) {
        cursor = lSzDuel_DrawBuildDeck_Bag_Pipe(cursor, c->slide, 0);
    }
    if (c->slide < 320) {
        cursor = lSzDuel_DrawBuildDeck_Deck_Pipe(cursor, c->slide, 0);
    }
    if (c->slide < 0) {
        cursor = lSzDuel_DrawBuildDeck_Detail_Pipe(cursor, c->deck_cards[c->deck_cursor + c->deck_top], c->slide, 0, 1,
                                                   c->mode == 6 ? 1 : 0, 0, 0);
    }
    cursor = lSzDuel_DrawBuildDeck_Ghost_Pipe(cursor, c->slide, 0);
    switch (c->mode) {
    case 0:
    case 1:
    case 6:
        break;
    case 2:
    case 3:
        cursor = lSzDuel_DrawBuildDeck_BagCursor_Pipe(cursor, c->slide, 0, c->list_top);
        break;
    case 4:
    case 5:
        cursor = lSzDuel_DrawBuildDeck_DeckCursor_Pipe(cursor, c->slide, 0, c->deck_top);
        break;
    case 7:
        cursor = lSzDuel_DrawBuildDeck_Reincarnation_Pipe(cursor, 0, 0);
        break;
    case 8:
        cursor = lSzDuel_DrawBuildDeck_ReincarnationDone_Pipe(cursor, 0, 0);
        break;
    case 9:
        cursor = lSzDuel_DrawBuildDeck_Password_Pipe(cursor, 0, 0);
        break;
    case 10:
        cursor = lSzDuel_DrawBuildDeck_PasswordSuccess_Pipe(cursor, 0, 0);
        break;
    case 11:
        cursor = lSzDuel_DrawBuildDeck_PasswordFail_Pipe(cursor, 0, 0);
        break;
    case 12:
        cursor = lSzDuel_DrawBuildDeck_PasswordDone_Pipe(cursor, 0, 0);
        break;
    case 13:
        cursor = lSzDuel_DrawBuildDeck_Caution_Pipe(cursor, 0, 0);
        break;
    }
    SzMsg_Free(0);
    RhBase_FreeRenderFrame();
    return cursor;
}

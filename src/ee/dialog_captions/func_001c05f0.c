#include "dotr/ee/dialog_captions.h"

DotrCaptionWord lSzDuel_DrawBuildDeck_Reincarnation_Pipe(DotrCaptionWord cursor, int x, int y)
{
    /* MATCHING DEVICE: the control block pointer declared first. */
    struct DotrCaptionControl *g = &data_00317490_caption;
    /* MATCHING DEVICE: base + 2 keeps the caption address in a saved register across the
     * calls (the plain symbol does not). */
    char *text = data_002bef9b_caption + 2;
    char *text2 = data_002bef6d_caption + 2;
    char *text3 = data_002bef71_caption + 2;
    int card;
    int name;
    struct DotrCaptionMsgInfo *info;

    card = g->list_cards[g->list_cursor + g->list_top];
    name = func_001de210_caption(card);
    cursor = func_001b0ce0_caption(cursor, 2, card, name, x + 320, y + 72, -460, 304, func_0019bef0_caption(text), text, 0L);
    cursor = func_001fd680_caption(cursor, 1, 146, 0, 22);
    cursor = func_0020f290_caption(cursor);
    info = func_001feab0_caption(1);
    cursor = func_001fe7d0_caption(cursor, 1, x + 320 - (info->width >> 1), y + 340 - (info->height + 12), 0, func_001fdfe0_caption);
    func_001feaf0_caption(1);
    cursor = func_001b6f70_choice(cursor, x + 205, y + 340, 2, 3, func_0019bef0(text2), text2, g->choice ? 0 : data_002be944_caption);
    return func_001b6f70_choice(cursor, x + 435, y + 340, 2, 3, func_0019bef0(text3), text3, g->choice ? data_002be944_caption : 0);
}

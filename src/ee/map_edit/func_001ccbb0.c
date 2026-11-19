#include "dotr/ee/map_edit.h"

DotrSpriteWord SzDuel_DrawMapEdit_Pipe(DotrSpriteWord cursor)
{
    struct DotrMapEditState *edit = DOTR_MAP_EDIT;
    int alpha = (unsigned char)(data_00318116 >> 8);
    struct DotrTintQuad colour = data_002914d0;
    int x;
    int y;
    int j;
    int i;
    int field;
    int width;
    float ratio;
    /* MATCHING DEVICE: unused; the original frame is 16 bytes larger than the named locals need.
     * Target build only (the host build rejects an unused local). */
#ifdef __MWERKS__
    int pad[4];
#endif

    colour.alpha = alpha + 64;
    SzDuel_InitMapColor2(0, 0, 0, 0);
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            if (edit->unknown_8f == x || edit->unknown_90 == y) {
                SzDuel_SetMapColor2(x, y, 1, &colour, 0, 0);
            }
        }
    }
    SzDuel_SetMapColor2(DOTR_MAP_EDIT->unknown_8f, DOTR_MAP_EDIT->unknown_90, 1, &colour, 0, 0);
    for (j = 0; j < 7; j++) {
        for (i = 0; i < 7; i++) {
            SzDuel_CalcMapColor(i, j);
        }
    }
    SzDuel_SetWindowDefPrioZ(0xfffff8);
    cursor = lSzDuel_DrawWindow_FieldPiece_Pipe(cursor, 0, SzDuel_GetField(DOTR_MAP_EDIT->unknown_8f, DOTR_MAP_EDIT->unknown_90), 100.0f, 6, 336);
    cursor = SzDuel_StartWindowSessionWithPalette_Pipe(cursor, 0);
    field = SzDuel_GetField(DOTR_MAP_EDIT->unknown_8f, DOTR_MAP_EDIT->unknown_90);
    ratio = 1.0f;
    cursor = SzMsg_LoadFont_Pipe(cursor, 1, field + 71, 0, 22);
    cursor = RhText_SetTextureFlush(cursor);
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, -1, 2, 3, 6, 336, 9, 18, 1);
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, 5, -1, 3, 6, 400, 9, 2, 1);
    width = SzMsg_GetInfo(1)->unknown_0c;
    if (width > 84) {
        ratio = 84.0f / (float)width;
    }
    cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, 54 - func_001a22c0((float)(width >> 1) * ratio), 403, 0, SzMsg_DrawRect_Pipe, ratio, 1.0f);
    SzMsg_Free(1);
    cursor = lSzDuel_DrawWindow_CommandGuide_Pipe(cursor, 0, 538, 336);
    cursor = lSzDuel_DrawMapEdit_Graph_Pipe(cursor, 6, 144);
    SzMsg_Free(0);
    return cursor;
}

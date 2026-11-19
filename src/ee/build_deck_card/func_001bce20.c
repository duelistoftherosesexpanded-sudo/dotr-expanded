#include "dotr/ee/build_deck_card.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* lSzDuel_DrawBuildDeck_Card_Pipe (0x1bce20): draws one card's panel on the build-deck screen. */
void *lSzDuel_DrawBuildDeck_Card_Pipe(void *arg0, int card, int x, int y, int flag, int flag2) {
    int player = DeckBuildingSide_buildcard;
    unsigned char *state = data_00317490_buildcard;
    DotrBuildCardWord id = (DotrBuildCardWord)arg0;
    struct DotrBuildCardCard *cardConstInfo;
    int currentDeck;
    int rank;
    int w;
    unsigned char *sprite;
    int kind;
    int attr;
    int hint = 0;
    signed char *text;
    int px;
    int py;
    float scale;
    struct DotrBuildCardBox box __attribute__((aligned(8)));
    signed char digit;

    cardConstInfo = (struct DotrBuildCardCard *)SzDuel_GetCardConstInfo(card);
    currentDeck = SzData_GetCurrentDeck(player);
    scale = 1.0f;
    if (cardConstInfo == NULL) {
        return (void *)lSzDuel_DrawWindow_Frame_Pipe(id, flag ? 6 : -1, -1, 9, x, y, -0x130, -0x36, flag);
    }
    kind = cardConstInfo->kind >> 5;
    attr = cardConstInfo->kind & 0x1F;
    if (kind == 0) {
        sprite = gSzDuel_Icon_buildcard + attr * 10;
    } else {
        sprite = gSzDuel_Icon_buildcard + (kind + 0x14) * 10;
    }
    attr = SzDuel_Kind2Type(kind);
    rank = SzDuel_Type2Attr(attr, cardConstInfo->attribute);
    if (attr == 0 && cardConstInfo->link != 0xFFFF) {
        attr = 4;
    }
    id = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(id, 1, SzDuel_GetMsgName(card), 0, 0x16));
    id = lSzDuel_DrawWindow_Frame_Pipe(id, flag ? 6 : attr + 7, -1, 9, x, y, -0x130, -0x36, flag);
    if (SzData_IsBagNew(player, card) != 0) {
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290a12_buildcard, x + 0x6C8, y + 0x727, 0, 1, 0, 0, 0, 0, 1,
                                            0, -1, 0x80);
    } else {
        id = SzDuel_DrawWindow_FontDigit_Pipe(id, x + 8, y + 7, 1, 0, 3, card);
    }
    if (kind == 0) {
        id = lSzDuel_DrawWindow_Emblem_Pipe(id, x + 0x28, y + 3, SzData_GetRank(player, card), 0, 0);
    }
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, sprite, x + 0x6C4, y + 0x73A, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290020_buildcard + rank * 10, x + 0x6DE, y + 0x73A, 0, 1, 0, 0, 0,
                                        0, 1, 0, -1, 0x80);
    if (flag) {
        text = data_002befb7_buildcard;
        px = x + 0xA0;
        py = y + 0x25;
        id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 0, 2, func_0019bef0((char *)text), text,
                                         hint != 0 ? (DotrBuildCardWord)data_002be944_buildcard : 0);
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, gSzDuel_Icon_buildcard + (currentDeck + 0xE3) * 10, x + 0x7C7, y + 0x730,
                                            0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    } else {
        if (kind == 0) {
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_002908e6_buildcard, x + 0x6F8, y + 0x743, 0, 1, 0, 0, 0, 0,
                                                1, 0, -1, 0x80);
            id = SzDuel_DrawWindow_FontDigit_Pipe(id, x + 0x4E, y + 0x25, 1, 0, 2, cardConstInfo->level);
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_002908be_buildcard, x + 0x7B8, y + 0x723, 0, 1, 0, 0, 0, 0,
                                                1, 0, -1, 0x80);
            id = SzDuel_DrawWindow_FontDigit_Pipe(id, x + 0x108, y + 5, 1, 0, 4, cardConstInfo->attack & 0x1FFF);
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_002908c8_buildcard, x + 0x7B8, y + 0x733, 0, 1, 0, 0, 0, 0,
                                                1, 0, -1, 0x80);
            id = SzDuel_DrawWindow_FontDigit_Pipe(id, x + 0x108, y + 0x15, 1, 0, 4, cardConstInfo->defense & 0x1FFF);
        }
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_002908f0_buildcard, x + 0x728, y + 0x743, 0, 1, 0, 0, 0, 0, 1,
                                            0, -1, 0x80);
        id = SzDuel_DrawWindow_FontDigit_Pipe(id, x + 0x7E, y + 0x25, 1, 0, 3, cardConstInfo->stars);
        px = x + 0x76D;
        py = y + 0x743;
        if (SzData_GetBagCount(player, card, -1) < 9) {
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_0029086e_buildcard, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1,
                                                0x80);
        } else {
            id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290878_buildcard, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1,
                                                0x80);
        }
        px = x + 0xC0;
        py = y + 0x25;
        id = SzDuel_DrawWindow_FontDigit_Pipe(id, px, py, 1, 0, 1, SzData_GetBagCount(player, card, hint != 0 ? -1 : 3));
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_0029097c_buildcard, x + 0x78D, y + 0x743, 0, 1, 0, 0, 0, 0, 1,
                                            0, -1, 0x80);
        px = x + 0xE0;
        py = y + 0x25;
        digit = SzData_GetBagCount(player, card, 0) + '0';
        id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, 1, &digit,
                                         hint != 0 ? (DotrBuildCardWord)data_002be944_buildcard : 0);
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290986_buildcard, x + 0x7AD, y + 0x743, 0, 1, 0, 0, 0, 0, 1,
                                            0, -1, 0x80);
        px = x + 0x100;
        py = y + 0x25;
        digit = SzData_GetBagCount(player, card, 1) + '0';
        id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, 1, &digit,
                                         hint != 0 ? (DotrBuildCardWord)data_002be944_buildcard : 0);
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_00290990_buildcard, x + 0x7CD, y + 0x743, 0, 1, 0, 0, 0, 0, 1,
                                            0, -1, 0x80);
        px = x + 0x120;
        py = y + 0x25;
        digit = SzData_GetBagCount(player, card, 2) + '0';
        id = SzDuel_DrawWindow_Font_Pipe(id, px, py, 1, 0, 1, &digit,
                                         hint != 0 ? (DotrBuildCardWord)data_002be944_buildcard : 0);
    }
    px = x + 0x48;
    py = y + 3;
    id = lSzDuel_DrawWindow_ExBoard_Pipe(id, flag ? 6 : attr + 7, px, py, -0xB0, -0x20, -1, 0);
    w = SzMsg_GetInfo(1)[3];
    if (w > 0xA8) {
        scale = 168.0f / (float)w;
    }
    id = SzMsg_DrawRatioX_Pipe(id, 1, x + 0x4E, y + 9, scale, 1.0f, (DotrBuildCardWord)gSzDuel_aicolMsg_buildcard,
                               SzMsg_DrawRect_Pipe);
    id = SzMsg_DrawRatioX_Pipe(id, 1, x + 0x4D, y + 8, scale, 1.0f, (DotrBuildCardWord)data_002be944_buildcard,
                               SzMsg_DrawRect_Pipe);
    id = SzMsg_DrawRatioX_Pipe(id, 1, x + 0x4C, y + 7, scale, 1.0f, (DotrBuildCardWord)data_002be948_buildcard,
                               SzMsg_DrawRect_Pipe);
    SzMsg_Free(1);
    if (flag2 != 0 && SzData_GetBagCount(player, card, 3) <= 0) {
        ((struct DotrBuildCardTag *)box.tag)->flags = 6;
        ((struct DotrBuildCardTag *)box.tag)->count = 3;
        ((struct DotrBuildCardTag *)box.tag)->registers = 0x441;
        ((struct DotrBuildCardTag *)box.tag)->padding = 0;
        ExAssignAlpha(box.tag, ExGetAlphaVal(2));
        box.color.r = 0x80;
        box.color.g = 0x80;
        box.color.b = 0x80;
        box.color.a = 0x40;
        box.color.q = 1.0f;
        box.vertex0.x = (x + 0x6C0) << 4;
        box.vertex0.y = (y + 0x720) << 4;
        box.vertex0.z = *(int *)(state + 0xC58);
        box.vertex0.fog = 0;
        box.vertex1.x = (x + 0x7F0) << 4;
        box.vertex1.y = (y + 0x756) << 4;
        box.vertex1.z = *(int *)(state + 0xC58);
        box.vertex1.fog = 0;
        id = ExSortPrim_Pipe(id, (DotrBuildCardWord)box.tag, 0, 0);
    }
    return (void *)id;
}

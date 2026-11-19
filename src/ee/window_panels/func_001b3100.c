#include "dotr/ee/window_panels.h"

/* lSzDuel_DrawWindow_CommandGuide_Pipe (0x1b3100): the command guide of the duel
 * information window. Draws its frame, picks a guide row from the control block's state
 * byte (+0xd57) and the selected units, and draws the row's four lines: an icon and a
 * message scaled to at most 44 wide. Returns after the frame when no row applies. Views and
 * assumed names: window_panels.h. */
DotrPanelWord lSzDuel_DrawWindow_CommandGuide_Pipe(DotrPanelWord cursor, int id, int x, int y)
{
    struct DotrWindowControl *g = &data_00317490_window;
    struct DotrWindowSide *side = &data_00317490_window.sides[data_00317490_window.side];
    int kind;
    int px = x + 4;
    int py = y + 4;
    int i;
    int text;
    struct DotrGuideEntry *e;
    int deck, hand, hide, r;

    kind = -1;
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, id, 2, 3, x, y, 9, 18, 1);
    switch (data_00317490_window.unknown_d57) {
    case 24:
        if (g->unit_6c8 != 0) {
            r = func_00250360_pair(g->unit_6c8->side, g->unit_6c8->place);
            if (g->side == r) {
                if (g->unit_6c8->place == 0) {
                    deck = SzDuel_GetDeckCount(g->side);
                    hand = SzDuel_GetHandCount(g->side);
                    if (side->unknown_346_bit_0 && deck + hand > 0) {
                        if (g->unit_6c8->unknown_57_bit_0) {
                            kind = 3;
                        } else {
                            kind = 1;
                        }
                    } else if (hand != 0) {
                        if (g->unit_6c8->unknown_57_bit_0) {
                            kind = 4;
                        } else {
                            kind = 2;
                        }
                    } else {
                        if (g->unit_6c8->unknown_57_bit_0) {
                            kind = 11;
                        } else {
                            kind = 10;
                        }
                    }
                } else {
                    if (g->unit_6c8->unknown_57_bit_0) {
                        kind = 11;
                    } else {
                        kind = 10;
                    }
                }
            } else {
                hide = 0;
                if (g->unit_6c8->place > 0) {
                    hide = Ta_IsUnitHide(g->unit_6c8, g->side);
                }
                kind = hide == 0 ? 11 : 0;
            }
        } else {
            kind = 0;
        }
        break;
    case 29:
        if (g->unit_6c8->place == 0) {
            kind = 5;
            if (g->unit_6cc != 0 && g->unit_6cc != g->unit_6c8) {
                r = func_00250360_pair(g->unit_6cc->side, g->unit_6cc->place);
                if (g->side == r) {
                    kind = 6;
                }
            }
        } else {
            if (g->unit_6cc != 0) {
                if (g->unit_6cc == g->unit_6c8) {
                    if (g->unit_6c8->unknown_57_bit_4) {
                        kind = 13;
                    } else {
                        kind = 12;
                    }
                } else {
                    r = func_00250360_pair(g->unit_6cc->side, g->unit_6cc->place);
                    if (g->side == r) {
                        kind = 14;
                    } else {
                        kind = 15;
                    }
                }
            } else {
                kind = 5;
            }
        }
        break;
    case 62:
        if (!SzDuel_GetMapMovable(side->x, side->y)) {
            kind = 7;
        }
        break;
    case 67:
        kind = 8;
        break;
    case 83:
        kind = 17;
        break;
    case 70:
        kind = 9;
        break;
    case 41:
    case 37:
        kind = 18;
        break;
    case 45:
        if (g->unknown_d66_bit_3) {
            kind = 16;
        } else {
            kind = 17;
        }
        break;
    case 48:
        if (SzView_InqRequest(kind, kind) == 0) {
            kind = 19;
        }
        break;
    case 99:
        kind = 20;
        break;
    }
    if (kind < 0) {
        return cursor;
    }
    id = px + 10;
    e = data_00291180[kind];
    for (i = 0; i < 4; i++) {
        int icon = e->icon;
        int dx = e->dx;
        int dy = e->dy;

        text = e->message;
        if (icon < 1023) {
            cursor = lSzDuel_DrawWindow_Icon_Pipe(cursor, &gSzDuel_Icon_bytes[icon * 10], id + 1728 + dx, py + 1824 + dy, 0, 1, 0, 0, 1, 0, -1);
        }
        if (text < 0xffff) {
            struct DotrPanelInfo *info;
            float ratio;

            cursor = SzMsg_LoadFont_Pipe(cursor, 1, text, 0, 22);
            cursor = RhText_SetTextureFlush(cursor);
            info = SzMsg_GetInfo(1);
            if (info->width > 44) {
                ratio = 44.0f / info->width;
            } else {
                ratio = 1.0f;
            }
            cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, id + 26, py, 0, SzMsg_DrawRect_Pipe, ratio, 1.0f);
            SzMsg_Free(1);
        }
        py += 22;
        e++;
    }
    return cursor;
}

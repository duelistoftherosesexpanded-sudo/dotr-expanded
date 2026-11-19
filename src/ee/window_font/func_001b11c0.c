#include "dotr/ee/window_font.h"

/* SzDuel_DrawWindow_Pipe (0x1b11c0): the duel's information window. Slides the window in or
 * out (8 or 16 per frame), then draws the field piece, both player panels, one or two card
 * panels, the command guide, the field name, the frame with the turn digits, the pad icon,
 * the level and summon-cost rows and the hand cursor. Views and assumed names:
 * window_font.h. */
DotrDrawWord SzDuel_DrawWindow_Pipe(DotrDrawWord cursor)
{
    struct DotrWindowControl *g = &data_00317490_window;
    int off;
    struct DotrWindowUnit *unit;
    struct DotrWindowUnit *other;
    int x;
    int y;
    int adv;
    DotrDrawWord out;
    struct DotrWindowSide *side;
    long first = 1;
    int adv2;
    int closing = data_00317490_window.unknown_d67_bit_0;
    int step = 8;
    struct DotrWindowSide *s;
    int v;
    int a;
    int px;
    int c1;
    int c2;
    int py;

    if (data_00317490_window.unknown_d67_bit_1) {
        return cursor;
    }
    if (g->unknown_d67_bit_2 || g->unknown_d45 != 0 || g->unknown_d47 != 0 || g->unknown_d48 != 0) {
        if (!g->unknown_d65_bit_4) {
            closing = 1;
            step = 16;
        }
    }
    if (closing) {
        v = g->slide - step;
        g->slide = v > 0 ? v : 0;
    } else {
        v = g->slide + step;
        g->slide = v < 128 ? v : 128;
        if (g->slide == 128) {
            g->side = g->unknown_d54;
            g->unknown_d3d = g->unknown_d53;
            g->unknown_d67_bit_0 = g->unknown_d66_bit_7;
            s = &g->sides[g->side];
            g->x = s->x;
            g->y = s->y;
        }
    }
    if (g->slide == 128) {
        return cursor;
    }
    off = g->slide << 7 >> 7;
    side = &g->sides[g->side];
    SzDuel_SetWindowDefPrioZ(0xfffff8);
    cursor = lSzDuel_DrawWindow_FieldPiece_Pipe(cursor, g->side, SzDuel_GetField(side->x, side->y), 100.0f, g->side != 0 ? 6 : 538,
                                                off + 336);
    cursor = SzDuel_StartWindowSessionWithPalette_Pipe(cursor, 0);
    cursor = lSzDuel_DrawWindow_PlayerInfo_Pipe(cursor, 0, 18, 20 - off, g->side == 0 ? 1 : 0);
    out = lSzDuel_DrawWindow_PlayerInfo_Pipe(cursor, 1, 440, 20 - off, g->side == 1 ? 1 : 0);
    if (g->unknown_d45 != 0 || g->unknown_d47 != 0) {
        other = g->unit_6cc;
        unit = SzDuel_GetUnit(2, g->unknown_d4f + 1);
        if (g->unknown_d67_bit_3 || (!g->unknown_d67_bit_4 && g->unknown_d47 != 0)) {
            v = g->slide2 - 16;
            g->slide2 = v > 0 ? v : 0;
        } else {
            v = g->slide2 + 16;
            g->slide2 = v < 128 ? v : 128;
        }
        /* MATCHING DEVICE: negated and-chains; `a == 81 || a == 82 || a == 83` is merged
         * into a range test, the original has three compares. */
        if (!(g->unknown_d57 != 81 && g->unknown_d57 != 82 && g->unknown_d57 != 83)) {
            other = g->unit_6c8;
        } else if (g->unknown_d35 == 83) {
            if (!(g->unknown_d57 != 32 && g->unknown_d57 != 33 && g->unknown_d57 != 34)) {
                other = g->unit_6c8;
            }
        }
        a = g->slide2;
        if (a != 0) {
            out = lSzDuel_DrawWindow_CardInfo_Pipe(out, unit, side->x, side->y, unit != 0 ? (int)unit->side : (int)g->side, 104,
                                                   368 - ((unsigned char)a << 6 >> 7) + off, -1, 6, 1);
            first = 0;
        }
        out = lSzDuel_DrawWindow_CardInfo_Pipe(out, other, side->x, side->y, other != 0 ? (int)other->side : (int)g->side, 104,
                                               off + 368, -1, 3, first);
    } else {
        x = -1;
        adv = 1;
        y = -1;
        adv2 = 1;
        if (g->unit_6c8 != 0 && g->unit_6c8->place < 11 && g->unit_6cc != 0 && g->unit_6cc->place < 11) {
            c1 = func_00250360_pair(g->unit_6c8->side, g->unit_6c8->place);
            c2 = func_00250360_pair(g->unit_6cc->side, g->unit_6cc->place);
            if (c1 != c2) {
            adv = Ta_IsVisualAttrAdv(g->unit_6c8->side, g->unit_6c8->place, g->unit_6cc->side, g->unit_6cc->place);
            if (adv < 0) {
                adv2 = adv;
            } else {
                adv2 = 2 - adv;
            }
            }
        }
        px = side->x;
        py = side->y;
        if (g->unit_6c8 != 0 && g->unit_6c8->place < 11 && g->unit_6c8->unknown_57_bit_6) {
            g->x = px = g->unit_6c8->x;
            g->y = py = g->unit_6c8->y;
            if (side->x != px || side->y != py) {
                x = side->x;
                y = side->y;
                if (g->slide2 == 0) {
                    g->x2 = x;
                    g->y2 = y;
                }
            }
        } else if (g->unknown_d57 == 24 || g->unknown_d57 == 62 || g->unknown_d57 == 48) {
            g->x = px;
            g->y = py;
        }
        if (g->x2 == x && g->y2 == y && SzDuel_GetMapUnit(x, y) != 0) {
            v = g->slide2 + 16;
            g->slide2 = v < 128 ? v : 128;
        } else {
            v = g->slide2 - 16;
            g->slide2 = v > 0 ? v : 0;
        }
        if (g->slide2 != 0) {
            unit = SzDuel_GetMapUnit(g->x2, g->y2);
            out = lSzDuel_DrawWindow_CardInfo_Pipe(out, unit, g->x2, g->y2, unit != 0 ? (int)unit->side : (int)g->side, 104,
                                                   368 - (g->slide2 << 6 >> 7) + off, adv2, 3, 1);
            first = 0;
        }
        unit = SzDuel_GetMapUnit(g->x, g->y);
        out = lSzDuel_DrawWindow_CardInfo_Pipe(out, unit, side->x, side->y, unit != 0 ? (int)unit->side : (int)g->side, 104, off + 368,
                                               adv, 3, first);
    }
    cursor = lSzDuel_DrawWindow_CommandGuide_Pipe(out, g->side, g->side != 0 ? 538 : 6, off + 336);
    cursor = lSzDuel_DrawWindow_Field_Pipe(cursor, SzDuel_GetField(side->x, side->y), g->side != 0 ? 6 : 538, off + 336);
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, 5, 3, 4, 292, 16 - off, 4, 2, 1);
    cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, 320, 22 - off, 2, 34, 2, 100 - g->unknown_d3d);
    if (g->unknown_d44 != 0) {
        cursor = lSzDuel_DrawWindow_DualShock2_Pipe(cursor, 320, 72 - off, (unsigned char)((unsigned char)g->unknown_d44 << 7 >> 7));
    }
    if (g->unknown_d48 != 0) {
        y = g->unknown_d45 << 7 >> 7;
        y = 54 - off + (y - 128);
        cursor = lSzDuel_DrawWindow_Level_Pipe(cursor, 220, y);
        cursor = lSzDuel_DrawWindow_SummonCost_Pipe(cursor, 332, y);
    }
    SzDuel_SetWindowDefPrioZ(0xfffffc);
    cursor = SzDuel_DrawHandCursor_Pipe(cursor);
    SzMsg_Free(0);
    return cursor;
}

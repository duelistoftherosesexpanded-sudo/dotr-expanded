#include "dotr/ee/window_panels.h"

/* lSzDuel_DrawWindow_PlayerInfo_Pipe (0x1b19f0): one player's panel of the duel information
 * window: frame, portrait with its frame, the board with the points digits, the name icon
 * (row 185 + 2 * side, plus one when the side's player id is below 100), the caption icons
 * and the hand, deck, monster and spell counts. The hand, monster and spell counts are
 * replaced by the text at 0x2bef76 when `hidden` stays set: it is cleared when both players'
 * ids are below 100, or when the side to move has an id of 100 or more and is this side, or
 * has an id below 100 and is the other side. Views and assumed names: window_panels.h.
 * Compiled under #pragma opt_propagation off (reset after). The callees are declared in the
 * header (views without prototypes for four of them), and the first icon row is cast to the
 * cursor word type. */
#ifdef __MWERKS__
#pragma opt_propagation off
#endif
DotrPanelWord lSzDuel_DrawWindow_PlayerInfo_Pipe(DotrPanelWord cursor, int side, int x, int y, int active)
{
    DotrPanelWord c0;
    DotrPanelWord c1;
    int baseX;
    int screenX;
    int lowLevel;
    int playerOffset;
    unsigned char *playerData;
    unsigned char *data;
    int iconX;
    int valueX;
    int baseY;
    int iconY;
    int screenY;
    int rowX;
    int secondValueY;
    int lastX;
    int screenValueX;
    int numberX;
    int numberY;
    int screenRowX;
    int lastY;
    int lowerY;
    int hidden;
    int rowX2;

    data = data_00317490_panels.b;
    playerData = data_00317490_panels.b + side * 0x350;
    baseY = y + 3;
    baseX = x + 3;
    screenY = baseY + 0x720;
    hidden = 1;
    screenX = baseX + 0x6C0;
    if (playerData[0x33C] < 100) {
        lowLevel = 1;
    } else {
        lowLevel = 0;
    }
    if (SzDuel_GetPlayer(0) < 100 && SzDuel_GetPlayer(1) < 100) {
        hidden = 0;
    } else {
        if (SzDuel_GetPlayer(data[0xD3C]) >= 100) {
            if (data[0xD3C] == side) {
                hidden = 0;
            }
        } else if (data[0xD3C] != side) {
            hidden = 0;
        }
    }
    cursor = func_001b4780_info(cursor, side, side, 0, x - 13, y - 15, 37, 13, 1);
    iconX = x + 3 + (side != 0 ? 1 : 127);
    iconY = y + 4;
    cursor = func_001b7f90_info(cursor, side, iconX, iconY, 48, 48, *(int *)(data + 0xC58), active);
    cursor = func_001b4780_info(cursor, -1, -1, 4, iconX, iconY, 3, 6, active);
    playerOffset = side * 48;
    numberY = baseY + 6;
    rowX = baseX + playerOffset;
    numberX = rowX + 4;
    cursor = lSzDuel_DrawWindow_Board_Pipe(cursor, side, numberX, numberY, 12, 1, -1);
    cursor = SzDuel_DrawWindow_Digit_Pipe(cursor, numberX + 34, numberY + 2, 4, *(unsigned short *)(playerData + 0x33A), 0);
    iconX = screenX + (side != 0 ? 1 : 127);
    cursor = func_001b3c80_info(cursor, (DotrPanelWord)(gSzDuel_Icon_panels + (lowLevel + (side * 2 + 185)) * 10), iconX, 48 + screenY, 0, 1, 0, 0, active, 0, -1);
    screenRowX = screenX + playerOffset;
    valueX = screenRowX + 8;
    iconX = screenRowX + 8;
    iconY = screenY + 12;
    cursor = func_001b3c80_info(cursor, data_00290a1c_panels, iconX, iconY, 0, 1, 0, 0, 1, 0, -1);
    cursor = func_001b3c80_info(cursor, data_002908e6_panels, valueX, screenY + 36, 0, 1, 0, 0, 1, 0, -1);
    rowX2 = rowX + 26;
    iconX = rowX + 26;
    lowerY = baseY + 37;
    if (hidden != 0) {
        cursor = func_001b6f70_info(cursor, iconX, lowerY, 0, 0, 2, data_002bef76_panels, 0);
    } else {
        cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, iconX, lowerY, 0, 0, 2, playerData[0x341]);
    }
    screenValueX = screenRowX + 64;
    iconX = screenRowX + 64;
    cursor = func_001b3c80_info(cursor, data_00290882_panels, iconX, screenY + 36, 0, 1, 0, 0, 1, 0, -1);
    lastX = rowX + 82;
    lowerY = baseY + 37;
    cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, lastX, lowerY, 0, 0, 2, SzDuel_GetDeckCount(side));
    c0 = func_001b3c80_info(cursor, data_0029094a_panels, valueX, screenY + 52, 0, 1, 0, 0, 1, 0, -1);
    secondValueY = 53 + baseY;
    if (0 != hidden) {
        c0 = func_001b6f70_info(c0, rowX2, secondValueY, 0, 0, 2, data_002bef76_panels, 0);
    } else {
        c0 = SzDuel_DrawWindow_FontDigit_Pipe(c0, rowX2, secondValueY, 0, 0, 2, SzDuel_GetUnitMonsterCount(side));
    }
    c1 = func_001b3c80_info(c0, data_00290954_panels, screenValueX, screenY + 52, 0, 1, 0, 0, 1, 0, -1);
    lastY = baseY + 53;
    if (hidden != 0) {
        c1 = func_001b6f70_info(c1, lastX, lastY, 0, 0, 2, data_002bef76_panels, 0);
    } else {
        c1 = SzDuel_DrawWindow_FontDigit_Pipe(c1, lastX, lastY, 0, 0, 2, SzDuel_GetUnitSpellCount(side));
    }
    return c1;
}
#ifdef __MWERKS__
#pragma opt_propagation reset
#endif

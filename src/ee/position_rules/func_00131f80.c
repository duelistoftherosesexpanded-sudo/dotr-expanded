#include "dotr/ee/position_rules.h"

int TaLogic008_GetNextInvadePoint(int word, int column, int row)
{
    struct DotrSpotNibbles bytes[12];
    int spot;
    int key;
    int i;
    struct DotrSpotNibbles *p;
    int odd;
    int side;

    spot = -1;
    key = func_00250370_word(word);
    for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
    if (word == 0) {
        return -1;
    }
    if (column >= 3 && (row == 0 || row == 1 || row == 4 || row == 5)) {
        spot = ((row % 2 == 0 ? row + 2 : row + 1) << 8) | 6;
    } else if (column < 3 && (row == 2 || row == 3)) {
        spot = 0x400;
    }
    if (spot >= 0) {
        if (func_0027a130_spot(word, bytes, column, row, spot & 0xff, spot >> 8, -1, 0, 7, TaLogic_GetWarnHideLvl__00134ae0(key)) >= 0) {
            return spot;
        }
    }
    if (column < 3 && (row == 0 || row == 1 || row == 4 || row == 5)
        && (odd = row % 2, func_00250a60_spot(word, 0, odd != 0 ? row + 1 : row) < 1500)) {
        spot = (odd == 0 ? row + 2 : row + 1) << 8;
    } else if (column >= 3 && row == 2 && func_00250a60_spot(word, 6, 3) < 1500) {
        spot = 0x406;
    }
    if (spot >= 0) {
        if (func_0027a130_spot(word, bytes, column, row, spot & 0xff, spot >> 8, -1, 0, 7, TaLogic_GetWarnHideLvl__00134ae0(key)) >= 0) {
            return spot;
        }
    }
    spot = SzDuel_GetUnitLoc((side = func_00250370_word(word)) == 0 ? 1 : 0, 0);
    if (spot >= 0) {
        if (func_0027a130_spot(word, bytes, column, row, spot & 0xff, spot >> 8, -1, 0, 7, TaLogic_GetWarnHideLvl__00134ae0(key)) >= 0) {
            return spot;
        }
    }
    return ((spot >> 8) << 8) | column;
}

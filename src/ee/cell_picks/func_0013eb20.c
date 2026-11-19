#include "dotr/ee/cell_picks.h"

int TaLogic011_GetNearMountain(int word, int column, int row)
{
    int result;
    int best;
    int x;
    int y;
    int cell;
    int owner;
    int d;

    best = 14;
    for (x = 0; x < 7; x++) {
        for (y = 0; y < 7; y++) {
            if (SzDuel_GetField(x, y) == 2) {
                if ((cell = SzDuel_GetMapUnitID(x, y)) < 0) {
                    d = ((column < x ? x : column) - (x < column ? x : column)) + ((row < y ? y : row) - (y < row ? y : row));
                    if (d >= 0 && d < best) {
                        best = d;
                        result = (y << 8) | x;
                    }
                } else {
                    owner = func_00250360_pair(cell >> 8, cell & 0xff);
                    if (owner != func_00250370_word(word)
                        && func_00278510_word(word, cell, TaLogic_GetWarnHideLvl__00141b30(func_00250370_word(word)), x, y, 0) > 0) {
                        d = ((column < x ? x : column) - (x < column ? x : column)) + ((row < y ? y : row) - (y < row ? y : row));
                        if (d >= 0 && d < best) {
                            best = d;
                            result = (y << 8) | x;
                        }
                    }
                }
            }
        }
    }
    return result;
}

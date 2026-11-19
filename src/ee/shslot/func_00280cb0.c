#include "dotr/ee/shslot.h"

/* ShSlot_CheckLine (0x280cb0): called when reel `reel` stops. Reads the 3x3 window
 * (symbols at position / 0x70 + 0..2 modulo the strip length; 0xffff for a reel not yet
 * stopped, phase != 4), then for each line still in the winning mask (0x351242 bits 0..4:
 * window rows 1, 2 and 0, then the diagonals [r][2 - r] and [r][r]) clears its bit when
 * this reel's symbol on the line is 0x334 or differs from another stopped reel's symbol on
 * it. Finally stores this reel's window row 1 symbol as its center.
 * The original slotLIB code is unoptimised: this function is compiled at optimisation
 * level 0 (docs/DECOMPILATION.md). */
#ifdef __MWERKS__
#pragma optimization_level 0
#endif
void ShSlot_CheckLine(int reel)
{
    unsigned short symbols[3][3];
    int row;
    int column;
    int offset;
    int clear;

    for (row = 0; row < 3; row++) {
        if (data_00351240_shslot[row].enabled != 4) {
            symbols[row][0] = symbols[row][1] = symbols[row][2] = 0xFFFF;
        } else {
            offset = data_00351240_shslot[row].position / 0x70;
            for (column = 0; column < 3; column++) {
                symbols[row][column] = data_00351240_shslot[row].symbols[(offset + column) % *data_00351230_shslot_bytes];
            }
        }
    }
    for (column = 0; column < 5; column++) {
        if (CanStillGet3InARow_shslot & (1 << column)) {
            clear = 0;
            switch (column) {
            case 0:
                if ((symbols[reel][1]) == 0x334) {
                    clear = 1;
                }
                switch (reel) {
                case 0:
                    if (((symbols[1][1]) != 0xFFFF) && ((symbols[0][1]) != (symbols[1][1]))) {
                        clear = 1;
                    }
                    if (((symbols[2][1]) != 0xFFFF) && ((symbols[0][1]) != (symbols[2][1]))) {
                        clear = 1;
                    }
                    break;
                case 1:
                    if (((symbols[0][1]) != 0xFFFF) && ((symbols[1][1]) != (symbols[0][1]))) {
                        clear = 1;
                    }
                    if (((symbols[2][1]) != 0xFFFF) && ((symbols[1][1]) != (symbols[2][1]))) {
                        clear = 1;
                    }
                    break;
                case 2:
                    if (((symbols[0][1]) != 0xFFFF) && ((symbols[2][1]) != (symbols[0][1]))) {
                        clear = 1;
                    }
                    if (((symbols[1][1]) != 0xFFFF) && ((symbols[2][1]) != (symbols[1][1]))) {
                        clear = 1;
                    }
                    break;
                }
                break;
            case 1:
                if ((symbols[reel][2]) == 0x334) {
                    clear = 1;
                }
                switch (reel) {
                case 0:
                    if (((symbols[1][2]) != 0xFFFF) && ((symbols[0][2]) != (symbols[1][2]))) {
                        clear = 1;
                    }
                    if (((symbols[2][2]) != 0xFFFF) && ((symbols[0][2]) != (symbols[2][2]))) {
                        clear = 1;
                    }
                    break;
                case 1:
                    if (((symbols[0][2]) != 0xFFFF) && ((symbols[1][2]) != (symbols[0][2]))) {
                        clear = 1;
                    }
                    if (((symbols[2][2]) != 0xFFFF) && ((symbols[1][2]) != (symbols[2][2]))) {
                        clear = 1;
                    }
                    break;
                case 2:
                    if (((symbols[0][2]) != 0xFFFF) && ((symbols[2][2]) != (symbols[0][2]))) {
                        clear = 1;
                    }
                    if (((symbols[1][2]) != 0xFFFF) && ((symbols[2][2]) != (symbols[1][2]))) {
                        clear = 1;
                    }
                    break;
                }
                break;
            case 2:
                if ((symbols[reel][0]) == 0x334) {
                    clear = 1;
                }
                switch (reel) {
                case 0:
                    if (((symbols[1][0]) != 0xFFFF) && ((symbols[0][0]) != (symbols[1][0]))) {
                        clear = 1;
                    }
                    if (((symbols[2][0]) != 0xFFFF) && ((symbols[0][0]) != (symbols[2][0]))) {
                        clear = 1;
                    }
                    break;
                case 1:
                    if (((symbols[0][0]) != 0xFFFF) && ((symbols[1][0]) != (symbols[0][0]))) {
                        clear = 1;
                    }
                    if (((symbols[2][0]) != 0xFFFF) && ((symbols[1][0]) != (symbols[2][0]))) {
                        clear = 1;
                    }
                    break;
                case 2:
                    if (((symbols[0][0]) != 0xFFFF) && ((symbols[2][0]) != (symbols[0][0]))) {
                        clear = 1;
                    }
                    if (((symbols[1][0]) != 0xFFFF) && ((symbols[2][0]) != (symbols[1][0]))) {
                        clear = 1;
                    }
                    break;
                }
                break;
            case 3:
                if ((symbols[reel][2 - reel]) == 0x334) {
                    clear = 1;
                }
                switch (reel) {
                case 0:
                    if (((symbols[1][1]) != 0xFFFF) && ((symbols[0][2]) != (symbols[1][1]))) {
                        clear = 1;
                    }
                    if (((symbols[2][0]) != 0xFFFF) && ((symbols[0][2]) != (symbols[2][0]))) {
                        clear = 1;
                    }
                    break;
                case 1:
                    if (((symbols[0][2]) != 0xFFFF) && ((symbols[1][1]) != (symbols[0][2]))) {
                        clear = 1;
                    }
                    if (((symbols[2][0]) != 0xFFFF) && ((symbols[1][1]) != (symbols[2][0]))) {
                        clear = 1;
                    }
                    break;
                case 2:
                    if (((symbols[0][2]) != 0xFFFF) && ((symbols[2][0]) != (symbols[0][2]))) {
                        clear = 1;
                    }
                    if (((symbols[1][1]) != 0xFFFF) && ((symbols[2][0]) != (symbols[1][1]))) {
                        clear = 1;
                    }
                    break;
                }
                break;
            case 4:
                if ((symbols[reel][reel]) == 0x334) {
                    clear = 1;
                }
                switch (reel) {
                case 0:
                    if (((symbols[1][1]) != 0xFFFF) && ((symbols[0][0]) != (symbols[1][1]))) {
                        clear = 1;
                    }
                    if (((symbols[2][2]) != 0xFFFF) && ((symbols[0][0]) != (symbols[2][2]))) {
                        clear = 1;
                    }
                    break;
                case 1:
                    if (((symbols[0][0]) != 0xFFFF) && ((symbols[1][1]) != (symbols[0][0]))) {
                        clear = 1;
                    }
                    if (((symbols[2][2]) != 0xFFFF) && ((symbols[1][1]) != (symbols[2][2]))) {
                        clear = 1;
                    }
                    break;
                case 2:
                    if (((symbols[0][0]) != 0xFFFF) && ((symbols[2][2]) != (symbols[0][0]))) {
                        clear = 1;
                    }
                    if (((symbols[1][1]) != 0xFFFF) && ((symbols[2][2]) != (symbols[1][1]))) {
                        clear = 1;
                    }
                    break;
                }
                break;
            }
            if (clear != 0) {
                CanStillGet3InARow_shslot &= (unsigned char)~(1 << column);
            }
        }
    }
    data_00351240_shslot[reel].center = symbols[reel][1];
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

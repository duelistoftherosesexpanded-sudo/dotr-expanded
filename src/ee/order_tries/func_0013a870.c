#include "dotr/ee/order_tries.h"

int TaLogic010_GetEmptyLine(int side)
{
    unsigned char row;
    int i;

    row = func_0013a9a0_side(side);
    for (i = 0; i < 5; i++) {
        switch (i) {
        case 0:
            if (func_001e36e0_try(3, row) == 0) {
                return (row << 8) | 3;
            }
            break;
        case 1:
            if (func_001e36e0_try(4, row) == 0) {
                return (row << 8) | 4;
            }
            break;
        case 2:
            if (func_001e36e0_try(2, row) == 0) {
                return (row << 8) | 2;
            }
            break;
        case 3:
            if (func_001e36e0_try(5, row) == 0) {
                return (row << 8) | 5;
            }
            break;
        case 4:
            if (func_001e36e0_try(1, row) == 0) {
                return (row << 8) | 1;
            }
            break;
        }
    }
    return -1;
}

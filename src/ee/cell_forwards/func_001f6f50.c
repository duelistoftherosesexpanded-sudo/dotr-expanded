#include "dotr/ee/cell_forwards.h"

int SzEffect_GetPlayMotion(int a)
{
    int packed = SzBattle_InqEntry(1);
    struct DotrForwardItem *item = func_001df540_forward(packed >> 8, packed & 0xff);
    int x2 = data_0031be98 >> 8;
    int y2 = data_0031be98 & 0xff;
    int x1 = data_0031be9c >> 8;
    int y1 = data_0031be9c & 0xff;
    int bit;
    signed char flag;
    unsigned char p, q;

    if (item == 0) {
        return -1;
    }
    if (x1 < 0 || x1 >= 3 || y1 < 0 || y1 >= 11 || x2 < 0 || x2 >= 3 || y2 < 0 || y2 >= 11) {
        return -1;
    }
    bit = item->status.unknown_bit_6;
    flag = bit;
    if (bit) {
        p = item->toMoveCol;
    } else {
        p = item->locCol;
    }
    if (flag) {
        q = item->toMoveRow;
    } else {
        q = item->locRow;
    }
    return SzDuel_GetPlayMotion(x1, y1, x2, y2, p, q, a);
}

#include "dotr/ee/state_grid.h"
int TaEnc_GetEnchantLst(unsigned short nCardNo, unsigned short *pEncLst)
{
    int i;
    int PushNum;
    unsigned short temp;
    temp = 752;
    PushNum = 0;
    for (i = 0; i < 49; i++) {
        if (PushNum >= 12) {
            return PushNum;
        }
        if ((EnchMatch[nCardNo * 7 + ((temp - 752) >> 3)] >> ((temp - 752) % 8)) & 1) {
            if (temp != 789 && temp != 780 && temp != 796) {
                *pEncLst++ = temp;
                PushNum++;
            }
        }
        temp++;
    }
    return PushNum;
}

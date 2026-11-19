#include "dotr/ee/sound_file.h"

int sendToIOP2area(int dst1, int dlen1, int dst2, int dlen2, int src1, int slen1, int src2, int slen2)
{
    int d;
    int over;

    if (dlen1 + dlen2 < slen1 + slen2) {
        over = (slen1 + slen2) - (dlen1 + dlen2);
        if (over >= slen2) {
            slen1 -= over - slen2;
            slen2 = 0;
        } else {
            slen2 -= over;
        }
    }
    if (slen1 >= dlen1) {
        sendToIOP(dst1, src1, dlen1);
        sendToIOP(dst2, src1 + dlen1, slen1 - dlen1);
        sendToIOP(dst2 + slen1 - dlen1, src2, slen2);
    } else {
    d = dlen1 - slen1;
    if (slen2 >= d) {
        sendToIOP(dst1, src1, slen1);
        sendToIOP(dst1 + slen1, src2, d);
        sendToIOP(dst2, src2 + dlen1 - slen1, slen2 - d);
    } else {
        sendToIOP(dst1, src1, slen1);
        sendToIOP(dst1 + slen1, src2, slen2);
    }
    }
    return slen1 + slen2;
}

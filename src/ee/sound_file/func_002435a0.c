#include "dotr/ee/sound_file.h"

int audioDecSendToIOP(struct DotrSoundDec *ad)
{
    int pd0, pd1, d0, d1;
    int ps0, ps1;
    int s0, s1;
    int count_sent = 0;
    int countAdj;
    int pos;

    switch (ad->state) {
    case 0:
        return 0;
    case 1:
        pd0 = ad->iopBuff + ad->totalBytesSent % ad->iopBuffSize;
        d0 = ad->iopBuffSize - ad->totalBytesSent;
        pd1 = 0;
        d1 = 0;
        break;
    case 2:
        pos = (func_00190810_ret3(1, 0x8100, 0) & 0xffffff) - ad->iopBuff;
        iopGetArea(&pd0, &d0, &pd1, &d1, ad, pos);
        break;
    case 3:
        return 0;
    }
    ps0 = ad->data + (ad->put - ad->count + ad->size) % ad->size;
    ps1 = ad->data;
    countAdj = (ad->count >> 10) << 10;
    s0 = ad->data + ad->size - ps0;
    if (countAdj < s0) {
        s0 = countAdj;
    }
    s1 = countAdj - s0;
    if (d0 + d1 >= 1024 && s0 + s1 >= 1024) {
        count_sent = sendToIOP2area(pd0, d0, pd1, d1, ps0, s0, ps1, s1);
    }
    ad->count -= count_sent;
    ad->totalBytesSent += count_sent;
    ad->iopLastPos = (ad->iopLastPos + count_sent) % ad->iopBuffSize;
    return count_sent;
}

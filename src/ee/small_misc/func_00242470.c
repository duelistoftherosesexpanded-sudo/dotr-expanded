#include "dotr/ee/small_misc.h"

void movie_play(int a, int b, int c)
{
    isWithAudio[0] = b;
    isCountVblank[28] = c;
    clearGsMem(0, 0, 0, 640, 448);
    func_001851e8(data_003503c0, 0, 640, 224, 0, 0, 1);
    func_0017bb60(0);
    isCountVblank[10] = 0x01200000;
    isCountVblank[12] = 0x0152A000;
    isCountVblank[14] = 0x015C3C80;
    isCountVblank[16] = 0x015C4CC0;
    isCountVblank[18] = 0x0178DE40;
    readBuf[0] = 0x0178E640;
    isCountVblank[20] = 0x017DE680;
    isCountVblank[22] = 0x017EA680;
    isCountVblank[24] = 0x0186A680;
    isCountVblank[26] = 0x0186E680;
    func_00242580(a);
}

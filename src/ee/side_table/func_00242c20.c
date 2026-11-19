#include "dotr/ee/small_misc.h"

void termAll(void)
{
    func_00241ac0_word(readBuf[0]);
    func_002457c0_object(&voBuf);
    func_0017b730(videoDecTh[0]);
    func_0017b6f0(videoDecTh[0]);
    func_0017b730(defaultTh[0]);
    func_0017b6f0(defaultTh[0]);
    func_0017c490(2);
    func_0017b610(2, data_00350730[0]);
    func_0017c3c0(2);
    func_0017b5e0(2, data_00350730[1]);
    videoDecDelete((struct DotrMiscObject *)videoDec);
    audioDecDelete(&audioDec);
    strFileClose((struct DotrMiscClose *)infile);
}

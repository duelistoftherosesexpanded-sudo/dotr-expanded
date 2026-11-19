#include "dotr/ee/small_misc.h"

/* Prints the counter at 0x3503b8 through func_00199b50 with the text at
 * 0x30f354 and increments it, then runs initAll(a),
 * readMpeg(0x350680, word 0x350788, 0x350740) and termAll, and
 * returns the word at 0x3505f0. */
int func_00242580(int a)
{
    func_00199b50(data_0030f354, data_003503b8[0]++);
    initAll(a);
    readMpeg(videoDec, readBuf[0], infile);
    termAll();
    return controller_val[0];
}

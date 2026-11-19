#include "dotr/ee/sound_file.h"

int audioDecCreate(int *p, int a, int b, int size)
{
    p[0] = 0;
    p[11] = 0;
    p[12] = a;
    p[13] = 0;
    p[14] = 0;
    p[15] = b;
    p[16] = 0;
    p[21] = 0;
    p[18] = size;
    p[19] = 0;
    p[20] = 0;
    p[17] = func_001828c0(size);
    if (p[17] < 0) {
        func_00199b50(data_0030f600, p[17]);
        return 0;
    }
    func_00199b50(data_0030f61c, p[17], size);
    p[22] = func_001828c0(0x800);
    if (p[22] < 0) {
        func_00199b50(data_0030f600, p[22]);
        return 0;
    }
    func_00199b50(data_0030f61c, p[22], 0x800);
    func_00198948(_0_buf, 0, 0x800);
    sendToIOP(p[22], (DotrSoundAddress)_0_buf, 0x800);
    changeMasterVolume(0x3fff);
    return 1;
}

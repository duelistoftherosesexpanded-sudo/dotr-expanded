#include "dotr/ee/sound_gate.h"

int SzSound_StereoPlaySE(int index, int a, int b)
{
    struct DotrSoundGate *g = data_0034f6e0_gate;
    if (!data_0034f6e0_gate[0].unknown_08.unknown_bit_0) {
        return 0;
    }
    if (index < 0 || index >= 4) {
        return 0;
    }
    if (g->unknown_00[index] < 0) {
        return 0;
    }
    if ((int)SD_iGetSDStatus() & 0x4000) {
        return 0;
    }
    a &= 0x7fff;
    b &= 0x7fff;
    SD_SEPlay2(index, a, b);
    return 1;
}

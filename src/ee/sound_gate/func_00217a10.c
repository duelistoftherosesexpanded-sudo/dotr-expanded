#include "dotr/ee/sound_gate.h"

int SzSound_StopBGM(int value)
{
    if (!data_0034f6e0_gate[0].unknown_08.unknown_bit_0) {
        return 0;
    }
    if (value > 0) {
        SD_BGMFadeOut(value);
        return 1;
    }
    SD_BGMStop();
    return 1;
}

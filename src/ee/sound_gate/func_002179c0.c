#include "dotr/ee/sound_gate.h"

int SzSound_IsLoadingBGM(void)
{
    if (!data_0034f6e0_gate[0].unknown_08.unknown_bit_0) {
        return 0;
    }
    return SD_IsLoadingNow() ? 1 : 0;
}

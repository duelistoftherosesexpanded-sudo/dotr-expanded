#include "dotr/ee/sound_gate.h"

int SzSound_IsPlaySE(void)
{
    if (!data_0034f6e0_gate[0].unknown_08.unknown_bit_0) {
        return 0;
    }
    return ((int)SD_iGetSDStatus() & 0x4000) ? 0 : 1;
}

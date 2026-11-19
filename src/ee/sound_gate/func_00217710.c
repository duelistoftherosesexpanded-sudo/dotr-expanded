#include "dotr/ee/sound_gate.h"

int SzSound_PlaySE(int index, int soundEffectNumber)
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
    SD_SEPlayOnlyOnce(index, soundEffectNumber & 0x7fff, (soundEffectNumber & 0x8000) ? 1 : 0);
    return 1;
}

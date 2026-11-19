#include "dotr/ee/sound_gate.h"

int SzSound_SetOutput(int flag)
{
    struct DotrSoundGate *g = data_0034f6e0_gate;
    if (!data_0034f6e0_gate[0].unknown_08.unknown_bit_0) {
        return 0;
    }
    g->unknown_08.unknown_bit_1 = (signed char)(flag ? 1 : 0);
    SD_SetOutputType(g->unknown_08.unknown_bit_1 ? 0 : 1);
    return 1;
}

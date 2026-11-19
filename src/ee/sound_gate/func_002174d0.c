#include "dotr/ee/sound_gate.h"

void SzSound_Term(void)
{
    struct DotrSoundGate *g = data_0034f6e0_gate;
    if (data_0034f6e0_gate[0].unknown_08.unknown_bit_0) {
        SD_Term();
        g->unknown_08.unknown_bit_0 = 0;
    }
}

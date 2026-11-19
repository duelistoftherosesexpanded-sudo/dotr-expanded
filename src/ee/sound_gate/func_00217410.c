#include "dotr/ee/sound_gate.h"

void SzSound_Init(void)
{
    struct DotrSoundGate *g = data_0034f6e0_gate;
    int i;
    RhMem_FillMemory(data_0034f6e0_gate, 0, 12);
    g->unknown_08.unknown_bit_0 = (signed char)(func_00197548(data_002d0930) ? 1 : 0);
    if (g->unknown_08.unknown_bit_0) {
        SD_Init((struct DotrSoundQueueManager *)0x400000);
        SD_BGMSetSourceLevel(128, 128);
        for (i = 0; i < 4; i++) {
            g->unknown_00[i] = -1;
        }
        SzSound_SetOutput(1);
    }
}

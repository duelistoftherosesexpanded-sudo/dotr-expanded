#include "dotr/ee/sound_gate.h"

int SzSound_TransferSEGrp(int index, int id, int force)
{
    struct DotrSoundGate *g = data_0034f6e0_gate;
    char buffer[128];
    if (!data_0034f6e0_gate[0].unknown_08.unknown_bit_0) {
        return 0;
    }
    if (index < 0 || index >= 4) {
        return 0;
    }
    if (!force) {
        if (id == g->unknown_00[index]) {
            return 1;
        }
    }
    g->unknown_00[index] = id;
    SD_SEStop(index, -1);
    switch (index) {
    case 0:
    case 1:
        func_0019b648(buffer, apchSEGrp[index], id);
        break;
    default:
        func_0019b648(buffer, apchSEGrp[index], id, id);
        break;
    }
    return SD_iTransferSEGrpToIop(index, buffer);
}

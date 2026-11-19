#include "dotr/ee/small_state.h"

int SzSound_PlayBGM(void)
{
    if (!data_0034f6e8[0].unknown_bit_0) {
        return 0;
    }
    SD_BGMPreLoad();
    return 1;
}

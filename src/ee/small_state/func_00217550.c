#include "dotr/ee/small_state.h"

void SzSound_VSync(void)
{
    if (data_0034f6e8[0].unknown_bit_0) {
        SD_CallEveryVSync();
    }
}

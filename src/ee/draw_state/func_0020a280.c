#include "dotr/ee/draw_state.h"

void RhBase_GetZBufInfo(struct DotrDrawFields *fields)
{
    struct DotrDrawFrameSetup *s = &data_00325f20_setup;

    if (data_00325f40[0].unknown_bit_0 && s->unknown_20.unknown_bit_1) {
        fields->low_9 = s->unknown_0c + s->unknown_1c;
        fields->low_4 = s->unknown_18;
    } else {
        fields->low_9 = data_00325fc0_fields[0].low_9;
        fields->low_4 = data_00325fc0_fields[0].low_4;
    }
}

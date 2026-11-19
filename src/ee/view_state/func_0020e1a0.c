#include "dotr/ee/view_state.h"

int RhView_Reset(struct DotrViewFrame *frame)
{
    if (frame == 0) {
        frame = (struct DotrViewFrame *)&gRhView_avl[gRhView_nStackIndex[0]].unknown_0c0;
    }
    RhGeom_UnitMatrix(&frame->unknown_00);
    frame->unknown_40[0] = 0.0f;
    frame->unknown_40[1] = 0.0f;
    frame->unknown_40[2] = 0.0f;
    frame->unknown_40[3] = 1.0f;
    frame->unknown_40[4] = 0.0f;
    frame->unknown_40[5] = 0.0f;
    frame->unknown_40[6] = -1.0f;
    frame->unknown_40[7] = 0.0f;
    frame->unknown_40[16] = 0.0f;
    frame->unknown_40[8] = 0.0f;
    frame->unknown_40[9] = 0.0f;
    frame->unknown_40[10] = 0.0f;
    frame->unknown_40[11] = 1.0f;
    frame->unknown_40[12] = 0.0f;
    frame->unknown_40[13] = 0.0f;
    frame->unknown_40[14] = -1.0f;
    frame->unknown_40[15] = 0.0f;
    frame->unknown_40[17] = 0.0f;
    frame->unknown_40[18] = 0.0f;
    frame->unknown_40[19] = 0.0f;
    return 1;
}

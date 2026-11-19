#include "dotr/ee/draw_state.h"

/* Ends the frame: closes the sort list, queues an end tag, waits for the drawing path and the
 * vertical syncs still owed, swaps the buffers, starts the DMA chain of the finished packet
 * area, switches to the other packet area and reports whether the frame overran (2) or not (1). */
int RhBase_FlipBuffer(void)
{
    struct DotrDrawState *s = &gRhBase_dl;
    int r;
    int height;
    int per;
    int n;
    int *p;

    RhZSort_Finish();
    RhFile_VSyncProcess();
    RhMemCard_Act();
    p = (int *)data_003261d0[0];
    p[0] = 0x70000000;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    DOTR_DRAW_T1_COUNT;
    if (func_00185578(0, 0) < 0) {
        RhBase_ResetGsEnv(data_00326180[2]);
    }
    if (s->unknown_298 != 0) {
        s->unknown_298();
    }
    n = frameSkipCounter;
    if (*(int *)(data_00293f14 + data_00326180[2] * 40) == 2) {
        height = 448;
        per = 520;
    } else {
        height = 512;
        per = 620;
    }
    while (n-- >= 0) {
        r = func_001854e0(0);
        if (s->unknown_240 == height) {
            if (r != 0) {
                r = func_001854e0(0);
            }
        }
        if (DOTR_DRAW_T1_COUNT > (unsigned)(per * (frameSkipCounter + 1))) {
            break;
        }
    }
    frameSkipCounter = 0;
    if (s->unknown_294 != 0) {
        s->unknown_294();
    }
    if (*(int *)&s->unknown_24c[0] != 0) {
        func_00186060((s->unknown_26c & 1) ? &s->unknown_108[0x48] : &s->unknown_000[0x60], *(short *)&s->unknown_260,
                      *(short *)&s->unknown_264, r);
    }
    func_0017bb60(0);
    func_00185480(s, s->unknown_26c);
    if (SzGame_InqSoftFocus()) {
        useSoftFocus(1);
    }
    func_00185578(0, 0);
    DOTR_DRAW_D1_QWC = 0;
    DOTR_DRAW_D1_TADR = s->unknown_27c & 0x0fffffff;
    DOTR_DRAW_D_STAT = 2;
    func_0017bb60(0);
    DOTR_DRAW_D1_CHCR = 0x145;
    if (s->unknown_288 != 0) {
        s->unknown_27c = s->unknown_290;
        s->unknown_280 = s->unknown_290;
        s->unknown_288 = 0;
    } else {
        s->unknown_27c = s->unknown_28c;
        s->unknown_280 = s->unknown_28c;
        s->unknown_288 = 1;
    }
    s->unknown_26c++;
    r = 1;
    if (*(int *)(data_00293f14 + data_00326180[2] * 40) == 2) {
        if (s->unknown_240 == 448) {
            if (DOTR_DRAW_T1_COUNT > 540) {
                r = 2;
            }
        } else {
            if (DOTR_DRAW_T1_COUNT > 270) {
                r = 2;
            }
        }
    } else {
        if (DOTR_DRAW_T1_COUNT > 640) {
            r = 2;
        }
    }
    DOTR_DRAW_T1_COUNT = 0;
    RhZSort_Start(s->unknown_27c, s->unknown_27c + s->unknown_284);
    RhBase_SetVifPacketArea(lRhBase_SetRandomDither(RhBase_GetVifPacketArea()));
    return r;
}

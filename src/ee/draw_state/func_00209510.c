#include "dotr/ee/draw_state.h"

/* Clears the 672-byte state, starts the file layer and, when that succeeds, sets up the
 * display for table row `mode`, timer 1, the packet buffers and the sort list. */
int RhBase_Init(int mode, int a1, int a2, int a3, int a4, int a5, int a6)
{
    struct DotrDrawState *b = &gRhBase_dl;
    struct DotrDrawEnv env;
    struct DotrDrawTimerMode tm;
    int *q;

    func_00198948(&gRhBase_dl, 0, 672);
    gRhBase_dl.unknown_278 = a6;
    if (!RhFile_Init()) {
        return 0;
    }
    func_00186fc8();
    func_00186bd0();
    func_001849b0();
    func_00186148(1);
    func_00186410(&env);
    env.unknown_06 = 256;
    func_00186238(&env);
    func_00184810(0, *(short *)(gRhSys_asd + mode * 40), *(short *)(data_00293f14 + mode * 40), *(short *)(data_00293f18 + mode * 40));
    b->unknown_274 = func_00186120(1);
    ((struct DotrDrawBit6 *)b->unknown_274)->unknown_bit_6 = 1;
    lRhBase_InitScreenVariables(b, mode, a1, a2);
    tm.CLKS = 3;
    tm.GATE = 0;
    tm.GATS = 0;
    tm.GATM = 0;
    tm.ZRET = 0;
    tm.CUE = 1;
    tm.CMPE = 0;
    tm.OVFE = 0;
    tm.EQUF = 0;
    tm.OVFF = 1;
    tm.p0 = 0;
    DOTR_DRAW_T1_MODE = *(unsigned *)&tm;
    DOTR_DRAW_T1_COUNT = 0;
    b->unknown_28c = a3;
    b->unknown_290 = a4;
    b->unknown_284 = a5;
    b->unknown_288 = 1;
    b->unknown_27c = a3;
    b->unknown_280 = a3;
    func_0017b770(func_0017b7d0(), 32);
    b->unknown_294 = 0;
    b->unknown_298 = 0;
    q = lRhBase_InitGsAndVif(b->unknown_27c);
    RhGeom_Init();
    RhView_Init();
    RhMemCard_Init();
    q[0] = 0x70000000;
    q[1] = 0;
    q[2] = 0;
    q[3] = 0;
    func_0017bb60(0);
    func_00185578(0, 0);
    func_00186470(b->unknown_274, b->unknown_27c);
    func_00185578(0, 0);
    func_00186470(b->unknown_274, (DotrDrawWord)data_00286980);
    func_00185578(0, 0);
    RhZSort_Start(b->unknown_27c, b->unknown_27c + b->unknown_284);
    func_001854e0(0);
    return 1;
}

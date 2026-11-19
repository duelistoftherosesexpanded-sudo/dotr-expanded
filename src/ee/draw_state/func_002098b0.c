#include "dotr/ee/draw_state.h"

/* Resets the view state, then writes table row i (shorts to func_00184810, two
 * words to the state table) and queues a 0x70000000 packet before redrawing. */
void RhBase_ResetGsEnv(int i)
{
    int *q;
    func_001869c0(data_00326180[17], 0, 0);
    func_00186148(1);
    func_00185578(0, 0);
    func_001849b0();
    func_001885c8();
    i = i * 40;
    func_00184810(0, *(short *)(gRhSys_asd + i), *(short *)(data_00293f14 + i), *(short *)(data_00293f18 + i));
    data_00326180[3] = *(int *)(data_00293f1c + i);
    data_00326180[4] = *(int *)(data_00293f20 + i);
    q = lRhBase_InitGsAndVif(data_00326180[19]);
    q[0] = 0x70000000;
    q[1] = 0;
    q[2] = 0;
    q[3] = 0;
    func_0017bb60(0);
    func_00185578(0, 0);
    func_00186470(data_00326180[17], data_00326180[19]);
    func_00185578(0, 0);
}

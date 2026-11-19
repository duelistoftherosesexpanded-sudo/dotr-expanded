#include "dotr/ee/boot_sequence.h"

void Main_Loop(void)
{
    func_00209f60_entry(SzEffect_StoneProc);
    for (;;) {
        if (SzGame_GetMode(1) == 1) {
            lMain_LoadModel();
        }
        Main_LoopProc();
    }
}

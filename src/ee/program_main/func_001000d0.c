/* main (0x1000d0, 244 bytes), the program entry: installs its stack, starts the memory
 * manager, registers seven name pairs with the SDK routine at 0x19b0f8, then runs Main_Init,
 * Main_Loop and Main_Term.
 * Yu-Gi-Oh! The Duelists of the Roses, NTSC-U SLUS_205.15. Evidence: docs/modules/main.md.
 *
 * C with an inline hardware block: the block sets the stack pointer to 0x912000, an operation C cannot
 * express (docs/DECOMPILATION.md, "What a source may contain"). The host build skips the block.
 */
#include "dotr/ee/program_main.h"

s32 main(void) {
#ifdef __MWERKS__
    asm {
        la sp, data_00912000
    }
#endif
    SzMem_Init();
    func_0019b0f8((s32)data_002b8f00, (s32)data_002b8f05, 1);
    func_0019b0f8((s32)data_002b8f07, (s32)data_002b8f05, 1);
    func_0019b0f8((s32)data_002b8f0d, (s32)data_002b8f05, 1);
    func_0019b0f8((s32)data_002b8f13, (s32)data_002b8f18, 1);
    func_0019b0f8((s32)data_002b8f1f, (s32)data_002b8f18, 1);
    func_0019b0f8((s32)data_002b8f27, (s32)data_002b8f18, 1);
    func_0019b0f8((s32)data_002b8f2c, (s32)data_002b8f32, 1);
    if (Main_Init() == 0) {
        return 0;
    }
    Main_Loop();
    Main_Term();
    return 0;
}

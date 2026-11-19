#include "dotr/ee/small_state.h"

/* Clears 704 bytes of the table, presets both entries and passes the two
 * 256-byte halves of its prefix to 0x1927e8. */
void SzPad_Init(void)
{
    RhMem_FillMemory(&gSzPad_pl, 0, 704);
    gSzPad_pl.unknown_200[0].unknown_59 = 10;
    gSzPad_pl.unknown_200[1].unknown_59 = 10;
    gSzPad_pl.unknown_200[0].unknown_5a.unknown_bits_0_3 = 4;
    gSzPad_pl.unknown_200[1].unknown_5a.unknown_bits_0_3 = 4;
    func_00192588(0);
    func_001927e8(0, 0, &gSzPad_pl);
    func_001927e8(1, 0, &gSzPad_pl.unknown_000[0x100]);
}

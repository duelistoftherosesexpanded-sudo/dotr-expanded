#include "dotr/ee/draw_state.h"

struct DotrDrawTag *RhVu1Mac_SetViewScreenMatrix(struct DotrDrawTag *tag, void *source)
{
    tag->unknown_00 = 0x10000004;
    tag->unknown_04 = 0;
    tag->unknown_08 = 0;
    tag->unknown_0c = 0x6c040044;
    RhMem_CopyMemory16(tag + 1, source, 4);
    return tag + 5;
}

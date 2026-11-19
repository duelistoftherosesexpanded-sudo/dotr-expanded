#include "dotr/ee/draw_state.h"

struct DotrDrawTag *RhVu1Mac_ReserveChangeDrawMethod(struct DotrDrawTag *tag)
{
    tag->unknown_00 = 0x20000000;
    tag->unknown_04 = (DotrDrawWord)(tag + 2);
    tag->unknown_08 = 0;
    tag->unknown_0c = 0;
    return tag + 2;
}

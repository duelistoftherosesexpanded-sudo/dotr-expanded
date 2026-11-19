#include "dotr/ee/overlay.h"

/* Evidence: docs/modules/other.md. */
void __initialize_cpp_rts(DotrOverlayInitializer *begin, DotrOverlayInitializer *end,
                   int unused_2, int unused_3)
{
    DotrOverlayInitializer *current = begin;
    (void)unused_2;
    (void)unused_3;
    while (current < end) {
        (*current)();
        current++;
    }
}

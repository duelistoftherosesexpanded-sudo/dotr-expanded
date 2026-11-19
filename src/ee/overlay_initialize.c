#include "dotr/ee/overlay.h"

/* Callee signatures describe only the arguments passed here.
 * Evidence: docs/modules/other.md.
 */
extern void func_0017bb60(int mode);
extern void *func_00198948(void *destination, int fill, unsigned int count);

void mwOverlayInit(void *address, int initialized_size)
{
    unsigned char *cursor = (unsigned char *)address;
    struct DotrOverlayInitPrefix *header = (struct DotrOverlayInitPrefix *)address;
    unsigned int zero_fill_size = header->zero_fill_size;
    func_0017bb60(2);
    if (zero_fill_size) {
        cursor += initialized_size;
        func_00198948(cursor, 0, zero_fill_size);
    }
    __initialize_cpp_rts(header->initializers_begin, header->initializers_end, 0, 0);
}

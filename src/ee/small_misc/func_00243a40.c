#include "dotr/ee/small_misc.h"

/* Two calls of 0x190810 with selectors 3968 and 4224. */
void changeInputVolume(int value)
{
    func_00190810(1, 0x8010, 3968, value);
    func_00190810(1, 0x8010, 4224, value);
}

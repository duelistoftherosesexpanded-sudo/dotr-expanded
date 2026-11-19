#include "dotr/ee/sound_file.h"

/* For i = 0 and 1 calls func_00190810(1, 0x8010, i | 0x980, a) and then
 * the same with i | 0xa80. */
void changeMasterVolume(int a)
{
    int i;
    for (i = 0; i < 2; i++) {
        func_00190810(1, 0x8010, i | 0x980, a);
        func_00190810(1, 0x8010, i | 0xa80, a);
    }
}

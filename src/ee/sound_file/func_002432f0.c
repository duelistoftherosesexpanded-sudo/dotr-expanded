#include "dotr/ee/sound_file.h"

/* Same sequence as audioDecPause, then clears the state word and the words
 * at +0x2c, +0x34, +0x38, +0x40, +0x54, +0x4c and +0x50. */
void audioDecReset(struct DotrSoundVoice *p)
{
    p->unknown_00 = 3;
    changeInputVolume(0);
    p->unknown_50 = (func_00190810_ret6(1, 0x80e0, 0, 2, 0, 0) & 0xffffff) - p->unknown_44;
    func_00190810_args7(1, 0x80d0, 0, 0, p->unknown_58, 16384, 2048);
    p->unknown_00 = 0;
    p->unknown_2c = 0;
    p->unknown_34 = 0;
    p->unknown_38 = 0;
    p->unknown_40 = 0;
    p->unknown_54 = 0;
    p->unknown_4c = 0;
    p->unknown_50 = 0;
}

#include "dotr/ee/sound_file.h"

/* Sets the state word to 3, calls changeInputVolume(0), stores the low 24 bits
 * of the six-argument func_00190810 result minus the word at +0x44 at
 * +0x50, then issues the seven-argument func_00190810 call with the word
 * at +0x58. */
void audioDecPause(struct DotrSoundStream *p)
{
    p->unknown_00 = 3;
    changeInputVolume(0);
    p->unknown_50 = (func_00190810_ret6(1, 0x80e0, 0, 2, 0, 0) & 0xffffff) - p->unknown_44;
    func_00190810_args7(1, 0x80d0, 0, 0, p->unknown_58, 16384, 2048);
}

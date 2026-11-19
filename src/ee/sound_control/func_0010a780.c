#include "dotr/ee/sound_control.h"

/* The remainder below compiles on the target to a mask whose negative-value
 * adjustment adds zero; host and target therefore agree only for nonnegative
 * positions.
 */
int _iDecodeLPCM(int position)
{
    short *source;
    short *left;
    int block;
    unsigned int sample;
    short *right;
    int l;
    int r;
    func_0017bb60(0);
    source = (short *)(g_SDBuffer[0] + 2048);
    source += position >> 1;
    left = (short *)(g_SDBuffer[0] + 0x181800);
    for (block = 0; block < 128; block++) {
        right = left + 256;
        for (sample = 0; sample < 128; sample++) {
            l = source[0];
            r = source[1];
            source += 2;
            if (g_SDValue[0]->unknown_2a6c) {
                r = (short)(((short)l + (short)r) >> 1);
                l = r;
            }
            left[0] = l;
            left[1] = l;
            right[0] = r;
            right[1] = r;
            left += 2;
            right += 2;
            if ((DotrSoundControlWord)source >= (DotrSoundControlWord)(g_SDBuffer[0] + 2048) + 0x100000) {
                source = (short *)((DotrSoundControlWord)source - 0x100000);
            }
        }
        left = right;
        position += 512;
        position %= 0x100000;
    }
    return position;
}

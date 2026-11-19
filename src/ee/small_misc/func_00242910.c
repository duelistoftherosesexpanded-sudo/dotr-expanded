#include "dotr/ee/small_misc.h"

int isAudioOK(void)
{
    if (isWithAudio[0]) return audioDecIsPreset(&audioDec);
    return 1;
}

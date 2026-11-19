#include "dotr/ee/sound_control.h"

static inline unsigned int flags(void)
{
    /* The explicit else and unsigned return reproduce the inlined branch and
     * zero extension; this helper's original source relationship is unknown.
     */
    if (g_SDValue[0] == 0) {
        return 0;
    } else {
        return g_SDValue[0]->unknown_0000;
    }
}

int SD_BGMGetVolume(void)
{
    if (!g_SDValue[0] || !g_SDBuffer[0]) {
        return 0;
    }
    if ((unsigned long)flags() & 0x100) {
        return g_SDValue[0]->unknown_01ec;
    }
    return 0;
}

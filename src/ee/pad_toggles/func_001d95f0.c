#include "dotr/ee/pad_toggles.h"

void SzDuel_ActCardTest(void)
{
    struct DotrPadSteps *p = &gSzDuel_dt;

    if (SzPad_Check(0, 2, 0x1000)) {
        p->unknown_5c = (p->unknown_5c + 0x10400) % 0x10000;
    }
    if (SzPad_Check(0, 2, 0x4000)) {
        p->unknown_5c = (p->unknown_5c + 0xfc00) % 0x10000;
    }
    if (SzPad_Check(0, 2, 0x2000)) {
        p->unknown_60 = (p->unknown_60 + 0x10400) % 0x10000;
    }
    if (SzPad_Check(0, 2, 0x8000)) {
        p->unknown_60 = (p->unknown_60 + 0xfc00) % 0x10000;
    }
    if (SzPad_Check(0, 2, 8)) {
        p->unknown_64 = (p->unknown_64 + 0x10400) % 0x10000;
    }
    if (SzPad_Check(0, 2, 4)) {
        p->unknown_64 = (p->unknown_64 + 0xfc00) % 0x10000;
    }
    if (SzPad_Check(0, 2, 2)) {
        p->unknown_70 = (p->unknown_70 + 855) % 854;
    }
    if (SzPad_Check(0, 2, 1)) {
        p->unknown_70 = (p->unknown_70 + 853) % 854;
    }
    if (SzPad_Check(0, 0, 16)) {
        SzDebug_OutputFrameBuffer();
    }
}

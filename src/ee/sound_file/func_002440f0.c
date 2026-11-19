#include "dotr/ee/sound_file.h"

void viBufBeginPut(struct DotrSoundStream *p, int *first, int *first_size, int *second, int *second_size)
{
    int pos;
    int avail;
    int rd;
    int fr;

    func_0017b920(p->unknown_40);
    rd = p->unknown_0c + p->unknown_10;
    fr = p->unknown_08 - 2 - p->unknown_10;
    pos = ((rd << 11) + p->unknown_14) % p->unknown_18;
    avail = (fr << 11) - p->unknown_14;
    if (p->unknown_18 - pos >= avail) {
        *first = p->unknown_00 + pos;
        *first_size = avail;
        *second = 0;
        *second_size = 0;
    } else {
        *first = p->unknown_00 + pos;
        *first_size = p->unknown_18 - pos;
        *second = p->unknown_00;
        *second_size = avail - (p->unknown_18 - pos);
    }
    func_0017b900(p->unknown_40);
}

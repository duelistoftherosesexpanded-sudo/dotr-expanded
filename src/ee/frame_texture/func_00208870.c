#include "dotr/ee/frame_texture.h"

DotrDrawWord ExFrameBuffer2Texture_Pipe(DotrDrawWord cursor, int tbp, int filter, int c, int d)
{
    struct DotrDrawBits64A *a = (struct DotrDrawBits64A *)(DotrDrawWord)c;
    struct DotrDrawBits64B *b = (struct DotrDrawBits64B *)(DotrDrawWord)d;
    unsigned long *pkt = (unsigned long *)cursor;
    int fbp;
    int psm;
    int w;
    int h;
    int tw;
    int th;
    unsigned long *base = pkt;
    int n;

    RhBase_GetBufferFormatEx(tbp ? 1 : 0, &fbp, &psm, 0);
    RhBase_GetScreenSize(&w, &h);
    for (tw = 0; (1 << tw) < w; tw++) {
    }
    for (th = 0; (1 << th) < h; th++) {
    }
    if (a != 0) {
        a->unknown_bits_0_13 = tbp > 0 ? tbp : fbp;
        a->unknown_bits_14_19 = w >> 6;
        a->unknown_bits_20_25 = psm;
        a->unknown_bits_26_29 = tw;
        a->unknown_bits_30_33 = th;
        a->unknown_bit_34 = 0;
        a->unknown_bits_35_36 = 0;
        a->unknown_bits_37_50 = 0;
        a->unknown_bits_51_54 = 0;
        a->unknown_bit_55 = 0;
        a->unknown_bits_56_60 = 0;
        a->unknown_bits_61_63 = 1;
    }
    if (b != 0) {
        b->unknown_bit_0 = 0;
        b->unknown_bits_2_4 = 0;
        b->unknown_bit_5 = filter;
        b->unknown_bits_6_8 = filter;
        b->unknown_bit_9 = 0;
        b->unknown_bits_19_20 = 0;
        b->unknown_bits_32_43 = 0;
    }
    if (tbp > 0) {
        pkt[1] = 0;
        pkt[0] = 0;
        ((unsigned char *)pkt)[3] = 16;
        pkt[2] = 0x1000000000008004;
        pkt[3] = 14;
        pkt[4] = (long)fbp | ((long)(w >> 6) << 16) | ((long)psm << 24) | ((long)tbp << 32)
            | ((long)(w >> 6) << 48) | ((long)psm << 56);
        pkt[5] = 0x50;
        pkt[6] = 0;
        pkt[7] = 0x51;
        pkt[8] = (long)w | ((long)h << 32);
        pkt[9] = 0x52;
        pkt[10] = 2;
        pkt[11] = 0x53;
        pkt += 12;
        n = ((pkt - base) >> 1) - 1;
        *(short *)base = n;
        ((int *)base)[2] = 0x11000000;
        ((int *)base)[3] = (n & 0xffff) | 0x51000000;
    }
    return (DotrDrawWord)pkt;
}

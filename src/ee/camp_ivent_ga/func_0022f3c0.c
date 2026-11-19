#include "dotr/ee/camp_ivent_ga.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */
/* 0x22f3c0. Its two switch tables (0x30ef30 and 0x30ef50) are compared as one range. The inline
 * helper is in camp_ivent_ga.h; the work area is addressed by its literal through AT and read and
 * written through the access macros B8/U8/H16/U16/F32. */
#ifndef __MWERKS__
/* Host build only: the code leaves some locals unset on some paths, which the target compiler
 * accepts. */
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

/* MATCHING DEVICE: the work area by its literal addresses (camp_ivent_ga.h). */
#define AT(a) DOTR_IVENT_GA_AT(a)

#define B8(a) (*(signed char *)(a))
#define U8(a) (*(unsigned char *)(a))
#define H16(a) (*(short *)(a))
#define U16(a) (*(unsigned short *)(a))
#define F32(a) (*(float *)(a))

int Taku_Camp_Ivent_Ga(int cursor) {
    float fv;
    unsigned char *ctx = (unsigned char *)&data_00299e10_iventga;
    int x13;
    unsigned char *wk = (unsigned char *)AT(0x01ED0000);
    DotrIventGaObj base = data_0029abf0_iventga;
    DotrIventGaObj4 bars = data_0029ac20_iventga;
    DotrIventGaObj4 frame = data_0029acc0_iventga;
    DotrIventGaObj5 page = data_0029ad60_iventga;
    DotrIventGaObj bar;
    DotrIventGaObj lamp;
    DotrIventGaObj lamp2;
    DotrIventGaPair size;
    short w;
    short h;
    short off;
    short i;
    short x;
    short y;
    short xa;
    short ya;
    int fya;
    DotrIventGaObj *pa;
    short ja;
    short ia;
    short yb;
    int fyb;
    short jb;
    short n1;
    short *tp1;
    int mm1;
    short m1;
    short x1p;
    short y1p;
    short t1;
    DotrIventGaTex *tex1;
    unsigned short idx1;
    short r2;
    short g2;
    short b2;
    short a2;
    short n2;
    short px2;
    DotrIventGaTex *tex2;
    short i3;
    short r3;
    short g3;
    DotrIventGaObj *pb;
    int x23;
    short b3;
    short px3;
    short c33;
    short xb;
    short k3;
    int q3;
    int dt3;
    DotrIventGaTex *tex3;
    short cnt4;
    float fx4;
    float fz4;
    short alpha4;
    short iq;

    off = 0;
    w = 0;
    h = 0;
    switch (CutSceneNumber_iventga) {
    case 0x2B0C:
        w = 0x280;
        h = 0x1C0;
        break;
    case 0x2B72:
        w = 0x280;
        h = 0x180;
        off = 0x40;
        break;
    case 0x2B86:
        w = 0x280;
        h = 0x180;
        off = 0x40;
        break;
    case 0x2B88:
        w = 0x280;
        h = 0x1C0;
        break;
    case 0x2B8A:
        w = 0x280;
        h = 0x1C0;
        break;
    case 0x2BAE:
        w = 0x200;
        h = 0x180;
        off = 0x40;
        break;
    case 0x2F7A:
        w = 0x200;
        h = 0x180;
        off = 0x40;
        break;
    }
    for (i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            frame.o[i].w[0] = 0;
            frame.o[i].w[1] = 0;
            break;
        case 1:
            frame.o[i].w[0] = w;
            frame.o[i].w[1] = 0;
            break;
        case 2:
            frame.o[i].w[0] = 0;
            frame.o[i].w[1] = h;
            break;
        case 3:
            frame.o[i].w[0] = w;
            frame.o[i].w[1] = h;
            break;
        }
        frame.o[i].w[2] = w + frame.o[i].w[0];
        frame.o[i].w[3] = h + frame.o[i].w[1];
        tex_uv_data[i + 0x94].f[0] = 0;
        tex_uv_data[i + 0x94].f[1] = 0;
        tex_uv_data[i + 0x94].f[2] = w;
        tex_uv_data[i + 0x94].f[3] = h;
    }
    if (B8(AT(0x01ED53AC)) != 0) {
        switch (H16(ctx + 0xC2)) {
        case 0x2B88:
            for (ia = 0; ia < 2; ia++) {
                frame.o[ia].w[0] = frame.o[0].w[0];
                frame.o[ia].w[1] = frame.o[0].w[1];
                frame.o[ia].w[2] = frame.o[0].w[2];
                frame.o[ia].w[3] = frame.o[0].w[3];
            }
            xa = H16(AT(0x01ED53AE));
            ya = H16(AT(0x01ED53B0));
            if (B8(AT(0x01ED54A0)) != 0) {
                frame.o[0].col[0][3] = U8(AT(0x01ED54A2));
                base.col[3][3] = 0x80;
                frame.o[3].col[0][3] = 0x80;
                base.col[2][3] = 0x80;
                frame.o[2].col[0][3] = 0x80;
                base.col[1][3] = 0x80;
                frame.o[1].col[0][3] = 0x80;
                base.col[0][3] = 0x80;
            }
            cursor = Taku_G4_Set(cursor, &base, 0, 0);
            fya = ya + off;
            pa = &frame.o[1];
            for (ja = 1; ja >= 0; ja--) {
                cursor = dotr_camp_ivent_ga_obj(cursor, pa, xa, fya);
                pa--;
            }
            switch (B8(AT(0x01ED54A0))) {
            case 1:
                if (0 < H16(AT(0x01ED54A2))) {
                    H16(AT(0x01ED54A2)) -= 2;
                    if (H16(AT(0x01ED54A2)) <= 0) {
                        H16(AT(0x01ED54A2)) = 0;
                        B8(AT(0x01ED54A0)) = 10;
                    }
                }
                break;
            }
            break;
        case 0x2B0C:
        case 0x2B72:
        case 0x2B86:
        case 0x2B8A:
        case 0x2BAE:
        case 0x2F7A:
            xb = H16(AT(0x01ED53AE));
            yb = H16(AT(0x01ED53B0));
            if (B8(AT(0x01ED54A0)) != 0) {
                base.col[0][3] = frame.o[0].col[0][3] = base.col[1][3] = frame.o[1].col[0][3] =
                    base.col[2][3] = frame.o[2].col[0][3] = base.col[3][3] = frame.o[3].col[0][3] =
                        U8(AT(0x01ED54A2));
            }
            cursor = Taku_G4_Set(cursor, &base, 0, 0);
            fyb = yb + off;
            for (jb = 0; jb < 4; jb++) {
                cursor = dotr_camp_ivent_ga_obj(cursor, &frame.o[jb], xb, fyb);
            }
            switch (B8(AT(0x01ED54A0))) {
            case 1:
                if (0 < H16(AT(0x01ED54A2))) {
                    H16(AT(0x01ED54A2)) -= 2;
                    if (H16(AT(0x01ED54A2)) <= 0) {
                        H16(AT(0x01ED54A2)) = 0;
                        B8(AT(0x01ED54A0)) = 10;
                    }
                }
                break;
            }
            break;
        }
    }
    for (iq = 0xAB; iq < 0xAF; iq++) {
        tex_uv_data[iq].f[2] = 0x100;
        tex_uv_data[iq].f[3] = 0x100;
    }

    if (H16(ctx + 0xC2) == 0x2724) {
        if (B8(AT(0x01ED5436)) != 0) {
        for (m1 = 0, tp1 = (short *)AT(0x01ED0000); (mm1 = m1) < 5; m1++, tp1++) {
            for (n1 = 0; n1 < 2; n1++) {
                bar = data_0029ae20_iventga;

                idx1 = bar.code;
                tex_uv_data[idx1].f[4] = 0;
                tex_uv_data[idx1].f[3] = 0;
                tex_uv_data[idx1].f[1] = 0;
                tex_uv_data[idx1].f[0] = 0;
                tex_uv_data[idx1].f[7] = 0x100;
                tex_uv_data[idx1].f[6] = 0x100;
                tex_uv_data[idx1].f[5] = 0x100;
                tex_uv_data[idx1].f[2] = 0x100;
                if (n1 == 0) {
                    tex_uv_data[idx1].f[3] = 0;
                    tex_uv_data[idx1].f[1] = 0;
                    tex_uv_data[idx1].f[7] = 0xB8;
                    tex_uv_data[idx1].f[5] = 0xB8;
                    bar.w[7] = 0xB8;
                    bar.w[5] = 0xB8;
                } else {
                    tex_uv_data[idx1].f[3] = 0xB8;
                    tex_uv_data[idx1].f[1] = 0xB8;
                    tex_uv_data[idx1].f[7] = 0x100;
                    tex_uv_data[idx1].f[5] = 0x100;
                    bar.w[3] = 0xB8;
                    bar.w[1] = 0xB8;
                    bar.w[7] = 0x100;
                    bar.w[5] = 0x100;
                    bar.col[1][3] = 0x40;
                    bar.col[0][3] = 0x40;
                    bar.col[3][3] = 0;
                    bar.col[2][3] = 0;
                }
                switch (mm1) {
                case 0:
                    t1 = tp1[0x2A16];
                    x1p = 0x280 - t1 * 0x200 / 100;
                    y1p = t1 * 300 / 100 - 200;
                    tex1 = &tex_uv_data[bar.code];
                    tex1->f[6] = 0x180;
                    tex1->f[2] = 0x180;
                    tex1->f[8] = 6;
                    bar.w[6] = 0x180;
                    bar.w[2] = 0x180;
                    break;
                case 1:
                    t1 = tp1[0x2A16];
                    x1p = t1 * 448 / 100 - 0x100;
                    y1p = t1 * 200 / 100 - 100;
                    tex_uv_data[bar.code].f[8] = 7;
                    break;
                case 2:
                    t1 = tp1[0x2A16];
                    x1p = 0x280 - t1 * 448 / 100;
                    y1p = 300 - t1 * 200 / 100;
                    tex_uv_data[bar.code].f[8] = 8;
                    break;
                case 3:
                    x1p = 0x40;
                    y1p = 100;
                    tex1 = &tex_uv_data[bar.code];
                    tex1->f[6] = 0x200;
                    tex1->f[2] = 0x200;
                    tex1->f[8] = 9;
                    bar.w[0] = 0x100 - tp1[0x2A16] * 256 / 100;
                    bar.w[1] = tp1[0x2A16] * (tex_uv_data[bar.code].f[3] - 0x80) / 100 + 0x80;
                    bar.w[2] = tp1[0x2A16] * 256 / 100 + 0x100;
                    bar.w[3] = tp1[0x2A16] * (tex_uv_data[bar.code].f[3] - 0x80) / 100 + 0x80;
                    bar.w[4] = 0x100 - tp1[0x2A16] * 256 / 100;
                    bar.w[5] = tp1[0x2A16] * (tex_uv_data[bar.code].f[7] - 0x80) / 100 + 0x80;
                    bar.w[6] = tp1[0x2A16] * 256 / 100 + 0x100;
                    bar.w[7] = tp1[0x2A16] * (tex_uv_data[bar.code].f[7] - 0x80) / 100 + 0x80;
                    if (tp1[0x2A16] > 100) {
                        switch (n1) {
                        case 0:
                            bar.col[2][3] = bar.col[3][3] = 0x40 - (tp1[0x2A16] - 100) * 64 / 100;
                            break;
                        }
                        bar.col[0][3] = bar.col[1][3] = 0x40 - (tp1[0x2A16] - 100) * 64 / 100;
                    }
                    break;
                case 4:
                    t1 = tp1[0x2A16];
                    x1p = t1 * 448 / 100 - 0x100;
                    y1p = 300 - t1 * 200 / 100;
                    tex_uv_data[bar.code].f[8] = 10;
                    break;
                }
                cursor = Taku_GT4_Set(cursor, &bar, x1p, y1p, 0, 0);
            }
        }
        }
    } else if (H16(ctx + 0xC2) == 0x2B9A || H16(ctx + 0xC2) == 0x2F64) {
        a2 = b2 = g2 = r2 = 0x80;
        for (n2 = 0; n2 < 2; n2++) {
            lamp = data_0029ae50_iventga;

            tex2 = &tex_uv_data[lamp.code];
            tex2->f[4] = 0;
            tex2->f[3] = 0;
            tex2->f[1] = 0;
            tex2->f[0] = 0;
            tex2->f[6] = 0x200;
            tex2->f[2] = 0x200;
            tex2->f[7] = 0x100;
            tex2->f[5] = 0x100;
            switch (n2) {
            case 0:
                tex2->f[8] = 6;
                px2 = B8(wk + 0x5437) * 576 / 100 - 0x200;
                break;
            case 1:
                switch (B8(wk + 0x546E)) {
                case 0:
                    r2 = 0;
                    g2 = 0;
                    b2 = 0;
                    a2 = 0;
                    break;
                case 1:
                    r2 = 0;
                    g2 = 0;
                    b2 = 0;
                    a2 = H16(wk + 0x5470) * 128 / 100;
                    break;
                case 2:
                    r2 = 0;
                    g2 = 0;
                    b2 = H16(wk + 0x5470) * 128 / 100;
                    a2 = 0x80;
                    break;
                case 3:
                    r2 = 0;
                    g2 = H16(wk + 0x5470) * 128 / 100;
                    b2 = 0x80;
                    a2 = b2;
                    break;
                case 4:
                    r2 = H16(wk + 0x5470) * 128 / 100;
                    g2 = 0x80;
                    b2 = g2;
                    a2 = g2;
                    break;
                case 5:
                    r2 = 0x80;
                    g2 = r2;
                    b2 = r2;
                    a2 = r2;
                    break;
                }
                if (0 < B8(wk + 0x546E) && B8(wk + 0x546E) < 5) {
                    H16(wk + 0x5470) += 2;
                    if (H16(wk + 0x5470) >= 100) {
                        H16(wk + 0x5470) = 0;
                        if (B8(wk + 0x546E) < 5) {
                            B8(wk + 0x546E) += 1;
                            if (B8(wk + 0x546E) >= 5) {
                                B8(wk + 0x546E) = 5;
                                B8(wk + 0x5484) = 0;
                                U16(wk + 0x5486) = 0;
                            }
                        }
                    }
                }
                lamp.col[0][0] = lamp.col[0][1] = lamp.col[0][2] = lamp.col[1][0] = lamp.col[1][1] = lamp.col[1][2] = r2;
                lamp.col[0][3] = lamp.col[1][3] = b2;
                lamp.col[2][0] = lamp.col[2][1] = lamp.col[2][2] = lamp.col[3][0] = lamp.col[3][1] = lamp.col[3][2] = g2;
                lamp.col[2][3] = lamp.col[3][3] = a2;
                tex2 = &tex_uv_data[lamp.code];
                tex2->f[6] = 0x180;
                tex2->f[2] = 0x180;
                tex2->f[8] = 7;
                lamp.w[6] = 0x180;
                lamp.w[2] = 0x180;
                px2 = B8(wk + 0x5437) * 512 / 100 + 0x280;
                break;
            }
            if (B8(wk + 0x5436) != 0) {
                fv = px2;
                cursor = Taku_GT4_Set(cursor, &lamp, (short)(fv + F32(wk + 0x548C)), (short)(100.0f + F32(wk + 0x5490)), 0, 0);
            }
        }
    } else if (H16(ctx + 0xC2) == 0x2F76 || H16(ctx + 0xC2) == 0x2BAC) {
        r3 = 0x80;
        g3 = r3;
        b3 = r3;
        c33 = r3;
        for (k3 = 0; k3 < 2; k3++) {
            lamp2 = data_0029ae80_iventga;

            tex3 = &tex_uv_data[lamp2.code];
            tex3->f[4] = 0;
            tex3->f[3] = 0;
            tex3->f[1] = 0;
            tex3->f[0] = 0;
            tex3->f[6] = 0x200;
            tex3->f[2] = 0x200;
            tex3->f[7] = 0x100;
            tex3->f[5] = 0x100;
            switch (k3) {
            case 0:
                tex3->f[8] = 6;
                px3 = B8(wk + 0x5437) * 576 / 100 - 0x200;
                break;
            case 1:
                switch (B8(wk + 0x546E)) {
                case 0:
                    r3 = 0x80;
                    g3 = r3;
                    b3 = r3;
                    c33 = r3;
                    break;
                case 1:
                    r3 = 0x80;
                    b3 = r3;
                    c33 = r3;
                    g3 = 0x80 - H16(wk + 0x5470) * 128 / 100;
                    break;
                case 2:
                    g3 = 0;
                    r3 = 0x80 - H16(wk + 0x5470) * 128 / 100;
                    b3 = 0x80;
                    c33 = b3;
                    break;
                case 3:
                    r3 = 0;
                    g3 = 0;
                    b3 = 0x80;
                    c33 = 0x80 - H16(wk + 0x5470) * 128 / 100;
                    break;
                case 4:
                    r3 = 0;
                    g3 = 0;
                    c33 = 0;
                    b3 = 0x80 - H16(wk + 0x5470) * 128 / 100;
                    break;
                case 5:
                    r3 = 0;
                    g3 = 0;
                    b3 = 0;
                    c33 = 0;
                    break;
                }
                if (0 < B8(wk + 0x546E) && B8(wk + 0x546E) < 5) {
                    H16(wk + 0x5470) += 2;
                    if (H16(wk + 0x5470) >= 100) {
                        H16(wk + 0x5470) = 0;
                        if (B8(wk + 0x546E) < 5) {
                            B8(wk + 0x546E) += 1;
                            if (B8(wk + 0x546E) >= 5) {
                                B8(wk + 0x546E) = 5;
                                B8(wk + 0x5484) = 0;
                                U16(wk + 0x5486) = 0;
                            }
                        }
                    }
                }
                if (B8(wk + 0x546E) < 2) {
                    B8(wk + 0x5484) = 0;
                    U16(wk + 0x5486) = 0;
                }
                if (B8(wk + 0x546E) == 2 && 0x32 < H16(wk + 0x5470)) {
                    B8(wk + 0x5484) = 1;
                }
                lamp2.col[0][0] = lamp2.col[0][1] = lamp2.col[0][2] = lamp2.col[1][0] = lamp2.col[1][1] = lamp2.col[1][2] = r3;
                lamp2.col[0][3] = lamp2.col[1][3] = b3;
                lamp2.col[2][0] = lamp2.col[2][1] = lamp2.col[2][2] = lamp2.col[3][0] = lamp2.col[3][1] = lamp2.col[3][2] = g3;
                lamp2.col[2][3] = lamp2.col[3][3] = c33;
                tex3 = &tex_uv_data[lamp2.code];
                tex3->f[6] = 0x180;
                tex3->f[2] = 0x180;
                tex3->f[8] = 7;
                lamp2.w[6] = 0x180;
                lamp2.w[2] = 0x180;
                px3 = B8(wk + 0x5437) * 512 / 100 + 0x280;
                break;
            }
            if (B8(wk + 0x5436) != 0) {
                fv = px3;
                cursor = Taku_GT4_Set(cursor, &lamp2, (short)(fv + F32(wk + 0x548C)), (short)(100.0f + F32(wk + 0x5490)), 0, 0);
                if (k3 == 1 && B8(wk + 0x5484) != 0) {
                    x13 = px3 + 0xB2;
                    x23 = px3 + 0xC6;

                    for (i3 = 0, pb = &bars.o[0]; i3 < 4; i3++, pb++) {
                        if (i3 < 2) {
                            if (U16(wk + 0x5486) % 2 != 0) {
                                for (q3 = 0; q3 < 8; q3++) {
                                    pb->w[q3] >>= 1;
                                }
                            }
                        }
                        switch (i3) {
                        case 2:
                            pb->w[0] = pb->w[4] = U16(wk + 0x5486) * -3 * 128 >> 5;
                            break;
                        case 3:
                            pb->w[2] = pb->w[6] = U16(wk + 0x5486) * 3 * 128 >> 5;
                            break;
                        }
                        if (U16(wk + 0x5486) > 0x18) {
                            dt3 = U16(wk + 0x5486) - 0x18;
                            pb->col[0][3] = pb->col[0][3] - (dt3 * pb->col[0][3] >> 3);
                            pb->col[1][3] = pb->col[1][3] - (dt3 * pb->col[1][3] >> 3);
                            pb->col[2][3] = pb->col[2][3] - (dt3 * pb->col[2][3] >> 3);
                            pb->col[3][3] = pb->col[3][3] - (dt3 * pb->col[3][3] >> 3);
                        }
                        cursor = Taku_G4_Set(cursor, pb, x13, 0xC8);
                        cursor = Taku_G4_Set(cursor, pb, x23, 0xC8);
                    }
                    U16(wk + 0x5486) += 1;
                    if (U16(wk + 0x5486) >= 0x20) {
                        U16(wk + 0x5486) = 0x20;
                    }
                }
            }
        }
    } else {
        size = data_0029aea8_iventga;
        cnt4 = 2;
        alpha4 = 0x80;
        tex_uv_data[page.o[0].code].f[2] = tex_uv_data[page.o[0].code].f[6] = tex_uv_data[page.o[1].code].f[2] =
            tex_uv_data[page.o[1].code].f[6] = 0x100;
        if ((H16(ctx + 0xC2) >= 0x2760 && H16(ctx + 0xC2) <= 0x2AF8) || H16(ctx + 0xC2) == 0x2EE0) {
            page.o[1].w[2] = page.o[1].w[6] = 0x200;
            tex_uv_data[page.o[1].code].f[2] = tex_uv_data[page.o[1].code].f[6] = 0x200;
            size.y = 0x100;
        }
        if (H16(ctx + 0xC2) >= 0x2B84 && H16(ctx + 0xC2) < 0x2B86) {
            page.o[0].w[2] = page.o[0].w[6] = 0x180;
            tex_uv_data[page.o[0].code].f[2] = tex_uv_data[page.o[0].code].f[6] = 0x180;
            size.x = 0x80;
        }
        if (H16(ctx + 0xC2) >= 0x2B98 && H16(ctx + 0xC2) < 0x2B9B) {
            page.o[0].w[2] = page.o[0].w[6] = 0x200;
            tex_uv_data[page.o[0].code].f[2] = tex_uv_data[page.o[0].code].f[6] = 0x200;
            page.o[1].w[2] = page.o[1].w[6] = 0x180;
            size.x = 0x100;
            tex_uv_data[page.o[1].code].f[2] = tex_uv_data[page.o[1].code].f[6] = 0x180;
            size.y = 0x80;
        }
        if (H16(ctx + 0xC2) >= 0x2F62 && H16(ctx + 0xC2) < 0x2F65) {
            page.o[0].w[2] = page.o[0].w[6] = 0x200;
            tex_uv_data[page.o[0].code].f[2] = tex_uv_data[page.o[0].code].f[6] = 0x200;
            page.o[1].w[2] = page.o[1].w[6] = 0x180;
            size.x = 0x100;
            tex_uv_data[page.o[1].code].f[2] = tex_uv_data[page.o[1].code].f[6] = 0x180;
            size.y = 0x80;
        }
        if (H16(ctx + 0xC2) == 0x2F78) {
            page.o[0].w[2] = page.o[0].w[6] = 0x200;
            tex_uv_data[page.o[0].code].f[2] = tex_uv_data[page.o[0].code].f[6] = 0x200;
            size.x = 0x100;
            cnt4 = 1;
            page.o[1].w[6] = 0x180;
            page.o[1].w[2] = 0x180;
            tex_uv_data[page.o[1].code].f[6] = 0x180;
            tex_uv_data[page.o[1].code].f[2] = 0x180;
            size.y = 0x80;
        }
        if (H16(ctx + 0xC2) == 0x2EE0) {
            switch (B8(AT(0x01ED546E))) {
            case 0:
                alpha4 = 0x80;
                break;
            case 1:
                alpha4 = 0x80 - H16(AT(0x01ED5470)) * 128 / 100;
                break;
            case 5:
                alpha4 = 0;
                break;
            }
            if (0 < B8(AT(0x01ED546E)) && B8(AT(0x01ED546E)) < 5) {
                H16(AT(0x01ED5470)) += 2;
                if (H16(AT(0x01ED5470)) >= 100) {
                    H16(AT(0x01ED5470)) = 0;
                    if (B8(AT(0x01ED546E)) < 5) {
                        B8(AT(0x01ED546E)) += 1;
                        if (B8(AT(0x01ED546E)) >= 2) {
                            B8(AT(0x01ED546E)) = 5;
                        }
                    }
                }
            }
            page.o[0].col[0][3] = alpha4;
        }
        if (B8(AT(0x01ED5436)) != 0) {
            fx4 = 0.0f;
            fz4 = fx4;
            if (H16(ctx + 0xC2) == 0x2F62 || H16(ctx + 0xC2) == 0x2B98) {
                fx4 = F32(AT(0x01ED548C));
                fz4 = F32(AT(0x01ED5490));
            }
            cursor = Taku_Obj_Set(cursor, &page.o[0],
                                    (short)(fx4 + (float)((-0x100 - size.x) + (short)(((size.x >> 1) + 0x1C0) * B8(AT(0x01ED5437)) / 100))),
                                    (short)(100.0f + fz4));
            if (cnt4 == 2) {
                cursor = Taku_Obj_Set(cursor, &page.o[1], (short)((short)(((size.y >> 1) + 0x1C0) * B8(AT(0x01ED5437)) / 100) + 0x280), 100);
            }
        }
    }
    return cursor;
}

#include "dotr/ee/camp_ivent.h"

/* Taku_Camp_Ivent (0x21e330): one frame of the running camp event. The event number (board
 * +0xc2, read as 0x299ed2) selects the event (0x2724.. the opening sequence, 0x64.. the voyage
 * and focus sequences, 0x2ee0.., 0x2f64.. and others); within it the step halfword at 0x1ed536c
 * runs one step: effects, BGM and sound effects, the camp cinema mode, model loads that wait on
 * the file reader, the focus position, the camera state at 0x1ed53a2 and its counters, and the
 * hand-over to the next event number. Unknown event numbers clear the byte at 0x1ed53b3. */

#ifndef NULL
#define NULL ((void *)0)
#endif

/* MATCHING DEVICE: the work area by its literal addresses (camp_ivent.h). */
#define AT(a) DOTR_CAMP_IVENT_AT(a)

void Taku_Camp_Ivent(void)
{
    unsigned char *ctx = (unsigned char *)&data_00299e10_campivent;
    short temp_a0;
    short temp_a0_2;
    short temp_a1;
    short temp_a1_2;
    short temp_a1_3;
    short temp_a1_4;
    short temp_a2;
    short temp_a2_2;
    short temp_a2_3;
    short temp_a2_4;
    short temp_a2_5;
    short temp_a2_6;
    short temp_a2_7;
    short temp_a2_8;
    short temp_a2_9;
    short temp_v1;
    short temp_v1_10;
    short temp_v1_11;
    short temp_v1_12;
    short temp_v1_13;
    short temp_v1_14;
    short temp_v1_15;
    short temp_v1_16;
    short temp_v1_17;
    short temp_v1_18;
    short temp_v1_19;
    short temp_v1_20;
    short temp_v1_21;
    short temp_v1_22;
    short temp_v1_23;
    short temp_v1_2;
    short temp_v1_3;
    short temp_v1_4;
    short temp_v1_5;
    short temp_v1_6;
    short temp_v1_7;
    short temp_v1_8;
    short temp_v1_9;

    switch (CutSceneNumber_campivent) {
        case 0x2724:
            temp_v1 = *(short *)AT(0x01ED536C);
            switch (temp_v1) {
            case 0:
                SzEffect_Entry(0, 0x190, 0, 0, 0);
                func_00217980_campivent(0x18, 1);
                *(float *)AT(0x01ED0040) = 0;
                *(float *)AT(0x01ED0048) = 0;
                *(int *)AT(0x01ED004C) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 2;
                *(signed char *)AT(0x01ED53B3) = 1;
                *(signed char *)AT(0x01ED0062) = 0;
                *(short *)AT(0x01ED0064) = 0x1000;
                *(short *)AT(0x01ED53A2) = 1;
                *(short *)AT(0x01ED53A4) = 0x1000;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(signed char *)AT(0x01ED53B4) = 0;
                *(signed char *)AT(0x01ED53B5) = 0;
                *(signed char *)AT(0x01ED53A6) = 0;
                *(signed char *)AT(0x01ED53A7) = 0;
                *(signed char *)AT(0x01ED546C) = 0;
                *(signed char *)AT(0x01ED546D) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            case 1:
                if (*(short *)AT(0x01ED001C) == 2) {
                    *(signed char *)AT(0x01ED537B) = 1;
                    *(signed char *)AT(0x01ED537D) = 1;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            default:
dflt_3:
                goto end_0021E330;
            case 2:
                if (*(signed char *)AT(0x01ED537B) == 0xC) {
                    *(signed char *)AT(0x01ED537B) = 0;
                    *(signed char *)AT(0x01ED537D) = 0;
                    *(short *)AT(0x01ED53A2) = 2;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 3:
                *(signed char *)AT(0x01ED0062) = 2;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            case 4:
                *(short *)AT(0x01ED53A2) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            case 5:
                if (*(short *)AT(0x01ED0064) < 0x7D0) {
                    if (*(signed char *)AT(0x01ED546C) == 0) {
                        func_00217980_campivent(0x19, 0);
                        *(signed char *)AT(0x01ED546C) = 1;
                    }
                }
                if (*(short *)AT(0x01ED0064) < 0x50) {
                    *(short *)AT(0x01ED53A2) = 0xA;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 6:
                if (*(short *)AT(0x01ED53A2) == 0xB) {
                    (*(short *)(ctx + 0xC2)) = 0x2738;
                    *(short *)AT(0x01ED536C) = 0;
                    goto end_0021E330;
                }
                break;
            }
            break;
        case 0x2738:
            temp_v1_2 = *(short *)AT(0x01ED536C);
            switch (temp_v1_2) {
            case 0:
                SzEffect_Entry(0, 0x191, 0, 0, 0);
                *(float *)AT(0x01ED0040) = 0;
                *(float *)AT(0x01ED0048) = 0;
                *(int *)AT(0x01ED004C) = 0;
                Taku_Camp_SINEMA_Init(2);
                *(signed char *)AT(0x01ED53B3) = 2;
                *(signed char *)AT(0x01ED0062) = 0;
                *(short *)AT(0x01ED0064) = 0x1000;
                *(short *)AT(0x01ED53A2) = 0x14;
                *(short *)AT(0x01ED53A4) = 0x1000;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(signed char *)AT(0x01ED53A6) = 0;
                *(signed char *)AT(0x01ED53A7) = 0;
                *(signed char *)AT(0x01ED546C) = 0;
                *(signed char *)AT(0x01ED5437) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            case 1:
                if (*(short *)AT(0x01ED53A2) == 0x15) {
                    SzSound_PlaySE(0, 0x22);
                    SzSound_PlaySE(0, 0x23);
                    *(signed char *)AT(0x01ED0062) = 2;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 2:
                if (*(signed char *)AT(0x01ED0062) == 0) {
                    *(short *)AT(0x01ED53A2) = 0x1E;
                    *(short *)AT(0x01ED53A4) = 0;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 3:
                if (*(short *)AT(0x01ED53A2) == 0x1F) {
                    *(short *)AT(0x01ED53A2) = 0x20;
                    *(short *)AT(0x01ED53A4) = 0x1000;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 4:
                if (*(short *)AT(0x01ED53A2) == 0x21) {
                    func_00217980_campivent(0x19, 1);
                    *(signed char *)AT(0x01ED537B) = 1;
                    *(signed char *)AT(0x01ED537D) = 1;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 5:
                if (*(signed char *)AT(0x01ED537B) == 0xC) {
                    *(signed char *)AT(0x01ED537B) = 0xD;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 6:
                if (*(signed char *)AT(0x01ED537B) == 0xE) {
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 0;
                    *(signed char *)AT(0x01ED53B3) = 3;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 7:
                if (*(short *)AT(0x01ED001C) == 2) {
                    *(signed char *)AT(0x01ED537B) = 0;
                    *(signed char *)AT(0x01ED537D) = 0;
                    *(signed char *)AT(0x01ED53A6) = 1;
                    *(signed char *)AT(0x01ED53A7) = 0;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 8:
                if (*(signed char *)AT(0x01ED53A6) == 4) {
                    *(signed char *)AT(0x01ED53A6) = 0;
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 2;
                    *(signed char *)AT(0x01ED53B3) = 3;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 9:
                if (*(short *)AT(0x01ED001C) == 2) {
                    *(signed char *)AT(0x01ED537B) = 0;
                    *(signed char *)AT(0x01ED537D) = 0;
                    *(signed char *)AT(0x01ED53A6) = 0;
                    *(signed char *)AT(0x01ED53A7) = 0;
                    (*(short *)(ctx + 0xC2)) = 0x2739;
                    *(short *)AT(0x01ED536C) = 0;
                    goto end_0021E330;
                }
                break;
            default:
                goto dflt_3;
            }
            break;
        case 0x2739:
            temp_a0 = *(short *)AT(0x01ED536C);
            switch (temp_a0) {
            case 0:
                Taku_Camp_SINEMA_Init(2);
                *(signed char *)AT(0x01ED53B3) = 3;
                *(signed char *)AT(0x01ED0062) = 0;
                *(short *)AT(0x01ED0064) = 0;
                *(short *)AT(0x01ED53A2) = 0;
                *(short *)AT(0x01ED53A4) = 0;
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(signed char *)AT(0x01ED53A6) = 0;
                *(signed char *)AT(0x01ED53A7) = 0;
                *(signed char *)AT(0x01ED5437) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            case 1:
                if (*(signed char *)AT(0x01ED537B) == 0xC) {
                    *(signed char *)AT(0x01ED537B) = 0;
                    *(signed char *)AT(0x01ED537D) = 0;
                    *(signed char *)AT(0x01ED53A6) = 0;
                    *(signed char *)AT(0x01ED53A7) = 0;
                    (*(short *)(ctx + 0xC2)) = 0x2742;
                    *(short *)AT(0x01ED536C) = 0;
                    goto end_0021E330;
                }
                break;
            }
            break;
        case 0x2742:
            temp_v1_3 = *(short *)AT(0x01ED536C);
            switch (temp_v1_3) {
            case 0:
                Taku_Camp_SINEMA_Init(2);
                *(signed char *)AT(0x01ED53B3) = 3;
                *(signed char *)AT(0x01ED0062) = 0;
                *(short *)AT(0x01ED0064) = 0;
                *(short *)AT(0x01ED53A2) = 0;
                *(short *)AT(0x01ED53A4) = 0;
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(signed char *)AT(0x01ED53A6) = 0;
                *(signed char *)AT(0x01ED53A7) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            case 1:
                if (*(signed char *)AT(0x01ED537B) == 0xC) {
                    *(signed char *)AT(0x01ED537B) = 0;
                    *(signed char *)AT(0x01ED537D) = 0;
                    *(signed char *)AT(0x01ED53A6) = 1;
                    *(signed char *)AT(0x01ED53A7) = 0;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 2:
                if (*(signed char *)AT(0x01ED53A6) == 4) {
                    *(signed char *)AT(0x01ED53A6) = 0;
                    (*(short *)(ctx + 0xC2)) = 0x274C;
                    *(short *)AT(0x01ED536C) = 0;
                    goto end_0021E330;
                }
                break;
            }
            break;
        case 0x274C:
            temp_v1_4 = *(short *)AT(0x01ED536C);
            switch (temp_v1_4) {
            case 0:
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 2;
                *(signed char *)AT(0x01ED53B3) = 3;
                *(signed char *)AT(0x01ED0062) = 0;
                *(short *)AT(0x01ED0064) = 0;
                *(short *)AT(0x01ED53A2) = 0;
                *(short *)AT(0x01ED53A4) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(signed char *)AT(0x01ED53A6) = 0;
                *(signed char *)AT(0x01ED53A7) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            case 1:
                if (*(short *)AT(0x01ED001C) == 2) {
                    *(signed char *)AT(0x01ED537B) = 1;
                    *(signed char *)AT(0x01ED537D) = 1;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 2:
                if (*(signed char *)AT(0x01ED537B) == 0xC) {
                    *(signed char *)AT(0x01ED537B) = 0;
                    *(signed char *)AT(0x01ED537D) = 0;
                    (*(short *)(ctx + 0xC2)) = 0x2756;
                    *(short *)AT(0x01ED536C) = 0;
                    goto end_0021E330;
                }
                break;
            }
            break;
        case 0x2756:
            temp_v1_5 = *(short *)AT(0x01ED536C);
            switch (temp_v1_5) {
            case 0:
                Taku_Camp_SINEMA_Init(2);
                *(signed char *)AT(0x01ED53B3) = 3;
                *(signed char *)AT(0x01ED0062) = 0;
                *(short *)AT(0x01ED0064) = 0;
                *(short *)AT(0x01ED53A2) = 0;
                *(short *)AT(0x01ED53A4) = 0;
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(signed char *)AT(0x01ED53A6) = 0;
                *(signed char *)AT(0x01ED53A7) = 0;
                *(signed char *)AT(0x01ED5494) = 0;
                *(signed char *)AT(0x01ED5495) = 0;
                *(short *)AT(0x01ED549A) = 0;
                *(short *)AT(0x01ED549C) = -0x64;
                *(short *)AT(0x01ED549E) = -0xC8;
                *(signed char *)AT(0x01ED5496) = 0;
                *(signed char *)AT(0x01ED5497) = 0;
                *(signed char *)AT(0x01ED5498) = 0;
                *(signed char *)AT(0x01ED53A8) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            case 1:
                if (*(signed char *)AT(0x01ED537B) == 0xC) {
                    *(signed char *)AT(0x01ED537B) = 0;
                    *(signed char *)AT(0x01ED537D) = 0;
                    *(signed char *)AT(0x01ED53A6) = 1;
                    *(signed char *)AT(0x01ED53A7) = 0;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
                break;
            case 2:
                if (*(signed char *)AT(0x01ED53A6) == 4) {
                    *(signed char *)AT(0x01ED53A6) = 4;
                    (*(short *)(ctx + 0xC2)) = 0x2757;
                    *(short *)AT(0x01ED536C) = 0;
                    goto end_0021E330;
                }
                break;
            }
            break;
    case 0x2757:
        temp_a0_2 = *(short *)AT(0x01ED536C);
        switch (temp_a0_2) {
        case 0:
            Taku_Camp_SINEMA_Init(2);
            *(signed char *)AT(0x01ED53B3) = 3;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 1;
            *(signed char *)AT(0x01ED537D) = 1;
            *(signed char *)AT(0x01ED53A6) = 4;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                (*(short *)(ctx + 0xC2)) = 0x2760;
                *(short *)AT(0x01ED536C) = 0;
                func_00217980_campivent(0x1A, 0);
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2760:
        temp_v1_6 = *(short *)AT(0x01ED536C);
        switch (temp_v1_6) {
        case 0:
            Taku_Camp_SINEMA_Init(2);
            *(signed char *)AT(0x01ED53B3) = 3;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 1;
            *(signed char *)AT(0x01ED537D) = 1;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED546D) = 0;
            *(signed char *)AT(0x01ED546C) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(signed char *)AT(0x01ED537D) == 3) {
                if ((*(signed char *)AT(0x01ED537B) == 5) && (*(signed char *)AT(0x01ED546C) == 0)) {
                    func_00217980_campivent(0x1A, 1);
                    *(signed char *)AT(0x01ED546C) = 1;
                }
            }
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(signed char *)AT(0x01ED53A8) = 1;
                (*(short *)(ctx + 0xC2)) = 0x276A;
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x276A:
        temp_a2 = *(short *)AT(0x01ED536C);
        switch (temp_a2) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            func_00217980_campivent(0x1A, 1);
            *(signed char *)AT(0x01ED53B3) = 3;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            func_00217980_campivent(0x1A, 1);
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(signed char *)AT(0x01ED53A6) = 1;
                *(signed char *)AT(0x01ED53A7) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED53A6) == 4) {
                *(signed char *)AT(0x01ED53A6) = 0;
                if (*(signed char *)AT(0x01ED53A8) == 0) {
                    (*(signed char *)(ctx + 0)) = 1;
                    (*(short *)(ctx + 0xC2)) = 0x2EE0;
                    (*(signed char *)(ctx + 0xC9)) = 2;
                } else {
                    (*(signed char *)(ctx + 0)) = 0;
                    (*(short *)(ctx + 0xC2)) = 0x2AF8;
                    (*(signed char *)(ctx + 0xC9)) = 0;
                }
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2AF8:
        temp_a1 = *(short *)AT(0x01ED536C);
        switch (temp_a1) {
        case 0:
            Taku_Camp_SINEMA_Init(2);
            *(signed char *)AT(0x01ED53B3) = 3;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 1;
            *(signed char *)AT(0x01ED537D) = 1;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED53A2) = 0x16;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED53A2) == 0x17) {
                Taku_Camp_SINEMA_Init(3);
                *(signed char *)AT(0x01ED0062) = 0;
                *(short *)AT(0x01ED0064) = 0;
                *(short *)AT(0x01ED53A2) = 0;
                *(short *)AT(0x01ED53A4) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(signed char *)AT(0x01ED53A6) = 0;
                *(signed char *)AT(0x01ED53A7) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                SzModel_LoadMerge(3, 2, 0);
                func_001f93f0_campivent(0, 7, 0, NULL);
                SzEffect_SetIntensityOnly(7, 1.0f);
                goto end_0021E330;
            }
            break;
        case 4:
            if (SzFile_InqRequestEx() == 0) {
                *(signed char *)AT(0x01ED53B3) = 0;
                *(float *)AT(0x01ED0040) = data_00298ddc_campivent;
                *(float *)AT(0x01ED0048) = data_00298de4_campivent;
                *(short *)AT(0x01ED536C) = 0;
                (*(short *)(ctx + 0xC2)) = 0x64;
                func_00217980_campivent(4, 1);
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B02:
        break;
    case 0x2B0C:
        temp_v1_7 = *(short *)AT(0x01ED536C);
        switch (temp_v1_7) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 1;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED53AC) = 1;
            *(signed char *)AT(0x01ED5437) = 0;
            func_00217980_campivent(0x1B, 1);
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED53AE) = (short) (*(short *)AT(0x01ED53AE) - 4);
                if (*(short *)AT(0x01ED53AE) < -0x280) {
                    *(short *)AT(0x01ED53AE) = -0x280;
                    if (*(signed char *)AT(0x01ED53AC) < 3) {
                        *(signed char *)AT(0x01ED53AC) = 2;
                        *(signed char *)AT(0x01ED537E) = 2;
                    }
                }
                if (*(signed char *)AT(0x01ED53AC) == 3) {
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 2;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 2:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                func_00217980_campivent(4, 0);
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 5:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x65;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED53AC) = 0;
                *(short *)AT(0x01ED53AE) = 0;
                *(short *)AT(0x01ED53B0) = 0;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2B70:
        temp_v1_8 = *(short *)AT(0x01ED536C);
        switch (temp_v1_8) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED5483) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            func_00217980_campivent(0x1C, 0);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED5483) = 1;
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED5483) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2B72;
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B72:
        temp_a2_2 = *(short *)AT(0x01ED536C);
        switch (temp_a2_2) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 1;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53B4) = 0;
            *(signed char *)AT(0x01ED53B5) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = -0x1C0;
            *(signed char *)AT(0x01ED53AC) = 1;
            func_00217980_campivent(0x1C, 1);
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED53B0) = (short) (*(short *)AT(0x01ED53B0) + 4);
                if (*(short *)AT(0x01ED53B0) > -0x50) {
                    *(short *)AT(0x01ED53B0) = -0x50;
                    if (*(signed char *)AT(0x01ED53AC) < 3) {
                        *(signed char *)AT(0x01ED53AC) = 2;
                        *(signed char *)AT(0x01ED537E) = 2;
                    }
                }
                if (*(signed char *)AT(0x01ED53AC) == 3) {
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 2;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 2:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2B74;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED53AC) = 0;
                *(short *)AT(0x01ED53AE) = 0;
                *(short *)AT(0x01ED53B0) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B74:
        temp_a2_3 = *(short *)AT(0x01ED536C);
        switch (temp_a2_3) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(float *)AT(0x01ED0040) = data_00298d0c_campivent;
            *(float *)AT(0x01ED0048) = data_00298d14_campivent;
            *(short *)AT(0x01ED52FA) = 0;
            *(signed char *)AT(0x01ED5483) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED5483) = 1;
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                func_00217980_campivent(4, 0);
                *(signed char *)AT(0x01ED5483) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED1C5E) = data_00298c32_campivent;
                *(short *)AT(0x01ED1E62) = data_00298cd0_campivent;
                *(short *)AT(0x01ED1A5A) = data_00298d6a_campivent;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED52FA) == 1) {
                (*(short *)(ctx + 0xC2)) = 0;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED5374) = 0;
                *(signed char *)AT(0x01ED5375) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED0060) = 0;
                *(signed char *)AT(0x01ED5372) = 0;
                *(signed char *)AT(0x01ED5373) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B84:
        temp_v1_9 = *(short *)AT(0x01ED536C);
        switch (temp_v1_9) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED5437) = 0;
            *(signed char *)AT(0x01ED546C) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537D) == 4) {
                if (*(signed char *)AT(0x01ED546C) == 0) {
                    func_00217980_campivent(0x1D, 0);
                    *(signed char *)AT(0x01ED546C) = 1;
                }
            }
            if (*(signed char *)AT(0x01ED537D) == 6) {
                if (*(signed char *)AT(0x01ED546C) < 2) {
                    func_00217980_campivent(0x1D, 1);
                    *(signed char *)AT(0x01ED546C) = 2;
                }
            }
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 5:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2B86;
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2B86:
        temp_a2_4 = *(short *)AT(0x01ED536C);
        switch (temp_a2_4) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 1;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53B4) = 0;
            *(signed char *)AT(0x01ED53B5) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED53AC) = 1;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED53B0) = (short) (*(short *)AT(0x01ED53B0) - 4);
                if (*(short *)AT(0x01ED53B0) < -0x1C0) {
                    *(short *)AT(0x01ED53B0) = -0x1C0;
                    if (*(signed char *)AT(0x01ED53AC) < 3) {
                        *(signed char *)AT(0x01ED53AC) = 2;
                        *(signed char *)AT(0x01ED537E) = 2;
                    }
                }
                if (*(signed char *)AT(0x01ED53AC) == 3) {
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 2;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 2:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2B87;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED53AC) = 0;
                *(short *)AT(0x01ED53AE) = 0;
                *(short *)AT(0x01ED53B0) = 0;
                func_00217980_campivent(0x1E, 0);
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B87:
        temp_v1_10 = *(short *)AT(0x01ED536C);
        switch (temp_v1_10) {
        case 0:
            Taku_Camp_SINEMA_Init(3);
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 1;
            *(signed char *)AT(0x01ED537D) = 1;
            *(signed char *)AT(0x01ED53B4) = 0;
            *(signed char *)AT(0x01ED53B5) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                (*(short *)(ctx + 0xC2)) = 0x2B88;
                *(short *)AT(0x01ED536C) = 0;
                func_00217980_campivent(0x1E, 1);
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B88:
        temp_v1_11 = *(short *)AT(0x01ED536C);
        switch (temp_v1_11) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 1;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53B4) = 0;
            *(signed char *)AT(0x01ED53B5) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED53AC) = 1;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                if (*(signed char *)AT(0x01ED53AC) < 3) {
                    *(signed char *)AT(0x01ED53AC) = 2;
                    *(signed char *)AT(0x01ED537E) = 2;
                }
                if (*(signed char *)AT(0x01ED53AC) == 3) {
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 2;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 2:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 1;
                *(signed char *)AT(0x01ED53AC) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED54A0) = 1;
                *(short *)AT(0x01ED54A2) = 0x80;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 5:
            if (*(signed char *)AT(0x01ED54A0) == 0xA) {
                if (*(signed char *)AT(0x01ED53AC) < 3) {
                    *(signed char *)AT(0x01ED53AC) = 2;
                    *(signed char *)AT(0x01ED537E) = 2;
                }
            }
            if (*(signed char *)AT(0x01ED53AC) == 3) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 6:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2B8A;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED53AC) = 0;
                *(short *)AT(0x01ED53AE) = 0;
                *(short *)AT(0x01ED53B0) = 0;
                *(short *)AT(0x01ED54A2) = 0x80;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2B8A:
        temp_a2_5 = *(short *)AT(0x01ED536C);
        switch (temp_a2_5) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 1;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53B4) = 0;
            *(signed char *)AT(0x01ED53B5) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED53AC) = 1;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                if (*(signed char *)AT(0x01ED53AC) < 3) {
                    *(signed char *)AT(0x01ED53AC) = 2;
                    *(signed char *)AT(0x01ED537E) = 2;
                }
                if (*(signed char *)AT(0x01ED53AC) == 3) {
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 2;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 2:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2B8B;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED53AC) = 0;
                *(short *)AT(0x01ED53AE) = 0;
                *(short *)AT(0x01ED53B0) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B8B:
        temp_a0_2 = *(short *)AT(0x01ED536C);
        switch (temp_a0_2) {
        case 0:
            Taku_Camp_SINEMA_Init(3);
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 1;
            *(signed char *)AT(0x01ED537D) = 1;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                (*(short *)(ctx + 0xC2)) = 0x2B8C;
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B8C:
        temp_a2_6 = *(short *)AT(0x01ED536C);
        switch (temp_a2_6) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED5483) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                (*(short *)(ctx + 0xC2)) = (short) ((*(short *)(ctx + 0xC2)) + 1);
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x6C;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED53AC) = 0;
                *(short *)AT(0x01ED53AE) = 0;
                *(short *)AT(0x01ED53B0) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B8D:
        temp_v1_12 = *(short *)AT(0x01ED536C);
        switch (temp_v1_12) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 1;
            *(signed char *)AT(0x01ED537D) = 1;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED5483) = 1;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED5483) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                func_00217980_campivent(4, 0);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x6C;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED53AC) = 0;
                *(short *)AT(0x01ED53AE) = 0;
                *(short *)AT(0x01ED53B0) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2B98:
        temp_v1_13 = *(short *)AT(0x01ED536C);
        switch (temp_v1_13) {
        case 0:
            func_00217980_campivent(0x1F, 0);
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 3;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED546C) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                SzModel_LoadMerge(3, 0, 0);
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (SzFile_InqRequestEx() == 0) {
                *(signed char *)AT(0x01ED53B3) = 3;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 2;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(signed char *)AT(0x01ED537D) == 3) {
                if (*(signed char *)AT(0x01ED546C) == 0) {
                    func_00217980_campivent(0x1F, 1);
                    *(signed char *)AT(0x01ED546C) = 1;
                }
            }
            if (*(signed char *)AT(0x01ED537D) == 0xD) {
                if (*(signed char *)AT(0x01ED546C) < 2) {
                    func_00217980_campivent(0x20, 1);
                    *(signed char *)AT(0x01ED546C) = 2;
                    *(signed char *)AT(0x01ED547F) = 1;
                    *(signed char *)AT(0x01ED5480) = 0;
                }
            }
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 5:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 6:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2B9A;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED53AC) = 0;
                *(short *)AT(0x01ED53AE) = 0;
                *(short *)AT(0x01ED53B0) = 0;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2B9A:
        temp_v1_14 = *(short *)AT(0x01ED536C);
        switch (temp_v1_14) {
        case 0:
            SzEffect_Entry(0, 0x191, 0, 0, 0);
            *(float *)AT(0x01ED0040) = 0;
            *(float *)AT(0x01ED0048) = 0;
            *(int *)AT(0x01ED004C) = 0;
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED53B3) = 2;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0x1000;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED546E) = 0;
            *(short *)AT(0x01ED5470) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                SzSound_PlaySE(0, 0x22);
                SzSound_PlaySE(0, 0x23);
                *(signed char *)AT(0x01ED0062) = 2;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED0062) == 0) {
                *(short *)AT(0x01ED53A2) = 0x1E;
                *(short *)AT(0x01ED53A4) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED53A2) == 0x1F) {
                *(short *)AT(0x01ED53A2) = 0x20;
                *(short *)AT(0x01ED53A4) = 0x1000;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED53A2) == 0x21) {
                *(signed char *)AT(0x01ED5437) = (signed char) (*(signed char *)AT(0x01ED5437) - 4);
                if (*(signed char *)AT(0x01ED5437) < -0x63) {
                    *(signed char *)AT(0x01ED5437) = -0x64;
                    *(signed char *)AT(0x01ED546E) = 1;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 5:
            if ((*(signed char *)AT(0x01ED546E) == 5) && (*(signed char *)AT(0x01ED547F) == 4)) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                func_00217980_campivent(0x21, 1);
                goto end_0021E330;
            }
            break;
        case 6:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 7:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                func_00217980_campivent(4, 0);
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 8:
            if (*(short *)AT(0x01ED001C) == 2) {
                SzModel_LoadMerge(3, 2, 0);
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 9:
            if (SzFile_InqRequestEx() == 0) {
                *(signed char *)AT(0x01ED53B3) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                (*(signed char *)(ctx + 0xC9)) = 1;
                *(float *)AT(0x01ED0040) = data_00298ddc_campivent;
                *(float *)AT(0x01ED0048) = data_00298de4_campivent;
                goto end_0021E330;
            }
            break;
        case 10:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(signed char *)(ctx + 0xC9)) = 1;
                *(float *)AT(0x01ED0040) = data_00298ddc_campivent;
                *(float *)AT(0x01ED0048) = data_00298de4_campivent;
                (*(short *)(ctx + 0xC2)) = 0x70;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED5374) = 1;
                *(signed char *)AT(0x01ED5375) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED0060) = 0;
                *(signed char *)AT(0x01ED5372) = 0;
                *(signed char *)AT(0x01ED5373) = 0;
                *(signed char *)AT(0x01ED546C) = 0;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2BAC:
        temp_v1_15 = *(short *)AT(0x01ED536C);
        switch (temp_v1_15) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 3;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED546E) = 0;
            *(short *)AT(0x01ED5470) = 0;
            *(signed char *)AT(0x01ED547F) = 0;
            *(signed char *)AT(0x01ED5480) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                SzModel_LoadMerge(3, 0, 0);
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (SzFile_InqRequestEx() == 0) {
                *(signed char *)AT(0x01ED53B3) = 3;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 2;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(signed char *)AT(0x01ED537D) == 3) {
                if ((*(signed char *)AT(0x01ED537B) == 3) && (*(signed char *)AT(0x01ED546E) == 0) && (*(signed char *)AT(0x01ED5437) < -0x63)) {
                    *(signed char *)AT(0x01ED546E) = 1;
                    *(short *)AT(0x01ED5470) = 0;
                    *(signed char *)AT(0x01ED547F) = 1;
                }
            }
            if ((*(signed char *)AT(0x01ED537B) == 0xC) && (*(signed char *)AT(0x01ED546E) == 5) && (*(signed char *)AT(0x01ED547F) == 4)) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(signed char *)AT(0x01ED547F) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 5:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(signed char *)AT(0x01ED546E) = 0;
                *(short *)AT(0x01ED5470) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 6:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2BAE;
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2BAE:
        temp_v1_16 = *(short *)AT(0x01ED536C);
        switch (temp_v1_16) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 1;
            *(signed char *)AT(0x01ED53B3) = 3;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53B4) = 0;
            *(signed char *)AT(0x01ED53B5) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = -0x180;
            *(signed char *)AT(0x01ED53AC) = 1;
            func_00217980_campivent(0x22, 1);
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED53AE) = (short) (*(short *)AT(0x01ED53AE) - 2);
                if (*(short *)AT(0x01ED53AE) < -0xC0) {
                    *(short *)AT(0x01ED53AE) = -0xC0;
                    if (*(signed char *)AT(0x01ED53AC) < 3) {
                        *(signed char *)AT(0x01ED53AC) = 2;
                        *(signed char *)AT(0x01ED537E) = 2;
                    }
                }
                temp_a1_2 = *(short *)AT(0x01ED53AE);
                *(short *)AT(0x01ED53B0) = (-temp_a1_2 * 0x180) / 384 - 0x180;
                if (*(signed char *)AT(0x01ED53AC) == 3) {
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 2;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 2:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 1;
                *(signed char *)AT(0x01ED53AC) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED53AE) = (short) (*(short *)AT(0x01ED53AE) - 2);
                if (*(short *)AT(0x01ED53AE) < -0x180) {
                    *(short *)AT(0x01ED53AE) = -0x180;
                    if (*(signed char *)AT(0x01ED53AC) < 3) {
                        *(signed char *)AT(0x01ED53AC) = 2;
                        *(signed char *)AT(0x01ED537E) = 2;
                    }
                }
                temp_a1_3 = *(short *)AT(0x01ED53AE);
                *(short *)AT(0x01ED53B0) = (-temp_a1_3 * 0x180) / 384 - 0x180;
                if (*(signed char *)AT(0x01ED53AC) == 3) {
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 3;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 5:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2BB0;
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2BB0:
        temp_v1_17 = *(short *)AT(0x01ED536C);
        switch (temp_v1_17) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED53B3) = 1;
            *(float *)AT(0x01ED0040) = 0;
            *(float *)AT(0x01ED0048) = 0;
            *(int *)AT(0x01ED004C) = 0;
            *(signed char *)AT(0x01ED0062) = 1;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53B4) = 0;
            *(signed char *)AT(0x01ED53B5) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = 0;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED5447) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto block_534;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            }
            goto block_534;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                (*(short *)(ctx + 0x3C)) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            }
            goto block_534;
        case 3:
            if (*(short *)AT(0x01ED001C) == 2) {
                if (*(signed char *)AT(0x01ED543C) == 0) {
                    (*(signed char *)(ctx + 1)) = 1;
                    if ((*(signed char *)(ctx + 1)) != 0) {
                        if ((*(signed char *)(ctx + 2)) != 0) {
                            SzData_SetMapEditFlag(0, 1);
                        }
                    }
                    SzGame_InitMemCardProc(2);
                    *(signed char *)AT(0x01ED543C) = 1;
                }
                if (0 > SzGame_ActMemCardProc()) {
                    *(signed char *)AT(0x01ED543C) = 0;
                    SzGame_PlayMovieNo(2, 1, 0);
                    Move_END_flg_campivent = 1;
                    SzGame_SetMenuNum(0xA, 1);
                    SzGame_Pop();
                    goto block_534;
                }
            } else {
                goto block_534;
            }
            break;
        default:
block_534:
            if (*(short *)AT(0x01ED0064) > 0xBB8) {
                *(short *)AT(0x01ED53A2) = 4;
            }
            if ((*(short *)AT(0x01ED0064) > 0x7D0) && (*(signed char *)AT(0x01ED5447) == 0)) {
                SzEffect_Entry(0, 0x192, 0, 0, 0);
                *(signed char *)AT(0x01ED5447) = 1;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2EE0:
        temp_v1_18 = *(short *)AT(0x01ED536C);
        switch (temp_v1_18) {
        case 0:
            Taku_Camp_SINEMA_Init(2);
            *(signed char *)AT(0x01ED53B3) = 3;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 1;
            *(signed char *)AT(0x01ED537D) = 1;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED546E) = 0;
            *(short *)AT(0x01ED5470) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(signed char *)AT(0x01ED537D) == 4) {
                if ((*(signed char *)AT(0x01ED537B) == 3) && (*(signed char *)AT(0x01ED546E) == 0)) {
                    *(signed char *)AT(0x01ED546E) = 1;
                    *(short *)AT(0x01ED5470) = 0;
                }
            }
            if ((*(signed char *)AT(0x01ED537B) == 0xC) && (*(signed char *)AT(0x01ED546E) == 5)) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED53A2) = 0x16;
                *(signed char *)AT(0x01ED546E) = 0;
                *(short *)AT(0x01ED5470) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED53A2) == 0x17) {
                Taku_Camp_SINEMA_Init(3);
                *(signed char *)AT(0x01ED0062) = 0;
                *(short *)AT(0x01ED0064) = 0;
                *(short *)AT(0x01ED53A2) = 0;
                *(short *)AT(0x01ED53A4) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(signed char *)AT(0x01ED53A6) = 0;
                *(signed char *)AT(0x01ED53A7) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                SzModel_LoadMerge(3, 2, 0);
                func_001f93f0_campivent(0, 7, 0, NULL);
                SzEffect_SetIntensityOnly(7, 1.0f);
                goto end_0021E330;
            }
            break;
        case 4:
            if (SzFile_InqRequestEx() == 0) {
                *(signed char *)AT(0x01ED53B3) = 0;
                *(float *)AT(0x01ED0040) = data_00298ddc_campivent;
                *(float *)AT(0x01ED0048) = data_00298de4_campivent;
                *(short *)AT(0x01ED536C) = 0;
                (*(short *)(ctx + 0xC2)) = 0x64;
                func_00217980_campivent(4, 1);
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2F08:
        temp_v1_19 = *(short *)AT(0x01ED536C);
        switch (temp_v1_19) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            func_00217980_campivent(0x23, 1);
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED536C) = 0;
                (*(short *)(ctx + 0xC2)) = 0x68;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2F09:
        temp_a1_4 = *(short *)AT(0x01ED536C);
        switch (temp_a1_4) {
        case 0:
            Taku_Camp_SINEMA_Init(2);
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 1;
            *(signed char *)AT(0x01ED537D) = 1;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED530C) = 2;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if ((*(short *)AT(0x01ED530C) == 0) && (My_LedarModel_a_set(1) != 0)) {
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0;
                *(short *)AT(0x01ED536C) = 0;
                *(signed char *)AT(0x01ED5374) = 0;
                *(signed char *)AT(0x01ED5375) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED0060) = 0;
                *(signed char *)AT(0x01ED5372) = 0;
                *(signed char *)AT(0x01ED5373) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2F62:
        temp_v1_20 = *(short *)AT(0x01ED536C);
        switch (temp_v1_20) {
        case 0:
            Taku_Camp_SINEMA_Init(3);
            *(signed char *)AT(0x01ED53B3) = 3;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            func_00217980_campivent(0x24, 1);
            SzModel_LoadMerge(3, 0, 0);
            *(signed char *)AT(0x01ED547F) = 0;
            *(signed char *)AT(0x01ED5480) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (SzFile_InqRequestEx() == 0) {
                *(signed char *)AT(0x01ED53B3) = 3;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 2;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537D) == 2) {
                if (*(signed char *)AT(0x01ED537B) == 3) {
                    *(signed char *)AT(0x01ED547F) = 1;
                }
            }
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 5:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED536C) = 0;
                (*(short *)(ctx + 0xC2)) = 0x2F64;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2F64:
        temp_v1_21 = *(short *)AT(0x01ED536C);
        switch (temp_v1_21) {
        case 0:
            SzEffect_Entry(0, 0x191, 0, 0, 0);
            *(float *)AT(0x01ED0040) = 0;
            *(float *)AT(0x01ED0048) = 0;
            *(int *)AT(0x01ED004C) = 0;
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED53B3) = 2;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0x1000;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED546E) = 0;
            *(short *)AT(0x01ED5470) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                SzSound_PlaySE(0, 0x22);
                SzSound_PlaySE(0, 0x23);
                *(signed char *)AT(0x01ED0062) = 2;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED0062) == 0) {
                *(short *)AT(0x01ED53A2) = 0x1E;
                *(short *)AT(0x01ED53A4) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED53A2) == 0x1F) {
                *(short *)AT(0x01ED53A2) = 0x20;
                *(short *)AT(0x01ED53A4) = 0x1000;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(short *)AT(0x01ED53A2) == 0x21) {
                *(signed char *)AT(0x01ED5437) = (signed char) (*(signed char *)AT(0x01ED5437) - 4);
                if (*(signed char *)AT(0x01ED5437) < -0x63) {
                    *(signed char *)AT(0x01ED5437) = -0x64;
                    *(signed char *)AT(0x01ED546E) = 1;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 5:
            if ((*(signed char *)AT(0x01ED546E) == 5) && (*(signed char *)AT(0x01ED547F) == 4)) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                func_00217980_campivent(0x21, 1);
                goto end_0021E330;
            }
            break;
        case 6:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 7:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                func_00217980_campivent(4, 0);
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 8:
            if (*(short *)AT(0x01ED001C) == 2) {
                SzModel_LoadMerge(3, 2, 0);
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 9:
            if (SzFile_InqRequestEx() == 0) {
                *(signed char *)AT(0x01ED53B3) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 0;
                *(float *)AT(0x01ED0040) = data_00298ddc_campivent;
                *(float *)AT(0x01ED0048) = data_00298de4_campivent;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 10:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED536C) = 0;
                (*(short *)(ctx + 0xC2)) = 0;
                *(signed char *)AT(0x01ED5374) = 0;
                *(signed char *)AT(0x01ED5375) = 0;
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED0060) = 0;
                *(signed char *)AT(0x01ED5372) = 0;
                *(signed char *)AT(0x01ED5373) = 0;
                *(signed char *)AT(0x01ED546C) = 0;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2F76:
        temp_a2_7 = *(short *)AT(0x01ED536C);
        switch (temp_a2_7) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 3;
            *(signed char *)AT(0x01ED53B3) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED546E) = 0;
            *(short *)AT(0x01ED5470) = 0;
            *(signed char *)AT(0x01ED547F) = 0;
            *(signed char *)AT(0x01ED5480) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                SzModel_LoadMerge(3, 0, 0);
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (SzFile_InqRequestEx() == 0) {
                *(signed char *)AT(0x01ED53B3) = 3;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 2;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(signed char *)AT(0x01ED537D) == 2) {
                if ((*(signed char *)AT(0x01ED546E) == 0) && (*(signed char *)AT(0x01ED5437) < -0x63)) {
                    *(signed char *)AT(0x01ED546E) = 1;
                    *(short *)AT(0x01ED5470) = 0;
                    *(signed char *)AT(0x01ED547F) = 1;
                }
            }
            if ((*(signed char *)AT(0x01ED537B) == 0xC) && (*(signed char *)AT(0x01ED546E) == 5) && (*(signed char *)AT(0x01ED547F) == 4)) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED536C) = 0;
                (*(short *)(ctx + 0xC2)) = 0x2F78;
                *(signed char *)AT(0x01ED546E) = 0;
                *(short *)AT(0x01ED5470) = 0;
                *(signed char *)AT(0x01ED547F) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2F78:
        temp_a2_8 = *(short *)AT(0x01ED536C);
        switch (temp_a2_8) {
        case 0:
            Taku_Camp_SINEMA_Init(2);
            *(signed char *)AT(0x01ED53B3) = 3;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 1;
            *(signed char *)AT(0x01ED537D) = 1;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            func_00217980_campivent(0x25, 1);
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(short *)AT(0x01ED001C) == 2) {
                (*(short *)(ctx + 0xC2)) = 0x2F7A;
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        }
        break;
    case 0x2F7A:
        temp_v1_22 = *(short *)AT(0x01ED536C);
        switch (temp_v1_22) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 1;
            *(signed char *)AT(0x01ED53B3) = 1;
            *(float *)AT(0x01ED0040) = 0;
            *(float *)AT(0x01ED0048) = 0;
            *(int *)AT(0x01ED004C) = 0;
            *(signed char *)AT(0x01ED0062) = 0;
            *(short *)AT(0x01ED0064) = 0;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED53AC) = 0;
            *(short *)AT(0x01ED53AE) = -0x180;
            *(short *)AT(0x01ED53B0) = 0;
            *(signed char *)AT(0x01ED53AC) = 1;
            *(signed char *)AT(0x01ED54A0) = 0;
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto end_0021E330;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(short *)AT(0x01ED53AE) = (short) (*(short *)AT(0x01ED53AE) + 2);
                if (0 < *(short *)AT(0x01ED53AE)) {
                    *(short *)AT(0x01ED53AE) = 0;
                    if (*(signed char *)AT(0x01ED53AC) < 3) {
                        *(signed char *)AT(0x01ED53AC) = 2;
                        *(signed char *)AT(0x01ED537E) = 2;
                    }
                }
                temp_a2_9 = *(short *)AT(0x01ED53AE);
                *(short *)AT(0x01ED53B0) = (-temp_a2_9 * 0x19C) / 384 - 0x19C;
                if (*(signed char *)AT(0x01ED53AC) == 3) {
                    *(short *)AT(0x01ED001C) = 1;
                    *(short *)AT(0x01ED001A) = 2;
                    *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                    goto end_0021E330;
                }
            }
            break;
        case 2:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 3:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 4:
            if (*(signed char *)AT(0x01ED537B) == 0xE) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(signed char *)AT(0x01ED54A0) = 1;
                *(short *)AT(0x01ED54A2) = 0x80;
                *(signed char *)AT(0x01ED0062) = 1;
                *(short *)AT(0x01ED0064) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
                goto end_0021E330;
            }
            break;
        case 5:
            if (*(signed char *)AT(0x01ED54A0) == 0xA) {
                (*(short *)(ctx + 0xC2)) = 0x2F7C;
                *(short *)AT(0x01ED536C) = 0;
                goto end_0021E330;
            }
            break;
        default:
            goto dflt_3;
        }
        break;
    case 0x2F7C:
        temp_v1_23 = *(short *)AT(0x01ED536C);
        switch (temp_v1_23) {
        case 0:
            *(short *)AT(0x01ED001C) = 1;
            *(short *)AT(0x01ED001A) = 2;
            *(signed char *)AT(0x01ED53B3) = 1;
            *(float *)AT(0x01ED0040) = 0;
            *(float *)AT(0x01ED0048) = 0;
            *(int *)AT(0x01ED004C) = 0;
            *(signed char *)AT(0x01ED0062) = 1;
            *(short *)AT(0x01ED53A2) = 0;
            *(short *)AT(0x01ED53A4) = 0;
            *(signed char *)AT(0x01ED537B) = 0;
            *(signed char *)AT(0x01ED537D) = 0;
            *(signed char *)AT(0x01ED53B4) = 0;
            *(signed char *)AT(0x01ED53B5) = 0;
            *(signed char *)AT(0x01ED53A6) = 0;
            *(signed char *)AT(0x01ED53A7) = 0;
            *(signed char *)AT(0x01ED5447) = 0;
            func_00217980_campivent(0x26, 1);
            *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            goto block_727;
        case 1:
            if (*(short *)AT(0x01ED001C) == 2) {
                *(signed char *)AT(0x01ED537B) = 1;
                *(signed char *)AT(0x01ED537D) = 1;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            }
            goto block_727;
        case 2:
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0;
                *(signed char *)AT(0x01ED537D) = 0;
                *(short *)AT(0x01ED001C) = 1;
                *(short *)AT(0x01ED001A) = 3;
                (*(short *)(ctx + 0x3C)) = 0;
                *(short *)AT(0x01ED536C) = (short) (*(short *)AT(0x01ED536C) + 1);
            }
            goto block_727;
        case 3:
            if (*(short *)AT(0x01ED001C) == 2) {
                if (*(signed char *)AT(0x01ED543C) == 0) {
                    (*(signed char *)(ctx + 2)) = 1;
                    if ((*(signed char *)(ctx + 1)) != 0) {
                        if ((*(signed char *)(ctx + 2)) != 0) {
                            SzData_SetMapEditFlag(0, 1);
                        }
                    }
                    SzGame_InitMemCardProc(2);
                    *(signed char *)AT(0x01ED543C) = 1;
                }
                if (0 > SzGame_ActMemCardProc()) {
                    *(signed char *)AT(0x01ED543C) = 0;
                    SzGame_PlayMovieNo(3, 1, 0);
                    Move_END_flg_campivent = 1;
                    SzGame_SetMenuNum(0xA, 1);
                    SzGame_Pop();
                    (*(short *)(ctx + 0xC2)) = 0;
                    goto block_727;
                }
            } else {
                goto block_727;
            }
            break;
        default:
block_727:
            if (*(short *)AT(0x01ED0064) > 0xBB8) {
                *(short *)AT(0x01ED53A2) = 4;
            }
            if ((*(short *)AT(0x01ED0064) > 0x7D0) && (*(signed char *)AT(0x01ED5447) == 0)) {
                SzEffect_Entry(0, 0x192, 0, 0, 0);
                *(signed char *)AT(0x01ED5447) = 1;
                goto end_0021E330;
            }
            break;
        }
        break;
    default:
        *(signed char *)AT(0x01ED53B3) = 0;
        break;
    }
end_0021E330:;
}

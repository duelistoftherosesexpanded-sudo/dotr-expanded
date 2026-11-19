#include "dotr/ee/tkcamp_act.h"

/* TkCamp_Act (0x237730): one frame of the camp map: the cursor fades, the map cursor and its
 * spot hit test (19-halfword spot records at 0x298260), the pad and analog-stick moves, the
 * option and message popups, the spot menus and camp mode steps (memory card, practice duel,
 * help, deck set-up), the camp model and the camera.
 * The control flow uses gotos and the work area is addressed by absolute addresses. Compiled
 * with #pragma conditional_move off and #pragma opt_dead_assignments off, set directly before
 * the function and reset after it (docs/DECOMPILATION.md). */

/* MATCHING DEVICE: the work area by its literal addresses (tkcamp_act.h). */
#define AT(a) DOTR_TKCAMP_AT(a)

#ifdef __MWERKS__
#pragma conditional_move off
#pragma opt_dead_assignments off
#endif
void TkCamp_Act(void)
{
    DotrTkCampWord ctx = (DotrTkCampWord) &data_00299e10_tkcamp;
    short temp_s1;
    short temp_s2;
    int var_s3;
    unsigned char *base = (unsigned char *)AT(0x01ED0000);
    short hit;
    short ax;
    short dx;
    short ay;
    short sgn;
    short temp_s1_3;
    int var_s2;
    short ok;
    short spot;
    float vp[4];
    float vq[4];
    float vr[4];
    float vs[4];
    struct DotrTkCampQ spE0;
    struct DotrTkCampQ spF0;
    float var_f0;
    float var_f0_2;
    float var_f0_3;
    float fq4;
    short *var_a3;
    short temp_a1;
    short temp_a1_2;
    short temp_v0_10;
    short temp_v0_11;
    short temp_v0_12;
    short temp_v0_5;
    short temp_v0_6;
    short temp_v0_7;
    short temp_v0_8;
    short temp_v0_9;
    int temp_hi;
    int temp_hi_2;
    int temp_hi_3;
    int temp_hi_4;
    int temp_s1_2;
    int temp_v0_19;
    int temp_v0_2;
    int var_a0_4;
    int var_a2;
    int var_s1;
    int var_s1_2;
    int var_v1;
    int temp_a1_4;
    signed char temp_a1_7;
    signed char temp_a1_8;
    int temp_v0;
    signed char temp_v0_15;
    signed char temp_v0_16;
    signed char temp_v0_17;
    int temp_v0_18;
    signed char temp_v0_20;
    signed char temp_v0_21;
    int temp_v0_22;
    signed char c23;
    signed char temp_v0_3;
    int temp_v1;
    signed char temp_v1_10;
    signed char temp_v1_11;
    signed char temp_v1_12;
    signed char temp_v1_13;
    signed char temp_v1_14;
    signed char temp_v1_4;
    signed char temp_v1_5;
    signed char temp_v1_6;
    signed char temp_v1_7;
    signed char temp_v1_8;
    signed char temp_v1_9;

    temp_s1 = *(short *)AT(0x01ED0000);
    temp_s2 = *(short *)AT(0x01ED0002);
    *(signed char *)AT(0x01ED547A) = 0;
    *(signed char *)AT(0x01ED5481) = 1;
    *(signed char *)AT(0x01ED5482) = 0;
    var_s3 = 0;
    if (CutSceneNumber_tkcamp < 0x2710) {
        temp_v0 = *(signed char *)AT(0x01ED006E);
        if (temp_v0 != 0) {
            *(short *)AT(0x01ED0070) = (short) (*(short *)AT(0x01ED0070) + 0x14);
            if (*(short *)AT(0x01ED0070) >= 0x100) {
                *(short *)AT(0x01ED0070) = 0x100;
                *(signed char *)AT(0x01ED5374) = 0;
            }
        } else if ((signed char) temp_v0 == 0) {
            *(signed char *)AT(0x01ED5374) = 1;
            *(short *)AT(0x01ED0070) = (short) (*(short *)AT(0x01ED0070) - 0x14);
            if (*(short *)AT(0x01ED0070) <= 0) {
                *(short *)AT(0x01ED0070) = 0;
            }
        }
        temp_v1 = *(signed char *)AT(0x01ED006A);
        if ((temp_v1 != 0) && (*(signed char *)AT(0x01ED006E) != 0) && (*(signed char *)AT(0x01ED53C1) == 0)) {
            if (*(signed char *)AT(0x01ED540A) == 0) {
                *(signed char *)AT(0x01ED540A) = 1;
                *(signed char *)AT(0x01ED5406) = 0;
            }
            *(short *)AT(0x01ED006C) = (short) (*(short *)AT(0x01ED006C) + 0x14);
            *(signed char *)AT(0x01ED53C1) = 0;
            if (*(short *)AT(0x01ED006C) >= 0x100) {
                *(short *)AT(0x01ED006C) = 0x100;
            }
        } else if (((signed char) temp_v1 == 0) || (*(signed char *)AT(0x01ED006E) == 0) || (*(signed char *)AT(0x01ED53C1) == 1)) {
            *(short *)AT(0x01ED006C) = (short) (*(short *)AT(0x01ED006C) - 0x14);
            if (*(short *)AT(0x01ED006C) <= 0) {
                *(short *)AT(0x01ED006C) = 0;
                *(signed char *)AT(0x01ED53AB) = 0;
                *(signed char *)AT(0x01ED53C1) = 0;
                *(signed char *)AT(0x01ED53C2) = (*(signed char *)(ctx + 0xC9));
                if (*(signed char *)AT(0x01ED540A) != 0) {
                    func_00245d80_tkcamp((unsigned char *)AT(0x01ED5424));
                    *(signed char *)AT(0x01ED540A) = 0;
                }
            }
        }
    } else {
        *(signed char *)AT(0x01ED5374) = 1;
        *(signed char *)AT(0x01ED006E) = 0;
        *(signed char *)AT(0x01ED006A) = 0;
        if (*(signed char *)AT(0x01ED006E) == 0) {
            *(short *)AT(0x01ED0070) = (short) (*(short *)AT(0x01ED0070) - 0x14);
            if (*(short *)AT(0x01ED0070) <= 0) {
                *(short *)AT(0x01ED0070) = 0;
            }
        }
        if ((*(signed char *)AT(0x01ED006A) == 0) || (*(signed char *)AT(0x01ED006E) == 0)) {
            *(short *)AT(0x01ED006C) = (short) (*(short *)AT(0x01ED006C) - 0x14);
            if (*(short *)AT(0x01ED006C) <= 0) {
                *(short *)AT(0x01ED006C) = 0;
                *(signed char *)AT(0x01ED53AB) = 0;
                if (*(signed char *)AT(0x01ED540A) != 0) {
                    func_00245d80_tkcamp((unsigned char *)AT(0x01ED5424));
                    *(signed char *)AT(0x01ED540A) = 0;
                }
            }
        }
    }
    for (var_v1 = 0; var_v1 <= 0x15; var_v1++) {
        if (((short *)base)[var_v1 + 0x2976] != 0) {
            *(signed char *)(base + 0x5482) = (signed char) (*(signed char *)(base + 0x5482) + 1);
        }
    }
    if (*(signed char *)AT(0x01ED5376) != 0) {
        if ((*(signed char *)AT(0x01ED5482) == 0) && (func_001f5240_tkcamp(-1, -1) == 0) && (*(signed char *)AT(0x01ED5374) == 0) && (*(signed char *)AT(0x01ED5375) == 0) && ((*(short *)(ctx + 0xC2)) < 0xC8)) {
            for (var_a2 = 0; var_a2 < 4; var_a2++) {
                var_a3 = &data_00298260_tkcamp[var_a2 * 0x13];
                temp_a1 = *(short *)(base + 0);
                if ((((*(short *)((signed char *)(var_a3) + (0))) - 2) <= temp_a1) && (temp_a1 <= ((*(short *)((signed char *)(var_a3) + (4))) + 2))) {
                    temp_a1_2 = *(short *)(base + 2);
                    if ((((*(short *)((signed char *)(var_a3) + (2))) - 2) <= temp_a1_2) && (temp_a1_2 <= ((*(short *)((signed char *)(var_a3) + (6))) + 2)) && (*(unsigned short *)(base + 4) != 1)) {
                        *(signed char *)(base + 0x547A) = 1;
                    }
                }
            }
            if ((*(signed char *)AT(0x01ED547A) != 0) && (*(signed char *)AT(0x01ED543E) == 0)) {
                switch (*(short *)AT(0x01ED0080)) {
                case 0:
                    if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                        func_00217710_tkcamp(0, 4);
                        func_001a8320_tkcamp(6, 0);
                        func_001a8870_tkcamp(6, 0);
                        goto end_TkCamp_Act;
                    }
                    break;
                case 1:
                    switch (*(signed char *)AT(0x01ED543C)) {
                    case 0:
                        if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                            func_00217710_tkcamp(0, 4);
                            func_001aafa0_tkcamp(1);
                            *(signed char *)AT(0x01ED543C) = 1;
                            goto end_TkCamp_Act;
                        }
                        break;
                    default:
                        if (0 > func_001ab1d0_tkcamp()) {
                            *(signed char *)AT(0x01ED543C) = 0;
                        }
                        goto end_TkCamp_Act;
                    }
                    break;
                case 2:
                    switch (*(signed char *)AT(0x01ED543C)) {
                    case 0:
                        if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                            func_00217710_tkcamp(0, 4);
                            func_001e3ff0_tkcamp((int) (*(signed char *)(ctx + 0)), 0, 0x200);
                            *(signed char *)AT(0x01ED543C) = 1;
                            *(signed char *)AT(0x01ED543D) = 0;
                            goto end_TkCamp_Act;
                        }
                        break;
                    case 1:
                        *(signed char *)AT(0x01ED543D) = (signed char) (*(signed char *)AT(0x01ED543D) + 1);
                        if (*(signed char *)AT(0x01ED543D) >= 8) {
                            *(signed char *)AT(0x01ED543D) = 8;
                            *(signed char *)AT(0x01ED543C) = 2;
                        }
                        func_001e3ff0_tkcamp(-1, 0, 0x200 - ((int) (*(signed char *)AT(0x01ED543D) << 9) >> 3));
                        goto end_TkCamp_Act;
                    case 2:
                        if (func_001e40a0_tkcamp(0) != 0) {
                            *(signed char *)AT(0x01ED543C) = 3;
                        }
                        goto end_TkCamp_Act;
                    case 3:
                        *(signed char *)AT(0x01ED543D) = (signed char) (*(signed char *)AT(0x01ED543D) - 1);
                        if (*(signed char *)AT(0x01ED543D) <= 0) {
                            *(signed char *)AT(0x01ED543D) = 8;
                            *(signed char *)AT(0x01ED543C) = 4;
                        }
                        func_001e3ff0_tkcamp(-1, 0, 0x200 - ((int) (*(signed char *)AT(0x01ED543D) << 9) >> 3));
                        if (*(signed char *)AT(0x01ED543C) == 4) {
                            *(signed char *)AT(0x01ED543C) = 0;
                            func_001e42b0_tkcamp();
                        }
                        goto end_TkCamp_Act;
                    }
                    break;
                case 3:
                    switch (*(signed char *)AT(0x01ED543C)) {
                    case 0:
                        if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                            func_00217710_tkcamp(0, 4);
                            *(signed char *)AT(0x01ED543C) = 1;
                            *(signed char *)AT(0x01ED543D) = 0;
                            *(signed char *)AT(0x01ED546B) = 0;
                            goto end_TkCamp_Act;
                        }
                        break;
                    case 1:
                        *(signed char *)AT(0x01ED543D) = (signed char) (*(signed char *)AT(0x01ED543D) + 1);
                        if (*(signed char *)AT(0x01ED543D) >= 8) {
                            *(signed char *)AT(0x01ED543D) = 8;
                            *(signed char *)AT(0x01ED543C) = 2;
                        }
                        goto end_TkCamp_Act;
                    case 2:
                        if (func_00203a40_tkcamp(0, 0, 0x20) != 0) {
                            func_00217710_tkcamp(0, 5);
                            *(signed char *)AT(0x01ED543C) = 0xA;
                        } else if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                            func_00217710_tkcamp(0, 1);
                            *(signed char *)AT(0x01ED543C) = 3;
                        }
                        if (*(signed char *)AT(0x01ED543C) == 3) {
                            func_0023bc80_tkcamp();
                        }
                        if (*(signed char *)AT(0x01ED543C) < 3) {
                            if (func_00203a40_tkcamp(0, 2, 0x1000) != 0) {
                                func_00217710_tkcamp(0, 3);
                                *(signed char *)AT(0x01ED546B) = (signed char) (*(signed char *)AT(0x01ED546B) - 1);
                                if (*(signed char *)AT(0x01ED546B) < 0) {
                                    *(signed char *)AT(0x01ED546B) = 4;
                                }
                            }
                            if (func_00203a40_tkcamp(0, 2, 0x4000) != 0) {
                                func_00217710_tkcamp(0, 3);
                                *(signed char *)AT(0x01ED546B) = (signed char) (*(signed char *)AT(0x01ED546B) + 1);
                                if (*(signed char *)AT(0x01ED546B) > 4) {
                                    *(signed char *)AT(0x01ED546B) = 0;
                                }
                            }
                        }
                        goto end_TkCamp_Act;
                    case 10:
                        *(signed char *)AT(0x01ED543D) = (signed char) (*(signed char *)AT(0x01ED543D) - 1);
                        if (*(signed char *)AT(0x01ED543D) <= 0) {
                            *(signed char *)AT(0x01ED543D) = 0;
                            *(signed char *)AT(0x01ED543C) = 0;
                        }
                        goto end_TkCamp_Act;
                    }
                    break;
                }
            }
            {
                if (*(signed char *)AT(0x01ED543C) == 0) {
                    switch (*(signed char *)AT(0x01ED543E)) {
                    case 0:
                        if (func_00203a40_tkcamp(0, 0, 0x100) != 0) {
                            func_00217710_tkcamp(0, 4);
                            func_001e42c0_tkcamp(7, 0, 0x200, 0x91);
                            *(signed char *)AT(0x01ED543E) = 1;
                            goto end_TkCamp_Act;
                        }
                        break;
                    case 1:
                        *(signed char *)AT(0x01ED543F) = (signed char) (*(signed char *)AT(0x01ED543F) + 1);
                        if (*(signed char *)AT(0x01ED543F) >= 8) {
                            *(signed char *)AT(0x01ED543F) = 8;
                            *(signed char *)AT(0x01ED543E) = 2;
                        }
                        func_001e42c0_tkcamp(7, 0, 0x200 - ((int) (*(signed char *)AT(0x01ED543F) << 9) >> 3), -1);
                        goto end_TkCamp_Act;
                    case 2:
                        temp_v0_2 = func_001e4430_tkcamp(0);
                        switch (temp_v0_2) {
                        case 1:
                            *(signed char *)AT(0x01ED543E) = 3;
                            break;
                        case 2:
                            *(signed char *)AT(0x01ED543E) = 4;
                            break;
                        }
                        return;
                    case 3:
                        func_001a8920_tkcamp();
                        goto end_TkCamp_Act;
                    case 4:
                        *(signed char *)AT(0x01ED543F) = (signed char) (*(signed char *)AT(0x01ED543F) - 1);
                        temp_v0_3 = *(signed char *)AT(0x01ED543F);
                        if (temp_v0_3 <= 0) {
                            *(signed char *)AT(0x01ED543F) = 0;
                            *(signed char *)AT(0x01ED543E) = 0;
                            func_001e4530_tkcamp();
                        } else {
                            func_001e42c0_tkcamp(7, 0, 0x200 - ((int) (temp_v0_3 << 9) >> 3), -1);
                        }
                        goto end_TkCamp_Act;
                    }
                }
                {
                    if (*(signed char *)AT(0x01ED543E) == 0) {
                        func_00203a40_tkcamp(0, 1, 0x800);
                        if (func_00203a40_tkcamp(0, 0, 0x800) != 0) {
                            func_00217710_tkcamp(0, 4);
                            temp_v1_4 = *(signed char *)AT(0x01ED0072);
                            if (temp_v1_4 == 0) {
                                *(signed char *)AT(0x01ED0072) = 1;
                                *(short *)AT(0x01ED007A) = 0;
                                *(short *)AT(0x01ED007C) = 0;
                            } else if (temp_v1_4 == 1) {
                                *(signed char *)AT(0x01ED0072) = 2;
                            }
                        }
                    }
                    if (func_00203bc0_tkcamp(0) != 0) {
                        ax = func_00203c40_tkcamp(0, 2) & 0xFF;
                        ay = func_00203c40_tkcamp(0, 3) & 0xFF;
                        dx = ax - 0x80;
                        if (func_00195070_tkcamp(dx) > 0x30) {
                            sgn = (0 <= dx) ? 1 : -1;
                            *(float *)AT(0x01ED0040) = *(float *)AT(0x01ED0040) + ((4.0f * (float) (dx - (sgn * 0x30))) / 79.0f);
                        }
                        ax = ay - 0x80;
                        if (func_00195070_tkcamp(ax) > 0x30) {
                            sgn = (0 <= ax) ? 1 : -1;
                            *(float *)AT(0x01ED0048) = *(float *)AT(0x01ED0048) + ((4.0f * (float) (ax - (sgn * 0x30))) / 79.0f);
                        }
                    }
                    func_00203a40_tkcamp(0, 1, 0x100);
                    func_00203a40_tkcamp(0, 2, 0x100);
                    if (*(unsigned short *)AT(0x01ED0004) == 1) {
                        vp[0] = (float) (*(short *)AT(0x01ED0000) - 0x140);
                        vp[1] = (float) (*(short *)AT(0x01ED0002) - 0xE0);
                        vp[2] = 1.0f;
                        vp[3] = 1.0f;
                        vq[0] = (float) (*(short *)AT(0x01ED0000) - 0x140);
                        vq[1] = (float) (*(short *)AT(0x01ED0002) - 0xE0);
                        vq[2] = 100.0f;
                        vq[3] = 1.0f;
                        func_00205cb0_tkcamp(vp, vp[0], vp[1]);
                        *(struct DotrTkCampVec4 *)vq = *(struct DotrTkCampVec4 *)func_001f9ad0_tkcamp(0);
                        func_00206430_tkcamp((void *)AT(0x01ED0190), vp, vq, vr);
                    }
                    if (func_00203a40_tkcamp(0, 1, 0x1000) != 0) {
                        if (*(signed char *)AT(0x01ED0072) != 0) {
                            *(short *)AT(0x01ED007C) = 0;
                            if (func_00203a40_tkcamp(0, 0, 0x1000) != 0) {
                                func_00217710_tkcamp(0, 3);
                            }
                        } else if (*(unsigned short *)AT(0x01ED0004) == 1) {
                            temp_v0_5 = *(short *)AT(0x01ED0002);
                            if (0 < temp_v0_5) {
                                *(short *)AT(0x01ED0002) = (short) (temp_v0_5 - 0x18);
                                if (*(short *)AT(0x01ED0002) <= 0) {
                                    *(short *)AT(0x01ED0002) = 0;
                                }
                            } else {
                                *(short *)AT(0x01ED0002) = 0;
                            }
                        } else {
                            temp_v0_6 = *(short *)AT(0x01ED0002);
                            if (0 < temp_v0_6) {
                                *(short *)AT(0x01ED0002) = (short) (temp_v0_6 - 0x10);
                                if (*(short *)AT(0x01ED0002) <= 0) {
                                    *(short *)AT(0x01ED0002) = 0;
                                }
                            } else {
                                *(short *)AT(0x01ED0002) = 0;
                                *(float *)AT(0x01ED0048) = (float) (*(float *)AT(0x01ED0048) - 4.0f);
                            }
                        }
                    }
                    if (func_00203a40_tkcamp(0, 1, 0x4000) != 0) {
                        if (*(signed char *)AT(0x01ED0072) != 0) {
                            *(short *)AT(0x01ED007C) = 1;
                            if (func_00203a40_tkcamp(0, 0, 0x4000) != 0) {
                                func_00217710_tkcamp(0, 3);
                            }
                        } else if (*(unsigned short *)AT(0x01ED0004) == 1) {
                            temp_v0_7 = *(short *)AT(0x01ED0002);
                            if (temp_v0_7 < 0x1A0) {
                                *(short *)AT(0x01ED0002) = (short) (temp_v0_7 + 0x18);
                                if (*(short *)AT(0x01ED0002) >= 0x1A0) {
                                    *(short *)AT(0x01ED0002) = 0x1A0;
                                }
                            } else {
                                *(short *)AT(0x01ED0002) = 0x1A0;
                            }
                        } else {
                            temp_v0_8 = *(short *)AT(0x01ED0002);
                            if (temp_v0_8 < 0x1A0) {
                                *(short *)AT(0x01ED0002) = (short) (temp_v0_8 + 0x10);
                                if (*(short *)AT(0x01ED0002) >= 0x1A0) {
                                    *(short *)AT(0x01ED0002) = 0x1A0;
                                }
                            } else {
                                *(short *)AT(0x01ED0002) = 0x1A0;
                                *(float *)AT(0x01ED0048) += 4.0f;
                            }
                        }
                    }
                    if (func_00203a40_tkcamp(0, 1, 0x8000) != 0) {
                        if (*(signed char *)AT(0x01ED0072) != 0) {
                            *(short *)AT(0x01ED007A) = 0;
                            if (func_00203a40_tkcamp(0, 0, 0x8000) != 0) {
                                func_00217710_tkcamp(0, 3);
                            }
                        } else if (*(unsigned short *)AT(0x01ED0004) == 1) {
                            temp_v0_9 = *(short *)AT(0x01ED0000);
                            if (0 < temp_v0_9) {
                                *(short *)AT(0x01ED0000) = (short) (temp_v0_9 - 0x18);
                                if (*(short *)AT(0x01ED0000) <= 0) {
                                    *(short *)AT(0x01ED0000) = 0;
                                }
                            } else {
                                *(short *)AT(0x01ED0000) = 0;
                            }
                        } else {
                            temp_v0_10 = *(short *)AT(0x01ED0000);
                            if (0 < temp_v0_10) {
                                *(short *)AT(0x01ED0000) = (short) (temp_v0_10 - 0x10);
                                if (*(short *)AT(0x01ED0000) <= 0) {
                                    *(short *)AT(0x01ED0000) = 0;
                                }
                            } else {
                                *(short *)AT(0x01ED0000) = 0;
                                *(float *)AT(0x01ED0040) = (float) (*(float *)AT(0x01ED0040) - 4.0f);
                            }
                        }
                    }
                    if (func_00203a40_tkcamp(0, 1, 0x2000) != 0) {
                        if (*(signed char *)AT(0x01ED0072) != 0) {
                            *(short *)AT(0x01ED007A) = 1;
                            if (func_00203a40_tkcamp(0, 0, 0x2000) != 0) {
                                func_00217710_tkcamp(0, 3);
                            }
                        } else if (*(unsigned short *)AT(0x01ED0004) == 1) {
                            temp_v0_11 = *(short *)AT(0x01ED0000);
                            if (temp_v0_11 < 0x260) {
                                *(short *)AT(0x01ED0000) = (short) (temp_v0_11 + 0x18);
                                if (*(short *)AT(0x01ED0000) >= 0x260) {
                                    *(short *)AT(0x01ED0000) = 0x260;
                                }
                            } else {
                                *(short *)AT(0x01ED0000) = 0x260;
                            }
                        } else {
                            temp_v0_12 = *(short *)AT(0x01ED0000);
                            if (temp_v0_12 < 0x260) {
                                *(short *)AT(0x01ED0000) = (short) (temp_v0_12 + 0x10);
                                if (*(short *)AT(0x01ED0000) >= 0x260) {
                                    *(short *)AT(0x01ED0000) = 0x260;
                                }
                            } else {
                                *(short *)AT(0x01ED0000) = 0x260;
                                *(float *)AT(0x01ED0040) += 4.0f;
                            }
                        }
                    }
                    if (*(unsigned short *)AT(0x01ED0004) == 1) {
                        vp[0] = (float) (*(short *)AT(0x01ED0000) - 0x140);
                        vp[1] = (float) (*(short *)AT(0x01ED0002) - 0xE0);
                        vp[2] = 1.0f;
                        vp[3] = 1.0f;
                        vq[0] = (float) (*(short *)AT(0x01ED0000) - 0x140);
                        vq[1] = (float) (*(short *)AT(0x01ED0002) - 0xE0);
                        vq[2] = 100.0f;
                        vq[3] = 1.0f;
                        hit = 0;
                        func_00205cb0_tkcamp(vp, vp[0], vp[1]);
                        *(struct DotrTkCampVec4 *)vq = *(struct DotrTkCampVec4 *)func_001f9ad0_tkcamp(0);
                        func_00206430_tkcamp((void *)AT(0x01ED0190), vp, vq, vs);
                        vs[0] -= vr[0];
                        vs[1] -= vr[1];
                        vs[2] -= vr[2];
                        *(float *)AT(0x01ED0040) = (float) (*(float *)AT(0x01ED0040) - vs[0]);
                        *(float *)AT(0x01ED0048) = (float) (*(float *)AT(0x01ED0048) - vs[2]);
                        if (*(float *)AT(0x01ED0040) < -200.0f) {
                            hit++;
                            *(short *)AT(0x01ED0000) = temp_s1;
                        } else if (110.0f < *(float *)AT(0x01ED0040)) {
                            hit++;
                            *(short *)AT(0x01ED0000) = temp_s1;
                        }
                        if (*(float *)AT(0x01ED0048) < -220.0f) {
                            hit++;
                            *(short *)AT(0x01ED0002) = temp_s2;
                        } else if (210.0f < *(float *)AT(0x01ED0048)) {
                            hit++;
                            *(short *)AT(0x01ED0002) = temp_s2;
                        }
                        if (hit) {
                            *(float *)AT(0x01ED0040) = (float) (*(float *)AT(0x01ED0040) + vs[0]);
                            *(float *)AT(0x01ED0048) = (float) (*(float *)AT(0x01ED0048) + vs[2]);
                            vp[0] = (float) (*(short *)AT(0x01ED0000) - 0x140);
                            vp[1] = (float) (*(short *)AT(0x01ED0002) - 0xE0);
                            vp[2] = 1.0f;
                            vp[3] = 1.0f;
                            vq[0] = (float) (*(short *)AT(0x01ED0000) - 0x140);
                            vq[1] = (float) (*(short *)AT(0x01ED0002) - 0xE0);
                            vq[2] = 100.0f;
                            vq[3] = 1.0f;
                            func_00205cb0_tkcamp(vp, vp[0], vp[1]);
                            *(struct DotrTkCampVec4 *)vq = *(struct DotrTkCampVec4 *)func_001f9ad0_tkcamp(0);
                            func_00206430_tkcamp((void *)AT(0x01ED0190), vp, vq, vs);
                            vs[0] -= vr[0];
                            vs[1] -= vr[1];
                            vs[2] -= vr[2];
                            *(float *)AT(0x01ED0040) = (float) (*(float *)AT(0x01ED0040) - vs[0]);
                            *(float *)AT(0x01ED0048) = (float) (*(float *)AT(0x01ED0048) - vs[2]);
                        }
                    }
                    if (*(float *)AT(0x01ED0040) < -200.0f) {
                        *(float *)AT(0x01ED0040) = -200.0f;
                    } else if (110.0f < *(float *)AT(0x01ED0040)) {
                        *(float *)AT(0x01ED0040) = 110.0f;
                    }
                    if (*(float *)AT(0x01ED0048) < -220.0f) {
                        *(float *)AT(0x01ED0048) = -220.0f;
                    } else if (210.0f < *(float *)AT(0x01ED0048)) {
                        *(float *)AT(0x01ED0048) = 210.0f;
                    }
                    func_00203a40_tkcamp(0, 1, 0x10);
                    temp_a1_4 = ((signed char *)((*(signed char *)(ctx + 0xC6)) + ctx))[3];
                    if ((temp_a1_4 != 0) && (*(short *)AT(0x01ED006C) == 0x100)) {
                        {
                            ok = 0;
                            if (*(signed char *)AT(0x01ED018C) != 0) {
                                switch ((*(signed char *)(ctx + 0xC6))) {
                                case 1:
                                case 0xF:
                                case 0x11:
                                    ok = 0;
                                    break;
                                default:
                                    ok = 1;
                                    break;
                                }
                            } else if ((*(signed char *)(ctx + 0)) == 0) {
                                switch ((*(signed char *)(ctx + 0xC6))) {
                                case 1:
                                case 0xF:
                                case 0x11:
                                    ok = 0;
                                    break;
                                default:
                                    if ((signed char) temp_a1_4 > 1) {
                                        ok = 1;
                                        if (((*(signed char *)(ctx + 0xC6)) == 0xB) && ((*(signed char *)(ctx + 0xE)) == 3) && (*(signed char *)AT(0x01ED53C2) == 1)) {
                                            ok = 0;
                                        }
                                    }
                                    break;
                                }
                            } else if ((*(signed char *)(ctx + 0)) == 1) {
                                switch ((*(signed char *)(ctx + 0xC6))) {
                                case 1:
                                case 0xF:
                                case 0x11:
                                    ok = 0;
                                    break;
                                default:
                                    if ((signed char) temp_a1_4 > 1) {
                                        ok = 1;
                                    }
                                    break;
                                }
                            }
                            if (ok) {
                                if ((func_00203a40_tkcamp(0, 0, 0x10) != 0) && (*(unsigned short *)AT(0x01ED0004) == 2) && (*(signed char *)AT(0x01ED0072) == 0)) {
                                    func_00217710_tkcamp(0, 4);
                                    *(signed char *)AT(0x01ED0066) = (signed char) (*(signed char *)AT(0x01ED0066) ^ 1);
                                } else if (*(unsigned short *)AT(0x01ED0004) != 2) {
                                    *(signed char *)AT(0x01ED0066) = 0;
                                }
                            } else {
                                *(signed char *)AT(0x01ED0066) = 0;
                            }
                        }
                    } else {
                        *(signed char *)AT(0x01ED0066) = 0;
                    }
                    if (func_00203a40_tkcamp(0, 0, 0x20) != 0) {
                        if (*(signed char *)AT(0x01ED0072) == 1) {
                            func_00217710_tkcamp(0, 5);
                            *(signed char *)AT(0x01ED0072) = 2;
                        }
                    }
                    if (func_00203a40_tkcamp(0, 1, 0x80) != 0) {
                        if (*(signed char *)AT(0x01ED0072) == 0) {
                            if (func_00203a40_tkcamp(0, 0, 0x80) != 0) {
                                func_00217710_tkcamp(0, 4);
                            }
                            *(unsigned short *)AT(0x01ED0004) = 1U;
                        } else {
                            goto block_306;
                        }
                    } else {
block_306:
                        if (*(unsigned short *)AT(0x01ED0004) == 1) {
                            *(unsigned short *)AT(0x01ED0004) = 0U;
                        }
                    }
                    func_00203a40_tkcamp(0, 2, 0x80);
                    func_00203a40_tkcamp(0, 1, 0x20);
                    func_00203a40_tkcamp(0, 0, 0x20);
                    func_00203a40_tkcamp(0, 1, 4);
                    func_00203a40_tkcamp(0, 2, 4);
                    func_00203a40_tkcamp(0, 1, 1);
                    func_00203a40_tkcamp(0, 2, 1);
                    func_00203a40_tkcamp(0, 1, 0x200);
                    func_00203a40_tkcamp(0, 1, 8);
                    func_00203a40_tkcamp(0, 1, 2);
                    func_00203a40_tkcamp(0, 1, 0x400);
                    if ((*(signed char *)(ctx + 0)) == 0) {
                        if ((*(signed char *)(ctx + 0xE)) < 3) {
                            goto block_312;
                        }
                        goto block_314;
                    }
block_312:
                    if (*(signed char *)AT(0x01ED018C) != 0) {
block_314:
                        if ((*(signed char *)AT(0x01ED547A) == 0) && (*(unsigned short *)AT(0x01ED0004) == 2) && ((*(signed char *)(ctx + 0xC6)) == 0xB) && (*(signed char *)AT(0x01ED53C1) == 0)) {
                            if (func_00203a40_tkcamp(0, 0, 4) != 0) {
                                func_00217710_tkcamp(0, 2);
                                (*(signed char *)(ctx + 0xC9)) = (signed char) ((*(signed char *)(ctx + 0xC9)) - 1);
                                *(signed char *)AT(0x01ED53C1) = 1;
                                if (*(signed char *)AT(0x01ED018C) != 0) {
                                    if ((*(signed char *)(ctx + 0xC9)) < 0) {
                                        (*(signed char *)(ctx + 0xC9)) = 2;
                                    }
                                } else if ((*(signed char *)(ctx + 0xC9)) < 0) {
                                    (*(signed char *)(ctx + 0xC9)) = 1;
                                }
                            } else if (func_00203a40_tkcamp(0, 0, 8) != 0) {
                                func_00217710_tkcamp(0, 2);
                                (*(signed char *)(ctx + 0xC9)) = (signed char) ((*(signed char *)(ctx + 0xC9)) + 1);
                                *(signed char *)AT(0x01ED53C1) = 1;
                                if (*(signed char *)AT(0x01ED018C) != 0) {
                                    (*(signed char *)(ctx + 0xC9)) = (signed char) ((signed char) (*(signed char *)(ctx + 0xC9)) % 3);
                                } else {
                                    (*(signed char *)(ctx + 0xC9)) = (signed char) ((signed char) (*(signed char *)(ctx + 0xC9)) % 2);
                                }
                            }
                        }
                    } else if (((*(signed char *)(ctx + 0)) == 0) && ((*(signed char *)(ctx + 0xE)) < 3)) {
                        (*(signed char *)(ctx + 0xC9)) = 0;
                    }
                }
            }
        }
    }
        if (*(short *)AT(0x01ED536E) != (*(short *)(ctx + 0xC2))) {
            *(short *)AT(0x01ED536C) = 0;
            *(signed char *)AT(0x01ED537A) = 0;
        }
        *(short *)AT(0x01ED536E) = (short) (*(short *)(ctx + 0xC2));
        switch ((*(short *)(ctx + 0xC2))) {
        case 0x2738:
            if (*(signed char *)AT(0x01ED53A6) == 2) {
                if (*(signed char *)AT(0x01ED53C9) == 0) {
                    if (func_00203a40_tkcamp(0, 2, 0x1000) != 0) {
                        func_00217710_tkcamp(0, 2);
                        *(signed char *)AT(0x01ED53C4) = (signed char) (*(signed char *)AT(0x01ED53C4) - 1);
                        if (*(signed char *)AT(0x01ED53C4) < 0) {
                            *(signed char *)AT(0x01ED53C4) = 5;
                        }
                    }
                    if (func_00203a40_tkcamp(0, 2, 0x4000) != 0) {
                        func_00217710_tkcamp(0, 2);
                        *(signed char *)AT(0x01ED53C4) = (signed char) (*(signed char *)AT(0x01ED53C4) + 1);
                        if (*(signed char *)AT(0x01ED53C4) > 5) {
                            *(signed char *)AT(0x01ED53C4) = 0;
                        }
                    }
                    if (func_00203a40_tkcamp(0, 2, 0x8000) != 0) {
                        func_00217710_tkcamp(0, 2);
                        *(signed char *)AT(0x01ED53C3) = (signed char) (*(signed char *)AT(0x01ED53C3) - 1);
                        if (*(signed char *)AT(0x01ED53C3) < 0) {
                            *(signed char *)AT(0x01ED53C3) = 0xE;
                        }
                    }
                    if (func_00203a40_tkcamp(0, 2, 0x2000) != 0) {
                        func_00217710_tkcamp(0, 2);
                        *(signed char *)AT(0x01ED53C3) = (signed char) (*(signed char *)AT(0x01ED53C3) + 1);
                        if (*(signed char *)AT(0x01ED53C3) > 0xE) {
                            *(signed char *)AT(0x01ED53C3) = 0;
                        }
                    }
                    if (func_00203a40_tkcamp(0, 0, 0x20) != 0) {
                        func_00217710_tkcamp(0, 5);
                        *(signed char *)AT(0x01ED53CD) = (signed char) (*(signed char *)AT(0x01ED53CD) - 1);
                        *(signed char *)AT(0x01ED53CE) = *(signed char *)AT(0x01ED53CD) - 1;
                        if (*(signed char *)AT(0x01ED53CD) <= 0) {
                            *(signed char *)AT(0x01ED53CD) = 0;
                            *(signed char *)AT(0x01ED53CE) = 0;
                        }
                        ((signed char *)AT(0x01ED53CF))[*(signed char *)AT(0x01ED53CD)] = 0;
                        ((signed char *)AT(0x01ED53DB))[*(signed char *)AT(0x01ED53CD)] = 6;
                        ((signed char *)AT(0x01ED53E7))[*(signed char *)AT(0x01ED53CD)] = 1;
                    }
                    if (*(signed char *)AT(0x01ED53CA) == 0) {
                        if ((*(signed char *)AT(0x01ED53CD) < 0xC) && (func_00203a40_tkcamp(0, 0, 0xC0) != 0)) {
                            func_00217710_tkcamp(0, 4);
                            *(signed char *)AT(0x01ED53CA) = 1;
                            *(signed char *)AT(0x01ED53C5) = *(signed char *)AT(0x01ED53C3);
                            *(signed char *)AT(0x01ED53C6) = *(signed char *)AT(0x01ED53C4);
                        }
                    }
                    if ((*(signed char *)AT(0x01ED53CA) == 0) && (*(signed char *)AT(0x01ED53CD) != 0) && (func_00203a40_tkcamp(0, 0, 0x800) != 0)) {
                        func_00217710_tkcamp(0, 1);
                        *(signed char *)AT(0x01ED53A6) = 3;
                        func_00224a40_tkcamp();
                    }
                }
            }
            temp_v1_5 = *(signed char *)AT(0x01ED53C8);
            if (temp_v1_5 == 1) {
                temp_v0_15 = *(signed char *)AT(0x01ED53C9);
                if (temp_v0_15 < 0x64) {
                    *(signed char *)AT(0x01ED53C9) = (signed char) (temp_v0_15 + 0xA);
                }
                if (*(signed char *)AT(0x01ED53C9) >= 0x64) {
                    *(signed char *)AT(0x01ED53C9) = 0;
                    *(signed char *)AT(0x01ED53C8) = 0;
                    *(signed char *)AT(0x01ED53C7) = (signed char) (*(signed char *)AT(0x01ED53C7) - 1);
                    if (*(signed char *)AT(0x01ED53C7) < 0) {
                        *(signed char *)AT(0x01ED53C7) = 2;
                    }
                }
            } else if (temp_v1_5 == 2) {
                temp_v0_16 = *(signed char *)AT(0x01ED53C9);
                if (temp_v0_16 > -0x64) {
                    *(signed char *)AT(0x01ED53C9) = (signed char) (temp_v0_16 - 0xA);
                }
                if (*(signed char *)AT(0x01ED53C9) < -0x63) {
                    *(signed char *)AT(0x01ED53C9) = 0;
                    *(signed char *)AT(0x01ED53C8) = 0;
                    *(signed char *)AT(0x01ED53C7) = (signed char) (*(signed char *)AT(0x01ED53C7) + 1);
                    if (*(signed char *)AT(0x01ED53C7) > 2) {
                        *(signed char *)AT(0x01ED53C7) = 0;
                    }
                }
            }
            if (*(signed char *)AT(0x01ED53CA) == 1) {
                temp_v0_17 = *(signed char *)AT(0x01ED53CB);
                if (temp_v0_17 < 0x64) {
                    *(signed char *)AT(0x01ED53CB) = (signed char) (temp_v0_17 + 0x14);
                }
                if (*(signed char *)AT(0x01ED53CB) >= 0x64) {
                    temp_v1_6 = *(signed char *)AT(0x01ED53CD);
                    ((signed char *)AT(0x01ED53CF))[temp_v1_6] = (signed char) *(signed char *)AT(0x01ED53C7);
                    ((signed char *)AT(0x01ED53DB))[temp_v1_6] = (signed char) *(signed char *)AT(0x01ED53C5);
                    ((signed char *)AT(0x01ED53E7))[temp_v1_6] = (signed char) *(signed char *)AT(0x01ED53C6);
                    *(signed char *)AT(0x01ED53CB) = 0;
                    *(signed char *)AT(0x01ED53CA) = 0;
                    *(signed char *)AT(0x01ED53CE) = (signed char) *(signed char *)AT(0x01ED53CD);
                    *(signed char *)AT(0x01ED53CD) = (signed char) (*(signed char *)AT(0x01ED53CD) + 1);
                    if (*(signed char *)AT(0x01ED53CD) >= 0xC) {
                        *(signed char *)AT(0x01ED53CD) = 0xC;
                        *(signed char *)AT(0x01ED53CE) = 0xB;
                    }
                }
            }
            break;
        case 0x2742:
        case 0x276A:
            if (*(signed char *)AT(0x01ED53A6) == 2) {
                var_s1 = 1;
                if ((*(short *)(ctx + 0xC2)) == 0x276A) {
                    if ((*(signed char *)(ctx + 1)) != 0) {
                        var_s1 = 0;
                    }
                    if ((*(signed char *)(ctx + 2)) != 0) {
                        var_s1 = 0;
                    }
                }
                if (func_00203a40_tkcamp(0, 0, 0x1000) != 0) {
                    if (var_s1 != 0) {
                        func_00217710_tkcamp(0, 3);
                        *(signed char *)AT(0x01ED53A8) = 0;
                    } else {
                        func_00217710_tkcamp(0, 6);
                    }
                }
                if (func_00203a40_tkcamp(0, 0, 0x4000) != 0) {
                    if (var_s1 != 0) {
                        func_00217710_tkcamp(0, 3);
                        *(signed char *)AT(0x01ED53A8) = 1;
                    } else {
                        func_00217710_tkcamp(0, 6);
                    }
                }
                if ((func_00203a40_tkcamp(0, 1, 0x1000) == 0) && (func_00203a40_tkcamp(0, 1, 0x4000) == 0) && (func_00203a40_tkcamp(0, 0, 0x800) != 0)) {
                    *(signed char *)AT(0x01ED53A6) = 3;
                    func_00217710_tkcamp(0, 1);
                    if (((*(short *)(ctx + 0xC2)) == 0x2742) && (*(signed char *)AT(0x01ED53A8) == 0)) {
                        func_0023bb00_tkcamp(0);
                    }
                }
            }
            break;
        case 0x2756:
            if ((*(signed char *)AT(0x01ED53A6) == 2) && (*(signed char *)AT(0x01ED5494) == 1)) {
                if (*(signed char *)AT(0x01ED53AA) == 0) {
                    if (func_00203a40_tkcamp(0, 0, 0x2000) != 0) {
                        func_00217710_tkcamp(0, 9);
                        *(signed char *)AT(0x01ED53AA) = 1;
                    }
                    if (func_00203a40_tkcamp(0, 0, 0x8000) != 0) {
                        func_00217710_tkcamp(0, 9);
                        *(signed char *)AT(0x01ED53AA) = 2;
                    }
                }
                temp_v0_18 = *(signed char *)AT(0x01ED53AA);
                if (temp_v0_18 != 0) {
                    switch ((signed char) temp_v0_18) {
                    case 1:
                        *(signed char *)AT(0x01ED53A9) = (signed char) (*(signed char *)AT(0x01ED53A9) + 0x14);
                        if (*(signed char *)AT(0x01ED53A9) >= 0x64) {
                            *(signed char *)AT(0x01ED53A9) = 0;
                            *(signed char *)AT(0x01ED53AA) = 0;
                            *(signed char *)AT(0x01ED53A8) = (signed char) (*(signed char *)AT(0x01ED53A8) + 1);
                            if (*(signed char *)AT(0x01ED53A8) > 2) {
                                *(signed char *)AT(0x01ED53A8) = 0;
                            }
                        }
                        break;
                    case 2:
                        *(signed char *)AT(0x01ED53A9) = (signed char) (*(signed char *)AT(0x01ED53A9) + 0x14);
                        if (*(signed char *)AT(0x01ED53A9) >= 0x64) {
                            *(signed char *)AT(0x01ED53A9) = 0;
                            *(signed char *)AT(0x01ED53AA) = 0;
                            *(signed char *)AT(0x01ED53A8) = (signed char) (*(signed char *)AT(0x01ED53A8) - 1);
                            if (*(signed char *)AT(0x01ED53A8) < 0) {
                                *(signed char *)AT(0x01ED53A8) = 2;
                            }
                        }
                        break;
                    }
                }
                if ((func_00203a40_tkcamp(0, 0, 0x800) != 0) && (*(signed char *)AT(0x01ED53AA) == 0)) {
                    func_00217710_tkcamp(0, 1);
                    *(signed char *)AT(0x01ED53A6) = 3;
                    *(signed char *)AT(0x01ED5494) = 2;
                }
            }
            /* fallthrough */
        case 0x2757:
            /* fallthrough */
        case 0x2760:
            if (*(signed char *)AT(0x01ED5494) == 4) {
                func_00214990_tkcamp(0, (int) ((unsigned short *)AT(0x01ED53F8))[*(signed char *)AT(0x01ED53A8)]);
                if (func_001f12a0_tkcamp(0x1E) == 0) {
                    spE0 = data_0029b000_tkcamp;
                    spF0 = data_0029b010_tkcamp;
                    temp_v0_19 = func_002158b0_tkcamp(0);
                    temp_s1_2 = func_002159d0_tkcamp(0, temp_v0_19);
                    *(short *)AT(0x01ED5404) = func_002159d0_tkcamp(0, temp_v0_19);
                    func_001f22b0_tkcamp(0x1E, &spE0);
                    func_001f2430_tkcamp(0x1E, &spF0);
                    func_001e8780_tkcamp(0x1E, -1, func_001de1a0_tkcamp(temp_s1_2), 1);
                }
                *(signed char *)AT(0x01ED5494) = 5;
            }
            break;
        }
        if (*(short *)AT(0x01ED007A) != 0) {
            if (*(short *)AT(0x01ED007C) != 0) {
                spot = 3;
            } else {
                spot = 1;
            }
        } else {
            spot = 0;
            if (*(short *)AT(0x01ED007C) != 0) {
                spot = 2;
            }
        }
        temp_v1_7 = *(signed char *)AT(0x01ED0072);
        if (temp_v1_7 == 1) {
            if (*(short *)AT(0x01ED0074) == 0) {
                *(short *)AT(0x01ED0076) = *(short *)AT(0x01ED0000);
                *(short *)AT(0x01ED0078) = *(short *)AT(0x01ED0002);
            }
            *(short *)AT(0x01ED0074) = (short) (*(short *)AT(0x01ED0074) + 0x20);
            if (*(short *)AT(0x01ED0074) >= 0x100) {
                *(short *)AT(0x01ED0074) = 0x100;
            }
            *(short *)AT(0x01ED0000) = *(short *)AT(0x01ED0076) + ((*(short *)AT(0x01ED0074) * ((data_00298260_tkcamp[spot * 0x13] + 0x10) - *(short *)AT(0x01ED0076))) >> 8);
            *(short *)AT(0x01ED0002) = *(short *)AT(0x01ED0078) + ((*(short *)AT(0x01ED0074) * ((data_00298260_tkcamp[spot * 0x13 + 1] + 0x10) - *(short *)AT(0x01ED0078))) >> 8);
        } else if (temp_v1_7 == 2) {
            *(short *)AT(0x01ED0074) = (short) (*(short *)AT(0x01ED0074) - 0x20);
            if (*(short *)AT(0x01ED0074) <= 0) {
                *(short *)AT(0x01ED0074) = 0;
                *(signed char *)AT(0x01ED0072) = 0;
                *(short *)AT(0x01ED007A) = 0;
                *(short *)AT(0x01ED007C) = 0;
            }
            *(short *)AT(0x01ED0000) = *(short *)AT(0x01ED0076) + ((*(short *)AT(0x01ED0074) * ((data_00298260_tkcamp[spot * 0x13] + 0x10) - *(short *)AT(0x01ED0076))) >> 8);
            *(short *)AT(0x01ED0002) = *(short *)AT(0x01ED0078) + ((*(short *)AT(0x01ED0074) * ((data_00298260_tkcamp[spot * 0x13 + 1] + 0x10) - *(short *)AT(0x01ED0078))) >> 8);
        }
        func_00226600_tkcamp();
        if (*(signed char *)AT(0x01ED537B) == 1) {
            func_0022d140_tkcamp();
        }
        if ((*(signed char *)(ctx + 0)) == 1) {
            if (((*(short *)(ctx + 0xC2)) == 0x68) || ((*(short *)(ctx + 0xC2)) == 0x69)) {
                var_s3 = 1;
            }
        }
        if (((*(signed char *)(ctx + 0)) == 1) && ((*(short *)(ctx + 0xC2)) == 0x6F)) {
            if (*(signed char *)AT(0x01ED537B) == 1) {
                func_0022d140_tkcamp();
            }
            if (*(signed char *)AT(0x01ED537B) == 0xC) {
                *(signed char *)AT(0x01ED537B) = 0xD;
            }
            if (*(signed char *)AT(0x01ED537B) == 4) {
                if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                    func_00217710_tkcamp(0, 2);
                    *(signed char *)AT(0x01ED537B) = (signed char) (*(signed char *)AT(0x01ED537B) + 1);
                    func_0022d140_tkcamp();
                }
            }
        } else if (((*(short *)(ctx + 0xC2)) >= 0x2711) || (var_s3 != 0)) {
            if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                if (*(signed char *)AT(0x01ED537B) == 4) {
                    *(signed char *)AT(0x01ED537B) = 5;
                    *(signed char *)AT(0x01ED53B2) = 0;
                    func_00217710_tkcamp(0, 2);
                }
                if (*(signed char *)AT(0x01ED53AC) == 2) {
                    *(signed char *)AT(0x01ED53AC) = 3;
                    *(signed char *)AT(0x01ED53B2) = 0;
                    func_00217710_tkcamp(0, 2);
                }
                func_0022d140_tkcamp();
            }
        } else if (*(unsigned short *)AT(0x01ED0004) == 2) {
            var_a0_4 = 0;
            if (*(signed char *)AT(0x01ED547A) == 0) {
                if ((*(signed char *)(ctx + 0)) != 0) {
                    if ((*(signed char *)(ctx + 0xC6)) == 0xF) {
                        var_a0_4 = 1;
                    }
                }
                if ((*(short *)AT(0x01ED5442) <= *(short *)AT(0x01ED5444)) || (*(signed char *)AT(0x01ED537B) != 0) || (var_a0_4 != 0)) {
                    var_s1_2 = 0;
                    if (*(signed char *)AT(0x01ED537B) == 0xC) {
                        if (*(signed char *)AT(0x01ED53A0) <= 0) {
                            if (*(signed char *)AT(0x01ED018C) == 0) {
                                temp_v0_20 = *(signed char *)AT(0x01ED5440);
                                if (!(0 > temp_v0_20)) {
                                    *(signed char *)AT(0x01ED5440) = (signed char) (temp_v0_20 - 0xA);
                                    if (*(signed char *)AT(0x01ED5440) <= 0) {
                                        *(signed char *)AT(0x01ED5440) = 0;
                                    }
                                }
                            }
                        }
                    }
                    if ((*(signed char *)AT(0x01ED5376) != 0) && (*(signed char *)AT(0x01ED5482) == 0) && (func_001f5240_tkcamp(-1, -1) == 0) && (*(signed char *)AT(0x01ED5374) == 0) && (*(signed char *)AT(0x01ED5375) == 0) && ((*(short *)(ctx + 0xC2)) < 0xC8)) {
                        var_s1_2 = 1;
                    }
                    if ((((signed char *)((*(signed char *)(ctx + 0xC6)) + ctx))[3] != 0) && (*(short *)AT(0x01ED006C) == 0x100)) {
                        if ((*(signed char *)(ctx + 0)) == 0) {
                            switch ((*(signed char *)(ctx + 0xC6))) {
                            case 15:
                            case 17:
                                func_00199b50_tkcamp(&data_0030f1f6_tkcamp, (*(signed char *)(ctx + 0xC6)));
                                break;
                            case 0:
                            case 1:
                                break;
                            default:
                                if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                                    if ((*(signed char *)AT(0x01ED546D) == 0) && (*(signed char *)AT(0x01ED537B) == 0) && (var_s1_2 != 0)) {
                                        func_00217710_tkcamp(0, 4);
                                        *(signed char *)AT(0x01ED546D) = 1;
                                        func_00199b50_tkcamp(&data_0030f213_tkcamp, (*(signed char *)(ctx + 0xC6)));
                                    }
                                    if (((signed char *)((*(signed char *)(ctx + 0xC6)) + ctx))[3] == 1) {
                                        if (*(signed char *)AT(0x01ED537B) == 0) {
                                            *(signed char *)AT(0x01ED537B) = 1;
                                            *(signed char *)AT(0x01ED53A0) = 0;
                                            *(signed char *)AT(0x01ED5375) = 1;
                                            if (*(signed char *)AT(0x01ED5373) == 0) {
                                                if (var_s1_2 != 0) {
                                                    *(signed char *)AT(0x01ED5373) = 1;
                                                }
                                            }
                                            *(signed char *)AT(0x01ED537D) = 1;
                                        }
                                        temp_v1_8 = *(signed char *)AT(0x01ED537B);
                                        if (temp_v1_8 == 4) {
                                            *(signed char *)AT(0x01ED537B) = (signed char) (temp_v1_8 + 1);
                                            func_00217710_tkcamp(0, 2);
                                        }
                                        func_0022d140_tkcamp();
                                    } else if ((*(signed char *)(ctx + 0xC6)) == 0xB) {
                                        if (((*(signed char *)(ctx + 0xE)) == 3) && (*(signed char *)AT(0x01ED53C2) == 1)) {
                                            if (*(signed char *)AT(0x01ED537B) == 0) {
                                                *(signed char *)AT(0x01ED537B) = 1;
                                                *(signed char *)AT(0x01ED53A0) = 0;
                                                *(signed char *)AT(0x01ED5375) = 1;
                                                if (*(signed char *)AT(0x01ED5373) == 0) {
                                                    if (var_s1_2 != 0) {
                                                        *(signed char *)AT(0x01ED5373) = 1;
                                                    }
                                                }
                                                *(signed char *)AT(0x01ED537D) = 1;
                                            }
                                            temp_v1_9 = *(signed char *)AT(0x01ED537B);
                                            if (temp_v1_9 == 4) {
                                                *(signed char *)AT(0x01ED537B) = (signed char) (temp_v1_9 + 1);
                                                func_00217710_tkcamp(0, 2);
                                            }
                                            func_0022d140_tkcamp();
                                        } else {
                                            goto block_544;
                                        }
                                    } else {
block_544:
                                        *(signed char *)AT(0x01ED5375) = 1;
                                        if (*(signed char *)AT(0x01ED5373) == 0) {
                                            if (var_s1_2 != 0) {
                                                *(signed char *)AT(0x01ED5373) = 1;
                                            }
                                        }
                                        temp_v1_10 = *(signed char *)AT(0x01ED537B);
                                        if (temp_v1_10 == 4) {
                                            *(signed char *)AT(0x01ED537B) = (signed char) (temp_v1_10 + 1);
                                            func_00217710_tkcamp(0, 2);
                                        }
                                    }
                                }
                                break;
                            }
                        } else {
                            switch ((*(signed char *)(ctx + 0xC6))) {
                            case 1:
                                func_00199b50_tkcamp(&data_0030f227_tkcamp, (*(signed char *)(ctx + 0xC6)));
                                break;
                            case 15:
                                if (((*(signed char *)(ctx + 0x13)) == 0) && (func_00203a40_tkcamp(0, 0, 0x40) != 0)) {
                                    if (*(signed char *)AT(0x01ED546D) == 0) {
                                        if ((*(signed char *)AT(0x01ED537B) == 0) && (var_s1_2 != 0)) {
                                            func_00217710_tkcamp(0, 4);
                                            *(signed char *)AT(0x01ED546D) = 1;
                                        }
                                    }
                                    if (*(signed char *)AT(0x01ED537B) == 0) {
                                        *(signed char *)AT(0x01ED53A0) = 0;
                                        *(signed char *)AT(0x01ED5375) = 1;
                                        if ((*(signed char *)AT(0x01ED5373) == 0) && (var_s1_2 != 0)) {
                                            *(signed char *)AT(0x01ED5373) = 1;
                                        }
                                    }
                                }
                                break;
                            case 17:
                                break;
                            default:
                                if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                                    if ((*(signed char *)AT(0x01ED546D) == 0) && (*(signed char *)AT(0x01ED537B) == 0) && (var_s1_2 != 0)) {
                                        func_00217710_tkcamp(0, 4);
                                        *(signed char *)AT(0x01ED546D) = 1;
                                        func_00199b50_tkcamp(&data_0030f244_tkcamp, (*(signed char *)(ctx + 0xC6)));
                                    }
                                    if (((signed char *)((*(signed char *)(ctx + 0xC6)) + ctx))[3] == 1) {
                                        if (*(signed char *)AT(0x01ED537B) == 0) {
                                            *(signed char *)AT(0x01ED537B) = 1;
                                            *(signed char *)AT(0x01ED537D) = 1;
                                            *(signed char *)AT(0x01ED53A0) = 0;
                                            *(signed char *)AT(0x01ED5375) = 1;
                                            if ((*(signed char *)AT(0x01ED5373) == 0) && (var_s1_2 != 0)) {
                                                *(signed char *)AT(0x01ED5373) = 1;
                                            }
                                        }
                                        temp_v1_11 = *(signed char *)AT(0x01ED537B);
                                        if (temp_v1_11 == 4) {
                                            *(signed char *)AT(0x01ED537B) = (signed char) (temp_v1_11 + 1);
                                            func_00217710_tkcamp(0, 2);
                                        }
                                        func_0022d140_tkcamp();
                                    } else {
                                        *(signed char *)AT(0x01ED5375) = 1;
                                        if (*(signed char *)AT(0x01ED5373) == 0) {
                                            if (var_s1_2 != 0) {
                                                *(signed char *)AT(0x01ED5373) = 1;
                                            }
                                        }
                                        temp_v1_12 = *(signed char *)AT(0x01ED537B);
                                        if (temp_v1_12 == 4) {
                                            *(signed char *)AT(0x01ED537B) = (signed char) (temp_v1_12 + 1);
                                            func_00217710_tkcamp(0, 2);
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                } else {
                    temp_s1_3 = (*(signed char *)(ctx + 0));
                    var_s2 = 0;
                    if ((*(short *)AT(0x01ED006C) == 0x100) && (*(signed char *)AT(0x01ED5376) != 0) && (*(signed char *)AT(0x01ED5482) == 0) && (func_001f5240_tkcamp(-1, -1) == 0) && (*(signed char *)AT(0x01ED5374) == 0) && (*(signed char *)AT(0x01ED5375) == 0) && ((*(short *)(ctx + 0xC2)) < 0xC8)) {
                        var_s2 = 1;
                    }
                    if (var_s2 != 0) {
                        if (temp_s1_3 == 0) {
                            switch ((*(signed char *)(ctx + 0xC6))) {
                            case 0:
                            case 1:
                                break;
                            case 11:
                                if (((*(signed char *)(ctx + 0xE)) != 0) && (func_00203a40_tkcamp(0, 1, 0x40) != 0)) {
                                    if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                                        func_00217710_tkcamp(0, 6);
                                    }
                                    *(signed char *)AT(0x01ED5446) = 1;
                                }
                                break;
                            default:
                                if (func_00203a40_tkcamp(0, 1, 0x40) != 0) {
                                    if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                                        func_00217710_tkcamp(0, 6);
                                    }
                                    *(signed char *)AT(0x01ED5446) = 1;
                                }
                                break;
                            }
                        } else {
                            switch ((*(signed char *)(ctx + 0xC6))) {
                            case 15:
                            case 17:
                                break;
                            case 11:
                                if (((*(signed char *)(ctx + 0xE)) != 0) && (func_00203a40_tkcamp(0, 1, 0x40) != 0)) {
                                    if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                                        func_00217710_tkcamp(0, 6);
                                    }
                                    *(signed char *)AT(0x01ED5446) = 1;
                                }
                                break;
                            default:
                                if (func_00203a40_tkcamp(0, 1, 0x40) != 0) {
                                    if (func_00203a40_tkcamp(0, 0, 0x40) != 0) {
                                        func_00217710_tkcamp(0, 6);
                                    }
                                    *(signed char *)AT(0x01ED5446) = 1;
                                }
                                break;
                            }
                        }
                    }
                }
            } else {
                goto block_623;
            }
        } else {
block_623:
            *(signed char *)AT(0x01ED5446) = 0;
        }
        temp_a1_7 = *(signed char *)AT(0x01ED53B3);
        switch (temp_a1_7) {
        case 1:
            temp_v0_21 = *(signed char *)AT(0x01ED0062);
            if (temp_v0_21 == 1) {
                *(short *)AT(0x01ED0064) = (short) (*(short *)AT(0x01ED0064) + 0x10);
                if (*(short *)AT(0x01ED0064) > 0x1000) {
                    *(short *)AT(0x01ED0064) = 0x1000;
                    *(signed char *)AT(0x01ED0062) = 0;
                }
            } else if (temp_v0_21 == 2) {
                *(short *)AT(0x01ED0064) = (short) (*(short *)AT(0x01ED0064) - 0x10);
                if (*(short *)AT(0x01ED0064) < 0) {
                    *(short *)AT(0x01ED0064) = 0;
                    *(signed char *)AT(0x01ED0062) = 0;
                }
            }
            *(float *)AT(0x01ED0040) = 0.0f;
            *(float *)AT(0x01ED0048) = 0.0f;
            *(int *)AT(0x01ED004C) = 0;
            *(float *)AT(0x01ED0030) = (float) *(float *)AT(0x01ED0040);
            *(float *)AT(0x01ED0034) = (float) (20.0f + *(float *)AT(0x01ED0044) + ((1700.0f * (float) *(short *)AT(0x01ED0064)) / 4096.0f));
            *(float *)AT(0x01ED0038) = (float) (1.0f + *(float *)AT(0x01ED0048));
            *(float *)AT(0x01ED003C) = *(float *)AT(0x01ED004C);
            *(short *)AT(0x01ED53F4) = (short) (*(short *)AT(0x01ED53F4) - 0x300);
            break;
        case 2:
            temp_v1_13 = *(signed char *)AT(0x01ED0062);
            if (temp_v1_13 == 1) {
                *(short *)AT(0x01ED0064) = (short) (*(short *)AT(0x01ED0064) + 0x100);
                if (*(short *)AT(0x01ED0064) > 0x1000) {
                    *(short *)AT(0x01ED0064) = 0x1000;
                    *(signed char *)AT(0x01ED0062) = 0;
                }
            } else if (temp_v1_13 == 2) {
                *(short *)AT(0x01ED0064) = (short) (*(short *)AT(0x01ED0064) - 0x100);
                if (*(short *)AT(0x01ED0064) < 0) {
                    *(short *)AT(0x01ED0064) = 0;
                    *(signed char *)AT(0x01ED0062) = 0;
                }
            }
            if (*(signed char *)AT(0x01ED547F) != 0) {
                *(float *)AT(0x01ED548C) = 0.0f;
                *(float *)AT(0x01ED5490) = 0.0f;
                temp_a1_8 = *(signed char *)AT(0x01ED547F);
                switch (temp_a1_8) {
                case 1:
                    *(signed char *)AT(0x01ED5480) = (signed char) (*(signed char *)AT(0x01ED5480) + 5);
                    if (*(signed char *)AT(0x01ED5480) >= 0x64) {
                        *(signed char *)AT(0x01ED5480) = 0x64;
                        *(signed char *)AT(0x01ED547F) = 2;
                    }
                    break;
                case 2:
                    *(signed char *)AT(0x01ED5480) = 0x64;
                    if (*(signed char *)AT(0x01ED546E) == 5) {
                        *(signed char *)AT(0x01ED547F) = 3;
                    }
                    break;
                case 3:
                    *(signed char *)AT(0x01ED5480) = (signed char) (*(signed char *)AT(0x01ED5480) - 5);
                    if (*(signed char *)AT(0x01ED5480) <= 0) {
                        *(signed char *)AT(0x01ED5480) = 0;
                        *(signed char *)AT(0x01ED547F) = 4;
                    }
                    break;
                case 4:
                    *(unsigned char *)AT(0x01ED547E) = 0;
                    break;
                }
                temp_hi = (int) *(unsigned char *)AT(0x01ED547E) % 3;
                switch (temp_hi) {
                case 0:
                    *(float *)AT(0x01ED548C) = 0.0f;
                    break;
                case 1:
                    var_f0 = (-2.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED548C) = var_f0;
                    break;
                case 2:
                    var_f0 = (2.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED548C) = var_f0;
                    break;
                }
                temp_hi_2 = (int) *(unsigned char *)AT(0x01ED547E) % 5;
                switch (temp_hi_2) {
                case 0:
                    *(float *)AT(0x01ED5490) = 0.0f;
                    break;
                case 1:
                    var_f0_2 = (-2.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED5490) = var_f0_2;
                    break;
                case 2:
                    var_f0_2 = (2.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED5490) = var_f0_2;
                    break;
                case 3:
                    var_f0_2 = (-1.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED5490) = var_f0_2;
                    break;
                case 4:
                    var_f0_2 = (float) *(signed char *)AT(0x01ED5480) / 100.0f;
                    *(float *)AT(0x01ED5490) = var_f0_2;
                    break;
                }
                *(float *)AT(0x01ED0030) = (float) *(float *)AT(0x01ED548C);
                *(float *)AT(0x01ED0034) = (float) *(float *)AT(0x01ED5490);
                *(float *)AT(0x01ED0038) = 512.0f;
                *(int *)AT(0x01ED003C) = 0;
                *(float *)AT(0x01ED0040) = (float) (((-64.0f * (float) *(signed char *)AT(0x01ED5437)) / 100.0f) + *(float *)AT(0x01ED548C));
                *(float *)AT(0x01ED0044) = (float) (20.2f + ((768.0f * (float) *(short *)AT(0x01ED0064)) / 4096.0f) + *(float *)AT(0x01ED5490));
                *(float *)AT(0x01ED0048) = 0.0f;
                *(int *)AT(0x01ED004C) = 0;
                *(unsigned char *)AT(0x01ED547E) = (unsigned char) (*(unsigned char *)AT(0x01ED547E) + 1);
            } else {
                *(float *)AT(0x01ED0030) = 0.0f;
                *(float *)AT(0x01ED0034) = 0.0f;
                *(float *)AT(0x01ED0038) = 512.0f;
                *(int *)AT(0x01ED003C) = 0;
                *(float *)AT(0x01ED0040) = (float) ((-64.0f * (float) *(signed char *)AT(0x01ED5437)) / 100.0f);
                *(float *)AT(0x01ED0044) = (float) (20.2f + ((768.0f * (float) *(short *)AT(0x01ED0064)) / 4096.0f));
                *(float *)AT(0x01ED0048) = 0.0f;
                *(int *)AT(0x01ED004C) = 0;
            }
            break;
        case 3:
            temp_v1_14 = *(signed char *)AT(0x01ED0062);
            if (temp_v1_14 == 1) {
                *(short *)AT(0x01ED0064) = (short) (*(short *)AT(0x01ED0064) + 0x80);
                if (*(short *)AT(0x01ED0064) > 0x1000) {
                    *(short *)AT(0x01ED0064) = 0x1000;
                    *(signed char *)AT(0x01ED0062) = 0;
                }
            } else if (temp_v1_14 == 2) {
                *(short *)AT(0x01ED0064) = (short) (*(short *)AT(0x01ED0064) - 0x80);
                if (*(short *)AT(0x01ED0064) < 0) {
                    *(short *)AT(0x01ED0064) = 0;
                    *(signed char *)AT(0x01ED0062) = 0;
                }
            }
            temp_v0_22 = *(signed char *)AT(0x01ED547F);
            if (temp_v0_22 != 0) {
                switch ((signed char) temp_v0_22) {
                case 1:
                    *(signed char *)AT(0x01ED5480) = (signed char) (*(signed char *)AT(0x01ED5480) + 5);
                    if (*(signed char *)AT(0x01ED5480) >= 0x64) {
                        *(signed char *)AT(0x01ED5480) = 0x64;
                        *(signed char *)AT(0x01ED547F) = 2;
                    }
                    break;
                case 2:
                    *(signed char *)AT(0x01ED5480) = 0x64;
                    if (*(signed char *)AT(0x01ED546E) == 5) {
                        *(signed char *)AT(0x01ED547F) = 3;
                    }
                    break;
                case 3:
                    *(signed char *)AT(0x01ED5480) = (signed char) (*(signed char *)AT(0x01ED5480) - 5);
                    if (*(signed char *)AT(0x01ED5480) <= 0) {
                        *(signed char *)AT(0x01ED5480) = 0;
                        *(signed char *)AT(0x01ED547F) = 4;
                    }
                    break;
                case 4:
                    *(unsigned char *)AT(0x01ED547E) = 0U;
                    break;
                }
                temp_hi_3 = (int) *(unsigned char *)AT(0x01ED547E) % 3;
                switch (temp_hi_3) {
                case 0:
                    *(float *)AT(0x01ED548C) = 0.0f;
                    break;
                case 1:
                    var_f0_3 = (-2.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED548C) = var_f0_3;
                    break;
                case 2:
                    var_f0_3 = (2.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED548C) = var_f0_3;
                    break;
                }
                temp_hi_4 = (int) *(unsigned char *)AT(0x01ED547E) % 5;
                switch (temp_hi_4) {
                case 0:
                    *(float *)AT(0x01ED5490) = 0.0f;
                    break;
                case 1:
                    fq4 = (-2.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED5490) = fq4;
                    break;
                case 2:
                    fq4 = (2.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED5490) = fq4;
                    break;
                case 3:
                    fq4 = (-1.0f * (float) *(signed char *)AT(0x01ED5480)) / 100.0f;
                    *(float *)AT(0x01ED5490) = fq4;
                    break;
                case 4:
                    fq4 = (float) *(signed char *)AT(0x01ED5480) / 100.0f;
                    *(float *)AT(0x01ED5490) = fq4;
                    break;
                }
                *(float *)AT(0x01ED0030) = (float) *(float *)AT(0x01ED548C);
                *(float *)AT(0x01ED0034) = (float) *(float *)AT(0x01ED5490);
                *(float *)AT(0x01ED0038) = 512.0f;
                *(int *)AT(0x01ED003C) = 0;
                *(float *)AT(0x01ED0040) = (float) (((-64.0f * (float) *(signed char *)AT(0x01ED5437)) / 100.0f) + *(float *)AT(0x01ED548C));
                *(float *)AT(0x01ED0044) = (float) (20.2f + ((512.0f * (float) *(short *)AT(0x01ED0064)) / 4096.0f) + *(float *)AT(0x01ED5490));
                *(float *)AT(0x01ED0048) = 0.0f;
                *(int *)AT(0x01ED004C) = 0;
                *(unsigned char *)AT(0x01ED547E) = (unsigned char) (*(unsigned char *)AT(0x01ED547E) + 1);
            } else {
                *(float *)AT(0x01ED0030) = 0.0f;
                *(float *)AT(0x01ED0034) = 0.0f;
                *(float *)AT(0x01ED0038) = 512.0f;
                *(int *)AT(0x01ED003C) = 0;
                *(float *)AT(0x01ED0040) = (float) ((-64.0f * (float) *(signed char *)AT(0x01ED5437)) / 100.0f);
                *(float *)AT(0x01ED0044) = (float) (20.2f + ((512.0f * (float) *(short *)AT(0x01ED0064)) / 4096.0f));
                *(float *)AT(0x01ED0048) = 0.0f;
                *(int *)AT(0x01ED004C) = 0;
                *(unsigned char *)AT(0x01ED547E) = 0U;
            }
            break;
        default:
            c23 = *(signed char *)AT(0x01ED0062);
            if (c23 == 1) {
                *(short *)AT(0x01ED0064) = (short) (*(short *)AT(0x01ED0064) + 8);
                if (*(short *)AT(0x01ED0064) > 0x100) {
                    *(short *)AT(0x01ED0064) = 0x100;
                    *(signed char *)AT(0x01ED0062) = 0;
                }
            } else if (c23 == 2) {
                *(short *)AT(0x01ED0064) = (short) (*(short *)AT(0x01ED0064) - 8);
                if (*(short *)AT(0x01ED0064) < 0) {
                    *(short *)AT(0x01ED0064) = 0;
                    *(signed char *)AT(0x01ED0062) = 0;
                }
            }
            *(float *)AT(0x01ED0044) = 0.0f;
            *(float *)AT(0x01ED0030) = *(float *)AT(0x01ED0040);
            *(float *)AT(0x01ED0034) = 96.0f + *(float *)AT(0x01ED0044) + ((96.0f * (float) *(short *)AT(0x01ED0064)) / 256.0f);
            *(float *)AT(0x01ED0038) = 96.0f + *(float *)AT(0x01ED0048);
            *(float *)AT(0x01ED003C) = *(float *)AT(0x01ED004C);
            break;
        }
        func_001f9fe0_tkcamp(0, (void *)AT(0x01ED0030), (void *)AT(0x01ED0040));
        func_0023b1f0_tkcamp(0);
end_TkCamp_Act:;
}

#ifdef __MWERKS__
#pragma opt_dead_assignments reset
#pragma conditional_move reset
#endif

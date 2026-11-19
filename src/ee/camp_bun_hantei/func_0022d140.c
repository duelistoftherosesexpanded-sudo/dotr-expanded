#include "dotr/ee/camp_bun_hantei.h"

/* Taku_Camp_BUN_HANTEI (0x22d140): chooses the campaign event message. From the camp counter
 * at 0x299e10+0xc2, the side byte and the selected location it sets the first message number
 * (0x1ed5378, often the current page 0x1ed537d plus a base), the message count (0x1ed537c),
 * the panel mode (0x1ed537e) and, for some events, the opponent panels' name number, side and
 * flags. Literal work-area addresses use the host-safe macros of the header.
 * Three spellings avoid host-build warnings and compile to the same bytes: two unused locals are
 * left out, the byte switched on with cases 0x8fc..0x8ff (never true) is cast to int, and `ret`,
 * which is returned but never set, is 0 on the host only. */
int Taku_Camp_BUN_HANTEI(void)
{
    extern short CutSceneNumber_bunhantei;
    int player;
#ifdef __MWERKS__
    int ret; /* never set: the target returns whatever v0 holds */
#else
    int ret = 0; /* host build only: a defined value instead of the target's leftover v0 */
#endif
    signed char *context;
    signed char temp_a0;
    signed char temp_a1;
    short temp_v1_2;
    short temp_v1_3;
    short temp_a0_2;
    signed char temp_a0_3;
    short temp_a0_4;
    signed char temp_a0_5;
    short temp_v1_5;
    short temp_a0_6;

    player = data_00299e10_bunhantei;
    context = &data_00299e10_bunhantei;
    if (CutSceneNumber_bunhantei < 0xC8) {
        DOTR_BUN_MODE = 0;
        if ((short)player == 0) {
            switch (context[0xC6]) {
            case 2:
                DOTR_BUN_MSG = 0x887;
                DOTR_BUN_COUNT = 1;
                break;
            case 3:
                DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x89E;
                DOTR_BUN_COUNT = 6;
                break;
            case 4:
                DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x88D;
                DOTR_BUN_COUNT = 2;
                break;
            case 5:
                DOTR_BUN_MSG = 0x895;
                DOTR_BUN_COUNT = 1;
                break;
            case 6:
                DOTR_BUN_MSG = 0x898;
                DOTR_BUN_COUNT = 1;
                break;
            case 7:
                DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8AD;
                DOTR_BUN_COUNT = 7;
                break;
            case 8:
                DOTR_BUN_MSG = 0x88A;
                DOTR_BUN_COUNT = 1;
                break;
            case 9:
                DOTR_BUN_MSG = 0x892;
                DOTR_BUN_COUNT = 1;
                break;
            case 10:
                DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x89A;
                DOTR_BUN_COUNT = 2;
                break;
            case 11:
                if (context[0xE] == 1 || context[0xE] == 2) {
                    DOTR_BUN_COUNT = 9;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8DF;
                } else if (context[0xE] >= 3) {
                    if (DOTR_BUN_BYTE_53C2 == 1) {
                        DOTR_BUN_COUNT = 1;
                        DOTR_BUN_MSG = 0x902;
                    } else {
                        DOTR_BUN_COUNT = 9;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8DF;
                    }
                }
                break;
            }
            if (*(short *)(context + 0xC2) != 0x64) {
                return ret;
            }
            DOTR_BUN_COUNT = 1;
            DOTR_BUN_MSG = 0x86C;
            DOTR_BUN_MODE = 1;
            return ret;
        }
        switch (context[0xC6]) {
        case 12:
            DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x935;
            DOTR_BUN_COUNT = 2;
            break;
        case 13:
            DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x939;
            DOTR_BUN_COUNT = 2;
            break;
        case 14:
            DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x93D;
            DOTR_BUN_COUNT = 2;
            break;
        case 16:
            DOTR_BUN_MSG = 0x94A;
            DOTR_BUN_COUNT = 1;
            break;
        case 18:
            DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x94E;
            DOTR_BUN_COUNT = 3;
            break;
        case 19:
            DOTR_BUN_COUNT = 4;
            DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x953;
            break;
        case 20:
            DOTR_BUN_MSG = 0x95C;
            DOTR_BUN_COUNT = 1;
            break;
        case 21:
            DOTR_BUN_COUNT = 5;
            DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x95E;
            break;
        case 0:
            DOTR_BUN_COUNT = 2;
            DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x968;
            break;
        case 11:
            DOTR_BUN_COUNT = 1;
            DOTR_BUN_MSG = 0x983;
            break;
        }
        switch (*(short *)(context + 0xC2)) {
        case 0x69:
            DOTR_BUN_COUNT = 1;
            DOTR_BUN_MSG = 0x94E;
            DOTR_BUN_MODE = 1;
            return ret;
        case 0x6F:
            DOTR_BUN_COUNT = 1;
            DOTR_BUN_MSG = 0x970;
            DOTR_BUN_MODE = 2;
            DOTR_BUN_NAME1 = 0xC5;
            DOTR_BUN_SIDE1 = 1;
            DOTR_BUN_FLAG1 = 1;
            return ret;
        }
    } else if (*(short *)(context + 0xC2) >= 0xC8 && *(short *)(context + 0xC2) < 0x12C) {
            {
                DOTR_BUN_MODE = 0;
                if ((short)player == 0) {
                    switch (context[0xC6]) {
                    case 2:
                        DOTR_BUN_MSG = 0x888;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 3:
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8A4;
                        DOTR_BUN_COUNT = 2;
                        return ret;
                    case 4:
                        DOTR_BUN_MSG = 0x890;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 5:
                        DOTR_BUN_MSG = 0x896;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 6:
                        DOTR_BUN_MSG = 0x899;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 7:
                        DOTR_BUN_COUNT = 1;
                        DOTR_BUN_MSG = 0x8B5;
                        return ret;
                    case 8:
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x88A;
                        DOTR_BUN_COUNT = 2;
                        return ret;
                    case 9:
                        DOTR_BUN_MSG = 0x893;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 10:
                        DOTR_BUN_MSG = 0x89D;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 11:
                        if (*(short *)(context + 0xC2) == 0xD4) {
                            DOTR_BUN_COUNT = 1;
                            DOTR_BUN_MSG = 0x8E9;
                            return ret;
                        }
                        if (*(short *)(context + 0xC2) == 0xD5) {
                            DOTR_BUN_COUNT = 2;
                            DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x902;
                            return ret;
                        }
                        break;
                    }
                } else {
                    switch (context[0xC6]) {
                    case 12:
                        DOTR_BUN_MSG = 0x938;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 13:
                        DOTR_BUN_MSG = 0x93C;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 14:
                        DOTR_BUN_MSG = 0x940;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 16:
                        DOTR_BUN_MSG = 0x94B;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 18:
                        DOTR_BUN_MSG = 0x952;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 19:
                        DOTR_BUN_COUNT = 1;
                        DOTR_BUN_MSG = 0x958;
                        return ret;
                    case 20:
                        DOTR_BUN_MSG = 0x95D;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 21:
                        DOTR_BUN_COUNT = 3;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x963;
                        return ret;
                    case 0:
                        DOTR_BUN_COUNT = 2;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x96A;
                        return ret;
                    case 11:
                        DOTR_BUN_COUNT = 2;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x983;
                        return ret;
                    }
                }
            }
    } else if (*(short *)(context + 0xC2) >= 0x12C && *(short *)(context + 0xC2) < 0x190) {
            {
                DOTR_BUN_MODE = 0;
                if ((short)player == 0) {
                    switch (context[0xC6]) {
                    case 2:
                        DOTR_BUN_COUNT = 1;
                        DOTR_BUN_MSG = 0x889;
                        return ret;
                    case 3:
                        DOTR_BUN_COUNT = 4;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8A6;
                        return ret;
                    case 4:
                        DOTR_BUN_MSG = 0x891;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 5:
                        DOTR_BUN_MSG = 0x897;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 6:
                        DOTR_BUN_MSG = 0x89A;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 7:
                        DOTR_BUN_COUNT = 1;
                        DOTR_BUN_MSG = 0x8B6;
                        return ret;
                    case 8:
                        DOTR_BUN_MSG = 0x88D;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 9:
                        DOTR_BUN_MSG = 0x894;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 10:
                        DOTR_BUN_MSG = 0x89E;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 11:
                        if (*(short *)(context + 0xC2) == 0x138) {
                            DOTR_BUN_COUNT = 1;
                            DOTR_BUN_MSG = 0x8EA;
                            return ret;
                        }
                        if (*(short *)(context + 0xC2) == 0x139) {
                            DOTR_BUN_COUNT = 1;
                            DOTR_BUN_MSG = 0x905;
                            return ret;
                        }
                        break;
                    }
                } else {
                    switch (context[0xC6]) {
                    case 12:
                        DOTR_BUN_MSG = 0x939;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 13:
                        DOTR_BUN_MSG = 0x93D;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 14:
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x940;
                        DOTR_BUN_COUNT = 2;
                        return ret;
                    case 16:
                        DOTR_BUN_COUNT = 2;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x94B;
                        return ret;
                    case 18:
                        DOTR_BUN_MSG = 0x953;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 19:
                        DOTR_BUN_COUNT = 3;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x958;
                        return ret;
                    case 20:
                        DOTR_BUN_MSG = 0x95E;
                        DOTR_BUN_COUNT = 1;
                        return ret;
                    case 21:
                        DOTR_BUN_COUNT = 2;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x966;
                        return ret;
                    case 0:
                        DOTR_BUN_COUNT = 3;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x96C;
                        return ret;
                    case 11:
                        DOTR_BUN_COUNT = 3;
                        DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x985;
                        return ret;
                    }
                }
            }
    } else {
            {
                switch (*(short *)(context + 0xC2)) {
                case 0x2724:
                    temp_a0 = DOTR_BUN_PAGE;
                    if (temp_a0 == 0xA) {
                        DOTR_BUN_PAGE = temp_a0 + 1;
                    }
                    DOTR_BUN_COUNT = 0xF;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x7EC;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_FLAG0 = 0;
                    DOTR_BUN_FLAG1 = 0;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2738:
                    DOTR_BUN_COUNT = 8;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x7FB;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC4;
                    DOTR_BUN_SIDE1 = 0;
                    DOTR_BUN_FLAG0 = 0;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2739:
                    DOTR_BUN_COUNT = 0xF;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x803;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC4;
                    DOTR_BUN_SIDE1 = 0;
                    DOTR_BUN_FLAG0 = 0;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2742:
                    DOTR_BUN_COUNT = 8;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x812;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC4;
                    DOTR_BUN_SIDE1 = 0;
                    DOTR_BUN_FLAG0 = 0;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x274C:
                    DOTR_BUN_COUNT = 7;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x81A;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC4;
                    DOTR_BUN_SIDE1 = 0;
                    DOTR_BUN_FLAG0 = 0;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2756:
                    DOTR_BUN_COUNT = 2;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x821;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC4;
                    DOTR_BUN_SIDE1 = 0;
                    DOTR_BUN_FLAG0 = 0;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2757:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x823;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC4;
                    DOTR_BUN_SIDE1 = 0;
                    DOTR_BUN_FLAG0 = 0;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2760:
                    DOTR_BUN_COUNT = 0x3E;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x824;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    switch (DOTR_BUN_MSG) {
                    case 0x825:
                    case 0x826:
                    case 0x828:
                    case 0x82A:
                    case 0x82B:
                    case 0x82C:
                    case 0x82D:
                    case 0x82E:
                    case 0x82F:
                    case 0x830:
                    case 0x832:
                    case 0x833:
                    case 0x834:
                    case 0x835:
                    case 0x83D:
                    case 0x83E:
                    case 0x83F:
                    case 0x840:
                    case 0x841:
                    case 0x842:
                    case 0x844:
                    case 0x845:
                    case 0x846:
                    case 0x847:
                    case 0x84C:
                    case 0x84E:
                    case 0x84F:
                    case 0x850:
                    case 0x851:
                    case 0x853:
                    case 0x854:
                    case 0x857:
                    case 0x858:
                    case 0x859:
                    case 0x85A:
                    case 0x85B:
                    case 0x85C:
                    case 0x85D:
                    case 0x85F:
                    case 0x860:
                    case 0x861:
                        DOTR_BUN_MODE = 3;
                        DOTR_BUN_NAME0 = 0xC5;
                        DOTR_BUN_SIDE0 = 1;
                        if (DOTR_BUN_PAGE >= 4) {
                            DOTR_BUN_FLAG0 = 1;
                            return ret;
                        }
                        break;
                    default:
                        DOTR_BUN_MODE = 2;
                        DOTR_BUN_NAME1 = 0xC4;
                        DOTR_BUN_SIDE1 = 0;
                        DOTR_BUN_FLAG1 = 1;
                        return ret;
                    }
                    break;
                case 0x276A:
                    DOTR_BUN_COUNT = 4;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x862;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    temp_a1 = DOTR_BUN_PAGE;
                    switch (temp_a1) {
                    case 1:
                    case 2:
                    case 4:
                        DOTR_BUN_MODE = 3;
                        DOTR_BUN_NAME0 = 0xC5;
                        DOTR_BUN_SIDE0 = 1;
                        DOTR_BUN_FLAG0 = 1;
                        return ret;
                    default:
                        DOTR_BUN_MODE = 2;
                        DOTR_BUN_NAME1 = 0xC4;
                        DOTR_BUN_SIDE1 = 0;
                        DOTR_BUN_FLAG1 = 1;
                        return ret;
                    }
                    break;
                case 0x2AF8:
                    DOTR_BUN_COUNT = 5;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x866;
                    DOTR_BUN_MODE = 3;
                    DOTR_BUN_NAME0 = 0xC5;
                    DOTR_BUN_SIDE0 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B02:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x86B;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B0C:
                    DOTR_BUN_COUNT = 0x18;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x86C;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    temp_v1_2 = DOTR_BUN_MSG;
                    switch (temp_v1_2) {
                    case 0x870:
                    case 0x872:
                    case 0x873:
                    case 0x874:
                    case 0x875:
                    case 0x876:
                    case 0x877:
                    case 0x878:
                    case 0x879:
                    case 0x87C:
                    case 0x87D:
                    case 0x87E:
                    case 0x87F:
                    case 0x880:
                    case 0x881:
                    case 0x882:
                    case 0x883:
                    case 0x884:
                    case 0x885:
                        DOTR_BUN_MODE = 3;
                        DOTR_BUN_NAME0 = 0xD7;
                        DOTR_BUN_SIDE0 = 0;
                        DOTR_BUN_FLAG0 = 1;
                        return ret;
                    default:
                        DOTR_BUN_MODE = 2;
                        DOTR_BUN_NAME1 = 0xC4;
                        DOTR_BUN_SIDE1 = 0;
                        DOTR_BUN_FLAG1 = 1;
                        return ret;
                    }
                    break;
                case 0x2B70:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8AA;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B72:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8AB;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B74:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8AC;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B84:
                    DOTR_BUN_COUNT = 0x16;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8B6;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    temp_v1_3 = DOTR_BUN_MSG;
                    switch (temp_v1_3) {
                    case 0x8BB:
                    case 0x8BD:
                    case 0x8BF:
                    case 0x8C0:
                    case 0x8C1:
                    case 0x8C3:
                    case 0x8C5:
                    case 0x8C6:
                    case 0x8C7:
                    case 0x8C8:
                    case 0x8C9:
                    case 0x8CB:
                    case 0x8CC:
                        DOTR_BUN_MODE = 3;
                        DOTR_BUN_NAME0 = 0xCD;
                        DOTR_BUN_SIDE0 = 1;
                        if (DOTR_BUN_PAGE >= 6) {
                            DOTR_BUN_FLAG0 = 1;
                            return ret;
                        }
                        break;
                    default:
                        DOTR_BUN_MODE = 2;
                        DOTR_BUN_NAME1 = 0xCE;
                        DOTR_BUN_SIDE1 = 1;
                        DOTR_BUN_FLAG1 = 1;
                        return ret;
                    }
                    break;
                case 0x2B86:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8CC;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B87:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8CD;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B88:
                    DOTR_BUN_COUNT = 3;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8CE;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B8A:
                    DOTR_BUN_COUNT = 3;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8D1;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B8B:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8D4;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B8C:
                    DOTR_BUN_COUNT = 9;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8D5;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    temp_a0_2 = DOTR_BUN_MSG;
                    switch (temp_a0_2) {
                    case 0x8D9:
                    case 0x8DB:
                        DOTR_BUN_MODE = 3;
                        DOTR_BUN_NAME0 = 0xCD;
                        DOTR_BUN_SIDE0 = 1;
                        DOTR_BUN_FLAG0 = 1;
                        break;
                    default:
                        DOTR_BUN_MODE = 2;
                        DOTR_BUN_NAME1 = 0xD7;
                        DOTR_BUN_SIDE1 = 0;
                        DOTR_BUN_FLAG1 = 1;
                        break;
                    }
                    if (DOTR_BUN_PAGE >= 0xA) {
                        DOTR_BUN_FLAG0 = 0;
                        DOTR_BUN_FLAG1 = 0;
                        return ret;
                    }
                    break;
                case 0x2B8D:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = 0x8DF;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_FLAG0 = 0;
                    DOTR_BUN_FLAG1 = 0;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2B98:
                    DOTR_BUN_COUNT = 0x11;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8EA;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC5;
                    DOTR_BUN_SIDE1 = 1;
                    DOTR_BUN_FLAG1 = 1;
                    temp_a0_3 = DOTR_BUN_PAGE;
                    switch (temp_a0_3) {
                    case 1:
                        DOTR_BUN_LOOK0 = 0;
                        DOTR_BUN_LOOK1 = 2;
                        return ret;
                    case 2:
                        DOTR_BUN_LOOK1 = 2;
                        return ret;
                    case 3:
                        DOTR_BUN_LOOK1 = 1;
                        return ret;
                    }
                    break;
                case 0x2B9A:
                    DOTR_BUN_COUNT = 6;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x8FB;
                    temp_a0_4 = DOTR_BUN_MSG;
                    switch (temp_a0_4) {
                    case 0x8FC:
                    case 0x900:
                    case 0x901:
                        DOTR_BUN_MODE = 3;
                        DOTR_BUN_NAME0 = 0xD8;
                        DOTR_BUN_SIDE0 = 1;
                        DOTR_BUN_FLAG0 = 1;
                        break;
                    default:
                        DOTR_BUN_MODE = 2;
                        DOTR_BUN_NAME1 = 0xC5;
                        DOTR_BUN_SIDE1 = 1;
                        DOTR_BUN_FLAG1 = 1;
                        break;
                    }
                    temp_a0_5 = DOTR_BUN_PAGE;
                    switch ((int)temp_a0_5) { /* (int): see the top comment */
                    case 0x8FC:
                        DOTR_BUN_LOOK0 = 0;
                        DOTR_BUN_LOOK1 = 0;
                        return ret;
                    case 0x8FD:
                    case 0x8FE:
                    case 0x8FF:
                        DOTR_BUN_LOOK1 = 1;
                        return ret;
                    }
                    break;
                case 0x2BAC:
                    DOTR_BUN_COUNT = 3;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x905;
                    switch (DOTR_BUN_PAGE) {
                    case 1:
                    case 3:
                        DOTR_BUN_MODE = 3;
                        DOTR_BUN_NAME0 = 0xD8;
                        DOTR_BUN_SIDE0 = 1;
                        DOTR_BUN_FLAG0 = 1;
                        break;
                    default:
                        DOTR_BUN_MODE = 2;
                        DOTR_BUN_NAME1 = 0xC5;
                        DOTR_BUN_SIDE1 = 1;
                        DOTR_BUN_FLAG1 = 1;
                        break;
                    }
                    if (DOTR_BUN_PAGE != 1) {
                        return ret;
                    }
                    DOTR_BUN_LOOK0 = 2;
                    DOTR_BUN_LOOK1 = 2;
                    return ret;
                case 0x2BAE:
                    DOTR_BUN_COUNT = 4;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x908;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2BB0:
                    DOTR_BUN_COUNT = 0x11;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x90C;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2EE0:
                    DOTR_BUN_COUNT = 0x17;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x91D;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    switch (DOTR_BUN_PAGE) {
                    case 4:
                        DOTR_BUN_MODE = 2;
                        DOTR_BUN_NAME1 = 0xC4;
                        DOTR_BUN_SIDE1 = 0;
                        DOTR_BUN_FLAG0 = 1;
                        DOTR_BUN_LOOK1 = 2;
                        break;
                    default:
                        DOTR_BUN_MODE = 3;
                        DOTR_BUN_NAME0 = 0xC5;
                        DOTR_BUN_SIDE0 = 1;
                        DOTR_BUN_FLAG1 = 1;
                        if (DOTR_BUN_PAGE >= 4) {
                            DOTR_BUN_LOOK1 = 2;
                        }
                        break;
                    }
                    if (DOTR_BUN_PAGE >= 5) {
                        DOTR_BUN_FLAG1 = 0;
                        return ret;
                    }
                    break;
                case 0x2F08:
                    DOTR_BUN_COUNT = 1;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x942;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xD2;
                    DOTR_BUN_SIDE1 = 0;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2F09:
                    DOTR_BUN_COUNT = 6;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x943;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xD2;
                    DOTR_BUN_SIDE1 = 0;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2F62:
                    DOTR_BUN_COUNT = 3;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x970;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC5;
                    DOTR_BUN_SIDE1 = 1;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2F64:
                    DOTR_BUN_COUNT = 0xF;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x973;
                    temp_v1_5 = DOTR_BUN_MSG;
                    switch (temp_v1_5) {
                    case 0x974:
                    case 0x975:
                    case 0x976:
                    case 0x977:
                    case 0x978:
                    case 0x979:
                    case 0x97B:
                    case 0x97C:
                    case 0x97D:
                    case 0x982:
                        DOTR_BUN_MODE = 3;
                        DOTR_BUN_NAME0 = 0xD8;
                        DOTR_BUN_SIDE0 = 0;
                        DOTR_BUN_FLAG0 = 1;
                        break;
                    default:
                        DOTR_BUN_MODE = 2;
                        DOTR_BUN_NAME1 = 0xC5;
                        DOTR_BUN_SIDE1 = 1;
                        DOTR_BUN_FLAG1 = 1;
                        break;
                    }
                    temp_a0_6 = DOTR_BUN_MSG;
                    switch (temp_a0_6) {
                    case 0x974:
                        DOTR_BUN_LOOK0 = 0;
                        DOTR_BUN_LOOK1 = 0;
                        return ret;
                    case 0x97A:
                        DOTR_BUN_LOOK0 = 0;
                        DOTR_BUN_LOOK1 = 2;
                        return ret;
                    }
                    break;
                case 0x2F76:
                    DOTR_BUN_COUNT = 2;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x988;
                    DOTR_BUN_MODE = 3;
                    DOTR_BUN_NAME0 = 0xD8;
                    DOTR_BUN_SIDE0 = 0;
                    DOTR_BUN_FLAG0 = 1;
                    if (DOTR_BUN_PAGE != 1) {
                        return ret;
                    }
                    DOTR_BUN_LOOK0 = 2;
                    DOTR_BUN_LOOK1 = 2;
                    return ret;
                case 0x2F78:
                    DOTR_BUN_COUNT = 0x11;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x98A;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC5;
                    DOTR_BUN_SIDE1 = 1;
                    DOTR_BUN_FLAG0 = 0;
                    DOTR_BUN_FLAG1 = 1;
                    if (DOTR_BUN_PAGE != 1) {
                        return ret;
                    }
                    DOTR_BUN_LOOK0 = 2;
                    DOTR_BUN_LOOK1 = 2;
                    return ret;
                case 0x2F7A:
                    DOTR_BUN_COUNT = 6;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x99B;
                    DOTR_BUN_MODE = 2;
                    DOTR_BUN_NAME1 = 0xC5;
                    DOTR_BUN_SIDE1 = 1;
                    DOTR_BUN_FLAG1 = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                case 0x2F7C:
                    DOTR_BUN_COUNT = 0xE;
                    DOTR_BUN_MSG = DOTR_BUN_PAGE + 0x9A1;
                    DOTR_BUN_MODE = 1;
                    DOTR_BUN_LOOK0 = 0;
                    DOTR_BUN_LOOK1 = 0;
                    return ret;
                }
            }
    }
    return ret;
}

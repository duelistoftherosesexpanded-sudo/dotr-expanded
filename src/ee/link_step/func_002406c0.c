#include "dotr/ee/link_step.h"
int TakuTrade_Save_card_kansi(void)
{
    struct DotrMiscLink *link = &data_0034fb00;
    data_0034fb00.unknown_00 = func_00194380(data_0034fb00.unknown_08, data_0034fb00.unknown_0c,
                                             &data_0034fb00.unknown_10, &data_0034fb00.unknown_14,
                                             &data_0034fb00.unknown_18);
    data_0034fb00.unknown_04 = func_00194248(1, &data_0034fb00.unknown_20, &data_0034fb00.unknown_24);
    if (data_0034fb00.unknown_04 == 1 && link->unknown_20 == 1) {
        switch (link->unknown_24) {
        case 0:
            break;
        case -1:
            link->unknown_30 = 14;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 0x700;
        case -2:
            link->unknown_30 = 14;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 0x700;
        default:
            link->unknown_30 = 14;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            break;
        }
        switch (link->unknown_10) {
        case 0:
            link->unknown_30 = 14;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 2;
        case 1:
            link->unknown_30 = 14;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 4;
        case 2:
            break;
        case 3:
            link->unknown_30 = 14;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 4;
        default:
            link->unknown_30 = 14;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 2;
        }
        if (link->unknown_10 == 2) {
            switch (link->unknown_18) {
            case 0:
                link->unknown_40++;
                if (link->unknown_40 >= 3) {
                    link->unknown_30 = 11;
                    link->unknown_34 = 0;
                    link->unknown_40 = 3;
                    return link->unknown_30 == 11 ? 14 : 3;
                }
                break;
            case 1:
                link->unknown_40 = 0;
                if (link->unknown_30 == 0) {
                    if (link->unknown_08 == 0) {
                        (&link->unknown_44)[link->unknown_08] = 1;
                        link->unknown_08 = 1;
                        return 0;
                    }
                    link->unknown_44 = 0;
                    link->unknown_46 = 0;
                    link->unknown_08 = 0;
                    link->unknown_30 = 5;
                    return 9;
                } else if (link->unknown_30 == 3) {
                    if (link->unknown_14 < 250) {
                        link->unknown_30 = 14;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 5;
                    }
                    link->unknown_30 = 4;
                    return 9;
                }
                break;
            default:
                link->unknown_30 = 14;
                link->unknown_34 = 0;
                link->unknown_40 = 0;
                return 2;
            }
        }
    }
    switch (link->unknown_30) {
    case 0:
        return 9;
    case 3:
        return 17;
    case 11:
        return 14;
    case 13:
        return 1;
    default:
        return 0;
    }
}

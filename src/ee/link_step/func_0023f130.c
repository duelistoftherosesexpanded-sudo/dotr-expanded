#include "dotr/ee/link_step.h"
int TakuLoad_card_kansi(int mode)
{
    struct DotrMiscLink *link = &data_0034fb00;
    (void)mode;
    data_0034fb00.unknown_00 = func_00194380(data_0034fb00.unknown_08, data_0034fb00.unknown_0c,
                                             &data_0034fb00.unknown_10, &data_0034fb00.unknown_14,
                                             &data_0034fb00.unknown_18);
    data_0034fb00.unknown_04 = func_00194248(1, &data_0034fb00.unknown_20, &data_0034fb00.unknown_24);
    if (data_0034fb00.unknown_04 == 1 && link->unknown_20 == 1) {
        switch (link->unknown_24) {
        case 0:
            break;
        case -1:
            link->unknown_30 = 3;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 0x700;
        case -2:
            link->unknown_30 = 3;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 0x700;
        default:
            link->unknown_30 = 3;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            break;
        }
        switch (link->unknown_10) {
        case 0:
            link->unknown_30 = 3;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 2;
        case 1:
            link->unknown_30 = 3;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 4;
        case 2:
            break;
        case 3:
            link->unknown_30 = 3;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 4;
        default:
            link->unknown_30 = 3;
            link->unknown_34 = 0;
            link->unknown_40 = 0;
            return 2;
        }
        if (link->unknown_10 == 2) {
            switch (link->unknown_18) {
            case 0:
                link->unknown_40++;
                if (link->unknown_40 >= 3) {
                    link->unknown_30 = 3;
                    link->unknown_34 = 0;
                    link->unknown_40 = 3;
                    return 3;
                }
                break;
            case 1:
                if (link->unknown_30 == 0) {
                    link->unknown_30 = 1;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 16;
                }
                break;
            default:
                link->unknown_30 = 3;
                link->unknown_34 = 0;
                link->unknown_40 = 0;
                return 2;
            }
        }
    }
    return 0;
}

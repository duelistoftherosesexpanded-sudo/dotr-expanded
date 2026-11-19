#include "dotr/ee/link_step.h"
int TakuSave_card_filename_check(int mode)
{
    struct DotrMiscLink *link = &data_0034fb00;
    switch (data_0034fb00.unknown_34) {
    case 0:
        link->unknown_00 = func_00194758(link->unknown_08, link->unknown_0c, data_0029b100, link->unknown_2c0);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 12) {
                switch (link->unknown_24) {
                case 0:
                    link->unknown_34 = 1;
                    link->unknown_40 = 0;
                    return 0;
                case -2:
                    link->unknown_40++;
                    if (link->unknown_40 >= 3) {
                        link->unknown_34 = 0;
                        link->unknown_40 = 3;
                        link->unknown_30 = 11;
                        return 3;
                    }
                    return 0;
                case -4:
                    link->unknown_30 = 3;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 17;
                default:
                    link->unknown_34 = 0;
                    link->unknown_30 = 14;
                    link->unknown_40 = 0;
                    return 2;
                }
            }
        }
        return 0;
    case 1:
        link->unknown_00 = func_00194578(link->unknown_08, link->unknown_0c, data_0029b1be, link->unknown_60, 1, link->unknown_80);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 13) {
                if (link->unknown_24 < 0) {
                    switch (link->unknown_24) {
                    case -2:
                        link->unknown_40++;
                        if (link->unknown_40 >= 3) {
                            link->unknown_34 = 0;
                            link->unknown_40 = 3;
                            link->unknown_30 = 11;
                            return 3;
                        }
                        return 0;
                    case -4:
                        link->unknown_30 = 14;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 10;
                    default:
                        link->unknown_34 = 0;
                        link->unknown_30 = 14;
                        link->unknown_40 = 0;
                        return 2;
                    }
                } else {
                    if (mode == 1) {
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        link->unknown_30 = 0;
                        return 0x100;
                    }
                    link->unknown_40 = 0;
                    link->unknown_34 = 0;
                    link->unknown_30 = 2;
                    return 16;
                }
            }
        }
        return 0;
    }
    return 0;
}

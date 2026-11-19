#include "dotr/ee/link_step.h"
int TakuSave_make_file(void)
{
    struct DotrMiscLink *link = &data_0034fb00;
    int result;
    switch (data_0034fb00.unknown_34) {
    case 0:
        link->unknown_00 = func_00193ce0(link->unknown_08, link->unknown_0c, data_0029b100);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 11) {
                if (link->unknown_24 != 0) {
                    switch (link->unknown_24) {
                    case -2:
                        link->unknown_40++;
                        if (link->unknown_40 >= 3) {
                            link->unknown_30 = 11;
                            link->unknown_34 = 0;
                            link->unknown_40 = 3;
                            return 3;
                        }
                        return 0;
                    case -3:
                        link->unknown_30 = 14;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 5;
                    case -4:
                        link->unknown_30 = 14;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 10;
                    default:
                        link->unknown_30 = 14;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 2;
                    }
                } else {
                    link->unknown_34 = 1;
                    link->unknown_40 = 0;
                    return 0;
                }
            }
        }
        return 0;
    case 1:
        link->unknown_00 = func_00194758(link->unknown_08, link->unknown_0c, data_0029b100, link->unknown_2c0);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 12) {
                switch (link->unknown_24) {
                case 0:
                    link->unknown_34 = 2;
                    link->unknown_40 = 0;
                    return 0;
                case -2:
                    link->unknown_40++;
                    if (link->unknown_40 >= 3) {
                        link->unknown_30 = 11;
                        link->unknown_34 = 0;
                        link->unknown_40 = 3;
                        return 3;
                    }
                    return 0;
                case -4:
                    link->unknown_30 = 14;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 10;
                default:
                    link->unknown_30 = 14;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 2;
                }
                break;
            }
        }
        return 0;
    case 2:
        result = card_icon_tukuru();
        if (result == 1) {
            link->unknown_40 = 0;
            link->unknown_34 = 3;
        } else if (result == 10) {
            return 10;
        }
        break;
    case 3:
        link->unknown_00 = func_00193ba8_mode(link->unknown_08, link->unknown_0c, data_0029b1be, 512);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 2) {
                if (link->unknown_24 < 0) {
                    switch (link->unknown_24) {
                case -2:
                    link->unknown_40++;
                    if (link->unknown_40 >= 3) {
                        link->unknown_30 = 11;
                        link->unknown_34 = 0;
                        link->unknown_40 = 3;
                        return 3;
                    }
                    return 0;
                case -3:
                    link->unknown_30 = 14;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 5;
                case -4:
                    link->unknown_30 = 14;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 10;
                case -5:
                    link->unknown_30 = 14;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 10;
                case -7:
                    link->unknown_30 = 14;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 10;
                    default:
                        link->unknown_30 = 14;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 2;
                    }
                } else {
                    (&link->unknown_48)[link->unknown_08] = link->unknown_24;
                    link->unknown_34 = 4;
                    link->unknown_40 = 0;
                    return 0;
                }
            }
        }
        return 0;
    case 4:
        link->unknown_00 = func_00193d18((&link->unknown_48)[link->unknown_08]);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 3) {
                switch (link->unknown_24) {
                case 0:
                    link->unknown_30 = 5;
                    link->unknown_40 = 0;
                    link->unknown_34 = 0;
                    return 9;
                case -2:
                    link->unknown_40++;
                    if (link->unknown_40 >= 3) {
                        link->unknown_30 = 11;
                        link->unknown_34 = 0;
                        link->unknown_40 = 3;
                        return 3;
                    }
                    return 0;
                case -4:
                    link->unknown_30 = 14;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 10;
                default:
                    link->unknown_30 = 14;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 2;
                }
                break;
            }
        }
        return 0;
    }
    return 0;
}

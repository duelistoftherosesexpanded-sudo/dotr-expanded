#include "dotr/ee/link_step.h"
int TakuSave_card_file_open(void)
{
    struct DotrMiscLink *link = &data_0034fb00;
    switch (data_0034fb00.unknown_34) {
    case 0:
        link->unknown_00 = func_00193ba8(link->unknown_08, link->unknown_0c, data_0029b1be);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 2) {
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
                        link->unknown_34 = 0;
                        link->unknown_30 = 14;
                        link->unknown_40 = 0;
                        return 2;
                    }
                } else {
                    (&link->unknown_48)[link->unknown_08] = link->unknown_24;
                    link->unknown_40 = 0;
                    link->unknown_34 = 1;
                    return 0;
                }
            }
        }
        return 0;
    case 1:
        link->unknown_00 = func_00193dd0((&link->unknown_48)[link->unknown_08], link->unknown_2b6, link->unknown_2b8, 1);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 4) {
                switch (link->unknown_24) {
                case 0:
                    link->unknown_40 = 0;
                    link->unknown_34 = 2;
                    return 0;
                case -2:
                    link->unknown_40++;
                    if (link->unknown_40 >= 3) {
                        link->unknown_34 = 0;
                        link->unknown_40 = 3;
                        link->unknown_30 = 11;
                        return 3;
                    }
                    break;
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
                break;
            }
        }
        return 0;
    case 2:
        func_0017bb60(0);
        link->unknown_00 = func_00193f38((&link->unknown_48)[link->unknown_08], &DOTR_LINK_IMAGES[link->unknown_08], 0x100f0);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 5) {
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
                    link->unknown_40 = 0;
                    link->unknown_34 = 3;
                    return 0;
                }
            }
        }
        return 0;
    case 3:
        link->unknown_00 = func_00193d18((&link->unknown_48)[link->unknown_08]);
        link->unknown_04 = func_00194248(1, &link->unknown_20, &link->unknown_24);
        if (link->unknown_04 == 1) {
            if (link->unknown_20 == 3) {
                switch (link->unknown_24) {
                case 0:
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    link->unknown_30 = 13;
                    return 1;
                case -2:
                    link->unknown_40++;
                    if (link->unknown_40 >= 3) {
                        link->unknown_30 = 11;
                        link->unknown_34 = 0;
                        link->unknown_40 = 3;
                        return 3;
                    }
                    break;
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

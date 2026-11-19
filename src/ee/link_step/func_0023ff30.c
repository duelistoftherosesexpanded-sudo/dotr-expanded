#include "dotr/ee/link_step.h"
int TakuCustom_card_file_open(void)
{
    struct DotrMiscLink *link = &data_0034fb00;
    struct DotrLinkTail *tail = &data_00299e10_link;
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
                            link->unknown_30 = 3;
                            link->unknown_34 = 0;
                            link->unknown_40 = 3;
                            return 3;
                        }
                        return 0;
                    case -3:
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 5;
                    case -4:
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 10;
                    case -5:
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 10;
                    case -7:
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 10;
                    default:
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 2;
                    }
                } else {
                    (&link->unknown_48)[link->unknown_08] = link->unknown_24;
                    link->unknown_34 = 1;
                    link->unknown_40 = 0;
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
                    link->unknown_34 = 2;
                    link->unknown_40 = 0;
                    return 0;
                case -2:
                    link->unknown_40++;
                    if (link->unknown_40 >= 3) {
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 3;
                        return 3;
                    }
                    return 0;
                case -4:
                    link->unknown_30 = 3;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 10;
                default:
                    link->unknown_30 = 3;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 2;
                }
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
                            link->unknown_30 = 3;
                            link->unknown_34 = 0;
                            link->unknown_40 = 3;
                            return 3;
                        }
                        return 0;
                    case -4:
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 10;
                    default:
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 2;
                    }
                } else {
                    link->unknown_34 = 3;
                    link->unknown_40 = 0;
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
                    link->unknown_30 = 0;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    if (link->unknown_08 == 0) {
                        *p_kaban_data = DOTR_LINK_IMAGES[link->unknown_08].body;
                        Taku_card_info.head = DOTR_LINK_IMAGES[link->unknown_08].head;
                        *tail = DOTR_LINK_IMAGES[link->unknown_08].tail;
                        if (LkCheckData((unsigned short *)&DOTR_LINK_IMAGES[link->unknown_08])) {
                            TakuLoad_Name_Set(link->unknown_08);
                            (&link->unknown_44)[link->unknown_08] = 1;
                            link->unknown_08 = 1;
                            return 0x10;
                        }
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 0;
                        return 10;
                    }
                    *data_0034faec = DOTR_LINK_IMAGES[link->unknown_08].body;
                    Taku_card_info.head = DOTR_LINK_IMAGES[link->unknown_08].head;
                    *tail = DOTR_LINK_IMAGES[link->unknown_08].tail;
                    if (LkCheckData((unsigned short *)&DOTR_LINK_IMAGES[link->unknown_08])) {
                        TakuLoad_Name_Set(link->unknown_08);
                        (&link->unknown_44)[link->unknown_08] = 1;
                        link->unknown_08 = 1;
                        return 0x100;
                    }
                    link->unknown_30 = 3;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 10;
                case -2:
                    link->unknown_40++;
                    if (link->unknown_40 >= 3) {
                        link->unknown_30 = 3;
                        link->unknown_34 = 0;
                        link->unknown_40 = 3;
                        return 3;
                    }
                    return 0;
                case -4:
                    link->unknown_30 = 3;
                    link->unknown_34 = 0;
                    link->unknown_40 = 0;
                    return 10;
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
    return 0;
}

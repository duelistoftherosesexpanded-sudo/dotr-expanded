#include "dotr/ee/mode_steps.h"

int TkMc_Save_Main(int mode)
{
    struct DotrMiscLink *link = &data_0034fb00;
    int result = 0;

    if (mode == 1) {
        TakuCard_inti(0);
        link->unknown_2be = 0;
    }
    switch (link->unknown_30) {
    case 0:
        result = TakuSave_card_kansi(0);
        if (result == 2) {
            link->unknown_30 = 0;
        }
        break;
    case 1:
        result = TakuSave_card_filename_check(0);
        break;
    case 2:
        result = TakuSave_card_file_open();
        break;
    case 3:
        result = TakuSave_card_kansi(0);
        break;
    case 4:
        result = TakuSave_make_file();
        break;
    case 5:
        result = TakuSave_card_file_kakikomi();
        break;
    case 10:
        result = TakuSave_card_format();
        break;
    case 15:
        result = TakuSave_card_unformat();
        break;
    case 11:
        result = TakuSave_card_kansi(0);
        if (link->unknown_2be == 14 && mode == 2) {
            link->unknown_30 = 10;
            link->unknown_34 = 0;
            result = 11;
            link->unknown_40 = 0;
        }
        break;
    case 13:
        result = TakuSave_card_kansi(0);
        if (link->unknown_2be == 1 && mode == 2) {
            link->unknown_30 = 5;
            link->unknown_34 = 0;
            result = 9;
            link->unknown_40 = 0;
        }
        break;
    case 14:
        result = TakuSave_card_kansi(0);
        if (result == 2) {
            result = 10;
        }
        if (mode == 2) {
            link->unknown_30 = 0;
            link->unknown_34 = 0;
            result = 0x700;
            link->unknown_40 = 0;
        }
        break;
    }
    if (mode == 1) {
        link->unknown_34 = 0;
        link->unknown_40 = 0;
        link->unknown_30 = 0;
    }
    if (result != 0) {
        link->unknown_2be = result;
    }
    if (result == 256) {
        return 256;
    }
    return link->unknown_2be;
}

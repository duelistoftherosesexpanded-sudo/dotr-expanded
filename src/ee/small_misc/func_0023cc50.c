#include "dotr/ee/small_misc.h"
int TkMc_Custom_Main(int mode)
{
    struct DotrMiscLink *link = &data_0034fb00;
    int result = 0;
    if (mode == 1) {
        TakuCard_inti(0);
        link->unknown_2be = 0;
    }
    switch (link->unknown_30) {
    case 0:
        result = TakuCustom_card_kansi(mode);
        if (result == 2) {
            link->unknown_30 = 0;
        }
        break;
    case 1:
        result = TakuLoad_card_filename_check();
        break;
    case 2:
        result = TakuCustom_card_file_open();
        break;
    case 3:
        result = TakuCustom_card_kansi(mode);
        if (result == 2) {
            result = 10;
        }
        break;
    }
    if (mode == 1) {
        link->unknown_34 = 0;
        link->unknown_40 = 0;
        link->unknown_30 = 0;
        result = 16;
        link->unknown_0c = 0;
    }
    if (result != 0) {
        if (link->unknown_08 == 1) {
            switch (result) {
            case 2:
                result = 0x200;
                break;
            case 3:
            case 5:
            case 6:
            case 8:
            case 10:
            case 7:
                result = 0x300;
                break;
            }
        } else if (result == 3) {
            result = 7;
        }
        link->unknown_2be = result;
    }
    if (result == 256) {
        return 256;
    }
    if (link->unknown_08 == 1) {
        link->unknown_2be |= 0x80;
    }
    return link->unknown_2be;
}

#include "dotr/ee/small_misc.h"

int TkMc_Check_Main(int mode)
{
    struct DotrMiscLink *link = &data_0034fb00;
    int result = 0;

    if (mode == 1) {
        TakuCard_inti(0);
        link->unknown_2be = 0;
    }
    switch (link->unknown_30) {
    case 0:
    case 14:
    case 11:
        result = TakuSave_card_kansi(1);
        break;
    case 1:
        result = TakuSave_card_filename_check(1);
        break;
    case 3:
        result = TakuSave_card_kansi(1);
        break;
    }
    if (mode == 1) {
        link->unknown_34 = 0;
        link->unknown_40 = 0;
        link->unknown_30 = 0;
    }
    switch (result) {
    case 2:
    case 3:
    case 14:
    case 6:
    case 8:
        result = 256;
        /* fall through */
    case 10:
        result = 256;
        break;
    case 0:
        break;
    }
    if (result != 0) {
        link->unknown_2be = result;
    }
    if (result == 256) {
        return 256;
    }
    return link->unknown_2be;
}

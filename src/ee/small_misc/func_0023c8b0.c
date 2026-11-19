#include "dotr/ee/small_misc.h"

int TkMc_Trade_Load_Main(int mode)
{
    struct DotrMiscLink *link = &data_0034fb00;
    int result = 0;

    if (mode == 1) {
        TakuCard_inti(0);
        link->unknown_2be = 0;
    }
    switch (link->unknown_30) {
    case 0:
        result = TakuLoad_card_kansi(mode);
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
        result = TakuLoad_card_kansi(mode);
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
    if (result == 3) {
        result = 7;
    }
    if (result != 0) {
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

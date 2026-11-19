#include "dotr/ee/card_save.h"

/* 0x23e7a0. Its switch table is placed at its original address, 0x30f2e0. The data, declared
 * inside the function body, are views with the _cardsave suffix; the icon data is addressed through
 * DOTR_CARD_SAVE_AT. */
#ifndef __MWERKS__
/* Host build only: the code passes char buffers to signed and unsigned char parameters. */
#pragma GCC diagnostic ignored "-Wpointer-sign"
#endif

int card_icon_tukuru(void) {
    extern struct DotrCardSaveStatus data_0034fb00_cardsave;
    extern short data_0034fb36_cardsave;
    extern unsigned char data_0034f720_cardsave[];
    extern char data_0034f7e0_cardsave[];
    extern unsigned short data_0034f726_cardsave;
    extern unsigned int data_0034f72c_cardsave;
    extern unsigned char data_0034f730_cardsave[];
    extern unsigned char data_0034f770_cardsave[];
    extern unsigned char data_0034f7a0_cardsave[];
    extern unsigned char data_0034f7d0_cardsave[];
    extern char data_0034f824_cardsave[];
    extern char data_0034f864_cardsave[];
    extern char data_0034f8a4_cardsave[];
    extern char *data_0029b06c_cardsave;
    extern char *data_0029b070_cardsave;
    extern unsigned char data_0029b100_cardsave[];
    extern char data_0030f321_cardsave[];
    extern char data_0030f32b_cardsave[];
    extern char data_0030f330_cardsave[];
    extern unsigned char bgColour_cardsave[];
    extern unsigned char lightdir_cardsave[];
    extern unsigned char lightcol_cardsave[];
    extern unsigned char ambient_cardsave[];
    extern char *iconName_cardsave[];
    extern unsigned int anOfs_ICON_cardsave[];
    extern unsigned int data_0029b054_cardsave[];
    DotrCardSaveIconState *s;
    char path[0x40];
    char *end;

    s = (DotrCardSaveIconState *)&data_0034fb00_cardsave;
    func_0019bdd8(path, data_0029b100_cardsave);
    end = path + func_0019bef0(path);
    func_0019bdd8(end, data_0030f321_cardsave);
    switch (data_0034fb36_cardsave) {
    case 0:
        s->ret = func_00193ba8(s->port, s->slot, path, 0x203);
        s->sync = func_00194248(0, &s->command, &s->result);
        if (s->sync == 1) {
            if (s->command == 2) {
                if (0 <= s->result) {
                    s->handles[s->port] = s->result;
                    s->retries = 0;
                    s->sub = 1;
                } else {
                    switch (s->result) {
                    case -2:
                        s->retries++;
                        if (s->retries >= 3) {
                            s->phase = 0;
                            s->sub = 0;
                            s->retries = 3;
                            s->state = 0xB;
                        }
                        break;
                    case -3:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -4:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -5:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -7:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    default:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    }
                }
            }
        }
        break;
    case 1:
        func_00198948(data_0034f720_cardsave, 0, 0x3C4);
        func_0019bdd8((char *)data_0034f720_cardsave, data_0030f32b_cardsave);
        func_0019bdd8(data_0034f7e0_cardsave, data_0030f330_cardsave);
        data_0034f726_cardsave = 0x1E;
        data_0034f72c_cardsave = 0x60;
        func_00198790(data_0034f730_cardsave, bgColour_cardsave, 0x40);
        func_00198790(data_0034f770_cardsave, lightdir_cardsave, 0x30);
        func_00198790(data_0034f7a0_cardsave, lightcol_cardsave, 0x30);
        func_00198790(data_0034f7d0_cardsave, ambient_cardsave, 0x10);
        func_0019bdd8(data_0034f824_cardsave, iconName_cardsave[0]);
        func_0019bdd8(data_0034f864_cardsave, data_0029b06c_cardsave);
        func_0019bdd8(data_0034f8a4_cardsave, data_0029b070_cardsave);
        s->ret = func_00194058(s->handles[s->port], data_0034f720_cardsave, 0x3C4);
        s->sync = func_00194248(0, &s->command, &s->result);
        if (s->sync == 1) {
            if (s->command == 6) {
                if (0 <= s->result) {
                    s->retries = 0;
                    s->sub = 2;
                } else {
                    switch (s->result) {
                    case -2:
                        s->retries++;
                        if (s->retries >= 3) {
                            s->phase = 0;
                            s->sub = 0;
                            s->retries = 3;
                            s->state = 0xB;
                        }
                        break;
                    case -3:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -4:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -5:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    default:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    }
                }
            }
        }
        break;
    case 2:
        s->ret = func_00193d18(s->handles[s->port]);
        s->sync = func_00194248(0, &s->command, &s->result);
        if (s->sync == 1) {
            if (s->command == 3) {
                switch (s->result) {
                case 0:
                    s->sub = 3;
                    s->retries = 0;
                    break;
                case -2:
                    s->retries++;
                    if (s->retries >= 3) {
                        s->phase = 0;
                        s->sub = 0;
                        s->retries = 3;
                        s->state = 0xB;
                    }
                    break;
                case -4:
                    s->phase = 0;
                    s->sub = 0;
                    s->state = 0xE;
                    s->retries = 0;
                    return 0xA;
                default:
                    s->phase = 0;
                    s->sub = 0;
                    s->state = 0xE;
                    s->retries = 0;
                    return 0xA;
                }
            }
        }
        break;
    case 3:
        *end = '/';
        func_0019bdd8(end + 1, iconName_cardsave[s->index]);
        s->ret = func_00193ba8(s->port, s->slot, path, 0x203);
        s->sync = func_00194248(1, &s->command, &s->result);
        if (s->sync == 1) {
            if (s->command == 2) {
                if (0 <= s->result) {
                    s->handles[s->port] = s->result;
                    s->retries = 0;
                    s->sub = 4;
                } else {
                    switch (s->result) {
                    case -2:
                        s->retries++;
                        if (s->retries >= 3) {
                            s->phase = 0;
                            s->sub = 0;
                            s->retries = 3;
                            s->state = 0xB;
                        }
                        break;
                    case -3:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -4:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -5:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -7:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    default:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    }
                }
            }
        }
        break;
    case 4:
        {
            int size = data_0029b054_cardsave[s->index] - anOfs_ICON_cardsave[s->index];
            s->ret = func_00194058(s->handles[s->port], (unsigned char *)DOTR_CARD_SAVE_AT(0x1E301E0) + anOfs_ICON_cardsave[s->index], size);
        }
        s->sync = func_00194248(1, &s->command, &s->result);
        if (s->sync == 1) {
            if (s->command == 6) {
                if (0 <= s->result) {
                    s->retries = 0;
                    s->sub = 5;
                } else {
                    switch (s->result) {
                    case -2:
                        s->retries++;
                        if (s->retries >= 3) {
                            s->phase = 0;
                            s->sub = 0;
                            s->retries = 3;
                            s->state = 0xB;
                        }
                        break;
                    case -3:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -4:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    case -5:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    default:
                        s->phase = 0;
                        s->sub = 0;
                        s->state = 0xE;
                        s->retries = 0;
                        return 0xA;
                    }
                }
            }
        }
        break;
    case 5:
        s->ret = func_00193d18(s->handles[s->port]);
        s->sync = func_00194248(1, &s->command, &s->result);
        if (s->sync == 1) {
            if (s->command == 3) {
                switch (s->result) {
                case 0:
                    s->index++;
                    if (s->index >= 3) {
                        s->sub = 0;
                        s->retries = 0;
                        return 1;
                    }
                    s->sub = 3;
                    s->retries = 0;
                    break;
                case -2:
                    s->retries++;
                    if (s->retries >= 3) {
                        s->state = 0xB;
                        s->phase = 0;
                        s->sub = 0;
                        s->retries = 3;
                    }
                    break;
                case -4:
                    s->phase = 0;
                    s->sub = 0;
                    s->state = 0xE;
                    s->retries = 0;
                    return 0xA;
                default:
                    s->phase = 0;
                    s->sub = 0;
                    s->state = 0xE;
                    s->retries = 0;
                    return 0xA;
                }
            }
        }
        break;
    }
    return 0;
}

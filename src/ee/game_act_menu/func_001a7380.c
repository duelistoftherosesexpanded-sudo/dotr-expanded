#include "dotr/ee/game_act_menu.h"

/* SzGame_ActMenu (0x1a7380): one frame of the game's debug start menu.
 * Addresses are cast through words (DotrActMenuWord is int in the game build). */
void SzGame_ActMenu(void) {
    struct DotrActMenuState *menu;
    unsigned int selectionIndex;
    int high;
    int low;
    unsigned int limit;
    unsigned int offset;
    unsigned int *value;
    int item;
    unsigned int previous;
    char filename[128];

    menu = (struct DotrActMenuState *) &gSzGame_gl_actmenu;
    selectionIndex = data_00317473_actmenu + (data_0031747d_actmenu & 7) * 20;
    if (SzGame_ActMemCardProc() >= 0) {
        return;
    }
        if (selectionIndex >= 29) {
            selectionIndex = (menu->page & 7) * 20;
            menu->selected = 0;
        }
        item = DebugMenuOptions_actmenu[selectionIndex];
        if (SzFade_GetRestCount(0) == 0) {
            SzGame_SetMode(menu->x, menu->y);
            SzFade_SetKillFlag(0);
            SzFade_Destroy(1);
            SzFade_Entry(1, 0, 1, 0, 60, 0);
            SzFade_SetKillFlag(1);
            SzFade_SetPauseFlag(1, 1);
            return;
        }
        if (SzPad_Check(0, 2, 0x5000) != 0) {
            int high;
            int low;
            low = menu->selected;
            high = 9;
            if ((unsigned int) ((menu->page & 7) + 1) < 2) {
                high = 20;
            }
            if (SzPad_Check(0, 2, 0x1000) != 0) {
                menu->selected = (menu->selected + high - 1) % high;
            } else {
                menu->selected = (menu->selected + high + 1) % high;
            }
            if (menu->selected != low) {
                SzSound_PlaySE(0, 3);
            }
            return;
        }
        if (SzPad_Check(0, 2, 0xA000) != 0) {
            int high;
            high = menu->page & 7;
            if (SzPad_Check(0, 2, 0x8000) != 0) {
                menu->page = (menu->page & ~7) | (((menu->page & 7) + 1) & 1);
            } else {
                menu->page = (menu->page & ~7) | (((menu->page & 7) + 3) & 1);
            }
            if ((menu->page & 7) != high) {
                SzSound_PlaySE(0, 3);
            }
            return;
        }
        {
            offset = item * 4;
            limit = menu->limits[item];
            if (limit != 0) {
                previous = menu->values[item];
                value = &menu->values[item];
                switch (item) {
                case 3:
                case 4:
                    high = *value / 1000U;
                    low = *value % 1000U;
                    if (SzPad_Check(0, 2, 4) != 0) {
                        low = (low + limit - 1) % limit;
                    }
                    if (SzPad_Check(0, 2, 8) != 0) {
                        low = (low + limit + 1) % limit;
                    }
                    if (SzPad_Check(0, 2, 1) != 0) {
                        high = (high + 1) % 2;
                    }
                    if (SzPad_Check(0, 2, 2) != 0) {
                        high = (high + 3) % 2;
                    }
                    *value = low + high * 1000;
                    break;
                case 17:
                case 19:
                case 22:
                    high = *value / 100U;
                    low = *value % 100U;
                    if (SzPad_Check(0, 2, 4) != 0) {
                        low = (low + 99) % 100;
                    }
                    if (SzPad_Check(0, 2, 8) != 0) {
                        low = (low + 101) % 100;
                    }
                    if (SzPad_Check(0, 2, 1) != 0) {
                        high = (high + 99) % 100;
                    }
                    if (SzPad_Check(0, 2, 2) != 0) {
                        high = (high + 101) % 100;
                    }
                    *value = low + high * 100;
                    break;
                case 23:
                    high = *value / 1000U;
                    low = *value % 1000U;
                    if (SzPad_Check(0, 2, 4) != 0) {
                        low = (low + 999) % 1000;
                    }
                    if (SzPad_Check(0, 2, 8) != 0) {
                        low = (low + 1001) % 1000;
                    }
                    if (SzPad_Check(0, 2, 1) != 0) {
                        high = (high + 99) % 100;
                    }
                    if (SzPad_Check(0, 2, 2) != 0) {
                        high = (high + 101) % 100;
                    }
                    *value = low + high * 1000;
                    break;
                case 24: {
                    unsigned int packed;
                    packed = *value;
                    high = packed >> 8;
                    low = packed & 0xFF;
                    if (SzPad_Check(0, 2, 4) != 0) {
                        low = (low + 255) % 256;
                    }
                    if (SzPad_Check(0, 2, 8) != 0) {
                        low = (low + 257) % 256;
                    }
                    if (SzPad_Check(0, 2, 1) != 0) {
                        high = (high + 255) % 256;
                    }
                    if (SzPad_Check(0, 2, 2) != 0) {
                        high = (high + 257) % 256;
                    }
                    *value = low + (high << 8);
                    break;
                }
                case 26:
                    if (SzPad_Check(0, 2, 4) != 0) {
                        *value = (*value + limit - 1) % limit;
                        if (*value == 0) {
                            *value = limit - 1;
                        }
                    }
                    if (SzPad_Check(0, 2, 8) != 0) {
                        *value = (*value + limit + 1) % limit;
                        if (*value == 0) {
                            *value = 1;
                        }
                    }
                    break;
                case 27:
                    if (SzPad_Check(0, 2, 4) != 0) {
                        *value = (*value + limit - 1) % limit;
                        if (*value == 0) {
                            *value = 12;
                        }
                    }
                    if (SzPad_Check(0, 2, 8) != 0) {
                        *value = (*value + limit + 1) % limit;
                        if (*value == 0) {
                            *value = 1;
                        }
                    }
                    break;
                default:
                    if (SzPad_Check(0, 2, 4) != 0) {
                        *value = (*value + limit - 1) % limit;
                    }
                    if (SzPad_Check(0, 2, 8) != 0) {
                        *value = (*value + limit + 1) % limit;
                    }
                    break;
                }
                if (*value != previous) {
                    SzSound_PlaySE(0, 3);
                }
            }
            switch (item) {
            case 2:
            case 3:
            case 4:
            case 29:
            case 30:
                return;
            case 6:
                if (SzPad_Check(0, 0, 0x10) != 0) {
                    if ((int) menu->values[1] < 22) {
                        high = menu->values[1] + 26;
                        if (menu->values[1] == 0) {
                            high = 17;
                        }
                        SzSound_PlaySE(0, 4);
                        SzData_SetupFromConstDeck(*(unsigned int *) (offset + (DotrActMenuUWord) menu + 0x138), high);
                        return;
                    } else {
                        SzSound_PlaySE(0, 6);
                        return;
                    }
                } else if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_PlaySE(0, 4);
                    SzGame_Push(item, 0);
                }
                return;
            case 10:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_PlaySE(0, 4);
                    SzGame_Push(item, 0);
                }
                return;
            case 13:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_PlaySE(0, 4);
                    *(unsigned int *) (offset + (DotrActMenuUWord) menu + 0x138) = 1;
                    SzGame_Push(item, 0);
                }
                return;
            case 15:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_PlaySE(0, 4);
                    *(unsigned int *) (offset + (DotrActMenuUWord) menu + 0x138) = 1;
                    SzGame_Push(item, 0);
                }
                return;
            case 17:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    unsigned char *address;
                    unsigned char *directory;
                    struct DotrActMenuFile *loaded;
                    address = DOTR_ACT_MENU_LOAD;
                    directory = SzFile_GetEnv((DotrActMenuWord) data_002be7d0_actmenu);
                    low = menu->values[17] % 100U;
                    high = menu->values[17] / 100U;
                    menu->secondAddress = 0;
                    menu->firstAddress = 0;
                    func_0019b648(filename, data_002be7d5_actmenu, (DotrActMenuWord) directory, low);
                    if ((loaded = (struct DotrActMenuFile *) SzFile_Read((DotrActMenuWord) filename, (DotrActMenuWord) address, -1)) != 0) {
                        menu->firstAddress = 0x04000000;
                        RhText_BindAddress(menu->firstAddress);
                        address = DOTR_ACT_MENU_LOAD + ((loaded->size + 63) & ~63);
                    }
                    if (high != 0) {
                        func_0019b648(filename, data_002be7d5_actmenu, directory, high);
                        if (SzFile_Read((DotrActMenuWord) filename, (DotrActMenuWord) address, -1) != 0) {
                            menu->secondAddress = (DotrActMenuWord) address;
                            RhText_BindAddress(menu->secondAddress);
                        }
                    }
                    if (menu->firstAddress != 0 || menu->secondAddress != 0) {
                        SzSound_PlaySE(0, 4);
                        SzGame_Push(item, 0);
                        return;
                    } else {
                        SzSound_PlaySE(0, 6);
                        return;
                    }
                }
                return;
            case 20:
                menu->mode = *(signed char *) &menu->values[20];
                return;
            case 21:
                menu->flags.enabled = *(signed char *) &menu->values[21];
                return;
            case 22:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_PlayBGM(menu->values[22], 1);
                    return;
                } else if (SzPad_Check(0, 0, 0x20) != 0) {
                    SzSound_StopBGM(0x100);
                    return;
                }
                return;
            case 23: {
                int channel;
                int sound;
                channel = menu->values[23] / 1000U;
                sound = menu->values[23] % 1000U;
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_TransferSEGrp(channel, sound, 1);
                }
                return;
            }
            case 24: {
                int channel;
                channel = menu->values[23] / 1000U;
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_PlaySE(channel, menu->values[24]);
                    return;
                } else if (SzPad_Check(0, 0, 0x20) != 0) {
                    SzSound_StopSE(channel, menu->values[24]);
                    return;
                }
                return;
            }
            case 25:
                if (SzPad_Check(0, 0, 0x10) != 0) {
                    if (menu->values[25] != 0) {
                        SD_SuspendSoundSystem(0);
                        SzSound_Init();
                    } else {
                        SD_StopAll();
                        SD_SuspendSoundSystem(1);
                        SzSound_Term();
                    }
                }
                SzData_SetOption(0, 2, menu->values[25]);
                return;
            case 26:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SD_BGMSetSourceLevel(menu->values[26], 0x80);
                    SzSound_PlaySE(0, 4);
                }
                return;
            case 27:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    int count;
                    int i;
                    count = menu->values[27];
                    i = 0;
                    do {
                        SzData_SetRankDirect(0, i, count);
                        SzData_SetRankDirect(1, i, count);
                        i++;
                    } while (i < 854);
                    SzSound_PlaySE(0, 4);
                }
                return;
            case 31:
                SzData_SetOption(0, 1, menu->values[31]);
                return;
            case 32:
                SzData_SetOption(0, 0, menu->values[32]);
                return;
            case 16:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzGame_PlayMovieNo(*(unsigned int *) (offset + (DotrActMenuUWord) menu + 0x138), 1, 1);
                }
                return;
            case 33:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    if (SzFile_InqRequestEx() == 0) {
                        SzText_LoadMerge(menu->values[33], 1);
                        SzSound_PlaySE(0, 4);
                        return;
                    } else {
                        SzSound_PlaySE(0, 6);
                        return;
                    }
                }
                return;
            case 34:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_PlaySE(0, 4);
                    SzGame_InitMemCardProc(0);
                }
                return;
            case 35:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_PlaySE(0, 4);
                    SzGame_InitMemCardProc(1);
                }
                return;
            default:
                if (SzPad_Check(0, 0, 0xC0) != 0) {
                    SzSound_PlaySE(0, 4);
                    SzGame_Push(item, 0);
                }
                return;
            }
        }
}

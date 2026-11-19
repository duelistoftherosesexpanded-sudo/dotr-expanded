#include "dotr/ee/glide_steps.h"

int Taku_Camp_3D_BOU(int cursor)
{
    struct DotrGlideVector3 pos;
    struct DotrGlideBank *bank = DOTR_GLIDE_BANK;
    int row;
    int n;
    int side;
    short model;
    short kind;
    short count;
    short state;
    int i;
    int k;
    int j;

    bank->idle = 1;
    for (side = 0; side < 2; side++) {
        if (side == 0) {
            model = 28;
            n = 16;
        } else {
            model = 29;
            n = 12;
        }
        for (row = 0; row < n; row++) {
            count = bank->records[side][row].count;
            state = bank->records[side][row].unknown_202;
            kind = 0;
            if (side == 0) {
                switch (row) {
                case 0:
                case 15:
                    kind = 0;
                    break;
                default:
                    switch (state) {
                    case 1:
                        kind = 1;
                        bank->idle = 0;
                        break;
                    case 2:
                        kind = 2;
                        bank->idle = 0;
                        bank->unknown_42ac[side][row] = count;
                        break;
                    case 3:
                        kind = 3;
                        break;
                    }
                    break;
                }
            } else {
                switch (row) {
                case 4:
                case 5:
                case 11:
                    kind = 0;
                    break;
                default:
                    if (state == 1) {
                        kind = 1;
                        bank->idle = 0;
                    } else if (state == 2) {
                        kind = 2;
                        bank->idle = 0;
                    } else if (state == 3) {
                        kind = 3;
                    } else {
                        /* MATCHING DEVICE: the redundant store leaves the branch the original has
                         * after `kind = 3`. */
                        kind = 0;
                    }
                    break;
                }
            }
            if (kind == 1) {
                for (i = 0; i <= bank->unknown_42ac[side][row]; i++) {
                    if (bank->unknown_42ec[side][row][i] < 256) {
                        pos.x = bank->records[side][row].x[i];
                        pos.y = data_00298ba0_glide[0].unknown_04 - 10.0f + 10.0f * (float)bank->unknown_42ec[side][row][i] / 256.0f;
                        pos.z = bank->records[side][row].y[i];
                    } else {
                        pos.x = bank->records[side][row].x[i];
                        pos.y = data_00298ba0_glide[0].unknown_04;
                        pos.z = bank->records[side][row].y[i];
                    }
                    cursor = SzModel_Draw_Pipe(cursor, model + 34, &pos, 0, 0, 0, 0);
                    if (i == bank->unknown_42ac[side][row] && bank->unknown_42ec[side][row][i] >= 64) {
                        if (bank->unknown_42ac[side][row] < count) {
                            bank->unknown_42ac[side][row]++;
                        } else {
                            bank->unknown_42ac[side][row] = count;
                        }
                    }
                    if (bank->unknown_42ec[side][row][i] < 256) {
                        bank->unknown_42ec[side][row][i] += 16;
                        if (bank->unknown_42ec[side][row][i] >= 256) {
                            bank->unknown_42ec[side][row][i] = 256;
                        }
                    }
                    if (count == bank->unknown_42ac[side][row]) {
                        if (bank->unknown_42ec[side][row][count] >= 256) {
                            bank->unknown_52ec[data_00299020_glide[side][row].cell] = 1;
                            bank->records[side][row].unknown_202 = 3;
                            bank->unknown_42ac[side][row] = 0;
                            for (j = 0; j < 64; j++) {
                                bank->unknown_42ec[side][row][j] = 0;
                            }
                        }
                    }
                }
            } else if (kind == 2) {
                for (i = count; i >= bank->unknown_42ac[side][row]; i--) {
                    if (bank->unknown_42ec[side][row][i] < 256) {
                        pos.x = bank->records[side][row].x[i];
                        pos.y = data_00298ba0_glide[0].unknown_04 - 10.0f + 10.0f * (float)bank->unknown_42ec[side][row][i] / 256.0f;
                        pos.z = bank->records[side][row].y[i];
                    } else {
                        pos.x = bank->records[side][row].x[i];
                        pos.y = data_00298ba0_glide[0].unknown_04;
                        pos.z = bank->records[side][row].y[i];
                    }
                    cursor = SzModel_Draw_Pipe(cursor, model + 34, &pos, 0, 0, 0, 0);
                    if (i == bank->unknown_42ac[side][row] && bank->unknown_42ec[side][row][i] >= 64) {
                        if (0 < bank->unknown_42ac[side][row]) {
                            bank->unknown_42ac[side][row]--;
                        } else {
                            bank->unknown_42ac[side][row] = 0;
                        }
                    }
                    if (bank->unknown_42ec[side][row][i] < 256) {
                        bank->unknown_42ec[side][row][i] += 16;
                        if (bank->unknown_42ec[side][row][i] >= 256) {
                            bank->unknown_42ec[side][row][i] = 256;
                        }
                    }
                    if (bank->unknown_42ac[side][row] == 0) {
                        if (bank->unknown_42ec[side][row][0] >= 256) {
                            bank->unknown_52ec[data_00299020_glide[side][row].other_cell] = 1;
                            bank->records[side][row].unknown_202 = 3;
                            bank->unknown_42ac[side][row] = 0;
                            for (k = 0; k < 64; k++) {
                                bank->unknown_42ec[side][row][k] = 0;
                            }
                        }
                    }
                }
            } else if (kind == 3) {
                for (i = 0; i <= count; i++) {
                    pos.x = bank->records[side][row].x[i];
                    pos.y = data_00298ba0_glide[0].unknown_04;
                    pos.z = bank->records[side][row].y[i];
                    cursor = SzModel_Draw_Pipe(cursor, model + 34, &pos, 0, 0, 0, 0);
                }
            }
        }
    }
    return cursor;
}

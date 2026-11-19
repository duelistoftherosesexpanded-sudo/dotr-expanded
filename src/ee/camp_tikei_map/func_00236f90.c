#include "dotr/ee/camp_tikei_map.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* Taku_Camp_TIKEI_Map_Hyouji (0x236f90): the terrain map panel. Clears ten counters at
 * 0x1ed0006, then draws the 7 x 7 byte map `mapIndex` cell by cell: codes 0-9 count into their
 * counter and draw as item code 'A' + code, anything else draws as 'A' uncounted; the grid
 * is mirrored on both axes when the first camp state byte is 1. Then ten bars from the
 * fourth window record at 16-pixel steps, and per terrain two quads whose eight halfwords
 * are refreshed from row 0, with a width of 76 * count / 49 (clamped to 0 and 76),
 * followed by the count as one or two digits.
 * Compiled warm (the float width is converted to a halfword argument). */
DotrTikeiWord Taku_Camp_TIKEI_Map_Hyouji(DotrTikeiWord gp, unsigned short mapIndex, unsigned short x,
                                         unsigned short y)
{
    struct DotrTikeiCounts *work;
    signed char *mode;
    int row;
    unsigned char *grid;
    int col;
    struct DotrTikeiItem card;
    struct DotrTikeiItem bar;
    int j;
    int i;
    short dy;
    int cx;
    short tile;
    int cy;
    float scale;
    unsigned char *cell;
    int k;

    mode = data_00299e10;
    work = DOTR_TIKEI_COUNTS;
    card = data_0029afa0;
    bar = data_0029afd0;
    for (i = 0; i < 10; i++) {
        work->count[i] = 0;
    }
    for (row = 0, grid = &((unsigned char *)gSzDuel_MapData)[mapIndex * 49]; row < 7; row++) {
        cell = grid;
        for (col = 0; col < 7; col++) {
            switch ((short)*cell) {
            case 0:
                tile = 'A';
                work->count[0]++;
                break;
            case 1:
                tile = 'B';
                work->count[1]++;
                break;
            case 2:
                tile = 'C';
                work->count[2]++;
                break;
            case 3:
                tile = 'D';
                work->count[3]++;
                break;
            case 4:
                tile = 'E';
                work->count[4]++;
                break;
            case 5:
                tile = 'F';
                work->count[5]++;
                break;
            case 6:
                tile = 'G';
                work->count[6]++;
                break;
            case 7:
                tile = 'H';
                work->count[7]++;
                break;
            case 8:
                tile = 'I';
                work->count[8]++;
                break;
            case 9:
                tile = 'J';
                work->count[9]++;
                break;
            default:
                tile = 'A';
                break;
            }
            card.code = tile;
            if (*mode == 1) {
                cx = 6 - col;
                cy = 6 - row;
            } else {
                cx = col;
                cy = row;
            }
            gp = Taku_Obj_Set(gp, &card, x + cx * 16, y + cy * 16);
            cell++;
        }
        grid += 7;
    }
    for (i = 0, dy = 0; i < 10; i++) {
        bar.w[0] = data_00297bae_tikeimap;
        bar.w[1] = data_00297bb0_tikeimap;
        bar.w[2] = data_00297bb2_tikeimap;
        bar.w[3] = data_00297bb0_tikeimap;
        bar.w[4] = data_00297bae_tikeimap;
        bar.w[5] = data_00297bb4_tikeimap;
        bar.w[6] = data_00297bb2_tikeimap;
        bar.w[7] = data_00297bb4_tikeimap;
        gp = Taku_G4_Set(gp, &bar, x, y + dy);
        dy += 16;
    }
    for (i = 0, dy = 0; i < 10; i++, dy += 16) {
        if (work->count[i] >= 49) {
            scale = 76.0f;
        } else if (work->count[i] <= 0) {
            scale = 0.0f;
        } else {
            scale = (76.0f * work->count[i]) / 49.0f;
        }
        for (k = 0; k < 2; k++) {
            for (j = 0; j < 8; j++) {
                data_002986b0[i][k].w[j] = data_002986b0[0][k].w[j];
            }
            gp = func_00218ab0_tikeimap(gp, &data_002986b0[i][k], x, y + dy, scale, 0);
        }
        gp = SzDuel_DrawWindow_FontDigit_Pipe(gp, x + ((data_00297bae_tikeimap + data_00297bb2_tikeimap) >> 1),
                                              y + (dy + data_002986b0[i][0].w[1]), 1, 2,
                                              (unsigned short)((work->count[i] < 10) ? 1 : 2), work->count[i]);
    }
    return gp;
}

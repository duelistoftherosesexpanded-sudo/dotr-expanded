#include "dotr/ee/glide_steps.h"

/* Taku_Nagare2_set (0x228460). Compiled at optimisation level 3 (docs/DECOMPILATION.md);
 * the pragma pair covers this function alone. The side-1 record index is read once into a
 * local. */
#ifdef __MWERKS__
#pragma optimization_level 3
#endif
void Taku_Nagare2_set(short side, short row, short *list, short count)
{
    struct DotrGlideBank *bank;
    short i;

    switch (*DOTR_GLIDE_LIMIT) {
    case 0:
        DOTR_GLIDE_HEAD->unknown_1c = 1;
        DOTR_GLIDE_HEAD->unknown_1a = 0;
        Taku_Dodai_Root_init(side, row, 3, list, count);
        *DOTR_GLIDE_LIMIT += 1;
        /* fall through */
    case 1:
        if (DOTR_GLIDE_HEAD->unknown_1c == 2) {
            if (side == 0) {
                bank = DOTR_GLIDE_BANK;
                for (i = 0; i < count; i++) {
                    if (bank->records[side][list[i]].unknown_202 != 3) {
                        bank->records[side][list[i]].unknown_202 = data_00298ba0_glide[row].marks[i];
                    } else {
                        bank->records[side][list[i]].unknown_202 = 3;
                    }
                }
            } else {
                short n = list[0];

                if (DOTR_GLIDE_BANK->records[side][n].unknown_202 != 3) {
                    DOTR_GLIDE_BANK->records[side][n].unknown_202 = 1;
                } else {
                    DOTR_GLIDE_BANK->records[side][n].unknown_202 = 3;
                }
            }
            *DOTR_GLIDE_LIMIT += 1;
        }
        break;
    }
    Taku_Tyumoku_ido0(side, row, list, count);
    Taku_Tyumoku_ido_kettei(side, row, 2);
}
#ifdef __MWERKS__
#pragma optimization_level reset
#endif

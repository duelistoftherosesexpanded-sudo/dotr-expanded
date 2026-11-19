#include "dotr/ee/camp_system.h"

/* The work area by offset, as in the target (lui/ori constants). The base is the literal
 * 0x1ed0000 under __MWERKS__ and the host fixture otherwise (camp_system.h). */
#define WB(o) (*(signed char *)(DOTR_CAMP_SYSTEM_BASE + (o)))
#define WH(o) (*(short *)(DOTR_CAMP_SYSTEM_BASE + (o)))
#define WF(o) (*(float *)(DOTR_CAMP_SYSTEM_BASE + (o)))
#define BANK DOTR_CAMP_SYSTEM_BANK

/* Macros of this source only: the original has each of these bodies written out at every
 * use. COPY uses the caller's `n`, `i`, `buf` and `sd`.
 * MATCHING DEVICE: the copy loops read the count into the short `n` first and index the
 * location's list with the short `sd`, while the callees get the int `side`. */
#define COPY(K) n = spot[K].n; for (i = 0; i < n; i++) { buf[i] = spot[K].list[sd][i]; }
#define FLOATS WF(0x40) = WF(0x1b8); WF(0x48) = WF(0x1c0);
#define NAG(C, K, F) case C: COPY(K) func_00228460_campsystem(side, K, buf, n, F); break;
#define NAG1(C, K) case C: buf[0] = spot[K].list[sd][0]; func_00228460_campsystem(side, K, buf, 1, 0); break;
#define KM(C, K) case C: func_002294b0_campsystem(side, K); break;
#define spot data_00298ba0_campsystem

/* MATCHING DEVICE (each needed for the exact bytes; none is a claim about the original source):
 * - `int side` from the board byte, and the short `sd` assigned from it only after the
 *   Taku_Camp_Ivent call; `sd` indexes and is tested, `side` goes to the callees;
 * - the test for tick 104 is a one-case switch inside `if (h194 < 10000)`;
 * - the outer switch cases end with `break`; the step switches whose cases leave the function
 *   end every case with `return;`; the one step switch that is followed by code (side 0, tick
 *   100, then `lv[11] = 0`) uses `break`;
 * - ticks 212/213 and 312/313 of the second switch share one body; every other repeated body
 *   is a case of its own;
 * - the local declaration order. */
void Taku_Camp_System(void)
{
    struct DotrCampSystemBoard *b = &data_00299e10_campsystem;
    int side = b->b0;
    short sd;
    short i;
    short n;
    short buf[8];

    if (WB(0x18c) != 0) {
        if (b->h194 < 200) {
            WB(0x537a) = 0;
            WB(0x537b) = 0;
            func_00229140_campsystem(side, 0, 0);
        } else if (b->h194 < 300) {
            func_002294b0_campsystem(side, b->b198);
        } else if (b->h194 < 400) {
            func_002294b0_campsystem(side, b->b198);
        }
        WB(0x53b4) = 0;
        WB(0x53b5) = 0;
        WH(0x53b6) = 0;
        WH(0x53b8) = 0;
        if (WB(0x546c) == 0) {
            func_00217980_campsystem(4, 1);
            WB(0x546c) = 1;
        }
        data_0034f6f0_campsystem = 0;
        return;
    }
    if (b->h194 >= 100 && b->h194 < 200) {
        WB(0x537a) = 0;
        data_0034f6f0_campsystem = 0;
    }
    if (b->h194 < 10000) {
        switch (b->h194) {
        case 104:
            break;
        default:
            if (WB(0x546c) == 0) {
                func_00217980_campsystem(4, 1);
                WB(0x546c) = 1;
            }
            break;
        }
    }
    func_0021e330_campsystem();
    sd = side;
    if (sd == 0) {
        switch (b->h194) {
        case 0:
            WB(0x537a) = 0;
            func_00229140_campsystem(side, 0, 0);
            data_0034f6f0_campsystem = 0;
            break;
        case 100:
            COPY(11)
            b->lv[11] = 1;
            switch (WH(0x536c)) {
            case 0:
                WH(0x1c) = 1;
                WH(0x1a) = 2;
                Taku_Fune_init(0, 0);
                func_00228660_campsystem(side, 0, 0, buf, 1);
                b->b199 = 11;
                WH(0x536c)++;
                break;
            case 1:
                if (My_LedarModel_a_set(0) && Taku_Tyumoku_ido() && WH(0x1c) == 2) {
                    Taku_Fune_init(1, 0);
                    WH(0x536c)++;
                }
                break;
            case 2:
                FLOATS
                if (WH(0x1a4) == 3) {
                    func_00217980_campsystem(27, 0);
                    WH(0x536c)++;
                    WB(0x537b)++;
                    WB(0x537d) = 1;
                }
                break;
            case 3:
                FLOATS
                if (b->lv[0] != 0) {
                    if (My_LedarModel_a_set(1)) {
                        WH(0x1c) = 1;
                        WH(0x1a) = 3;
                        WH(0x536c)++;
                    }
                }
                break;
            case 4:
                if (WB(0x537b) == 4) {
                    WB(0x537b)++;
                }
                if (WB(0x537b) == 12 && WH(0x1c) == 2) {
                    WB(0x537b) = 0;
                    WB(0x537d) = 0;
                    b->h194 = 11020;
                    WH(0x536c) = 0;
                }
                break;
            }
            b->lv[11] = 0;
            break;
        case 101:
            COPY(0)
            b->lv[0] = 1;
            switch (WH(0x536c)) {
            case 0:
                WF(0x40) = spot[0].x;
                WF(0x48) = spot[0].y;
                WH(0x1c) = 1;
                WH(0x1a) = 2;
                Taku_Fune_init(0, 1);
                buf[0] = 0;
                func_00228660_campsystem(side, 0, 3, buf, 1);
                buf[0] = 1;
                buf[1] = 2;
                func_00228660_campsystem(side, 1, 0, buf, 2);
                b->lv[0] = 0;
                WH(0x536c)++;
                return;
            case 1:
                if (My_LedarModel_a_set(0) && Taku_Tyumoku_ido() && WH(0x1c) == 2) {
                    Taku_Fune_init(1, 1);
                    WH(0x536c)++;
                }
                return;
            case 2:
                FLOATS
                if (WH(0x1a4) == 3) {
                    WH(0x536c)++;
                }
                return;
            case 3:
                FLOATS
                if (b->lv[1] != 0) {
                    if (My_LedarModel_a_set(1)) {
                        WH(0x1c) = 1;
                        WH(0x1a) = 0;
                        b->h194++;
                        WH(0x536c) = 0;
                    }
                }
                return;
            }
            break;
        NAG(102, 1, 0)
        NAG(103, 2, 1)
        NAG(104, 3, 1)
        NAG(105, 4, 1)
        NAG(106, 5, 1)
        NAG(107, 6, 1)
        NAG(108, 7, 0)
        NAG(109, 8, 1)
        case 110: func_00228f50_campsystem(side, 9); func_00229140_campsystem(0, 9, 0); break;
        case 111: func_00228f50_campsystem(side, 10); func_00229140_campsystem(0, 10, 0); break;
        case 112: func_00228f50_campsystem(side, 11); func_00229140_campsystem(0, 11, 0); break;
        case 113: func_00228f50_campsystem(side, 11); func_00229140_campsystem(0, 11, 0); break;
        KM(203, 2) KM(204, 3) KM(205, 4) KM(206, 5) KM(207, 6) KM(208, 7) KM(209, 8) KM(210, 9) KM(211, 10)
        case 212: b->b201 = 0; func_002294b0_campsystem(side, 11); break;
        case 213: b->b201 = 1; func_002294b0_campsystem(side, 11); break;
        KM(303, 2) KM(304, 3) KM(305, 4) KM(306, 5) KM(307, 6) KM(308, 7) KM(309, 8) KM(310, 9) KM(311, 10)
        case 312: b->b201 = 0; func_002294b0_campsystem(side, 11); break;
        case 313: b->b201 = 1; func_002294b0_campsystem(side, 11); break;
        }
    } else {
        switch (b->h194) {
        case 0:
            WB(0x537a) = 0;
            func_00229140_campsystem(side, 0, 0);
            data_0034f6f0_campsystem = 0;
            break;
        case 100:
            buf[0] = spot[11].list[sd][0];
            func_00228460_campsystem(side, 11, buf, 1, 0);
            b->lv[11] = 0;
            break;
        NAG1(101, 12)
        NAG1(102, 13)
        NAG1(103, 14)
        case 104:
            buf[0] = spot[15].list[sd][0];
            b->lv[15] = 3;
            switch (WH(0x536c)) {
            case 0:
                WH(0x1c) = 1;
                WH(0x1a) = 2;
                WH(0x60) = 0;
                WB(0x5372) = 1;
                WB(0x5373) = 0;
                WB(0x537b) = 0;
                WB(0x53a0) = 0;
                WB(0x5375) = 0;
                Taku_Fune_init(0, 2);
                func_00228660_campsystem(side, 15, 3, buf, 1);
                WH(0x536c)++;
                return;
            case 1:
                if (My_LedarModel_a_set(0) && Taku_Tyumoku_ido() && WH(0x1c) == 2) {
                    Taku_Fune_init(1, 2);
                    WH(0x536c)++;
                }
                return;
            case 2:
                FLOATS
                if (WH(0x1a4) == 3) {
                    WH(0x536c) = 0;
                    b->h194 = 12041;
                    b->b199 = 16;
                }
                return;
            case 3:
                FLOATS
                if (b->lv[spot[15].list[sd][buf[0]]] != 0) {
                    if (My_LedarModel_a_set(1)) {
                        WH(0x1c) = 1;
                        WH(0x1a) = 0;
                        BANK->records[sd][buf[0]].flag = 1;
                        WH(0x536c)++;
                    }
                }
                return;
            }
            break;
        case 105:
            buf[0] = spot[16].list[sd][0];
            buf[1] = spot[17].list[sd][0];
            b->lv[16] = 3;
            switch (WH(0x536c)) {
            case 0:
                WH(0x1c) = 0;
                WH(0x1a) = 2;
                WH(0x60) = 0;
                WB(0x5372) = 1;
                WB(0x5373) = 0;
                WB(0x537b) = 0;
                WB(0x53a0) = 0;
                WB(0x5375) = 0;
                Taku_Fune_init(0, 3);
                func_00228660_campsystem(side, 16, 3, buf, 2);
                func_00228660_campsystem(side, 17, 0, buf, 2);
                func_00217980_campsystem(35, 1);
                b->b199 = 16;
                WH(0x536c)++;
                return;
            case 1:
                if (WH(0x6c) == 0) {
                    WH(0x1c) = 1;
                    WH(0x536c)++;
                }
                return;
            case 2:
                if (My_LedarModel_a_set(0) && Taku_Tyumoku_ido() && WH(0x1c) == 2) {
                    Taku_Fune_init(1, 3);
                    WH(0x536c)++;
                }
                return;
            case 3:
                FLOATS
                if (WH(0x1a4) == 3) {
                    WH(0x536c)++;
                    WB(0x537b)++;
                    WB(0x537d) = 1;
                    b->b199 = 17;
                }
                return;
            case 4:
                FLOATS
                if (b->lv[data_00299020_campsystem[sd][buf[0]].cell] != 0) {
                    if (My_LedarModel_a_set(1)) {
                        WH(0x1c) = 1;
                        WH(0x1a) = 0;
                        BANK->records[sd][buf[0]].flag = 1;
                        b->h194++;
                        WH(0x536c) = 0;
                    }
                }
                return;
            }
            break;
        case 106:
            buf[0] = spot[17].list[sd][0];
            func_00228460_campsystem(side, 17, buf, 1, 0);
            if (WB(0x546c) == 0) {
                func_00217980_campsystem(4, 1);
                WB(0x546c) = 1;
            }
            break;
        NAG1(107, 18)
        NAG1(108, 19)
        NAG1(109, 20)
        NAG1(110, 21)
        case 111:
            buf[0] = spot[0].list[sd][0];
            switch (WH(0x536c)) {
            case 0:
                WH(0x1c) = 0;
                WH(0x1a) = 2;
                WH(0x60) = 0;
                WB(0x5372) = 1;
                WB(0x5373) = 0;
                WB(0x537b) = 0;
                WB(0x53a0) = 0;
                WB(0x5375) = 0;
                Taku_Fune_init(0, 4);
                func_00228660_campsystem(side, 0, 3, buf, 1);
                b->lv[0] = 3;
                WH(0x536c)++;
                return;
            case 1:
                if (WH(0x6c) == 0) {
                    WH(0x1c) = 1;
                    WH(0x536c)++;
                }
                return;
            case 2:
                if (WH(0x1c) == 2) {
                    WB(0x537b)++;
                    WB(0x537d) = 1;
                    WB(0x5375) = 1;
                    WH(0x536c)++;
                }
                return;
            case 3:
                if (WB(0x537b) == 14) {
                    WB(0x537b) = 0;
                    WB(0x537d) = 0;
                    WH(0x536c)++;
                }
                return;
            case 4:
                if (My_LedarModel_a_set(0) && Taku_Tyumoku_ido()) {
                    Taku_Fune_init(1, 4);
                    WH(0x536c)++;
                }
                return;
            case 5:
                FLOATS
                if (WH(0x1a4) == 3) {
                    b->b199 = 11;
                    WH(0x536c)++;
                }
                return;
            case 6:
                FLOATS
                if (b->lv[11] != 0) {
                    if (My_LedarModel_a_set(1)) {
                        WH(0x1c) = 1;
                        WH(0x1a) = 0;
                        WH(0x536c)++;
                    }
                }
                return;
            case 7:
                if (WH(0x60) == 0) {
                    WF(0x50) = WF(0x40);
                    WF(0x58) = WF(0x48);
                }
                WF(0x40) = WF(0x50) * (float)(256 - WH(0x60)) / 256.0f + spot[11].x * (float)WH(0x60) / 256.0f;
                WF(0x48) = WF(0x58) * (float)(256 - WH(0x60)) / 256.0f + spot[11].y * (float)WH(0x60) / 256.0f;
                WH(0x60) += 16;
                if (WH(0x60) > 256) {
                    WH(0x60) = 0;
                    WF(0x50) = WF(0x40);
                    WF(0x58) = WF(0x48);
                    WB(0x5372) = 0;
                    WB(0x5375) = 0;
                    WH(0x536c)++;
                }
                return;
            case 8:
                WH(0x1c) = 1;
                WH(0x1a) = 3;
                WH(0x536c)++;
                return;
            case 9:
                if (WH(0x1c) == 2) {
                    WH(0x536c) = 0;
                    b->h194 = 12130;
                }
                return;
            }
            break;
        case 112: func_00228f50_campsystem(side, 11); func_00229140_campsystem(1, 11, 0); break;
        KM(201, 12) KM(202, 13) KM(203, 14) KM(205, 16) KM(207, 18) KM(208, 19) KM(209, 20) KM(210, 21) KM(211, 0)
        case 212: KM(213, 11)
        KM(301, 12) KM(302, 13) KM(303, 14) KM(305, 16) KM(307, 18) KM(308, 19) KM(309, 20) KM(310, 21) KM(311, 0)
        case 312: KM(313, 11)
        }
    }
}

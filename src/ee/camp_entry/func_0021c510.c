#include "dotr/ee/camp_entry.h"

void TkCamp_Init(int mode)
{
    /* MATCHING DEVICE: the declaration order of the locals below (`w` before `board`; then
     * deck, leader, i, j, r, n, k, p, flag, m) and which variable each loop uses decide the
     * saved registers. `w` holds the work area for the whole function; the other accesses
     * spell the literal address again (DOTR_CAMP_ENTRY_WORK) or go through `p`, as marked. */
    struct DotrCampEntryQuad pos;
    struct DotrCampEntryQuad rot;
    struct DotrCampEntryWork *w = DOTR_CAMP_ENTRY_WORK;
    struct DotrCampEntryBoard *board = &data_00299e10_campentry;
    int deck;
    int leader;
    int i;
    int j;
    short r;
    short n;
    short k;
    struct DotrCampEntryWork *p;
    int flag;
    short m;

    Move_END_flg_byte = 0;
    Taku_Camp_Data_Init();
    Taku_spline_init();
    Taku_Camp_3D_Poly_Init();
    func_00233f50_campentry(3);
    for (i = 0; i < 2; i++) {
        SzModel_SetDrawFlag(i + 30, 0);
    }
    if (board->done0 != 0 && board->done1 != 0) {
        DOTR_CAMP_ENTRY_WORK->all = 1;
    } else {
        DOTR_CAMP_ENTRY_WORK->all = 0;
    }
    Taku_Screen_Init();
    SzGame_SetMenuNum(11, 1);
    DOTR_CAMP_ENTRY_WORK->byte_53c7 = 2;
    /* MATCHING DEVICE: the cases are written in the order 0 | 1, 2, 4-7 | 3 | 20 | 30 | 40 |
     * 50 | 60 | 10, the order of the target's blocks (a compare chain, no jump table). */
    switch (mode) {
    case 0:
        func_00195108(board, 332);
        SzModel_LoadMerge(3, 4, 0);
        SzText_LoadMerge(10, 0);
        DOTR_CAMP_ENTRY_MODULES();
        board->side = 0;
        board->done0 = 0;
        board->done1 = 0;
        DOTR_CAMP_ENTRY_WORK->all = 0;
        DOTR_CAMP_ENTRY_CLEAR_SEEN();
        board->ticks = 10020;
        board->started = 1;
        func_00233f50_campentry(3);
        DOTR_CAMP_ENTRY_WORK->byte_53b3 = 1;
        data_0034f6f8 = 0;
        data_0034f700 = 0;
        func_00217980_campentry(24, 0);
        break;
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
        if (SzModel_IsModelLoading(30) == 0) {
            pos = data_0029a790_campentry;
            rot = data_0029a7a0_campentry;
            deck = SzData_GetCurrentDeck(0);
            leader = SzData_GetLeader(0, deck);
            DOTR_CAMP_ENTRY_WORK->leader = SzData_GetLeader(0, deck);
            func_001f22b0_campentry(30, &pos);
            func_001f2430_campentry(30, &rot);
            SzModel_Load(30, -1, SzDuel_New2Xax(leader), 1);
        }
        if (DOTR_CAMP_ENTRY_WORK->all != 0) {
            SzModel_LoadMerge(3, 2, 0);
            func_001f93f0_campentry(0, 7, 0, 0);
            SzEffect_SetIntensityOnly(7, 1.0f);
            board->done0 = 1;
            board->done1 = 1;
            DOTR_CAMP_ENTRY_WORK->all = 1;
            board->ticks = 0;
            if (DOTR_CAMP_ENTRY_WORK->all != 0) {
                p = DOTR_CAMP_ENTRY_WORK;
                for (i = 0; i < 32; i++) {
                    p->rise[i] = 0;
                    p->count[i] = 0;
                    board->seen[i] = 3;
                }
                p = DOTR_CAMP_ENTRY_WORK;
                /* MATCHING DEVICE: a short `m` outside and the int `i` inside. */
                for (m = 0; m < 2; m++) {
                    for (i = 0; i < 16; i++) {
                        p->records[m][i].state = 3;
                    }
                }
            }
            board->ticks = 0;
            func_00233f50_campentry(0);
            DOTR_CAMP_ENTRY_WORK->byte_53b3 = 0;
            func_00217980_campentry(4, 0);
            break;
        }
        flag = 0;
        if (data_0034f708 != 0) {
            data_0034f708 = 0;
            SzModel_LoadMerge(3, 4, 0);
            SzText_LoadMerge(10, 0);
            if (board->done0 != 0) {
                board->side = 1;
                board->done0 = 1;
                board->done1 = 0;
                DOTR_CAMP_ENTRY_WORK->all = 0;
            } else if (board->done1 != 0) {
                board->side = 0;
                board->done0 = 0;
                board->done1 = 1;
                DOTR_CAMP_ENTRY_WORK->all = 0;
            }
            DOTR_CAMP_ENTRY_CLEAR_SEEN();
            board->ticks = 10060;
            board->started = 1;
            func_00233f50_campentry(3);
            DOTR_CAMP_ENTRY_WORK->byte_53b3 = 3;
            func_00217980_campentry(25, 1);
            flag = 1;
        } else if (board->started != 0) {
            SzModel_LoadMerge(3, 2, 0);
            func_001f93f0_campentry(0, 7, 0, 0);
            SzEffect_SetIntensityOnly(7, 1.0f);
            if (board->side == 0) {
                /* MATCHING DEVICE: `n = 0` before the store; the bounds as `<=`; the location
                 * record indexed afresh in each expression (no pointer variable); a short
                 * `r` of its own; the records reached through `w`. */
                n = 0;
                board->seen[0] = 1;
                for (j = 1; j <= 11; j++) {
                    if (board->seen[j] == 3) {
                        for (r = 0; r < data_00298ba0_campentry[j].n; r++) {
                            w->records[0][data_00298ba0_campentry[j].list[r]].state = 3;
                        }
                    }
                }
                for (i = 2; i <= 10; i++) {
                    if (board->seen[i] >= 3) {
                        n++;
                        if (n >= 8) {
                            w->records[0][12].state = 3;
                            w->records[0][13].state = 3;
                            w->records[0][11].state = 3;
                        }
                    }
                }
            } else {
                DOTR_CAMP_ENTRY_WORK->records[1][0].state = 3;
                p = DOTR_CAMP_ENTRY_WORK;
                /* MATCHING DEVICE: two counters in one `for`, bound as `<=`. */
                for (j = 12, n = 1; j <= 21; j++, n++) {
                    if (board->seen[j] == 3) {
                        p->records[1][n].state = 3;
                    }
                }
                if (board->seen[0] == 3) {
                    DOTR_CAMP_ENTRY_WORK->records[1][11].state = 3;
                }
            }
            board->started = 1;
            func_00233f50_campentry(0);
            DOTR_CAMP_ENTRY_WORK->byte_53b3 = 0;
            /* As in the target: the target point is written from kind[0] and then again
             * from kind[1]; `to.w` is not written. */
            DOTR_CAMP_ENTRY_WORK->to.x = data_00298ba0_campentry[board->kind[0]].x;
            DOTR_CAMP_ENTRY_WORK->to.y = 0.0f;
            DOTR_CAMP_ENTRY_WORK->to.z = data_00298ba0_campentry[board->kind[0]].z;
            DOTR_CAMP_ENTRY_WORK->to.x = data_00298ba0_campentry[board->kind[1]].x;
            DOTR_CAMP_ENTRY_WORK->to.y = 0.0f;
            DOTR_CAMP_ENTRY_WORK->to.z = data_00298ba0_campentry[board->kind[1]].z;
            DOTR_CAMP_ENTRY_WORK->from.x = DOTR_CAMP_ENTRY_WORK->to.x;
            DOTR_CAMP_ENTRY_WORK->from.y = 96.0f + DOTR_CAMP_ENTRY_WORK->to.y;
            DOTR_CAMP_ENTRY_WORK->from.z = 96.0f + DOTR_CAMP_ENTRY_WORK->to.z;
            DOTR_CAMP_ENTRY_WORK->from.w = DOTR_CAMP_ENTRY_WORK->to.w;
            func_001f9fe0_campentry(0, &DOTR_CAMP_ENTRY_WORK->from, &DOTR_CAMP_ENTRY_WORK->to);
            func_00217980_campentry(4, 0);
        } else {
            SzModel_LoadMerge(3, 4, 0);
            SzText_LoadMerge(10, 0);
            if (board->done0 != 0) {
                board->side = 1;
                board->done0 = 1;
                board->done1 = 0;
                DOTR_CAMP_ENTRY_WORK->all = 0;
                DOTR_CAMP_ENTRY_WORK->byte_53a8 = 0;
                board->byte_c9 = 0;
            } else if (board->done1 != 0) {
                board->side = 0;
                board->done0 = 0;
                board->done1 = 1;
                DOTR_CAMP_ENTRY_WORK->all = 0;
                DOTR_CAMP_ENTRY_WORK->byte_53a8 = 1;
                board->byte_c9 = 2;
            }
            DOTR_CAMP_ENTRY_CLEAR_SEEN();
            board->ticks = 10090;
            board->started = 1;
            func_00233f50_campentry(3);
            DOTR_CAMP_ENTRY_WORK->byte_53b3 = 3;
        }
        DOTR_CAMP_ENTRY_MODULES();
        if (flag == 0) {
            func_00217980_campentry(26, 0);
        }
        break;
    case 3:
        SzModel_LoadMerge(3, 4, 0);
        DOTR_CAMP_ENTRY_MODULES();
        board->side = 0;
        board->done0 = 0;
        board->done1 = 0;
        DOTR_CAMP_ENTRY_WORK->all = 0;
        DOTR_CAMP_ENTRY_CLEAR_SEEN();
        board->ticks = 10040;
        board->started = 1;
        func_00233f50_campentry(2);
        func_00217980_campentry(24, 1);
        break;
    case 20: /* the game won on the Lancastrian side */
        SzModel_LoadMerge(3, 0, 0);
        DOTR_CAMP_ENTRY_MODULES();
        board->side = 0;
        board->done0 = 0;
        board->done1 = 0;
        DOTR_CAMP_ENTRY_WORK->all = 0;
        DOTR_CAMP_ENTRY_CLEAR_SEEN();
        board->ticks = 11182;
        board->started = 1;
        func_00233f50_campentry(2);
        func_00217980_campentry(34, 1);
        break;
    case 30: /* the game won on the Yorkist side */
        SzModel_LoadMerge(3, 0, 0);
        DOTR_CAMP_ENTRY_MODULES();
        board->side = 0;
        board->done0 = 0;
        board->done1 = 0;
        DOTR_CAMP_ENTRY_WORK->all = 0;
        DOTR_CAMP_ENTRY_CLEAR_SEEN();
        board->ticks = 12154;
        board->started = 1;
        func_00233f50_campentry(2);
        func_00217980_campentry(38, 1);
        break;
    case 40: /* the Lancastrian side chosen */
        SzModel_LoadMerge(3, 4, 0);
        DOTR_CAMP_ENTRY_MODULES();
        board->side = 0;
        board->done0 = 0;
        board->done1 = 0;
        DOTR_CAMP_ENTRY_WORK->all = 0;
        DOTR_CAMP_ENTRY_CLEAR_SEEN();
        board->ticks = 11000;
        func_00217980_campentry(24, 1);
        break;
    case 50: /* the Yorkist side chosen */
        SzModel_LoadMerge(3, 4, 0);
        DOTR_CAMP_ENTRY_MODULES();
        board->side = 1;
        board->done0 = 0;
        board->done1 = 0;
        DOTR_CAMP_ENTRY_WORK->all = 0;
        DOTR_CAMP_ENTRY_CLEAR_SEEN();
        board->ticks = 12000;
        func_00217980_campentry(24, 1);
        break;
    case 60:
        /* MATCHING DEVICE (here and in case 10): a short `k`, the stores through `p` and the
         * hash read back through `w`. */
        DOTR_CAMP_ENTRY_WORK->hash = 0;
        p = DOTR_CAMP_ENTRY_WORK;
        for (k = 0; k < 3; k++) {
            p->pick[k] = (DeckTab_campentry[w->hash] >> (k * 5)) & 0x1f;
            p->deck[k] = data_00299d00_campentry[p->pick[k]].id & 0x3ff;
        }
        SzModel_LoadMerge(3, 4, 0);
        DOTR_CAMP_ENTRY_MODULES();
        board->side = 0;
        board->done0 = 0;
        board->done1 = 0;
        DOTR_CAMP_ENTRY_WORK->all = 0;
        DOTR_CAMP_ENTRY_CLEAR_SEEN();
        board->ticks = 10050;
        board->started = 1;
        func_00233f50_campentry(2);
        func_00217980_campentry(25, 1);
        break;
    case 10:
        DOTR_CAMP_ENTRY_WORK->hash = 0;
        p = DOTR_CAMP_ENTRY_WORK;
        for (k = 0; k < 3; k++) {
            p->pick[k] = (DeckTab_campentry[w->hash] >> (k * 5)) & 0x1f;
            p->deck[k] = data_00299d00_campentry[p->pick[k]].id & 0x3ff;
        }
        SzModel_LoadMerge(3, 4, 0);
        DOTR_CAMP_ENTRY_MODULES();
        board->side = 0;
        board->done0 = 0;
        board->done1 = 0;
        DOTR_CAMP_ENTRY_WORK->all = 0;
        DOTR_CAMP_ENTRY_CLEAR_SEEN();
        board->ticks = 10070;
        board->started = 1;
        func_00233f50_campentry(2);
        func_00217980_campentry(25, 1);
        break;
    }
}

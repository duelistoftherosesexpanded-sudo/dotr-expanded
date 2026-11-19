#include "dotr/ee/memcard_proc.h"

/* MATCHING DEVICE: the end sequence and the step back in exactly these statement orders. */
#define FINISH g->state = 0; g->msg = -1; done = 1;
#define BACK old = g->state = 1; g->second = 1;
#define TOGGLE \
    sec = g->second; \
    if (SzPad_Check(0, 0, 0x8000)) { \
        g->second = 1; \
    } else { \
        g->second = 0; \
    } \
    if (g->second != sec) { \
        SzSound_PlaySE(0, 3); \
    }

int SzGame_ActMemCardProc(void)
{
    struct DotrMemCardProcView *g = (struct DotrMemCardProcView *)gSzGame_gl_mcbytes;
    int old;
    int done;
    int op;
    int msg;
    int sec;

    /* MATCHING DEVICE: the first test reads the block through its structure symbol, not
     * through g, and the whole body is inside it; the four locals are then assigned (not
     * initialised) in this order, and declared in the order above. */
    if (gSzGame_gl_mcview.state != 0) {
    old = g->state;
    op = 5;
    done = 0;
    msg = 0;
    if (SzFile_InqRequestEx()) {
        return 0;
    }
    switch (g->kind) {
    case 0:
        switch (g->state) {
        case 1:
            g->state = 2;
            /* fall through */
        case 2:
        case 3:
        case 4:
            g->res = TkMc_Load_Main(g->state - 2);
            msg = lSzGame_GetMCardMsg(g->kind, g->res, &g->msg);
            g->state = 2;
            switch (g->res & 0xff00) {
            case 0:
                if (msg) {
                    if (SzPad_Check(0, 0, 240)) {
                        SzSound_PlaySE(0, 4);
                        FINISH
                    }
                }
                break;
            case 0x100:
                g->state = 5;
                break;
            case 0x700:
                g->second = 0;
                g->state = 3;
                break;
            }
            break;
        case 5:
            msg = 1;
            if (SzPad_Check(0, 0, 240)) {
                SzSound_PlaySE(0, 4);
                g->state = 0;
                g->msg = -1;
            }
            break;
        }
        break;
    case 1:
    case 2:
        switch (g->state) {
        case 1:
            if (g->kind == 1) {
                TkMc_Save_Main(1);
                g->state = 2;
                break;
            }
            msg = 2;
            g->msg = 2505;
            if (SzPad_Check(0, 0, 0xa000)) {
                TOGGLE
            } else if (SzPad_Check(0, 0, 192)) {
                SzSound_PlaySE(0, 4);
                if (g->second) {
                    TkMc_Save_Main(1);
                    g->state = 2;
                } else {
                    FINISH
                }
            } else if (SzPad_Check(0, 0, 32)) {
                SzSound_PlaySE(0, 5);
                FINISH
            }
            break;
        case 2:
        case 3:
        case 4:
            g->res = TkMc_Save_Main(g->state - 2);
            msg = lSzGame_GetMCardMsg(g->kind, g->res, &g->msg);
            g->state = 2;
            switch (g->res & 0xff00) {
            case 0:
                if (msg) {
                    if (msg == 2) {
                        if (SzPad_Check(0, 0, 0xa000)) {
                            TOGGLE
                        } else if (SzPad_Check(0, 0, 192)) {
                            SzSound_PlaySE(0, 4);
                            if (g->second) {
                                g->state = 4;
                            } else if (g->kind == 1) {
                                FINISH
                            } else {
                                BACK
                            }
                        } else if (SzPad_Check(0, 0, 32)) {
                            SzSound_PlaySE(0, 5);
                            if (g->kind == 1) {
                                FINISH
                            } else {
                                BACK
                            }
                        }
                    } else if (msg == 3) {
                        if (SzPad_Check(0, 0, 240)) {
                            SzSound_PlaySE(0, 4);
                            g->state = 4;
                        }
                    } else {
                        if (SzPad_Check(0, 0, 240)) {
                            SzSound_PlaySE(0, 4);
                            if (g->kind == 1) {
                                FINISH
                            } else {
                                BACK
                            }
                        }
                    }
                }
                break;
            case 0x100:
                g->state = 5;
                break;
            case 0x700:
                g->second = 0;
                g->state = 3;
                break;
            }
            break;
        case 5:
            msg = 1;
            if (SzPad_Check(0, 0, 240)) {
                SzSound_PlaySE(0, 4);
                g->state = 0;
                g->msg = -1;
            }
            break;
        }
        break;
    case 3:
        switch (g->state) {
        case 1:
            g->state = 2;
            /* fall through */
        case 2:
        case 3:
        case 4:
            g->res = TkMc_Custom_Main(g->state - 2);
            msg = lSzGame_GetMCardMsg(g->kind, g->res, &g->msg);
            g->state = 2;
            switch (g->res & 0xff00) {
            case 0:
                if (msg) {
                    if (SzPad_Check(0, 0, 240)) {
                        SzSound_PlaySE(0, 4);
                        FINISH
                    }
                }
                break;
            case 0x100:
            case 0x200:
            case 0x300:
            case 0x400:
            case 0x500:
                g->state = 5;
                break;
            case 0x700:
                g->second = 0;
                g->state = 3;
                break;
            }
            break;
        case 5:
            msg = 1;
            if (SzPad_Check(0, 0, 240)) {
                SzSound_PlaySE(0, 4);
                g->state = 0;
                g->msg = -1;
            }
            break;
        }
        break;
    case 4:
        switch (g->state) {
        case 1:
            g->state = 2;
            /* fall through */
        case 2:
        case 3:
        case 4:
            g->res = TkMc_Trade_Load_Main(g->state - 2);
            msg = lSzGame_GetMCardMsg(g->kind, g->res, &g->msg);
            g->state = 2;
            switch (g->res & 0xff00) {
            case 0:
                if (msg) {
                    if (SzPad_Check(0, 0, 240) || SzPad_Check(1, 0, 240)) {
                        SzSound_PlaySE(0, 4);
                        FINISH
                    }
                }
                break;
            case 0x100:
                g->state = 5;
                break;
            case 0x700:
                g->second = 0;
                g->state = 3;
                break;
            }
            break;
        case 5:
            msg = 1;
            if (SzPad_Check(0, 0, 240) || SzPad_Check(1, 0, 240)) {
                SzSound_PlaySE(0, 4);
                g->state = 0;
                g->msg = -1;
            }
            break;
        }
        break;
    case 5:
        switch (g->state) {
        case 1:
            g->state = 2;
            /* fall through */
        case 2:
        case 3:
        case 4:
            g->res = TkMc_Trade_Save_Main(g->state - 2);
            msg = lSzGame_GetMCardMsg(g->kind, g->res, &g->msg);
            g->state = 2;
            switch (g->res & 0xff00) {
            case 0:
                if (msg) {
                    msg = 1;        /* MATCHING DEVICE: stored inside the test, before the pad is asked */
                    if (SzPad_Check(0, 0, 240) || SzPad_Check(1, 0, 240)) {
                        SzSound_PlaySE(0, 4);
                        FINISH
                    }
                }
                break;
            case 0x100:
            case 0x600:
                g->state = 5;
                break;
            case 0x700:
                g->second = 0;
                g->state = 3;
                break;
            }
            break;
        case 5:
            msg = 1;
            if (SzPad_Check(0, 0, 240) || SzPad_Check(1, 0, 240)) {
                SzSound_PlaySE(0, 4);
                g->state = 0;
                g->msg = -1;
            }
            break;
        }
        break;
    case 6:
        switch (g->state) {
        case 1:
            g->state = 2;
            /* fall through */
        case 2:
        case 3:
        case 4:
            g->res = TkMc_Check_Main(g->state - 2);
            msg = lSzGame_GetMCardMsg(g->kind, g->res, &g->msg);
            if (g->msg == 2482) {
                g->msg = -1;
            }
            g->state = 2;
            switch (g->res & 0xff00) {
            case 0:
                if (msg) {
                    if (SzPad_Check(0, 0, 240)) {
                        SzSound_PlaySE(0, 4);
                        FINISH
                    }
                }
                break;
            case 0x100:
                g->state = 0;
                g->msg = -1;
                break;
            case 0x700:
                g->second = 0;
                g->state = 3;
                break;
            }
            break;
        }
        break;
    }
    if (g->msg < 0) {
        SzDuel_CloseMsgPopup();
        return done ? 2 : 1;
    }
    /* MATCHING DEVICE: 1 and 3 listed before 2. */
    switch (msg) {
    case 1:
    case 3:
        op = 6;
        break;
    case 2:
        if (g->second) {
            g->msg |= 0x8000;
        } else {
            g->msg &= ~0x8000;
        }
        if (g->state != old) {
            g->second = 0;
        }
        op = 7;
        break;
    }
    SzDuel_OpenMsgPopup(op, 0, 0, g->msg);
    return 0;
    }
    return -1;
}

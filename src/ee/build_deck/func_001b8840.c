#include "dotr/ee/build_deck.h"

#define DOTR_BUILD_MAX(a, b) ((a) > (b) ? (a) : (b))
#define DOTR_BUILD_MIN(a, b) ((a) < (b) ? (a) : (b))
#define DOTR_BUILD_CLAMP(v, lo, hi) DOTR_BUILD_MIN(DOTR_BUILD_MAX(v, lo), hi)

void SzDuel_ActBuildDeck(void)
{
    struct DotrBuildDeckControl *c;
    int side;
    int deck;
    int inDeck;
    int dBag;
    int dDeck;
    int id;
    int id2;
    int at;
    int old;
    int old2;
    int old3;
    int d;
    int ok;
    int hold;
    int r;
    int i;
    int v;
    int n;
    unsigned char *p;
    int n2;

    /* Which variable holds which value below (r for the deck cursor's old position, v for the
     * bag sort key's old value, id2 and n2 for the deck list) and the order of the
     * declarations are MATCHING DEVICES: they decide the saved registers and stack slots. */
    c = &DOTR_BUILD_DECK;
    side = c->side;
    deck = SzData_GetCurrentDeck(side);
    inDeck = (c->mode == 2 || c->mode == 5) ? 1 : 0;
    dBag = 0;
    dDeck = 0;
    if (SzFile_InqRequestEx() == 0 && c->reload) {
        SzText_LoadMerge(side + 5, 1);
        c->reload = 0;
    }
    {
        struct DotrBuildDeckControl *p = &DOTR_BUILD_DECK;
        int x;

        if (p->fadeOn) {
            p->dirA = 1;
        }
        x = p->fadeA + p->dirA * 0x200;
        p->fadeA = DOTR_BUILD_CLAMP(x, 0, 0x8000);
        if (p->fadeA == 0 || p->fadeA >= 0x8000) {
            p->dirA = -p->dirA;
        }
    }
    {
        struct DotrBuildDeckControl *p = &DOTR_BUILD_DECK;
        int x;

        x = p->dirB * 0x800;
        p->fadeB = DOTR_BUILD_CLAMP(p->fadeB + x, 0, 0x8000);
        if (p->fadeB == 0 || p->fadeB >= 0x8000) {
            p->dirB = -p->dirB;
        }
    }
    if (c->slide != c->slideTo) {
        c->slide += DOTR_BUILD_CLAMP(c->slideTo - c->slide, -45, 45);
        return;
    }
    if (SzPad_Check(0, 1, 0xffff)) {
        c->idle = 90;
    } else if (c->idle) {
        c->idle--;
    }
    switch (c->mode) {
    case 0:
        if (SzPad_Check(0, 0, 0x20)) {
            c->mode = 1;
            c->slideTo = 320;
            SzSound_PlaySE(0, 17);
        } else if (SzPad_Check(0, 2, 0xf00c)) {
            old = c->gridTop;
            old2 = c->gridCol;
            old3 = c->gridRow;
            d = 0;
            if (SzPad_Check(0, 2, 0xc)) {
                if (SzPad_Check(0, 2, 4)) {
                    d = -10;
                } else {
                    d = 10;
                }
            } else if (SzPad_Check(0, 2, 0x5000)) {
                if (SzPad_Check(0, 2, 0x1000)) {
                    if (c->gridRow > 0) {
                        c->gridRow--;
                    } else {
                        d = -1;
                    }
                } else {
                    if (c->gridRow < 9) {
                        c->gridRow++;
                    } else {
                        d = 1;
                    }
                }
            }
            if (d) {
                c->gridTop = DOTR_BUILD_CLAMP(c->gridTop + d, 0, 76);
            }
            if (SzPad_Check(0, 2, 0xa000)) {
                if (SzPad_Check(0, 2, 0x8000)) {
                    c->gridCol = (c->gridCol + 9) % 10;
                } else {
                    c->gridCol = (c->gridCol + 11) % 10;
                }
            }
            if (c->gridCol != old2 || c->gridRow != old3 || c->gridTop != old) {
                SzSound_PlaySE(0, 3);
            }
        } else if (SzPad_Check(0, 2, 3)) {
            old = c->tabA;
            if (SzPad_Check(0, 2, 1)) {
                c->tabA = (c->tabA + 3) % 4;
            } else {
                c->tabA = (c->tabA + 5) % 4;
            }
            if (c->tabA != old) {
                SzSound_PlaySE(0, 19);
            }
        } else if (SzPad_Check(0, 0, 0xd0)) {
            id = c->gridCol + (c->gridTop + c->gridRow) * 10;
            ok = 0;
            if (id == 671) {
                id = 999;
            }
            if (id < 683) {
                if (SzData_GetBagCount(side, id, -1) > 0) {
                    ok = 1;
                }
                if (SzData_GetLookFlag(side, id)) {
                    ok = 1;
                }
            }
            if (ok) {
                SzDuel_Init3DView(id);
                SzGame_Push(7, 2);
                SzSound_PlaySE(0, 4);
            } else {
                SzSound_PlaySE(0, 6);
            }
        }
        break;
    case 1:
        if (SzPad_Check(0, 0, 0x800)) {
            c->mode = 0;
            c->slideTo = 640;
            SzSound_PlaySE(0, 16);
        } else if (SzPad_Check(0, 2, 0x5000)) {
            old = c->tabA;
            if (SzPad_Check(0, 2, 0x1000)) {
                c->tabA = (c->tabA + 3) % 4;
            } else {
                c->tabA = (c->tabA + 5) % 4;
            }
            if (c->tabA != old) {
                SzSound_PlaySE(0, 19);
            }
            SzPad_ResetCounter(0, 0xa000);
        } else if (SzPad_Check(0, 2, 0x2000)) {
            c->mode = 2;
            SzSound_PlaySE(0, 18);
        } else if (SzPad_Check(0, 2, 0x20)) {
            c->mode = 2;
            SzSound_PlaySE(0, 17);
        }
        break;
    case 2:
    case 3:
        id = c->bag[c->bagTop + c->bagCur];
        at = SzData_CheckDeck(side, deck, id);
        hold = SzPad_Check(0, 1, 0x100) ? 1 : 0;
        old = c->bagCur;
        if (SzPad_Check(0, 0, 0x200)) {
            n = SzData_GetBagCount(side, id, 3);
            if (SzFile_InqRequestEx()) {
                n = 0;
            }
            if (Taku_Get_Duel5_flg() < 5) {
                n = 0;
            }
            if (lSzDuel_IsDeckReady(side, 0) == 0 && lSzDuel_IsDeckReady(side, 1) == 0 && lSzDuel_IsDeckReady(side, 2) == 0) {
                n = 0;
            }
            if (n > 0) {
                c->sel = 0;
                c->prevMode = c->mode;
                c->mode = 7;
                SzSound_PlaySE(0, 16);
            } else {
                SzSound_PlaySE(0, 6);
            }
        } else if (SzPad_Check(0, 0, 0x400)) {
            for (i = 0; i < 8; i++) {
                c->pass[i] = '0';
            }
            c->sel = 0;
            c->prevMode = c->mode;
            c->mode = 9;
            SzSound_PlaySE(0, 16);
        } else {
            if (SzPad_Check(0, 2, 0xc)) {
                if (SzPad_Check(0, 2, 4)) {
                    dBag = -5;
                } else {
                    dBag = 5;
                }
            } else if (SzPad_Check(0, 2, 0x5000)) {
                if (SzPad_Check(0, 2, 0x1000)) {
                    if (c->bagCur > 0) {
                        c->bagCur--;
                    } else {
                        dBag = -1;
                    }
                } else {
                    if (c->bagCur < 4) {
                        c->bagCur++;
                    } else {
                        dBag = 1;
                    }
                }
                SzPad_ResetCounter(0, 0xa000);
            }
            if (c->bagCur != old) {
                SzSound_PlaySE(0, 3);
            } else if (dBag == 0) {
                if (SzPad_Check(0, 0, 0x800)) {
                    if (SzData_GetRank(side, id) > 0) {
                        if (SzData_GetBagCount(side, id, 3) > 0) {
                            if (c->deck[0] != 999) {
                                SzData_PullDeckLeader(side, deck);
                                c->deck[0] = 999;
                            }
                            if (SzData_PushDeckLeader(side, deck, id)) {
                                c->deck[0] = id;
                                lSzDuel_ReqGhostCard(id, c->bagCur, 1, 0, 0);
                                SzText_Free(2, side);
                                c->reload = 1;
                            }
                            SzSound_PlaySE(0, 4);
                        } else {
                            SzSound_PlaySE(0, 6);
                        }
                    } else {
                        SzSound_PlaySE(0, 6);
                    }
                } else if (SzPad_Check(0, 2, 0xc0)) {
                    if (SzPad_Check(0, 2, 0x80)) {
                        if (SzData_PullDeck(side, deck, at)) {
                            {
                                int j;

                                for (j = 0; j < 40; j++) {
                                    if (c->deck[j + 1] == id) {
                                        c->deck[j + 1] = 999;
                                        break;
                                    }
                                }
                            }
                            lSzDuel_SortDeck();
                            lSzDuel_ReqGhostCard(id, c->bagCur, 0, 0, 1);
                            SzSound_PlaySE(0, 4);
                        } else {
                            SzSound_PlaySE(0, 6);
                        }
                    } else {
                        if (SzData_PushDeck(side, deck, id)) {
                            c->deck[c->deckCount - 1] = id;
                            lSzDuel_SortDeck();
                            lSzDuel_ReqGhostCard(id, c->bagCur, 1, 0, 0);
                            SzSound_PlaySE(0, 4);
                        } else {
                            SzSound_PlaySE(0, 6);
                        }
                    }
                } else if (SzPad_Check(0, 2, 3)) {
                    r = c->bagSort[hold];
                    p = &c->bagSort[hold];
                    if (SzPad_Check(0, 2, 1)) {
                        *p = (*p + 9) % 10;
                    } else {
                        *p = (*p + 11) % 10;
                    }
                    if (*p != r) {
                        lSzDuel_SortBag();
                        SzSound_PlaySE(0, 19);
                    }
                } else if (SzPad_Check(0, 0, 0x20)) {
                    if (inDeck) {
                        c->mode = 3;
                        c->slideTo = 0;
                        SzSound_PlaySE(0, 17);
                    } else if (lSzDuel_IsDeckReady(side, deck)) {
                        SzSound_PlaySE(0, 5);
                        SzGame_Pop();
                    } else {
                        SzSound_PlaySE(0, 6);
                        c->prevMode = c->mode;
                        c->mode = 13;
                    }
                } else if (SzPad_Check(0, 0, 0x10)) {
                    if (inDeck) {
                        ok = 0;
                        if (id < 683) {
                            if (SzData_GetBagCount(side, id, -1) > 0) {
                                ok = 1;
                            }
                            if (SzData_GetLookFlag(side, id)) {
                                ok = 1;
                            }
                        }
                        if (ok) {
                            SzDuel_Init3DView(id);
                            SzGame_Push(7, 2);
                            SzSound_PlaySE(0, 4);
                        } else {
                            SzSound_PlaySE(0, 6);
                        }
                    } else {
                        c->mode = 2;
                        c->slideTo = 320;
                        SzSound_PlaySE(0, 16);
                    }
                } else if (SzPad_Check(0, 2, 0xa000)) {
                    if (SzPad_Check(0, 2, 0x8000)) {
                        if (inDeck) {
                            c->mode = 1;
                        } else {
                            c->mode = 2;
                            c->slideTo = 320;
                        }
                        SzSound_PlaySE(0, 18);
                    } else {
                        if (inDeck) {
                            c->mode = 3;
                            c->slideTo = 0;
                        } else {
                            c->mode = 4;
                            c->deckCur = c->bagCur;
                        }
                        SzSound_PlaySE(0, 18);
                    }
                }
            }
        }
        break;
    case 4:
    case 5:
        n2 = c->deckTop + c->deckCur;
        id2 = c->deck[n2];
        at = SzData_CheckDeck(side, deck, id2);
        hold = SzPad_Check(0, 1, 0x100) ? 1 : 0;
        r = c->deckCur;
        if (SzPad_Check(0, 2, 0xc)) {
            if (SzPad_Check(0, 2, 4)) {
                dDeck = -5;
            } else {
                dDeck = 5;
            }
        } else if (SzPad_Check(0, 2, 0x5000)) {
            if (SzPad_Check(0, 2, 0x1000)) {
                if (c->deckCur > 0) {
                    c->deckCur--;
                } else {
                    dDeck = -1;
                }
            } else {
                if (c->deckCur < 4) {
                    c->deckCur++;
                } else {
                    dDeck = 1;
                }
            }
            SzPad_ResetCounter(0, 0xa000);
        }
        if (c->deckCur != r) {
            SzSound_PlaySE(0, 3);
        } else if (dDeck == 0) {
            if (SzPad_Check(0, 0, 0x800)) {
                int j;
                unsigned short *q;
                int d2;
                int s2;
                struct DotrBuildDeckControl *p;

                SzData_SetCurrentDeck(side, (deck + 1) % 3);
                p = &DOTR_BUILD_DECK;
                s2 = p->side;
                d2 = SzData_GetCurrentDeck(s2);
                q = p->deck;
                p->deckCount = 0;
                *q = SzData_GetLeader(s2, d2);
                p->deckCount++;
                q++;
                for (j = 0; j < 40; j++) {
                    *q = SzData_GetCardNo(s2, d2, j);
                    q++;
                    p->deckCount++;
                }
                lSzDuel_SortDeck();
                SzText_Free(2, side);
                c->reload = 1;
                SzSound_PlaySE(0, 9);
            } else if (SzPad_Check(0, 2, 0xc0)) {
                if (n2 > 0) {
                    if (id2 == 999) {
                        SzSound_PlaySE(0, 6);
                    } else if (SzPad_Check(0, 2, 0x80)) {
                        if (SzData_PullDeck(side, deck, at)) {
                            c->deck[n2] = 999;
                            lSzDuel_SortDeck();
                            lSzDuel_ReqGhostCard(id2, c->deckCur, 0, 0, 1);
                            SzSound_PlaySE(0, 4);
                        } else {
                            SzSound_PlaySE(0, 6);
                        }
                    } else {
                        if (SzData_PushDeck(side, deck, id2)) {
                            c->deck[c->deckCount - 1] = id2;
                            lSzDuel_SortDeck();
                            lSzDuel_ReqGhostCard(id2, c->deckCur, 1, 0, 0);
                            SzSound_PlaySE(0, 4);
                        } else {
                            SzSound_PlaySE(0, 6);
                        }
                    }
                } else if (SzPad_Check(0, 2, 0x80)) {
                    if (c->deck[0] != 999) {
                        SzData_PullDeckLeader(side, deck);
                        c->deck[0] = 999;
                        lSzDuel_ReqGhostCard(id2, c->deckCur, 0, 1, 0);
                        SzText_Free(2, side);
                        c->reload = 1;
                        SzSound_PlaySE(0, 4);
                    } else {
                        SzSound_PlaySE(0, 6);
                    }
                }
            } else if (SzPad_Check(0, 2, 3)) {
                r = c->deckSort[hold];
                p = &c->deckSort[hold];
                if (SzPad_Check(0, 2, 1)) {
                    *p = (*p + 9) % 10;
                } else {
                    *p = (*p + 11) % 10;
                }
                if (*p != r) {
                    lSzDuel_SortDeck();
                    SzSound_PlaySE(0, 19);
                }
            } else if (SzPad_Check(0, 0, 0x20)) {
                if (inDeck) {
                    c->mode = 4;
                    c->slideTo = 0;
                    SzSound_PlaySE(0, 17);
                } else if (lSzDuel_IsDeckReady(side, deck)) {
                    SzSound_PlaySE(0, 5);
                    SzGame_Pop();
                } else {
                    SzSound_PlaySE(0, 6);
                    c->prevMode = c->mode;
                    c->mode = 13;
                }
            } else if (SzPad_Check(0, 0, 0x10)) {
                if (inDeck) {
                    ok = 0;
                    if (id2 < 683) {
                        if (SzData_GetBagCount(side, id2, -1) > 0) {
                            ok = 1;
                        }
                        if (SzData_GetLookFlag(side, id2)) {
                            ok = 1;
                        }
                    }
                    if (ok) {
                        SzDuel_Init3DView(id2);
                        SzGame_Push(7, 2);
                        SzSound_PlaySE(0, 4);
                    } else {
                        SzSound_PlaySE(0, 6);
                    }
                } else {
                    c->mode = 5;
                    c->slideTo = -320;
                    SzSound_PlaySE(0, 16);
                }
            } else if (SzPad_Check(0, 2, 0xa000)) {
                if (SzPad_Check(0, 2, 0x8000)) {
                    if (inDeck) {
                        c->mode = 4;
                        c->slideTo = 0;
                    } else {
                        c->mode = 3;
                        c->bagCur = c->deckCur;
                    }
                    SzSound_PlaySE(0, 18);
                } else {
                    if (inDeck) {
                        c->mode = 6;
                    } else {
                        c->mode = 5;
                        c->slideTo = -320;
                    }
                    SzSound_PlaySE(0, 18);
                }
            }
        }
        break;
    case 6:
        if (SzPad_Check(0, 2, 0x5000)) {
            old = c->tabB;
            if (SzPad_Check(0, 2, 0x1000)) {
                c->tabB = (c->tabB + 3) % 4;
            } else {
                c->tabB = (c->tabB + 5) % 4;
            }
            if (c->tabB != old) {
                SzSound_PlaySE(0, 19);
            }
            SzPad_ResetCounter(0, 0xa000);
        } else if (SzPad_Check(0, 2, 0x8000)) {
            c->mode = 5;
            SzSound_PlaySE(0, 18);
        } else if (SzPad_Check(0, 2, 0x20)) {
            c->mode = 5;
            SzSound_PlaySE(0, 17);
        }
        break;
    case 7:
        if (SzPad_Check(0, 0, 0xa000)) {
            old = c->sel;
            if (SzPad_Check(0, 0, 0x8000)) {
                c->sel = 1;
            } else {
                c->sel = 0;
            }
            if (c->sel != old) {
                SzSound_PlaySE(0, 3);
            }
        } else if (SzPad_Check(0, 0, 0xc0)) {
            if (c->sel) {
                int rid;
                unsigned short *rc;

                rid = c->bag[c->bagTop + c->bagCur];
                rc = (unsigned short *)(c->work + 0x20);
                rc[0] = Ta_GetRecycle(side, rid);
                rc[1] = Ta_GetRecycle(side, rid);
                rc[2] = Ta_GetRecycle(side, rid);
                SzData_DelBag(side, rid);
                c->step = 0;
                c->sel = 0;
                c->mode = 8;
            } else {
                c->mode = c->prevMode;
            }
            SzSound_PlaySE(0, 4);
        } else if (SzPad_Check(0, 0, 0x20)) {
            c->mode = c->prevMode;
            SzSound_PlaySE(0, 17);
        }
        break;
    case 8:
        if (c->step < 3) {
            if (c->sel < 0x80) {
                if (c->sel == 0x7f) {
                    SzData_AddBag(side, 0, ((unsigned short *)(c->work + 0x20))[c->step]);
                    lSzDuel_ReBuildBag();
                    SzSound_PlaySE(0, 7);
                }
                c->sel++;
            } else if (SzPad_Check(0, 0, 0xf0)) {
                SzSound_PlaySE(0, 4);
                c->step++;
                if (c->step < 3) {
                    c->sel = 0;
                }
            }
        } else if (SzPad_Check(0, 0, 0xf0)) {
            Taku_Set_Duel5_flg();
            c->mode = c->prevMode;
            SzSound_PlaySE(0, 4);
        }
        break;
    case 9:
        if (SzPad_Check(0, 2, 0xa000)) {
            if (SzPad_Check(0, 2, 0x8000)) {
                c->sel = (c->sel + 7) % 8;
            } else {
                c->sel = (c->sel + 9) % 8;
            }
            SzSound_PlaySE(0, 3);
        } else if (SzPad_Check(0, 2, 0x5000)) {
            v = (unsigned char)c->pass[c->sel];
            if (v >= 'a') {
                n = v - 87;
            } else {
                n = v - 48;
            }
            if (SzPad_Check(0, 2, 0x1000)) {
                v = (n + 37) % 36;
            } else {
                v = (n + 35) % 36;
            }
            if (v >= 10) {
                c->pass[c->sel] = v + 87;
            } else {
                c->pass[c->sel] = v + 48;
            }
            SzSound_PlaySE(0, 3);
        } else if (SzPad_Check(0, 0, 0xc0)) {
            r = lSzDuel_HitPassword(c->pass);
            if (r >= 0) {
                if (SzData_GetPassFlag(side, r)) {
                    c->mode = 12;
                } else {
                    *(short *)(c->work + 0x20) = r;
                    c->mode = 10;
                    SzData_SetPassFlag(side, r, 1);
                    SzData_AddBag(side, 0, r);
                    lSzDuel_ReBuildBag();
                }
                SzSound_PlaySE(0, 4);
            } else {
                c->mode = 11;
                SzSound_PlaySE(0, 6);
            }
        } else if (SzPad_Check(0, 0, 0x20)) {
            c->mode = c->prevMode;
            SzSound_PlaySE(0, 17);
        }
        break;
    case 10:
    case 11:
    case 12:
    case 13:
        if (SzPad_Check(0, 0, 0xf0)) {
            c->mode = c->prevMode;
            SzSound_PlaySE(0, 4);
        }
        break;
    }
    if (SzPad_IsAnalog(0)) {
        if (!(c->mode != 1 && c->mode != 2 && c->mode != 3 && c->mode != 4) && dBag == 0) {
            v = SzPad_GetAnalogInfo(0, 3) & 0xff;
            d = v - 0x80;
            if ((d >= 0 ? d : -d) > 0x30) {
                if (d > 0) {
                    d -= 0x30;
                } else {
                    d += 0x30;
                }
                dBag = d * 15 >> 7;
            }
        }
        if (!(c->mode != 3 && c->mode != 4 && c->mode != 5 && c->mode != 6) && dDeck == 0) {
            v = SzPad_GetAnalogInfo(0, 1) & 0xff;
            d = v - 0x80;
            if ((d >= 0 ? d : -d) > 0x30) {
                if (d > 0) {
                    d -= 0x30;
                } else {
                    d += 0x30;
                }
                dDeck = d * 5 >> 7;
            }
        }
    }
    if (dBag) {
        n = DOTR_BUILD_MAX(c->bagCount - 5, 0);
        old = c->bagTop;
        v = old + dBag;
        c->bagTop = DOTR_BUILD_CLAMP(v, 0, n);
        if (c->bagTop != old) {
            SzSound_PlaySE(0, 3);
        }
        c->idle = 90;
    }
    if (dDeck) {
        n = DOTR_BUILD_MAX(c->deckCount - 5, 0);
        old = c->deckTop;
        v = old + dDeck;
        c->deckTop = DOTR_BUILD_CLAMP(v, 0, n);
        if (c->deckTop != old) {
            SzSound_PlaySE(0, 3);
        }
        c->idle = 90;
    }
}

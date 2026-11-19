#include "dotr/ee/duel_init.h"

/* SzDuel_Init (0x1add00): resets both players' deck, grave and hand; on a fresh start reads the
 * current decks and leaders, clears the request and control lists and the 11 units of each
 * side, places the two leaders at (3, 6) and (3, 0), fills the 40 deck positions (SzDuel_AddDeck
 * when menu number 3 or 4 is 1000 or more, else SzDuel_SetDeck), copies the three options of
 * `side` to the other side, sets the camera from the two vectors at 0x2910c0 / 0x2910d0 with
 * the look angle pi/2 or 3*pi/2, and sets all 49 field rates to -128; otherwise sets them to 0.
 * Then sets up the map, runs lSzDuel_InitCom and resets the two side records and the control
 * block. Views and assumed names: duel_init.h. */
void SzDuel_Init(int map, int side, int fresh)
{
    struct DotrInitControl *g = (struct DotrInitControl *)&data_00317490_window;
    struct DotrInitSide *p;
    int deck0;
    int deck1;
    int leader0;
    int leader1;
    int card0;
    int card1;
    int i;
    int j;
    /* MATCHING DEVICE: the cleared flags are assigned from this zero variable; with a literal 0
     * the compiler does not build the shared `0 & 1` and its shifted copies. */
    int off = 0;

    SzDuel_InitDeck(0);
    SzDuel_InitGrave(0);
    SzDuel_InitHand(0);
    SzDuel_InitDeck(1);
    SzDuel_InitGrave(1);
    SzDuel_InitHand(1);
    if (fresh) {
        struct DotrInitVector cp;
        struct DotrInitVector look;
        int angle;

        deck0 = SzData_GetCurrentDeck(0);
        deck1 = SzData_GetCurrentDeck(1);
        leader0 = SzData_GetLeader(0, deck0);
        leader1 = SzData_GetLeader(1, deck1);
        SzDuel_InitCardRequest();
        SzDuel_InitCardControl();
        for (i = 0; i < 11; i++) {
            SzDuel_DelUnit(0, i);
            SzDuel_DelUnit(1, i);
        }
        SzDuel_InitMapColor(0, 0, 1);
        SzDuel_AddUnit(0, 3, 6, leader0, 1);
        SzDuel_AddUnit(1, 3, 0, leader1, 1);
        SzDuel_SetUnitCtlRateM(0, 0, 0, 0);
        SzDuel_SetUnitCtlModeM(0, 0, 7);
        SzDuel_SetUnitCtlRateM(1, 0, 0, 0);
        SzDuel_SetUnitCtlModeM(1, 0, 7);
        for (i = 0; i < 40; i++) {
            card0 = SzData_GetCardNo(0, deck0, i);
            card1 = SzData_GetCardNo(1, deck1, i);
            if (SzGame_GetMenuNum(3) >= 1000) {
                SzDuel_AddDeck(0, card0);
            } else {
                SzDuel_SetDeck(0, i, card0);
            }
            if (SzGame_GetMenuNum(4) >= 1000) {
                SzDuel_AddDeck(1, card1);
            } else {
                SzDuel_SetDeck(1, i, card1);
            }
        }
        for (i = 0; i < 3; i++) {
            SzData_SetOption(side ^ 1, i, SzData_GetOption(side, i));
        }
        angle = side != 0 ? 0xc000 : 16384;
        cp = data_002910c0;
        look = data_002910d0;
        look.y = 3.1415927f * (float)angle / 32768.0f;
        SzView_SetCP(0, &cp);
        SzView_SetLookSP(0, &look);
        for (i = 0; i < 7; i++) {
            for (j = 0; j < 7; j++) {
                SzDuel_SetFieldRate(j, i, -128);
            }
        }
        g->unknown_d34 = 0;
        SzSound_TransferSEGrp(1, 0, 0);
    } else {
        for (i = 0; i < 7; i++) {
            for (j = 0; j < 7; j++) {
                SzDuel_SetFieldRate(j, i, 0);
            }
        }
        g->unknown_d34 = 128;
    }
    g->map = map;
    SzDuel_SetupMap(g->map);
    lSzDuel_InitCom(fresh);
    TaLogic_InitLogicData(0, &g->sides[0]);
    g->sides[0].unknown_330 = -1;
    if (g->sides[0].memory_334) {
        SzMem_Free((void *)(DotrInitWord)g->sides[0].memory_334);
    }
    g->sides[0].memory_334 = 0;
    g->sides[0].unknown_338 = 0;
    g->sides[0].unknown_1d0 = 200;
    g->sides[0].unknown_1d4 = 16384;
    g->sides[0].unknown_1d8 = 8192;
    g->sides[0].unknown_341 = 4;
    if (map < 22) {
        g->sides[0].points = 4000;
        g->sides[0].unknown_342 = 3;
    }
    g->sides[0].unknown_343 = 0;
    g->sides[0].unknown_346_bit_0 = -1;
    g->sides[0].unknown_344 = 0;
    g->sides[0].unknown_345 = 40;
    g->sides[0].x = 3;
    g->sides[0].y = 6;
    p = &g->sides[1];
    TaLogic_InitLogicData(1, p);
    p->unknown_330 = -1;
    if (p->memory_334) {
        SzMem_Free((void *)(DotrInitWord)p->memory_334);
    }
    p->memory_334 = 0;
    p->unknown_338 = 0;
    p->unknown_1d0 = 200;
    p->unknown_1d4 = 0xc000;
    p->unknown_1d8 = 8192;
    p->unknown_341 = 4;
    if (map < 22) {
        p->points = 4000;
        p->unknown_342 = 3;
    }
    p->unknown_343 = 0;
    p->unknown_346_bit_0 = -1;
    p->unknown_344 = 0;
    p->unknown_345 = 40;
    p->x = 3;
    p->y = 0;
    g->unit_6c8 = 0;
    g->unit_6cc = 0;
    g->unknown_d35 = 0;
    g->unknown_d36 = 0;
    g->unknown_d38[0] = 0;
    g->unknown_d38[1] = 0;
    g->unknown_d38[2] = 0;
    g->unknown_d38[3] = 0;
    g->unknown_d50 = 2;
    g->unknown_d51 = 0;
    g->unknown_d52 = side;
    g->unknown_d54 = g->unknown_d52;
    g->unknown_d53 = 0;
    g->unknown_d55 = 255;
    g->unknown_d56 = 255;
    g->unknown_d57 = 0;
    g->unknown_d58 = 24;
    g->unknown_d5b = 0;
    g->unknown_d59 = 0;
    g->unknown_d5a = 0;
    g->side = g->unknown_d52;
    g->unknown_d3d = g->unknown_d53;
    g->unknown_d3e = 128;
    g->unknown_d3f[0] = 0;
    g->unknown_d3f[1] = 0;
    g->unknown_d3f[2] = 0;
    g->unknown_d3f[3] = 0;
    g->unknown_d3f[4] = 0;
    g->unknown_d3f[5] = 0;
    g->unknown_d3f[6] = 0;
    g->unknown_d3f[7] = 0;
    g->unknown_d3f[8] = 0;
    g->unknown_d3f[9] = 0;
    g->unknown_d3f[10] = 0;
    g->unknown_d3f[11] = 0;
    g->unknown_d3f[12] = 0;
    g->unknown_d3f[13] = 0;
    g->unknown_d3f[14] = 0;
    g->unknown_d3f[15] = 0;
    g->unknown_d3f[16] = 0;
    g->unknown_d63 = 0;
    g->unknown_c86 = 0;
    g->unknown_c88 = 0;
    g->unknown_d64_01 = 1;
    g->unknown_d64_23 = 1;
    g->unknown_c96 = 0;
    g->unknown_c98 = 0;
    for (i = 0; i < 4; i++) {
        g->unknown_cb2[i] = 24;
        g->unknown_caa[i] = 0;
        g->unknown_c9a[i].b = 0;
        g->unknown_c9a[i].a = 0;
    }
    g->unknown_c5c[0] = 0;
    g->unknown_c5c[1] = 0;
    g->unknown_c5c[2] = 0;
    g->unknown_c5c[3] = 0;
    g->unknown_c78 = -1;
    g->unknown_c5c[4] = 0;
    g->unknown_c5c[5] = 0;
    g->unknown_c5c[6] = 0;
    g->unknown_d64_6 = off;
    g->unknown_d64_7 = off;
    g->unknown_d65_0 = off;
    g->unknown_d65_1 = off;
    g->unknown_d65_2 = off;
    g->unknown_d65_3 = off;
    g->unknown_d65_4 = off;
    g->unknown_d65_5 = off;
    g->unknown_d65_6 = off;
    g->unknown_d65_7 = off;
    g->unknown_d66_0 = off;
    g->unknown_d66_1 = off;
    g->unknown_d66_2 = off;
    g->unknown_d66_3 = off;
    g->unknown_d66_4 = off;
    g->unknown_d66_5 = off;
    g->unknown_d66_6 = off;
    g->unknown_d66_7 = -1;
    g->unknown_d67_0 = g->unknown_d66_7;
    g->unknown_d67_1 = -1;
    g->unknown_d67_2 = off;
    g->unknown_d67_3 = off;
    g->unknown_d67_4 = off;
    g->unknown_d67_5 = off;
}

/* The mod's achievements (achievements.json, tools/dlc_achievements.py), built into the game.
 *
 * Counted: campaign duels won and played (DlcAch_Duel, from bosses.c's Taku_Camp_What_KATIMAKE
 * wrap), cards bought in the card shop (DlcAch_Bought, shop.c); looked up when checked: the new
 * podiums beaten (bosses.c's marks) and the different cards owned. DlcAch_Check, after each of
 * those and after a save is loaded, marks every achievement whose goal is reached, gives its card
 * (to the chest) and queues a pop-up, "ACHIEVEMENT UNLOCKED", which the campaign map and the card
 * shop draw (DlcAch_DrawPopup) for four seconds each. A sleeve named as a reward is locked until
 * then (shop.c). The counts and the earned marks go into the save (card_save.c, five words). */

#include "dlc_achievements.h"
#include "dlc_bosses.h"
#include "dlc_cards.h"

extern int SzData_GetBagCount(int side, int cardNo, int kind);
extern void SzData_AddBag(int side, int a, int cardNo);
extern int SzSound_PlaySE(int a, int number);
extern int SzDuel_DrawWindow_Font_Pipe(int cursor, int x, int y, int font, int flags, int count,
                                       const char *text, void *colour);
extern int lSzDuel_DrawWindow_ExBoard_Pipe(int cursor, int colour, int x, int y, int w, int h, int shade, int a7);
extern const char *DlcSleeves_Name(int k);

#define FRAME_COUNTER (*(volatile int *)0x317254)
#define POPUP_FRAMES 240
#define QUEUE 16

static unsigned int earned[2];          /* bit k % 30 of word k / 30 */
static unsigned int won, played, bought, coins;

/* Star chips for a duel: a win a tenth of the opponent's deck cost, rounded up (DC 1000: 100,
 * DC 1234: 124); a loss none. */
#define CHIPS_WIN_DIVISOR 10
#define COINS_MAX 999999

/* The last duel's result, for the results box on the map (DlcAch_DrawPopup): shown once. */
static int result_pending, result_won, result_cost, result_chips, result_since;
static unsigned char queue[QUEUE];
static int queued, shown_since;

int DlcAch_Earned(int k)
{
    return k >= 0 && k < 60 && (earned[k / 30] >> (k % 30) & 1);
}

static void earn(int k)
{
    const struct DlcAchievement *a = &dlc_achievements[k];

    earned[k / 30] |= 1u << (k % 30);
    if (a->card >= 0 && DlcCard_Valid(a->card)) {
        SzData_AddBag(0, 0, a->card);
    }
    if (queued < QUEUE) {
        queue[queued++] = (unsigned char)k;
    }
}

static int bosses_beaten(void)
{
    int k, n = 0;

    for (k = 0; k < dlc_boss_count; k++) {
        if (dlc_bosses[k].action == DLC_BOSS_DUEL && (DlcBosses_State(k / 15) >> (2 * (k % 15)) & 2)) {
            n++;
        }
    }
    return n;
}

static int cards_owned(void)
{
    int id, n = 0, end = DlcCard_End();

    for (id = 0; id < end; id++) {
        if (DlcCard_Valid(id) && SzData_GetBagCount(0, id, -1) > 0) {
            n++;
        }
    }
    return n;
}

/* Where achievement k's goal stands; owned and beaten are counted once per call of Check (-1: not
 * yet). */
static int progress(int k, int *owned, int *beaten)
{
    switch (dlc_achievements[k].kind) {
    case DLC_GOAL_DUELS_WON:
        return (int)won;
    case DLC_GOAL_DUELS:
        return (int)played;
    case DLC_GOAL_CARDS_BOUGHT:
        return (int)bought;
    case DLC_GOAL_BOSSES_BEATEN:
        if (*beaten < 0) {
            *beaten = bosses_beaten();
        }
        return *beaten;
    case DLC_GOAL_CARDS_OWNED:
        if (*owned < 0) {
            *owned = cards_owned();
        }
        return *owned;
    }
    return 0;
}

int DlcAch_Progress(int k)
{
    int owned = -1, beaten = -1;

    return progress(k, &owned, &beaten);
}

void DlcAch_Check(void)
{
    int k, owned = -1, beaten = -1;

    for (k = 0; k < dlc_achievement_count && k < 60; k++) {
        if (!DlcAch_Earned(k) && progress(k, &owned, &beaten) >= dlc_achievements[k].count) {
            earn(k);
        }
    }
}

int DlcAch_EarnedCount(void)
{
    int k, n = 0;

    for (k = 0; k < dlc_achievement_count && k < 60; k++) {
        n += DlcAch_Earned(k);
    }
    return n;
}

void DlcAch_Duel(int win, int deck_cost)
{
    int chips;

    if (deck_cost < 0) {
        deck_cost = 0;
    }
    /* A win: a tenth of the deck cost, rounded up; a loss pays nothing. */
    chips = win ? (deck_cost + CHIPS_WIN_DIVISOR - 1) / CHIPS_WIN_DIVISOR : 0;
    played++;
    if (win) {
        won++;
    }
    coins += chips;
    if (coins > COINS_MAX) {
        coins = COINS_MAX;
    }
    result_pending = 1;
    result_won = win != 0;
    result_cost = deck_cost;
    result_chips = chips;
    result_since = 0;
    DlcAch_Check();
}

int DlcAch_Coins(void)
{
    return (int)coins;
}

int DlcAch_Spend(int n)
{
    if (n < 0 || (unsigned int)n > coins) {
        return 0;
    }
    coins -= n;
    return 1;
}

void DlcAch_Bought(void)
{
    bought++;
    DlcAch_Check();
}

int DlcAch_ForSleeve(int sleeve)
{
    int k;

    for (k = 0; k < dlc_achievement_count && k < 60; k++) {
        if (dlc_achievements[k].sleeve == sleeve) {
            return k;
        }
    }
    return -1;
}

int DlcAch_SleeveOpen(int sleeve)
{
    int k = DlcAch_ForSleeve(sleeve);

    return sleeve == 0 || k < 0 || DlcAch_Earned(k);
}

/* ---------------------------------------------------------------------------- the save */

unsigned int DlcAch_Word(int w)
{
    switch (w) {
    case 0: return earned[0];
    case 1: return earned[1];
    case 2: return won;
    case 3: return played;
    case 4: return bought;
    default: return coins;
    }
}

void DlcAch_SetWord(int w, unsigned int v)
{
    v &= 0x3fffffff;
    switch (w) {
    case 0: earned[0] = v; break;
    case 1: earned[1] = v; break;
    case 2: won = v; break;
    case 3: played = v; break;
    case 4: bought = v; break;
    default: coins = v; break;
    }
}

void DlcAch_Reset(void)
{
    int w;

    for (w = 0; w < DLC_ACH_WORDS; w++) {
        DlcAch_SetWord(w, 0);
    }
    queued = 0;
    shown_since = 0;
}

/* --------------------------------------------------------------------------- the pop-up */

static int line(int cursor, int x, int y, int flags, const char *s)
{
    int n;

    for (n = 0; s[n] != 0; n++) {
    }
    (void)flags;
    /* 3: narrow spacing, centred on x (as shop.c finds them). */
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, 0, 3, n, s, 0);
}

int DlcAch_DrawPopupAt(int cursor, int mid);
static int put_number(char *s, int n);

#define RESULT_FRAMES 300
#define PAD_CROSS 0x40
extern int SzPad_Check(int pad, int kind, int mask);
extern int DlcButtons_Xbox(void);

/* Whether the results box is up: the map then takes no X (bosses.c), which closes the box. */
int DlcAch_ResultShowing(void)
{
    return result_pending && result_since != 0;
}

/* The last campaign duel's results, in the middle of the map for RESULT_FRAMES once the map shows
 * again, or until X (A) closes it: won or lost, the opponent's deck cost, the star chips it paid
 * and the total. Achievement pop-ups wait until it is gone. */
int DlcAch_DrawResults(int cursor)
{
    char s[32];
    int n;
    const char *t;

    if (!result_pending) {
        return cursor;
    }
    if (result_since == 0) {
        result_since = FRAME_COUNTER;
    } else if (FRAME_COUNTER - result_since > RESULT_FRAMES) {
        result_pending = 0;
        return cursor;
    } else if (SzPad_Check(0, 0, PAD_CROSS)) {
        result_pending = 0;
        SzSound_PlaySE(0, 4);
        return cursor;
    }
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, 180, 150, -280, -140, 2, 0);
    cursor = line(cursor, 320, 266, 0, DlcButtons_Xbox() ? "A  CLOSE" : "X  CLOSE");
    cursor = line(cursor, 320, 156, 3, "DUEL RESULTS");
    cursor = line(cursor, 320, 178, 3, result_won ? "VICTORY" : "DEFEAT");
    n = 0;
    for (t = "OPPONENT DC  "; *t; t++) s[n++] = *t;
    n += put_number(s + n, result_cost);
    s[n] = 0;
    cursor = line(cursor, 320, 202, 0, s);
    n = 0;
    for (t = "STAR CHIPS  +"; *t; t++) s[n++] = *t;
    n += put_number(s + n, result_chips);
    s[n] = 0;
    cursor = line(cursor, 320, 222, 3, s);
    n = 0;
    for (t = "TOTAL  "; *t; t++) s[n++] = *t;
    n += put_number(s + n, (int)coins);
    s[n] = 0;
    return line(cursor, 320, 244, 0, s);
}

int DlcAch_DrawPopup(int cursor)
{
    return DlcAch_DrawPopupAt(cursor, 320);
}

/* The oldest queued achievement, at the top of the screen, centred on mid (screen units), for
 * POPUP_FRAMES. */
int DlcAch_DrawPopupAt(int cursor, int mid)
{
    const struct DlcAchievement *a;
    int i;

    if (queued == 0 || result_pending) {
        return cursor;
    }
    if (shown_since == 0) {
        shown_since = FRAME_COUNTER;
        SzSound_PlaySE(0, 7);
    } else if (FRAME_COUNTER - shown_since > POPUP_FRAMES) {
        for (i = 1; i < queued; i++) {
            queue[i - 1] = queue[i];
        }
        queued--;
        shown_since = 0;
        return cursor;
    }
    a = &dlc_achievements[queue[0]];
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, mid - 140, 14, -280, -66, 2, 0);
    cursor = line(cursor, mid, 18, 3, "ACHIEVEMENT UNLOCKED");
    cursor = line(cursor, mid, 36, 3, a->name);
    if (a->sleeve >= 0) {
        cursor = line(cursor, mid, 54, 0, "NEW SLEEVE IN THE SHOP");
    } else if (a->card >= 0) {
        cursor = line(cursor, mid, 54, 0, "NEW CARD IN YOUR CHEST");
    } else {
        cursor = line(cursor, mid, 54, 0, a->desc);
    }
    return cursor;
}

/* ------------------------------------------------- the campaign map's TROPHIES (was HELP) window */

extern int SzPad_Check(int pad, int kind, int mask);
extern int DlcShop_DrawPicture(int cursor, const unsigned char *texture, int x, int y, int w, int h);
extern const unsigned char *const dlc_trophy_icon;

/* The help window's state (TkCamp_Act): 0 closed, 1 sliding in, 2 open, 3 X pressed (the game then
 * starts a tutorial duel, Taku_Camp_Go_Help), 10 sliding out; the slide step 0-8. */
#define HELP_OPEN (*(volatile signed char *)0x1ed543c)
#define HELP_STEP (*(volatile signed char *)0x1ed543d)
#define HELP_TEXT ((char *)0x30f113)          /* "HELP", the system panel's label for the icon */
#define PAD_UP 0x1000
#define PAD_DOWN 0x4000
#define ROWS 6
#define ROW_HEIGHT 52

static int scroll;

static int left(int cursor, int x, int y, const char *s)
{
    int n;

    for (n = 0; s[n] != 0; n++) {
    }
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, 1, 1, n, s, 0);
}

static int put_number(char *s, int n)
{
    char d[8];
    int k = 0, i = 0;

    do {
        d[k++] = '0' + n % 10;
        n /= 10;
    } while (n > 0 && k < 8);
    while (k > 0) {
        s[i++] = d[--k];
    }
    return i;
}

/* "a OF b" into s. */
static void of(char *s, int a, int b)
{
    int n = put_number(s, a);

    s[n++] = ' '; s[n++] = 'O'; s[n++] = 'F'; s[n++] = ' ';
    n += put_number(s + n, b);
    s[n] = 0;
}

/* Taku_Camp_Help_Window (hooked): the achievements, how each is earned, its reward and how far
 * along it is; UP / DOWN scroll, CIRCLE closes (the game's own handling). */
extern int DlcWs_DialogBegin(void);
extern void DlcWs_DialogEnd(int old);

static int help_window(int cursor);

int DlcAch_HelpWindow(int cursor)
{
    int old;

    if (HELP_OPEN == 0) {
        scroll = 0;
        return cursor;
    }
    /* One box about the screen's centre (widescreen.c), not pulled to the map's corner panels. */
    old = DlcWs_DialogBegin();
    cursor = help_window(cursor);
    DlcWs_DialogEnd(old);
    return cursor;
}

static int help_window(int cursor)
{
    const struct DlcAchievement *a;
    int y, i, k, n = dlc_achievement_count, ry, p;
    char s[40];

    if (n > 60) {
        n = 60;
    }
    if (HELP_OPEN == 2 && n > ROWS) {
        if (SzPad_Check(0, 2, PAD_UP) && scroll > 0) {
            scroll--;
        }
        if (SzPad_Check(0, 2, PAD_DOWN) && scroll < n - ROWS) {
            scroll++;
        }
    }
    y = 512 - HELP_STEP * 64;
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, 2, 96, y + 36, -448, -380, 2, 0);
    s[0] = 0;
    {
        int m = 0;
        const char *t = "ACHIEVEMENTS  ";
        while (*t) {
            s[m++] = *t++;
        }
        of(s + m, DlcAch_EarnedCount(), n);
    }
    cursor = line(cursor, 320, y + 44, 3, s);
    for (i = 0; i < ROWS && scroll + i < n; i++) {
        k = scroll + i;
        a = &dlc_achievements[k];
        ry = y + 72 + i * ROW_HEIGHT;
        cursor = left(cursor, 112, ry, a->name);
        if (DlcAch_Earned(k)) {
            cursor = left(cursor, 420, ry, "EARNED");
        } else {
            p = DlcAch_Progress(k);
            of(s, p < a->count ? p : a->count, a->count);
            cursor = left(cursor, 420, ry, s);
        }
        cursor = left(cursor, 128, ry + 16, a->desc);
        if (a->reward[0] != 0) {
            int m = 0;
            const char *t = a->sleeve >= 0 ? "NEW " : "CARD ";
            while (*t) {
                s[m++] = *t++;
            }
            for (t = a->reward; *t && m < 38; t++) {
                s[m++] = *t;
            }
            s[m] = 0;
            cursor = left(cursor, 128, ry + 32, s);
        }
    }
    if (n > ROWS) {
        cursor = line(cursor, 320, y + 386, 0, scroll + ROWS < n ? "DOWN  MORE" : "UP  BACK TO THE TOP");
    }
    return cursor;
}

/* Taku_Camp_Go_Help (hooked): X in the window starts no tutorial; the window stays open. */
void DlcAch_GoHelp(void)
{
    HELP_OPEN = 2;
}

/* After the campaign map's system panel (Taku_Camp_SYS_ICON, widescreen.c's wrap): a trophy over
 * its fourth icon (the "?"), grown as the game grows the picked icon, and TROPHIES for its label. */
int DlcAch_SysIcon(int cursor)
{
    int y = 128 - ((*(short *)0x1ed0070 << 7) >> 8);
    int menu = *(signed char *)0x1ed007e;
    int pick = *(short *)0x1ed0080;
    int grow = *(short *)(0x1ed0082 + 2 * 3);
    int g = pick == 3 && menu != 0 ? (grow * 6) >> 4 : 0;
    int i;

    for (i = 0; i < 4; i++) {
        HELP_TEXT[i] = ' ';
    }
    cursor = DlcShop_DrawPicture(cursor, dlc_trophy_icon, 578 - g, 394 - g + y, 32 + 2 * g, 32 + 2 * g);
    if (menu == 1 && pick == 3) {
        cursor = left(cursor, 562, y + 335, "TROPHIES");
    }
    return cursor;
}

/* ------------------------------------------------------------- the duel's YOU WIN window */

extern int SzData_GetDeckCost(int side, int deckNo);
extern int SzData_GetCurrentDeck(int side);

#define DUEL ((unsigned char *)0x317490)
#define DUEL_STATE (DUEL[0xd57])      /* 0x5c: the result window, waiting for a button */
#define DUEL_NUMBER_BYTE (DUEL[0xd37])  /* below 22: a campaign duel (it calls KATIMAKE) */
#define DUEL_WINNER (DUEL[0xd52])
#define DUEL_PLAYER (DUEL[0xd50])
#define DUEL_DIALOG (DUEL[0xcb2])     /* the result window's dialog kind: 2, 4 or 5 */

/* After the duel's dialogs (SzDuel_DrawDialog_Pipe, widescreen.c's wrap): under the YOU WIN
 * window's card, the star chips the win pays (the same sum DlcAch_Duel then adds, from the
 * opponent's deck cost, which side 1 still holds). */
int DlcAch_DuelWindow(int cursor)
{
    char s[32];
    int n, chips;
    const char *t;

    if (DUEL_STATE != 0x5c || DUEL_NUMBER_BYTE >= 0x16 || DUEL_WINNER != DUEL_PLAYER) {
        return cursor;
    }
    if (DUEL_DIALOG != 2 && DUEL_DIALOG != 4 && DUEL_DIALOG != 5) {
        return cursor;
    }
    chips = (SzData_GetDeckCost(1, SzData_GetCurrentDeck(1)) + CHIPS_WIN_DIVISOR - 1) / CHIPS_WIN_DIVISOR;
    n = 0;
    for (t = "STAR CHIPS  +"; *t; t++) {
        s[n++] = *t;
    }
    n += put_number(s + n, chips);
    s[n] = 0;
    return line(cursor, 320, 262, 3, s);
}

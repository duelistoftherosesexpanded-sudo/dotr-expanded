/* New opponents on the campaign map (bosses.json; see dlc_bosses.h and tools/dlc_bosses.py), in
 * the game's world and the second world (world.c).
 *
 * The game's 22 locations are tied to its story: arrays of 22 in the save, the routes, the events
 * after each win or loss. A new opponent is kept apart from all that, by the DLC:
 *
 *   drawing   after the game's map (Taku_Camp_3D, wrapped in mod.json) its podium is drawn with the
 *             base and building models of the game location named by "look" (record +16 and +18,
 *             each drawn as model number + 34, as the game does). In the second world the game's
 *             map drawing is left out: only the map models (34-41) and the second world's podiums;
 *   choosing  when the map's hand cursor (0x1ed0000) is over the podium its name, record and
 *             "X DUEL" are shown (drawn after the system panel, widescreen.c); X starts the duel
 *             while the map takes picks (0x1ed006e; in the second world set here from the same
 *             conditions, the game's locations being kept from the hand) and the system menu is closed
 *             (0x1ed007e); on a gate podium SQUARE goes to the other world;
 *   the duel  as Taku_Camp_Set_Go_Duel starts a location's: opponent (menu number 1, and the duel
 *             number at 0x299ed4 while it lasts), the player's side (2), who plays each side
 *             (3 and 4: 1100 the pad, 1000 + opponent the computer), then SzGame_Push(1, 0);
 *   afterwards Taku_Camp_What_KATIMAKE, which records a campaign duel's result and moves the story
 *             on by the location last picked, is wrapped: after one of these duels it records
 *             the result here instead (duelled, beaten); it does not set ACT2_Rock (see there).
 *
 * In the second world the game's opponent panel (name, portrait, deck cost, leader, record) is
 * shown for the boss under the hand: the game is told the hand is over the location whose panel
 * names that opponent (DlcBosses_Act2), the board reads as reached during the panel's call, and
 * the game's own X is hidden while it lasts. The boss's deck and leader are random (boss_deck).
 *
 * The marks are saved with the new cards (card_save.c). */

#include "dotr/ee/side_table.h"
#include "dlc_bosses.h"
#include "dlc_cards.h"
#include "dlc_achievements.h"

extern int SzModel_Draw_Pipe(int cursor, int index, void *pos, int d, int e, int f, int g);
extern int SzDuel_DrawWindow_Font_Pipe(int cursor, int x, int y, int font, int flags, int count,
                                       signed char *text, unsigned char *colour);
extern void Taku_3D_2D_Set(int *x, int *y, void *point);
extern int SzPad_Check(int pad, int mode, int mask);
extern int SzGame_SetMenuNum(int index, int value);
extern int SzGame_Push(int mode, int argument);
extern int SzGame_GetMode(int which);
extern int SzEffect_IsUse2(int a, int b);
extern int SzSound_PlaySE(int channel, int id);

#define LOCATIONS ((unsigned char *)0x298ba0)  /* 52-byte records: x, height, z floats first */
#define BOARD ((unsigned char *)0x299e10)      /* the campaign state, saved */
#define SIDE (BOARD[0])                        /* 0 Lancaster, 1 York */
#define BEATEN(i) (BOARD[3 + (i)] >= 3)        /* seen[] of the game's locations */
#define TICKS (*(short *)0x299ed2)
#define DUEL_NUMBER (*(short *)0x299ed4)
#define PICK_OK (*(signed char *)0x1ed006e)
#define SYSTEM_MENU (*(signed char *)0x1ed007e)
#define HAND_X (*(short *)0x1ed0000)
#define HAND_Y (*(short *)0x1ed0002)
#define CAMP_BYTE(o) (*(signed char *)(0x1ed0000 + (o)))
#define ACT2_ROCK (*(signed char *)0x34f6f0)
#define CAMPAIGN_MODE 11
#define PAD_CROSS 0x40
#define PAD_SQUARE 0x80
#define REACH 22   /* screen units between the hand and the podium */

#define FRAME_COUNTER (*(volatile int *)0x317254)

static unsigned int state[DLC_BOSS_WORDS];
static int active = -1;
static int started_at;
static short saved_duel_number;

#define KIND (*(signed char *)0x299ed6)   /* board byte 198: the location the panel shows */
#define C2 CAMP_BYTE(0x53c2)              /* location 11's choice between its two opponents */
#define BOSS_MAX 64
static int window_boss = -1;              /* the second-world boss the panel shows */
static int in_window;
static unsigned char rolls[BOSS_MAX];     /* a new random deck after each duel */
static int voyage;                        /* the ship between the worlds: 0 none, 1 out, 2 in */
static int voyage_t;
static int voyage_draw(int cursor);

unsigned int DlcBosses_State(int word)
{
    return word >= 0 && word < DLC_BOSS_WORDS ? state[word] : 0;
}

void DlcBosses_SetState(int word, unsigned int s)
{
    if (word >= 0 && word < DLC_BOSS_WORDS) {
        state[word] = s;
    }
}

static void mark(int k, unsigned int bits)
{
    state[k / 15] |= bits << (2 * (k % 15));
}

static unsigned int marks(int k)
{
    return state[k / 15] >> (2 * (k % 15)) & 3;
}

static int visible(int k)
{
    const struct DlcBoss *b = &dlc_bosses[k];

    if (b->gate) {
        return 1;
    }
    if (b->world != (dlc_world ? 2 : 1)) {
        return 0;
    }
    if (dlc_world) {
        return 1;
    }
    if (b->campaign >= 0 && b->campaign != SIDE) {
        return 0;
    }
    if (b->after >= 0 && !BEATEN(b->after)) {
        return 0;
    }
    return 1;
}

static void position(int k, float *p, float height)
{
    const struct DlcBoss *b = &dlc_bosses[k];

    if (b->at >= 0) {
        float *r = (float *)(LOCATIONS + 52 * b->at);
        p[0] = r[0];
        p[2] = r[2];
    } else {
        p[0] = b->x;
        p[2] = b->z;
    }
    p[1] = height;
    p[3] = 1.0f;
}

/* Taku_Camp_3D: the game's map, then the new podiums; in the second world only the map models. */
typedef int (*Camp3D)(int, int, int, int, int, int, int, int);
extern unsigned int DlcTramp_Taku_Camp_3D[];
static int can_pick(void);

int DlcBosses_Camp3D(int cursor, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    float p[4];
    int k;

    DlcWorld_Check();
    if (dlc_world) {
        for (k = 34; k <= 39; k++) {
            cursor = SzModel_Draw_Pipe(cursor, k, 0, 0, 0, 0, 0);
        }
        cursor = SzModel_Draw_Pipe(cursor, 41, 0, 0, 0, 0, 0);
        cursor = SzModel_Draw_Pipe(cursor, 40, 0, 0, 0, 0, 0);
        /* "No route growing": Taku_Camp_3D_BOU (the route markers, drawn by the game's Taku_Camp_3D,
         * which the second world does not call) sets it every frame. The map's start after a duel
         * clears it, and without it the map takes no picks (can_pick, TkCamp_Act). */
        CAMP_BYTE(0x5376) = 1;
        /* The flag the game would set here: the map is idle. It also shows the hand cursor and
         * lets it move (TkCamp_Act); the game's locations are kept from being hovered instead
         * (DlcBosses_Act2). */
        PICK_OK = can_pick() ? 1 : 0;
    } else {
        cursor = ((Camp3D)DlcTramp_Taku_Camp_3D)(cursor, a1, a2, a3, a4, a5, a6, a7);
    }
    for (k = 0; k < dlc_boss_count; k++) {
        unsigned char *look;
        if (!visible(k)) {
            continue;
        }
        look = LOCATIONS + 52 * dlc_bosses[k].look;
        position(k, p, 0.0f);
        cursor = SzModel_Draw_Pipe(cursor, *(short *)(look + 16) + 34, p, 0, 0, 0, 0);
        cursor = SzModel_Draw_Pipe(cursor, *(short *)(look + 18) + 34, p, 0, 0, 0, 0);
    }
    if (voyage) {
        cursor = voyage_draw(cursor);
    }
    return cursor;
}

static int text(int cursor, int x, int y, const char *s)
{
    signed char buffer[40];
    int n = 0;

    while (s[n] && n < 39) {
        buffer[n] = s[n];
        n++;
    }
    /* Flags 3: proportional and centred on x. */
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, 0, 3, n, buffer, 0);
}

static void boss_deck(int k);
static void const_deck(int side, int deck);
static void boss_result(int win);
static int deck_cost_of(int k);

/* The voyage between the worlds, from Milford Haven (the game's location 1; the second world's map
 * has its copy at the same place). SQUARE there puts the ship (map model 27, as Taku_Camp_3D_FUNE
 * draws it) at the harbour; it sails out to sea (towards -x, the map's left) for VOYAGE_FRAMES, the
 * world changes (world.c), and it sails back in to the other Milford Haven. The map takes no picks
 * meanwhile. */
#define GATE_LOCATION 1
#define SHIP_MODEL (27 + 34)
/* Taku_Camp_3D_FUNE's height (location 0's), raised a little: the coast here would cut the hull. */
#define SHIP_HEIGHT (*(float *)0x298ba4 + 3.0f)
/* The map's focus point, which the camera keeps in view (Taku_Screen_Init, Taku_Tyumoku_ido...):
 * it follows the ship during the voyage. */
#define FOCUS_X (*(float *)0x1ed0040)
#define FOCUS_Z (*(float *)0x1ed0048)
#define HARBOUR_OFFSET 10.0f               /* the ship lies this far out from the location */
#define SEA_DISTANCE 80.0f
#define VOYAGE_FRAMES 150
#define HEADING_LEFT 0xc000                /* -x, as Taku_Camp_3D_FUNE's angles go */
#define HEADING_RIGHT 0x4000

static void gate_point(float *p, float height)
{
    float *r = (float *)(LOCATIONS + 52 * GATE_LOCATION);

    p[0] = r[0];
    p[1] = height;
    p[2] = r[2];
    p[3] = 1.0f;
}

/* Whether the hand is over Milford Haven; its screen position in *sx, *sy. */
static int near_gate(int *sx, int *sy)
{
    float p[4];
    int dx, dy;

    gate_point(p, 8.65f);
    Taku_3D_2D_Set(sx, sy, p);
    if (*sx < 0 || *sy < 0) {
        return 0;
    }
    dx = HAND_X - *sx;
    dy = HAND_Y - *sy;
    return dx * dx + dy * dy <= REACH * REACH;
}

/* Each map frame while the voyage lasts (DlcBosses_Camp3D): the ship, and the world changing
 * half way. */
static int voyage_draw(int cursor)
{
    float p[4], f;
    int rot[3];

    gate_point(p, SHIP_HEIGHT);
    f = (float)voyage_t / VOYAGE_FRAMES;
    rot[0] = 0;
    rot[2] = 0;
    if (voyage == 1) {
        p[0] -= HARBOUR_OFFSET + SEA_DISTANCE * f;
        rot[1] = HEADING_LEFT;
    } else {
        p[0] -= HARBOUR_OFFSET + SEA_DISTANCE * (1.0f - f);
        rot[1] = HEADING_RIGHT;
    }
    cursor = SzModel_Draw_Pipe(cursor, SHIP_MODEL, p, (int)rot, 0, 0, 0);
    FOCUS_X = p[0];
    FOCUS_Z = p[2];
    if (++voyage_t >= VOYAGE_FRAMES) {
        voyage_t = 0;
        if (voyage == 1) {
            DlcWorld_Switch();
            voyage = 2;
        } else {
            voyage = 0;
        }
    }
    PICK_OK = 0;
    return cursor;
}

/* The boss under the hand cursor, or -1. */
static int hovered(void)
{
    float p[4];
    int k, sx, sy, dx, dy;

    for (k = 0; k < dlc_boss_count; k++) {
        if (!visible(k)) {
            continue;
        }
        position(k, p, 8.65f);
        Taku_3D_2D_Set(&sx, &sy, p);
        if (sx < 0 || sy < 0) {
            continue;
        }
        dx = HAND_X - sx;
        dy = HAND_Y - sy;
        if (dx * dx + dy * dy <= REACH * REACH) {
            return k;
        }
    }
    return -1;
}

static void start_duel(int k)
{
    int opponent = dlc_bosses[k].opponent;

    if (dlc_bosses[k].world == 2 || dlc_bosses[k].deck >= 0) {
        boss_deck(k);
    } else if (opponent + 26 < 51) {
        const_deck(1, opponent + 26);
    }
    active = k;
    started_at = FRAME_COUNTER;
    saved_duel_number = DUEL_NUMBER;
    DUEL_NUMBER = opponent;
    mark(k, 1);
    SzGame_SetMenuNum(1, opponent);
    SzGame_SetMenuNum(2, SIDE);
    if (SIDE == 0) {
        SzGame_SetMenuNum(3, 1100);
        SzGame_SetMenuNum(4, opponent + 1000);
    } else {
        SzGame_SetMenuNum(3, opponent + 1000);
        SzGame_SetMenuNum(4, 1100);
    }
    SzGame_Push(1, 0);
}

/* TkCamp_Act2: the hand's hit test against the game's 22 locations (and the system icons). In
 * the second world a hovered game location is dropped, as if the hand were over nothing, so its
 * panel never opens and X cannot pick it; the system icons still work. */
typedef int (*Act2)(int, int, int, int, int, int, int, int);
extern unsigned int DlcTramp_TkCamp_Act2[];
#define HAND_STATE (*(unsigned short *)0x1ed0004)   /* 1 dragging the map, 2 over something */

static int hovered(void);

/* The game location whose panel shows opponent n (its name is string 196 + n), with the board
 * side (b0) and location 11's choice (0x53c2) that make it so; -1 if none. */
static int spot(int n, int *b0, int *c2)
{
    static const signed char loc[22] = {
        1, 11, 2, 8, 5, 10, 9, 6, 4, 3, 7, 12, 13, 14, 16, 18, 19, 20, 21, 0, 11, 11
    };

    *b0 = 1;
    *c2 = 0;
    if (n < 0 || n >= 22) {
        return -1;
    }
    if (n == 1 || n == 20) {
        *b0 = 0;
        *c2 = n == 20;
    }
    return loc[n];
}

/* The Deck Masters (opponents 22-25) have no location of their own: their panel borrows location
 * DM_SPOT's (its name, portrait, tallies and deck replaced: DlcBosses_PanelText ...), in either
 * world. Their names are strings 141-144 (K, S, T, I). */
#define DM_FIRST 22
#define DM_SPOT 2
static const short dm_name[4] = {141, 144, 143, 142};   /* K (22), I (23), T (24), S (25) */
static int dm_panel;    /* the game's world: the panel shows a Deck Master (set by Act2) */

static int is_dm(int k)
{
    return k >= 0 && k < dlc_boss_count && dlc_bosses[k].opponent >= DM_FIRST && dlc_bosses[k].opponent < DM_FIRST + 4;
}

/* The location whose panel shows boss k (b0, c2 as spot gives them), or -1 when it has no panel. */
static int panel_spot(int k, int *b0, int *c2)
{
    if (is_dm(k)) {
        *b0 = 1;
        *c2 = 0;
        return DM_SPOT;
    }
    if (!dlc_world) {
        return -1;
    }
    return spot(dlc_bosses[k].opponent, b0, c2);
}

int DlcBosses_Act2(int force, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int r = ((Act2)DlcTramp_TkCamp_Act2)(force, a1, a2, a3, a4, a5, a6, a7);
    int k, s, b0, c2;

    /* Over a boss with a panel (any second-world boss; a Deck Master in either world): the game is
     * told the hand is over the location whose panel shows it (a panel still closing for another
     * one closes first). */
    k = HAND_STATE != 1 && ACT2_ROCK == 0 ? hovered() : -1;
    s = k >= 0 && dlc_bosses[k].action == DLC_BOSS_DUEL ? panel_spot(k, &b0, &c2) : -1;
    if (!dlc_world && s < 0 && !dm_panel) {
        return r;                       /* the game's world, its own locations: as it was */
    }
    if (s >= 0 && (*(short *)0x1ed006c == 0 || window_boss == k)) {
        window_boss = k;
        KIND = s;
        CAMP_BYTE(0x6a) = 1;
        CAMP_BYTE(0x7e) = 0;
        HAND_STATE = 2;
        dm_panel = !dlc_world;
    } else if (CAMP_BYTE(0x6a) != 0 && (dlc_world || dm_panel)) {
        CAMP_BYTE(0x6a) = 0;
        CAMP_BYTE(0x7e) = 0;
        HAND_STATE = 0;
        dm_panel = 0;
    } else {
        dm_panel = 0;
    }
    return r;
}

/* The panel (Taku_Camp_TEKI_WINDOW) for a boss: for the duration of the call the board reads as if
 * the panel's location were reached (lv 2, or 4 for location 11) on the side that names the boss's
 * opponent; restored after. */
static signed char saved_b0, saved_lv, saved_c2;
static int saved_at = -1;

void DlcBosses_WindowBegin(void)
{
    int b0, c2, s;

    if (window_boss < 0 || (!dlc_world && !dm_panel)) {
        return;
    }
    s = panel_spot(window_boss, &b0, &c2);
    if (s < 0 || s != KIND) {
        return;
    }
    saved_at = s;
    saved_b0 = BOARD[0];
    saved_lv = BOARD[3 + s];
    saved_c2 = C2;
    BOARD[0] = b0;
    BOARD[3 + s] = s == 11 ? 4 : 2;
    C2 = c2;
    in_window = 1;
}

void DlcBosses_WindowEnd(void)
{
    if (saved_at >= 0) {
        BOARD[0] = saved_b0;
        BOARD[3 + saved_at] = saved_lv;
        C2 = saved_c2;
        saved_at = -1;
    }
    in_window = 0;
}

static int panel_dm(void)
{
    return in_window && is_dm(window_boss);
}

/* Taku_Camp_TEKI_WINDOW's "b->h196 = name - 196" (the opponent number the panel's portrait, deck
 * and tallies use): a Deck Master's own number, his name in the name bar and the tooltip. */
int DlcBosses_PanelText(int number, short *name, short *msg)
{
    int n;

    if (!panel_dm()) {
        return number;
    }
    n = dlc_bosses[window_boss].opponent;
    *name = dm_name[n - DM_FIRST];
    *msg = *name;
    return n;
}

/* The portrait: none for a Deck Master (the game has none). The request keeps a slot of its own
 * (the map closes the panel's portrait by its slot number, TkCamp_Act, so it must stay a real
 * one): one is taken with a portrait that exists (number 1) when there is none yet, and closed at
 * once, every frame, so nothing shows behind the "?". */
extern int ShFace_Request(void *req);
extern void ShFace_Close(void *req);
extern int DlcShop_DrawPicture(int cursor, const unsigned char *texture, int x, int y, int w, int h);
extern const unsigned char *const dlc_question_face;
#define FACE_SLOTS 8

int DlcBosses_FaceRequest(void *face)
{
    unsigned char *f = (unsigned char *)face;

    if (!panel_dm()) {
        return ShFace_Request(face);
    }
    if (f[0] >= FACE_SLOTS) {
        f[1] = 1;
        f[2] = 0;
        ShFace_Request(face);
    }
    if (f[0] < FACE_SLOTS) {
        ShFace_Close(face);
    }
    return 0;
}

void DlcBosses_FaceClose(void *face)
{
    if (((unsigned char *)face)[0] < FACE_SLOTS) {
        ShFace_Close(face);
    }
}

/* The "?" picture in the portrait's place (window 3 of the panel: 496-624 across, 128 high). */
int DlcBosses_PanelPortrait(int cursor, int y)
{
    if (!panel_dm()) {
        return cursor;
    }
    return DlcShop_DrawPicture(cursor, dlc_question_face, 502, y + 6, 116, 116);
}

/* TkCamp_Act: in the second world, while the hand is over a boss the game's own X (pick the
 * location) is not seen; X is the boss's (DlcBosses_Draw2D). SzPad_Check is wrapped for that. */
typedef int (*Act)(int, int, int, int, int, int, int, int);
extern unsigned int DlcTramp_TkCamp_Act[];
extern unsigned int DlcTramp_SzPad_Check[];
static int hide_cross;

int DlcBosses_Act(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int r;

    /* Also while the duel results box is up: X closes it (achievements.c) and picks nothing. */
    hide_cross = ((dlc_world || dm_panel) && CAMP_BYTE(0x6a) != 0) || DlcAch_ResultShowing();
    r = ((Act)DlcTramp_TkCamp_Act)(a0, a1, a2, a3, a4, a5, a6, a7);
    hide_cross = 0;
    return r;
}

int DlcBosses_PadCheck(int pad, int mode, int mask, int a3, int a4, int a5, int a6, int a7)
{
    if (hide_cross) {
        mask &= ~PAD_CROSS;
        if (mask == 0) {
            return 0;
        }
    }
    return ((Act)DlcTramp_SzPad_Check)(pad, mode, mask, a3, a4, a5, a6, a7);
}

/* Decks. The computer's deck in a campaign duel is whatever side 1 holds: the panel loads the
 * opponent's (SzData_SetupFromConstDeck(1, opponent + 26)). A second-world boss gets one of the
 * decks of decks.json at random instead (random cards when there are none), the same until it is
 * duelled, its deck cost shown as the opponent's own.
 * The new bosses of the game's world get their opponent's deck. */
extern unsigned int DlcTramp_SzData_SetupFromConstDeck[];
extern int SzData_GetConstDeck(int deck, int slot);
extern int SzData_PushDeck(int side, int deckNo, int cardNo);
extern int SzData_GetDeckCost(int side, int deckNo);
extern int SzData_GetLeader(int side, int deckNo);
typedef void (*Setup)(int, int, int, int, int, int, int, int);

static void const_deck(int side, int deck)
{
    ((Setup)DlcTramp_SzData_SetupFromConstDeck)(side, deck, 0, 0, 0, 0, 0, 0);
}

static unsigned int seed;

static int roll(int n)
{
    seed = seed * 1103515245u + 12345u;
    return (int)((seed >> 8) % (unsigned int)n);
}

static short spells[256];
static int spell_count = -1;

/* The game's non-monster cards that some opponent plays (683-853), as the pool for the rest. */
static void find_spells(void)
{
    int d, s, i, id;

    spell_count = 0;
    for (d = 0; d < 51; d++) {
        for (s = 1; s < 41; s++) {
            id = SzData_GetConstDeck(d, s);
            if (id < 683 || id >= DLC_CARD_ORIGINAL) {
                continue;
            }
            for (i = 0; i < spell_count && spells[i] != id; i++) {
            }
            if (i == spell_count && spell_count < 256) {
                spells[spell_count++] = (short)id;
            }
        }
    }
}

static int random_monster(void)
{
    int id, i;

    if (roll(6) == 0) {
        i = roll(dlc_card_count);
        id = dlc_cards[i].number;
        if (DlcCard_IsNew(id) && DlcCard_Donor(id) < 683) {
            return id;
        }
    }
    do {
        id = 1 + roll(682);
    } while (id == 671);
    return id;
}

/* The pool a second-world boss draws its deck from: the decks of decks.json, each held by one boss
 * at a time, and random decks (random cards), as many as wanted. pool_deck[k] is the boss's deck
 * (-1 random), drawn again when its rolls changes (after a duel with it). */
static signed char pool_deck[BOSS_MAX];
static unsigned char pool_roll[BOSS_MAX];   /* rolls[k] + 1 when pool_deck[k] was drawn; 0 none */

static int deck_taken(int d, int k)
{
    int j;

    for (j = 0; j < BOSS_MAX; j++) {
        if (j != k && pool_roll[j] != 0 && pool_deck[j] == d) {
            return 1;
        }
    }
    /* A deck a boss always plays (bosses.json "deck") is not in the pool. */
    for (j = 0; j < dlc_boss_count; j++) {
        if (j != k && dlc_bosses[j].deck == d) {
            return 1;
        }
    }
    return 0;
}

static void pick_pool_deck(int k)
{
    int d, free = 0, r;

    if (pool_roll[k] == (unsigned char)(rolls[k] + 1)) {
        return;
    }
    if (dlc_bosses[k].deck >= 0 && dlc_bosses[k].deck < dlc_boss_deck_count) {
        pool_deck[k] = (signed char)dlc_bosses[k].deck;      /* its own deck, always */
        pool_roll[k] = (unsigned char)(rolls[k] + 1);
        if (pool_roll[k] == 0) {
            pool_roll[k] = 1;
        }
        return;
    }
    for (d = 0; d < dlc_boss_deck_count; d++) {
        free += !deck_taken(d, k);
    }
    /* Each free deck and "random cards" equally likely. */
    r = roll(free + 1);
    pool_deck[k] = -1;
    for (d = 0; d < dlc_boss_deck_count; d++) {
        if (!deck_taken(d, k) && r-- == 0) {
            pool_deck[k] = (signed char)d;
            break;
        }
    }
    pool_roll[k] = (unsigned char)(rolls[k] + 1);
    if (pool_roll[k] == 0) {
        pool_roll[k] = 1;
    }
}

/* lSzDuel_GetControl (ported, mod.json): the AI table entry a computer player uses this turn. In a
 * duel with a second-world boss its deck's (decks.json "ai"), not the opponent's own: each
 * duelist's AI has plays written for his own deck, which the boss does not have. Pads (100, 101)
 * and every other duel are left alone. */
int DlcBosses_Control(int side, int kind)
{
    int ai;

    (void)side;
    if (kind >= 100 || active < 0 || active >= dlc_boss_count || active >= BOSS_MAX
        || (dlc_bosses[active].world != 2 && dlc_bosses[active].deck < 0)) {
        return kind;
    }
    ai = pool_deck[active] >= 0 ? dlc_boss_deck_ai[(int)pool_deck[active]] : dlc_random_deck_ai;
    return ai >= 0 && ai <= 25 ? ai : kind;
}

/* The deck cost of boss k's deck, as the game counts it (its 40 cards' costs, the leader not
 * counted; SzData_PushDeck), from the cards' records now (SPEX, the Card Maker's changes): its own
 * deck (bosses.json "deck"), else in the game's world its opponent's built-in deck; -1 when it has
 * neither (a second-world boss's deck is shown by the game's panel). */
static int deck_cost_of(int k)
{
    const unsigned char *r;
    int i, deck, sum = 0;

    if (dlc_bosses[k].deck >= 0 && dlc_bosses[k].deck < dlc_boss_deck_count) {
        for (i = 1; i <= 40; i++) {
            r = DlcCard_Record(dlc_boss_decks[dlc_bosses[k].deck][i]);
            sum += r != 0 ? r[3] : 0;
        }
        return sum;
    }
    deck = dlc_bosses[k].opponent + 26;
    if (dlc_world || deck >= 51) {
        return -1;
    }
    for (i = 1; i <= 40; i++) {
        r = DlcCard_Record(SzData_GetConstDeck(deck, i));
        sum += r != 0 ? r[3] : 0;
    }
    return sum;
}

/* Whether the duel starting is against one of bosses.json's opponents (fields.c then makes the
 * board for its deck). */
int DlcBosses_FieldDuel(void)
{
    return active >= 0 && active < dlc_boss_count && dlc_bosses[active].action == DLC_BOSS_DUEL;
}

static void boss_deck(int k)
{
    struct DotrSide *p = DOTR_SIDE(1);
    int deck = dlc_bosses[k].opponent + 26;
    int cost, leader, rank, key, placed, tries, i, id;

    if (deck >= 0 && deck < 51) {
        const_deck(1, deck);   /* the opponent's own: its leader's rank */
        cost = SzData_GetDeckCost(1, 0);
        rank = SzData_GetRank(1, SzData_GetLeader(1, 0));
    } else {
        /* An opponent without a built-in deck (Deck Master S, 25): a mid rank for its leader. */
        cost = 0;
        rank = 6;
    }
    key = p->unknown_ff98;
    if (spell_count < 0) {
        find_spells();
    }
    seed = (unsigned int)(k + 1) * 2654435761u ^ (unsigned int)(rolls[k] + 1) * 40503u;
    roll(2);

    SzData_ClearData(1);
    p->unknown_ff98 = key;
    pick_pool_deck(k);
    if (pool_deck[k] >= 0) {
        /* One of the decks of decks.json. */
        const short *d = dlc_boss_decks[pool_deck[k]];

        leader = d[0];
        SzData_AddBag(1, 0, leader);
        SzData_SetRankDirect(1, leader, rank > 0 ? rank : 1);
        SzData_PushDeckLeader(1, 0, leader);
        for (i = 1; i <= 40; i++) {
            SzData_AddBag(1, 0, d[i]);
            SzData_PushDeck(1, 0, d[i]);
        }
    } else {
        leader = 1 + roll(682);
        if (leader == 671) {
            leader = 1;
        }
        SzData_AddBag(1, 0, leader);
        SzData_SetRankDirect(1, leader, rank > 0 ? rank : 1);
        SzData_PushDeckLeader(1, 0, leader);
        for (placed = 0, tries = 0; placed < 40 && tries < 400; tries++) {
            id = spell_count > 0 && roll(40) < 14 ? spells[roll(spell_count)] : random_monster();
            SzData_AddBag(1, 0, id);
            if (SzData_PushDeck(1, 0, id)) {
                placed++;
            }
        }
    }
    for (i = 0; i < 5; i++) {
        p->newcomers[i] = 999;
    }
    SzText_Free(0, 1);
    SzText_Free(0, 0);
    SzText_Free(1, -1);
    /* The deck cost is the deck's own, added up card by card as they went in (SzData_PushDeck, the
     * player's rule too), not the opponent's built-in deck's: the panel shows it and the win pays
     * star chips by it (achievements.c). */
    (void)cost;
}

int DlcBosses_SetupConstDeck(int side, int deck, int a2, int a3, int a4, int a5, int a6, int a7)
{
    if (in_window && side == 1 && window_boss >= 0) {
        /* A boss's deck, as its duel will have it: a second-world boss's or its own (boss_deck),
         * else its opponent's built-in one (a Deck Master in the game's world). */
        if (dlc_bosses[window_boss].world == 2 || dlc_bosses[window_boss].deck >= 0
            || dlc_bosses[window_boss].opponent + 26 >= 51) {
            boss_deck(window_boss);
        } else {
            const_deck(1, dlc_bosses[window_boss].opponent + 26);
        }
        return 0;
    }
    ((Setup)DlcTramp_SzData_SetupFromConstDeck)(side, deck, a2, a3, a4, a5, a6, a7);
    return 0;
}

/* Whether the map is taking picks. The game's world: its own flag. The second world: the same
 * conditions Taku_Camp_3D sets that flag from (map idle: no event, no effect running, no glide). */
static int can_pick(void)
{
    if (!dlc_world) {
        return PICK_OK == 1;
    }
    return TICKS < 10000 && CAMP_BYTE(0x5481) != 0 && CAMP_BYTE(0x5376) != 0 && CAMP_BYTE(0x5482) == 0
           && CAMP_BYTE(0x5372) == 0 && !SzEffect_IsUse2(-1, -1);
}

/* The panel over a podium under the hand cursor, and X to duel. Called after the system panel. */
int DlcBosses_Draw2D(int cursor)
{
    float p[4];
    int k, sx, sy, b0, c2;

    if (SzGame_GetMode(0) != CAMPAIGN_MODE) {
        return cursor;
    }
    if (active >= 0) {
        /* Back on the map without a result. The map is still drawn for a few frames after the duel
         * is asked for, hence the delay. */
        if (FRAME_COUNTER - started_at < 120) {
            return cursor;
        }
        /* A Deck Master's duel (opponent 22-25): the game reports no result for it, so it is
         * taken here from the duel's winner and player bytes, which stay until the next duel. */
        if (active < dlc_boss_count && dlc_bosses[active].opponent >= 22) {
            extern void DlcShop_DuelDone(void);
            DlcShop_DuelDone();
            boss_result(*(unsigned char *)(0x317490 + 0xd52) == *(unsigned char *)(0x317490 + 0xd50));
            return cursor;
        }
        /* Otherwise the duel was left another way: forget it. */
        DUEL_NUMBER = saved_duel_number;
        if (active < BOSS_MAX) {
            rolls[active]++;
        }
        active = -1;
    }
    if (dlc_world) {
        cursor = text(cursor, 320, 24, "THE SECOND WORLD");
    }
    if (voyage) {
        return cursor;
    }
    /* The duel results box is up: X and SQUARE are its (achievements.c), the podiums wait. */
    if (DlcAch_ResultShowing()) {
        return cursor;
    }
    /* Milford Haven, in either world: SQUARE sails to the other. */
    if (can_pick() && SYSTEM_MENU == 0 && near_gate(&sx, &sy)) {
        /* Above the podium: the game's name plate for the location sits below it. */
        cursor = text(cursor, sx, sy - 72, dlc_world ? "SQUARE  SAIL HOME" : "SQUARE  SET SAIL");
        if (SzPad_Check(0, 0, PAD_SQUARE)) {
            SzSound_PlaySE(0, 4);
            voyage = 1;
            voyage_t = 0;
            return cursor;
        }
    }
    k = hovered();
    if (k >= 0) {
        position(k, p, 8.65f);
        Taku_3D_2D_Set(&sx, &sy, p);
        /* In the second world the game's panel names the boss. */
        if (panel_spot(k, &b0, &c2) < 0) {
            cursor = text(cursor, sx, sy - 56, dlc_bosses[k].name);
        }
        if (dlc_bosses[k].action == DLC_BOSS_DECK || dlc_bosses[k].action == DLC_BOSS_SHOP) {
            /* A podium of the DLC's own: the deck builder, as the system panel opens it, or the
             * card shop (the deck builder again, which shop.c turns into the shop). */
            if (can_pick() && SYSTEM_MENU == 0) {
                cursor = text(cursor, sx, sy + 20,
                              dlc_bosses[k].action == DLC_BOSS_SHOP ? "X  CARD SHOP" : "X  BUILD DECK");
                if (SzPad_Check(0, 0, PAD_CROSS)) {
                    extern void DlcShop_Request(int shop);
                    DlcShop_Request(dlc_bosses[k].action == DLC_BOSS_SHOP);
                    SzSound_PlaySE(0, 4);
                    SzGame_SetMenuNum(6, 0);
                    SzGame_Push(6, 0);
                }
            }
            return cursor;
        }
        /* Its deck cost, as the game's panel shows a duelist's (in the second world the panel does). */
        if (panel_spot(k, &b0, &c2) < 0) {
            int dc = deck_cost_of(k), n = 3, d;
            char s[12];
            s[0] = 'D';
            s[1] = 'C';
            s[2] = ' ';
            if (dc >= 0) {
                for (d = 10000; d > 1 && dc < d; d /= 10) {
                }
                for (; d > 0; d /= 10) {
                    s[n++] = (char)('0' + dc / d % 10);
                }
                s[n] = 0;
                cursor = text(cursor, sx, sy - 40, s);
            }
        }
        if (marks(k) & 2) {
            cursor = text(cursor, sx, sy - 24, "BEATEN");
        }
        if (can_pick() && SYSTEM_MENU == 0) {
            cursor = text(cursor, sx, sy + 20, "X  DUEL");
            if (dlc_bosses[k].gate) {
                cursor = text(cursor, sx, sy + 36, dlc_world ? "SQUARE  RETURN" : "SQUARE  SECOND WORLD");
            }
            if (SzPad_Check(0, 0, PAD_CROSS)) {
                start_duel(k);
            } else if (dlc_bosses[k].gate && SzPad_Check(0, 0, PAD_SQUARE)) {
                DlcWorld_Switch();
            }
        }
    }
    return cursor;
}

/* For the map debug overlay: the boss count, then for the boss nearest the hand its number,
 * name, whether it is shown (V1) and its distance from the hand in screen units (D). */
int DlcBosses_Debug(int cursor, int x, int y)
{
    signed char s[64];
    float p[4];
    int k, n, sx, sy, dx, dy, d, best = -1, best_d = 0x7fffffff;
    const char *c;

    for (k = 0; k < dlc_boss_count; k++) {
        position(k, p, 8.65f);
        Taku_3D_2D_Set(&sx, &sy, p);
        dx = HAND_X - sx;
        dy = HAND_Y - sy;
        d = dx * dx + dy * dy;
        if (d < best_d) {
            best_d = d;
            best = k;
        }
    }
    n = 0;
    for (c = "BOSSES "; *c; c++) s[n++] = *c;
    s[n++] = '0' + dlc_boss_count / 10 % 10;
    s[n++] = '0' + dlc_boss_count % 10;
    for (c = " W"; *c; c++) s[n++] = *c;
    s[n++] = '1' + (dlc_world != 0);
    if (best >= 0) {
        s[n++] = ' ';
        for (c = dlc_bosses[best].name; *c && n < 40; c++) s[n++] = *c;
        for (c = " V"; *c; c++) s[n++] = *c;
        s[n++] = '0' + visible(best);
        for (c = " D"; *c; c++) s[n++] = *c;
        for (d = 1; d * d <= best_d && d < 999; d++) {
        }
        d--;
        if (d >= 100) s[n++] = '0' + d / 100;
        if (d >= 10) s[n++] = '0' + d / 10 % 10;
        s[n++] = '0' + d % 10;
    }
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, 0, 1, n, s, 0);
}

/* The deck cost of the opponent just duelled (side 1's current deck; a second-world boss shows its
 * opponent's own cost, boss_deck), for the coins it pays (achievements.c). */
static int opponent_cost(void)
{
    extern int SzData_GetCurrentDeck(int side);

    return SzData_GetDeckCost(1, SzData_GetCurrentDeck(1));
}

/* Taku_Camp_What_KATIMAKE(win): after one of these duels, the result is kept here. */
typedef int (*Katimake)(int, int, int, int, int, int, int, int);
extern unsigned int DlcTramp_Taku_Camp_What_KATIMAKE[];

static void boss_result(int win);

int DlcBosses_Katimake(int win, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    extern void DlcShop_DuelDone(void);

    /* After every duel of the campaign the card shop has new cards. */
    DlcShop_DuelDone();
    if (active < 0 || active >= dlc_boss_count) {
        int r;

        active = -1;
        r = ((Katimake)DlcTramp_Taku_Camp_What_KATIMAKE)(win, a1, a2, a3, a4, a5, a6, a7);
        DlcAch_Duel((signed char)win != 0, opponent_cost());   /* achievements.c */
        return r;
    }
    boss_result((signed char)win != 0);
    return 0;
}

/* The end of a duel with one of bosses.json's opponents: its marks, a new deck next time, the
 * star chips and the results box. From the KATIMAKE wrap above, or for a Deck Master (opponent
 * 22-25, whose duels the game does not report: SzDuel_Act calls KATIMAKE only for duel numbers
 * below 22) from DlcBosses_Draw2D when the map is back, with the winner the duel left behind. */
static void boss_result(int win)
{
    if (win) {
        mark(active, 2);
    }
    /* A Deck Master's record in the panel: the board's won / lost tallies by opponent number
     * (halfwords at +66 and +130, 32 each; 22-25 the game itself never counts), saved with it. */
    if (is_dm(active)) {
        short *tally = (short *)(BOARD + (win ? 66 : 130)) + dlc_bosses[active].opponent;
        if (*tally < 9999) {
            (*tally)++;
        }
    }
    if (active < BOSS_MAX) {
        rolls[active]++;
    }
    DUEL_NUMBER = saved_duel_number;
    active = -1;
    /* ACT2_Rock (the map's "an event is playing" lock, which the game's KATIMAKE sets) is left
     * alone: only Taku_Camp_System lifts it, and only while the story's tick counter is 0 or
     * 100-199; past that (h194 >= 10000 later in the campaign) it stayed set and the map took no
     * more picks. These duels start no story event, so there is nothing to lock for. */
    /* After the mark: "new podiums beaten" counts it. */
    DlcAch_Duel(win, opponent_cost());
}

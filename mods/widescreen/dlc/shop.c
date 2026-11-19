/* The card shop: the deck builder (game mode 6) opened from the Card Shop podium (bosses.c), with
 *
 *   - its deck list (the right one) holding the shop's cards: 25 drawn by rarity (rarity_gen.c,
 *     tools/dlc_shop.py), new ones after every campaign duel (DlcShop_DuelDone, bosses.c);
 *   - CROSS on a shop card buying it (no money yet): the card goes to the chest (the left list) and
 *     is greyed out in the shop until it restocks;
 *   - START choosing the card sleeve (sleeves.c), shown in the bar below the shop's cards;
 *   - the shop keeper's picture where the deck leader's is, "CARD SHOP" for "BUILD DECK", and the
 *     Kame Game shop as the background (widescreen.c DlcWs_DeckBase);
 *   - the deck builder's other buttons that change a deck (put a card in or out, deck leader,
 *     reincarnation, password, the next deck) doing nothing.
 *
 * SzDuel_ActBuildDeck (ported) asks DlcShop_Act first each frame; lSzDuel_DrawBuildDeck_Deck_Pipe
 * and lSzDuel_DrawBuildDeck_Card_Pipe (ported) ask for the picture, texts and grey; the deck builder's
 * set-up (card_builddeck.c) calls DlcShop_Init. The deck builder changes the save only through
 * SzData_* calls, which the shop leaves out, so the player's decks stay as they were. */

#include "dotr/ee/build_deck.h"
#include "dlc_cards.h"
#include "dlc_mods.h"
#include "dlc_achievements.h"

extern int DlcSave_AddBag(int side, int key, int cardNo);

extern int SzDuel_DrawWindow_Font_Pipe(int cursor, int x, int y, int font, int flags, int count,
                                       const char *text, void *colour);
extern int lSzDuel_DrawWindow_PicMini_Pipe(int cursor, int id, int x, int y, int width, int height,
                                           unsigned int z, int bright);
extern int SzText_InitClutManage(int base, int size);
extern int SzText_Load_Pipe(int cursor, void *texture, int clut, int a);
extern int RhText_SetTextureFlush(int cursor);
extern int RhText_BindAddress(void *texture);
extern int ExAssignTexture(void *packet, void *texture, int a);
extern int DlcWs_SortPrim(int packet, int source, int table, int z);

extern const unsigned char dlc_rarity[DLC_CARD_LIMIT];
extern const unsigned short dlc_rarity_chances[6];
extern const int dlc_shop_tiles_x, dlc_shop_tiles_y;
extern const unsigned char *const dlc_shop_tiles[];
extern const unsigned char *const dlc_shop_keeper;
extern const int dlc_shop_sleeve_count;
extern const unsigned char *const dlc_shop_sleeves[];

#define STOCK 25
#define TIERS 6
#define LIST_SIZE 41                       /* the deck list: leader and 40 cards */
#define CONTROL ((unsigned char *)0x317490)
#define DRAW_Z (*(int *)(CONTROL + 0xc58))
#define CLUT_BASE (*(int *)(CONTROL + 0xc7c))
#define FRAME_COUNTER (*(volatile int *)0x317254)
#define TITLE_TEXT ((char *)0x2bef82)     /* "BUILD DECK" (lSzDuel_DrawBuildDeck_Title_Pipe) */

#define PAD_LT_RT 0x3
#define PAD_TRIANGLE 0x10
#define PAD_CIRCLE 0x20
#define PAD_UP 0x1000
#define PAD_UP_DOWN 0x5000
#define PAD_LEFT_RIGHT 0xa000
#define SORT_COLOUR 9
#define PAD_CROSS 0x40
#define PAD_CROSS_SQUARE 0xc0
#define PAD_L3 0x200
#define PAD_R3 0x400
#define PAD_START 0x800

static int requested;          /* the podium asked for the shop; the next set-up takes it */
static int shop_open;
static int stale = 1;          /* a duel since the last stock (or none yet) */
static unsigned short stock[STOCK];
static unsigned char sold[STOCK];
static int stock_count;
static unsigned int seed;
static int sleeve_panel, sleeve_pick;

extern void func_00199bc0(void *base, int count, int size, void *compare);   /* qsort */
extern int lSzDuel_SortFunc2();

void DlcShop_Request(int shop)
{
    requested = shop;
}

int DlcShop_IsOpen(void)
{
    return shop_open;
}

void DlcShop_DuelDone(void)
{
    stale = 1;
}

/* The stock in the save (card_save.c), so that loading a save gives back the same cards (and
 * which are sold) rather than new ones: entry k as 0x800 | sold << 10 | card, 0 past the stock. */
int DlcShop_StockSize(void)
{
    return STOCK;
}

unsigned int DlcShop_SaveEntry(int k)
{
    if (k < 0 || k >= stock_count) {
        return 0;
    }
    return 0x800 | (sold[k] ? 0x400 : 0) | (stock[k] & 0x3ff);
}

int DlcShop_Stale(void)
{
    return stale;
}

/* A loaded save's stock (entries as above); a save without one (or a new game) gets new cards
 * the first time the shop opens. */
void DlcShop_Load(const unsigned int *entries, int count, int is_stale)
{
    int k;

    stock_count = 0;
    for (k = 0; k < count && k < STOCK; k++) {
        if (entries[k] & 0x800) {
            stock[stock_count] = (unsigned short)(entries[k] & 0x3ff);
            sold[stock_count] = (entries[k] >> 10) & 1;
            stock_count++;
        }
    }
    stale = is_stale || stock_count == 0;
}

void DlcShop_Reset(void)
{
    stock_count = 0;
    stale = 1;
}

/* ------------------------------------------------------------------------------------- stock */

static unsigned int shop_random(void)
{
    seed = seed * 1103515245u + 12345u;
    return seed >> 8;
}

static int in_stock(int id)
{
    int k;

    for (k = 0; k < stock_count; k++) {
        if (stock[k] == id) {
            return k;
        }
    }
    return -1;
}

static int can_stock(int id, int tier)
{
    return id < DLC_CARD_LIMIT && DlcCard_Valid(id) && DlcCard_Status(id) == DLC_STATUS_NORMAL
           && dlc_rarity[id] == tier && in_stock(id) < 0;
}

/* A card of the tier not in the shop yet, at random; -1 when there is none. */
static int pick(int tier)
{
    int end = DlcCard_End(), id, n = 0, k;

    for (id = 0; id < end; id++) {
        n += can_stock(id, tier);
    }
    if (n == 0) {
        return -1;
    }
    k = shop_random() % n;
    for (id = 0; id < end; id++) {
        if (can_stock(id, tier) && k-- == 0) {
            return id;
        }
    }
    return -1;
}

static void restock(void)
{
    unsigned int total = 0, r;
    int k, tier, id, step;

    /* The frame count since the game started: never the same twice after a duel. */
    seed ^= (unsigned int)FRAME_COUNTER * 2654435761u;
    for (k = 0; k < TIERS; k++) {
        total += dlc_rarity_chances[k];
    }
    stock_count = 0;
    for (k = 0; k < STOCK && total > 0; k++) {
        r = shop_random() % total;
        for (tier = 0; tier < TIERS - 1 && r >= dlc_rarity_chances[tier]; tier++) {
            r -= dlc_rarity_chances[tier];
        }
        /* A tier with no card left: the next commoner one, then the rarer ones. */
        id = -1;
        for (step = tier; step >= 0 && id < 0; step--) {
            id = pick(step);
        }
        for (step = tier + 1; step < TIERS && id < 0; step++) {
            id = pick(step);
        }
        if (id < 0) {
            break;
        }
        stock[stock_count] = id;
        sold[stock_count] = 0;
        stock_count++;
    }
    stale = 0;
}

static void set_title(const char *text)
{
    int i;

    for (i = 0; text[i] != 0; i++) {
        TITLE_TEXT[i] = text[i];
    }
    TITLE_TEXT[i] = 0;
}

/* After the deck builder's set-up (card_builddeck.c). */
void DlcShop_Init(int side)
{
    struct DotrBuildDeckControl *c = &DOTR_BUILD_DECK;
    int i;

    shop_open = requested;
    requested = 0;
    set_title(shop_open ? "CARD SHOP" : "BUILD DECK");
    if (!shop_open) {
        return;
    }
    if (stale) {
        restock();
    }
    for (i = 0; i < LIST_SIZE; i++) {
        c->deck[i] = i < stock_count ? stock[i] : 999;
    }
    c->deckCount = stock_count;
    c->deckTop = 0;
    c->deckCur = 0;
    /* One page: the shop's cards with the card view beside them (the deck builder's deck list
     * slid left), never the chest. */
    c->mode = 5;
    c->slide = c->slideTo = -320;
    c->tabB = 0;              /* the card view's Basic Info */
    /* Card colour sort only (the tenth sort, SORT_COLOUR), all 25 (the game's deck sort leaves
     * the leader's place out). */
    c->deckSort[0] = c->deckSort[1] = SORT_COLOUR;
    func_00199bc0(c->deck, stock_count, 2, lSzDuel_SortFunc2);
    sleeve_panel = 0;
    (void)side;
}

/* -------------------------------------------------------------------------------- buttons */

static void buy(struct DotrBuildDeckControl *c)
{
    int id = c->deck[c->deckTop + c->deckCur];
    int k = in_stock(id);

    if (k < 0 || sold[k]) {
        SzSound_PlaySE(0, 6);
        return;
    }
    /* Refused for a new card the player already has DLC_CARD_COPIES of (card_save.c; the game's
     * SzData_AddBag is hooked to it, and build_deck.h declares it without its result). */
    if (!DlcSave_AddBag(c->side, 0, id)) {
        SzSound_PlaySE(0, 6);
        return;
    }
    sold[k] = 1;
    DlcAch_Bought();            /* achievements.c: may give a card too */
    lSzDuel_ReBuildBag();
    SzSound_PlaySE(0, 7);
}

/* The next sleeve after k (step 1 or -1) that is unlocked. */
static int next_open_sleeve(int k, int step)
{
    int n = DlcSleeves_Count(), i;

    for (i = 0; i < n; i++) {
        k = (k + step + n) % n;
        if (DlcAch_SleeveOpen(k)) {
            return k;
        }
    }
    return k;
}

/* The sleeve panel (LT / RT): the card view shows a sleeve instead; UP / DOWN pick, CROSS uses it,
 * CIRCLE or LT / RT go back to the cards. */
static void sleeve_act(void)
{
    int n = DlcSleeves_Count();

    if (SzPad_Check(0, 2, PAD_UP_DOWN)) {
        if (n > 1) {
            sleeve_pick = SzPad_Check(0, 2, PAD_UP) ? (sleeve_pick + n - 1) % n : (sleeve_pick + 1) % n;
            SzSound_PlaySE(0, 3);
        }
    } else if (SzPad_Check(0, 0, PAD_CROSS)) {
        if (sleeve_pick != DlcSleeves_Chosen() && DlcAch_SleeveOpen(sleeve_pick)) {
            DlcSleeves_Set(sleeve_pick);
            SzSound_PlaySE(0, 4);
        } else {
            SzSound_PlaySE(0, 6);
        }
    } else if (SzPad_Check(0, 0, PAD_CIRCLE) || SzPad_Check(0, 0, PAD_LT_RT)) {
        sleeve_panel = 0;
        SzSound_PlaySE(0, 17);
    }
}

int DlcShop_Act(void *control)
{
    struct DotrBuildDeckControl *c = control;

    if (!shop_open) {
        return 0;
    }
    if (sleeve_panel) {
        sleeve_act();
        return 1;
    }
    switch (c->mode) {
    case 5:
        if (SzPad_Check(0, 2, PAD_LT_RT)) {
            /* The sort is fixed: LT / RT open the sleeve panel instead. */
            sleeve_panel = 1;
            sleeve_pick = DlcSleeves_Chosen();
            SzSound_PlaySE(0, 18);
            return 1;
        }
        if (SzPad_Check(0, 0, PAD_CIRCLE)) {
            /* Out of the shop (the deck builder would go back to the chest). */
            SzSound_PlaySE(0, 5);
            SzGame_Pop();
            return 1;
        }
        if (SzPad_Check(0, 2, PAD_LEFT_RIGHT)) {
            /* No chest, and no card view tabs. */
            return 1;
        }
        if (SzPad_Check(0, 0, PAD_START)) {
            if (next_open_sleeve(DlcSleeves_Chosen(), 1) != DlcSleeves_Chosen()) {
                DlcSleeves_Set(next_open_sleeve(DlcSleeves_Chosen(), 1));
                SzSound_PlaySE(0, 19);
            } else {
                SzSound_PlaySE(0, 6);
            }
            return 1;
        }
        if (SzPad_Check(0, 2, PAD_CROSS_SQUARE)) {
            if (SzPad_Check(0, 2, PAD_CROSS)) {
                buy(c);
            } else {
                SzSound_PlaySE(0, 6);
            }
            return 1;
        }
        break;
    default:
        /* Anything else (the chest, tabs, messages): back to the shop's page. */
        c->mode = 5;
        c->slide = c->slideTo = -320;
        return 1;
    }
    return 0;
}

/* ---------------------------------------------------------------------------------- drawing */

/* A shop card bought since the last stock: greyed out (the game greys a chest card that is all
 * in decks, bag_list). */
int DlcShop_Grey(int card, int bag_list, int original)
{
    int k;

    if (!shop_open || bag_list) {
        return original;
    }
    k = in_stock(card);
    return k >= 0 && sold[k];
}

char *DlcShop_DeckText(char *original)
{
    return shop_open ? "SLEEVE" : original;
}

/* One of the shop's 128 x 128 pictures at (x, y) in screen units, w x h, as
 * lSzDuel_DrawWindow_PicItem_Pipe draws the card pictures (its texture loaded first). */
struct ShopPicture {
    unsigned long long tag[2];
    unsigned char colour[4];
    float q;
    unsigned long long uv0, xyz0, uv1, xyz1;
    unsigned long long texture[6];
};

static unsigned long long xyz(int x, int y, unsigned int z)
{
    return (unsigned long long)(x << 4 & 0xffff) | (unsigned long long)(y << 4 & 0xffff) << 16
           | (unsigned long long)(z & 0xffffff) << 32;
}

int DlcShop_DrawPicture(int cursor, const unsigned char *texture, int x, int y, int w, int h)
{
    struct ShopPicture p __attribute__((aligned(16)));
    int clut, i;

    if (texture == 0 || texture[0] != 'R' || texture[1] != 'H' || texture[2] != '2') {
        return cursor;
    }
    /* A texture file as it is on the disc holds offsets; the game's loader binds each one once it
     * is read (its own address at +0x0c, added to the offsets). These are in the DLC: bound the
     * first time they are drawn. */
    if (*(const unsigned int *)(texture + 0x0c) != (unsigned int)texture) {
        RhText_BindAddress((void *)texture);
    }
    clut = CLUT_BASE;
    clut += SzText_InitClutManage(clut, 1);
    cursor = SzText_Load_Pipe(cursor, (void *)texture, clut, 0);
    cursor = RhText_SetTextureFlush(cursor);
    for (i = 0; i < 6; i++) {
        p.texture[i] = 0;
    }
    p.tag[0] = 5 | (unsigned long long)(38 | 0x800) << 16;
    p.tag[1] = 0x42421;
    ExAssignTexture(p.tag, (void *)texture, 0);
    p.colour[0] = p.colour[1] = p.colour[2] = p.colour[3] = 128;
    p.q = 1.0f;
    p.uv0 = 8 | 8 << 16;
    p.uv1 = 2056 | 2056 << 16;
    p.xyz0 = xyz(x + 1728, y + 1824, DRAW_Z);
    p.xyz1 = xyz(x + w + 1728, y + h + 1824, DRAW_Z);
    return DlcWs_SortPrim(cursor, (int)p.tag, 0, 0);
}

/* The deck leader's picture in the deck list's corner: the shop keeper in the shop. */
int DlcShop_Portrait(int cursor, int id, int x, int y, int width, int height, unsigned int z, int bright)
{
    if (!shop_open) {
        return lSzDuel_DrawWindow_PicMini_Pipe(cursor, id, x, y, width, height, z, bright);
    }
    return DlcShop_DrawPicture(cursor, dlc_shop_keeper, x, y, width, height);
}

/* The background, the whole screen (widescreen.c draws it unscaled: the game's 640 units across
 * are the whole 16:9 picture). */
#define WIDE_LEFT 0
#define WIDE_WIDTH 640

int DlcShop_DrawBackground(int cursor)
{
    int x, y, w, h;

    if (dlc_shop_tiles_x <= 0 || dlc_shop_tiles_y <= 0) {
        return cursor;
    }
    w = WIDE_WIDTH / dlc_shop_tiles_x;
    h = 448 / dlc_shop_tiles_y;
    for (y = 0; y < dlc_shop_tiles_y; y++) {
        for (x = 0; x < dlc_shop_tiles_x; x++) {
            cursor = DlcShop_DrawPicture(cursor, dlc_shop_tiles[y * dlc_shop_tiles_x + x],
                                         WIDE_LEFT + x * w, y * h, w, h);
        }
    }
    return cursor;
}

/* The bar below the shop's cards, where the deck builder has SELECT DECK and the three deck
 * numbers: "SLEEVE" (DlcShop_DeckText; START changes it), the sleeve's name below it and a small
 * picture of the sleeve to their right. base_x and y as lSzDuel_DrawBuildDeck_Deck_Pipe has them. */
int DlcShop_DrawSleeve(int cursor, int base_x, int y)
{
    int k = DlcSleeves_Chosen();
    const char *name = DlcSleeves_Name(k);
    int n;

    for (n = 0; name[n] != 0; n++) {
    }
    cursor = SzDuel_DrawWindow_Font_Pipe(cursor, base_x - 0xce, y + 0x184 + 20, 1, 0, n, name, 0);
    if (k >= 0 && k < dlc_shop_sleeve_count) {
        /* Right of the text, left of BACK. */
        cursor = DlcShop_DrawPicture(cursor, dlc_shop_sleeves[k], base_x - 0x78, y + 0x17e + 4, 30, 42);
    }
    return cursor;
}

/* ------------------------------------------------------------------------- the card view */

extern int lSzDuel_DrawBuildDeck_Detail_Pipe(int cursor, int card, int x, int y, int a4, int a5, int a6, int a7);
extern int lSzDuel_DrawWindow_ExBoard_Pipe(int cursor, int colour, int x, int y, int w, int h, int shade, int a7);

/* The right column (the shop keeper's box, the card view, the coins) starts level with the list's
 * header (y + 0x52, DlcShop_DeckList) and ends level with its footer (y + 0x21c): RIGHT_DROP moves
 * the whole column down below "CARD SHOP" (top middle). */
#define RIGHT_DROP 0x42
/* How far the card view comes down, below the shop keeper's box (KEEPER_BOX_HEIGHT from y + 0x10). */
#define KEEPER_BOX_HEIGHT 0x58
#define VIEW_DROP 0x60
#define BOX_COLOUR 2            /* a solid backing, as the deck builder's other boxes (DLC_BOX_SHADE) */

/* SzDuel_DrawWindow_Font_Pipe's flags as seen in the shop: 1 the narrow spacing, 2 centred on x
 * (without 2 the text starts at x). The flags argument below is kept for the callers' reading. */
#define FONT_LEFT 1
#define FONT_CENTRED 3

static int text(int cursor, int x, int y, int flags, const char *s)
{
    int n;

    (void)flags;
    for (n = 0; s[n] != 0; n++) {
    }
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, 1, FONT_LEFT, n, s, 0);
}

static int centred(int cursor, int x, int y, int flags, const char *s)
{
    int n;

    (void)flags;
    for (n = 0; s[n] != 0; n++) {
    }
    return SzDuel_DrawWindow_Font_Pipe(cursor, x, y, 0, FONT_CENTRED, n, s, 0);
}

/* The sleeve panel in place of the card view: the sleeve being picked, large, with its name. */
static int sleeve_panel_draw(int cursor, int x, int y)
{
    int bx = x + 0x280, mid = bx + 0xa0;
    int k = sleeve_pick;

    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, BOX_COLOUR, bx + 0x14, y + 0x10, -0x118, -0x118, BOX_COLOUR, 0);
    cursor = centred(cursor, mid, y + 0x1c, 3, "CARD SLEEVES");
    if (k >= 0 && k < dlc_shop_sleeve_count) {
        cursor = DlcShop_DrawPicture(cursor, dlc_shop_sleeves[k], mid - 56, y + 0x34, 112, 160);
    }
    cursor = centred(cursor, mid, y + 0xdc, 3, DlcSleeves_Name(k));
    cursor = centred(cursor, mid, y + 0xf4, 0,
                     !DlcAch_SleeveOpen(k) ? "LOCKED" : k == DlcSleeves_Chosen() ? "IN USE" : "X  USE THIS SLEEVE");
    return centred(cursor, mid, y + 0x10c, 0, "LT RT  BACK");
}

/* The shop keeper's box above the card view: his picture, and beside it the card's purchase (or,
 * with the sleeve panel open, how the sleeve is unlocked). x and y as the card view has them. */
static int keeper_box(int cursor, int x, int y, int card)
{
    int bx = x + 0x280, tx = bx + 0x84, a, k;

    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, BOX_COLOUR, bx + 0x14, y + 0x10, -0x118, -KEEPER_BOX_HEIGHT,
                                             BOX_COLOUR, 0);
    cursor = DlcShop_DrawPicture(cursor, dlc_shop_keeper, bx + 0x1c, y + 0x16, 0x4c, 0x4c);
    tx = bx + 0x74;
    cursor = text(cursor, tx, y + 0x18, 3, "SOLOMON MUTO");
    if (sleeve_panel) {
        k = sleeve_pick;
        a = DlcAch_ForSleeve(k);
        if (k == 0 || a < 0) {
            return text(cursor, tx, y + 0x30, 0, "THIS SLEEVE IS OPEN");
        }
        cursor = text(cursor, tx, y + 0x2e, 0, dlc_achievements[a].name);
        cursor = text(cursor, tx, y + 0x42, 0, dlc_achievements[a].desc);
        return text(cursor, tx, y + 0x56, 0, DlcAch_Earned(a) ? "UNLOCKED" : "TO UNLOCK IT");
    }
    k = in_stock(card);
    cursor = text(cursor, tx, y + 0x30, 0, "PRICE   FREE");
    return text(cursor, tx, y + 0x48, 0, k >= 0 && sold[k] ? "SOLD" : "X  BUY THIS CARD");
}

/* Below the card view: the coins the player has (achievements.c pays them after each duel). */
#define COIN_BOX_Y 0x1ae            /* + RIGHT_DROP + 0x2c: 0x21c, the list footer's bottom */

static int put_number(char *s, int n);

static int coin_box(int cursor, int x, int y)
{
    int bx = x + 0x280, n;
    char s[12];

    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, BOX_COLOUR, bx + 0x14, y + COIN_BOX_Y, -0x118, -0x2c,
                                             BOX_COLOUR, 0);
    cursor = text(cursor, bx + 0x24, y + COIN_BOX_Y + 0x0e, 3, "STAR CHIPS");
    n = put_number(s, DlcAch_Coins());
    s[n] = 0;
    return text(cursor, bx + 0xa0, y + COIN_BOX_Y + 0x0e, 3, s);
}

/* SzDuel_DrawBuildDeck_Pipe (ported) draws the card view through this. */
int DlcShop_Detail(int cursor, int card, int x, int y, int a4, int a5, int a6, int a7)
{
    if (!shop_open) {
        return lSzDuel_DrawBuildDeck_Detail_Pipe(cursor, card, x, y, a4, a5, a6, a7);
    }
    y += RIGHT_DROP;
    cursor = keeper_box(cursor, x, y, card);
    cursor = coin_box(cursor, x, y);
    y += VIEW_DROP;
    if (sleeve_panel) {
        return sleeve_panel_draw(cursor, x, y);
    }
    return lSzDuel_DrawBuildDeck_Detail_Pipe(cursor, card, x, y, a4, 0, 0, a7);
}

extern int lSzDuel_DrawWindow_PicItem_Pipe(int cursor, int id, int x, int y, int width, int height, unsigned int z);
extern void *SzText_GetReadyItem(int group, int id);
extern int SzData_ExchSpecial(int side, int cardNo);

/* lSzDuel_DrawBuildDeck_Detail_Pipe (ported): in the shop the box of Basic Info, Special Info,
 * Power-up Card and Leader Ability is left out (the card view shows Basic Info). */
int DlcShop_TextBox(int cursor, int x, int y, int card)
{
    (void)x;
    (void)y;
    (void)card;
    return cursor;
}

/* SzDuel_DrawBuildDeck_Pipe (ported), after everything else: an achievement pop-up, at the top
 * centre over everything. */
int DlcShop_Last(int cursor)
{
    if (!shop_open) {
        return cursor;
    }
    return DlcAch_DrawPopup(cursor);
}

/* ------------------------------------------------------------------------ the card list */

extern int lSzDuel_DrawBuildDeck_Card_Pipe(int cursor, int card, int x, int y, int flag, int flag2);

#define LIST_ROWS 7             /* dlc_port.h DLC_ROWS */
#define ROW_HEIGHT 0x36
#define ROW_WIDTH 0x130
#define BAR_HEIGHT 0x1c
#define FOOTER_HEIGHT 0x30

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

/* lSzDuel_DrawBuildDeck_Deck_Pipe (ported) in the shop: a header, the cards and a footer with the
 * place in the list and the sleeve in use. x and y as that function has them. */
int DlcShop_DeckList(int cursor, int x, int y)
{
    struct DotrBuildDeckControl *c = &DOTR_BUILD_DECK;
    int cx = x + 0x280 - 0x138, cy = y + 0x70, fy, i, n, k;
    char line[24];

    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, BOX_COLOUR, cx, cy - BAR_HEIGHT - 2, -ROW_WIDTH, -BAR_HEIGHT,
                                             BOX_COLOUR, 0);
    cursor = centred(cursor, cx + ROW_WIDTH / 2, cy - BAR_HEIGHT + 4, 3, "CARDS FOR SALE");
    for (i = 0; i < LIST_ROWS; i++) {
        cursor = lSzDuel_DrawBuildDeck_Card_Pipe(cursor, c->deck[c->deckTop + i], cx, cy + i * ROW_HEIGHT, 0, 0);
    }
    fy = cy + LIST_ROWS * ROW_HEIGHT + 2;
    cursor = lSzDuel_DrawWindow_ExBoard_Pipe(cursor, BOX_COLOUR, cx, fy, -ROW_WIDTH, -FOOTER_HEIGHT, BOX_COLOUR, 0);
    /* 3 OF 25 */
    n = 0;
    n += put_number(line + n, c->deckTop + c->deckCur + 1);
    line[n++] = ' '; line[n++] = 'O'; line[n++] = 'F'; line[n++] = ' ';
    n += put_number(line + n, c->deckCount);
    line[n] = 0;
    cursor = text(cursor, cx + 10, fy + 0x10, 3, line);
    /* The sleeve in use, at the right end, and how to change it. */
    k = DlcSleeves_Chosen();
    /* Centred just left of the sleeve's picture, clear of the count. */
    cursor = centred(cursor, cx + 0xb8, fy + 0x10, 0, "LT RT TO CHANGE");
    if (k >= 0 && k < dlc_shop_sleeve_count) {
        cursor = DlcShop_DrawPicture(cursor, dlc_shop_sleeves[k], cx + ROW_WIDTH - 32, fy + 4, 28, 40);
    }
    return cursor;
}

/* lSzDuel_DrawBuildDeck_Detail_Basic_Pipe (ported): the card view's picture is the shop keeper. */
int DlcShop_KeeperArt(int cursor, int x, int y, int w, int h)
{
    return DlcShop_DrawPicture(cursor, dlc_shop_keeper, x, y, w, h);
}

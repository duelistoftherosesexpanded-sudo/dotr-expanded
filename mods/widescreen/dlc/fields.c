/* Fields made for the deck: in a duel with one of the mod's bosses (bosses.json: the second world's
 * bosses, the Deck Masters ...) the 7 x 7 board is generated at the start of the duel instead of
 * the opponent's fixed map, from the boss's deck.
 *
 * SzDuel_SetupMap (ported unchanged, hooked here) lays out the map as usual; then, when a boss duel
 * is starting (DlcBosses_FieldDuel, bosses.c), the board is drawn again:
 *
 *   1. Each terrain is scored against the deck's monsters (the leader counts double), as the game
 *      scores a monster on it (Do_CheckLandAgreement): +1 a monster it strengthens, -1 one it
 *      weakens. Toon is scored by the deck's toon monsters (Ta_GetToonAgreement); crush only by
 *      Immortals (it destroys monsters of 1500 ATK and more).
 *   2. A terrain's weight is its score (none below zero), plus a little for Normal so a board is
 *      never all one terrain.
 *   3. A handful of seeds are scattered over the board, each given a terrain by weight; every
 *      square takes the terrain of its nearest seed (a little jitter makes the edges uneven), so
 *      the board is made of patches, a different one each duel.
 *   4. The two leaders' squares and those next to them are never crush, and no square is
 *      labyrinth (walls the game's AI handles poorly on a random board).
 */

extern void DlcPort_SzDuel_SetupMap(int index);
extern int SzDuel_SetField(int x, int y, int value);
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetCardNo(int side, int deck, int slot);
extern int SzData_GetLeader(int side, int deck);
extern int Ta_GetToonAgreement(int card);
extern unsigned char *DlcCard_Record(int id);
extern int DlcBosses_FieldDuel(void);

#define FRAME_COUNTER (*(volatile int *)0x317254)

#define FOREST 0
#define WASTELAND 1
#define MOUNTAIN 2
#define MEADOW 3
#define SEA 4
#define DARK 5
#define TOON 6
#define NORMAL 7
#define CRUSH 9
#define TERRAINS 10
#define SEEDS 6

static unsigned int seed;

static unsigned int field_random(void)
{
    seed = seed * 1103515245u + 12345u;
    return seed >> 8;
}

/* The game's terrain table (Do_CheckLandAgreement): 1 strengthens monster type `kind`, 2 weakens. */
static int agreement(int land, int kind)
{
    switch (land) {
    case FOREST:
        return kind == 4 || kind == 5 || kind == 9 || kind == 17 || kind == 19 ? 1 : kind == 7 ? 2 : 0;
    case WASTELAND:
        return kind == 2 || kind == 10 || kind == 14 || kind == 18 ? 1
             : kind == 19 || kind == 12 || kind == 13 || kind == 16 ? 2 : 0;
    case MOUNTAIN:
        return kind == 0 || kind == 6 || kind == 8 || kind == 15 ? 1 : kind == 2 ? 2 : 0;
    case MEADOW:
        return kind == 3 || kind == 4 ? 1 : kind == 1 ? 2 : 0;
    case SEA:
        return kind == 12 || kind == 13 || kind == 15 || kind == 16 ? 1 : kind == 14 || kind == 17 ? 2 : 0;
    case DARK:
        return kind == 2 || kind == 1 || kind == 7 ? 1 : kind == 8 ? 2 : 0;
    case CRUSH:
        return kind == 20 ? 1 : 0;
    }
    return 0;
}

/* Adds card's weight to each terrain's score. */
static void score_card(int *score, int card, int weight)
{
    const unsigned char *r = DlcCard_Record(card);
    int land, kind, a;

    if (r == 0 || (r[0] >> 5) != 0) {
        return;                                  /* monsters only */
    }
    kind = r[0] & 0x1f;
    for (land = 0; land < TERRAINS; land++) {
        if (land == TOON) {
            score[land] += Ta_GetToonAgreement(card) != 0 ? 3 * weight : -weight;
            continue;
        }
        a = agreement(land, kind);
        score[land] += a == 1 ? weight : a == 2 ? -weight : 0;
    }
}

static int pick(const int *weight, int total)
{
    int r = (int)(field_random() % (unsigned int)total), land;

    for (land = 0; land < TERRAINS; land++) {
        if (r < weight[land]) {
            return land;
        }
        r -= weight[land];
    }
    return NORMAL;
}

static void generate(void)
{
    int score[TERRAINS], weight[TERRAINS];
    int sx[SEEDS], sy[SEEDS], st[SEEDS];
    int deck = SzData_GetCurrentDeck(1);
    int i, x, y, land, total = 0, best, d, k, monsters = 0;

    for (land = 0; land < TERRAINS; land++) {
        score[land] = 0;
    }
    score_card(score, SzData_GetLeader(1, deck), 2);
    for (i = 0; i < 40; i++) {
        int card = SzData_GetCardNo(1, deck, i);
        const unsigned char *r = DlcCard_Record(card);
        if (r != 0 && (r[0] >> 5) == 0) {
            monsters++;
        }
        score_card(score, card, 1);
    }
    for (land = 0; land < TERRAINS; land++) {
        weight[land] = score[land] > 0 && land != 8 ? score[land] : 0;
    }
    /* Normal: a share of the board whatever the deck (about one patch in six). */
    weight[NORMAL] = monsters / 4 + 2;
    for (land = 0; land < TERRAINS; land++) {
        total += weight[land];
    }

    seed ^= (unsigned int)FRAME_COUNTER * 2654435761u;
    for (k = 0; k < SEEDS; k++) {
        sx[k] = (int)(field_random() % 7);
        sy[k] = (int)(field_random() % 7);
        st[k] = pick(weight, total);
    }
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            best = 0;
            d = 1000;
            for (k = 0; k < SEEDS; k++) {
                int dx = x - sx[k], dy = y - sy[k];
                int e = (dx * dx + dy * dy) * 4 + (int)(field_random() % 6);
                if (e < d) {
                    d = e;
                    best = k;
                }
            }
            land = st[best];
            /* The leaders start at (3, 0) and (3, 6): no crush on or next to them. */
            if (land == CRUSH && x >= 2 && x <= 4 && (y <= 1 || y >= 5)) {
                land = NORMAL;
            }
            SzDuel_SetField(x, y, land);
        }
    }
}

void DlcField_SetupMap(int index)
{
    DlcPort_SzDuel_SetupMap(index);
    if (DlcBosses_FieldDuel()) {
        generate();
    }
}

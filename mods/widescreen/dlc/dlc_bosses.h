/* New opponents on the campaign map (bosses.json, tools/dlc_bosses.py; the code in bosses.c), and
 * the second world (world.c). */

#ifndef DLC_BOSSES_H
#define DLC_BOSSES_H

struct DlcBoss {
    const char *name;  /* upper case */
    short opponent;    /* duelist number 0-31 */
    float x, z;        /* on the map (unless `at` names a location) */
    short look;        /* the game's location whose podium models it uses */
    short after;       /* -1, or the game's location that must be beaten first */
    short campaign;    /* -1 both, 0 Lancaster, 1 York */
    short world;       /* 1 the game's map, 2 the second world */
    short at;          /* -1, or the game's location it stands at */
    short gate;        /* 1: shown in both worlds, SQUARE goes between them */
    short action;      /* DLC_BOSS_DUEL, or what else X does on this podium */
    short deck;        /* -1, or the decks.json deck it always plays (bosses.c) */
};

#define DLC_BOSS_DUEL 0
#define DLC_BOSS_DECK 1   /* opens the deck builder */
#define DLC_BOSS_SHOP 2   /* opens the card shop (shop.c) */

extern const struct DlcBoss dlc_bosses[];
extern const int dlc_boss_count;

/* The second world's decks (decks.json): a leader, then 40 cards. Each second-world boss draws one
 * that no other boss holds, or a deck of random cards (bosses.c pick_pool_deck). */
extern const short dlc_boss_decks[32][41];
extern const int dlc_boss_deck_count;
/* The computer player (an entry of the game's AI table, 0-25) for each deck of decks.json ("ai"),
 * and for a deck of random cards. */
extern const signed char dlc_boss_deck_ai[32];
extern const int dlc_random_deck_ai;

/* Two bits a boss in the save, 15 bosses to a word (card_save.c packs word w into the rank and
 * experience of record 852 - w): 1 duelled, 2 beaten. */
#define DLC_BOSS_WORDS 3
unsigned int DlcBosses_State(int word);
void DlcBosses_SetState(int word, unsigned int state);

/* Around the game's opponent panel (Taku_Camp_TEKI_WINDOW, widescreen.c): in the second world the
 * panel shows the hovered boss (bosses.c). */
void DlcBosses_WindowBegin(void);
void DlcBosses_WindowEnd(void);

/* The second world (world.c): 0 the game's map, 1 the second. */
extern int dlc_world;
void DlcWorld_Switch(void);
void DlcWorld_Set(int world);
void DlcWorld_Check(void);

#endif

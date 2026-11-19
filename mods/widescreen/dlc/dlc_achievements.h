/* The mod's achievements (achievements.json, tools/dlc_achievements.py; the code in
 * achievements.c). */

#ifndef DLC_ACHIEVEMENTS_H
#define DLC_ACHIEVEMENTS_H

struct DlcAchievement {
    const char *name;         /* capitals, digits, spaces */
    const char *desc;
    short kind;               /* DLC_GOAL_... */
    short count;              /* the number to reach */
    short sleeve;             /* the sleeve it unlocks, or -1 */
    short card;               /* the card it gives, or -1 */
    const char *reward;       /* the sleeve's or card's name in the window font, or "" */
};

#define DLC_GOAL_DUELS_WON 0
#define DLC_GOAL_DUELS 1
#define DLC_GOAL_CARDS_BOUGHT 2
#define DLC_GOAL_BOSSES_BEATEN 3
#define DLC_GOAL_CARDS_OWNED 4

extern const struct DlcAchievement dlc_achievements[60];
extern const int dlc_achievement_count;

/* Events. A duel also pays coins: more for a win, and more against a costlier deck. */
void DlcAch_Duel(int won, int deck_cost);
/* The duel results box is on the map (X closes it; the map takes no X meanwhile). */
int DlcAch_ResultShowing(void);
void DlcAch_Bought(void);
void DlcAch_Check(void);
int DlcAch_Progress(int k);       /* where the goal stands now */
int DlcAch_EarnedCount(void);
int DlcAch_Coins(void);
int DlcAch_Spend(int coins);      /* 1 when there were enough */

/* Sleeves: unlocked unless an achievement not yet earned gives it; the achievement that gives it. */
int DlcAch_SleeveOpen(int sleeve);
int DlcAch_ForSleeve(int sleeve);
int DlcAch_Earned(int k);

/* The pop-up, drawn by the screens that show it (the map, the card shop). */
int DlcAch_DrawPopup(int cursor);
int DlcAch_DrawPopupAt(int cursor, int mid);

/* The campaign map's HELP (TROPHIES) window and its X: the achievements list instead of the
 * tutorials (mod.json hooks). */
int DlcAch_HelpWindow(int cursor);
void DlcAch_GoHelp(void);

/* The save (card_save.c): six 30-bit words (earned marks x2, duels won, duels, cards bought, coins). */
#define DLC_ACH_WORDS 6
unsigned int DlcAch_Word(int w);
void DlcAch_SetWord(int w, unsigned int v);
void DlcAch_Reset(void);

#endif

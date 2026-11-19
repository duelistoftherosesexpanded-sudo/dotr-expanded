/* Included first by a ported game function whose source tools/mod.py has changed (mod.json
 * "dlc": {"port": {"<function>": {"replace": [[old, new], ...]}}}): the helpers the changes use.
 *
 *   DLC_MON(id)     the card is a monster: below 683, or a new card whose donor is.
 *   DLC_BEYOND(id)  past the game's card numbers and not a new card (the test that was ">= 854").
 *
 * The prototypes here use plain types so they agree with any game header. */

#ifndef DLC_PORT_H
#define DLC_PORT_H

int DlcCard_IsNew(int id);
int DlcCard_Donor(int id);
int DlcCard_End(void);
int DlcCard_DuelCard(int id);
void DlcModel_Bind(int object, int card, int place);
char *DlcModel_PackFile(char *name, int object, int place);
int DlcModel_PackOffset(int object, int place, int card);
int DlcModel_Area(int object, int original);
int DlcModel_AreaBytes(int object);
int DlcModel_ReadBytes(int object, int place);
void DlcPic_ViewerOpen(int open);
void DlcSleeves_FieldCard(unsigned char *unit);
int DlcWorld_PackOffset(int type, int fileIndex, int original);
void DlcWs_ModelAspect(void);
int DlcWs_SortPrim(int packet, int source, int table, int z);

/* The AI table entry a computer player uses (bosses.c): a second-world boss's deck's. */
int DlcBosses_Control(int side, int kind);
/* The opponent panel (Taku_Camp_TEKI_WINDOW, ported) for a Deck Master (bosses.c): his opponent
 * number for the panel's tallies, his name in the name bar and the tooltip, no portrait (a "?"
 * picture in its place). */
int DlcBosses_PanelText(int number, short *name, short *msg);
int DlcBosses_FaceRequest(void *face);
void DlcBosses_FaceClose(void *face);
int DlcBosses_PanelPortrait(int cursor, int y);

#define DLC_MON(id) (DlcCard_Donor(id) < 683)
/* Card rows in each list of the deck builder (the game has five; widescreen.c shrinks that
 * screen towards the top, which leaves room for two more). */
#define DLC_ROWS 7
/* The deck builder's boxes (sort tabs, page counts, the bars below the lists) get a solid backing
 * in this colour row of gSzDuel_aaicolRgba (2: brown to orange, opaque) instead of none (-1), so
 * the wallpaper does not show through them. */
#define DLC_BOX_SHADE 2
/* Ex3d_InitClipPlane (polygons clipped on the EE) and RhVu1Mac_SetClipParameter (the cone the VU1
 * clips models to, 645 units wide at the screen distance): both reach 4/3 further to the sides,
 * as far as the widescreen view now reaches (DlcWs_SetAspect draws x at 3/4); the game had them
 * at the 4:3 edges, which cut the field, the backdrop and the cards there. */
#define DLC_CLIP_WIDE 1.3334f
#define DLC_BEYOND(id) ((id) >= 854 && !DlcCard_IsNew(id))

/* Card effects and leader abilities (effects.c, effects.json): the rows the game reads from
 * EffIdx_M / EffIdx_S / EffIdx_L, or a card's own from effects.json. Row answers the effect row
 * to use (not 0xffff when the card has effects of its own). */
int DlcEffect_Row(int card, int row);
void *DlcEffect_Monster(int card, int row);
void *DlcEffect_Spell(int card, int row);
void *DlcEffect_Leader(int card);

/* Stacked effects (stack.c). */
unsigned int DlcStack_AtkEfc(unsigned int eID, unsigned int eParam, void *pDat, int side, int id,
                             int side2, int id2, unsigned int fAtkSide);
int DlcStack_HasType(int side, int pos, int slot, int type);
void *DlcStack_UnitPair(void *unit, int slot);

/* The title screen (modoptions.c): a fifth row, MOD OPTIONS, below the game's four, the rows
 * closer together to make room (the game's step is 0x70). */
#define DLC_TITLE_ROWS 5
#define DLC_TITLE_STEP 0x3c   /* the fifth row then ends above the description box */
void DlcModOptions_Open(void);
int DlcTitle_RowAlpha(int alpha);
int DlcTitle_Rose(int cursor, int index, int x, int y, int w, int h, float z);

/* The card shop (shop.c): the deck builder opened from its podium, its deck list the shop's cards.
 * Act answers 1 when it took this frame's button (SzDuel_ActBuildDeck then runs no state). */
int DlcShop_IsOpen(void);
/* The stock kept in the save (card_save.c). */
int DlcShop_StockSize(void);
unsigned int DlcShop_SaveEntry(int k);
int DlcShop_Stale(void);
void DlcShop_Load(const unsigned int *entries, int count, int is_stale);
void DlcShop_Reset(void);
int DlcShop_Act(void *control);
int DlcShop_Grey(int card, int bag_list, int original);
char *DlcShop_DeckText(char *original);
int DlcShop_Portrait(int cursor, int id, int x, int y, int width, int height, unsigned int z, int bright);
int DlcShop_DrawSleeve(int cursor, int base_x, int y);
int DlcShop_Detail(int cursor, int card, int x, int y, int a4, int a5, int a6, int a7);
int DlcShop_TextBox(int cursor, int x, int y, int card);
/* Drawn last on the deck builder's screen (SzDuel_DrawBuildDeck_Pipe, ported): the achievement
 * pop-up, over everything. */
int DlcShop_Last(int cursor);
int DlcShop_KeeperArt(int cursor, int x, int y, int w, int h);
/* The shop's card list (lSzDuel_DrawBuildDeck_Deck_Pipe draws only this in the shop; it answers
 * the cursor). */
int DlcShop_DeckList(int cursor, int x, int y);
/* "CARD SHOP" moves this far right, to the top middle of the screen (lSzDuel_DrawBuildDeck_Title_Pipe,
 * ported; the list slid left, the title would be at the screen's left edge). */
#define DLC_SHOP_TITLE_DX 320

#endif

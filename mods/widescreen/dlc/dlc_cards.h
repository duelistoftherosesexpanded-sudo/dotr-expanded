/* New cards: what the DLC's card code shares (cards.c, card_save.c, card_lists.c and the C that
 * tools/dlc_cards.py writes from cards.json).
 *
 * Card numbers: 0-853 are the game's own. 854-870 are the alternate pictures of the special cards
 * (SzData_ExchSpecial gives them for pictures only) and 999 means "no card", so new cards are
 * 871-998 and 1000-1531 (999 is never one). The game keeps card numbers in 16 bits (a hand entry
 * in 12), so numbers past 999 fit; its tests that took 999 and up for "no card" are changed
 * (SzDuel_DrawDeck ported, SzDuel_SetupCard_Pipe patched: mod.json). A new card copies a donor
 * card's record and uses the donor wherever the game looks something up by card number that the
 * DLC does not have (model, effects, sounds). */

#ifndef DLC_CARDS_H
#define DLC_CARDS_H

#define DLC_CARD_ORIGINAL 854  /* the game's own cards: 0-853 */
#define DLC_CARD_FIRST 871
#define DLC_CARD_NONE 999      /* "no card" */
#define DLC_CARD_LIMIT 1532    /* one past the last new card number (660 new cards, the save's room) */
#define DLC_CARD_COPIES 3      /* the most copies of a new card a player may have (card_save.c) */
#define DLC_CARD_SLOTS (DLC_CARD_LIMIT - DLC_CARD_FIRST)

/* The message table's offsets, copied into the DLC's memory with room after the game's 3,073
 * strings; the DLC's versions of SzMsg_LoadFont_Pipe and lSzMsg_FontTrace read this copy
 * (mod.json). New card k (in cards.json order) has its name in string DLC_TEXT_FIRST + 2k and
 * its description in the next. */
#define DLC_TEXT_TABLE ((int *)0x2800000)
#define DLC_TEXT_ORIGINAL 3073
#define DLC_TEXT_FIRST DLC_TEXT_ORIGINAL
#define DLC_TEXT_SLOTS 4096
#define DLC_STRING_BLOB ((unsigned short *)0x2d49d4)

/* Pictures: new card k (in cards.json order) has picture DLC_PICTURE_FIRST + k, record of that
 * number in DATA/PICTURE.MRG, which tools/mod.py extends (its own art, or a copy of the donor's).
 * Duels load a deck's pictures by number from the file; elsewhere a new card's picture is loaded
 * when it is asked for (card_pictures.c). */
#define DLC_PICTURE_FIRST 900

struct DlcCardDef {
    short number;
    short donor;
    short attack;   /* -1: the donor's */
    short defense;  /* -1: the donor's */
    short picture;
    signed char monster_type;  /* -1: the donor's; else 0-20 (low five bits of record byte 0) */
    signed char attribute;     /* -1: the donor's; else 0-5 (low four bits of byte 2) */
    signed char level;         /* -1: the donor's; else 0-15 (high four bits of byte 2) */
    short deck_cost;           /* -1: the donor's; else 0-255 (byte 3) */
    short model;               /* -1: the donor's 3D model; else its own (card_models.c) */
    short effect_from;         /* -1: the donor's effect; else that card's (record bytes 4-5) */
    char has_password;       /* typed in the deck builder, it gives a copy (lSzDuel_HitPassword) */
    unsigned char code[8];   /* the password as the card record keeps it (tools/dlc_cards.py) */
    const unsigned short *name;
    const unsigned short *desc;
};

/* The picture number SzData_ExchSpecial gives for a new card, or -1. */
int DlcCard_Picture(int id);
/* The new card's own 3D model (0, 1, ...: its 1 MiB slot after the DLC's code in DLC.BIN), or -1. */
int DlcCard_Model(int id);

extern const struct DlcCardDef dlc_cards[];
extern const int dlc_card_count;
/* Changes to the game's own cards (cards.json entries numbered 0-853): number and donor the card
 * itself, picture unused; a name or description of 0 keeps the game's; has_password 0 keeps the
 * game's password, 1 gives code, 2 none. */
extern const struct DlcCardDef dlc_card_edits[];
extern const int dlc_card_edit_count;

/* A new card's status (dlc_card_status, in the live region, which the player's setup tool
 * writes): DLC_STATUS_NORMAL; DLC_STATUS_UNOBTAINABLE (its model's game was not given): the player
 * cannot get it (shop, rewards, passwords, the starting copies); DLC_STATUS_DISABLED (its model did
 * not match this build and the player turned it off): unobtainable too, and a deck that has it
 * plays its donor instead. */
#define DLC_STATUS_NORMAL 0
#define DLC_STATUS_UNOBTAINABLE 1
#define DLC_STATUS_DISABLED 2
extern const unsigned char dlc_card_status[DLC_CARD_LIMIT];
/* A card newer than the era chosen in the title's MOD OPTIONS (eras.c) counts as disabled. */
int DlcCard_Status(int id);

/* Eras (eras.c; cards.json "era", tools/card_eras.py): 0 OG (before 2004, every game card),
 * 1 GX, 2 5D's, 3 ZEXAL, 4 ARC-V, 5 VRAINS, 6 Modern. Choosing an era plays with its cards and
 * every earlier era's. */
#define DLC_ERA_COUNT 7
extern const unsigned char dlc_card_era[DLC_CARD_LIMIT];
int DlcEra_Chosen(void);
void DlcEra_Set(int era);          /* the title's choice (kept over a loaded save's) */
void DlcEra_Load(int era);         /* a loaded save's, unless the title chose one since start */
const char *DlcEra_Name(int era);
/* The card a duel deck holds for id: the donor of a disabled card, else id. */
int DlcCard_DuelCard(int id);

/* 1 for a card number the game (with the DLC) knows: 0-853 except 671, and the new cards. */
int DlcCard_Valid(int id);
/* 1 for a new card. */
int DlcCard_IsNew(int id);
/* The new card's donor; any other number unchanged. */
int DlcCard_Donor(int id);
/* One past the highest valid card number. */
int DlcCard_End(void);
/* String numbers of a new card's name and description, or -1. */
int DlcCard_NameString(int id);
int DlcCard_DescString(int id);
/* The 20-byte card record (the game's or the DLC's copy), or 0. */
unsigned char *DlcCard_Record(int id);

#endif

/* New cards, the core: the card records with room for new numbers, the donor of each new card,
 * the text of names and descriptions, and the DLC's versions of the game's small per-card lookups
 * (hooked in mod.json). See dlc_cards.h. */

#include "dlc_cards.h"
#include "dlc_options.h"

/* The game's 854 card records of 20 bytes (gSzDuel_CardConstInfo). */
#define GAME_RECORDS ((unsigned char *)0x2bf080)
/* passwordKeyHannibal: the eight signed shifts of the password check ("HANNIBAL"). */
#define PASSWORD_SHIFTS ((signed char *)0x2bef79)
#define GAME_TEXT_TABLE ((int *)0x2d19d0)

extern int SzData_GetOption(int side, int which);
extern unsigned short AlternateCardIds[];
extern unsigned short gSzDuel_CardDefaultCamDist[];

static unsigned char records[DLC_CARD_LIMIT][20];
static unsigned char valid[DLC_CARD_LIMIT];
static short index_of[DLC_CARD_SLOTS];  /* card - DLC_CARD_FIRST -> position in dlc_cards, or -1 */
static int card_end = DLC_CARD_ORIGINAL;

static const struct DlcCardDef *def_of(int id)
{
    int k;

    if (id < DLC_CARD_FIRST || id >= DLC_CARD_LIMIT) {
        return 0;
    }
    k = index_of[id - DLC_CARD_FIRST];
    return k < 0 ? 0 : &dlc_cards[k];
}

int DlcCard_Valid(int id)
{
    return id >= 0 && id < DLC_CARD_LIMIT && valid[id];
}

int DlcCard_IsNew(int id)
{
    return def_of(id) != 0;
}

int DlcCard_Donor(int id)
{
    const struct DlcCardDef *d = def_of(id);

    return d != 0 ? d->donor : id;
}

int DlcCard_End(void)
{
    return card_end;
}

int DlcCard_Status(int id)
{
    if (!DlcCard_IsNew(id)) {
        return DLC_STATUS_NORMAL;
    }
    if (dlc_card_status[id] == DLC_STATUS_NORMAL && dlc_card_era[id] > DlcEra_Chosen()) {
        return DLC_STATUS_DISABLED;      /* newer than the era chosen (eras.c) */
    }
    return dlc_card_status[id];
}

int DlcCard_DuelCard(int id)
{
    return DlcCard_Status(id) == DLC_STATUS_DISABLED ? DlcCard_Donor(id) : id;
}

static short edit_model[DLC_CARD_ORIGINAL];  /* a game card's own model (cards.json), or -1 */

int DlcCard_Model(int id)
{
    const struct DlcCardDef *d = def_of(id);

    if (d == 0 && id >= 0 && id < DLC_CARD_ORIGINAL) {
        return edit_model[id];
    }
    return d != 0 ? d->model : -1;
}

int DlcCard_NameString(int id)
{
    if (id >= DLC_CARD_FIRST && id < DLC_CARD_LIMIT && index_of[id - DLC_CARD_FIRST] >= 0) {
        return DLC_TEXT_FIRST + 2 * index_of[id - DLC_CARD_FIRST];
    }
    return -1;
}

int DlcCard_DescString(int id)
{
    int s = DlcCard_NameString(id);

    return s < 0 ? -1 : s + 1;
}

unsigned char *DlcCard_Record(int id)
{
    if (id == 671) {
        id = 999;
    }
    if (!DlcCard_Valid(id)) {
        return 0;
    }
    return records[id];
}

/* The DLC's copy of the message offsets: the game's 3,073 strings, then the DLC's. An offset counts
 * 16-bit words from the start of the game's string blob, so text in the DLC's memory is reached
 * with a large offset. */
static void set_text(int index, const unsigned short *words)
{
    DLC_TEXT_TABLE[index] = (int)(words - DLC_STRING_BLOB);
}

void DlcText_Init(void)
{
    int i;

    for (i = 0; i < DLC_TEXT_ORIGINAL; i++) {
        DLC_TEXT_TABLE[i] = GAME_TEXT_TABLE[i];
    }
    for (; i < DLC_TEXT_ORIGINAL + DLC_TEXT_SLOTS; i++) {
        DLC_TEXT_TABLE[i] = GAME_TEXT_TABLE[0];
    }
    /* The memory card's "Saving... Do not remove the MEMORY CARD (PS2) ..." (slot 1, slot 2, slots):
     * just "Saving......", there being no memory card to take out (SzGame_ActMemCardProc, ported,
     * also shows it in place of the overwrite question). */
    {
        extern unsigned short dlc_text_saving[];
        set_text(2516, dlc_text_saving);
        set_text(2517, dlc_text_saving);
        set_text(2518, dlc_text_saving);
    }
    /* The duel settings' SOUND row is AI SPEED (aispeed.c): the sound is always stereo. */
    {
        extern unsigned short dlc_text_ai_speed[], dlc_text_ai_normal[], dlc_text_ai_fast[], dlc_text_ai_help[];
        set_text(116, dlc_text_ai_speed);
        set_text(122, dlc_text_ai_normal);
        set_text(121, dlc_text_ai_fast);
        set_text(3030, dlc_text_ai_help);
    }
}

/* A card's values from its definition over the record r (attack ... effect). */
static void apply(unsigned char *r, const struct DlcCardDef *d)
{
    /* Attack and defence: the low 13 bits of the halfwords at +8 and +10. */
    if (d->attack >= 0) {
        *(unsigned short *)(r + 8) = (*(unsigned short *)(r + 8) & ~0x1fff) | d->attack;
    }
    if (d->defense >= 0) {
        *(unsigned short *)(r + 10) = (*(unsigned short *)(r + 10) & ~0x1fff) | d->defense;
    }
    /* Record bytes 0-3: card type (top three bits, the donor's) and monster type (low five),
     * the number within the type (the donor's), attribute (low four bits) and level (high
     * four), deck cost. */
    if (d->monster_type >= 0) {
        r[0] = (r[0] & 0xe0) | (d->monster_type & 0x1f);
    }
    if (d->attribute >= 0) {
        r[2] = (r[2] & 0xf0) | (d->attribute & 0x0f);
    }
    if (d->level >= 0) {
        r[2] = (r[2] & 0x0f) | (d->level << 4);
    }
    if (d->deck_cost >= 0) {
        r[3] = d->deck_cost;
    }
    /* Bytes 4-5: the effect (wEffIdx), from another of the game's cards when asked. */
    if (d->effect_from >= 0 && d->effect_from < DLC_CARD_ORIGINAL) {
        r[4] = GAME_RECORDS[d->effect_from * 20 + 4];
        r[5] = GAME_RECORDS[d->effect_from * 20 + 5];
    }
}

/* The game's cards as cards.json changes them: record, name and description, own model. Their
 * text first goes back to the game's (a live edit may have dropped a change). */
static void apply_edits(void)
{
    int i, j, k;

    for (i = 0; i < DLC_CARD_ORIGINAL; i++) {
        DLC_TEXT_TABLE[320 + i] = GAME_TEXT_TABLE[320 + i];
        DLC_TEXT_TABLE[1174 + i] = GAME_TEXT_TABLE[1174 + i];
        edit_model[i] = -1;
    }
    for (k = 0; k < dlc_card_edit_count; k++) {
        const struct DlcCardDef *d = &dlc_card_edits[k];
        unsigned char *r;
        int n = d->number;

        if (n < 0 || n >= DLC_CARD_ORIGINAL || n == 671) {
            continue;
        }
        r = records[n];
        apply(r, d);
        if (d->has_password == 1) {
            r[9] |= 0x40;
            for (j = 0; j < 8; j++) {
                r[12 + j] = d->code[j];
            }
        } else if (d->has_password == 2) {
            r[9] &= ~0x40;
        }
        if (d->name != 0) {
            set_text(320 + n, d->name);
        }
        if (d->desc != 0) {
            set_text(1174 + n, d->desc);
        }
        edit_model[n] = d->model;
    }
}

void DlcCard_Init(void)
{
    int i, j, k;

    /* Run again after a live edit (header.c Dlc_LiveCheck): start from nothing. */
    card_end = DLC_CARD_ORIGINAL;
    for (i = DLC_CARD_ORIGINAL; i < DLC_CARD_LIMIT; i++) {
        valid[i] = 0;
    }
    for (i = 0; i < DLC_CARD_ORIGINAL; i++) {
        for (j = 0; j < 20; j++) {
            records[i][j] = GAME_RECORDS[i * 20 + j];
        }
        valid[i] = i != 671;
#if DLC_NUMBER_PASSWORDS
        if (valid[i]) {
            /* lSzDuel_HitPassword adds passwordKeyHannibal[(card + i) % 8] to byte 12 + i and
             * compares the result with the typed character; the record keeps the difference. */
            int number = i, digit;
            for (j = 7; j >= 0; j--, number /= 10) {
                digit = '0' + number % 10;
                records[i][12 + j] = (unsigned char)(digit - PASSWORD_SHIFTS[(i + j) % 8]);
            }
            records[i][9] |= 0x40;
        }
#endif
    }
    for (i = 0; i < DLC_CARD_SLOTS; i++) {
        index_of[i] = -1;
    }
    for (k = 0; k < dlc_card_count; k++) {
        const struct DlcCardDef *d = &dlc_cards[k];
        unsigned char *r;
        int n = d->number;

        if (n < DLC_CARD_FIRST || n >= DLC_CARD_LIMIT || d->donor < 0 || d->donor >= DLC_CARD_ORIGINAL) {
            continue;
        }
        r = records[n];
        for (j = 0; j < 20; j++) {
            r[j] = records[d->donor][j];
        }
        apply(r, d);
        /* The password: bit 6 of byte 9 marks a card that has one, kept in bytes 12-19. */
        r[9] &= ~0x40;
        if (d->has_password) {
            r[9] |= 0x40;
            for (j = 0; j < 8; j++) {
                r[12 + j] = d->code[j];
            }
        }
        valid[n] = 1;
        index_of[n - DLC_CARD_FIRST] = k;
        if (n + 1 > card_end) {
            card_end = n + 1;
        }
        set_text(DLC_TEXT_FIRST + 2 * k, d->name);
        set_text(DLC_TEXT_FIRST + 2 * k + 1, d->desc);
    }
    /* After the new cards, which copy their donors as the game has them. */
    apply_edits();
}

/* SzDuel_GetCardConstInfo (state_grid/func_001dd0e0.c). */
unsigned char *DlcCard_GetCardConstInfo(int cardNo)
{
    return DlcCard_Record(cardNo);
}

/* SzDuel_GetMsgName (resource_support/func_001de210.c): the name's string number. */
int DlcCard_GetMsgName(int cardNo)
{
    SzData_GetOption(0, 0);
    if (cardNo >= 0 && cardNo < DLC_CARD_ORIGINAL) {
        return cardNo + 320;
    }
    return DlcCard_NameString(cardNo);
}

/* SzDuel_GetCamDist (resource_support/func_001de260.c): the donor's for a new card. */
int DlcCard_GetCamDist(int cardId)
{
    cardId = DlcCard_Donor(cardId);
    if (cardId < 0 || cardId >= DLC_CARD_ORIGINAL) {
        return -1;
    }
    return gSzDuel_CardDefaultCamDist[cardId];
}

/* SzDuel_New2Xax (resource_support/func_001de1a0.c): a card's position in AlternateCardIds,
 * which picks its model, animations and sounds: the donor's for a new card. */
int DlcCard_New2Xax(int cardNo)
{
    int i;

    cardNo = DlcCard_Donor(cardNo);
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= DLC_CARD_ORIGINAL) {
        return 999;
    }
    for (i = 0; i < DLC_CARD_ORIGINAL; i++) {
        if (AlternateCardIds[i] == cardNo) {
            return i;
        }
    }
    return 999;
}

extern int DlcPort_lSzDuel_DrawBuildDeck_Detail_Special_Pipe(int cursor, int id, int x, int y, int flag);

/* lSzDuel_DrawBuildDeck_Detail_Special_Pipe (scaled_sprite/func_001bf180.c) draws string
 * id + 1174, the card's description; for a new card the id is shifted so that this lands on the
 * DLC's string. */
int DlcCard_DrawDetailSpecial(int cursor, int id, int x, int y, int flag)
{
    int s = DlcCard_DescString(id);

    if (s >= 0) {
        id = s - 1174;
    }
    return DlcPort_lSzDuel_DrawBuildDeck_Detail_Special_Pipe(cursor, id, x, y, flag);
}

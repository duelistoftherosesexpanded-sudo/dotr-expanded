/* MOD OPTIONS' SLEEVE row: the card back (the brown swirl of face-down cards).
 *
 * The back is card kind 7 of the card faces (data_0028ff30): the right half of card texture 2
 * (group 3, item 2 of the texture items: TEXSYS.MRG's third texture, which stays loaded) for the
 * large face, and a smaller copy on texture 4 for the small one, which is pointed here at the large
 * swirl instead (the GS scales it), so one picture makes both. tools/dlc_sleeves.py makes, from the
 * sleeves added in the launcher (mods/<mod>/sleeves/*.png), a version of the texture per sleeve in
 * the palette entries only the swirl uses; sleeves_gen.c holds the palette and the right half's
 * pixel blocks of each (sleeve 0 the game's swirl, ORIGINAL). Choosing a sleeve copies its bytes into the
 * loaded texture; screens that upload their textures afterwards (a duel) show it. The choice is
 * saved with the game (card_save.c). (The first version changed the leader's rose platform; that
 * is kept for the campaign project, mods/widescreen/campaign/platforms.c.) */

/* data_0028ff30[0][7] and [1][7], the small and large faces' back: texture byte, then u0, v0, u1,
 * v1. Only the player's cards wear the sleeve: DlcSleeves_ForCard sets both before each card is
 * drawn, to the sleeve (texture 2's right half) for owner 0 and to the game's small swirl on
 * texture 4 (left as it is) for the other side. */
#define SMALL_BACK ((unsigned char *)0x28ff76)
#define LARGE_BACK ((unsigned char *)0x28ffda)

static int want;                /* the chosen sleeve, 0 the game's */

static void set_back(unsigned char *entry, int tex, int u0, int v0, int u1, int v1)
{
    unsigned short *uv = (unsigned short *)(entry + 2);

    entry[0] = tex;
    uv[0] = u0;
    uv[1] = v0;
    uv[2] = u1;
    uv[3] = v1;
}

static void for_side(int mine);

/* Set by DlcSleeves_FieldCard for the field card about to be drawn, cleared once it is. */
static int field_card;

/* Called with the unit about to be drawn by DlcWs_SetupCard (widescreen.c). */
void DlcSleeves_ForCard(unsigned char *unit)
{
    for_side(unit != 0 && unit[0x44] == 0);
}

/* SzDuel_DrawCardRequest_Pipe (ported, mod.json), for each card of the field before it learns
 * whether the card shows its face or its back (a card showing its back is drawn without its
 * unit). */
void DlcSleeves_FieldCard(unsigned char *unit)
{
    DlcSleeves_ForCard(unit);
    field_card = 1;
}

extern int SzDuel_GetTurnOwner(void);

/* DlcWs_SetupCard for a card drawn without its unit: a field card keeps what DlcSleeves_FieldCard
 * set; any other (a card on its way from the hand to the field) is the turn owner's. */
void DlcSleeves_NoUnit(void)
{
    if (!field_card) {
        for_side(SzDuel_GetTurnOwner() == 0);
    }
}

/* After each card is drawn. */
void DlcSleeves_Drawn(void)
{
    field_card = 0;
}

static void for_side(int mine)
{
    if (want == 0 || mine) {
        set_back(SMALL_BACK, want == 0 ? 4 : 2, want == 0 ? 240 : 256, want == 0 ? 180 : 0,
                 want == 0 ? 360 : 512, want == 0 ? 360 : 368);
        set_back(LARGE_BACK, 2, 256, 0, 512, 368);
    } else {
        set_back(SMALL_BACK, 4, 240, 180, 360, 360);
        set_back(LARGE_BACK, 4, 240, 180, 360, 360);
    }
}

#include "dlc_mods.h"

struct TextureItem {
    unsigned char *data;    /* the texture file in memory; bit 31 set while loading */
    int name;
    unsigned short flags;
    short id;
};

extern struct TextureItem *SzText_GetItem(int group, int id);
extern void FlushCache(int mode);

extern const unsigned int dlc_sleeve_texture_size;
extern const int dlc_sleeve_count;
extern const int dlc_sleeve_span_count;
extern const unsigned int dlc_sleeve_spans[][2];
extern const char *const dlc_sleeve_names[];
extern const unsigned char *const dlc_sleeve_bytes[];

static unsigned char *known;    /* the texture last written, or 0 */
static int skip;

int DlcSleeves_Count(void)
{
    return dlc_sleeve_count;
}

const char *DlcSleeves_Name(int k)
{
    return k >= 0 && k < dlc_sleeve_count ? dlc_sleeve_names[k] : "";
}

int DlcSleeves_Chosen(void)
{
    return want;
}

void DlcSleeves_Update(void);

void DlcSleeves_Set(int k)
{
    want = k >= 0 && k < dlc_sleeve_count ? k : 0;
    skip = 0;
    DlcSleeves_Update();
}

/* Whether the texture at p holds sleeve k: the first and last 8 bytes of its first span. */
static int holds(const unsigned char *p, int k)
{
    const unsigned char *b = dlc_sleeve_bytes[k];
    unsigned int at = dlc_sleeve_spans[0][0], n = dlc_sleeve_spans[0][1];
    int i;

    for (i = 0; i < 8; i++) {
        if (p[at + i] != b[i] || p[at + n - 8 + i] != b[n - 8 + i]) {
            return 0;
        }
    }
    return 1;
}

void DlcSleeves_Update(void)
{
    struct TextureItem *item;
    unsigned char *p;
    const unsigned char *src;
    int s;
    unsigned int i;

    if (dlc_sleeve_count <= 1 && want == 0 && known == 0) {
        return;
    }
    if (known != 0 && holds(known, want)) {
        return;
    }
    if (skip > 0) {
        skip--;
        return;
    }
    skip = 30;
    item = SzText_GetItem(3, 2);
    if (item == 0 || item->data == 0 || ((unsigned int)item->data & 0x80000000)) {
        known = 0;
        return;
    }
    p = item->data;
    if (p[0] != 'R' || p[1] != 'H' || p[2] != '2' || *(unsigned int *)(p + 8) != dlc_sleeve_texture_size) {
        known = 0;
        return;
    }
    src = dlc_sleeve_bytes[want];
    for (s = 0; s < dlc_sleeve_span_count; s++) {
        unsigned char *dst = p + dlc_sleeve_spans[s][0];
        for (i = 0; i < dlc_sleeve_spans[s][1]; i++) {
            dst[i] = *src++;
        }
    }
    known = p;
    FlushCache(0);
}

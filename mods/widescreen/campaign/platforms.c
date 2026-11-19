/* MOD OPTIONS' SLEEVE row: what the player's own face-down cards look like.
 *
 * They show their side's rose shield, the halves of window texture 5 (group 3, item 5 of the
 * texture items: TEXSYS.MRG's 256 x 128 shield texture, which stays loaded). tools/dlc_sleeves.py
 * makes, from the sleeves added in the launcher (mods/<mod>/sleeves/*.png), a version of that
 * texture per sleeve, both halves the sleeve, with a palette of its own; sleeves_gen.c holds the
 * palette and pixel bytes of each (sleeve 0 the game's ROSE). Choosing a sleeve copies its bytes
 * into the loaded texture; screens that upload their textures afterwards (a duel) show it. The
 * choice is saved with the game (card_save.c). */

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

static int want;                /* the chosen sleeve, 0 the game's */
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
    item = SzText_GetItem(3, 5);
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

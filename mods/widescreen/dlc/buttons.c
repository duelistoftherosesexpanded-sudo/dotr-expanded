/* MOD OPTIONS' BUTTONS row: the game's button pictures as Xbox ones.
 *
 * The pictures are tiles of window texture 7 (group 3, item 7 of the texture items: the last
 * texture of DATA/TEXSYS.MRG, which stays loaded). tools/dlc_buttons.py lists the bytes of that
 * texture file that differ in the Xbox version, with both values (buttons_gen.c, made at build
 * time from the disc). Choosing XBOX writes the Xbox bytes into the loaded texture, PLAYSTATION
 * the game's; the next time a screen uploads its textures (opening a menu, a duel ...) it shows
 * them. The palette is not changed. The choice is saved with the game (card_save.c). */

#include "dlc_mods.h"

struct TextureItem {
    unsigned char *data;    /* the texture file in memory; bit 31 set while loading */
    int name;
    unsigned short flags;   /* three flags and the group (struct DotrEntryTableEntry) */
    short id;
};

extern struct TextureItem *SzText_GetItem(int group, int id);
extern void FlushCache(int mode);

extern const unsigned int dlc_button_texture_size;
extern const int dlc_button_count;
extern const unsigned int dlc_button_at[];
extern const unsigned char dlc_button_ps[];
extern const unsigned char dlc_button_xbox[];

static int want;                    /* the choice */
static unsigned char *known;        /* the texture last written, or 0 */
static int skip;                    /* calls left before looking for the texture again */

int DlcButtons_Xbox(void)
{
    return want;
}

void DlcButtons_Update(void);

void DlcButtons_Set(int xbox)
{
    want = xbox != 0;
    skip = 0;
    DlcButtons_Update();
}

void DlcButtons_Update(void)
{
    const unsigned char *bytes = want ? dlc_button_xbox : dlc_button_ps;
    struct TextureItem *item;
    unsigned char *p;
    int k;

    if (dlc_button_count <= 0) {
        return;
    }
    /* Quick check: the first changed byte of the texture written last. */
    if (known != 0 && known[dlc_button_at[0]] == bytes[0]) {
        return;
    }
    if (skip > 0) {
        skip--;
        return;
    }
    skip = 30;
    item = SzText_GetItem(3, 7);
    if (item == 0 || item->data == 0 || ((unsigned int)item->data & 0x80000000)) {
        known = 0;
        return;
    }
    p = item->data;
    /* The texture file this was made from: "RH2" and its size. */
    if (p[0] != 'R' || p[1] != 'H' || p[2] != '2' || *(unsigned int *)(p + 8) != dlc_button_texture_size) {
        known = 0;
        return;
    }
    for (k = 0; k < dlc_button_count; k++) {
        p[dlc_button_at[k]] = bytes[k];
    }
    known = p;
    FlushCache(0);
}

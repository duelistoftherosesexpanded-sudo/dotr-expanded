/* MOD OPTIONS: a fifth row on the title screen, and the community patches it chooses.
 *
 * The title (lSzGame_ActTitle, lSzGame_DrawTitle_Pipe) is ported (mod.json) with five rows instead
 * of four, closer together, and hooked to DlcTitle_Act / DlcTitle_Draw here. The game's four rows
 * are pictures; the fifth is drawn as text. Choosing it opens a panel over the title:
 *
 *   (GAME PATCH and its extras: no longer listed; the game is always SPEX, DlcMods_Init)
 *   CARD ERA       the newest era of cards played with, OG to MODERN (eras.c)
 *   BUTTONS        PLAYSTATION or XBOX button pictures (buttons.c)
 *   (SLEEVE         no longer listed: the card shop chooses the sleeve, shop.c)
 *   DONE            (or CIRCLE) closes the panel and applies the choice
 *
 * A patch is runs of bytes of the executable (tools/dlc_community.py): applying writes the patched
 * bytes into memory, removing writes the original ones back. The DLC's own copies of the game's
 * card records and message offsets are taken again afterwards (DlcText_Init, DlcCard_Init), and
 * the caches are flushed since code may have changed. The choice is saved with the game
 * (card_save.c) and a loaded save brings back the choice it was made with. */

#include "dlc_mods.h"
#include "dlc_port.h"
#include "dlc_cards.h"

extern int SzPad_Check(int pad, int mode, int mask);
extern int SzSound_PlaySE(int channel, int id);
extern int SzDuel_DrawWindow_Font_Pipe(int cursor, int x, int y, int font, int flags, int count,
                                       signed char *text, unsigned char *colour);
extern int lSzGame_DrawTitle_Rose_Pipe(int cursor, int index, int x, int y, int w, int h, float z);
extern int DlcPort_lSzGame_DrawTitle_Pipe(int cursor);
extern void DlcPort_lSzGame_ActTitle(void);
extern void FlushCache(int mode);
extern void DlcText_Init(void);
extern void DlcCard_Init(void);

#define TITLE_ALPHA (*(unsigned char *)(0x317250 + 0x226))
#define TITLE_SELECTED (*(unsigned char *)(0x317250 + 0x225))

#define PAD_UP 0x1000
#define PAD_DOWN 0x4000
#define PAD_LEFT 0x8000
#define PAD_RIGHT 0x2000
#define PAD_CROSS 0x40
#define PAD_CIRCLE 0x20

#define MAX_PATCHES 16
/* Duelists of the Roses Expanded's version, shown on the title screen. */
#define DLC_VERSION "VER 0.55"

static int applied_main, applied_extras;   /* what memory holds */
static int pick_main, pick_extras;         /* what the panel shows */
static int pick_buttons;                   /* BUTTONS: 0 PlayStation, 1 Xbox (buttons.c) */
static int pick_sleeve;                    /* SLEEVE: 0 the game's rose, k a sleeve (sleeves.c) */
static int pick_era;                       /* CARD ERA: the newest era of cards played with (eras.c) */
static int panel_open, panel_row;

/* ---- the patches ---- */

static int main_count(void)
{
    int k, n = 0;

    for (k = 0; k < dlc_mod_patch_count; k++) {
        n += dlc_mod_patches[k].kind == 0;
    }
    return n;
}

/* Patch index of main option m (1-based), or -1. */
static int main_patch(int m)
{
    int k, n = 0;

    for (k = 0; k < dlc_mod_patch_count; k++) {
        if (dlc_mod_patches[k].kind == 0 && ++n == m) {
            return k;
        }
    }
    return -1;
}

/* Patch index of extra e (0-based, its bit), or -1. */
static int extra_patch(int e)
{
    int k, n = 0;

    for (k = 0; k < dlc_mod_patch_count; k++) {
        if (dlc_mod_patches[k].kind < 0 && n++ == e) {
            return k;
        }
    }
    return -1;
}

static int extra_count(void)
{
    return dlc_mod_patch_count - main_count();
}

/* Whether extra e goes with main option m. */
static int extra_fits(int e, int m)
{
    int k = extra_patch(e);

    return k >= 0 && dlc_mod_patches[k].kind == -1 - m;
}

static void write_patch(int k, int on)
{
    const struct DlcModPatch *p = &dlc_mod_patches[k];
    int r;
    unsigned int i;

    for (r = p->first; r < p->first + p->count; r++) {
        const struct DlcModRun *run = &dlc_mod_runs[r];
        const unsigned char *src = dlc_mod_bytes + run->at + (on ? run->length : 0);
        unsigned char *dst = (unsigned char *)run->address;
        for (i = 0; i < run->length; i++) {
            dst[i] = src[i];
        }
    }
}

int DlcMods_Main(void)
{
    return applied_main;
}

int DlcMods_Extras(void)
{
    return applied_extras;
}

/* The game is SPEX, always: the first main patch of community.json, without extras (a save made
 * with another choice is played with it too). */
#define ALWAYS_MAIN (main_count() > 0 ? 1 : 0)

void DlcMods_Init(void)
{
    DlcMods_Set(ALWAYS_MAIN, 0);
}

void DlcMods_Set(int main, int extras)
{
    int e, n = extra_count();

    main = ALWAYS_MAIN;
    extras = 0;
    for (e = 0; e < n; e++) {
        if (!extra_fits(e, main)) {
            extras &= ~(1 << e);
        }
    }
    if (main == applied_main && extras == applied_extras) {
        return;
    }
    for (e = n - 1; e >= 0; e--) {
        if (applied_extras & (1 << e)) {
            write_patch(extra_patch(e), 0);
        }
    }
    if (applied_main > 0) {
        write_patch(main_patch(applied_main), 0);
    }
    if (main > 0) {
        write_patch(main_patch(main), 1);
    }
    for (e = 0; e < n; e++) {
        if (extras & (1 << e)) {
            write_patch(extra_patch(e), 1);
        }
    }
    applied_main = main;
    applied_extras = extras;
    FlushCache(0);
    FlushCache(2);
    DlcText_Init();
    DlcCard_Init();
}

/* ---- the panel ---- */

/* Text: the title screen has no font loaded (its words are pictures), so the DLC draws its own,
 * a 5 x 7 dot font, each dot `scale` screen dots, as flat rectangles of the kind the title
 * uses for its shading (GS sprite, untextured: tag 3 / 6 / 0x441). A glyph is cut into a few
 * rectangles (each run of dots grown down as far as the same run repeats). */
extern int ExSortPrim_Pipe(int cursor, void *packet, int table, int z);
extern int ExAssignAlpha(void *packet, long long alpha);
extern long long ExGetAlphaVal(int which);

struct FlatRect {
    unsigned long long tag[2];
    unsigned char r, g, b, a;
    float q;
    unsigned long long xyz[2];
    unsigned long long reserved;
};

/* The title draws the 640 x 448 screen at GS (1728, 1824) - (2368, 2272), in sixteenths. */
#define SCREEN_X 1728
#define SCREEN_Y 1824

static int rect(int cursor, int x, int y, int w, int h, const unsigned char *colour)
{
    struct FlatRect p;
    unsigned long long z = 0xfffff8;

    p.tag[0] = 3 | (6 << 16);
    p.tag[1] = 0x441;
    ExAssignAlpha(&p, ExGetAlphaVal(2));
    p.r = colour[0];
    p.g = colour[1];
    p.b = colour[2];
    p.a = colour[3];
    p.q = 1.0f;
    p.xyz[0] = (unsigned long long)((SCREEN_X + x) << 4 & 0xffff)
               | (unsigned long long)((SCREEN_Y + y) << 4 & 0xffff) << 16 | z << 32;
    p.xyz[1] = (unsigned long long)((SCREEN_X + x + w) << 4 & 0xffff)
               | (unsigned long long)((SCREEN_Y + y + h) << 4 & 0xffff) << 16 | z << 32;
    p.reserved = 0;
    return ExSortPrim_Pipe(cursor, &p, 0, 0);
}

static const unsigned char glyphs[][7] = {
    {0x0e, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11}, {0x1e, 0x11, 0x11, 0x1e, 0x11, 0x11, 0x1e},  /* A B */
    {0x0e, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0e}, {0x1e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1e},  /* C D */
    {0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f}, {0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x10},  /* E F */
    {0x0e, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0f}, {0x11, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11},  /* G H */
    {0x0e, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e}, {0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0c},  /* I J */
    {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11}, {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f},  /* K L */
    {0x11, 0x1b, 0x15, 0x15, 0x11, 0x11, 0x11}, {0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11},  /* M N */
    {0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e}, {0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10, 0x10},  /* O P */
    {0x0e, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0d}, {0x1e, 0x11, 0x11, 0x1e, 0x14, 0x12, 0x11},  /* Q R */
    {0x0f, 0x10, 0x10, 0x0e, 0x01, 0x01, 0x1e}, {0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04},  /* S T */
    {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e}, {0x11, 0x11, 0x11, 0x11, 0x11, 0x0a, 0x04},  /* U V */
    {0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0a}, {0x11, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x11},  /* W X */
    {0x11, 0x11, 0x11, 0x0a, 0x04, 0x04, 0x04}, {0x1f, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1f},  /* Y Z */
    {0x0e, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0e}, {0x04, 0x0c, 0x04, 0x04, 0x04, 0x04, 0x0e},  /* 0 1 */
    {0x0e, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1f}, {0x1f, 0x02, 0x04, 0x02, 0x01, 0x11, 0x0e},  /* 2 3 */
    {0x02, 0x06, 0x0a, 0x12, 0x1f, 0x02, 0x02}, {0x1f, 0x10, 0x1e, 0x01, 0x01, 0x11, 0x0e},  /* 4 5 */
    {0x06, 0x08, 0x10, 0x1e, 0x11, 0x11, 0x0e}, {0x1f, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08},  /* 6 7 */
    {0x0e, 0x11, 0x11, 0x0e, 0x11, 0x11, 0x0e}, {0x0e, 0x11, 0x11, 0x0f, 0x01, 0x02, 0x0c},  /* 8 9 */
    {0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c},  /* - . */
    {0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02}, {0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08},  /* < > */
};

static const unsigned char *glyph(char c)
{
    if (c >= 'a' && c <= 'z') c -= 'a' - 'A';
    if (c >= 'A' && c <= 'Z') return glyphs[c - 'A'];
    if (c >= '0' && c <= '9') return glyphs[26 + c - '0'];
    if (c == '-') return glyphs[36];
    if (c == '.') return glyphs[37];
    if (c == '<') return glyphs[38];
    if (c == '>') return glyphs[39];
    return 0;
}

static int draw_glyph(int cursor, int x, int y, int scale, const unsigned char *g, const unsigned char *colour)
{
    unsigned char left[7];
    int r, c, w, h, k;

    for (r = 0; r < 7; r++) {
        left[r] = g[r];
    }
    for (r = 0; r < 7; r++) {
        for (c = 0; c < 5; c++) {
            unsigned char bit = 0x10 >> c, run = 0;
            if (!(left[r] & bit)) {
                continue;
            }
            for (w = 0; c + w < 5 && (left[r] & (0x10 >> (c + w))); w++) {
                run |= 0x10 >> (c + w);
            }
            for (h = 1; r + h < 7 && (left[r + h] & run) == run; h++) {
            }
            for (k = 0; k < h; k++) {
                left[r + k] &= ~run;
            }
            cursor = rect(cursor, x + c * scale, y + r * scale, w * scale, h * scale, colour);
        }
    }
    return cursor;
}

static int text_width(const char *a, const char *b, int scale)
{
    int n = 0;

    while (*a++) n++;
    while (b && *b++) n++;
    return n * 6 * scale - scale;
}

/* `a` then `b`, centred on x, from y down. */
static int text_in(int cursor, int x, int y, int scale, const char *a, const char *b, const unsigned char *colour)
{
    int pass;

    x -= text_width(a, b, scale) / 2;
    for (pass = 0; pass < 2; pass++) {
        const char *s = pass ? b : a;
        for (; s && *s; s++, x += 6 * scale) {
            const unsigned char *g = glyph(*s);
            if (g) {
                cursor = draw_glyph(cursor, x, y, scale, g, colour);
            }
        }
    }
    return cursor;
}

static const unsigned char WHITE[4] = {128, 128, 128, 128};
static const unsigned char GREY[4] = {80, 80, 80, 128};
static const unsigned char GOLD[4] = {128, 110, 40, 128};
static const unsigned char SHADE[4] = {0, 0, 0, 100};

static int text(int cursor, int x, int y, const char *a, const char *b)
{
    return text_in(cursor, x, y, 2, a, b, WHITE);
}

/* The panel's rows: 0 the main patch, then the extras that fit it, BUTTONS (-3), then DONE (-2). */
static int rows(int *which)
{
    int n = 0;

    /* No GAME PATCH row or patch extras: the game is always SPEX (DlcMods_Init). */
    which[n++] = -5;          /* CARD ERA (eras.c) */
    which[n++] = -3;
    /* The SLEEVE row (-4) is left out: the card shop chooses the sleeve now (shop.c). */
    which[n++] = -2;
    return n;
}

void DlcModOptions_Open(void)
{
    panel_open = 1;
    panel_row = 0;
    pick_main = applied_main;
    pick_extras = applied_extras;
    pick_buttons = DlcButtons_Xbox();
    pick_sleeve = DlcSleeves_Chosen();
    pick_era = DlcEra_Chosen();
}

static void close_panel(void)
{
    panel_open = 0;
    DlcMods_Set(pick_main, pick_extras);
    DlcButtons_Set(pick_buttons);
    DlcSleeves_Set(pick_sleeve);
    if (pick_era != DlcEra_Chosen()) {
        DlcEra_Set(pick_era);      /* kept over the era a save is loaded with */
    }
}

static void panel_act(void)
{
    int which[MAX_PATCHES + 4];
    int n = rows(which), w, step = 0;

    if (panel_row >= n) {
        panel_row = n - 1;
    }
    if (SzPad_Check(0, 2, PAD_UP)) {
        panel_row = (panel_row + n - 1) % n;
        SzSound_PlaySE(0, 3);
        return;
    }
    if (SzPad_Check(0, 2, PAD_DOWN)) {
        panel_row = (panel_row + 1) % n;
        SzSound_PlaySE(0, 3);
        return;
    }
    if (SzPad_Check(0, 0, PAD_CIRCLE)) {
        SzSound_PlaySE(0, 5);
        close_panel();
        return;
    }
    if (SzPad_Check(0, 2, PAD_LEFT)) {
        step = -1;
    } else if (SzPad_Check(0, 2, PAD_RIGHT)) {
        step = 1;
    } else if (SzPad_Check(0, 0, PAD_CROSS)) {
        step = 2;
    }
    if (step == 0) {
        return;
    }
    w = which[panel_row];
    if (w == -2) {
        if (step == 2) {
            SzSound_PlaySE(0, 4);
            close_panel();
        }
        return;
    }
    if (w == -1) {
        int count = main_count() + 1;
        pick_main = (pick_main + (step < 0 ? count - 1 : 1)) % count;
    } else if (w == -3) {
        pick_buttons ^= 1;
    } else if (w == -5) {
        pick_era = (pick_era + (step < 0 ? DLC_ERA_COUNT - 1 : 1)) % DLC_ERA_COUNT;
    } else if (w == -4) {
        int count = DlcSleeves_Count();
        pick_sleeve = (pick_sleeve + (step < 0 ? count - 1 : 1)) % count;
    } else {
        pick_extras ^= 1 << w;
    }
    SzSound_PlaySE(0, 3);
}

static int panel_draw(int cursor)
{
    int which[MAX_PATCHES + 4];
    int n = rows(which), r, y = 96;
    char line[48];

    cursor = rect(cursor, 64, 24, 512, 72 + 30 * n + 40, SHADE);
    cursor = text_in(cursor, 320, 40, 4, "MOD OPTIONS", 0, GOLD);
    for (r = 0; r < n; r++, y += 30) {
        const char *label, *value = 0;
        int w = which[r], k = 0;

        if (w == -1) {
            label = "GAME PATCH  ";
            value = pick_main == 0 ? "ORIGINAL" : dlc_mod_patches[main_patch(pick_main)].name;
        } else if (w == -2) {
            label = "DONE";
        } else if (w == -3) {
            label = "BUTTONS  ";
            value = pick_buttons ? "XBOX" : "PLAYSTATION";
        } else if (w == -5) {
            label = "CARD ERA  ";
            value = DlcEra_Name(pick_era);
        } else if (w == -4) {
            label = "SLEEVE  ";
            value = DlcSleeves_Name(pick_sleeve);
        } else {
            label = dlc_mod_patches[extra_patch(w)].name;
            value = pick_extras & (1 << w) ? "  ON" : "  OFF";
        }
        if (r == panel_row) {
            line[k++] = '>';
            line[k++] = ' ';
        }
        while (*label && k < 40) {
            line[k++] = *label++;
        }
        while (value && *value && k < 44) {
            line[k++] = *value++;
        }
        if (r == panel_row) {
            line[k++] = ' ';
            line[k++] = '<';
        }
        line[k] = 0;
        cursor = text_in(cursor, 320, y, 2, line, 0, r == panel_row ? GOLD : WHITE);
    }
    cursor = text_in(cursor, 320, y + 10, 2, pick_buttons ? "LEFT RIGHT CHANGE   A DONE" : "LEFT RIGHT CHANGE   X DONE",
                     0, GREY);
    return cursor;
}

/* ---- the title screen ---- */

void DlcTitle_Act(void)
{
    if (panel_open) {
        panel_act();
        return;
    }
    DlcPort_lSzGame_ActTitle();
}

int DlcTitle_Draw(int cursor)
{
    int y = 32 + 4 * DLC_TITLE_STEP;   /* the game's rows start 32 dots down, 48 high */

    DlcButtons_Update();
    DlcSleeves_Update();
    cursor = DlcPort_lSzGame_DrawTitle_Pipe(cursor);
    /* The mod's version, in the bottom right corner of the title picture, always. */
    cursor = text_in(cursor, 640 - 16 - text_width(DLC_VERSION, 0, 2) / 2, 448 - 30, 2, DLC_VERSION, 0, GREY);
    if (TITLE_ALPHA < 0x60) {
        return cursor;
    }
    if (panel_open) {
        return panel_draw(cursor);
    }
    /* The fifth row: a dark band as wide as the game's rows (256), the words lit when chosen. */
    cursor = rect(cursor, 192, y, 256, 48, SHADE);
    return text_in(cursor, 320, y + 13, 3, "MOD OPTIONS", 0, TITLE_SELECTED == 4 ? WHITE : GREY);
}

/* While the panel is open the game's rows, frame and roses are not drawn. */
int DlcTitle_RowAlpha(int alpha)
{
    return panel_open ? 0 : alpha;
}

int DlcTitle_Rose(int cursor, int index, int x, int y, int w, int h, float z)
{
    if (panel_open) {
        return cursor;
    }
    return lSzGame_DrawTitle_Rose_Pipe(cursor, index, x, y, w, h, z);
}

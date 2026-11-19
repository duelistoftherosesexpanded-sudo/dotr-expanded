/* Monster card frames in more colours (cards.json "frame", tools/dlc_frames.py).
 *
 * A card face is drawn by SzDuel_SetupCard_Pipe (wrapped, widescreen.c DlcWs_SetupCard): its frame
 * from the card face table (data_0028ff30[size][kind]: kind 0 the normal monster, 6 the effect
 * monster) on texture 0 (large, uploaded for the card by lSzDuel_SetupCardTexture) or 4 (small,
 * always loaded).
 *
 *   normal / effect  the face table's entry 6 is copied over entry 0 (or the other way) while the
 *                    card is drawn (DlcFrame_Before / DlcFrame_After);
 *   a new colour     before the card is drawn its frame texture is made sure of and the colour's
 *                    palette uploaded (frames_gen.c: the texture file cut to its palette transfer,
 *                    recoloured; SzText_Load_Pipe gives it a palette address). The function's six
 *                    calls of ExAssignTexture are pointed here (DlcFrame_Install): the first one
 *                    for the frame texture gets a copy of the texture's header whose TEX0 names
 *                    that palette; the picture itself stays where it is. */

extern int ExAssignTexture(void *packet, void *texture, int a);
extern int SzText_Load_Pipe(int cursor, void *texture, int gs, int used);
extern int RhText_SetTextureFlush(int cursor);
extern int RhText_BindAddress(void *texture);
extern int lSzDuel_SetupCardTexture(int cursor, int texture);
extern void FlushCache(int mode);
#include "dlc_cards.h"

extern const unsigned char dlc_card_frame[DLC_CARD_LIMIT];
extern const int dlc_frame_colour_count;
extern const unsigned char *const dlc_frame_palettes[][2];

#define CONTROL ((unsigned char *)0x317490)
#define SLOTS ((void **)(CONTROL + 0x6a0))     /* the loaded textures by number */
#define CLUT_BASE (*(int *)(CONTROL + 0xc80))  /* lSzDuel_SetupCardTexture's upload base */
#define FACE_TABLE ((unsigned char *)0x28ff30) /* data_0028ff30[2][10], 10 bytes an entry */
#define FACE(size, kind) (FACE_TABLE + (size) * 100 + (kind) * 10)
#define FRAME_NORMAL 1
#define FRAME_EFFECT 2
#define FRAME_FIRST_COLOUR 3
#define CBP_MASK (0x3fffULL << 37)             /* TEX0's palette address */
#define LARGE_TEXTURE 0
#define SMALL_TEXTURE 4

/* SzDuel_SetupCard_Pipe's calls of ExAssignTexture (0x208700), found in the executable. */
static const unsigned int call_sites[] = {0x1daab4, 0x1dac60, 0x1daff0, 0x1db190, 0x1db440, 0x1db96c};
#define JAL(target) (0x0c000000 | (((unsigned int)(target) >> 2) & 0x3ffffff))

int DlcFrame_AssignTexture(void *packet, void *texture, int a);

static int installed;
static unsigned char header[0x60] __attribute__((aligned(16)));
static void *pending_texture;          /* the frame texture to draw with the palette below */
static unsigned long long pending_cbp;
static unsigned char saved[10];
static unsigned char *restore_at;

void DlcFrame_Install(void)
{
    unsigned int i;

    if (installed) {
        return;
    }
    for (i = 0; i < sizeof(call_sites) / sizeof(call_sites[0]); i++) {
        unsigned int *p = (unsigned int *)call_sites[i];
        if (*p == JAL(ExAssignTexture)) {
            *p = JAL(DlcFrame_AssignTexture);
        }
    }
    FlushCache(0);
    FlushCache(2);
    installed = 1;
}

static void copy10(unsigned char *to, const unsigned char *from)
{
    int i;

    for (i = 0; i < 10; i++) {
        to[i] = from[i];
    }
}

/* Before SzDuel_SetupCard_Pipe draws a card (unit, draw settings). Returns the cursor. */
int DlcFrame_Before(int cursor, const unsigned char *unit, const unsigned char *draw)
{
    const unsigned char *r;
    unsigned char *pal;
    int id, f, large, c, tex;

    DlcFrame_Install();
    pending_texture = 0;
    restore_at = 0;
    if (unit == 0 || draw == 0) {
        return cursor;
    }
    id = *(const unsigned short *)(unit + 0x3c);
    if (id >= DLC_CARD_LIMIT || (f = dlc_card_frame[id]) == 0) {
        return cursor;
    }
    r = DlcCard_Record(id);
    if (r == 0 || (r[0] >> 5) != 0) {
        return cursor;                       /* monsters only */
    }
    large = (draw[0x81] >> 4) & 1;
    if (f == FRAME_NORMAL || f == FRAME_EFFECT) {
        /* The other frame's entry takes this one's place while the card is drawn. */
        restore_at = FACE(large, f == FRAME_NORMAL ? 6 : 0);
        copy10(saved, restore_at);
        copy10(restore_at, FACE(large, f == FRAME_NORMAL ? 0 : 6));
        return cursor;
    }
    c = f - FRAME_FIRST_COLOUR;
    if (c >= dlc_frame_colour_count) {
        return cursor;
    }
    tex = large ? LARGE_TEXTURE : SMALL_TEXTURE;
    if (large) {
        cursor = lSzDuel_SetupCardTexture(cursor, LARGE_TEXTURE);
    }
    if (SLOTS[tex] == 0) {
        return cursor;
    }
    pal = (unsigned char *)dlc_frame_palettes[c][tex == SMALL_TEXTURE];
    if (*(unsigned int *)(pal + 0x0c) != (unsigned int)pal) {
        RhText_BindAddress(pal);
    }
    cursor = SzText_Load_Pipe(cursor, pal, CLUT_BASE, 0);
    cursor = RhText_SetTextureFlush(cursor);
    pending_texture = SLOTS[tex];
    pending_cbp = *(unsigned long long *)(pal + 0x30) & CBP_MASK;
    return cursor;
}

void DlcFrame_After(void)
{
    if (restore_at != 0) {
        copy10(restore_at, saved);
        restore_at = 0;
    }
    pending_texture = 0;
}

/* SzDuel_SetupCard_Pipe's ExAssignTexture: the frame (the first use of its texture) with the
 * colour's palette. */
int DlcFrame_AssignTexture(void *packet, void *texture, int a)
{
    unsigned long long *t0;
    int i;

    if (pending_texture == 0 || texture != pending_texture) {
        return ExAssignTexture(packet, texture, a);
    }
    pending_texture = 0;
    for (i = 0; i < 0x60; i++) {
        header[i] = ((unsigned char *)texture)[i];
    }
    t0 = (unsigned long long *)(header + 0x30);
    *t0 = (*t0 & ~CBP_MASK) | pending_cbp;
    return ExAssignTexture(packet, header, a);
}

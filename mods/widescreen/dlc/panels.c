/* The chest's rows and the card detail panel in the frame colour of the card (cards.json "frame",
 * as the card faces have them on the field and in the hand, frames.c).
 *
 * The deck builder's row of a card (lSzDuel_DrawBuildDeck_Card_Pipe) and its detail panel
 * (lSzDuel_DrawBuildDeck_Detail_Pipe), both ported, colour a monster's panel from the window colour
 * table (gSzDuel_aaicolRgba, 0x2be840: per row two colours, blue green red alpha each) by its kind:
 * row 7 a monster, row 11 a fusion. They are hooked here: while one draws a monster whose frame is
 * set, rows 7 and 11 hold the frame's colour, and are put back after. */

#include "dlc_cards.h"

extern const unsigned char dlc_card_frame[DLC_CARD_LIMIT];
extern int DlcPort_lSzDuel_DrawBuildDeck_Card_Pipe(int cursor, int card, int x, int y, int flag, int flag2);
extern int DlcPort_lSzDuel_DrawBuildDeck_Detail_Pipe(int cursor, int card, int x, int y, int a4, int a5, int a6, int a7);

#define COLOURS ((unsigned char *)0x2be840)
#define ROW_MONSTER 7
#define ROW_FUSION 11

/* Red, green, blue of each frame (tools/dlc_frames.py FRAMES): automatic, normal, effect, red,
 * ritual, link, white (synchro), purple (fusion), black (xyz). */
static const unsigned char frame_rgb[][3] = {
    {0, 0, 0}, {160, 128, 0}, {176, 84, 24}, {176, 24, 24}, {64, 96, 176},
    {24, 56, 150}, {170, 170, 170}, {128, 48, 150}, {44, 44, 52},
};

static unsigned char saved[2][8];
static int changed;

static void begin(int card)
{
    const unsigned char *r;
    const unsigned char *c;
    int f, k, rows[2] = {ROW_MONSTER, ROW_FUSION};

    changed = 0;
    if (card < 0 || card >= DLC_CARD_LIMIT || (f = dlc_card_frame[card]) == 0
        || f >= (int)(sizeof(frame_rgb) / sizeof(frame_rgb[0]))) {
        return;
    }
    r = DlcCard_Record(card);
    if (r == 0 || (r[0] >> 5) != 0) {
        return;                              /* monsters only */
    }
    c = frame_rgb[f];
    for (k = 0; k < 2; k++) {
        unsigned char *row = COLOURS + rows[k] * 8;
        int i;
        for (i = 0; i < 8; i++) {
            saved[k][i] = row[i];
        }
        /* Two colours (top, bottom): the frame's, and a little darker below. */
        row[0] = c[2]; row[1] = c[1]; row[2] = c[0]; row[3] = 128;
        row[4] = c[2] * 3 / 4; row[5] = c[1] * 3 / 4; row[6] = c[0] * 3 / 4; row[7] = 128;
    }
    changed = 1;
}

static void end(void)
{
    int k, i, rows[2] = {ROW_MONSTER, ROW_FUSION};

    if (!changed) {
        return;
    }
    for (k = 0; k < 2; k++) {
        for (i = 0; i < 8; i++) {
            COLOURS[rows[k] * 8 + i] = saved[k][i];
        }
    }
    changed = 0;
}

int DlcPanel_Card(int cursor, int card, int x, int y, int flag, int flag2)
{
    begin(card);
    cursor = DlcPort_lSzDuel_DrawBuildDeck_Card_Pipe(cursor, card, x, y, flag, flag2);
    end();
    return cursor;
}

int DlcPanel_Detail(int cursor, int card, int x, int y, int a4, int a5, int a6, int a7)
{
    begin(card);
    cursor = DlcPort_lSzDuel_DrawBuildDeck_Detail_Pipe(cursor, card, x, y, a4, a5, a6, a7);
    end();
    return cursor;
}

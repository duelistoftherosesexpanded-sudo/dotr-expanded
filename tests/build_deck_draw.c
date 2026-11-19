/* Synthetic host tests for the build_deck_draw group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/build_deck_draw.h"

struct DotrCaptionControl data_00317490_caption;
#define C data_00317490_caption

static unsigned short bag[64];
static unsigned short deck[64];

/* Every callee appends one line "name args" to the log; the pipe callees check they got the
 * cursor the previous one returned and return it plus one. */
static char log_text[4096];
static DotrCaptionWord expect_cursor;
static int bag_count, look_flag, count_side, count_id, count_kind, look_side, look_id;

static void note(const char *line)
{
    assert(strlen(log_text) + strlen(line) + 2 < sizeof log_text);
    strcat(log_text, line);
    strcat(log_text, "\n");
}
static DotrCaptionWord pipe(DotrCaptionWord cursor, const char *line)
{
    assert(cursor == expect_cursor);
    note(line);
    return ++expect_cursor;
}
static char buf[128];

int RhBase_AssignRenderFrame(int index, int mode, int kind)
{
    sprintf(buf, "assign %d %d %d", index, mode, kind);
    note(buf);
    return 0;
}
DotrCaptionWord RhBase_EnableRenderFrame(DotrCaptionWord cursor)
{
    return pipe(cursor, "enable");
}
DotrCaptionWord RhBase_ClearRenderFrame(DotrCaptionWord cursor, void *color)
{
    assert(color == NULL);
    return pipe(cursor, "clear");
}
DotrCaptionWord RhBase_DisableRenderFrame(DotrCaptionWord cursor)
{
    return pipe(cursor, "disable");
}
DotrCaptionWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrCaptionWord cursor, int a1)
{
    sprintf(buf, "session %d", a1);
    return pipe(cursor, buf);
}
DotrCaptionWord RhBase_MaskFBuffer(DotrCaptionWord cursor, unsigned int mask)
{
    sprintf(buf, "mask %08x", mask);
    return pipe(cursor, buf);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_Base_Pipe(DotrCaptionWord cursor)
{
    return pipe(cursor, "base");
}
DotrCaptionTitleWord lSzDuel_DrawBuildDeck_Title_Pipe(DotrCaptionTitleWord cursor, int x, int y)
{
    sprintf(buf, "title %d %d", x, y);
    return (DotrCaptionTitleWord)pipe((DotrCaptionWord)cursor, buf);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_Library_Pipe(DotrCaptionWord cursor, int x, int y)
{
    sprintf(buf, "library %d %d", x, y);
    return pipe(cursor, buf);
}
int SzData_GetBagCount(int side, int id, int kind)
{
    count_side = side;
    count_id = id;
    count_kind = kind;
    return bag_count;
}
int SzData_GetLookFlag(int side, int id)
{
    look_side = side;
    look_id = id;
    return look_flag;
}
DotrCaptionWord lSzDuel_DrawBuildDeck_Detail_Pipe(DotrCaptionWord cursor, int card, int x, int a, int b, int c, int d, int e)
{
    sprintf(buf, "detail %d %d %d %d %d %d %d", card, x, a, b, c, d, e);
    return pipe(cursor, buf);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_Bag_Pipe(DotrCaptionWord cursor, int x, int y)
{
    sprintf(buf, "bag %d %d", x, y);
    return pipe(cursor, buf);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_Deck_Pipe(DotrCaptionWord cursor, int x, int y)
{
    sprintf(buf, "deck %d %d", x, y);
    return pipe(cursor, buf);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_Ghost_Pipe(DotrCaptionWord cursor, int x, int y)
{
    sprintf(buf, "ghost %d %d", x, y);
    return pipe(cursor, buf);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_BagCursor_Pipe(DotrCaptionWord cursor, int x, int y, int cur)
{
    sprintf(buf, "bagcursor %d %d %d", x, y, cur);
    return pipe(cursor, buf);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_DeckCursor_Pipe(DotrCaptionWord cursor, int x, int y, int cur)
{
    sprintf(buf, "deckcursor %d %d %d", x, y, cur);
    return pipe(cursor, buf);
}
static DotrCaptionWord caption(DotrCaptionWord cursor, const char *name, int x, int y)
{
    sprintf(buf, "%s %d %d", name, x, y);
    return pipe(cursor, buf);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_Reincarnation_Pipe(DotrCaptionWord cursor, int x, int y)
{
    return caption(cursor, "reincarnation", x, y);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_ReincarnationDone_Pipe(DotrCaptionWord cursor, int x, int y)
{
    return caption(cursor, "reincarnation-done", x, y);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_Password_Pipe(DotrCaptionWord cursor, int x, int y)
{
    return caption(cursor, "password", x, y);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_PasswordSuccess_Pipe(DotrCaptionWord cursor, int x, int y)
{
    return caption(cursor, "password-success", x, y);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_PasswordFail_Pipe(DotrCaptionWord cursor, int x, int y)
{
    return caption(cursor, "password-fail", x, y);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_PasswordDone_Pipe(DotrCaptionWord cursor, int x, int y)
{
    return caption(cursor, "password-done", x, y);
}
DotrCaptionWord lSzDuel_DrawBuildDeck_Caution_Pipe(DotrCaptionWord cursor, int x, int y)
{
    return caption(cursor, "caution", x, y);
}
void SzMsg_Free(int index)
{
    sprintf(buf, "free-msg %d", index);
    note(buf);
}
void RhBase_FreeRenderFrame(void)
{
    note("free-frame");
}

/* Runs one frame with the given slide and mode; returns the log, checks the returned cursor
 * is the last one handed out. */
static const char *run(int slide, int mode)
{
    DotrCaptionWord out;

    C.slide = (short)slide;
    C.mode = (unsigned char)mode;
    log_text[0] = '\0';
    expect_cursor = 1000;
    count_id = look_id = -999;
    out = SzDuel_DrawBuildDeck_Pipe(1000);
    assert(out == expect_cursor);
    return log_text;
}

/* The fixed head of every frame (title at the slide). */
static void head(char *e, int slide)
{
    sprintf(e, "assign 5 2 50\nenable\nclear\ndisable\nsession 2\nmask ff000000\nbase\nmask 00000000\ntitle %d 0\n", slide);
}
static void add(char *e, const char *line)
{
    strcat(e, line);
    strcat(e, "\n");
}
static void tail(char *e)
{
    add(e, "free-msg 0");
    add(e, "free-frame");
}

int main(void)
{
    static const char *const captions[7] = {"reincarnation", "reincarnation-done", "password", "password-success",
                                            "password-fail", "password-done", "caution"};
    char e[4096], line[128];
    int i, mode;

    assert(offsetof(struct DotrCaptionControl, side) + 1 == offsetof(struct DotrCaptionControl, mode));
    assert(offsetof(struct DotrCaptionControl, grid_row) == offsetof(struct DotrCaptionControl, side) + 6);
    assert(offsetof(struct DotrCaptionControl, slide) == offsetof(struct DotrCaptionControl, list_cursor) + 8);

    for (i = 0; i < 64; i++) {
        bag[i] = (unsigned short)(1000 + i);
        deck[i] = (unsigned short)(2000 + i);
    }
    C.list_cards = bag;
    C.deck_cards = deck;
    C.side = 1;
    C.list_cursor = 20;
    C.list_top = 3;
    C.deck_cursor = 30;
    C.deck_top = 4;
    C.grid_top = 2;
    C.grid_row = 1;
    C.grid_col = 7;

    /* Slide 0, mode 0: no library, no detail, bag and deck panes, no deck detail. */
    head(e, 0);
    add(e, "bag 0 0");
    add(e, "deck 0 0");
    add(e, "ghost 0 0");
    tail(e);
    bag_count = 1;
    assert(strcmp(run(0, 0), e) == 0);
    assert(count_id == -999 && look_id == -999);

    /* Slide 100, mode 0: the grid card (2 + 1) * 10 + 7 = 37 when the side has it in the bag. */
    bag_count = 1;
    look_flag = 0;
    head(e, 100);
    add(e, "detail 37 100 0 0 0 0 1");
    add(e, "bag 100 0");
    add(e, "deck 100 0");
    add(e, "ghost 100 0");
    tail(e);
    assert(strcmp(run(100, 0), e) == 0);
    assert(count_side == 1 && count_id == 37 && count_kind == -1 && look_side == 1 && look_id == 37);
    /* ... or only seen (look flag), and nothing when neither; a count of 0 or below is none. */
    bag_count = 0;
    look_flag = 1;
    assert(strcmp(run(100, 0), e) == 0);
    bag_count = -1;
    look_flag = 0;
    head(e, 100);
    add(e, "bag 100 0");
    add(e, "deck 100 0");
    add(e, "ghost 100 0");
    tail(e);
    assert(strcmp(run(100, 0), e) == 0);
    assert(look_id == 37);

    /* Slide 100, mode 1: the bag card bag[20 + 3] with the fourth flag set. */
    head(e, 100);
    add(e, "detail 1023 100 0 0 1 0 1");
    add(e, "bag 100 0");
    add(e, "deck 100 0");
    add(e, "ghost 100 0");
    tail(e);
    assert(strcmp(run(100, 1), e) == 0);
    assert(count_id == -999 && look_id == -999);
    /* Mode 6 (no cursor pane): the same with the flag clear. */
    head(e, 100);
    add(e, "detail 1023 100 0 0 0 0 1");
    add(e, "bag 100 0");
    add(e, "deck 100 0");
    add(e, "ghost 100 0");
    tail(e);
    assert(strcmp(run(100, 6), e) == 0);

    /* Slide 330: library pane, no deck pane. Slide 320: neither library nor deck. */
    head(e, 330);
    add(e, "library 330 0");
    add(e, "detail 1023 330 0 0 1 0 1");
    add(e, "bag 330 0");
    add(e, "ghost 330 0");
    tail(e);
    assert(strcmp(run(330, 1), e) == 0);
    head(e, 320);
    add(e, "detail 1023 320 0 0 1 0 1");
    add(e, "bag 320 0");
    add(e, "ghost 320 0");
    tail(e);
    assert(strcmp(run(320, 1), e) == 0);
    /* Slide 640 and 700: no bag pane. 639 still has it. */
    head(e, 640);
    add(e, "library 640 0");
    add(e, "detail 1023 640 0 0 1 0 1");
    add(e, "ghost 640 0");
    tail(e);
    assert(strcmp(run(640, 1), e) == 0);
    head(e, 700);
    add(e, "library 700 0");
    add(e, "detail 1023 700 0 0 1 0 1");
    add(e, "ghost 700 0");
    tail(e);
    assert(strcmp(run(700, 1), e) == 0);
    head(e, 639);
    add(e, "library 639 0");
    add(e, "detail 1023 639 0 0 1 0 1");
    add(e, "bag 639 0");
    add(e, "ghost 639 0");
    tail(e);
    assert(strcmp(run(639, 1), e) == 0);

    /* Slide -100: bag, deck, and the deck card deck[30 + 4] with flags (1, mode == 6, 0, 0). */
    head(e, -100);
    add(e, "bag -100 0");
    add(e, "deck -100 0");
    add(e, "detail 2034 -100 0 1 1 0 0");
    add(e, "ghost -100 0");
    tail(e);
    assert(strcmp(run(-100, 6), e) == 0);
    head(e, -100);
    add(e, "bag -100 0");
    add(e, "deck -100 0");
    add(e, "detail 2034 -100 0 1 0 0 0");
    add(e, "ghost -100 0");
    tail(e);
    assert(strcmp(run(-100, 1), e) == 0);
    /* Slide -320 and -400: no bag pane. */
    head(e, -320);
    add(e, "deck -320 0");
    add(e, "detail 2034 -320 0 1 0 0 0");
    add(e, "ghost -320 0");
    tail(e);
    assert(strcmp(run(-320, 0), e) == 0);
    head(e, -400);
    add(e, "deck -400 0");
    add(e, "detail 2034 -400 0 1 0 0 0");
    add(e, "ghost -400 0");
    tail(e);
    assert(strcmp(run(-400, 0), e) == 0);

    /* The mode switch after the ghost pane (slide -400 keeps the panes fixed). */
    for (mode = 0; mode < 20; mode++) {
        head(e, -400);
        add(e, "deck -400 0");
        sprintf(line, "detail 2034 -400 0 1 %d 0 0", mode == 6);
        add(e, line);
        add(e, "ghost -400 0");
        if (mode == 2 || mode == 3) {
            add(e, "bagcursor -400 0 3");
        } else if (mode == 4 || mode == 5) {
            add(e, "deckcursor -400 0 4");
        } else if (mode >= 7 && mode <= 13) {
            sprintf(line, "%s 0 0", captions[mode - 7]);
            add(e, line);
        }
        tail(e);
        assert(strcmp(run(-400, mode), e) == 0);
    }
    puts("build_deck_draw: ok");
    return 0;
}

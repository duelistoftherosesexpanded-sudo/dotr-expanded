/* Synthetic host tests for the dialog_captions group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/dialog_captions.h"

/* Host captions: two filler bytes, then the text the drawers pass on (base + 2). */
char data_002bf000_caption[] = "##PASSWORD";
char data_002bef65_caption[] = "##CAUTION";
char data_002bf009_caption[] = "##TRADE";
char data_002bf03e_caption[] = "##NOTICE";
char data_002bef96_caption[] = "##EXIT";
char data_002bef6d_caption[] = "##YES";
char data_002bef71_caption[] = "##NO";
char data_002bef9b_caption[] = "##REINCARNATION";
char data_002bef80_caption[] = "##BUILD DECK";
char gSzDuel_Icon_caption[4096];
char data_002be944_caption[12];
unsigned char data_00a9c08e_caption;
struct DotrCaptionControl data_00317490_caption;

static char order[24];
static int calls;
static const char *measured;

static void said(char c)
{
    assert(calls < (int)sizeof order - 1);
    order[calls++] = c;
    order[calls] = 0;
}

unsigned long func_0019bef0_caption(const char *string)
{
    said('L');
    measured = string;
    return 1000 + strlen(string);
}
int func_0019bef0(const char *string)
{
    said('L');
    measured = string;
    return (int)strlen(string) + 2000;
}

static struct {
    DotrCaptionWord cursor;
    int a1, a2, texture, x, y, w, h;
    unsigned long length;
    const char *text;
    int last;
} dialog;
/* The confirmation drawers pass the last stack argument as a long (0L). */
static int last_is_long;

DotrCaptionWord func_001b0ce0_caption(DotrCaptionWord cursor, int a1, int a2, int texture, int x, int y, int w, int h, ...)
{
    va_list ap;

    said('D');
    dialog.cursor = cursor;
    dialog.a1 = a1;
    dialog.a2 = a2;
    dialog.texture = texture;
    dialog.x = x;
    dialog.y = y;
    dialog.w = w;
    dialog.h = h;
    va_start(ap, h);
    dialog.length = va_arg(ap, unsigned long);
    dialog.text = va_arg(ap, const char *);
    dialog.last = last_is_long ? (int)va_arg(ap, long) : va_arg(ap, int);
    va_end(ap);
    return cursor + 64;
}

static int base[5], frame[9], font[8], frame_log[2][9], frames;
static const char *font_text;

DotrCaptionTitleWord func_001b62f0_caption(DotrCaptionTitleWord cursor, int x, int y, int cols, int rows)
{
    said('B');
    base[0] = (int)cursor;
    base[1] = x;
    base[2] = y;
    base[3] = cols;
    base[4] = rows;
    return cursor + 10;
}
DotrCaptionTitleWord func_001b4780_caption(DotrCaptionTitleWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8)
{
    said('F');
    frame[0] = (int)cursor;
    frame[1] = a1;
    frame[2] = a2;
    frame[3] = a3;
    frame[4] = x;
    frame[5] = y;
    frame[6] = a6;
    frame[7] = a7;
    frame[8] = (int)a8;
    if (frames < 2)
        memcpy(frame_log[frames++], frame, sizeof frame);
    return cursor + 100;
}
DotrCaptionTitleWord func_001b6f70_caption(DotrCaptionTitleWord cursor, int x, int y, int fnt, int flags, int n, const char *text, int c)
{
    said('T');
    font[0] = (int)cursor;
    font[1] = x;
    font[2] = y;
    font[3] = fnt;
    font[4] = flags;
    font[5] = n;
    font[7] = c;
    font_text = text;
    return cursor + 1000;
}

typedef DotrCaptionWord (*Drawer)(DotrCaptionWord cursor, int x, int y);

static void check_dialog(Drawer draw, int texture, int dy, int w, int h, const char *base_text, const char *caption)
{
    calls = 0;
    memset(&dialog, 0, sizeof dialog);
    assert(draw(500, 7, -3) == 564);
    assert(strcmp(order, "LD") == 0);
    assert(measured == base_text + 2 && strcmp(measured, caption) == 0);
    assert(dialog.cursor == 500 && dialog.a1 == 2 && dialog.a2 == -1 && dialog.texture == texture);
    assert(dialog.x == 327 && dialog.y == dy - 3 && dialog.w == w && dialog.h == h);
    assert(dialog.length == 1000 + strlen(caption) && dialog.text == base_text + 2 && dialog.last == 1);
}

/* Stubs for the confirmation drawers. */
static struct {
    DotrCaptionWord cursor;
    int x, y, font, flags, n;
    const char *text;
    char *c;
} choice[2];
static int choices;
DotrCaptionWord func_001b6f70_choice(DotrCaptionWord cursor, int x, int y, int fnt, int flags, int n, const char *text, char *c)
{
    said('C');
    assert(choices < 2);
    choice[choices].cursor = cursor;
    choice[choices].x = x;
    choice[choices].y = y;
    choice[choices].font = fnt;
    choice[choices].flags = flags;
    choice[choices].n = n;
    choice[choices].text = text;
    choice[choices].c = c;
    choices++;
    return cursor + 1000;
}
static int load[5], loads[2][5];
DotrCaptionWord func_001fd680_caption(DotrCaptionWord cursor, int index, int message, int a3, int size)
{
    said('M');
    if (index == 1 || index == 2) {
        loads[index - 1][0] = (int)cursor;
        loads[index - 1][1] = index;
        loads[index - 1][2] = message;
        loads[index - 1][3] = a3;
        loads[index - 1][4] = size;
    }
    load[0] = (int)cursor;
    load[1] = index;
    load[2] = message;
    load[3] = a3;
    load[4] = size;
    return cursor + 3;
}
static DotrCaptionWord flushed;
DotrCaptionWord func_0020f290_caption(DotrCaptionWord cursor)
{
    said('F');
    flushed = cursor;
    return cursor + 5;
}
static struct DotrCaptionMsgInfo info, info2;
static int info_index;
struct DotrCaptionMsgInfo *func_001feab0_caption(int index)
{
    said('I');
    info_index = index;
    return index == 2 ? &info2 : &info;
}
static struct DrawX {
    DotrCaptionWord cursor;
    int index, x, y;
    void *e;
    void (*function)(void);
} drawx, drawx_log[2];
static int drawxs;
DotrCaptionWord func_001fe7d0_caption(DotrCaptionWord cursor, int index, int x, int y, void *e, void (*function)(void))
{
    said('X');
    drawx.cursor = cursor;
    drawx.index = index;
    drawx.x = x;
    drawx.y = y;
    drawx.e = e;
    drawx.function = function;
    if (drawxs < 2)
        drawx_log[drawxs++] = drawx;
    return cursor + 7;
}
static int freed, freed_mask;
void func_001feaf0_caption(int index)
{
    said('R');
    freed = index;
    freed_mask = freed_mask * 4 + index;
}
void func_001fdfe0_caption(void)
{
}
struct DotrCaptionRecord password_record;
struct DotrCaptionRecord *data_00317b60_caption = &password_record;
int data_003180e8;
static int pic[8], pic_log[3][7], pics;
DotrCaptionWord func_001b7d60_caption(DotrCaptionWord cursor, int id, int x, int y, int width, int height, unsigned int z)
{
    said('P');
    pic[0] = (int)cursor;
    pic[1] = id;
    pic[2] = x;
    pic[3] = y;
    pic[4] = width;
    pic[5] = height;
    pic[6] = (int)z;
    if (pics < 3)
        memcpy(pic_log[pics++], pic, sizeof pic_log[0]);
    return cursor + 11;
}
static int ratio_calls;
static struct {
    DotrCaptionWord cursor;
    int index, x, y;
    float sx, sy, half;
    void *e;
    void (*function)(void);
} ratio;
DotrCaptionWord func_001fe660_caption(DotrCaptionWord cursor, int index, int x, int y, float sx, float sy, void *e,
                                     void (*function)(void))
{
    said('Q');
    ratio_calls++;
    ratio.cursor = cursor;
    ratio.index = index;
    ratio.x = x;
    ratio.y = y;
    ratio.sx = sx;
    ratio.sy = sy;
    ratio.e = e;
    ratio.function = function;
    return cursor + 13;
}
/* Host stand-in for the library float-to-int routine: truncation, recording its argument. */
int func_001a22c0(float value)
{
    said('V');
    ratio.half = value;
    return (int)value;
}
static int name_card;
int func_001de210_caption(int card)
{
    said('N');
    name_card = card;
    return card + 7000;
}

/* Draws at (7, -3) from cursor 500; flag 0 gives YES the 0x2be944 colour, non-zero gives it to NO. */
static void check_choice(Drawer draw, int texture, int dy, int h, const char *base_text, const char *caption,
                         int choice_y, int message, int flag)
{
    DotrCaptionWord after;

    calls = 0;
    choices = 0;
    last_is_long = 1;
    memset(&dialog, 0, sizeof dialog);
    memset(&drawx, 0, sizeof drawx);
    data_00a9c08e_caption = 0;
    data_00317490_caption.choice = 0;
    if (message) {
        data_00317490_caption.choice = (unsigned char)flag;
    } else {
        data_00a9c08e_caption = (unsigned char)flag;
    }
    info.width = 101;
    info.height = 30;
    after = message ? 564 + 3 + 5 + 7 : 564;
    assert(draw(500, 7, -3) == after + 2000);
    last_is_long = 0;
    assert(strcmp(order, message ? "LDMFIXRLCLC" : "LDLCLC") == 0);
    assert(dialog.cursor == 500 && dialog.a1 == 2 && dialog.a2 == -1 && dialog.texture == texture);
    assert(dialog.x == 327 && dialog.y == dy - 3 && dialog.w == 320 && dialog.h == h);
    assert(dialog.length == 1000 + strlen(caption) && dialog.text == base_text + 2 && dialog.last == 0);
    assert(strcmp(dialog.text, caption) == 0);
    if (message) {
        assert(load[0] == 564 && load[1] == 1 && load[2] == 127 && load[3] == 0 && load[4] == 22);
        assert(flushed == 567 && info_index == 1 && freed == 1);
        /* Centred: 327 - (101 >> 1) = 277; above the choices by height + 12. */
        assert(drawx.cursor == 572 && drawx.index == 1 && drawx.x == 277 && drawx.y == choice_y - 3 - 42);
        assert(drawx.e == NULL && drawx.function == func_001fdfe0_caption);
    }
    assert(choice[0].cursor == after && choice[0].x == 247 && choice[0].y == choice_y - 3);
    assert(choice[0].font == 2 && choice[0].flags == 3 && choice[0].n == 2003);
    assert(choice[0].text == data_002bef6d_caption + 2 && strcmp(choice[0].text, "YES") == 0);
    assert(choice[1].cursor == after + 1000 && choice[1].x == 407 && choice[1].y == choice_y - 3);
    assert(choice[1].font == 2 && choice[1].flags == 3 && choice[1].n == 2002);
    assert(choice[1].text == data_002bef71_caption + 2 && strcmp(choice[1].text, "NO") == 0);
    if (flag) {
        assert(choice[0].c == NULL && choice[1].c == data_002be944_caption);
    } else {
        assert(choice[0].c == data_002be944_caption && choice[1].c == NULL);
    }
}

static void check_choices(void)
{
    int flag;

    for (flag = 0; flag < 3; flag += 2) {
        check_choice(lSzDuel_DrawCustomDuel_Exit_Pipe, 145, 148, 152, data_002bef96_caption, "EXIT", 264, 0, flag);
        check_choice(lSzDuel_DrawTrade_Exit_Pipe, 126, 131, 186, data_002bef96_caption, "EXIT", 281, 1, flag);
        check_choice(lSzDuel_DrawTrade_Do_Pipe, 125, 142, 164, data_002bf009_caption, "TRADE", 270, 1, flag);
    }
    /* Each drawer reads only its own flag. */
    data_00317490_caption.choice = 1;
    data_00a9c08e_caption = 0;
    calls = 0;
    choices = 0;
    last_is_long = 1;
    lSzDuel_DrawCustomDuel_Exit_Pipe(0, 0, 0);
    last_is_long = 0;
    assert(choice[0].c == data_002be944_caption);
    data_00317490_caption.choice = 0;
    data_00a9c08e_caption = 1;
    calls = 0;
    choices = 0;
    last_is_long = 1;
    lSzDuel_DrawTrade_Exit_Pipe(0, 0, 0);
    last_is_long = 0;
    assert(choice[0].c == data_002be944_caption);
}

/* The reincarnation drawer: the card id at list_cards[list_cursor + list_top] goes to the window
 * with its name; message 146; YES / NO at x + 205 / x + 435, y + 340. */
static void check_reincarnation_case(int flag, int width)
{
    static unsigned short cards[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0x3a1};

    calls = 0;
    choices = 0;
    last_is_long = 1;
    memset(&dialog, 0, sizeof dialog);
    memset(&drawx, 0, sizeof drawx);
    data_00317490_caption.list_cards = cards;
    data_00317490_caption.list_cursor = 4;
    data_00317490_caption.list_top = 7;
    data_00317490_caption.choice = (unsigned char)flag;
    info.width = width;
    info.height = 30;
    assert(lSzDuel_DrawBuildDeck_Reincarnation_Pipe(500, 7, -3) == 564 + 3 + 5 + 7 + 2000);
    last_is_long = 0;
    assert(strcmp(order, "NLDMFIXRLCLC") == 0);
    /* Index cursor + top = 11. */
    assert(name_card == 0x3a1);
    assert(dialog.cursor == 500 && dialog.a1 == 2 && dialog.a2 == 0x3a1 && dialog.texture == 0x3a1 + 7000);
    assert(dialog.x == 327 && dialog.y == 69 && dialog.w == -460 && dialog.h == 304);
    assert(dialog.length == 1000 + 13 && dialog.text == data_002bef9b_caption + 2 && dialog.last == 0);
    assert(strcmp(dialog.text, "REINCARNATION") == 0);
    assert(load[0] == 564 && load[1] == 1 && load[2] == 146 && load[3] == 0 && load[4] == 22);
    assert(flushed == 567 && info_index == 1 && freed == 1);
    /* Centred with an arithmetic shift: 327 - (width >> 1); above the choices by height + 12. */
    assert(drawx.cursor == 572 && drawx.index == 1 && drawx.x == 327 - (width >> 1) && drawx.y == 337 - 42);
    assert(drawx.e == NULL && drawx.function == func_001fdfe0_caption);
    assert(choice[0].cursor == 579 && choice[0].x == 212 && choice[0].y == 337);
    assert(choice[0].font == 2 && choice[0].flags == 3 && choice[0].n == 2003);
    assert(choice[0].text == data_002bef6d_caption + 2 && strcmp(choice[0].text, "YES") == 0);
    assert(choice[1].cursor == 1579 && choice[1].x == 442 && choice[1].y == 337);
    assert(choice[1].font == 2 && choice[1].flags == 3 && choice[1].n == 2002);
    assert(choice[1].text == data_002bef71_caption + 2 && strcmp(choice[1].text, "NO") == 0);
    if (flag) {
        assert(choice[0].c == NULL && choice[1].c == data_002be944_caption);
    } else {
        assert(choice[0].c == data_002be944_caption && choice[1].c == NULL);
    }
}

static void check_reincarnation(void)
{
    check_reincarnation_case(0, 101);
    check_reincarnation_case(1, 100);
    check_reincarnation_case(255, -5);
    assert(drawx.x == 327 - (-5 >> 1));
    data_00317490_caption.choice = 0;
}

/* Password accepted: card picture, message 149, the card name scaled down past 436 wide. */
static void check_password_case(int width2, float scale, int x_ratio)
{
    calls = 0;
    ratio_calls = 0;
    freed_mask = 0;
    last_is_long = 0;
    memset(&dialog, 0, sizeof dialog);
    memset(&drawx, 0, sizeof drawx);
    memset(&ratio, 0, sizeof ratio);
    password_record.id = 0x2b7;
    data_003180e8 = 0x123456;
    info.width = 101;
    info.height = 30;
    info2.width = width2;
    assert(lSzDuel_DrawBuildDeck_PasswordSuccess_Pipe(500, 7, -3) == 564 + 11 + 3 + 3 + 5 + 7 + 13);
    assert(strcmp(order, "NLDPMMFIXIVQRR") == 0);
    assert(name_card == 0x2b7);
    assert(dialog.cursor == 500 && dialog.a1 == 2 && dialog.a2 == -1 && dialog.texture == -1);
    assert(dialog.x == 327 && dialog.y == 62 && dialog.w == 460 && dialog.h == 318);
    assert(dialog.length == 1000 + 8 && dialog.text == data_002bf000_caption + 2 && dialog.last == 1);
    assert(strcmp(dialog.text, "PASSWORD") == 0);
    assert(pic[0] == 564 && pic[1] == 0x2b7 && pic[2] == 263 && pic[3] == 178);
    assert(pic[4] == 128 && pic[5] == 128 && pic[6] == 0x123456);
    assert(loads[0][0] == 575 && loads[0][2] == 149 && loads[0][3] == 0 && loads[0][4] == 22);
    assert(loads[1][0] == 578 && loads[1][2] == 0x2b7 + 7000 && loads[1][3] == 0 && loads[1][4] == 66);
    assert(flushed == 581 && info_index == 2);
    assert(drawx.cursor == 586 && drawx.index == 1 && drawx.x == 327 - 50 && drawx.y == 122);
    assert(drawx.e == NULL && drawx.function == func_001fdfe0_caption);
    assert(ratio_calls == 1 && ratio.cursor == 593 && ratio.index == 2 && ratio.y == 318);
    assert(ratio.sx == scale && ratio.sy == 1.0f && ratio.half == (float)width2 * scale / 2.0f);
    assert(ratio.x == x_ratio && ratio.e == NULL && ratio.function == func_001fdfe0_caption);
    /* Slot 1 freed, then slot 2. */
    assert(freed_mask == 1 * 4 + 2);
}

static void check_password(void)
{
    /* At most 436 wide: scale 1. Wider: scaled to 436 (half of 436 = 218). */
    check_password_case(436, 1.0f, 327 - 218);
    check_password_case(301, 1.0f, 327 - 150);
    check_password_case(872, 0.5f, 327 - 218);
}

static void check_title(void)
{
    calls = 0;
    assert(lSzDuel_DrawTrade_Title_Pipe(40, 10, 20) == 1150);
    assert(strcmp(order, "BFLT") == 0);
    assert(base[0] == 40 && base[1] == 234 && base[2] == 52 && base[3] == 6 && base[4] == 1);
    assert(frame[0] == 50 && frame[1] == -1 && frame[2] == 115 && frame[3] == 4);
    assert(frame[4] == 234 && frame[5] == 52 && frame[6] == 21 && frame[7] == 2 && frame[8] == 1);
    assert(measured == data_002bf009_caption + 2);
    assert(font[0] == 150 && font[1] == 330 && font[2] == 59 && font[3] == 2 && font[4] == 3);
    assert(font[5] == 2005 && font_text == data_002bf009_caption + 2 && font[7] == 0);
    assert(strcmp(font_text, "TRADE") == 0);
}

/* --- Build-deck title, password entry and reincarnation done --- */

/* ExIcon calls of both views: cursor, definition offset from 0x290080, position, ten constants. */
static struct Icon {
    long cursor;
    long def;
    int x, y, rest[10];
} icon_log[4];
static int icons;
static void icon(long cursor, char *def, int x, int y, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11,
                 int a12, int a13)
{
    struct Icon *i;

    said('i');
    assert(icons < 4);
    i = &icon_log[icons++];
    i->cursor = cursor;
    i->def = def - gSzDuel_Icon_caption;
    i->x = x;
    i->y = y;
    i->rest[0] = a4;
    i->rest[1] = a5;
    i->rest[2] = a6;
    i->rest[3] = a7;
    i->rest[4] = a8;
    i->rest[5] = a9;
    i->rest[6] = a10;
    i->rest[7] = a11;
    i->rest[8] = a12;
    i->rest[9] = a13;
}
DotrCaptionTitleWord func_001b3cc0_title(DotrCaptionTitleWord cursor, char *def, int x, int y, int a4, int a5, int a6,
                                         int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
    icon((long)cursor, def, x, y, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
    return cursor + 30;
}
DotrCaptionWord func_001b3cc0_caption(DotrCaptionWord cursor, char *def, int x, int y, int a4, int a5, int a6, int a7,
                                      int a8, int a9, int a10, int a11, int a12, int a13)
{
    icon((long)cursor, def, x, y, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
    return cursor + 30;
}
static void check_icon(int k, long cursor, long def, int x, int y)
{
    static const int rest[10] = {0, 1, 0, 0, 0, 0, 1, 0, -1, 128};

    assert(icon_log[k].cursor == cursor && icon_log[k].def == def && icon_log[k].x == x && icon_log[k].y == y);
    assert(memcmp(icon_log[k].rest, rest, sizeof rest) == 0);
}
static long cframe[9];
DotrCaptionWord func_001b4780_choice(DotrCaptionWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8)
{
    said('f');
    cframe[0] = (long)cursor;
    cframe[1] = a1;
    cframe[2] = a2;
    cframe[3] = a3;
    cframe[4] = x;
    cframe[5] = y;
    cframe[6] = a6;
    cframe[7] = a7;
    cframe[8] = a8;
    return cursor + 20;
}
static int digit_log[2][7], digits;
DotrCaptionTitleWord func_001b6ea0_caption(DotrCaptionTitleWord cursor, int x, int y, int a, int flags, int width, int value)
{
    said('d');
    assert(digits < 2);
    digit_log[digits][0] = (int)cursor;
    digit_log[digits][1] = x;
    digit_log[digits][2] = y;
    digit_log[digits][3] = a;
    digit_log[digits][4] = flags;
    digit_log[digits][5] = width;
    digit_log[digits][6] = value;
    digits++;
    return cursor + 300;
}
static int deck_side, cost_args[2], count_args[2];
int SzData_GetCurrentDeck(int side)
{
    said('g');
    deck_side = side;
    return 5;
}
int SzData_GetDeckCost(int side, int list)
{
    said('k');
    cost_args[0] = side;
    cost_args[1] = list;
    return 1105;
}
int SzData_GetDeckCount(int side, int list)
{
    said('n');
    count_args[0] = side;
    count_args[1] = list;
    return 15;
}
static int reg_args[2];
long ExGetRegVal(int a, int b)
{
    said('G');
    reg_args[0] = a;
    reg_args[1] = b;
    return 0x123456789L;
}
static long clamp_log[4];
static int clamps;
DotrCaptionWord func_002084d0_caption(DotrCaptionWord cursor, long value)
{
    said('K');
    assert(clamps < 4);
    clamp_log[clamps++] = value;
    return cursor + 2;
}

static void check_build_title(void)
{
    calls = 0;
    icons = digits = frames = 0;
    data_00317490_caption.side = 1;
    assert(lSzDuel_DrawBuildDeck_Title_Pipe(40, 10, 20) == 1940);
    assert(strcmp(order, "gBFLTFiikdind") == 0);
    assert(deck_side == 1 && cost_args[0] == 1 && cost_args[1] == 5 && count_args[0] == 1 && count_args[1] == 5);
    assert(base[0] == 40 && base[1] == 234 && base[2] == 52 && base[3] == 6 && base[4] == 1);
    {
        static const int f0[9] = {50, -1, 115, 4, 234, 52, 21, 2, 1};
        static const int f1[9] = {1150, 12, -1, 11, 442, 36, -128, -48, 1};
        assert(memcmp(frame_log[0], f0, sizeof f0) == 0 && memcmp(frame_log[1], f1, sizeof f1) == 0);
    }
    assert(measured == data_002bef80_caption + 2 && strcmp(measured, "BUILD DECK") == 0);
    assert(font[0] == 150 && font[1] == 330 && font[2] == 59 && font[3] == 2 && font[4] == 3);
    assert(font[5] == 2010 && font_text == data_002bef80_caption + 2 && font[7] == 0);
    check_icon(0, 1250, 2330, 2212, 1868);
    check_icon(1, 1280, 2160, 2182, 1886);
    check_icon(2, 1610, 2050, 2246, 1886);
    {
        static const int d0[7] = {1310, 474, 64, 1, 0, 4, 1105};
        static const int d1[7] = {1640, 538, 64, 1, 0, 2, 15};
        assert(memcmp(digit_log[0], d0, sizeof d0) == 0 && memcmp(digit_log[1], d1, sizeof d1) == 0);
    }
    data_00317490_caption.side = 0;
}

/* Password entry at (7, -3) from cursor 500 with the cursor column `column` and odd widths. */
static void check_password_entry_case(int column, int width1, int width2)
{
    calls = 0;
    choices = icons = drawxs = 0;
    last_is_long = 1;
    memset(&dialog, 0, sizeof dialog);
    memset(loads, 0, sizeof loads);
    memcpy(data_00317490_caption.pass, "AB12CD34", 8);
    data_00317490_caption.choice = (unsigned char)column;
    info.width = width1;
    info2.width = width2;
    assert(lSzDuel_DrawBuildDeck_Password_Pipe(500, 7, -3) == 1699);
    last_is_long = 0;
    assert(strcmp(order, "LDCfMMFIXiiIXi") == 0);
    assert(dialog.cursor == 500 && dialog.a1 == 2 && dialog.a2 == -1 && dialog.texture == 148);
    assert(dialog.x == 327 && dialog.y == 139 && dialog.w == 346 && dialog.h == 164);
    assert(dialog.length == 1000 + 8 && dialog.text == data_002bf000_caption + 2 && dialog.last == 0);
    assert(choices == 1 && choice[0].cursor == 564 && choice[0].x == 327 && choice[0].y == 233);
    assert(choice[0].font == 2 && choice[0].flags == 2 && choice[0].n == 8 && choice[0].c == NULL);
    assert(choice[0].text == data_00317490_caption.pass);
    assert(cframe[0] == 1564 && cframe[1] == -1 && cframe[2] == -1 && cframe[3] == 7);
    assert(cframe[4] == 237 + 22 * column && cframe[5] == 228 && cframe[6] == -26 && cframe[7] == -28 && cframe[8] == 1);
    assert(loads[0][0] == 1584 && loads[0][2] == 170 && loads[0][3] == 0 && loads[0][4] == 22);
    assert(loads[1][0] == 1587 && loads[1][2] == 173 && loads[1][3] == 0 && loads[1][4] == 44);
    assert(flushed == 1590);
    assert(drawx_log[0].cursor == 1595 && drawx_log[0].index == 1 && drawx_log[0].x == 241 - (width1 >> 1));
    assert(drawx_log[0].y == 269 && drawx_log[0].e == NULL && drawx_log[0].function == func_001fdfe0_caption);
    check_icon(0, 1602, 2490, 1921 - (width1 >> 1), 2093);
    check_icon(1, 1632, 2480, 1945 - (width1 >> 1), 2093);
    assert(drawx_log[1].cursor == 1662 && drawx_log[1].index == 2 && drawx_log[1].x == 413 - (width2 >> 1));
    assert(drawx_log[1].y == 269 && drawx_log[1].e == NULL && drawx_log[1].function == func_001fdfe0_caption);
    check_icon(2, 1669, 2500, 2117 - (width2 >> 1), 2093);
}

static void check_password_entry(void)
{
    check_password_entry_case(0, 101, 51);
    check_password_entry_case(7, 77, -9);
    data_00317490_caption.choice = 0;
}

/* Reincarnation done at (7, -3) from cursor 500 with fade `fade` and step `step`. */
static void check_reincarnation_done_case(int fade, int step)
{
    static char work[64];
    static const unsigned short ids[3] = {11, 22, 33};
    char expect[24];
    int e = 0, i, clamp_count = 0;
    long fade_clamp = ((long)fade << 14) | 0xa | ((long)fade << 34);

    calls = 0;
    pics = clamps = 0;
    last_is_long = 1;
    name_card = -99;
    memset(&dialog, 0, sizeof dialog);
    memcpy(work + 32, ids, sizeof ids);
    data_00317490_caption.work = (DotrCaptionAddress)work;
    data_00317490_caption.pic_z = 0x777;
    data_00317490_caption.choice = (unsigned char)fade;
    data_00317490_caption.step = (unsigned char)step;
    if (fade >= 128 && step < 3)
        expect[e++] = 'N';
    expect[e++] = 'L';
    expect[e++] = 'D';
    for (i = 0; i < 3; i++) {
        if (step == i) {
            expect[e++] = 'K';
            clamp_count++;
        }
        expect[e++] = 'P';
    }
    if (step < 3) {
        expect[e++] = 'G';
        expect[e++] = 'K';
        clamp_count++;
    }
    expect[e] = 0;
    assert(lSzDuel_DrawBuildDeck_ReincarnationDone_Pipe(500, 7, -3) == (DotrCaptionWord)(564 + 33 + 2 * clamp_count));
    last_is_long = 0;
    assert(strcmp(order, expect) == 0);
    if (fade < 128) {
        assert(dialog.texture == -1 && name_card == -99 && dialog.last == 0);
    } else {
        assert(dialog.last == 1);
        if (step < 3)
            assert(name_card == ids[step] && dialog.texture == ids[step] + 7000);
        else
            assert(dialog.texture == 147 && name_card == -99);
    }
    assert(dialog.cursor == 500 && dialog.a1 == 2 && dialog.a2 == -1);
    assert(dialog.x == 327 && dialog.y == 90 && dialog.w == -460 && dialog.h == 262);
    assert(dialog.length == 1000 + 13 && dialog.text == data_002bef9b_caption + 2);
    assert(pics == 3 && clamps == clamp_count);
    for (i = 0; i < 3; i++) {
        int size = step < i ? 0 : step == i ? fade : 128;
        assert(pic_log[i][1] == ids[i] && pic_log[i][2] == 123 + 140 * i && pic_log[i][3] == 184);
        assert(pic_log[i][4] == size && pic_log[i][5] == size && pic_log[i][6] == 0x777);
    }
    if (step < 3) {
        assert(clamp_log[0] == fade_clamp && clamp_log[1] == 0x123456789L);
        assert(reg_args[0] == 0 && reg_args[1] == 4);
    }
}

static void check_reincarnation_done(void)
{
    int step;

    for (step = 0; step < 4; step++) {
        check_reincarnation_done_case(0x40, step);
        check_reincarnation_done_case(0x80, step);
        check_reincarnation_done_case(0xff, step);
    }
    /* The clamp value keeps the 64-bit high copy of the fade. */
    check_reincarnation_done_case(0xff, 1);
    assert(clamp_log[0] == 0x3fc003fc00aL);
    data_00317490_caption.choice = 0;
    data_00317490_caption.step = 0;
}

int main(void)
{
    check_dialog(lSzDuel_DrawBuildDeck_PasswordFail_Pipe, 150, 141, 460, 166, data_002bf000_caption, "PASSWORD");
    check_dialog(lSzDuel_DrawBuildDeck_PasswordDone_Pipe, 151, 141, 460, 166, data_002bf000_caption, "PASSWORD");
    check_dialog(lSzDuel_DrawBuildDeck_Caution_Pipe, 124, 163, 320, 122, data_002bef65_caption, "CAUTION");
    check_dialog(lSzDuel_DrawTrade_Notice_Pipe, 128, 163, 320, 122, data_002bf03e_caption, "NOTICE");
    check_title();
    check_choices();
    check_reincarnation();
    check_password();
    check_build_title();
    check_password_entry();
    check_reincarnation_done();
    puts("dialog_captions: ok");
    return 0;
}

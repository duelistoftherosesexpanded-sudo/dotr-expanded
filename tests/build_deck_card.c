/* Synthetic host tests for the build_deck_card group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/build_deck_card.h"

unsigned char data_00317490_buildcard[0xd70];
unsigned char DeckBuildingSide_buildcard;
unsigned char gSzDuel_Icon_buildcard[0x1000];
unsigned char data_00290020_buildcard[0x400];
int data_00290a12_buildcard;
int data_002908e6_buildcard;
int data_002908be_buildcard;
int data_002908c8_buildcard;
int data_002908f0_buildcard;
int data_0029086e_buildcard;
int data_00290878_buildcard;
int data_0029097c_buildcard;
int data_00290986_buildcard;
int data_00290990_buildcard;
signed char data_002befb7_buildcard[] = "deck";
char gSzDuel_aicolMsg_buildcard[4];
char data_002be944_buildcard[4];
char data_002be948_buildcard[4];

enum { F_FRAME = 1, F_ICON, F_DIGIT, F_EMBLEM, F_FONT, F_BOARD, F_RATIO, F_SORT, F_LOAD, F_FLUSH, F_FREE };

struct call {
    int fn;
    long a[8];
    const void *p;
    float s;
    signed char ch;
};

static struct call trace[64];
static int ntrace, deck, bag_new, rank_value, bag[4], msg_width, kind_type, freed;
static DotrBuildCardWord next_cursor;
static struct DotrBuildCardCard card_record;
static struct DotrBuildCardCard *card_ptr;
static struct DotrBuildCardBox sorted;
static long long alpha_seen;

static DotrBuildCardWord add(int fn, DotrBuildCardWord cursor, long a0, long a1, long a2, long a3, long a4,
                             const void *p)
{
    struct call *t;

    assert(ntrace < 64);
    assert(cursor == next_cursor);
    t = &trace[ntrace++];
    memset(t, 0, sizeof(*t));
    t->fn = fn;
    t->a[0] = a0;
    t->a[1] = a1;
    t->a[2] = a2;
    t->a[3] = a3;
    t->a[4] = a4;
    t->p = p;
    return ++next_cursor;
}

static int count(int fn)
{
    int i, n = 0;

    for (i = 0; i < ntrace; i++) {
        n += trace[i].fn == fn;
    }
    return n;
}

static struct call *nth(int fn, int k)
{
    int i;

    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == fn && k-- == 0) {
            return &trace[i];
        }
    }
    assert(0);
    return 0;
}

static struct call *icon(const void *def)
{
    int i;

    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == F_ICON && trace[i].p == def) {
            return &trace[i];
        }
    }
    return 0;
}

unsigned char *SzDuel_GetCardConstInfo(int id)
{
    assert(id == 77);
    return (unsigned char *)card_ptr;
}

int SzData_GetCurrentDeck(int side)
{
    assert(side == DeckBuildingSide_buildcard);
    return deck;
}

int SzDuel_Kind2Type(int kind) { return kind_type + kind; }

int SzDuel_Type2Attr(int type, int attribute) { return type * 16 + attribute; }

int SzDuel_GetMsgName(int id) { return id + 1000; }

int SzData_IsBagNew(int side, int id)
{
    assert(side == DeckBuildingSide_buildcard && id == 77);
    return bag_new;
}

int SzData_GetRank(int side, int id)
{
    assert(side == DeckBuildingSide_buildcard && id == 77);
    return rank_value;
}

int SzData_GetBagCount(int side, int id, int kind)
{
    assert(side == DeckBuildingSide_buildcard && id == 77);
    return kind < 0 ? bag[3] * 10 : bag[kind];
}

int func_0019bef0(const char *s) { return (int)strlen(s); }

int *SzMsg_GetInfo(int index)
{
    static int info[4];

    assert(index == 1);
    info[3] = msg_width;
    return info;
}

void SzMsg_Free(int index)
{
    assert(index == 1);
    freed++;
}

void SzMsg_DrawRect_Pipe(void) {}

int ExAssignAlpha(void *packet, long long alpha)
{
    assert(packet != NULL);
    alpha_seen = alpha;
    return 0;
}

long long ExGetAlphaVal(int index) { return 0x1000 + index; }

DotrBuildCardWord SzMsg_LoadFont_Pipe(DotrBuildCardWord cursor, int a1, int msg, int a3, int a4)
{
    return add(F_LOAD, cursor, a1, msg, a3, a4, 0, 0);
}

DotrBuildCardWord RhText_SetTextureFlush(DotrBuildCardWord cursor) { return add(F_FLUSH, cursor, 0, 0, 0, 0, 0, 0); }

DotrBuildCardWord SzMsg_DrawRatioX_Pipe(DotrBuildCardWord cursor, int index, int x, int y, float sx, float sy,
                                        DotrBuildCardWord color, void (*function)(void))
{
    DotrBuildCardWord r;

    assert(index == 1 && sy == 1.0f && function == SzMsg_DrawRect_Pipe);
    r = add(F_RATIO, cursor, x, y, 0, 0, 0, (const void *)color);
    trace[ntrace - 1].s = sx;
    return r;
}

DotrBuildCardWord lSzDuel_DrawWindow_Frame_Pipe(DotrBuildCardWord cursor, int a1, int a2, int a3, int x, int y,
                                                int w, int h, int a8)
{
    assert(a2 == -1 && a3 == 9 && w == -0x130 && h == -0x36);
    return add(F_FRAME, cursor, a1, x, y, a8, 0, 0);
}

DotrBuildCardWord lSzDuel_DrawWindow_ExIcon_Pipe(DotrBuildCardWord cursor, void *def, int x, int y, int a4, int a5,
                                                 int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13)
{
    assert(a4 == 0 && a5 == 1 && a6 == 0 && a7 == 0 && a8 == 0 && a9 == 0 && a10 == 1 && a11 == 0 && a12 == -1);
    assert(a13 == 0x80);
    return add(F_ICON, cursor, x, y, 0, 0, 0, def);
}

DotrBuildCardWord SzDuel_DrawWindow_FontDigit_Pipe(DotrBuildCardWord cursor, int x, int y, int a, int flags,
                                                   int width, int value)
{
    assert(a == 1 && flags == 0);
    return add(F_DIGIT, cursor, x, y, width, value, 0, 0);
}

DotrBuildCardWord lSzDuel_DrawWindow_Emblem_Pipe(DotrBuildCardWord cursor, int x, int y, int rank, int a4, int a5)
{
    assert(a4 == 0 && a5 == 0);
    return add(F_EMBLEM, cursor, x, y, rank, 0, 0, 0);
}

DotrBuildCardWord SzDuel_DrawWindow_Font_Pipe(DotrBuildCardWord cursor, int x, int y, int a, int flags, int n,
                                              signed char *text, DotrBuildCardWord color)
{
    DotrBuildCardWord r;

    assert(color == 0);
    r = add(F_FONT, cursor, x, y, a, flags, n, text);
    trace[ntrace - 1].ch = text[0];
    return r;
}

DotrBuildCardWord lSzDuel_DrawWindow_ExBoard_Pipe(DotrBuildCardWord cursor, int colour, int x, int y, int w, int h,
                                                  int a6, int a7)
{
    assert(w == -0xb0 && h == -0x20 && a6 == -1 && a7 == 0);
    return add(F_BOARD, cursor, colour, x, y, 0, 0, 0);
}

DotrBuildCardWord ExSortPrim_Pipe(DotrBuildCardWord cursor, DotrBuildCardWord packet, int a2, int a3)
{
    assert(a2 == 0 && a3 == 0);
    memcpy(&sorted, (const void *)packet, sizeof(sorted));
    return add(F_SORT, cursor, 0, 0, 0, 0, 0, 0);
}

static void reset(int kind, int attribute)
{
    ntrace = 0;
    next_cursor = 100;
    freed = 0;
    alpha_seen = 0;
    memset(&sorted, 0, sizeof(sorted));
    memset(&card_record, 0, sizeof(card_record));
    card_record.kind = (unsigned char)(kind << 5 | attribute);
    card_record.attribute = 5;
    card_record.level = 7;
    card_record.stars = 33;
    card_record.link = 0xffff;
    card_record.attack = 0xe000 | 1500;
    card_record.defense = 0xe000 | 1200;
    card_ptr = &card_record;
    DeckBuildingSide_buildcard = 1;
    deck = 2;
    bag_new = 0;
    rank_value = 9;
    bag[0] = 1;
    bag[1] = 2;
    bag[2] = 3;
    bag[3] = 4;
    msg_width = 100;
    kind_type = 0;
}

static void test_no_card(void)
{
    void *r;

    reset(0, 0);
    card_ptr = NULL;
    r = lSzDuel_DrawBuildDeck_Card_Pipe((void *)100, 77, 10, 20, 0, 1);
    assert((DotrBuildCardWord)r == 101 && ntrace == 1);
    assert(trace[0].fn == F_FRAME && trace[0].a[0] == -1 && trace[0].a[1] == 10 && trace[0].a[2] == 20);
    reset(0, 0);
    card_ptr = NULL;
    lSzDuel_DrawBuildDeck_Card_Pipe((void *)100, 77, 10, 20, 3, 0);
    assert(trace[0].a[0] == 6 && trace[0].a[3] == 3);
}

static void test_monster(void)
{
    void *r;
    struct call *c;

    reset(0, 3);
    r = lSzDuel_DrawBuildDeck_Card_Pipe((void *)100, 77, 10, 20, 0, 0);
    assert((DotrBuildCardWord)r == next_cursor);
    /* Font load of the card's name, flush, then the frame in colour type + 7. */
    assert(trace[0].fn == F_LOAD && trace[0].a[1] == 1077 && trace[0].a[3] == 0x16);
    assert(trace[1].fn == F_FLUSH && trace[2].fn == F_FRAME && trace[2].a[0] == 7);
    /* Not new: the card number in three digits. */
    c = nth(F_DIGIT, 0);
    assert(c->a[0] == 18 && c->a[1] == 27 && c->a[2] == 3 && c->a[3] == 77);
    assert(icon(&data_00290a12_buildcard) == NULL);
    c = nth(F_EMBLEM, 0);
    assert(c->a[0] == 50 && c->a[1] == 23 && c->a[2] == 9);
    /* Kind icon by attribute, attribute icon by Type2Attr(type 0, attribute 5). */
    assert(icon(gSzDuel_Icon_buildcard + 30)->a[0] == 10 + 0x6c4);
    assert(icon(data_00290020_buildcard + 50)->a[0] == 10 + 0x6de);
    /* Level, attack and defense masked to 13 bits, stars. */
    assert(nth(F_DIGIT, 1)->a[3] == 7 && nth(F_DIGIT, 1)->a[2] == 2);
    assert(nth(F_DIGIT, 2)->a[3] == 1500 && nth(F_DIGIT, 2)->a[2] == 4);
    assert(nth(F_DIGIT, 3)->a[3] == 1200 && nth(F_DIGIT, 3)->a[1] == 20 + 0x15);
    assert(nth(F_DIGIT, 4)->a[3] == 33 && nth(F_DIGIT, 4)->a[0] == 10 + 0x7e);
    /* Bag count (kind -1) 40 >= 9: the second count icon; then kind 3 count, three digits. */
    assert(icon(&data_00290878_buildcard) != NULL && icon(&data_0029086e_buildcard) == NULL);
    assert(nth(F_DIGIT, 5)->a[3] == 4 && nth(F_DIGIT, 5)->a[0] == 10 + 0xc0);
    assert(nth(F_FONT, 0)->ch == '1' && nth(F_FONT, 1)->ch == '2' && nth(F_FONT, 2)->ch == '3');
    assert(nth(F_FONT, 2)->a[0] == 10 + 0x120 && nth(F_FONT, 2)->a[4] == 1);
    assert(icon(&data_00290990_buildcard)->a[0] == 10 + 0x7cd);
    assert(nth(F_BOARD, 0)->a[0] == 7 && nth(F_BOARD, 0)->a[1] == 10 + 0x48);
    /* Name in three colours at scale 1 (width 100 <= 168). */
    assert(count(F_RATIO) == 3 && nth(F_RATIO, 0)->s == 1.0f);
    assert(nth(F_RATIO, 0)->p == gSzDuel_aicolMsg_buildcard && nth(F_RATIO, 0)->a[0] == 10 + 0x4e);
    assert(nth(F_RATIO, 2)->p == data_002be948_buildcard && nth(F_RATIO, 2)->a[1] == 20 + 7);
    assert(freed == 1 && count(F_SORT) == 0);
}

static void test_link_new_dim(void)
{
    reset(0, 3);
    card_record.link = 5;
    bag_new = 1;
    bag[3] = 0;
    msg_width = 336;
    data_00317490_buildcard[0xc58] = 0x34;
    data_00317490_buildcard[0xc59] = 0x12;
    lSzDuel_DrawBuildDeck_Card_Pipe((void *)100, 77, 10, 20, 0, 1);
    /* Type 0 with a link: colour 4 + 7. */
    assert(trace[2].a[0] == 11 && nth(F_BOARD, 0)->a[0] == 11);
    /* New: the new icon instead of the number. */
    assert(icon(&data_00290a12_buildcard)->a[1] == 20 + 0x727);
    /* Bag count 0 < 9: the first count icon. */
    assert(icon(&data_0029086e_buildcard) != NULL && icon(&data_00290878_buildcard) == NULL);
    assert(nth(F_RATIO, 1)->s == 0.5f);
    /* Kind 3 count 0 with flag2: the dimming box. */
    assert(count(F_SORT) == 1 && alpha_seen == 0x1002);
    assert(((struct DotrBuildCardTag *)sorted.tag)->flags == 6 && ((struct DotrBuildCardTag *)sorted.tag)->count == 3);
    assert(((struct DotrBuildCardTag *)sorted.tag)->registers == 0x441);
    assert(sorted.color.r == 0x80 && sorted.color.a == 0x40 && sorted.color.q == 1.0f);
    assert(sorted.vertex0.x == (10 + 0x6c0) << 4 && sorted.vertex0.y == (20 + 0x720) << 4);
    assert(sorted.vertex1.x == (10 + 0x7f0) << 4 && sorted.vertex1.y == (20 + 0x756) << 4);
    assert(sorted.vertex0.z == 0x1234 && sorted.vertex1.z == 0x1234 && sorted.vertex1.fog == 0);
    /* With a positive count no box. */
    reset(0, 3);
    lSzDuel_DrawBuildDeck_Card_Pipe((void *)100, 77, 10, 20, 0, 1);
    assert(count(F_SORT) == 0);
}

static void test_other_kind_and_deck_layout(void)
{
    reset(2, 9);
    kind_type = 1;
    lSzDuel_DrawBuildDeck_Card_Pipe((void *)100, 77, 10, 20, 0, 0);
    /* Kind 2: icon entry 22, no emblem and no monster digits; type 3 + 7. */
    assert(icon(gSzDuel_Icon_buildcard + 220) != NULL);
    assert(count(F_EMBLEM) == 0 && icon(&data_002908e6_buildcard) == NULL);
    assert(trace[2].a[0] == 10 && nth(F_DIGIT, 1)->a[3] == 33);

    /* The deck-count layout: colour 6, the deck text and deck icon 0xe3 + deck. */
    reset(0, 3);
    lSzDuel_DrawBuildDeck_Card_Pipe((void *)100, 77, 10, 20, 1, 0);
    assert(trace[2].a[0] == 6 && nth(F_BOARD, 0)->a[0] == 6);
    assert(count(F_FONT) == 1 && nth(F_FONT, 0)->p == data_002befb7_buildcard && nth(F_FONT, 0)->a[4] == 4);
    assert(nth(F_FONT, 0)->a[3] == 2 && nth(F_FONT, 0)->a[0] == 10 + 0xa0);
    assert(icon(gSzDuel_Icon_buildcard + (2 + 0xe3) * 10)->a[0] == 10 + 0x7c7);
    assert(count(F_DIGIT) == 1);
}

int main(void)
{
    test_no_card();
    test_monster();
    test_link_new_dim();
    test_other_kind_and_deck_layout();
    return 0;
}

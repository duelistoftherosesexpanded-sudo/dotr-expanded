/* Synthetic host tests for the shslot group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. On the target several of the scalars below lie
 * inside the reel and flash structures; here they are separate host objects, and the
 * byte view of the reel-table pointer is kept equal to the structure view by hand. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/shslot.h"

/* Data fixtures. The reel array has eight entries because ShSlot_HitCardRequest reads
 * the center field at its step index (0..7) before the switch. */
struct DotrShSlotBoard data_00351210_shslot;
unsigned char MiddleStraightLineHighlight_shslot, TopStraightLineHighlight_shslot, BottomStraightLineHighlight_shslot;
unsigned char TopLeftDiagonalHighlight_shslot, BottomLeftDiagonalLineHighlight_shslot;
DotrShSlotWord data_00351228_shslot;
struct DotrShSlotDeck *data_00351230_shslot;
unsigned char *data_00351230_shslot_bytes;
struct DotrShSlotReel data_00351240_shslot[8];
unsigned char data_00351241_shslot, CanStillGet3InARow_shslot, data_00351244_shslot;
unsigned short data_00351246_shslot;
short data_00351248_shslot, SquareSlotReelImageOffset_shslot;
unsigned short SquareSlotCardID_shslot, data_00351292_shslot;
struct DotrShSlotFlash data_00351180_shslot[3];
unsigned char data_00351178_shslot, VisualDelayTimer_shslot, nextSlotDropNumber_shslot;
unsigned short nextSlotDrop_shslot;
int SlotTable_shslot[5] = { 0, 2, 1, 2, 0 };
struct DotrShSlotFloats data_002b8760_shslot;
unsigned char gSh_Lamp_shslot[17];
struct DotrShSlotPulse data_002b8e40_shslot;
struct DotrShSlotDeck dotr_shslot_demo_deck;

static struct DotrShSlotDeck deck;

enum {
    F_PLAY = 1, F_STOP, F_TIMER, F_FEVER, F_LOAD, F_CLUT, F_DOREQ, F_FLUSH, F_FREE, F_ITEM,
    F_LOADPIPE, F_POPUP, F_CLOSE, F_POP, F_PARTS, F_ALPHA, F_CLAMP, F_TEXTURE, F_SORT, F_DEPTH
};

struct call {
    int fn;
    long a[4];
};

static struct call trace[512];
static int ntrace;

static void add(int fn, long a, long b, long c, long d)
{
    assert(ntrace < 512);
    trace[ntrace].fn = fn;
    trace[ntrace].a[0] = a;
    trace[ntrace].a[1] = b;
    trace[ntrace].a[2] = c;
    trace[ntrace].a[3] = d;
    ntrace++;
}

static int count(int fn)
{
    int i, n = 0;

    for (i = 0; i < ntrace; i++) {
        n += trace[i].fn == fn;
    }
    return n;
}

/* The k-th call to fn, or NULL. */
static struct call *nth(int fn, int k)
{
    int i;

    for (i = 0; i < ntrace; i++) {
        if (trace[i].fn == fn && k-- == 0) {
            return &trace[i];
        }
    }
    return NULL;
}

static void expect(int fn, int k, long a, long b, long c, long d)
{
    struct call *c_ = nth(fn, k);

    assert(c_ != NULL);
    assert(c_->a[0] == a && c_->a[1] == b && c_->a[2] == c && c_->a[3] == d);
}

/* Packets sent to ExSortPrim_Pipe are copied (sort_size bytes, set per test). The
 * sprite packets of ShSlot_FlashBox and ShSlot_DrawReel are sent from their header,
 * 8 bytes in: those are stored at sort_offset 8 so the copy reads as the packet. */
static unsigned char sorted[64][136];
static int nsorted;
static size_t sort_size, sort_offset;
static int timer_running, pad_mask, analog, analog_value, fever_card;
static unsigned int random_next;
static struct {
    void *texture;
} items[4];
static int item_null;

int ShFx_isTimer(void *timer)
{
    (void)timer;
    return timer_running;
}
void ShFx_initTimer(void *timer, int a, int b, int c)
{
    add(F_TIMER, (long)((struct DotrShSlotFlash *)timer - data_00351180_shslot), a, b, c);
}
int ExAssignAlpha(void *packet, long long value)
{
    (void)packet;
    add(F_ALPHA, (long)value, 0, 0, 0);
    return 0;
}
int ExAssignClamp(void *packet, long long value)
{
    (void)packet;
    add(F_CLAMP, (long)value, 0, 0, 0);
    return 0;
}
int ExAssignTexture(void *packet, void *texture, int a)
{
    (void)packet;
    add(F_TEXTURE, (long)texture, a, 0, 0);
    return 0;
}
DotrShSlotWord ExSortPrim_Pipe(DotrShSlotWord cursor, void *packet, int a, int depth)
{
    add(F_SORT, cursor, a, depth, 0);
    assert(nsorted < 64);
    memcpy(sorted[nsorted++] + sort_offset, packet, sort_size);
    return cursor + 1;
}
float func_001a6898(float x) { return x; }
float func_001a6a88(float x) { return x + 1.0f; }
int func_001a22c0(float x) { return (int)x; }
int func_001a23b0(float x)
{
    add(F_DEPTH, (long)x, 0, 0, 0);
    return (int)x;
}
int func_0019a5c8(void)
{
    random_next = random_next * 1103515245u + 12345u;
    return (int)(random_next & 0x7fffffff);
}
int SzSound_PlaySE(int a, int b)
{
    add(F_PLAY, a, b, 0, 0);
    return 0;
}
int SzSound_StopSE(int a, int b)
{
    add(F_STOP, a, b, 0, 0);
    return 0;
}
int TaSlot_GetFeverGain(unsigned short card, unsigned char line)
{
    add(F_FEVER, card, line, 0, 0);
    return fever_card;
}
DotrShSlotWord SzText_LoadRequest(int group, int id)
{
    add(F_LOAD, group, id, 0, 0);
    return 0x1000 + group * 0x10 + id;
}
int RhBase_GetUserGsAddress(void) { return 0x2000; }
int SzText_InitClutManage(int base, int size)
{
    add(F_CLUT, base, size, 0, 0);
    return 0x40;
}
DotrShSlotWord SzText_DoRequest_Pipe(DotrShSlotWord cursor, int a, int b, int *out)
{
    add(F_DOREQ, cursor, a, b, out == NULL);
    return cursor + 0x100;
}
DotrShSlotWord RhText_SetTextureFlush(DotrShSlotWord cursor)
{
    add(F_FLUSH, cursor, 0, 0, 0);
    return cursor + 0x10;
}
int SzText_FreeRequest(int a, int b)
{
    add(F_FREE, a, b, 0, 0);
    return 0;
}
/* Item k is returned for the k-th request, except number item_null (NULL). */
DotrShSlotWord SzText_GetReadyItem(int kind, int card)
{
    int k = count(F_ITEM);

    add(F_ITEM, kind, card, 0, 0);
    return k == item_null ? 0 : (DotrShSlotWord)&items[k % 4];
}
DotrShSlotWord SzText_Load_Pipe(DotrShSlotWord cursor, int image, int base, int *size)
{
    add(F_LOADPIPE, cursor, image, base, 0);
    *size = 0x80;
    return cursor + 0x20;
}
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value) { add(F_POPUP, operation, a, b, value); }
void SzDuel_CloseMsgPopup(void) { add(F_CLOSE, 0, 0, 0, 0); }
void SzGame_Pop(void) { add(F_POP, 0, 0, 0, 0); }
int SzPad_Check(int pad, int kind, int mask)
{
    assert(pad == 0 && kind == 0);
    return (pad_mask & mask) != 0;
}
int SzPad_IsAnalog(int pad)
{
    assert(pad == 0);
    return analog;
}
int SzPad_GetAnalogInfo(int pad, int which)
{
    assert(pad == 0 && which == 3);
    return analog_value;
}
void ShSlot_DrawParts(int part) { add(F_PARTS, part, 0, 0, 0); }

static void reset(void)
{
    memset(&data_00351210_shslot, 0, sizeof(data_00351210_shslot));
    memset(data_00351240_shslot, 0, sizeof(data_00351240_shslot));
    memset(data_00351180_shslot, 0, sizeof(data_00351180_shslot));
    memset(&deck, 0, sizeof(deck));
    memset(&dotr_shslot_demo_deck, 0, sizeof(dotr_shslot_demo_deck));
    MiddleStraightLineHighlight_shslot = TopStraightLineHighlight_shslot = BottomStraightLineHighlight_shslot = 0;
    TopLeftDiagonalHighlight_shslot = BottomLeftDiagonalLineHighlight_shslot = 0;
    data_00351228_shslot = 0;
    data_00351230_shslot = &deck;
    data_00351230_shslot_bytes = &deck.count;
    data_00351241_shslot = CanStillGet3InARow_shslot = data_00351244_shslot = 0;
    data_00351246_shslot = 0;
    data_00351248_shslot = SquareSlotReelImageOffset_shslot = 0;
    SquareSlotCardID_shslot = data_00351292_shslot = 0;
    data_00351178_shslot = VisualDelayTimer_shslot = nextSlotDropNumber_shslot = 0;
    nextSlotDrop_shslot = 0;
    ntrace = nsorted = 0;
    sort_size = sort_offset = 0;
    timer_running = pad_mask = analog = analog_value = fever_card = 0;
    random_next = 1;
    item_null = -1;
}

static void check_is_flash_stop(void)
{
    reset();
    data_00351210_shslot.line_timer[4] = 1;
    data_00351210_shslot.cell_timer[2][2] = 1;
    assert(ShSlot_IsFlashStop() == 1);
    data_00351210_shslot.line_timer[3] = 2;
    assert(ShSlot_IsFlashStop() == 0);
    data_00351210_shslot.line_timer[3] = 0;
    data_00351210_shslot.cell_timer[1][2] = 2;
    assert(ShSlot_IsFlashStop() == 0);
}

static void check_start(void)
{
    int i;

    reset();
    data_00351241_shslot = 0x15;
    data_00351246_shslot = 77;
    ShSlot_Start();
    for (i = 0; i < 3; i++) {
        const struct DotrShSlotFlash *f = &data_00351180_shslot[i];

        assert(data_00351240_shslot[i].enabled == 1);
        expect(F_TIMER, i, i, 0x28, 0, 0xff);
        assert(f->flags == 0x80 && f->divisions == 9);
        assert(f->radius == 40.0f && f->speed == 0.0f);
        assert(f->center_r == 0xf0 && f->center_g == 0xf0 && f->center_b == 0xf0 && f->alpha == 0xf0);
        assert(f->r == 0x40 && f->g == 0x10 && f->b == 0x80);
        assert(f->x == (float)(0x78e + i * 0x70) && f->y == 2249.0f);
    }
    assert(data_00351240_shslot[0].state == 1);
    assert(count(F_PLAY) == 1);
    expect(F_PLAY, 0, 0, 0x1e, 0, 0);
    assert(trace[0].fn == F_PLAY && trace[1].fn == F_TIMER);
    assert(CanStillGet3InARow_shslot == 0x15 && data_00351246_shslot == 0);
}

static void check_hit_card_request(void)
{
    reset();
    deck.count = 4;
    deck.extra[1] = 0xfffe;
    deck.extra[3] = 0x123;
    data_00351240_shslot[0].center = 0x50;
    data_00351240_shslot[1].center = 0x50;
    data_00351240_shslot[2].center = 0x60;
    data_00351240_shslot[0].symbols[0] = 0x71;
    data_00351240_shslot[0].symbols[1] = 0x72;
    data_00351240_shslot[0].symbols[2] = 0x73;
    data_00351240_shslot[0].symbols[3] = 0x74;
    SquareSlotCardID_shslot = 0x50; /* reel 0's centre symbol */
    data_00351292_shslot = 0x50; /* reel 1's centre symbol */
    SquareSlotReelImageOffset_shslot = 3 * 0x70 + 5; /* reel 0's position */
    data_00351210_shslot.line_timer[1] = 0x10;
    data_00351210_shslot.line_timer[3] = 0x10;
    fever_card = 0x2a5;

    /* Step 0: reel 0's centre. */
    ShSlot_HitCardRequest();
    assert(nextSlotDrop_shslot == 0x50 && nextSlotDropNumber_shslot == 1);
    /* Step 1 repeats reel 0's symbol and is skipped; step 2 differs from both. */
    ShSlot_HitCardRequest();
    assert(nextSlotDrop_shslot == 0x60 && nextSlotDropNumber_shslot == 3);
    /* Steps 3 (line 0, not flashing) skipped; line 1 flashes with stop 0xfffe: fever draw
     * from reel 0's symbol (3 + offset[1] = 3 + 2) % 4 = 1. */
    ShSlot_HitCardRequest();
    assert(nextSlotDrop_shslot == 0x2a5 && nextSlotDropNumber_shslot == 5);
    expect(F_FEVER, 0, 0x72, 1, 0, 0);
    /* Line 3 flashes with a fixed stop card. */
    ShSlot_HitCardRequest();
    assert(nextSlotDrop_shslot == 0x123 && nextSlotDropNumber_shslot == 7);
    /* Nothing left: 0xffff and step 8. */
    ShSlot_HitCardRequest();
    assert(nextSlotDrop_shslot == 0xffff && nextSlotDropNumber_shslot == 8);
    assert(count(F_FEVER) == 1);
    /* A reel-0 centre of 0xffff is skipped too. */
    nextSlotDropNumber_shslot = 0;
    data_00351240_shslot[0].center = 0xffff;
    data_00351240_shslot[1].center = 0x51;
    ShSlot_HitCardRequest();
    assert(nextSlotDrop_shslot == 0x51 && nextSlotDropNumber_shslot == 2);
}

static void set_strip(int reel, unsigned short a, unsigned short b, unsigned short c, unsigned short d)
{
    data_00351240_shslot[reel].symbols[0] = a;
    data_00351240_shslot[reel].symbols[1] = b;
    data_00351240_shslot[reel].symbols[2] = c;
    data_00351240_shslot[reel].symbols[3] = d;
}

static void check_check_line(void)
{
    reset();
    deck.count = 4;
    /* Reel 0 window (position 0x70: from symbol 1) = A B C; reel 1 window = A B D;
     * reel 2 still spinning. */
    set_strip(0, 9, 0xa, 0xb, 0xc);
    set_strip(1, 0xa, 0xb, 0xd, 9);
    data_00351240_shslot[0].enabled = 4;
    data_00351240_shslot[0].position = 0x70;
    data_00351240_shslot[1].enabled = 4;
    data_00351240_shslot[1].position = 0;
    data_00351240_shslot[2].enabled = 2;
    CanStillGet3InARow_shslot = 0x1f;
    ShSlot_CheckLine(1);
    /* Rows 1 (B = B) and 0 (A = A) hold; row 2 (D, C) and both diagonals break. */
    assert(CanStillGet3InARow_shslot == 0x05);
    assert(data_00351240_shslot[1].center == 0xb);
    /* The strip wraps modulo count: position 3 * 0x70 reads symbols 3, 0, 1. */
    data_00351240_shslot[1].position = 3 * 0x70;
    CanStillGet3InARow_shslot = 0x1f;
    ShSlot_CheckLine(1);
    assert(data_00351240_shslot[1].center == 0xa);
    /* 0x334 on the line clears it even alone. */
    reset();
    deck.count = 4;
    set_strip(0, 1, 0x334, 3, 4);
    data_00351240_shslot[0].enabled = 4;
    CanStillGet3InARow_shslot = 0x1f;
    ShSlot_CheckLine(0);
    assert(CanStillGet3InARow_shslot == 0x1e);
    assert(data_00351240_shslot[0].center == 0x334);
    /* Lines not in the mask are left alone. */
    CanStillGet3InARow_shslot = 0x02;
    ShSlot_CheckLine(0);
    assert(CanStillGet3InARow_shslot == 0x02);
}

static void check_flear(void)
{
    const struct DotrShSlotFlearPacket *p;
    struct DotrShSlotFlash *f = &data_00351180_shslot[1];

    reset();
    sort_size = sizeof(struct DotrShSlotFlearPacket);
    timer_running = 1;
    f->age = 2;
    f->duration = 0x28;
    f->flags = 0x80;
    f->divisions = 2;
    f->radius = 10.0f;
    f->speed = 2.0f;
    f->r = 1;
    f->g = 2;
    f->b = 3;
    f->center_r = 4;
    f->center_g = 5;
    f->center_b = 6;
    f->alpha = 0x40;
    f->x = 100.0f;
    f->y = 50.0f;
    data_00351228_shslot = 0x500;
    assert(ShSlot_Flear(f, 1) == 1);
    assert(f->radius == 12.0f);
    /* divisions * 4 triangles, each sorted with depth 0 (flags 0x80). */
    assert(nsorted == 8 && count(F_ALPHA) == 1 && count(F_DEPTH) == 8);
    expect(F_ALPHA, 0, 0x48, 0, 0, 0);
    expect(F_DEPTH, 0, 0, 0, 0, 0);
    expect(F_SORT, 0, 0x500, 0, 0, 0);
    assert(data_00351228_shslot == 0x508);
    p = (const struct DotrShSlotFlearPacket *)sorted[0];
    assert(p->count == 6 && p->primitive == 0x13 && p->flags == 0 && p->registers == 0x414141);
    assert(p->vertex[0].r == 4 && p->vertex[0].g == 5 && p->vertex[0].b == 6);
    assert(p->vertex[1].r == 1 && p->vertex[2].g == 2 && p->vertex[2].b == 3);
    /* Fading in: (alpha >> 3) * age at the centre, 0 at the rim. */
    assert(p->vertex[0].a == (0x40 >> 3) * 2 && p->vertex[1].a == 0 && p->vertex[2].a == 0);
    assert(p->vertex[0].position.bits.z == 0xfffffe && p->vertex[2].position.bits.adc == 1);
    assert(p->vertex[0].position.coordinates.x == 100 * 16 && p->vertex[0].position.coordinates.y == 50 * 16);
    /* Stub sine(0) = 0, cosine(0) = 1: the first rim point is (x, y + radius). */
    assert(p->vertex[1].position.coordinates.x == 100 * 16 && p->vertex[1].position.coordinates.y == 62 * 16);

    /* Behind (no 0x80): z 0xfffff8 and depth -1; fading out near the end; divisions
     * capped at 9; a stopped timer draws nothing. */
    reset();
    sort_size = sizeof(struct DotrShSlotFlearPacket);
    timer_running = 1;
    f->age = 0x24;
    f->duration = 0x28;
    f->divisions = 12;
    f->alpha = 0x80;
    assert(ShSlot_Flear(f, 1) == 1);
    assert(nsorted == 36);
    expect(F_DEPTH, 0, -1, 0, 0, 0);
    p = (const struct DotrShSlotFlearPacket *)sorted[0];
    assert(p->vertex[0].position.bits.z == 0xfffff8 && p->vertex[0].a == (0x80 >> 3) * 4);
    /* Bit 0x40 keeps the alpha as set. */
    reset();
    sort_size = sizeof(struct DotrShSlotFlearPacket);
    timer_running = 1;
    f->age = 0;
    f->flags = 0x40;
    f->divisions = 1;
    f->alpha = 0x80;
    ShSlot_Flear(f, 1);
    p = (const struct DotrShSlotFlearPacket *)sorted[0];
    assert(nsorted == 4 && p->vertex[0].a == 0x80);
    reset();
    ShSlot_Flear(data_00351180_shslot, 3);
    assert(nsorted == 0);
}

static void check_draw_eye(void)
{
    const struct DotrShSlotEyePacket *p;
    const struct DotrShSlotUv *uv;

    reset();
    sort_size = sizeof(struct DotrShSlotEyePacket);
    data_002b8e40_shslot.values[3] = 8;
    data_00351246_shslot = 0x13; /* pulse index 3 */
    data_00351240_shslot[1].enabled = 4;
    ShSlot_DrawEye();
    expect(F_LOAD, 0, 3, 7, 0, 0);
    expect(F_LOAD, 1, 9, 0, 0, 0);
    expect(F_DOREQ, 0, 0, 3, 0x2040, 1);
    expect(F_FREE, 0, 3, -1, 0, 0);
    /* Reels 0 and 2 (not stopped), twice. */
    assert(nsorted == 4);
    expect(F_TEXTURE, 0, 0x1037, 0, 0, 0);
    expect(F_TEXTURE, 2, 0x1090, 0, 0, 0);
    expect(F_ALPHA, 0, 0x44, 0, 0, 0);
    expect(F_ALPHA, 2, 0x48, 0, 0, 0);
    p = (const struct DotrShSlotEyePacket *)sorted[1];
    assert(((const struct DotrShSlotTag *)&p->header[0])->count == 9);
    assert(((const struct DotrShSlotTag *)&p->header[0])->flags == (0x24 | 0x800));
    assert(((const struct DotrShSlotTag *)&p->header[0])->registers == 0x424242421LL);
    assert(p->v0.xyz.x == (unsigned)((0x77e + 2 + 2 * 0x70) << 4) && p->v0.xyz.y == (unsigned)((0x8b8 + 2) << 4));
    assert(p->v3.xyz.x == p->v0.xyz.x + ((0x20 - 4) << 4) && p->v3.xyz.y == p->v0.xyz.y + ((0x20 - 4) << 4));
    uv = (const struct DotrShSlotUv *)&p->v0.texture;
    assert(uv->u == 0x420 && uv->v == 0xb00);
    uv = (const struct DotrShSlotUv *)&p->v3.texture;
    assert(uv->u == 0x580 && uv->v == 0xc60);
    p = (const struct DotrShSlotEyePacket *)sorted[0];
    uv = (const struct DotrShSlotUv *)&p->v1.texture;
    assert(uv->u == 0x2c0 && uv->v == 0xb00);
    p = (const struct DotrShSlotEyePacket *)sorted[2];
    assert(p->color.r == 0x60 && p->color.g == 0x80 && p->color.a == 8 * 3);
    assert(p->v0.xyz.x == 0x77d << 4 && p->v3.xyz.y == (0x8b7 << 4) + 0x220);
}

static void check_flash_box(void)
{
    const struct DotrShSlotTexPacket *p;
    int x = 1 * 0x70 + 0x760, y = 0x840 - 2 * 0x70;

    reset();
    sort_size = sizeof(struct DotrShSlotTexPacket) - 8;
    sort_offset = 8;
    gSh_Lamp_shslot[3] = 0x60;
    data_00351210_shslot.cell_timer[1][2] = 3;
    ShSlot_FlashBox();
    expect(F_LOAD, 0, 9, 1, 0, 0);
    expect(F_FREE, 0, 9, -1, 0, 0);
    assert(nsorted == 8 && count(F_CLAMP) == 4);
    expect(F_CLAMP, 0, 0x1C4391F044ALL, 0, 0, 0);
    expect(F_TEXTURE, 0, 0x1091, 1, 0, 0);
    expect(F_ALPHA, 0, 0x44, 0, 0, 0);
    expect(F_ALPHA, 1, 0x49, 0, 0, 0);
    p = (const struct DotrShSlotTexPacket *)sorted[0];
    assert(p->color.r == 0x60 && p->color.b == 0x60 && p->color.a == 0x30);
    assert(p->v0.xyz.x == (unsigned)((x - 9) * 16) && p->v0.xyz.y == (unsigned)((y - 9) * 16));
    assert(p->v3.xyz.x == p->v0.xyz.x + 0x390 && p->v3.xyz.y == p->v0.xyz.y + 0x390);
    /* The fourth corner is mirrored both ways. */
    p = (const struct DotrShSlotTexPacket *)sorted[6];
    assert(p->v3.xyz.x == (unsigned)((x + 0x30) * 16) && p->v3.xyz.y == (unsigned)((y + 0x30) * 16));
    assert(p->v0.xyz.x == p->v3.xyz.x + 0x390 && p->v0.xyz.y == p->v3.xyz.y + 0x390);
    /* No cell flashing: nothing drawn. */
    reset();
    ShSlot_FlashBox();
    assert(nsorted == 0 && count(F_LOAD) == 1);
}

static void check_draw_reel(void)
{
    const struct DotrShSlotTexPacket *p;
    const struct DotrShSlotUv *uv;
    struct call *c;
    int top = ((0x30 + 0x2b) << 7) / 96;

    reset();
    sort_size = sizeof(struct DotrShSlotTexPacket) - 8;
    sort_offset = 8;
    deck.count = 4;
    set_strip(1, 0x11, 0x12, 0x13, 0x14);
    data_00351240_shslot[1].angle = 10;
    data_00351240_shslot[1].position = 2 * 112 + 5;
    item_null = 1;
    ShSlot_DrawReel(1);
    expect(F_LOAD, 0, 9, 0, 0, 0);
    /* Symbols (2 + i) % 4. */
    expect(F_ITEM, 0, 1, 0x13, 0, 0);
    expect(F_ITEM, 1, 1, 0x14, 0, 0);
    expect(F_ITEM, 2, 1, 0x11, 0, 0);
    expect(F_ITEM, 3, 1, 0x12, 0, 0);
    /* Three background pieces, three images (item 1 is missing), four shading bands. */
    assert(count(F_LOADPIPE) == 3 && nsorted == 3 + 3 + 4);
    c = nth(F_LOADPIPE, 1);
    assert(c->a[2] == 0x2040 + 0x80);
    p = (const struct DotrShSlotTexPacket *)sorted[0];
    assert(p->v0.xyz.x == (unsigned)((1 * 0x70 + 0x75c) << 4) && p->v0.xyz.y == (unsigned)((0x750 + 10) << 4));
    uv = (const struct DotrShSlotUv *)&p->v0.texture;
    assert(uv->u == 0x30 << 4 && uv->v == 0x500);
    expect(F_CLAMP, 0, (0x30 << 4) | 0xa | (0x98 << 14) | 0x50000000 | (0x3fcLL << 32), 0, 0, 0);
    /* The first image is whole; the last (y = 5 + 0x120 - 0x150) is clipped at the top. */
    p = (const struct DotrShSlotTexPacket *)sorted[3];
    assert(p->v0.xyz.y == (unsigned)((5 + 0x120 + 0x720) << 4) && p->v2.xyz.y == p->v0.xyz.y + (0x60 << 4));
    expect(F_TEXTURE, 3, (long)items[0].texture, 1, 0, 0);
    p = (const struct DotrShSlotTexPacket *)sorted[5];
    uv = (const struct DotrShSlotUv *)&p->v0.texture;
    assert(uv->v == top << 4);
    assert(p->v0.xyz.y == (unsigned)((0x30 + 0x720) << 4) && p->v2.xyz.y == p->v0.xyz.y + ((0x60 - 0x5b) << 4));
    expect(F_CLAMP, 5, ((long long)top << 24) | 0x20000a | (0x80LL << 34), 0, 0, 0);
    /* Shading: from 0x80 to 0x20 in the first band. */
    p = (const struct DotrShSlotTexPacket *)sorted[6];
    assert(((const struct DotrShSlotGradPacket *)p)->v0.color.r == 0x80);
    assert(((const struct DotrShSlotGradPacket *)p)->v2.color.r == 0x20);
    expect(F_ALPHA, 3, 0x42 | (0x80LL << 32), 0, 0, 0);
    /* From angle 176 the background halves swap. */
    reset();
    sort_size = sizeof(struct DotrShSlotTexPacket) - 8;
    sort_offset = 8;
    deck.count = 4;
    data_00351240_shslot[0].angle = 180;
    ShSlot_DrawReel(0);
    expect(F_CLAMP, 0, (0x98 << 4) | 0xa | (0x100 << 14) | 0x50000000 | (0x3fcLL << 32), 0, 0, 0);
}

static void check_draw_pipe(void)
{
    reset();
    deck.count = 4;
    data_00351230_shslot_bytes = &deck.count;
    data_00351248_shslot = 4 * 0x70;
    set_strip(2, 1, 2, 3, 4);
    data_00351240_shslot[0].enabled = 1;
    data_00351240_shslot[0].velocity = 31;
    data_00351240_shslot[1].enabled = 2;
    data_00351240_shslot[1].velocity = 20;
    data_00351240_shslot[1].position = 4 * 0x70 - 10;
    data_00351240_shslot[1].angle = 340;
    data_00351240_shslot[2].enabled = 3;
    data_00351240_shslot[2].countdown = 1;
    data_00351240_shslot[2].position = 0x70 * 2 - 4;
    data_00351240_shslot[2].velocity = 4;
    data_00351246_shslot = 5;
    /* A zero cursor moves the reels but draws nothing. */
    assert(ShSlot_Draw_Pipe(0) == 0);
    assert(ntrace == 0);
    assert(data_00351240_shslot[0].velocity == 32 && data_00351240_shslot[0].enabled == 1);
    assert(data_00351240_shslot[0].position == 32);
    assert(data_00351240_shslot[1].position == 10 && data_00351240_shslot[1].angle == 360 - 351);
    /* Braking at countdown 1: velocity 4, then stopped and checked. */
    assert(data_00351240_shslot[2].position == 0x70 * 2 && data_00351240_shslot[2].velocity == 0);
    assert(data_00351240_shslot[2].enabled == 4 && data_00351240_shslot[2].center == 4);
    assert(data_00351246_shslot == 6);
    ShSlot_Draw_Pipe(0);
    assert(data_00351240_shslot[0].enabled == 2);
    /* Braking at countdown 5 snaps to the next symbol at velocity 16. */
    data_00351240_shslot[2].enabled = 3;
    data_00351240_shslot[2].countdown = 5;
    data_00351240_shslot[2].position = 0x70 + 7;
    ShSlot_Draw_Pipe(0);
    assert(data_00351240_shslot[2].position == 0xe0 + 16 && data_00351240_shslot[2].countdown == 4);

    /* Drawing while flashing, in hit phase 1 with a card found at step 0. */
    reset();
    deck.count = 4;
    data_00351210_shslot.active = 1;
    data_00351210_shslot.line_timer[2] = 3;
    data_00351210_shslot.line_timer[3] = 1;
    data_00351210_shslot.cell_timer[1][0] = 2;
    data_00351240_shslot[0].state = 3;
    data_00351240_shslot[0].center = 0x77;
    data_00351178_shslot = 1;
    {
        DotrShSlotWord cursor = ShSlot_Draw_Pipe(0x100);

        assert(cursor == data_00351228_shslot && cursor > 0x100);
    }
    assert(count(F_PARTS) == 3);
    expect(F_PARTS, 0, 0, 0, 0, 0);
    expect(F_PARTS, 1, 2, 0, 0, 0);
    expect(F_PARTS, 2, 1, 0, 0, 0);
    assert(data_00351210_shslot.line_timer[2] == 2 && data_00351210_shslot.line_timer[3] == 1);
    assert(data_00351210_shslot.cell_timer[1][0] == 1);
    expect(F_PLAY, 0, 0, 7, 0, 0);
    expect(F_POPUP, 0, 2, 0, 0, 0x77);
    assert(data_00351178_shslot == 2 && VisualDelayTimer_shslot == 0);
    /* A card from the line steps: sound 25 and popup 3. */
    data_00351178_shslot = 1;
    nextSlotDropNumber_shslot = 3;
    data_00351210_shslot.line_timer[0] = 1;
    deck.extra[0] = 0x99;
    ntrace = nsorted = 0;
    ShSlot_Draw_Pipe(0x100);
    assert(count(F_PARTS) == 3);
    expect(F_PLAY, 0, 0, 25, 0, 0);
    expect(F_POPUP, 0, 3, 0, 0, 0x99);
    /* Nothing left: close the popup, hit phase 6, wait 8. */
    data_00351178_shslot = 1;
    memset(&data_00351210_shslot, 0, sizeof(data_00351210_shslot));
    ntrace = nsorted = 0;
    ShSlot_Draw_Pipe(0x100);
    assert(count(F_PARTS) == 2 && count(F_CLOSE) == 1 && count(F_POPUP) == 0);
    assert(data_00351178_shslot == 6 && VisualDelayTimer_shslot == 8);
}

static void check_init(void)
{
    int i, j, k, n;

    /* Demo mode fills the table at 0x1ed0000. */
    reset();
    ShSlot_Init(1);
    assert(data_00351244_shslot == 1 && data_00351230_shslot == &dotr_shslot_demo_deck);
    assert(dotr_shslot_demo_deck.count == 0x10 && dotr_shslot_demo_deck.flags == 2);
    for (i = 0; i < 5; i++) {
        assert(dotr_shslot_demo_deck.extra[i] == 0xfffe);
    }
    for (i = 0; i < 0x10; i++) {
        assert(i == 8 ? dotr_shslot_demo_deck.cards[0][i] == 0x334
                      : dotr_shslot_demo_deck.cards[0][i] >= 5 && dotr_shslot_demo_deck.cards[0][i] <= 7);
        assert(data_00351240_shslot[0].symbols[i] == dotr_shslot_demo_deck.cards[0][i]);
    }
    assert(data_00351241_shslot == 0x1f && CanStillGet3InARow_shslot == 0);
    assert(data_00351248_shslot == 0x10 * 0x70 && count(F_PLAY) == 0);
    /* Reels 1 and 2 are shuffled copies of reel 0 (flags bit 0 clear). */
    for (j = 1; j < 3; j++) {
        for (k = 0; k < 0x10; k++) {
            n = 0;
            for (i = 0; i < 0x10; i++) {
                n += data_00351240_shslot[j].symbols[i] == data_00351240_shslot[0].symbols[k];
                n -= data_00351240_shslot[0].symbols[i] == data_00351240_shslot[0].symbols[k];
            }
            assert(n == 0);
        }
    }

    /* A set table: own strips, lines from the stops, then ShSlot_Start. */
    reset();
    dotr_shslot_demo_deck.count = 3;
    dotr_shslot_demo_deck.flags = 1;
    dotr_shslot_demo_deck.extra[0] = 0xffff;
    dotr_shslot_demo_deck.extra[1] = 0xffff;
    dotr_shslot_demo_deck.extra[2] = 5;
    dotr_shslot_demo_deck.extra[3] = 0xffff;
    dotr_shslot_demo_deck.extra[4] = 0xfffe;
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
            dotr_shslot_demo_deck.cards[j][i] = (unsigned short)(0x100 * j + i);
        }
    }
    data_00351210_shslot.active = 1;
    data_00351210_shslot.cell_timer[2][1] = 4;
    BottomStraightLineHighlight_shslot = 9;
    data_00351240_shslot[1].position = 0x55;
    data_00351240_shslot[1].velocity = 3;
    data_00351240_shslot[1].center = 2;
    ShSlot_Init(0);
    assert(dotr_shslot_demo_deck.extra[0] == 0xfffe && dotr_shslot_demo_deck.count == 3);
    assert(data_00351241_shslot == 0x15 && data_00351248_shslot == 3 * 0x70);
    assert(data_00351210_shslot.active == 0 && data_00351210_shslot.cell_timer[2][1] == 0 && BottomStraightLineHighlight_shslot == 0);
    assert(data_00351240_shslot[1].position == 0 && data_00351240_shslot[1].velocity == 0);
    assert(data_00351240_shslot[1].center == 0xffff);
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 3; i++) {
            assert(data_00351240_shslot[j].symbols[i] == 0x100 * j + i);
        }
    }
    /* ShSlot_Start ran: reels speeding up, game phase 1, all lines in play winning. */
    expect(F_PLAY, 0, 0, 0x1e, 0, 0);
    assert(data_00351240_shslot[0].state == 1 && data_00351240_shslot[2].enabled == 1);
    assert(CanStillGet3InARow_shslot == 0x15);
}

static void check_act(void)
{
    /* Phase 0: start on pad 0x4000 or on an analogue value above 0xf0. */
    reset();
    pad_mask = 0x4000;
    ShSlot_Act();
    expect(F_PLAY, 0, 0, 0x20, 0, 0);
    expect(F_PLAY, 1, 0, 0x1e, 0, 0);
    assert(data_00351240_shslot[0].state == 1);
    reset();
    analog = 1;
    analog_value = 0xf0;
    ShSlot_Act();
    assert(ntrace == 0);
    analog_value = 0xf1;
    ShSlot_Act();
    assert(count(F_PLAY) == 2 && data_00351240_shslot[1].enabled == 1);

    /* Phase 1: pad 0x10 stops reel 1. */
    reset();
    data_00351240_shslot[0].state = 1;
    data_00351240_shslot[0].enabled = 2;
    data_00351240_shslot[1].enabled = 2;
    data_00351240_shslot[1].position = 0x70 + 0x10;
    data_00351240_shslot[1].velocity = 32;
    data_00351240_shslot[2].enabled = 4;
    pad_mask = 0x10;
    ShSlot_Act();
    expect(F_PLAY, 0, 0, 0x21, 0, 0);
    expect(F_TIMER, 0, 1, 0x10, 0, 0xff);
    assert(data_00351180_shslot[1].radius == 40.0f && data_00351180_shslot[1].speed == 1.0f);
    assert(data_00351240_shslot[1].enabled == 3);
    assert(data_00351240_shslot[1].countdown == (0x70 - 0x10) / 32 + 5);
    assert(data_00351240_shslot[0].enabled == 2 && data_00351240_shslot[0].state == 1);
    /* With deck flags bit 1 the countdown gets (0x70 / velocity) * ((random >> 4) % 3). */
    deck.flags = 2;
    data_00351240_shslot[0].velocity = 16;
    pad_mask = 0x80;
    {
        unsigned int saved = random_next;
        int extra = (0x70 / 16) * ((func_0019a5c8() >> 4) % 3);

        random_next = saved;
        ShSlot_Act();
        assert(data_00351240_shslot[0].countdown == (unsigned char)(0x70 / 16 + 5 + extra));
    }
    /* All stopped: phase 2 and sound 0x1e stopped. */
    data_00351240_shslot[0].enabled = data_00351240_shslot[1].enabled = 4;
    pad_mask = 0;
    ntrace = 0;
    ShSlot_Act();
    assert(data_00351240_shslot[0].state == 2);
    expect(F_STOP, 0, 0, 0x1e, 0, 0);

    /* Phase 2: no winning line left -> phase 3; else the lowest line's flash starts. */
    reset();
    data_00351240_shslot[0].state = 2;
    CanStillGet3InARow_shslot = 0x18;
    ShSlot_Act();
    assert(data_00351210_shslot.active == 1 && TopLeftDiagonalHighlight_shslot == 0x10);
    assert(data_00351210_shslot.cell_timer[0][2] == 0x10 && data_00351210_shslot.cell_timer[1][1] == 0x10);
    assert(data_00351210_shslot.cell_timer[2][0] == 0x10 && BottomLeftDiagonalLineHighlight_shslot == 0);
    assert(CanStillGet3InARow_shslot == 0x10 && data_00351240_shslot[0].state == 2);
    /* Still flashing: wait. */
    ShSlot_Act();
    assert(CanStillGet3InARow_shslot == 0x10);
    memset(&data_00351210_shslot, 0, sizeof(data_00351210_shslot));
    TopLeftDiagonalHighlight_shslot = 0;
    ShSlot_Act();
    assert(BottomLeftDiagonalLineHighlight_shslot == 0x10 && data_00351210_shslot.cell_timer[2][2] == 0x10);
    assert(CanStillGet3InARow_shslot == 0);
    memset(&data_00351210_shslot, 0, sizeof(data_00351210_shslot));
    BottomLeftDiagonalLineHighlight_shslot = 0;
    CanStillGet3InARow_shslot = 0x20; /* not one of the five lines */
    ShSlot_Act();
    assert(data_00351240_shslot[0].state == 3 && data_00351178_shslot == 0 && VisualDelayTimer_shslot == 0x10);

    /* Phase 3: hit phases 0, 2 and 6. */
    VisualDelayTimer_shslot = 1;
    ShSlot_Act();
    assert(VisualDelayTimer_shslot == 0 && data_00351178_shslot == 0);
    nextSlotDropNumber_shslot = 5;
    ShSlot_Act();
    assert(data_00351178_shslot == 1 && nextSlotDropNumber_shslot == 0 && VisualDelayTimer_shslot == 8);
    ShSlot_Act();
    assert(data_00351178_shslot == 1);
    data_00351178_shslot = 2;
    ShSlot_Act();
    assert(data_00351178_shslot == 2);
    pad_mask = 0x20;
    ShSlot_Act();
    assert(data_00351178_shslot == 1 && VisualDelayTimer_shslot == 8);
    pad_mask = 0;
    data_00351178_shslot = 6;
    VisualDelayTimer_shslot = 0;
    ShSlot_Act();
    assert(data_00351240_shslot[0].state == 4);

    /* Phase 4 pops the game state outside demo mode only. */
    ntrace = 0;
    ShSlot_Act();
    assert(count(F_POP) == 1);
    data_00351244_shslot = 1;
    ntrace = 0;
    ShSlot_Act();
    assert(count(F_POP) == 0);
    /* Demo mode: pad 0x800 restarts the demo, pad 0x100 pops. */
    pad_mask = 0x900;
    ShSlot_Act();
    assert(count(F_CLOSE) == 1 && count(F_POP) == 1);
    assert(trace[0].fn == F_CLOSE && data_00351230_shslot == &dotr_shslot_demo_deck);
    assert(dotr_shslot_demo_deck.count == 0x10 && data_00351240_shslot[0].state == 0);
}

int main(void)
{
    check_is_flash_stop();
    check_start();
    check_hit_card_request();
    check_check_line();
    check_flear();
    check_draw_eye();
    check_flash_box();
    check_draw_reel();
    check_draw_pipe();
    check_init();
    check_act();
    return 0;
}

/* Synthetic host tests for the model_texanim group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/model_texanim.h"

static int pause_flag, anim_number, calls[4];
static float anim_frame;

int SzModel_GetPauseFlag(int index) { assert(index == 5); calls[0]++; return pause_flag; }
int SzModel_GetAnim(int index) { assert(index == 5); calls[1]++; return anim_number; }
float SzModel_GetAnimFrame(int index) { assert(index == 5); calls[2]++; return anim_frame; }
int func_001a23b0(float value) { calls[3]++; return (int)value; }

static struct DotrTexAnimHead head;
static struct DotrTexAnimEntry entries[2];
static struct DotrTexAnimRecord record = {&head, 0, entries};
static DotrTexAnimLong packet[26];

/* One entry on a 4x2 sheet of 10x20 cells at (100, 200), drawn to (300, 400). */
static struct DotrTexAnimEntry *entry(int type)
{
    struct DotrTexAnimEntry *e = &entries[0];

    memset(entries, 0, sizeof(entries));
    e->last = 1;
    e->type = type;
    e->mask = 1 << 3;
    e->cols = 4;
    e->rows = 2;
    e->w = 10;
    e->h = 20;
    e->sx = 100;
    e->sy = 200;
    e->dx = 300;
    e->dy = 400;
    pause_flag = 0;
    anim_number = 3;
    return e;
}
/* Returns the number of packets written (from the result). */
static int run(float frame)
{
    DotrTexAnimWord end;

    anim_frame = frame;
    memset(packet, 0xee, sizeof(packet));
    end = SzModel_LoadTexAnim_Pipe((DotrTexAnimWord)packet, 5, 0x2a0, &record);
    return (int)(((DotrTexAnimLong *)end - packet) / 12);
}
/* TRXPOS of the first packet for a given cell (source x, y; destination x, y). */
static int at(int x, int y)
{
    return packet[6] == ((DotrTexAnimLong)x | ((DotrTexAnimLong)y << 16) | (300ul << 32) | (400ul << 48));
}

/* Branch coverage: each test and bound of the function is taken both ways below. */
static void check_load_tex_anim(void)
{
    struct DotrTexAnimEntry *e;

    assert(sizeof(struct DotrTexAnimEntry) == 24 && offsetof(struct DotrTexAnimEntry, counter) == 20);
    assert(offsetof(struct DotrTexAnimHead, tex) + offsetof(struct DotrTexAnimTexture, width) == 84);
    head.tex.psm = 0x13;
    head.tex.width = 0x100;

    /* Type 7 has no case: one packet for cell 0; every stored word and the tag words. */
    e = entry(7);
    memset(calls, 0, sizeof(calls));
    assert(run(9.0f) == 1);
    assert(calls[0] == 1 && calls[1] == 1 && calls[2] == 1 && calls[3] == 1);
    assert(packet[0] == 0x10000005 && packet[1] == 0x5100000511000000ul);
    assert(packet[2] == 0x1000000000008004ul && packet[3] == 0xe);
    assert(packet[4] == (0x2a0ul | (4ul << 16) | (0x13ul << 24) | (0x2a0ul << 32) | (4ul << 48) | (0x13ul << 56)));
    assert(packet[5] == 0x50 && at(100, 200) && packet[7] == 0x51);
    assert(packet[8] == (10ul | (20ul << 32)) && packet[9] == 0x52 && packet[10] == 2 && packet[11] == 0x53);
    assert(packet[12] == 0xeeeeeeeeeeeeeeeeul);

    /* Width below two units of 64 gives a buffer width of 1 (0x7f -> 1, 0x80 -> 2). */
    head.tex.width = 0x7f;
    assert(run(9.0f) == 1 && ((packet[4] >> 16) & 0xff) == 1 && ((packet[4] >> 48) & 0xff) == 1);
    head.tex.width = 0x80;
    assert(run(9.0f) == 1 && ((packet[4] >> 16) & 0xff) == 2);

    /* Nothing written: no cells, or the animation's bit is not in the mask; the buffer
     * and its tag words stay untouched. */
    e = entry(7);
    e->rows = 0;
    assert(run(9.0f) == 0 && packet[0] == 0xeeeeeeeeeeeeeeeeul && packet[1] == 0xeeeeeeeeeeeeeeeeul);
    e = entry(7);
    anim_number = 2;
    assert(run(9.0f) == 0);
    /* The same position as the source with cell 0 is skipped; either coordinate differing draws. */
    e = entry(7);
    e->dx = 100;
    e->dy = 200;
    assert(run(9.0f) == 0);
    e->dy = 201;
    assert(run(9.0f) == 1);
    e->dx = 101;
    e->dy = 200;
    assert(run(9.0f) == 1);

    /* Type 1: the cell advances and wraps when frame is a non-zero multiple of the span. */
    e = entry(1);
    e->u3.cell = 2;
    e->u4.h.a = 3;
    assert(run(6.0f) == 1 && e->u3.cell == 3 && at(130, 200));
    assert(run(7.0f) == 1 && e->u3.cell == 3);
    assert(run(0.0f) == 1 && e->u3.cell == 3);
    pause_flag = 1;
    assert(run(6.0f) == 1 && e->u3.cell == 3);
    pause_flag = 0;
    e->u3.cell = 7;
    assert(run(6.0f) == 1 && e->u3.cell == 0 && at(100, 200));
    e->u3.cell = 5;
    assert(run(1.0f) == 1 && e->u3.cell == 5 && at(110, 220));
    /* Modes 2 and 3 negate the cell (C remainder and quotient of -5 by 4); mode 1 does not. */
    e->mode = 2;
    assert(run(1.0f) == 1 && at(100 - 10, 200 - 20));
    e->mode = 3;
    assert(run(1.0f) == 1 && at(100 - 10, 200 - 20));
    e->mode = 1;
    assert(run(1.0f) == 1 && at(110, 220));
    /* The entry's own counter replaces the frame and advances by the step. */
    e = entry(1);
    e->count = 1;
    e->counter = 6;
    e->u3.cell = 2;
    e->u4.h.a = 3;
    assert(run(7.0f) == 1 && e->u3.cell == 3 && e->counter == 7);
    pause_flag = 1;
    assert(run(6.0f) == 1 && e->u3.cell == 3 && e->counter == 7);

    /* Type 0: one more cell than the sheet; cell 0 draws nothing, cell n draws sheet cell n-1. */
    e = entry(0);
    e->u3.cell = 8;
    e->u4.h.a = 3;
    assert(run(1.0f) == 1 && at(130, 220));
    assert(run(3.0f) == 0 && e->u3.cell == 0);
    assert(run(3.0f) == 1 && e->u3.cell == 1 && at(100, 200));

    /* Type 3: the position moves by the direction (span a going up, b going down), is kept
     * in 0..cells-1 and reverses at either end, setting the counter to 1. */
    e = entry(3);
    e->u3.swing.dir = 1;
    e->u3.swing.pos = 3;
    e->u4.h.a = 2;
    e->u4.h.b = 5;
    assert(run(4.0f) == 1 && e->u3.swing.pos == 4 && e->u3.swing.dir == 1 && e->counter == 0 && at(100, 220));
    assert(run(5.0f) == 1 && e->u3.swing.pos == 4);
    assert(run(0.0f) == 1 && e->u3.swing.pos == 4);
    pause_flag = 1;
    assert(run(4.0f) == 1 && e->u3.swing.pos == 4);
    pause_flag = 0;
    e->u3.swing.pos = 6;
    assert(run(4.0f) == 1 && e->u3.swing.pos == 7 && e->u3.swing.dir == -1 && e->counter == 1 && at(130, 220));
    e->counter = 0;
    assert(run(4.0f) == 1 && e->u3.swing.pos == 7);
    assert(run(5.0f) == 1 && e->u3.swing.pos == 6 && e->u3.swing.dir == -1 && e->counter == 0);
    e->u3.swing.pos = 1;
    assert(run(5.0f) == 1 && e->u3.swing.pos == 0 && e->u3.swing.dir == 1 && e->counter == 1);
    e->u3.swing.pos = 0;
    e->u3.swing.dir = -1;
    assert(run(5.0f) == 1 && e->u3.swing.pos == 0 && e->u3.swing.dir == 1);
    e->u3.swing.pos = 7;
    e->u3.swing.dir = 1;
    assert(run(4.0f) == 1 && e->u3.swing.pos == 7 && e->u3.swing.dir == -1);
    /* Type 2: the same with one more cell; position 0 draws nothing. */
    e = entry(2);
    e->u3.swing.dir = 1;
    e->u3.swing.pos = 7;
    e->u4.h.a = 2;
    e->u4.h.b = 5;
    assert(run(4.0f) == 1 && e->u3.swing.pos == 8 && e->u3.swing.dir == -1 && at(130, 220));
    e->u3.swing.pos = 0;
    assert(run(1.0f) == 0);

    /* Type 6: off -> on at a multiple of the span (if running and the limit is non-zero);
     * on counts time up to the limit, then goes off without drawing. */
    e = entry(6);
    e->u4.b.a = 4;
    e->u4.b.limit = 2;
    e->u4.b.time = 9;
    assert(run(5.0f) == 0 && e->u3.gate.on == 0);
    assert(run(8.0f) == 1 && e->u3.gate.on == 1 && e->u4.b.time == 0);
    assert(run(5.0f) == 1 && e->u3.gate.on == 1 && e->u4.b.time == 1);
    assert(run(5.0f) == 0 && e->u3.gate.on == 0 && e->u4.b.time == 2);
    pause_flag = 1;
    assert(run(8.0f) == 1 && e->u3.gate.on == 0);
    pause_flag = 0;
    e->u4.b.limit = 0;
    assert(run(8.0f) == 1 && e->u3.gate.on == 0);

    /* Type 5: the same with the frame equal to the span; while on, that frame holds the time. */
    e = entry(5);
    e->u4.b.a = 4;
    e->u4.b.limit = 2;
    e->u4.b.time = 9;
    assert(run(8.0f) == 0 && e->u3.gate.on == 0);
    assert(run(4.0f) == 1 && e->u3.gate.on == 1 && e->u4.b.time == 0);
    assert(run(4.0f) == 1 && e->u3.gate.on == 1 && e->u4.b.time == 0);
    assert(run(5.0f) == 1 && e->u3.gate.on == 1 && e->u4.b.time == 1);
    assert(run(5.0f) == 0 && e->u3.gate.on == 0 && e->u4.b.time == 2);
    pause_flag = 1;
    assert(run(4.0f) == 1 && e->u3.gate.on == 0);
    pause_flag = 0;
    e->u4.b.limit = 0;
    assert(run(4.0f) == 1 && e->u3.gate.on == 0);

    /* Type 4: drawn only for a..b inclusive. */
    e = entry(4);
    e->u4.h.a = 3;
    e->u4.h.b = 5;
    assert(run(2.0f) == 0 && run(3.0f) == 1 && run(5.0f) == 1 && run(6.0f) == 0);

    /* Two entries: the walk continues until the entry marked last; the tag counts both packets. */
    e = entry(7);
    e->last = 0;
    entries[1] = entries[0];
    entries[1].last = 1;
    entries[1].sx = 150;
    memset(calls, 0, sizeof(calls));
    assert(run(1.0f) == 2 && calls[0] == 1 && calls[1] == 2);
    assert(packet[0] == 0x1000000b && packet[1] == 0x5100000b11000000ul && packet[12] == 0x10000000);
    assert((packet[18] & 0xffff) == 150 && packet[24] == 0xeeeeeeeeeeeeeeeeul);
}

int main(void)
{
    check_load_tex_anim();
    return 0;
}

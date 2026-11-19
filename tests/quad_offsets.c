/* Synthetic host tests for the quad_offsets group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/quad_offsets.h"

static DotrDrawWord seen_cursor;
static int seen_points[8], seen_bytes[16], seen_mode, seen_tail, seen_calls;

DotrDrawWord Taku_Prim_Set_G4(DotrDrawWord cursor, short x0, short y0, short x1, short y1, short x2, short y2,
                           short x3, short y3, unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6, unsigned char b7, unsigned char b8, unsigned char b9, unsigned char b10, unsigned char b11, unsigned char b12, unsigned char b13, unsigned char b14, unsigned char b15,
                           unsigned short mode, short tail)
{
    int points[8] = {x0, y0, x1, y1, x2, y2, x3, y3};
    int bytes[16] = {b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15};

    memcpy(seen_points, points, sizeof(points));
    memcpy(seen_bytes, bytes, sizeof(bytes));
    seen_cursor = cursor;
    seen_mode = mode;
    seen_tail = tail;
    seen_calls++;
    return cursor + 0x40;
}

static struct DotrQuadRecord quad;

static void fill(void)
{
    int i;

    for (i = 0; i < 8; i++) {
        quad.points[i] = (short)(100 * (i + 1));
    }
    for (i = 0; i < 16; i++) {
        quad.bytes[i] = (unsigned char)(0xf0 + i);
    }
    quad.mode = 0x83;
    quad.unknown_21 = 0x55;
    quad.unknown_22 = -77;
    seen_calls = 0;
}

static void check_bytes(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        assert(seen_bytes[i] == 0xf0 + i);
    }
    assert(seen_mode == 0x83 && seen_tail == -77 && seen_calls == 1);
}

/* Straight-line forwarders: one call each checks every forwarded value and the result. */
static void check_offset(void)
{
    int i;

    fill();
    quad.points[7] = 0x7ffe;
    assert(Taku_G4_Set(0x1000, &quad, 3, -5) == 0x1040 && seen_cursor == 0x1000);
    for (i = 0; i < 7; i++) {
        assert(seen_points[i] == 100 * (i + 1) + (i & 1 ? -5 : 3));
    }
    assert(seen_points[7] == 0x7ffe - 5);
    check_bytes();
    /* a sum past the halfword range wraps */
    fill();
    quad.points[0] = 0x7fff;
    Taku_G4_Set(0, &quad, 1, 0);
    assert(seen_points[0] == -0x8000);
}

static void check_offset_size(void)
{
    static const int want[8] = {103, 195, 343, 1395, 503, 595, 743, 1795};
    int i;

    fill();
    assert(Taku_G4_Set2(0x2000, &quad, 3, -5, 40, 1000) == 0x2040 && seen_cursor == 0x2000);
    for (i = 0; i < 8; i++) {
        assert(seen_points[i] == want[i]);
    }
    check_bytes();
    fill();
    quad.points[2] = 0x7fff;
    Taku_G4_Set2(0, &quad, 0, 0, 1, 0);
    assert(seen_points[2] == -0x8000);
}

struct DotrSmallSheetRow tex_uv_data[3];
static int seen_values[8], seen_row_word, seen_flag;

DotrDrawWord Taku_Prim_Set_GT4(DotrDrawWord cursor, short x0, short y0, short x1, short y1, short x2, short y2,
                           short x3, short y3, short v0, short v1, short v2, short v3, short v4, short v5, short v6, short v7,
                           unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6, unsigned char b7,
                           unsigned char b8, unsigned char b9, unsigned char b10, unsigned char b11, unsigned char b12, unsigned char b13, unsigned char b14, unsigned char b15,
                           unsigned char mode, unsigned short row_word, unsigned char unknown_21, short tail)
{
    int points[8] = {x0, y0, x1, y1, x2, y2, x3, y3};
    int values[8] = {v0, v1, v2, v3, v4, v5, v6, v7};
    int bytes[16] = {b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15};

    memcpy(seen_points, points, sizeof(points));
    memcpy(seen_values, values, sizeof(values));
    memcpy(seen_bytes, bytes, sizeof(bytes));
    seen_cursor = cursor;
    seen_mode = mode;
    seen_row_word = row_word;
    seen_flag = unknown_21;
    seen_tail = tail;
    seen_calls++;
    return cursor + 0x80;
}

/* Straight-line forwarder: one call checks every forwarded value and the result. */
static void check_offset_size_row(void)
{
    static const int want[8] = {103, 195, 343, 1395, 503, 595, 743, 1795};
    struct DotrQuadIndexedRecord record;
    short *row = (short *)&tex_uv_data[2];
    int i;

    fill();
    record.quad = quad;
    record.row = 2;
    memset(tex_uv_data, 0, sizeof(tex_uv_data));
    for (i = 0; i < 8; i++) {
        row[i] = (short)(-1000 - i);
    }
    tex_uv_data[2].unknown_10 = 0xfedc;
    assert(Taku_GT4_Set(0x3000, &record, 3, -5, 40, 1000) == 0x3080 && seen_cursor == 0x3000);
    for (i = 0; i < 8; i++) {
        assert(seen_points[i] == want[i]);
        assert(seen_values[i] == -1000 - i);
    }
    check_bytes();
    assert(seen_row_word == 0xfedc && seen_flag == 0x55);
}

static int seen_ft4_calls, seen_st_calls;

static DotrDrawWord record_ft4(DotrDrawWord cursor, short x0, short y0, short x1, short y1, short x2, short y2,
        short x3, short y3, short v0, short v1, short v2, short v3, short v4, short v5, short v6, short v7,
        unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3,
        unsigned char mode, unsigned short row_word, unsigned char unknown_21, short tail)
{
    int points[8] = {x0, y0, x1, y1, x2, y2, x3, y3};
    int values[8] = {v0, v1, v2, v3, v4, v5, v6, v7};

    memcpy(seen_points, points, sizeof(points));
    memcpy(seen_values, values, sizeof(values));
    seen_bytes[0] = b0;
    seen_bytes[1] = b1;
    seen_bytes[2] = b2;
    seen_bytes[3] = b3;
    seen_cursor = cursor;
    seen_mode = mode;
    seen_row_word = row_word;
    seen_flag = unknown_21;
    seen_tail = tail;
    return cursor + 0x30;
}

DotrDrawWord Taku_Prim_Set_FT4(DotrDrawWord cursor, short x0, short y0, short x1, short y1, short x2, short y2,
        short x3, short y3, short v0, short v1, short v2, short v3, short v4, short v5, short v6, short v7,
        unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3,
        unsigned char mode, unsigned short row_word, unsigned char unknown_21, short tail)
{
    seen_ft4_calls++;
    return record_ft4(cursor, x0, y0, x1, y1, x2, y2, x3, y3, v0, v1, v2, v3, v4, v5, v6, v7, b0, b1, b2, b3, mode, row_word, unknown_21, tail);
}

DotrDrawWord Taku_Prim_Set_FT4_ST(DotrDrawWord cursor, short x0, short y0, short x1, short y1, short x2, short y2,
        short x3, short y3, short v0, short v1, short v2, short v3, short v4, short v5, short v6, short v7,
        unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3,
        unsigned char mode, unsigned short row_word, unsigned char unknown_21, short tail)
{
    seen_st_calls++;
    return record_ft4(cursor, x0, y0, x1, y1, x2, y2, x3, y3, v0, v1, v2, v3, v4, v5, v6, v7, b0, b1, b2, b3, mode, row_word, unknown_21, tail) + 0x10;
}

/* Branch coverage: mode 0 (first builder, row unchanged) and modes 1, 2, 3 and
 * an unlisted mode 4 (second builder; 1 off the even, odd, both, neither). */
static void check_ft4_set(void)
{
    static const int du[5] = {0, 1, 0, 1, 0};
    static const int dv[5] = {0, 0, 1, 1, 0};
    struct DotrQuadIndexedRecord record;
    short *row = (short *)&tex_uv_data[2];
    int mode, i;

    fill();
    record.quad = quad;
    record.row = 2;
    memset(tex_uv_data, 0, sizeof(tex_uv_data));
    for (i = 0; i < 8; i++) {
        row[i] = (short)(-1000 - i);
    }
    tex_uv_data[2].unknown_10 = 0xfedc;
    for (mode = 0; mode < 5; mode++) {
        seen_ft4_calls = seen_st_calls = 0;
        memset(seen_bytes, 0, sizeof(seen_bytes));
        assert(Taku_FT4_Set(0x3000, &record, 3, -5, (unsigned short)mode) == (mode == 0 ? 0x3030 : 0x3040));
        assert(seen_cursor == 0x3000 && seen_ft4_calls == (mode == 0) && seen_st_calls == (mode != 0));
        for (i = 0; i < 8; i++) {
            assert(seen_points[i] == 100 * (i + 1) + ((i & 1) ? -5 : 3));
            assert(seen_values[i] == -1000 - i - ((i & 1) ? dv[mode] : du[mode]));
        }
        for (i = 0; i < 4; i++) {
            assert(seen_bytes[i] == 0xf0 + i);
        }
        assert(seen_mode == 0x83 && seen_tail == -77 && seen_row_word == 0xfedc && seen_flag == 0x55);
    }
}

int main(void)
{
    check_offset();
    check_offset_size();
    check_offset_size_row();
    check_ft4_set();
    puts("quad_offsets: ok");
    return 0;
}

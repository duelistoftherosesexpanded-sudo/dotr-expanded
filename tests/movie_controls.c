/* Synthetic host tests for the movie_controls group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. No hardware registers are modelled. */
#include "dotr/ee/movie_controls.h"
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>

struct DotrMovieControlPrefix gSzGame_gl;

/* SzGame_ModeDraw callees: each records an identifying code and its arguments. */
static int draw_log[16];
static int draw_count;
static struct DotrModeDrawPacket draw_packet;
static void draw_note(int code) { draw_log[draw_count++] = code; }
void SzGame_DrawMenu(void) { draw_note(-1); }
void SzDuel_DrawWindow(void) { draw_note(1); }
void SzDuel_DrawBuildDeck(void) { draw_note(6); }
void SzDuel_Draw3DView(void) { draw_note(7); }
void SzDuel_DrawTrade(void) { draw_note(8); }
void SzDuel_DrawCustomDuel(void) { draw_note(9); }
void lSzGame_DrawTitle(void) { draw_note(10); }
void ExDitherMatrixEz(int a) { draw_note(100 + a); }
void TkCamp_Draw(void) { draw_note(11); }
void SzEvent_Draw(void) { draw_note(12); }
void SzDuel_DrawMapEdit(void) { draw_note(13); }
void SzDuel_DrawCardTest(void) { draw_note(18); }
void ShFace_Draw(void) { draw_note(14); }
void ShSlot_Draw(void) { draw_note(15); }
void ExSortPrim(void *packet, int a1, int a2)
{
    memcpy(&draw_packet, packet, sizeof(draw_packet));
    draw_note(17);
    draw_note(a1);
    draw_note(a2);
}
void lSzGame_TEXTURE_TEST(int texture, int a1, int a2, int a3)
{
    draw_note(200);
    draw_note(texture);
    draw_note(a1);
    draw_note(a2);
    draw_note(a3);
}
static void draw_mode(int mode)
{
    gSzGame_gl.byte_222 = 3;
    gSzGame_gl.levels_1f1[3] = (signed char)mode;
    draw_count = 0;
    SzGame_ModeDraw();
}

static void expect_bytes(const unsigned char *expected)
{
    assert(memcmp(expected, &gSzGame_gl, sizeof(gSzGame_gl)) == 0);
}

int main(void)
{
    static const int values[] = {INT_MIN, -2, -1, 0, 1, 2, 36, 37,
                                127, 128, 255, 256, 257, 999, INT_MAX};
    static const unsigned int counts[] = {0, 1, 37, 1000, 0x80000000u};
    static const int counters[] = {INT_MIN, -1, 0, 1, INT_MAX - 1};
    unsigned char expected[sizeof(gSzGame_gl)];
    unsigned char *raw = (unsigned char *)&gSzGame_gl;
    unsigned int i, j, flags;

    assert(offsetof(struct DotrMovieControlPrefix, word_000) == 0);
    assert(offsetof(struct DotrMovieControlPrefix, word_004) == 4);
    assert(offsetof(struct DotrMovieControlPrefix, word_0f4) == 0xf4);
    assert(offsetof(struct DotrMovieControlPrefix, byte_22a) == 0x22a);
    assert(offsetof(struct DotrMovieControlPrefix, byte_22b) == 0x22b);
    assert(offsetof(struct DotrMovieControlPrefix, bits_22e) == 0x22e);
    assert(sizeof(struct DotrMovieControlBits) == 1);

    for (i = 0; i < sizeof(counts) / sizeof(counts[0]); ++i) {
        for (j = 0; j < sizeof(values) / sizeof(values[0]); ++j) {
            memset(&gSzGame_gl, 0x5a, sizeof(gSzGame_gl));
            gSzGame_gl.word_0f4 = counts[i];
            memcpy(expected, raw, sizeof(expected));
            if (values[j] >= 0 && (unsigned int)values[j] < counts[i])
                expected[0x22b] = (unsigned char)values[j];
            SzGame_SetAutoBlur(values[j]);
            expect_bytes(expected);
            assert(SzGame_InqAutoBlur() == (signed char)expected[0x22b]);
        }
    }

    for (i = 0; i < 256; ++i) {
        memset(&gSzGame_gl, 0x5a, sizeof(gSzGame_gl));
        memcpy(expected, raw, sizeof(expected));
        expected[0x22a] = (unsigned char)i;
        SzGame_SetFrameSpeed((unsigned char)i);
        expect_bytes(expected);
        assert(SzGame_GetFrameSpeed() == i);
    }

    for (flags = 0; flags < 256; ++flags) {
        for (j = 0; j < sizeof(values) / sizeof(values[0]); ++j) {
            memset(&gSzGame_gl, 0x5a, sizeof(gSzGame_gl));
            raw[0x22e] = (unsigned char)flags;
            memcpy(expected, raw, sizeof(expected));
            assert(SzGame_InqSoftFocus() == ((flags & 0x20u) != 0));
            assert(SzGame_GetStabilizer() == ((flags & 0x40u) != 0));
            expected[0x22e] = (unsigned char)((flags & 0xbfu) |
                                  (((unsigned int)values[j] & 1u) << 6));
            SzGame_SetStabilizer(values[j]);
            expect_bytes(expected);
            assert(SzGame_InqSoftFocus() == ((flags & 0x20u) != 0));
            assert(SzGame_GetStabilizer() == (((unsigned int)values[j] & 1u) != 0));
        }
    }

    /* Excludes signed overflow: target addiu wraps; hosted C does not define it. */
    for (i = 0; i < sizeof(counters) / sizeof(counters[0]); ++i) {
        int incremented = counters[i] + 1;
        memset(&gSzGame_gl, 0x5a, sizeof(gSzGame_gl));
        gSzGame_gl.word_000 = counters[i];
        memcpy(expected, raw, sizeof(expected));
        memcpy(expected, &incremented, sizeof(incremented));
        assert(SzGame_VSync() == 1);
        expect_bytes(expected);

        gSzGame_gl.word_004 = counters[i];
        memcpy(expected, raw, sizeof(expected));
        memcpy(expected + 4, &incremented, sizeof(incremented));
        assert(SzGame_GetFrameCount() == counters[i]);
        assert(SzGame_AddFrameCount() == incremented);
        expect_bytes(expected);
    }

    /* SzGame_GetMode: one assertion per branch outcome and the 0 boundary. */
    assert(offsetof(struct DotrMovieControlPrefix, levels_1f1) == 0x1f1);
    assert(offsetof(struct DotrMovieControlPrefix, level_21e) == 0x21e);
    assert(offsetof(struct DotrMovieControlPrefix, byte_222) == 0x222);
    memset(&gSzGame_gl, 0x5a, sizeof(gSzGame_gl));
    gSzGame_gl.byte_222 = 0x12;
    gSzGame_gl.levels_1f1[0x12] = -3;
    gSzGame_gl.level_21e = -128;
    gSzGame_gl.level_21f = 127;
    memcpy(expected, raw, sizeof(expected));
    assert(SzGame_GetMode(1) == -3);
    assert(SzGame_GetMode(INT_MAX) == -3);
    assert(SzGame_GetMode(-1) == -128);
    assert(SzGame_GetMode(0) == 127);
    expect_bytes(expected);

    /* SzGame_ModeDraw: one assertion per branch outcome: the level bound (2 draws, 3 does
     * not), the hidden byte, every case, modes without a case on both sides of the table
     * (-2, 0, 16, 19), and in mode 17 each texture word zero and non-zero. */
    {
        static const int single[] = { -1, 1, 6, 7, 8, 9, 10, 13, 14, 15, 18 };
        static const int none[] = { -2, 0, 2, 5, 16, 19 };
        unsigned int k;
        memset(&gSzGame_gl, 0, sizeof(gSzGame_gl));
        assert(offsetof(struct DotrMovieControlPrefix, texture_204) == 0x204);
        assert(offsetof(struct DotrMovieControlPrefix, word_218) == 0x218);
        assert(offsetof(struct DotrMovieControlPrefix, hidden_228) == 0x228);
        assert(offsetof(struct DotrMovieControlPrefix, bits_22c) == 0x22c);
        assert(offsetof(struct DotrMovieControlPrefix, bits_22f) == 0x22f);
        gSzGame_gl.bits_22c.level = 3;
        draw_mode(1);
        assert(draw_count == 0);
        gSzGame_gl.bits_22c.level = 2;
        draw_mode(1);
        assert(draw_count == 1 && draw_log[0] == 1);
        gSzGame_gl.hidden_228 = 1;
        draw_mode(1);
        assert(draw_count == 0);
        gSzGame_gl.hidden_228 = 0;
        for (k = 0; k < sizeof(single) / sizeof(single[0]); ++k) {
            draw_mode(single[k]);
            assert(draw_count == 1 && draw_log[0] == single[k]);
        }
        for (k = 0; k < sizeof(none) / sizeof(none[0]); ++k) {
            draw_mode(none[k]);
            assert(draw_count == 0);
        }
        draw_mode(11);
        assert(draw_count == 2 && draw_log[0] == 101 && draw_log[1] == 11);
        draw_mode(12);
        assert(draw_count == 2 && draw_log[0] == 101 && draw_log[1] == 12);
        gSzGame_gl.colour_20c[0] = 0x11;
        gSzGame_gl.colour_20c[1] = 0x22;
        gSzGame_gl.colour_20c[2] = 0x33;
        gSzGame_gl.colour_20c[3] = 0x44;
        draw_mode(17);
        assert(draw_count == 3 && draw_log[0] == 17 && draw_log[1] == 0 && draw_log[2] == 0);
        assert(((struct DotrModeDrawHeader *)draw_packet.header)->unknown_00 == 3 && ((struct DotrModeDrawHeader *)draw_packet.header)->unknown_02 == 6);
        assert(((struct DotrModeDrawHeader *)draw_packet.header)->unknown_04 == 0 && ((struct DotrModeDrawHeader *)draw_packet.header)->unknown_08 == 1089);
        assert(draw_packet.colour[0] == 0x33 && draw_packet.colour[1] == 0x22);
        assert(draw_packet.colour[2] == 0x11 && draw_packet.colour[3] == 0x44);
        assert(draw_packet.unknown_14 == 1.0f);
        assert(draw_packet.vertex[0].x == 27640 && draw_packet.vertex[0].y == 29176);
        assert(draw_packet.vertex[0].z == 0xfffff8 && draw_packet.vertex[0].f == 0);
        assert(draw_packet.vertex[1].x == 0x93f8 && draw_packet.vertex[1].y == 0x8df8);
        assert(draw_packet.vertex[1].z == 0xfffff8 && draw_packet.vertex[1].f == 0);
        gSzGame_gl.texture_208 = 70;
        draw_mode(17);
        assert(draw_count == 8 && draw_log[3] == 200 && draw_log[4] == 70);
        assert(draw_log[5] == 0 && draw_log[6] == 0 && draw_log[7] == 0);
        gSzGame_gl.texture_208 = 0;
        gSzGame_gl.texture_204 = 71;
        gSzGame_gl.word_214 = 5;
        gSzGame_gl.word_218 = 6;
        gSzGame_gl.bits_22f.flag = -1;
        draw_mode(17);
        assert(draw_count == 8 && draw_log[3] == 200 && draw_log[4] == 71);
        assert(draw_log[5] == 5 && draw_log[6] == 6 && draw_log[7] == -1);
        gSzGame_gl.texture_208 = 70;
        gSzGame_gl.bits_22f.flag = 0;
        draw_mode(17);
        assert(draw_count == 13 && draw_log[4] == 70 && draw_log[9] == 71 && draw_log[12] == 0);
    }
    return 0;
}

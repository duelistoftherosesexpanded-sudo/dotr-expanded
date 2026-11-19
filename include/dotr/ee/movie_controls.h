#ifndef DOTR_EE_MOVIE_CONTROLS_H
#define DOTR_EE_MOVIE_CONTROLS_H

/* Address-based fields and names are provisional. Only the referenced prefix
 * is described; this is not evidence of a complete original object/type.
 * Evidence: docs/modules/game-modes.md.
 */
typedef char DotrMovieIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];
struct DotrMovieControlBits {
    signed char unknown_0_4 : 5;
    signed char bit_5 : 1;
    signed char bit_6 : 1;
    signed char bit_7 : 1;
};
/* SzGame_ModeDraw: a 2-bit unsigned field in bits 4-5 of +0x22c (drawing needs it below 3)
 * and a signed 1-bit field in bit 0 of +0x22f. */
struct DotrModeDrawBits {
    unsigned char unknown_0_3 : 4;
    unsigned char level : 2;
};
struct DotrModeDrawLast {
    signed char flag : 1;
};
struct DotrMovieControlPrefix {
    int word_000;
    int word_004;
    unsigned char unknown_008[0xec];
    unsigned int word_0f4;
    unsigned char unknown_0f8[0xf9];
    /* SzGame_GetMode: per-index signed levels, selected by byte_222. The table's length is
     * not known; it ends before the word at +0x204. */
    signed char levels_1f1[0x13];
    /* SzGame_ModeDraw (mode 17): two values passed to lSzGame_TEXTURE_TEST when non-zero,
     * the rectangle's colour bytes and two further arguments of the second call. */
    int texture_204;
    int texture_208;
    unsigned char colour_20c[4];
    int unknown_210;
    int word_214;
    int word_218;
    unsigned char unknown_21c[2];
    signed char level_21e;
    signed char level_21f;
    unsigned char unknown_220[2];
    unsigned char byte_222;
    unsigned char unknown_223[5];
    /* SzGame_ModeDraw draws nothing while this is non-zero. */
    unsigned char hidden_228;
    unsigned char unknown_229;
    unsigned char byte_22a;
    signed char byte_22b;
    struct DotrModeDrawBits bits_22c;
    unsigned char unknown_22d;
    struct DotrMovieControlBits bits_22e;
    struct DotrModeDrawLast bits_22f;
};
extern struct DotrMovieControlPrefix gSzGame_gl;

int SzGame_InqAutoBlur(void);
void SzGame_SetAutoBlur(int value);
int SzGame_InqSoftFocus(void);
int SzGame_VSync(void);
unsigned int SzGame_GetHSyncCount(void);
int SzGame_GetFrameCount(void);
int SzGame_AddFrameCount(void);
void SzGame_SetFrameSpeed(unsigned char value);
unsigned char SzGame_GetFrameSpeed(void);
void SzGame_SetStabilizer(int value);
int SzGame_GetStabilizer(void);
/* Returns int through a signed char local (extension at the single exit). */
int SzGame_GetMode(int direction);

/* SzGame_ModeDraw (0x1a9590, jump table at 0x2be680). The local packet is a 16-byte header,
 * four colour bytes, a float and two 64-bit vertex words (16-bit x and y, a 24-bit value and
 * a byte), the layout cursor_packets.h uses. Assumed: the callees take no arguments except
 * ExDitherMatrixEz(int), ExSortPrim (packet address and two ints; draw_state.h declares it
 * with three ints) and lSzGame_TEXTURE_TEST (four ints, from the argument moves). */
struct DotrModeDrawHeader {
    unsigned short unknown_00;
    unsigned short unknown_02;
    int unknown_04;
    long unknown_08;
};
struct DotrModeDrawXyz {
    unsigned long x : 16;
    unsigned long y : 16;
    unsigned long z : 24;
    unsigned long f : 8;
};
struct DotrModeDrawPacket {
    long header[2]; /* written through struct DotrModeDrawHeader */
    unsigned char colour[4];
    float unknown_14;
    struct DotrModeDrawXyz vertex[2];
};
void SzGame_DrawMenu(void);
void SzDuel_DrawWindow(void);
void SzDuel_DrawBuildDeck(void);
void SzDuel_Draw3DView(void);
void SzDuel_DrawTrade(void);
void SzDuel_DrawCustomDuel(void);
void lSzGame_DrawTitle(void);
void ExDitherMatrixEz(int a);
void TkCamp_Draw(void);
void SzEvent_Draw(void);
void SzDuel_DrawMapEdit(void);
void SzDuel_DrawCardTest(void);
void ShFace_Draw(void);
void ShSlot_Draw(void);
void ExSortPrim(void *packet, int a1, int a2);
void lSzGame_TEXTURE_TEST(int texture, int a1, int a2, int a3);
void SzGame_ModeDraw(void);

#endif

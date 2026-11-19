#ifndef DOTR_EE_MOVIE_UPDATE_H
#define DOTR_EE_MOVIE_UPDATE_H

/* Only the referenced prefix is declared; field meanings and the original type are assumed.
 * The storage overlaps the menu/control views at 0x317250 declared elsewhere.
 * Evidence: docs/modules/game-modes.md.
 */
typedef char DotrMovieUpdateIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];
struct MovieUpdateBits {
    signed char unknown_0_6 : 7;
    signed char bit_7 : 1;
};
struct MovieUpdatePrefix {
    unsigned char unknown_000[0x138];
    unsigned int menu_nums[37];
    unsigned char unknown_1cc[0x52];
    signed char byte_21e;
    unsigned char unknown_21f[0x0f];
    struct MovieUpdateBits bits_22e;
    unsigned char unknown_22f;
    unsigned char byte_230;
};
extern struct MovieUpdatePrefix dotr_ee_movie_update_state_00317250;
void SzGame_ModeInit(int GameMode);

#endif

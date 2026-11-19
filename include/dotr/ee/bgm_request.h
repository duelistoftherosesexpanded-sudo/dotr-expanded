#ifndef DOTR_EE_BGM_REQUEST_H
#define DOTR_EE_BGM_REQUEST_H

/* The request that picks and starts the background music for a game mode from the game block's
 * stage, flag and place words.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/game-modes.md. */

/* View of the game block gSzGame_gl at 0x317250 with the three words 0x1aaaf0 reads. Assumed: the
 * names (the word at +0x13c selects the duel music, the one at +0x14c the campaign music). */
struct DotrBgmGame {
    unsigned char unknown_000[316];
    int stage;
    int flag;
    unsigned char unknown_144[8];
    int place;
};
extern struct DotrBgmGame gSzGame_gl_bgm;

/* SzSound_PlayBGM (0x217980) and SzSound_StopBGM (0x217a10) as this caller uses them: the second
 * argument register is left holding this function's own second parameter at every call of the
 * first, so it is declared with two parameters here (small_state.h has it with none). */
extern int func_00217980_bgm(int n, int b);
extern int func_00217a10_bgm(int n);

/* 0x1aaaf0 SzGame_ReqBGM (name from the Japanese build's symbol table). kind = n % 1000:
 * -2 and -1 stop the music (256); 0 plays 7; 1 the duel music by `stage` (0: 7, 1: 10, 19: 11,
 * 20: 12, 21: 44, other stages below 22: 9 when `flag` is set else 8, from 22: 40) plus the
 * thousands of n; 5 the campaign music by `place` (0-9: 17, 18, 15, 14, 16, 20, 21, 13, 22, 19;
 * nothing otherwise); 6 and 7 play 5; 8: 41; 9: 3; 10: 2; 15: 21; any other kind does nothing.
 * Assumed: the second parameter (never read here, only passed on). */
void SzGame_ReqBGM(int n, int b);

#endif

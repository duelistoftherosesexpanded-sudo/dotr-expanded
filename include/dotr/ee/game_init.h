#ifndef DOTR_EE_GAME_INIT_H
#define DOTR_EE_GAME_INIT_H

/* The initialiser of the game block gSzGame_gl at 0x317250 (the first game function after the
 * library block).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/game-modes.md. */

/* SzGame_Init (the Japanese build's name, placed by position: no body match): clears the
 * 560-byte game block, gives each of 37 entries a kind, a limit, a start value and a step
 * byte, sets the remaining bytes and bit-fields, and reports three of the start values to
 * SzData_SetOption. Assumed: this complete view of the block (member names are this
 * project's; kind/limit/value/step are named after how the initialiser fills them), the
 * signedness of the bit-fields from the byte loads, void result. */
struct DotrGameBits556 {
    unsigned char b01 : 2;
    signed char b23 : 2;
    unsigned char b45 : 2;
    unsigned char b67 : 2;
};
struct DotrGameBits557 {
    unsigned char b02 : 3;
    unsigned char b35 : 3;
    unsigned char b67 : 2;
};
struct DotrGameBits558 {
    unsigned char b02 : 3;
    unsigned char b3 : 1;
    signed char b4 : 1;
    signed char b5 : 1;
    signed char b6 : 1;
    signed char b7 : 1;
};
struct DotrGameBlock {
    int w000;
    int w004;
    int w008;
    int w00c;
    int kind[37];        /* +0x010 */
    int limit[37];       /* +0x0a4; entry 7 is 683, the monster count */
    int value[37];       /* +0x138 */
    signed char step[37]; /* +0x1cc */
    signed char b497;
    char pad498[7];
    signed char b505;
    char pad506[10];
    int w516;
    int w520;
    signed char b524;
    signed char b525;
    signed char b526;
    unsigned char b527;
    int pad528;
    int w532;
    int w536;
    short h540;
    signed char b542;
    signed char b543;
    signed char b544;
    signed char b545;
    signed char b546;
    signed char b547;
    char pad548[4];
    signed char b552;
    signed char b553;
    signed char b554;
    signed char b555;
    struct DotrGameBits556 f556;
    struct DotrGameBits557 f557;
    struct DotrGameBits558 f558;
    char pad559;
};

typedef char DotrGameBlockMustBe230[(sizeof(struct DotrGameBlock) == 0x230) ? 1 : -1];
extern struct DotrGameBlock gSzGame_gl;
extern void *RhMem_FillMemory(void *destination, int value, int byte_count);
extern int SzData_SetOption(int side, int which, int flag);
void SzGame_Init(void);

#endif

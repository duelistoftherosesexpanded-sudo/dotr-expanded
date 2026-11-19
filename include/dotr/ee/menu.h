#ifndef DOTR_EE_MENU_H
#define DOTR_EE_MENU_H

/* Verified prefix only; the complete object's size and earlier fields are unknown.
 * The two accessors at 0x1a82e0/0x1a8320 use base 0x317250, member 0x138.
 * ANSI C spelling keeps this header usable by older C compilers.
 */
typedef char DotrMenuIntMustBe32Bits[(sizeof(int) == 4) ? 1 : -1];
struct DotrMenuStatePrefix {
    unsigned char unknown_000[0x138];
    int menu_nums[37];
};

extern struct DotrMenuStatePrefix dotr_ee_state_00317250;
int SzGame_GetMenuNum(int index);
/* Both names are the Japanese build's symbols. */
int SzGame_SetMenuNum(int index, int value);

#endif

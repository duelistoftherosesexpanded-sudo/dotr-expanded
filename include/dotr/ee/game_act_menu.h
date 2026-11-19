#ifndef DOTR_EE_GAME_ACT_MENU_H
#define DOTR_EE_GAME_ACT_MENU_H

/* The game's debug start menu: item selection, value editing and the action each item starts.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/game-modes.md. */

/* Addresses kept in 32-bit words in the game; a host pointer needs a wider word. */
#ifdef __MWERKS__
typedef int DotrActMenuWord;
typedef unsigned int DotrActMenuUWord;
#else
typedef long DotrActMenuWord;
typedef unsigned long DotrActMenuUWord;
#endif

/* The load address for item 17's text files (0x4000000, the second 32 MB of memory). The
 * function only computes and passes it on; the host form keeps the cast warning-free. */
#ifdef __MWERKS__
#define DOTR_ACT_MENU_LOAD ((unsigned char *)0x04000000)
#else
#define DOTR_ACT_MENU_LOAD ((unsigned char *)(DotrActMenuWord)0x04000000)
#endif

/* The flag byte at +0x22e: bit-field `enabled` set from item 21's value. Bit-fields of signed
 * char are an extension in ISO C; the host build marks them so. */
#ifdef __MWERKS__
#define DOTR_ACT_MENU_EXT
#else
#define DOTR_ACT_MENU_EXT __extension__
#endif
struct DotrActMenuFlags {
    DOTR_ACT_MENU_EXT signed char reserved0 : 5;
    DOTR_ACT_MENU_EXT signed char enabled : 1;
    DOTR_ACT_MENU_EXT signed char reserved1 : 2;
};

/* The menu block at 0x317250 (0x230 bytes) as this function uses it: per-item value limits at
 * +0xa4 and values at +0x138 (37 words each), two text load addresses at +0x204/+0x208, the
 * return mode at +0x220/+0x221, the selected row at +0x223, a mode byte at +0x22b, the page
 * (low three bits) at +0x22d and flags at +0x22e. Assumed: field meanings from their use here. */
struct DotrActMenuState {
    unsigned char pad000[0xA4];
    unsigned int limits[37];
    unsigned int values[37];
    unsigned char pad1CC[0x38];
    int firstAddress;
    int secondAddress;
    unsigned char pad20C[0x14];
    signed char x;
    signed char y;
    unsigned char pad222;
    unsigned char selected;
    unsigned char pad224[7];
    signed char mode;
    unsigned char pad22C;
    unsigned char page;
    struct DotrActMenuFlags flags;
};

/* A file read by SzFile_Read: the size in the low 31 bits of the word at +8. Assumed. */
struct DotrActMenuFile {
    unsigned char pad000[8];
    int size : 31;
    unsigned int flag : 1;
};

extern struct DotrActMenuState gSzGame_gl_actmenu;
/* The selected row and page bytes of the same block, read through their own addresses
 * (0x317473 = +0x223, 0x31747d = +0x22d). */
extern unsigned char data_00317473_actmenu;
extern unsigned char data_0031747d_actmenu;
/* The item number of each row, 20 rows per page. */
extern unsigned char DebugMenuOptions_actmenu[];
/* The environment label and the file name format for item 17. */
extern char data_002be7d0_actmenu[];
extern char data_002be7d5_actmenu[];

/* Callees (names from the Japanese build's symbols): SzFade_SetPauseFlag and SzSound_StopBGM take a
 * 64-bit argument; SzFade_Entry three unsigned char arguments; SzFile_GetEnv and SzFile_Read take
 * addresses as words; func_0019b648 is a sprintf-like formatter. Assumed: SzGame_ActMemCardProc
 * returns a negative value when no memory-card process runs; SzFade_GetRestCount(0) returns 0 when
 * the fade has finished; SzFile_Read returns the read file's header address or 0. */
extern int SzGame_ActMemCardProc(void);
extern int SzFade_GetRestCount(int index);
extern void SzGame_SetMode(int x, int y);
extern int SzFade_SetKillFlag(int index);
extern int SzFade_Destroy(int index);
extern int SzFade_Entry(int index, int kind, unsigned char a, unsigned char b, unsigned char frames, void *arg);
extern int SzFade_SetPauseFlag(int index, long long flag);
extern int SzPad_Check(int port, int kind, int mask);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
extern void SzData_SetupFromConstDeck(int side, int deckIndex);
extern void SzGame_Push(int mode, int arg);
extern unsigned char *SzFile_GetEnv(DotrActMenuWord label);
extern int func_0019b648(char *str, const char *fmt, ...);
extern DotrActMenuWord SzFile_Read(DotrActMenuWord name, DotrActMenuWord address, int size);
extern void RhText_BindAddress(int address);
extern int SzSound_PlayBGM(int index, int flag);
extern int SzSound_StopBGM(long long flag);
extern int SzSound_TransferSEGrp(int channel, int group, int flag);
extern int SzSound_StopSE(int channel, int index);
extern void SD_SuspendSoundSystem(int on);
extern void SzSound_Init(void);
extern void SD_StopAll(void);
extern void SzSound_Term(void);
extern int SzData_SetOption(int side, int which, int value);
extern void SD_BGMSetSourceLevel(int level, int value);
extern int SzData_SetRankDirect(int side, int cardNo, int rank);
extern void SzGame_PlayMovieNo(int movie, int a, int b);
extern int SzFile_InqRequestEx(void);
extern int SzText_LoadMerge(int index, int flag);
extern void SzGame_InitMemCardProc(int kind);

/* 0x1a7380 SzGame_ActMenu (name from the Japanese build's symbols). */
void SzGame_ActMenu(void);

#endif

#ifndef DOTR_EE_DUEL_DIALOG_H
#define DOTR_EE_DUEL_DIALOG_H

/* The duel dialog drawer and the hand-card drawer, which draw the duel's card dialogs and the
 * cards in hand from the control block at 0x317490.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* A packet cursor or an address kept as a 32-bit EE word (pointer-sized on the host). */
#ifdef __MWERKS__
typedef int DotrDialogWord;
#else
#include <stdint.h>
typedef intptr_t DotrDialogWord;
#endif

/* The control block at 0x317490 as bytes, in a union (an int stands for the alignment of a
 * partial field view this code does not use). */
union DotrDialogControl {
    unsigned char b[0xd70];
    int align;
};
extern union DotrDialogControl data_00317490_dialog;

/* ---- SzDuel_DrawHandCard_Pipe (0x1af180) views. Assumed: the
 * field meanings, from this function's use: a card node (screen x, y; colour at +0x44; flag
 * bytes +0x81 and +0x83; resource word at +0x90) and the card control entry (node pointer,
 * two 16-byte depth tracks at +0x10, scale at +0x70, duration and timer at +0x98 / +0x9a,
 * flag byte +0x9f). */
typedef struct {
    signed char bit0 : 1;
    signed char bit1 : 1;
    signed char bit2 : 1;
    signed char bit3 : 1;
    signed char bit4 : 1;
    signed char bit5 : 1;
    signed char bit6 : 1;
    signed char bit7 : 1;
} DotrDialogHandFlags;

typedef struct {
    unsigned char r, g, b, a;
} DotrDialogHandColor;

/* One 8-byte row of gSzDuel_aaicolRgba (0x2be840): the colour is bytes 4-7. */
typedef struct {
    unsigned char pad00[4];
    DotrDialogHandColor color;
} DotrDialogHandPalette;

struct DotrDialogHandCardNode {
    float x, y;
    unsigned char pad08[0x3C];
    DotrDialogHandColor color;
    unsigned char pad48[0x39];
    DotrDialogHandFlags flags81;
    unsigned char pad82;
    DotrDialogHandFlags flags83;
    unsigned char pad84[0xC];
    int resource;
};
typedef struct DotrDialogHandCardNode DotrDialogHandCardNode;

typedef struct {
    DotrDialogHandCardNode *object;
    unsigned char pad04[0xC];
    unsigned char tracks[2][0x10];
    unsigned char pad30[0x40];
    float scale;
    unsigned char pad74[0x24];
    unsigned short duration;
    unsigned short timer;
    unsigned char pad9C[3];
    DotrDialogHandFlags flags9F;
} DotrDialogHandActor;

/* The halfword at 0x318118 (control block + 0xc88); its high byte is the highlight alpha. */
extern unsigned short data_00318118_dialog;

/* Callees (Ex3d_GetSampleZ is declared inside the function, with the float first). Assumed:
 * these types. */
long long SzGame_GetMode(int arg0);
void *SzDuel_GetCardControl(int arg0);
void SzDuel_SetCardDefPrioZ(int arg0);
int SzDuel_GetCardNode(void *arg0, int arg1);
int SzDuel_GetCardNodeOnScreen(void *arg0, int arg1, float fparg0, float fparg1);
int SzDuel_SetupCard2_Pipe(int arg0, int unit, void *arg2);
int RhBase_SetZBufferMode(int arg0, int arg1);
int SzDuel_DrawCard_Pipe(int arg0, DotrDialogWord arg1);

/* 0x1af180. Assumed: the packet cursor and a card node pointer (overwritten before use).
 * Returns the advanced cursor. */
int SzDuel_DrawHandCard_Pipe(int arg0, DotrDialogHandCardNode *object);

/* ---- SzDuel_DrawDialog_Pipe (0x1af7e0) views. Assumed: the
 * field meanings, from this function's use: the card draw record (scale words at +0x30, flag
 * byte +0x81, 0x90 bytes) and the card information record (owner byte at +0x44, 0x60 bytes). */
typedef struct DotrDialogFlags {
    signed char unused : 4;
    signed char horizontal : 1;
    signed char enabled : 1;
    signed char colored : 1;
    signed char reserved : 1;
} DotrDialogFlags;

typedef struct DotrDialogDraw {
    unsigned char pad00[0x30];
    int scale[4];
    unsigned char pad40[0x41];
    DotrDialogFlags flags;
    unsigned char pad82[0xE];
} DotrDialogDraw;

typedef struct DotrDialogCard {
    unsigned char pad00[0x44];
    signed char owner;
    unsigned char pad45[0x1B];
} DotrDialogCard;

/* Data (each symbol links by address): the dialog title pointers
 * (0x291130), the icon table at 0x290080 (10-byte rows) and the single rows 0x290828,
 * 0x290832, 0x290aa8, 0x290ab2, 0x290af8 and 0x290b02 in it, the two choice strings 0x2bef6f
 * and 0x2bef73 and the highlight colour 0x2be944. MATCHING DEVICE: on the target each row is
 * a separate symbol; on the host the rows point into one
 * fixture. */
extern char *data_00291130_dialog[];
#ifdef __MWERKS__
extern unsigned char gSzDuel_Icon_dialog[];
extern unsigned char data_00290828_dialog[];
extern unsigned char data_00290832_dialog[];
extern unsigned char data_00290aa8_dialog[];
extern unsigned char data_00290ab2_dialog[];
extern unsigned char data_00290af8_dialog[];
extern unsigned char data_00290b02_dialog[];
#else
extern unsigned char gSzDuel_Icon_dialog[0xb00];
#define data_00290828_dialog (gSzDuel_Icon_dialog + 0x7a8)
#define data_00290832_dialog (gSzDuel_Icon_dialog + 0x7b2)
#define data_00290aa8_dialog (gSzDuel_Icon_dialog + 0xa28)
#define data_00290ab2_dialog (gSzDuel_Icon_dialog + 0xa32)
#define data_00290af8_dialog (gSzDuel_Icon_dialog + 0xa78)
#define data_00290b02_dialog (gSzDuel_Icon_dialog + 0xa82)
#endif
extern char data_002bef6f_dialog[];
extern char data_002bef73_dialog[];
extern char data_002be944_dialog[];

/* Callees with file-scope prototypes; the window, icon, frame, board, detail, font, message, rank,
 * hand-cursor and card-clear callees are declared inside the function. SzMsg_DrawRect_Pipe is only
 * passed by address. func_0019bef0 is unnamed (a string length). Assumed: these types. */
int func_0019bef0(const char *s);
int SzDuel_StartWindowSessionWithPalette_Pipe(int arg0, int arg1);
void SzDuel_SetWindowDefPrioZ(int arg0);
void SzMsg_SetDefPrioZ(int arg0);
int RhText_SetTextureFlush(int arg0);
void SzMsg_DrawRect_Pipe(void);
void SzMsg_Free(int arg0);
void *SzMsg_GetInfo(int arg0);
int SzDuel_GetMsgName(int cardNo);
void SzDuel_InitCardInfo(void *unit, int cardId);
int SzDuel_StartCardSession_Pipe(int arg0);
int SzDuel_FinishCardSessionX_Pipe(int arg0);
unsigned char SzDuel_GetPlayer(int side);
unsigned char *SzDuel_GetCardConstInfo(int cardNo);
int SzDuel_InqHand(int side, int arg1);

/* 0x1af7e0. Assumed: the packet cursor; returns the advanced cursor. */
int SzDuel_DrawDialog_Pipe(int arg0);

#endif

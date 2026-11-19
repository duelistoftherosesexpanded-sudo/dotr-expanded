#ifndef DOTR_EE_CARD_REQUEST_DRAW_H
#define DOTR_EE_CARD_REQUEST_DRAW_H

/* The card request pass of the duel scene (0x1cdb00), called from SzDuel_DrawAllCard_Pipe; reads
 * the 32-entry request table gSzDuel_aCardRequest.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */
#ifdef __MWERKS__
typedef int DotrCardRequestWord;
typedef __int128 DotrCardRequestWord128;
#else
#include <stdint.h>
typedef intptr_t DotrCardRequestWord;
__extension__ typedef __int128 DotrCardRequestWord128;
#endif

/* Board cell: only the signed level at +0x14 is read. */
struct DotrCardRequestCell {
    char unknown_00[20];
    short level;
};
/* The card a request belongs to: offsets at +0x34/+0x36, cell at +0x40/+0x41, two bytes at
 * +0x49/+0x4a that select the tint, and the top bit of +0x57. Names are assumed. */
struct DotrCardRequestItem {
    unsigned char unknown_00[52];
    short offset_x;
    short offset_z;
    unsigned char unknown_38[8];
    unsigned char x;
    unsigned char y;
    unsigned char unknown_42[7];
    unsigned char unknown_49;
    unsigned char unknown_4a;
    unsigned char unknown_4b[12];
    signed char unknown_57_bits : 7;
    signed char hidden : 1;
};
struct DotrCardRequestColor {
    unsigned char value[4];
};
/* The 144-byte card block, copied with quadword moves (so one member is quadword-aligned;
 * assumed to be the position). Bit-field names are positions, not meanings. */
struct DotrCardRequestBlock {
    union {
        DotrCardRequestWord128 quad;
        float value[4];
    } position;
    unsigned char unknown_10[0x30];
    unsigned char color[4];
    struct DotrCardRequestColor tint;
    unsigned char unknown_48[0x38];
    short unknown_80_bits_0_11 : 12;
    short flag_12 : 1;
    short flag_13 : 1;
    short flag_14 : 1;
    short flag_15 : 1;
    short count : 12;
    unsigned short unknown_82_bits_12_13 : 2;
    short flag_30 : 1;
    short flag_31 : 1;
    unsigned char unknown_84[12];
};
struct DotrCardRequest {
    struct DotrCardRequestBlock block;
    struct DotrCardRequestItem *item;
    int unknown_94[3];
};
/* Other headers view this 0x1400-byte table with their own element types. */
extern struct DotrCardRequest gSzDuel_aCardRequest[32];
/* Four constant bytes copied to the block's tint. */
extern struct DotrCardRequestColor data_002914d8;

/* Callees. Signatures are assumed from this call site. */
extern struct DotrCardRequestCell *SzDuel_GetMapInfo(int x, int y);
extern int SzDuel_GetCardNode(struct DotrCardRequestBlock *block, int a);
extern DotrCardRequestWord SzDuel_SetupCard2_Pipe(DotrCardRequestWord cursor, DotrCardRequestWord unit,
                                                  struct DotrCardRequestBlock *block);
extern DotrCardRequestWord SzDuel_DrawCard_Pipe(DotrCardRequestWord cursor, struct DotrCardRequestBlock *block);
extern DotrCardRequestWord RhBase_SetZBufferMode(DotrCardRequestWord cursor, int a);

DotrCardRequestWord SzDuel_DrawCardRequest_Pipe(DotrCardRequestWord cursor);

#endif

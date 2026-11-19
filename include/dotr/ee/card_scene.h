#ifndef DOTR_EE_CARD_SCENE_H
#define DOTR_EE_CARD_SCENE_H

/* SzDuel_DrawAllCard_Pipe (0x1cd710): the per-frame card pass of the duel scene.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */
#ifdef __MWERKS__
typedef int DotrCardSceneWord;
typedef __int128 DotrCardSceneQuad;
#else
#include <stdint.h>
typedef intptr_t DotrCardSceneWord;
__extension__ typedef __int128 DotrCardSceneQuad;
#endif

struct DotrCardSceneVector {
    float x;
    float y;
    float z;
    float w;
};
/* The 144-byte card block is copied with quadword moves, so it holds a quadword-aligned
 * member; which one is not known (assumed: the unread area at +0x10). color[3] is the alpha. */
struct DotrCardSceneBlock {
    struct DotrCardSceneVector pos;
    DotrCardSceneQuad unknown_10[3];
    unsigned char color[4];
    unsigned char unknown_44[0x4c];
};
/* The moving card: read at +4, +0x44, +0x45 and bit 6 of +0x57 only. */
struct DotrCardSceneItem {
    int unknown_00;
    struct DotrCardSceneBlock *block;
    unsigned char unknown_08[0x3c];
    unsigned char leader;
    unsigned char kind;
    unsigned char unknown_46[0x11];
    signed char unknown_57_bits_0_5 : 6;
    signed char active : 1;
};
struct DotrCardScenePlayer {
    unsigned char unknown_000[0x33d];
    unsigned char x;
    unsigned char y;
    unsigned char unknown_33f[0x11];
};
/* View of the control block at 0x317490: two 848-byte player records, the moving card at
 * +0x6c8 and a state byte at +0xd57. The current player index is read through its own
 * symbol 0x3181e4 (+0xd54 of the same block on the target; a separate host object). */
struct DotrCardSceneControl {
    struct DotrCardScenePlayer player[2];
    unsigned char unknown_6a0[0x28];
    struct DotrCardSceneItem *item;
    unsigned char unknown_6cc[0x68b];
    unsigned char state;
};
extern struct DotrCardSceneControl data_00317490;
extern unsigned char turnOwner[];

/* Callees. Signatures are assumed from this call site: arguments are the registers set at
 * each call; other headers declare some of them with their own views. */
extern int SzGame_GetMode(int a);
extern int SzModel_GetDrawFlag(int index);
extern DotrCardSceneWord ExSetAnchor_Pipe(DotrCardSceneWord cursor);
extern DotrCardSceneWord SzDuel_StartCardSession_Pipe(DotrCardSceneWord cursor);
extern DotrCardSceneWord SzDuel_DrawCardRequest_Pipe(DotrCardSceneWord cursor);
extern DotrCardSceneWord RhBase_SetZBufferMode(DotrCardSceneWord cursor, int a);
extern int SzDuel_GetUnitPos(int a, int b, struct DotrCardSceneVector *out);
extern DotrCardSceneWord SzDuel_DrawLeaderBase_Pipe(DotrCardSceneWord cursor, int side, struct DotrCardSceneVector *pos,
                                                    int alpha, int flag);
extern void *SzView_InqRequest(int a, int b);
extern struct DotrCardSceneVector *SzView_GetCP(int index);
extern void SzDuel_CalcUnitPos(int x, int y, int z, struct DotrCardSceneVector *v);
extern int SzDuel_GetCardNode(struct DotrCardSceneBlock *block, int a);
extern DotrCardSceneWord SzDuel_SetupCard2_Pipe(DotrCardSceneWord cursor, struct DotrCardSceneItem *unit,
                                                struct DotrCardSceneBlock *block);
extern DotrCardSceneWord SzDuel_DrawCard_Pipe(DotrCardSceneWord cursor, struct DotrCardSceneBlock *block);
extern DotrCardSceneWord SzDuel_DrawHandCard_Pipe(DotrCardSceneWord cursor);
extern DotrCardSceneWord SzDuel_FinishCardSession_Pipe(DotrCardSceneWord cursor);
extern int *ExGetMyOT(void);
extern DotrCardSceneWord ExSortAnchor_Pipe(DotrCardSceneWord cursor, DotrCardSceneWord anchor, int *table,
                                           unsigned int index);

DotrCardSceneWord SzDuel_DrawAllCard_Pipe(DotrCardSceneWord cursor);

#endif

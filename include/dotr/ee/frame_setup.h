#ifndef DOTR_EE_FRAME_SETUP_H
#define DOTR_EE_FRAME_SETUP_H

/* Functions that prepare a draw cursor through the 0x1fd320/0x202b00/0x1fd680 helpers and
 * record the result in the control block at 0x317490 (first member 0x1aefd0).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* A packet cursor kept as a 32-bit EE word (pointer on the target). */
typedef int DotrSetupWord;

/* View of the block at 0x317490 as 0x1aefd0 uses it: nine ints at +0x6a0 and two ints at
 * +0xc58 / +0xc7c. Other groups view the same storage under their own data_00317490_* names;
 * the slot entry is read at +0xc only. */
struct DotrSetupControl {
    unsigned char unknown_000[0x6a0];
    int unknown_6a0[9];
    unsigned char unknown_6c4[0xc54 - 0x6c4];
    int unknown_c54;
    int unknown_c58;
    unsigned char unknown_c5c[0xc7c - 0xc5c];
    int unknown_c7c;
    int unknown_c80;
    unsigned char unknown_c84[0xd64 - 0xc84];
    /* 0x1d9a30 clears bit 5 of the signed byte at +0xd64 as a one-bit field. */
    signed char unknown_d64_0 : 5;
    signed char unknown_d64_5 : 1;
};
struct DotrSetupEntry { unsigned char unknown_00[0xc]; int unknown_0c; };
extern struct DotrSetupControl data_00317490_setup;
extern int RhBase_GetUserGsAddress(void);
extern int SzText_InitClutManage(int base, int size);
extern DotrSetupWord SzMsg_Init_Pipe(DotrSetupWord cursor, int a0, int a1, int *out);
extern int SzText_LoadRequest(int group, int id);
extern DotrSetupWord SzText_DoRequest_Pipe(DotrSetupWord cursor, int a0, int a1, int *out);
extern int SzText_FreeRequest(int textureFlags, int textureId);
extern DotrSetupWord SzMsg_LoadFont_Pipe(DotrSetupWord cursor, int msgInfoIndex, int stringIndex, int a2, int a3);
extern struct DotrSetupEntry *SzMsg_GetInfo(int index);
extern DotrSetupWord RhText_SetTextureFlush(DotrSetupWord cursor);
extern void SzMsg_SetDefPrioZ(int v);


/* 0x1d9a30's first three callees; argument and result types are assumed (words passed
 * through). */
extern int RhBase_AssignRenderFrame(int index, int mode, int kind);
extern DotrSetupWord RhBase_EnableRenderFrame(DotrSetupWord cursor);
extern DotrSetupWord RhBase_ClearRenderFrame(DotrSetupWord cursor, int a1);
extern DotrSetupWord RhBase_DisableRenderFrame(DotrSetupWord cursor);

DotrSetupWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrSetupWord cursor, int a1);
DotrSetupWord SzDuel_StartCardSessionWithPalette_Pipe(DotrSetupWord cursor, int a1);
/* 0x1d9a20: one parameter assumed; the palette argument 2 is supplied here. */
DotrSetupWord SzDuel_StartCardSession_Pipe(DotrSetupWord cursor);
DotrSetupWord lSzDuel_SetupCardTexture(DotrSetupWord cursor, int textureId);

#endif

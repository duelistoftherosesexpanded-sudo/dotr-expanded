#ifndef DOTR_EE_CAMP_2D_H
#define DOTR_EE_CAMP_2D_H

/* The campaign map's 2D overlay frame Taku_Camp_2D, which loads the event card textures and runs
 * the camp window, event, status, help, cinema and message drawers in order.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Taku_Camp_2D reads the camp work area at the literal address 0x1ed0000 (the hand position at
 * 0x1ed0000/0x1ed0002, its kind 0x1ed0004, the help state 0x1ed0080 and flag bytes
 * 0x1ed5374..0x1ed5482) and the camp state at 0x299e10 as bytes (the event number at +0xc2; the
 * separate symbol 0x299ed2 is declared inside the function). The target build keeps the literal and
 * the symbol; the host build points them at test objects. Assumed: these meanings, from use. */
#ifdef __MWERKS__
extern signed char data_00299e10_camp2d;
#define DOTR_CAMP_2D_AT(a) (a)
#else
#include <stdint.h>
union DotrCamp2dBoard {
    signed char bytes[0x100];
    short align;
};
extern union DotrCamp2dBoard dotr_camp_2d_board;
#define data_00299e10_camp2d (dotr_camp_2d_board.bytes[0])
union DotrCamp2dWork {
    unsigned char bytes[0x5500];
    double align;
};
extern union DotrCamp2dWork dotr_camp_2d_work;
#define DOTR_CAMP_2D_AT(a) ((intptr_t)dotr_camp_2d_work.bytes + ((a) - 0x01ED0000))
#endif

/* Callees with file-scope prototypes (Taku_Camp_BUN_DARE and Taku_Camp_BOTAN_MATI are named by the
 * Japanese build's symbols at 0x2337a0 and 0x22f1c0). SzText_LoadRequest, SzText_InitClutManage and
 * Taku_Te_Set are declared inside the function body. Several are declared in other groups with
 * other views. Cursors are 32-bit words (int). */
extern int SzDuel_StartWindowSession_Pipe(int arg0);
extern int SzDuel_FinishWindowSession_Pipe(int arg0);
extern int SzDuel_GetWindowTexture(int arg0);
extern int SzDuel_GetUserGsAddress(void);
extern int SzEffect_IsUse2(int arg0, int arg1);
extern int SzText_DoRequest_Pipe(int arg0, int arg1, int arg2, int *arg3);
extern int SzText_FreeRequest(int textureFlags, int textureId);
extern int RhBase_GetUserGsAddress(void);
extern int RhText_SetTextureFlush(int arg0);
extern int Taku_Camp_Ivent_Window(int arg0);
extern int Taku_DECK_CARD_DISP(int arg0);
extern int Taku_Camp_Ivent_Ga(int cursor);
extern int Taku_Camp_BUN_HYOUJI(int p);
extern int Taku_Camp_SINEMA_CUT(int cursor, unsigned short fade);
extern int Taku_Camp_ST_WINDOW(int cursor);
extern void *Taku_Camp_SYS_ICON(void *arg0);
extern int Taku_Camp_TEKI_WINDOW(int cursor);
extern int Taku_Camp_TIKEI_WINDOW(int arg0);
extern int Taku_Camp_Help_Window(int cursor);
extern int Taku_Camp_BUN_DARE(int cursor, signed char who);
extern int Taku_Camp_BOTAN_MATI(int cursor);

/* 0x2297c0: one frame of the camp 2D layer; returns the cursor as a pointer. */
void *Taku_Camp_2D(int arg0);

#endif

/* Synthetic host tests for the camp_2d group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/camp_2d.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

union DotrCamp2dBoard dotr_camp_2d_board;
union DotrCamp2dWork dotr_camp_2d_work;
/* Declared inside the function. */
unsigned char te_data_camp2d[38];
unsigned char data_002964c6_camp2d[38];
unsigned char data_002964ec_camp2d[38];
int tex_data[16];
int data_00299dd4_camp2d;
short CutSceneNumber_camp2d;
static int calls;

int SzText_LoadRequest(void) { return 0; }
int SzText_InitClutManage(void) { return 0; }
int Taku_Te_Set(void) { return 0; }
int SzDuel_StartWindowSession_Pipe(int arg0) { calls++; return arg0 + 1; }
int SzDuel_FinishWindowSession_Pipe(int arg0) { calls++; return arg0 + 1; }
int SzDuel_GetWindowTexture(int arg0) { return 0x100 + arg0; }
int SzDuel_GetUserGsAddress(void) { return 0; }
int SzEffect_IsUse2(int arg0, int arg1) { return 0; }
int SzText_DoRequest_Pipe(int arg0, int arg1, int arg2, int *arg3) { return arg0 + 1; }
int SzText_FreeRequest(int arg0, int arg1) { return 0; }
int RhBase_GetUserGsAddress(void) { return 0; }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
int Taku_Camp_Ivent_Window(int arg0) { calls++; return arg0 + 1; }
int Taku_DECK_CARD_DISP(int arg0) { calls++; return arg0 + 1; }
int Taku_Camp_Ivent_Ga(int cursor) { calls++; return cursor + 1; }
int Taku_Camp_BUN_HYOUJI(int p) { calls++; return p + 1; }
int Taku_Camp_SINEMA_CUT(int cursor, unsigned short fade) { calls++; return cursor + 1; }
int Taku_Camp_ST_WINDOW(int cursor) { calls++; return cursor + 1; }
void *Taku_Camp_SYS_ICON(void *arg0) { calls++; return (char *)arg0 + 1; }
int Taku_Camp_TEKI_WINDOW(int cursor) { calls++; return cursor + 1; }
int Taku_Camp_TIKEI_WINDOW(int arg0) { calls++; return arg0 + 1; }
int Taku_Camp_Help_Window(int cursor) { calls++; return cursor + 1; }
int Taku_Camp_BUN_DARE(int cursor, signed char who) { calls++; return cursor + 1; }
int Taku_Camp_BOTAN_MATI(int cursor) { calls++; return cursor + 1; }

int main(void)
{
    /* No event (number 0), all flags clear: no texture loads; the session start, the system
     * icon, the status, enemy, terrain, cinema and message drawers, the button wait, the session
     * end and the deck card display each add one to the cursor; the window texture is stored. */
    assert(Taku_Camp_2D(0x1000) == (void *)(0x1000 + 10));
    assert(calls == 10 && tex_data[0] == 0x107);
    return 0;
}

/* Synthetic host tests for the duel_dialog group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/duel_dialog.h"

union DotrDialogControl data_00317490_dialog;
unsigned short data_00318118_dialog;
DotrDialogHandPalette gSzDuel_aaicolRgba_dialog[16];
char *data_00291130_dialog[32];
unsigned char gSzDuel_Icon_dialog[0xb00];
char data_002bef6f_dialog[4];
char data_002bef73_dialog[4];
char data_002be944_dialog[4];

static int card_control_ids[8];
static int card_control_calls;
static int session_calls;
static int frees[8];
static int free_calls;
static int cursor_args[4];
static int cursor_calls;

/* SzDuel_DrawHandCard_Pipe's callees. */
long long SzGame_GetMode(int arg0)
{
    return arg0 == 1 ? 1 : 0;
}
void *SzDuel_GetCardControl(int arg0)
{
    card_control_ids[card_control_calls++] = arg0;
    return 0;
}
void SzDuel_SetCardDefPrioZ(int arg0) { (void)arg0; }
int SzDuel_GetCardNode(void *arg0, int arg1) { (void)arg0; (void)arg1; return 0; }
int SzDuel_GetCardNodeOnScreen(void *arg0, int arg1, float fparg0, float fparg1)
{
    (void)arg0; (void)arg1; (void)fparg0; (void)fparg1;
    return 0;
}
int SzDuel_SetupCard2_Pipe(int arg0, int arg1, void *arg2) { (void)arg1; (void)arg2; return arg0 + 1; }
int RhBase_SetZBufferMode(int arg0, int arg1) { (void)arg1; return arg0 + 1; }
int SzDuel_DrawCard_Pipe(int arg0, DotrDialogWord arg1) { (void)arg1; return arg0 + 1; }
unsigned int Ex3d_GetSampleZ(float z, void *track) { (void)z; (void)track; return 0; }

/* SzDuel_DrawDialog_Pipe's callees (the block-scope ones with the argument types used here). */
int func_0019bef0(const char *s) { return (int)strlen(s); }
int SzDuel_StartWindowSessionWithPalette_Pipe(int arg0, int arg1) { (void)arg1; session_calls++; return arg0 + 10; }
void SzDuel_SetWindowDefPrioZ(int arg0) { assert(arg0 == 0xfffffe); }
void SzMsg_SetDefPrioZ(int arg0) { assert(arg0 == 0xfffffe); }
int RhText_SetTextureFlush(int arg0) { return arg0 + 1; }
void SzMsg_DrawRect_Pipe(void) {}
void SzMsg_Free(int arg0) { frees[free_calls++] = arg0; }
void *SzMsg_GetInfo(int arg0) { (void)arg0; return 0; }
int SzDuel_GetMsgName(int arg0) { return arg0; }
void SzDuel_InitCardInfo(void *arg0, int arg1) { (void)arg0; (void)arg1; }
int SzDuel_StartCardSession_Pipe(int arg0) { return arg0 + 1; }
int SzDuel_FinishCardSessionX_Pipe(int arg0) { return arg0 + 1; }
unsigned char SzDuel_GetPlayer(int arg0) { (void)arg0; return 0; }
unsigned char *SzDuel_GetCardConstInfo(int arg0) { (void)arg0; return 0; }
int SzDuel_InqHand(int arg0, int arg1) { (void)arg0; (void)arg1; return -1; }
/* Declared without prototypes inside SzDuel_DrawDialog_Pipe; not reached on the path tested. */
int lSzDuel_DrawDialog_Window_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawWindow_Icon_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawWindow_Frame_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawWindow_Board_Pipe(int packet) { return packet + 1; }
int lSzDuel_DrawBuildDeck_Detail_Pipe(int packet) { return packet + 1; }
int SzMsg_LoadFont_Pipe(int packet) { return packet + 1; }
int SzMsg_DrawX_Pipe(int packet) { return packet + 1; }
int SzDuel_DrawWindow_Font_Pipe(int packet) { return packet + 1; }
int SzData_GetRank(int side, int id) { (void)side; (void)id; return 0; }
void SzDuel_ClearDrawCard(void *draw) { (void)draw; }
int SzMsg_DrawRatioX_Pipe(int packet, int index, int x, int y, float sx, float sy, int e, void (*function)(void))
{
    (void)index; (void)x; (void)y; (void)sx; (void)sy; (void)e; (void)function;
    return packet + 1;
}
int lSzDuel_OpenHandCursor_Pipe(int packet, int angle, int x, int y)
{
    cursor_args[0] = packet;
    cursor_args[1] = angle;
    cursor_args[2] = x;
    cursor_args[3] = y;
    cursor_calls++;
    return packet + 100;
}

/* SzDuel_DrawHandCard_Pipe: mode 1 with flag bit 2 of +0xd67 set walks the six card slots
 * +0xd2c..+0xd31 from the last; no slot has a card control entry, so the cursor is unchanged. */
static void check_hand_card(void)
{
    unsigned char *state = data_00317490_dialog.b;
    int i;

    memset(state, 0, sizeof(data_00317490_dialog));
    state[0xd67] = 0x04;
    for (i = 0; i < 6; i++) {
        state[0xd2c + i] = (unsigned char)(10 + i);
    }
    assert(SzDuel_DrawHandCard_Pipe(500, 0) == 500);
    assert(card_control_calls == 6 && card_control_ids[0] == 15 && card_control_ids[5] == 10);
}

/* SzDuel_DrawDialog_Pipe: one hand-cursor dialog (kind 23) in slot 1, the others none (0x18). */
static void check_dialog(void)
{
    unsigned char *state = data_00317490_dialog.b;
    short x = 40;
    short y = -12;
    unsigned short value = 3;

    memset(state, 0, sizeof(data_00317490_dialog));
    state[0xcb2] = 0x18;
    state[0xcb3] = 23;
    state[0xcb4] = 0x18;
    state[0xcb5] = 0x18;
    memcpy(state + 0xc9a + 4, &x, 2);
    memcpy(state + 0xc9c + 4, &y, 2);
    memcpy(state + 0xcaa + 2, &value, 2);
    assert(SzDuel_DrawDialog_Pipe(1000) == 1110);
    assert(session_calls == 1 && cursor_calls == 1);
    assert(cursor_args[0] == 1010 && cursor_args[1] == 3 && cursor_args[2] == 40 && cursor_args[3] == -12);
    assert(free_calls == 1 && frees[0] == 0);
}

int main(void)
{
    check_hand_card();
    check_dialog();
    return 0;
}

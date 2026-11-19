/* Synthetic host tests for the card_save group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/card_save.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

unsigned char dotr_card_save_icon[0x100];
signed char dotr_card_save_board[0x100];

/* card_icon_tukuru's data (block-scope declarations in the function). */
struct DotrCardSaveStatus data_0034fb00_cardsave;
short data_0034fb36_cardsave;
unsigned char data_0034f720_cardsave[0x3c4];
char data_0034f7e0_cardsave[0x40];
unsigned short data_0034f726_cardsave;
unsigned int data_0034f72c_cardsave;
unsigned char data_0034f730_cardsave[0x40];
unsigned char data_0034f770_cardsave[0x30];
unsigned char data_0034f7a0_cardsave[0x30];
unsigned char data_0034f7d0_cardsave[0x10];
char data_0034f824_cardsave[0x40];
char data_0034f864_cardsave[0x40];
char data_0034f8a4_cardsave[0x40];
char *data_0029b06c_cardsave;
char *data_0029b070_cardsave;
unsigned char data_0029b100_cardsave[16] = "BASLUS";
char data_0030f321_cardsave[8];
char data_0030f32b_cardsave[8];
char data_0030f330_cardsave[8];
unsigned char bgColour_cardsave[0x40];
unsigned char lightdir_cardsave[0x30];
unsigned char lightcol_cardsave[0x30];
unsigned char ambient_cardsave[0x10];
char *iconName_cardsave[3];
unsigned int anOfs_ICON_cardsave[4];
unsigned int data_0029b054_cardsave[4];

/* TakuLoad_Name_Set's data. */
volatile int playerNameOffset_cardsave;
int data_002d19e0_cardsave;
unsigned short StringBlob_cardsave[64];

static int closed_fd = -1;

int func_00193ba8(int port, int slot, signed char *name, int mode) { return 0; }
int func_00193d18(int fd) { closed_fd = fd; return 0; }
int func_00194058(int fd, void *buffer, int size) { return 0; }
int func_00194248(int mode, int *command, int *result) { return 0; } /* not finished yet */
void *func_00198790(void *dst, const void *src, int n) { return memcpy(dst, src, n); }
void *func_00198948(void *dst, int c, unsigned int n) { return memset(dst, c, n); }
char *func_0019bdd8(char *dst, const char *src) { return strcpy(dst, src); }
int func_0019bef0(const char *s) { return (int)strlen(s); }

int main(void)
{
    DotrCardSaveIconState *s = (DotrCardSaveIconState *)&data_0034fb00_cardsave;
    unsigned short name;

    /* card_icon_tukuru, step 2 (close the icon.sys file), the card still busy: the handle of
     * the current port is closed and the step returns 0. */
    data_0034fb36_cardsave = 2;
    s->port = 1;
    s->handles[1] = 7;
    assert(card_icon_tukuru() == 0 && closed_fd == 7);

    /* TakuLoad_Name_Set, side 1: the twelve name halfwords from camp state +0x24 land at the
     * index in 0x2d19e0. */
    name = 0x1234;
    memcpy(dotr_card_save_board + 0x24 + 2 * 11, &name, sizeof name);
    data_002d19e0_cardsave = 4;
    TakuLoad_Name_Set(1);
    assert(StringBlob_cardsave[4 + 11] == 0x1234);
    return 0;
}

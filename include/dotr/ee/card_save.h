#ifndef DOTR_EE_CARD_SAVE_H
#define DOTR_EE_CARD_SAVE_H

/* Memory-card save and load helpers of the Taku card screens: the save icon builder
 * card_icon_tukuru and TakuLoad_Name_Set, which copies the twelve name halfwords of the camp state
 * into the loaded name table.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* The functions here read the icon model data at the literal address 0x1e301e0 and the camp state
 * at 0x299e10 as bytes past a signed-char symbol. The target build keeps the literal and the
 * symbol; the host build points them at test objects. Assumed: these meanings, from this use. */
#ifdef __MWERKS__
#define DOTR_CARD_SAVE_AT(a) (a)
extern signed char data_00299e10_cardsave;
#else
#include <stdint.h>
extern unsigned char dotr_card_save_icon[0x100];
#define DOTR_CARD_SAVE_AT(a) ((intptr_t)dotr_card_save_icon + ((a) - 0x1E301E0))
extern signed char dotr_card_save_board[0x100];
#define data_00299e10_cardsave (dotr_card_save_board[0])
#endif

/* The memory-card status block at 0x34fb00 and card_icon_tukuru's view of the same block: request
 * result, sync flag, port and slot, the command and result words libmc returns, the screen state,
 * phase and step, a retry count, the icon file index and one file handle per port. Assumed: names
 * from use. The host build adds trailing bytes so that the second view stays inside the object. */
struct DotrCardSaveStatus {
    int result;
    unsigned char pad04[0x1C];
    int command;
    int status;
    unsigned char pad28[8];
    short field30;
    unsigned char pad32[2];
    short field34;
    unsigned char pad36[0xA];
    short field40;
#ifndef __MWERKS__
    unsigned char host_pad[0x20];
#endif
};
typedef struct {
    int ret;
    int sync;
    int port;
    int slot;
    unsigned char pad10[0x10];
    int command;
    int result;
    unsigned char pad28[8];
    short state;
    unsigned char pad32[2];
    short phase;
    short sub;
    unsigned char pad38[8];
    short retries;
    short index;
    unsigned char pad44[4];
    int handles[2];
} DotrCardSaveIconState;

/* Callees: library-block functions (libmc open/write/close/sync, memcpy, memset, strcpy, strlen as
 * they are used here). Assumed: those meanings. */
extern int func_00193ba8(int port, int slot, signed char *name, int mode);
extern int func_00193d18(int fd);
extern int func_00194058(int fd, void *buffer, int size);
extern int func_00194248(int mode, int *command, int *result);
extern void *func_00198790(void *dst, const void *src, int n);
extern void *func_00198948(void *dst, int c, unsigned int n);
extern char *func_0019bdd8(char *dst, const char *src);
extern int func_0019bef0(const char *s);

/* 0x23e7a0: one step of writing the save icon files (icon.sys, then the three icon files) by
 * the step halfword at 0x34fb36; returns 1 when all three are written, 0xA on a card error,
 * 0 otherwise. */
int card_icon_tukuru(void);

/* 0x23fbe0: copies the twelve name halfwords at camp state +0x24 into the name table at
 * 0x2d49d4, at the index in 0x2d19d8 for side 0 (written from +4 on, with the first halfword
 * also stored at +0) or at the index in 0x2d19e0 otherwise (written from +0). */
void TakuLoad_Name_Set(int side);

#endif

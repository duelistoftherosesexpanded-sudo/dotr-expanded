#ifndef DOTR_EE_LINK_STEP_H
#define DOTR_EE_LINK_STEP_H

/* Memory-card transfer step routines on the link state at 0x34fb00 (struct DotrMiscLink),
 * called by the small_misc dispatchers (0x23cc50 family).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */
#include "dotr/ee/small_misc.h"

/* 0x194380 is SDK library code; two ints, three int
 * pointers and an int result are assumed from the call in 0x23fce0. The
 * argument of 0x23fce0 is set up by its caller 0x23cc50 and never read. */
extern int func_00194380(int a, int b, int *c, int *d, int *e);
int TakuCustom_card_kansi(int mode);
/* 0x23f130: the same routine without the +8 test in the 1 and 3 cases. */
int TakuLoad_card_kansi(int mode);

/* 0x23f340: two-stage request on the halfword at +0x34. 0x194758 and 0x194578
 * are in the SDK block; their argument lists are assumed from the two calls
 * (the fourth argument of 0x194578 converts from the int at +0x60 as unsigned).
 * 0x29b100 and 0x29b1be are passed by address only; char data is assumed. */
extern int func_00194758(int a, int b, char *c, unsigned char *d);
extern int func_00194578(int a, int b, char *c, unsigned d, int e, unsigned char *f);
extern char data_0029b100[];
extern char data_0029b1be[];
int TakuLoad_card_filename_check(void);
/* 0x23c170: reset. 0x34fae8 / 0x34faec receive the two SzData_GetData results
 * and 0x34faf0 a byte 1; pointer and byte types follow the stores only.
 * 0x195108 is in the SDK block; (address, length) is assumed from the call. */
extern struct DotrMiscLargeEntry *p_kaban_data;
extern struct DotrMiscLargeEntry *data_0034faec;
struct DotrLinkHead {
    short unknown_00;
    short unknown_02;
};
/* 0x23c170 stores a byte 1 here; 0x23f5a0 copies four bytes as two halfwords
 * (struct assignment); 0x23de90 copies them back. MATCHING DEVICE: 0x23de90
 * needs the struct type on the object itself and 0x23c170 a byte object for
 * its direct store; the union gives both without a second symbol. */
union DotrLinkHeadStore {
    unsigned char first_byte;
    struct DotrLinkHead head;
};
extern union DotrLinkHeadStore Taku_card_info;
extern void func_00195108(void *a, int b);
int TakuCard_inti(int mode);
/* 0x23d170: the 0x23f340 request with other state values and results; its
 * argument is read only when stage 1 succeeds. */
int TakuSave_card_filename_check(int mode);
/* 0x23ce40: the 0x23fce0 poll with state 14, an argument that is read (1
 * keeps the state on an unlisted status word and restarts from state 3) and
 * a final mapping of the state at +0x30 to a result. */
int TakuSave_card_kansi(int mode);
/* 0x23f5a0: four-stage transfer on the halfword at +0x34. The blocks it
 * reads are 0x100f0 bytes each at the literal address 0x1e10000 (`lui at`
 * plus index, so the target build uses an integer constant; the host build
 * points the name at an array the test defines): two halfwords, a 65440-byte
 * block and 83 words, copied to 0x34faf0, through 0x34fae8 and to 0x299e10.
 * Member types follow the copy widths only. 0x299e10 has other views
 * (movie_operations.h, glide_steps.h, sheet_marks.h); this one is 332 bytes
 * of words. 0x193ba8, 0x193dd0, 0x193f38 and 0x193d18 are in the SDK block;
 * their argument lists are assumed from the calls. 0x23fbe0's int argument
 * and missing result are assumed from this call. */
struct DotrLinkTail {
    int unknown_words[83];
};
struct DotrLinkImage {
    struct DotrLinkHead head;
    struct DotrMiscLargeEntry body;
    struct DotrLinkTail tail;
};
#ifdef __MWERKS__
#define DOTR_LINK_IMAGES ((struct DotrLinkImage *)0x1e10000)
#else
extern struct DotrLinkImage dotr_link_images_01e10000[];
#define DOTR_LINK_IMAGES dotr_link_images_01e10000
#endif
extern struct DotrLinkTail data_00299e10_link;
extern int func_00193ba8(int a, int b, char *c);
extern int func_00193dd0(int a, int b, int c, int d);
extern int func_00193f38(int a, struct DotrLinkImage *b, int c);
extern int func_00193d18(int a);
extern void TakuLoad_Name_Set(int a);
int TakuLoad_card_file_open(void);
/* 0x23d400: the 0x23f5a0 four stages with failure state 14 (11 when the
 * retry count reaches 3) and no copies: the last stage stores state 13 and
 * returns 1. No argument is read; the int result follows the returns. */
int TakuSave_card_file_open(void);
/* 0x2406c0: the 0x23ce40 poll without an argument: an unlisted status word
 * always sets state 14, state 0 with third word 1 flips the word at +8 (the
 * halfwords at +0x44 / +0x46 are indexed by it) and the final mapping gives 9
 * for state 0. No argument is read; the int result follows the returns. */
int TakuTrade_Save_card_kansi(void);
/* 0x23ff30: the 0x23f5a0 four stages, except that the last stage's copies go
 * through 0x34fae8 when the word at +8 is 0 (result 0x10) and through
 * 0x34faec otherwise (result 0x100); on success 0x23fbe0 gets the word at +8,
 * the halfword at +0x44 indexed by it becomes 1 and the word becomes 1.
 * No argument is read; the int result follows the returns. */
int TakuCustom_card_file_open(void);
/* 0x23de90: five-stage transfer in the other direction on the halfword at
 * +0x34: stage 2 copies through 0x34fae8, 0x34faf0 and 0x299e10 into the
 * block at 0x1e10000, calls 0x241190 on it (result unused) and sends it with
 * 0x194058. 0x194058 and 0x194a98 are in the SDK block; their argument lists
 * are assumed from the calls, as is the fourth 0x193dd0 argument 2 (the
 * register still holds the stage constant). */
extern int func_00194058(int a, struct DotrLinkImage *b, int c);
extern int func_00194a98(int a);
int TakuSave_card_file_kakikomi(void);
/* 0x2409e0: the 0x23de90 stages run once per slot: a finished stage flips the
 * word at +8 (0 -> 1 keeps the stage, 1 -> 0 moves on), stage 2 copies only
 * the body, taken from the pointer at 0x34fae8 indexed by that word (0x34faec
 * is the second element; the host build selects the two declared objects),
 * and stage 4 leaves 5 in the halfword at +0x30 and returns 0x100 only after
 * the second slot. No argument is read; the int result follows the returns. */
#ifdef __MWERKS__
#define DOTR_LINK_BODY(index) ((&p_kaban_data)[index])
#else
#define DOTR_LINK_BODY(index) ((index) == 0 ? p_kaban_data : data_0034faec)
#endif
int TakuTrade_Save_card_file_kakikomi_Ver2(void);
/* 0x23d920: five-stage routine on the halfword at +0x34 with the 0x23d400
 * failure states: 0x193ce0 (reply 11), 0x194758 (reply 12), card_icon_tukuru (1
 * advances, 10 is returned), 0x193ba8 (reply 2, word stored by the index) and
 * 0x193d18 (reply 3; word 0 leaves state 5 and returns 9). 0x193ce0 is in the
 * SDK block and 0x23e7a0 is card_icon_tukuru (card_save); their argument lists
 * here are assumed from the calls. This call of 0x193ba8 loads a fourth argument 512,
 * which the three-argument declaration above (0x23d400, 0x23de90) cannot
 * express and a cast turns into an indirect call: func_00193ba8_mode is a
 * second view of the same address. No argument is read; the int result
 * follows the returns. */
extern int func_00193ce0(int a, int b, char *c);
extern int card_icon_tukuru(void);
extern int func_00193ba8_mode(int a, int b, char *c, int d);
int TakuSave_make_file(void);
#endif

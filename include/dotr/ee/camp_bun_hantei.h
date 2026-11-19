#ifndef DOTR_EE_CAMP_BUN_HANTEI_H
#define DOTR_EE_CAMP_BUN_HANTEI_H

/* Campaign-map judge Taku_Camp_BUN_HANTEI (0x22d140): picks the message number, count and panel
 * mode for the campaign's current event from the camp state at 0x299e10 and the literal-addressed
 * block at 0x1ed5378.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* The camp state at 0x299e10 as this function reads it: a signed byte at +0 (the side, as in
 * camp_teki_window.h), the state byte of location 11 at +0xe, the 16-bit counter at +0xc2 (`ticks`
 * in camp_entry.h, compared with 0xc8, 0x12c, 0x190 and event values 0x2724..0x2f7c) and the
 * selected location at +0xc6. The function reads +0 as a signed char object, the other fields
 * through a signed char pointer to it, and +0xc2 once more under its own symbol (declared inside
 * the function). Assumed: the field meanings. The host build gives the byte object the bytes behind
 * it (an array the test defines) so that the pointer reads stay inside one object; its test keeps
 * CutSceneNumber_bunhantei equal to the counter at +0xc2. */
#ifdef __MWERKS__
extern signed char data_00299e10_bunhantei;
#else
extern signed char dotr_bun_camp_00299e10[0xc8];
#define data_00299e10_bunhantei (dotr_bun_camp_00299e10[0])
#endif

/* The block at 0x1ed5378 is addressed with literal addresses (`lui` per access), so the
 * target build uses integer constants (MATCHING DEVICE: the macros expand to `*(type *)address`
 * expressions on the literal addresses); the host build points the same names at a host array
 * the test defines. Assumed meanings: MSG the first message number, COUNT the number of
 * messages, PAGE the current page (added to the base), MODE the panel mode (0..3), NAME0/NAME1,
 * SIDE0/SIDE1 and LOOK0/LOOK1 the two opponent panels' name number, side and look byte (the
 * names camp_dare.h gives the same addresses), FLAG0/FLAG1 their show flags, BYTE_53C2 a byte
 * tested once. */
#ifdef __MWERKS__
#define DOTR_BUN_MSG (*(short *)0x01ED5378)
#define DOTR_BUN_COUNT (*(signed char *)0x01ED537C)
#define DOTR_BUN_PAGE (*(signed char *)0x01ED537D)
#define DOTR_BUN_MODE (*(signed char *)0x01ED537E)
#define DOTR_BUN_FLAG0 (*(signed char *)0x01ED53B4)
#define DOTR_BUN_FLAG1 (*(signed char *)0x01ED53B5)
#define DOTR_BUN_NAME0 (*(short *)0x01ED53BA)
#define DOTR_BUN_NAME1 (*(short *)0x01ED53BC)
#define DOTR_BUN_SIDE0 (*(signed char *)0x01ED53BE)
#define DOTR_BUN_SIDE1 (*(signed char *)0x01ED53BF)
#define DOTR_BUN_BYTE_53C2 (*(signed char *)0x01ED53C2)
#define DOTR_BUN_LOOK1 (*(signed char *)0x01ED5469)
#define DOTR_BUN_LOOK0 (*(signed char *)0x01ED546A)
#else
/* Host stand-in for 0x1ed5378..0x1ed546a (0xf4 bytes), indexed by offset from 0x1ed5378. */
extern short dotr_bun_work_01ed5378[0x7a];
#define DOTR_BUN_AT(type, address) (*(type *)((char *)dotr_bun_work_01ed5378 + ((address) - 0x01ED5378)))
#define DOTR_BUN_MSG DOTR_BUN_AT(short, 0x01ED5378)
#define DOTR_BUN_COUNT DOTR_BUN_AT(signed char, 0x01ED537C)
#define DOTR_BUN_PAGE DOTR_BUN_AT(signed char, 0x01ED537D)
#define DOTR_BUN_MODE DOTR_BUN_AT(signed char, 0x01ED537E)
#define DOTR_BUN_FLAG0 DOTR_BUN_AT(signed char, 0x01ED53B4)
#define DOTR_BUN_FLAG1 DOTR_BUN_AT(signed char, 0x01ED53B5)
#define DOTR_BUN_NAME0 DOTR_BUN_AT(short, 0x01ED53BA)
#define DOTR_BUN_NAME1 DOTR_BUN_AT(short, 0x01ED53BC)
#define DOTR_BUN_SIDE0 DOTR_BUN_AT(signed char, 0x01ED53BE)
#define DOTR_BUN_SIDE1 DOTR_BUN_AT(signed char, 0x01ED53BF)
#define DOTR_BUN_BYTE_53C2 DOTR_BUN_AT(signed char, 0x01ED53C2)
#define DOTR_BUN_LOOK1 DOTR_BUN_AT(signed char, 0x01ED5469)
#define DOTR_BUN_LOOK0 DOTR_BUN_AT(signed char, 0x01ED546A)
#endif

/* 0x22d140 Taku_Camp_BUN_HANTEI (name from the Japanese build's symbols). Its return value
 * is a local that is never set (`ret` is returned uninitialised): callers get
 * whatever v0 holds. */
int Taku_Camp_BUN_HANTEI(void);

#endif

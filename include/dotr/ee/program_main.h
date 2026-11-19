#ifndef DOTR_EE_PROGRAM_MAIN_H
#define DOTR_EE_PROGRAM_MAIN_H

/* The program entry `main` (0x1000d0), called by the SDK start-up code at 0x100008 (not in
 * scope). The prototypes are assumed, not recovered declarations. Evidence: docs/modules/main.md. */

typedef signed int s32;

/* Callees. ASSUMED: argument counts and types as main passes them; results unused except
 * Main_Init's. func_0019b0f8 lies in the SDK library block (0x17b48c-0x1a6c80), unnamed here;
 * main passes it two string addresses and 1. */
void SzMem_Init(void);
void func_0019b0f8(s32 arg0, s32 arg1, s32 arg2);
s32 Main_Init(void);
void Main_Loop(void);
void Main_Term(void);

/* Strings at 0x2b8f00-0x2b8f32 (.data/.rodata), passed by address. ASSUMED: char arrays. */
extern char data_002b8f00[], data_002b8f05[], data_002b8f07[], data_002b8f0d[], data_002b8f13[], data_002b8f18[];
extern char data_002b8f1f[], data_002b8f27[], data_002b8f2c[], data_002b8f32[];

/* The stack top main installs (0x912000), used only by its inline block. */
#ifdef __MWERKS__
extern unsigned char data_00912000[];
#endif

s32 main(void);

#endif

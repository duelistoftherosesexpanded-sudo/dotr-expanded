#ifndef DOTR_EE_MOVIE_VBLANK_H
#define DOTR_EE_MOVIE_VBLANK_H

/* The movie player's interrupt handlers: vblankHandler, which flips the display buffer from the
 * GS CSR field bit and kicks the next image transfer from the movie read buffer, and
 * handler_endimage, which releases a sent image's buffer element.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* The handler reads the GS CSR register at the literal address 0x12001000 and indexes past the
 * word at 0x350340 ((&isCountVblank_word)[6] is the word at 0x350358, the current field). The target
 * build keeps the literal and the separate symbol; the host build points both at test objects (a
 * host array for the words from 0x350340 on). Assumed: these meanings, from this use. */
#ifdef __MWERKS__
typedef int DotrVblankWord;
#define DOTR_VBLANK_CSR 0x12001000
extern int isCountVblank_vblank;
#else
#include <stdint.h>
typedef intptr_t DotrVblankWord;
extern unsigned long long dotr_movie_vblank_csr;
#define DOTR_VBLANK_CSR ((DotrVblankWord)&dotr_movie_vblank_csr)
extern int dotr_movie_vblank_state[8];
#define isCountVblank_vblank (dotr_movie_vblank_state[0])
#endif

/* Words and buffers the handler uses: 0x3505f8 a count of
 * frames with no image ready, 0x350350 set when the second buffer has been sent, 0x3503c0 the
 * image state handed to 0x185480, 0x350600 the read buffer whose tag voBufGetTag returns. The
 * others are declared inside the function. */
extern int frd_vblank;
extern int isFrameEnd_vblank;
extern unsigned char data_003503c0_vblank[];
extern unsigned char data_00350600_vblank[];

/* Callees; all but voBufGetTag
 * (defined in small_misc) are library-block functions. Assumed: meanings. */
extern int func_00186120(int channel);
extern int func_00185578(int mode, int timeout);
extern int voBufGetTag(void *buffer);
extern void func_00186060(unsigned char *env, short a1, short a2, short a3);
extern int func_00185480(unsigned char *state, int frame);
extern void func_00186470(void *channel, void *tag);

/* 0x242190: the vblank handler; returns 0 and ends with sync, ei. */
int vblankHandler(int val);

/* voBufDecCount (0x245960, defined in small_misc); it is called with the read buffer at
 * 0x350600. */
extern void voBufDecCount(void *arg0);

/* 0x2423a0: the DMA end-of-image handler; when the second buffer was sent (0x350350 set) it
 * releases one read-buffer element and clears the flag; returns 0 and ends with sync, ei. */
int handler_endimage(void);

#endif

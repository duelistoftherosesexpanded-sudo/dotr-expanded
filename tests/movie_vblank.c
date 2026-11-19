/* Synthetic host tests for the movie_vblank group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
/* The sync, ei block is left out of the host build; it is verified only by make match. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/movie_vblank.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

unsigned long long dotr_movie_vblank_csr;
int dotr_movie_vblank_state[8];
int frd_vblank;
int isFrameEnd_vblank;
unsigned char data_003503c0_vblank[16];
unsigned char data_00350600_vblank[16];
volatile int vblankCount_vblank;
int data_00350360_vblank;
unsigned char data_00350420_vblank[16];
unsigned char data_00350510_vblank[16];
static int sends;

int func_00186120(int channel) { return 0; }
int func_00185578(int mode, int timeout) { return 0; }
int voBufGetTag(void *buffer) { return 0; } /* no image ready */
void func_00186060(unsigned char *env, short a1, short a2, short a3) { sends++; }
int func_00185480(unsigned char *state, int frame) { return 0; }
void func_00186470(void *channel, void *tag) { sends++; }
static void *released;
void voBufDecCount(void *arg0) { released = arg0; }

int main(void)
{
    /* Playing (word 0x350340 set), the path wait idle, no image ready: the handler counts the
     * missed frame, stores the CSR field bit and sends nothing. */
    dotr_movie_vblank_state[0] = 1;
    dotr_movie_vblank_csr = 1 << 13;
    assert(vblankHandler(0) == 0);
    assert(frd_vblank == 1 && dotr_movie_vblank_state[6] == 1 && sends == 0);

    /* handler_endimage: with the second buffer sent, it releases one element of the read
     * buffer and clears the flag. */
    isFrameEnd_vblank = 1;
    assert(handler_endimage() == 0);
    assert(released == (void *)data_00350600_vblank && isFrameEnd_vblank == 0);
    return 0;
}

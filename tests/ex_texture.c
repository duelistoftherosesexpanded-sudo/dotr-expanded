/* Synthetic host tests for the ex_texture group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/ex_texture.h"

/* Header (two words) and twelve packet words in one buffer. */
static union DotrExTexWord buffer[14];
#define FILL(i) ((long)(0x1111111111111111UL * (unsigned long)(i)))

static struct DotrExTexHdr *header(int count, int flags)
{
    struct DotrExTexHdr *h = (struct DotrExTexHdr *)buffer;
    int i;

    for (i = 0; i < 14; i++) {
        buffer[i].l = FILL(i + 1);
    }
    h->count = (unsigned short)count;
    h->flags = (unsigned short)flags;
    return h;
}

static void check_assign(void)
{
    struct DotrExTexTex tex;
    struct DotrExTexHdr *h;
    int a, i;

    memset(&tex, 0, sizeof tex);
    tex.w48 = 0x0123456789abcdefL;
    tex.w64 = 0x7edcba9876543200L;   /* bits 5-8 clear */

    /* Flag 0x20 clear, or no texture: 0 and nothing written. */
    h = header(3, 0xffdf);
    assert(ExAssignTexture(h, &tex, 5) == 0);
    for (i = 2; i < 14; i++) {
        assert(buffer[i].l == FILL(i + 1));
    }
    h = header(3, 0x20);
    assert(ExAssignTexture(h, NULL, 5) == 0 && buffer[6].l == FILL(7));

    for (a = 0; a < 8; a++) {
        h = header(3, 0x20);
        assert(ExAssignTexture(h, &tex, a) == 1);
        /* Words count + 1 and count + 2 after the header (buffer index 2 + 4, 2 + 5). */
        assert(buffer[5].l == FILL(6) && buffer[6].l == tex.w48);
        assert(buffer[7].l == (tex.w64 | ((long)a << 6) | ((long)(a & 1) << 5)));
        assert(buffer[8].l == FILL(9));
    }
    /* Only the low three bits of the value are kept. */
    h = header(0, 0x21);
    assert(ExAssignTexture(h, &tex, 13) == 1);
    assert(buffer[3].l == tex.w48 && buffer[4].l == (tex.w64 | (5L << 6) | (1L << 5)));
}

int main(void)
{
    assert(offsetof(struct DotrExTexHdr, w) == 16 && sizeof(union DotrExTexWord) == 8);
    check_assign();
    puts("ex_texture: ok");
    return 0;
}

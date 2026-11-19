/* Synthetic host tests for the frame_texture group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include "dotr/ee/frame_texture.h"

/* Stubs (assumed semantics: both getters fill their out-parameters). */
static int fmt_arg, fmt_z_seen, fmt_calls, fmt_base, fmt_format, size_w, size_h;
int RhBase_GetBufferFormatEx(int a, int *x, int *y, int *z)
{
    fmt_arg = a;
    fmt_z_seen = z != 0;
    fmt_calls++;
    *x = fmt_base;
    *y = fmt_format;
    return 0;
}
int RhBase_GetScreenSize(int *a, int *b)
{
    *a = size_w;
    *b = size_h;
    return 0;
}

/* ExFrameBuffer2Texture_Pipe (branch coverage: one assertion per branch outcome and per loop
 * boundary). The two records live in a 32-bit addressable page because the
 * function's signature passes their addresses as int. */
static void check_frame_texture(void)
{
    unsigned char *page = mmap(0, 4096, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    struct DotrDrawBits64A *a = (struct DotrDrawBits64A *)page;
    struct DotrDrawBits64B *b = (struct DotrDrawBits64B *)(page + 8);
    unsigned long pkt[13];
    unsigned long before;

    assert(page != MAP_FAILED);
    /* tbp > 0: both records and the packet */
    memset(page, 0xff, 16);
    memset(pkt, 0xee, sizeof pkt);
    fmt_base = 0x8c; fmt_format = 2; size_w = 640; size_h = 448; fmt_calls = 0;
    assert(ExFrameBuffer2Texture_Pipe((DotrDrawWord)pkt, 0x2a0, 1, (int)(intptr_t)a, (int)(intptr_t)b)
           == (DotrDrawWord)(pkt + 12));
    assert(fmt_calls == 1 && fmt_arg == 1 && fmt_z_seen == 0);
    assert(a->unknown_bits_0_13 == 0x2a0 && a->unknown_bits_14_19 == 10 && a->unknown_bits_20_25 == 2);
    assert(a->unknown_bits_26_29 == 10 && a->unknown_bits_30_33 == 9);
    assert(a->unknown_bit_34 == 0 && a->unknown_bits_35_36 == 0 && a->unknown_bits_37_50 == 0);
    assert(a->unknown_bits_51_54 == 0 && a->unknown_bit_55 == 0 && a->unknown_bits_56_60 == 0);
    assert(a->unknown_bits_61_63 == 1);
    assert(b->unknown_bit_0 == 0 && b->unknown_bits_2_4 == 0 && b->unknown_bit_5 == 1);
    assert(b->unknown_bits_6_8 == 1 && b->unknown_bit_9 == 0 && b->unknown_bits_19_20 == 0);
    assert(b->unknown_bits_32_43 == 0);
    /* untouched padding keeps the fill */
    assert(b->unknown_bit_1 == 1 && b->unknown_bits_10_18 == 0x1ff && b->unknown_bits_44_63 == 0xfffff);
    assert(pkt[0] == 0x10000005UL && pkt[1] == 0x5100000511000000UL);
    assert(pkt[2] == 0x1000000000008004UL && pkt[3] == 14);
    assert(pkt[4] == (0x8cUL | 10UL << 16 | 2UL << 24 | 0x2a0UL << 32 | 10UL << 48 | 2UL << 56));
    assert(pkt[5] == 0x50 && pkt[6] == 0 && pkt[7] == 0x51);
    assert(pkt[8] == (640UL | 448UL << 32) && pkt[9] == 0x52 && pkt[10] == 2 && pkt[11] == 0x53);
    assert(pkt[12] == 0xeeeeeeeeeeeeeeeeUL);

    /* tbp == 0: getter asked with 0, base used, no packet, cursor returned;
     * exact powers of two stop the exponent loops at the power */
    memset(page, 0, 16);
    memset(pkt, 0xee, sizeof pkt);
    size_w = 512; size_h = 256;
    assert(ExFrameBuffer2Texture_Pipe((DotrDrawWord)pkt, 0, 6, (int)(intptr_t)a, (int)(intptr_t)b)
           == (DotrDrawWord)pkt);
    assert(fmt_arg == 0 && a->unknown_bits_0_13 == 0x8c && a->unknown_bits_14_19 == 8);
    assert(a->unknown_bits_26_29 == 9 && a->unknown_bits_30_33 == 8);
    assert(b->unknown_bit_5 == 0 && b->unknown_bits_6_8 == 6);
    assert(pkt[0] == 0xeeeeeeeeeeeeeeeeUL);

    /* tbp < 0: getter asked with 1, base used, no packet; sizes of 1 and one
     * above a power of two */
    size_w = 1; size_h = 257;
    assert(ExFrameBuffer2Texture_Pipe((DotrDrawWord)pkt, -1, 0, (int)(intptr_t)a, (int)(intptr_t)b)
           == (DotrDrawWord)pkt);
    assert(fmt_arg == 1 && a->unknown_bits_0_13 == 0x8c);
    assert(a->unknown_bits_26_29 == 0 && a->unknown_bits_30_33 == 9);
    assert(pkt[0] == 0xeeeeeeeeeeeeeeeeUL);

    /* null records: nothing but the packet is written */
    memcpy(&before, page, 8);
    size_w = 640; size_h = 448;
    assert(ExFrameBuffer2Texture_Pipe((DotrDrawWord)pkt, 5, 1, 0, (int)(intptr_t)b) == (DotrDrawWord)(pkt + 12));
    assert(memcmp(&before, page, 8) == 0 && b->unknown_bit_5 == 1);
    memcpy(&before, page + 8, 8);
    assert(ExFrameBuffer2Texture_Pipe((DotrDrawWord)pkt, 5, 0, (int)(intptr_t)a, 0) == (DotrDrawWord)(pkt + 12));
    assert(memcmp(&before, page + 8, 8) == 0 && a->unknown_bits_0_13 == 5);
    munmap(page, 4096);
}

int main(void)
{
    check_frame_texture();
    puts("frame_texture: ok");
    return 0;
}

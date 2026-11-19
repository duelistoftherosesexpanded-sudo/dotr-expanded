/* Synthetic host tests for the packed_rebase group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/packed_rebase.h"

/* One host block: the header, then a list of 8 words, then four segments of 4 words each. */
static struct {
    DotrRebaseHeader header;
    DotrRebaseWord list[8];
    DotrRebaseWord seg[4][4];
} blk;

static void check_rebase(void)
{
    char *base = (char *)&blk.header;
    int i;

    memset(&blk, 0, sizeof(blk));
    blk.header.list = (int)((char *)blk.list - base);
    for (i = 0; i < 4; i++) {
        blk.header.segment[i + 1].offset = (int)((char *)blk.seg[i] - base);
    }
    blk.header.segment[0].offset = 0x7777; /* record 0 is never used */

    /* List tags 2..5 pick records 1..4; the target word's own tag picks the record again. */
    blk.list[0] = 0x20000000 | (0 * sizeof(DotrRebaseWord)); /* seg[0][0] holds tag 5 */
    blk.seg[0][0] = 0x50000000 | 0x10;
    blk.list[1] = 0x30000000 | (1 * sizeof(DotrRebaseWord)); /* seg[1][1] holds tag 4 */
    blk.seg[1][1] = 0x40000000 | 0x20;
    blk.list[2] = 0x40000000 | (2 * sizeof(DotrRebaseWord)); /* seg[2][2] holds tag 3 */
    blk.seg[2][2] = 0x30000000 | 0x30;
    blk.list[3] = 0x50000000 | (3 * sizeof(DotrRebaseWord)); /* seg[3][3] holds tag 2 */
    blk.seg[3][3] = 0x20000000 | 0x0ffffff0;
    /* Unknown list tag: entry skipped, nothing read through it. */
    blk.list[4] = 0x10000000 | 8;
    blk.list[5] = 0x60000000;
    /* Known list tag, unknown target tag: target left alone. */
    blk.list[6] = 0x20000000 | (1 * sizeof(DotrRebaseWord));
    blk.seg[0][1] = 0x10000123;
    blk.list[7] = (DotrRebaseWord)-1;
    blk.seg[0][2] = 0x20000000 | 4; /* not listed: unchanged */

    assert(RhModel_BindAddress(&blk.header) == 1);
    assert(blk.seg[0][0] == (DotrRebaseWord)((char *)blk.seg[3] + 0x10));
    assert(blk.seg[1][1] == (DotrRebaseWord)((char *)blk.seg[2] + 0x20));
    assert(blk.seg[2][2] == (DotrRebaseWord)((char *)blk.seg[1] + 0x30));
    assert(blk.seg[3][3] == (DotrRebaseWord)((char *)blk.seg[0] + 0x0ffffff0));
    assert(blk.seg[0][1] == 0x10000123 && blk.seg[0][2] == (0x20000000 | 4));
    assert(blk.list[0] == (0x20000000 | (0 * sizeof(DotrRebaseWord))) && blk.list[7] == (DotrRebaseWord)-1);

    /* Empty list: returns 1 at once. */
    blk.list[0] = (DotrRebaseWord)-1;
    blk.seg[0][0] = 0x50000000;
    assert(RhModel_BindAddress(&blk.header) == 1 && blk.seg[0][0] == 0x50000000);
}

int main(void)
{
    check_rebase();
    return 0;
}

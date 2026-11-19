/* Synthetic host tests for the block_setup group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/block_setup.h"

struct DotrBlock *gSzView_hView_blocks[2];
static struct DotrBlock block;
static int calls[32][4], call_count;
static float pair_at_part_calls[2];

static void note(int id, int a, int b, int c)
{
    calls[call_count][0] = id; calls[call_count][1] = a; calls[call_count][2] = b; calls[call_count++][3] = c;
}
int SzView_SetStraightLightDirection(int index, int position, int part, struct DotrBlockVector4 *in)
{
    /* The pair at slot 9 is still unwritten while the parts are set. */
    pair_at_part_calls[0] = block.unknown_7e0[9].x;
    pair_at_part_calls[1] = block.unknown_830[9].y;
    assert(in == 0 && block.unknown_8a8 == 512);
    note(0x5e0, index, position, part);
    return 1;
}
int SzView_SetFogColor(int index, int position, struct DotrBlockQuad *in) { note(0x170, index, position, in != 0); return 1; }
int lSzView_CalcRel(int index) { note(0x0e0, index, 0, 0); return 1; }
int lSzView_CalcLook(int index) { note(0xf80, index, 0, 0); return 1; }

int main(void)
{
    struct DotrBlockDelta *d = (struct DotrBlockDelta *)&block;
    float *f = (float *)block.unknown_048;
    struct DotrBlockElement *e = &block.modelViewData[9];
    int *angles;
    int i, j;

    /* Index below 0 and above 0, and an empty slot: 0 and nothing is called. */
    gSzView_hView_blocks[0] = &block;
    gSzView_hView_blocks[1] = &block;
    assert(lSzView_InitVars(-1) == 0 && lSzView_InitVars(1) == 0 && call_count == 0);
    gSzView_hView_blocks[0] = 0;
    assert(lSzView_InitVars(0) == 0 && call_count == 0);

    /* Slot 0 filled: every stored value, then the calls in order. */
    gSzView_hView_blocks[0] = &block;
    memset(&block, 0x55, sizeof block);
    assert(lSzView_InitVars(0) == 1);
    assert(d->sourcePoint.x == 0.0f && d->sourcePoint.y == 50.0f && d->sourcePoint.z == 300.0f && d->sourcePoint.unknown_w == 1.0f);
    assert(d->cameraPoint.x == 0.0f && d->cameraPoint.y == 50.0f && d->cameraPoint.z == 0.0f && d->cameraPoint.unknown_w == 1.0f);
    assert(block.unknown_040 == 0 && block.screenDistance == 800.0f);
    assert(f[0] == 1.0f && f[1] == 0.94f && f[2] == 1.0f && f[3] == 4096.0f);
    assert(e->unknown_00[0].unknown_00[0].x == 1.0f && e->unknown_00[0].unknown_00[0].y == 0.5f);
    assert(e->unknown_00[0].unknown_00[0].z == 0.5f && e->unknown_00[0].unknown_00[0].unknown_w == 0.5f);
    for (i = 1; i < 3; i++) {
        assert(e->unknown_00[i].unknown_00[0].x == 1.0f && e->unknown_00[i].unknown_00[0].y == 0.0f);
        assert(e->unknown_00[i].unknown_00[0].z == 0.0f && e->unknown_00[i].unknown_00[0].unknown_w == 0.0f);
    }
    angles = (int *)&e->unknown_00[0].unknown_00[1];
    assert(angles[0] == -14336 && angles[1] == 2048 && angles[2] == 0 && angles[3] == 0);
    for (i = 1; i < 3; i++) {
        angles = (int *)&e->unknown_00[i].unknown_00[1];
        for (j = 0; j < 4; j++) assert(angles[j] == 0);
    }
    assert(block.unknown_8a8 == 512);
    assert(block.unknown_7e0[9].x == 2000.0f && block.unknown_7e0[9].y == 4000.0f);
    assert(block.unknown_830[9].x == 2000.0f && block.unknown_830[9].y == 4000.0f);
    assert(pair_at_part_calls[0] != 2000.0f && pair_at_part_calls[1] != 4000.0f);
    assert(call_count == 15);
    for (i = 0; i < 3; i++) {
        assert(calls[i][0] == 0x5e0 && calls[i][1] == 0 && calls[i][2] == 9 && calls[i][3] == i);
    }
    for (i = 0; i < 10; i++) {
        assert(calls[3 + i][0] == 0x170 && calls[3 + i][1] == 0 && calls[3 + i][2] == i && calls[3 + i][3] == 0);
    }
    assert(calls[13][0] == 0x0e0 && calls[13][1] == 0 && calls[14][0] == 0xf80 && calls[14][1] == 0);
    /* Untouched neighbours: element 8's last word and the third part of element 9's first triple. */
    assert(((unsigned char *)&block)[0x56c] == 0x55 && ((unsigned char *)&block)[0x590] == 0x55);
    return 0;
}

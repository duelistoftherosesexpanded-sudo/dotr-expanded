/* Synthetic host tests for the prim_sp1 group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/prim_sp1.h"

void *tex_data[8];
static int texture_words[2];
static int calls;
static void *seen_packet;
static DotrSp1Packet sorted;

unsigned char *ExGetMyOT(void)
{
    assert(calls++ == 0);
    return NULL;
}

void TAKU_SP_XYWH(void *packet, short x, short y, short width, short height, int z, int f)
{
    DotrSp1Packet *p = packet;
    DotrSp1Header *h = packet;

    assert(calls++ == 1);
    assert(x == 100 && y == 50 && width == 30 && height == -10 && z == 3 + 0xfffff8 && f == 0);
    assert(h->count == 5 && h->flags == 38 && h->pad == 0 && h->registers == 0x42421);
    assert(p->r == 1 && p->g == 2 && p->b == 3 && p->a == 4);
    assert(p->q == 1.0f);
    seen_packet = packet;
}

int ExAssignTexture(void *packet, void *texture, int a)
{
    assert(calls++ == 2);
    assert(packet == seen_packet && texture == &texture_words[1] && a == 0);
    assert(((DotrSp1Header *)packet)->flags == (38 | 0x800));
    return 0;
}

int ExAssignAlpha(void *packet, long long value)
{
    assert(calls++ == 3);
    assert(packet == seen_packet && value == 0x8000000044);
    return 0;
}

DotrSp1Word ExSortPrim_Pipe(DotrSp1Word cursor, DotrSp1Word packet, int a2, int a3)
{
    assert(calls++ == 4);
    assert(cursor == 7000 && (void *)packet == seen_packet && a2 == 0 && a3 == 0);
    memcpy(&sorted, (void *)packet, sizeof sorted);
    return cursor + 96;
}

/* Straight-line coverage: one call that checks every stored value, every forwarded
 * argument and the call order (the function returns nothing). */
static void check_sp1(void)
{
    DotrSp1Vertex *uv0 = (DotrSp1Vertex *)sorted.vertices;
    DotrSp1Vertex *uv1 = (DotrSp1Vertex *)(sorted.vertices + 0x10);

    tex_data[5] = &texture_words[1];
    Taku_Prim_Set_SP1(7000, 100, 50, 130, 40, 6, 9, 20, 12, 1, 2, 3, 4, 0, 5, 0, 3);
    assert(calls == 5);
    assert(uv0->u == 6 * 16 + 8 && uv0->v == 9 * 16 + 8);
    assert(uv1->u == (6 + 20) * 16 + 8 && uv1->v == (9 + 12) * 16 + 8);
    assert(sorted.a == 4);
}

int main(void)
{
    check_sp1();
    return 0;
}

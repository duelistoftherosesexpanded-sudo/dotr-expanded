/* Synthetic host tests for the object_frees group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/object_frees.h"

struct DotrFreeObject *gSzModel_ahModel_free[84];
struct DotrFreeEntry112 data_0031ad00_free[32];

static struct DotrFreeObject object;
static struct DotrFreeDesc desc;
/* head and list in one host block so that head + offset reaches the list */
static struct {
    struct DotrFreeHead head;
    int gap[2];
    struct DotrFreeList list;
    struct DotrFreeEntry more[2];
} block;
static void *freed[32];
static int free_n, slot_calls, slot_mask, block_calls, pool_calls, pool_arg;
static char tokens[32];

void SzMem_Free(void *pointer)
{
    assert(pointer != NULL && free_n < 32);
    freed[free_n++] = pointer;
}

int lSzModel_FreeMotion(int index, int slot)
{
    assert(index == 83 && slot == slot_calls);
    slot_calls++;
    slot_mask |= 1 << slot;
    return 1;
}

int SzView_FreeRequest(int a, int b)
{
    assert(a == 0 && b == 1);
    /* both words are already cleared when it is called */
    assert(object.OverlayM == 0 && object.OverlayB == 0);
    block_calls++;
    return 0;
}

int SzEffect_Destroy(int index)
{
    pool_calls++;
    pool_arg = index;
    return 0;
}

static void fill(int flags)
{
    int i;

    memset(&object, 0, sizeof(object));
    memset(&block, 0, sizeof(block));
    memset(&desc, 0, sizeof(desc));
    block.head.unknown_04 = flags;
    object.modelBaseData = &block.head;
    object.unknown_13c = &desc;
    object.mainEffectTexture = &tokens[0];
    for (i = 0; i < 8; i++) {
        object.effectTextureB[i] = &tokens[1 + i];
        object.AnimationDataPointers[i] = &tokens[9 + i];
    }
    object.OverlayM = 5;
    object.OverlayB = 6;
    object.pCameraMotion = &tokens[17];
    object.pCameraMotion2 = &tokens[18];
    object.ptrBlock = &tokens[19];
    object.effectCallback = -1;
    gSzModel_ahModel_free[83] = &object;
    for (i = 0; i < 32; i++) {
        data_0031ad00_free[i].unknown_68_bits_0_26 = 83;
        data_0031ad00_free[i].unknown_68_bits_27_31 = 5;
    }
    data_0031ad00_free[2].unknown_68_bits_0_26 = 82;
    data_0031ad00_free[3].unknown_68_bits_0_26 = -2;
    free_n = slot_calls = slot_mask = block_calls = pool_calls = 0;
}

static void check_tail(void)
{
    int i;

    assert(object.modelBaseData == NULL && object.OverlayM == 0 && object.OverlayB == 0);
    assert(object.mainEffectTexture == NULL && object.pCameraMotion == NULL && object.pCameraMotion2 == NULL && object.ptrBlock == NULL);
    for (i = 0; i < 8; i++) {
        assert(object.effectTextureB[i] == NULL);
    }
    assert(block_calls == 1);
    /* entry 0 is not visited; entries 1..31 that held the index become -1 */
    assert(data_0031ad00_free[0].unknown_68_bits_0_26 == 83);
    assert(data_0031ad00_free[1].unknown_68_bits_0_26 == -1 && data_0031ad00_free[31].unknown_68_bits_0_26 == -1);
    assert(data_0031ad00_free[2].unknown_68_bits_0_26 == 82 && data_0031ad00_free[3].unknown_68_bits_0_26 == -2);
    for (i = 0; i < 32; i++) {
        assert(data_0031ad00_free[i].unknown_68_bits_27_31 == 5);
    }
}

/* lSzModel_FreeModel (branch coverage: both index bounds, null entry, null head,
 * bit 3, head word zero and non-zero, list flag and counts, null and
 * non-null words, each of the three flag bits, +0x1ec sign, bit 0). */
static void check_free(void)
{
    int i;

    memset(gSzModel_ahModel_free, 0, sizeof(gSzModel_ahModel_free));
    fill(0);
    assert(lSzModel_FreeModel(-1) == 0 && lSzModel_FreeModel(84) == 0);
    assert(lSzModel_FreeModel(82) == 0);
    assert(free_n == 0 && block_calls == 0 && object.modelBaseData == &block.head);
    /* null head answers 1 and touches nothing */
    object.modelBaseData = NULL;
    assert(lSzModel_FreeModel(83) == 1 && block_calls == 0 && object.OverlayM == 5);
    assert(data_0031ad00_free[1].unknown_68_bits_0_26 == 83);
    /* bit 3 answers 0 and touches nothing */
    fill(0);
    object.drawFlag.unknown_bit_3 = -1;
    assert(lSzModel_FreeModel(83) == 0 && block_calls == 0 && free_n == 0 && object.modelBaseData == &block.head);

    /* head word with all three flags: everything cleared, nothing released */
    fill(0x380000);
    assert(lSzModel_FreeModel(83) == 1 && free_n == 0 && slot_calls == 0 && pool_calls == 0);
    check_tail();
    for (i = 0; i < 8; i++) {
        assert(object.AnimationDataPointers[i] == NULL);
    }
    /* head word non-zero with one flag: the other two words are released */
    fill(0x80000);
    object.effectCallback = 0;
    assert(lSzModel_FreeModel(83) == 1 && free_n == 2 && freed[0] == &tokens[18] && freed[1] == &tokens[19]);
    assert(pool_calls == 1 && pool_arg == 0);
    check_tail();
    fill(0x100000);
    assert(lSzModel_FreeModel(83) == 1 && free_n == 2 && freed[0] == &tokens[17] && freed[1] == &tokens[19]);
    check_tail();
    fill(0x200000);
    object.pCameraMotion = NULL;
    assert(lSzModel_FreeModel(83) == 1 && free_n == 1 && freed[0] == &tokens[18]);
    check_tail();
    /* a non-zero head word without the flags: null words are skipped; bit 0 releases the head last */
    fill(1);
    object.pCameraMotion2 = NULL;
    object.ptrBlock = NULL;
    object.drawFlag.unknown_bit_0 = -1;
    assert(lSzModel_FreeModel(83) == 1 && free_n == 2 && freed[0] == &tokens[17] && freed[1] == (void *)&block.head);
    check_tail();

    /* head word 0, list flag clear: slots 0..7, +0x140 and +0x144[] released, +0x178[] left to 0x1f0c10 */
    fill(0);
    desc.unknown_28 = (int)offsetof(__typeof__(block), list);
    block.list.count = 2;
    block.list.entries[0].unknown_0 = &tokens[20];
    object.effectTextureB[3] = NULL;
    object.effectCallback = 7;
    assert(lSzModel_FreeModel(83) == 1 && slot_calls == 8 && slot_mask == 0xff);
    assert(free_n == 11 && freed[0] == &tokens[0] && freed[1] == &tokens[1] && freed[3] == &tokens[3] && freed[4] == &tokens[5]);
    assert(freed[7] == &tokens[8] && freed[8] == &tokens[17] && freed[9] == &tokens[18] && freed[10] == &tokens[19]);
    assert(object.AnimationDataPointers[0] == &tokens[9] && pool_calls == 1 && pool_arg == 7);
    check_tail();
    /* list flag set: first and third word of each of the count entries, nulls skipped */
    fill(0);
    desc.unknown_28 = (int)offsetof(__typeof__(block), list);
    desc.unknown_2c = 1;
    block.list.count = 3;
    block.list.entries[0].unknown_0 = &tokens[20];
    block.list.entries[0].unknown_8 = &tokens[21];
    block.list.entries[1].unknown_8 = &tokens[22];
    block.list.entries[2].unknown_0 = &tokens[23];
    object.mainEffectTexture = NULL;
    assert(lSzModel_FreeModel(83) == 1 && slot_calls == 8);
    assert(free_n == 15 && freed[0] == &tokens[20] && freed[1] == &tokens[21] && freed[2] == &tokens[22] && freed[3] == &tokens[23]);
    assert(freed[4] == &tokens[1] && freed[11] == &tokens[8] && freed[12] == &tokens[17]);
    check_tail();
    /* count 0 releases no list word */
    fill(0);
    desc.unknown_28 = (int)offsetof(__typeof__(block), list);
    desc.unknown_2c = 1;
    block.list.entries[0].unknown_0 = &tokens[20];
    assert(lSzModel_FreeModel(83) == 1 && free_n == 12 && freed[0] == &tokens[0]);
    check_tail();
}

static int draw_calls, draw_args[9];
static void (*draw_callback)(void);
static void *draw_pointer;

void lSzModel_Draw_DefFunc(void)
{
}

int SzModel_DrawWithFunc_Pipe(int cursor, int index, int a2, void *a3, int a4, int a5, int a6, void (*callback)(void), int zero)
{
    draw_calls++;
    draw_args[0] = cursor; draw_args[1] = index; draw_args[2] = a2; draw_pointer = a3;
    draw_args[4] = a4; draw_args[5] = a5; draw_args[6] = a6; draw_args[8] = zero;
    draw_callback = callback;
    return cursor + 1000;
}

/* SzModel_Draw_Pipe: each early answer, each listed kind with and without the
 * header word, and the kinds on both sides of 4..6. */
static void check_draw_forward(void)
{
    static struct DotrFreeObject o;
    static struct DotrFreeDesc header;
    static struct DotrFreeHead link;
    int kind, i;

    memset(gSzModel_ahModel_free, 0, sizeof(gSzModel_ahModel_free));
    memset(&o, 0, sizeof(o));
    draw_calls = 0;
    assert(SzModel_Draw_Pipe(50, -1, 1, &o, 3, 4, 5) == 50 && SzModel_Draw_Pipe(51, 84, 1, &o, 3, 4, 5) == 51);
    assert(SzModel_Draw_Pipe(52, 83, 1, &o, 3, 4, 5) == 52);          /* no object */
    gSzModel_ahModel_free[83] = &o;
    gSzModel_ahModel_free[0] = &o;
    o.unknown_13c = &header;
    assert(SzModel_Draw_Pipe(53, 83, 1, &o, 3, 4, 5) == 53);          /* +0x138 is 0 */
    o.modelBaseData = &link;
    o.unknown_13c = 0;
    assert(SzModel_Draw_Pipe(54, 83, 1, &o, 3, 4, 5) == 54);          /* +0x13c is 0 */
    assert(draw_calls == 0);
    o.unknown_13c = &header;
    for (kind = 3; kind <= 7; kind++) {
        for (i = 0; i < 2; i++) {
            o.type = (signed char)kind;
            header.unknown_1c = i;
            draw_calls = 0;
            draw_args[8] = -1;
            assert(SzModel_Draw_Pipe(60, kind == 3 ? 0 : 83, 11, &header, 13, 14, 15) == 1060 && draw_calls == 1);
            assert(draw_args[0] == 60 && draw_args[1] == (kind == 3 ? 0 : 83) && draw_args[2] == 11);
            assert(draw_pointer == &header && draw_args[4] == 13 && draw_args[5] == 14 && draw_args[6] == 15);
            assert(draw_args[8] == 0);
            assert((draw_callback == lSzModel_Draw_DefFunc) == (kind >= 4 && kind <= 6 && i == 1));
            assert(draw_callback == lSzModel_Draw_DefFunc || draw_callback == 0);
        }
    }
}

int main(void)
{
    check_draw_forward();
    check_free();
    return 0;
}

/* Synthetic host tests for the cell_forwards group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cell_forwards.h"

int data_0031be98;
int data_0031be9c;
static struct DotrForwardItem item;
static struct DotrForwardItem *item_result;
static int word_arg, item_args[2], send_args[7], send_calls;
static int index_args[2], index_result, index_calls;
static int draw_args[6], draw_calls, object_args[6], object_calls;

int SzBattle_InqEntry(int index)
{
    word_arg = index;
    return 0x0203;
}
struct DotrForwardItem *func_001df540_forward(int a, int b)
{
    item_args[0] = a; item_args[1] = b;
    return item_result;
}
int SzDuel_GetPlayMotion(int a0, int a1, int a2, int a3, int a4, int a5, int a6)
{
    send_args[0] = a0; send_args[1] = a1; send_args[2] = a2; send_args[3] = a3;
    send_args[4] = a4; send_args[5] = a5; send_args[6] = a6;
    send_calls++;
    return 77;
}
int SzDuel_GetUnitModelID(int a, int b)
{
    index_args[0] = a; index_args[1] = b;
    index_calls++;
    return index_result;
}
void SzModel_ShotTextureWithFunc(int a, int b, int c, int d, int e, int f)
{
    draw_args[0] = a; draw_args[1] = b; draw_args[2] = c;
    draw_args[3] = d; draw_args[4] = e; draw_args[5] = f;
    draw_calls++;
}
int SzModel_ReqMotionBlur(int index, int value, int mode, struct DotrObjectQuad *a, struct DotrObjectQuad *b, int flag)
{
    object_args[0] = index; object_args[1] = value; object_args[2] = mode;
    object_args[3] = (int)(size_t)a; object_args[4] = (int)(size_t)b; object_args[5] = flag;
    object_calls++;
    return 55;
}

static void check_item_send(void)
{
    static const int bad[] = { -0x100, 0x0300, 0x020b };
    size_t i;

    assert(offsetof(struct DotrForwardItem, status) == 0x57);
    item.locCol = 0x91; item.locRow = 0x92;
    item.toMoveCol = 0x93; item.toMoveRow = 0x94;
    data_0031be9c = 0x020a; data_0031be98 = 0x0000;

    /* No item. */
    item_result = NULL;
    assert(SzEffect_GetPlayMotion(9) == -1 && send_calls == 0);
    assert(word_arg == 1 && item_args[0] == 2 && item_args[1] == 3);

    /* Each word out of range: x below 0, x above 2, y above 10. */
    item_result = &item;
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        data_0031be9c = bad[i]; data_0031be98 = 0;
        assert(SzEffect_GetPlayMotion(9) == -1);
        data_0031be9c = 0; data_0031be98 = bad[i];
        assert(SzEffect_GetPlayMotion(9) == -1);
    }
    assert(send_calls == 0);

    /* Boundaries in range, bit 6 clear: bytes +0x40/+0x41. */
    data_0031be9c = 0x020a; data_0031be98 = 0x0000;
    item.status.unknown_bit_5 = -1; item.status.unknown_bit_7 = -1;
    assert(SzEffect_GetPlayMotion(9) == 77 && send_calls == 1);
    assert(send_args[0] == 2 && send_args[1] == 10 && send_args[2] == 0 && send_args[3] == 0);
    assert(send_args[4] == 0x91 && send_args[5] == 0x92 && send_args[6] == 9);

    /* Bit 6 set: bytes +0x42/+0x43. */
    data_0031be9c = 0x0000; data_0031be98 = 0x020a;
    item.status.unknown_bit_5 = 0; item.status.unknown_bit_7 = 0;
    item.status.unknown_bit_6 = -1;
    assert(SzEffect_GetPlayMotion(4) == 77 && send_calls == 2);
    assert(send_args[0] == 0 && send_args[1] == 0 && send_args[2] == 2 && send_args[3] == 10);
    assert(send_args[4] == 0x93 && send_args[5] == 0x94 && send_args[6] == 4);
}

static void check_draw_forward(void)
{
    static const int bad[] = { -0x100, 0x0300, 0x020b };
    size_t i;

    index_result = 6;
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        assert(SzEffect_ModelShotTextureWithFunc2(bad[i], 1, 2, 3, 4, 5) == 0);
    }
    assert(index_calls == 0 && draw_calls == 0);

    index_result = -1;
    assert(SzEffect_ModelShotTextureWithFunc2(0x020a, 1, 2, 3, 4, 5) == 0);
    assert(index_calls == 1 && index_args[0] == 2 && index_args[1] == 10 && draw_calls == 0);

    index_result = 0;
    assert(SzEffect_ModelShotTextureWithFunc2(0x0000, 1, 2, 3, 4, 5) == 1 && draw_calls == 1);
    assert(index_args[0] == 0 && index_args[1] == 0);
    assert(draw_args[0] == 0 && draw_args[1] == 1 && draw_args[2] == 2);
    assert(draw_args[3] == 3 && draw_args[4] == 4 && draw_args[5] == 5);

    /* Forwarders: straight-line coverage. */
    index_result = 7;
    assert(SzEffect_ModelShotTexture2(0x0104, 11, 12, 13) == 1 && draw_calls == 2);
    assert(index_args[0] == 1 && index_args[1] == 4);
    assert(draw_args[0] == 7 && draw_args[1] == 11 && draw_args[2] == 12);
    assert(draw_args[3] == 13 && draw_args[4] == 0 && draw_args[5] == 0);

    data_0031be9c = 0x0209;
    assert(SzEffect_ModelShotTextureWithFunc(21, 22, 23, 24, 25) == 1 && draw_calls == 3);
    assert(index_args[0] == 2 && index_args[1] == 9);
    assert(draw_args[0] == 7 && draw_args[1] == 21 && draw_args[2] == 22);
    assert(draw_args[3] == 23 && draw_args[4] == 24 && draw_args[5] == 25);
}

static void check_object_forward(void)
{
    static const int bad[] = { -0x100, 0x0300, 0x020b };
    struct DotrObjectQuad *c = (struct DotrObjectQuad *)(size_t)0x30;
    struct DotrObjectQuad *d = (struct DotrObjectQuad *)(size_t)0x40;
    size_t i;

    index_calls = 0;
    index_result = 6;
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        assert(SzEffect_ReqMotionBlur2(bad[i], 1, 2, c, d, 5) == 0);
    }
    assert(index_calls == 0 && object_calls == 0);

    index_result = -1;
    assert(SzEffect_ReqMotionBlur2(0x020a, 1, 2, c, d, 5) == 0);
    assert(index_calls == 1 && index_args[0] == 2 && index_args[1] == 10 && object_calls == 0);

    index_result = 0;
    assert(SzEffect_ReqMotionBlur2(0x0000, 1, 2, c, d, 5) == 55 && object_calls == 1);
    assert(index_args[0] == 0 && index_args[1] == 0);
    assert(object_args[0] == 0 && object_args[1] == 1 && object_args[2] == 2);
    assert(object_args[3] == 0x30 && object_args[4] == 0x40 && object_args[5] == 5);

    /* Forwarder: straight-line coverage. */
    index_result = 8;
    data_0031be9c = 0x0105;
    assert(SzEffect_ReqMotionBlur(31, 32, d, c, 35) == 55 && object_calls == 2);
    assert(index_args[0] == 1 && index_args[1] == 5);
    assert(object_args[0] == 8 && object_args[1] == 31 && object_args[2] == 32);
    assert(object_args[3] == 0x40 && object_args[4] == 0x30 && object_args[5] == 35);
}

int main(void)
{
    check_item_send();
    check_draw_forward();
    check_object_forward();
    return 0;
}

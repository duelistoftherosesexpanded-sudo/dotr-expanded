/* Synthetic host tests for the camp_model_disp group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/camp_model_disp.h"

static char trace[64];
static int n;
static int push_ok;
static long alpha_value;
static struct DotrCampModelView seen_view, model_view;
static int modifies;
static float clip_a, clip_b;
static struct DotrCursorSlotRectPacket sorted;
static long tags[4];

static void step(char c)
{
    assert(n < 62);
    trace[n++] = c;
    trace[n] = 0;
}
int ExAssignAlpha(struct DotrCursorPacketHeader *packet, long value)
{
    assert(packet->unknown_00 == 5 && packet->unknown_02 == (38 | 0x800) && packet->unknown_04 == 0 && packet->unknown_08 == 0x42421);
    alpha_value = value;
    step('a');
    return 0;
}
int func_0020def0_campmodel(struct DotrCampModelView *out)
{
    int i;

    for (i = 0; i < 4; i++) {
        out->eye[i] = 9.0f;
        out->dir[i] = 9.0f;
    }
    out->roll = 9.0f;
    step('g');
    return 0;
}
int func_0020e460_campmodel(void)
{
    step('P');
    return push_ok;
}
int func_0020e4f0_campmodel(void)
{
    step('p');
    return 0;
}
int RhBase_AssignRenderFrame(int index, int mode, int kind)
{
    assert(index == 0 && mode == 10 && kind == 49);
    step('A');
    return 1;
}
DotrDrawWord RhBase_EnableRenderFrame(DotrDrawWord p)
{
    step('E');
    return p + 1;
}
DotrDrawWord RhBase_ClearRenderFrame(DotrDrawWord p, void *color)
{
    assert(color == 0);
    step('C');
    return p + 1;
}
DotrDrawWord RhBase_DisableRenderFrame(DotrDrawWord p)
{
    step('D');
    return p + 1;
}
int func_001f2050_campmodel(int index, struct DotrCampModelVector *out)
{
    assert(index == 6);
    out->x = 1.0f; out->y = 2.5f; out->z = 3.0f; out->w = 4.0f;
    step('c');
    return 0;
}
int func_0020dfd0_campmodel(float a, float b)
{
    clip_a = a; clip_b = b;
    step('z');
    return 0;
}
int func_0020e360_campmodel(struct DotrCampModelView *view)
{
    seen_view = *view;
    if (modifies++ == 0) {
        model_view = *view;
    }
    step('m');
    return 0;
}
struct DotrDrawTag *RhView_SendViewParameters(struct DotrDrawTag *tag)
{
    step('s');
    return (struct DotrDrawTag *)((char *)tag + 1);
}
int func_001f30b0_campmodel(int index, int value)
{
    assert(index == 6);
    step(value ? '1' : '0');
    return 0;
}
DotrDrawWord func_001ee4b0_campmodel(DotrDrawWord cursor, int index, int c, int d, int e, int f, int g)
{
    assert(index == 6 && c == 0 && d == 0 && e == 0 && f == 0 && g == 0);
    step('d');
    return cursor + 1;
}
int func_001ed4a0_campmodel(int index)
{
    assert(index == 6);
    step('M');
    return 0;
}
int RhBase_GetTextureInfoForRender(struct DotrCursorTex *a, struct DotrCursorTex *b)
{
    a->word = 0x1111;
    b->word = 0x2222;
    step('t');
    return 0;
}
DotrDrawWord RhText_SetTextureFlush(DotrDrawWord cursor)
{
    step('f');
    return cursor + 1;
}
DotrDrawWord ExSortPrim_Pipe(DotrDrawWord cursor, void *packet, int a1, int a2)
{
    assert(a1 == 0 && a2 == 0);
    memcpy(&sorted.header, packet, sizeof sorted - sizeof sorted.unknown_m8);
    step('S');
    return cursor + 1;
}
void RhBase_FreeRenderFrame(void)
{
    step('F');
}

static void check(int w, int h, int half)
{
    DotrDrawWord start = (DotrDrawWord)tags;
    struct DotrCursorSlotUv *uv0 = (struct DotrCursorSlotUv *)&sorted.vertex[0].uv;
    struct DotrCursorSlotUv *uv1 = (struct DotrCursorSlotUv *)&sorted.vertex[1].uv;

    n = 0;
    push_ok = 1;
    memset(&sorted, 0, sizeof sorted);
    modifies = 0;
    /* eight callees move the cursor by one each */
    assert(Taku_Camp_Teki_Model_Disp(start, 150.0f, 6, 400, 300, w, h, 77) == start + 8);
    assert(strcmp(trace, "agPAECczms1dM0tDfpgmsSF") == 0);
    assert(alpha_value == 0x8000000044L && clip_a == 1.0f && clip_b == 512.0f);
    /* the model is drawn through: eye at the centre's height, 150 back, looking along +z, no roll */
    assert(model_view.eye[0] == 0.0f && model_view.eye[1] == 2.5f && model_view.eye[2] == -150.0f);
    assert(model_view.dir[0] == 0.0f && model_view.dir[1] == 0.0f && model_view.dir[2] == 1.0f && model_view.roll == 0.0f);
    /* and the view handed over afterwards is the fetched one again */
    assert(seen_view.eye[0] == 9.0f && seen_view.roll == 9.0f);
    assert(sorted.colour[0] == 128 && sorted.colour[1] == 128 && sorted.colour[2] == 128 && sorted.colour[3] == 77);
    assert(sorted.unknown_14 == 1.0f);
    assert(sorted.vertex[0].xyz.z == 0xfffffa && sorted.vertex[0].xyz.f == 0);
    assert(sorted.vertex[1].xyz.z == 0xfffffa && sorted.vertex[1].xyz.f == 0);
    assert(sorted.vertex[0].xyz.x == (unsigned)((400 - 320 + 2048) << 4) && sorted.vertex[0].xyz.y == (unsigned)((300 - 224 + 2048) << 4));
    assert(sorted.vertex[1].xyz.x == (unsigned)((400 - 320 + 2048 + w) << 4));
    assert(sorted.vertex[1].xyz.y == (unsigned)((300 - 224 + 2048 + h) << 4));
    assert(uv0->u == (unsigned)((320 - half) << 4) && uv0->v == 0);
    assert(uv1->u == (unsigned)((320 + half) << 4) && uv1->v == 7168);
    assert(sorted.tex0.word == 0x1111 && sorted.tex1.word == 0x2222);
}

int main(void)
{
    DotrDrawWord start = (DotrDrawWord)tags;

    check(100, 448, 50);
    check(64, 128, 112);
    check(448, 448, 224);

    /* The view stack refuses: only the packet is prepared and the view fetched. */
    n = 0;
    push_ok = 0;
    assert(Taku_Camp_Teki_Model_Disp(start, 150.0f, 6, 400, 300, 100, 448, 77) == start);
    assert(strcmp(trace, "agP") == 0);
    puts("camp_model_disp: ok");
    return 0;
}

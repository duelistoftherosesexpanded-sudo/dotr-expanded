/* Synthetic host tests for the card_scene group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/card_scene.h"

struct DotrCardSceneControl data_00317490;
unsigned char turnOwner[4];

/* Every stub appends an id to the trace and returns cursor + 1, so the chain is checked. */
static char trace[128];
static int mode_result, flag_result, request_result, node_result, ot[1];
static struct DotrCardSceneVector camera, leader_pos[3], calc_pos, setup_pos, draw_pos;
static int leader_side[3], leader_alpha[3], leader_flag[3], leaders, calc_args[3], zmodes[8], zcount;
static struct DotrCardSceneItem *setup_item;
static unsigned char setup_alpha;
static DotrCardSceneWord sort_args[3];

static DotrCardSceneWord mark(DotrCardSceneWord cursor, char id)
{
    size_t n = strlen(trace);
    trace[n] = id;
    trace[n + 1] = 0;
    return cursor + 1;
}
int SzGame_GetMode(int a) { assert(a == 1); return mode_result; }
int SzModel_GetDrawFlag(int index) { assert(index == 0); return flag_result; }
DotrCardSceneWord ExSetAnchor_Pipe(DotrCardSceneWord cursor) { return mark(cursor, 'A'); }
DotrCardSceneWord SzDuel_StartCardSession_Pipe(DotrCardSceneWord cursor) { return mark(cursor, 'S'); }
DotrCardSceneWord SzDuel_DrawCardRequest_Pipe(DotrCardSceneWord cursor) { return mark(cursor, 'R'); }
DotrCardSceneWord RhBase_SetZBufferMode(DotrCardSceneWord cursor, int a)
{
    zmodes[zcount++] = a;
    return mark(cursor, 'Z');
}
int SzDuel_GetUnitPos(int a, int b, struct DotrCardSceneVector *out)
{
    assert(b == 0);
    out->x = 10.0f + (float)a;
    out->y = 20.0f;
    out->z = 30.0f;
    out->w = 40.0f;
    mark(0, 'U');
    return 0;
}
DotrCardSceneWord SzDuel_DrawLeaderBase_Pipe(DotrCardSceneWord cursor, int side, struct DotrCardSceneVector *pos, int alpha,
                                             int flag)
{
    leader_side[leaders] = side;
    leader_pos[leaders] = *pos;
    leader_alpha[leaders] = alpha;
    leader_flag[leaders] = flag;
    leaders++;
    return mark(cursor, 'L');
}
void *SzView_InqRequest(int a, int b)
{
    assert(a == -1 && b == -1);
    mark(0, 'Q');
    return request_result ? &camera : 0;
}
struct DotrCardSceneVector *SzView_GetCP(int index)
{
    assert(index == 0);
    mark(0, 'P');
    return &camera;
}
void SzDuel_CalcUnitPos(int x, int y, int z, struct DotrCardSceneVector *v)
{
    calc_args[0] = x;
    calc_args[1] = y;
    calc_args[2] = z;
    *v = calc_pos;
    mark(0, 'C');
}
int SzDuel_GetCardNode(struct DotrCardSceneBlock *block, int a)
{
    assert(a == 1 && block != 0);
    mark(0, 'N');
    return node_result;
}
DotrCardSceneWord SzDuel_SetupCard2_Pipe(DotrCardSceneWord cursor, struct DotrCardSceneItem *item,
                                         struct DotrCardSceneBlock *block)
{
    setup_item = item;
    setup_pos = block->pos;
    setup_alpha = block->color[3];
    return mark(cursor, 'T');
}
DotrCardSceneWord SzDuel_DrawCard_Pipe(DotrCardSceneWord cursor, struct DotrCardSceneBlock *block)
{
    draw_pos = block->pos;
    return mark(cursor, 'D');
}
DotrCardSceneWord SzDuel_DrawHandCard_Pipe(DotrCardSceneWord cursor) { return mark(cursor, 'H'); }
DotrCardSceneWord SzDuel_FinishCardSession_Pipe(DotrCardSceneWord cursor) { return mark(cursor, 'F'); }
int *ExGetMyOT(void) { mark(0, 'O'); return ot; }
DotrCardSceneWord ExSortAnchor_Pipe(DotrCardSceneWord cursor, DotrCardSceneWord anchor, int *table, unsigned int index)
{
    assert(table == ot && index == 0xffffff);
    sort_args[0] = cursor;
    sort_args[1] = anchor;
    mark(0, 'X');
    return 0x7000;
}

static struct DotrCardSceneItem item;
static struct DotrCardSceneBlock source;

static void reset(void)
{
    memset(&data_00317490, 0, sizeof(data_00317490));
    memset(&item, 0, sizeof(item));
    memset(&source, 0, sizeof(source));
    trace[0] = 0;
    leaders = zcount = 0;
    mode_result = 0;
    flag_result = 1;
    request_result = 0;
    node_result = 1;
    turnOwner[0] = 1;
    data_00317490.player[1].x = 3;
    data_00317490.player[1].y = 4;
    calc_pos.x = 1.0f;
    calc_pos.y = 2.0f;
    calc_pos.z = 3.0f;
    calc_pos.w = 4.0f;
    camera.x = 5.0f;
    camera.y = 6.0f;
    camera.z = 7.0f;
    camera.w = 8.0f;
    setup_item = 0;
}

/* Branch coverage: one case per outcome of each test in the function. */
static void check_all_card(void)
{
    /* Mode other than 0 or 1: unchanged, nothing called. */
    reset();
    mode_result = 2;
    assert(SzDuel_DrawAllCard_Pipe(0x1000) == 0x1000 && trace[0] == 0);
    /* Draw flag clear: unchanged. */
    reset();
    mode_result = 1;
    flag_result = 0;
    assert(SzDuel_DrawAllCard_Pipe(0x1000) == 0x1000 && trace[0] == 0);

    /* Mode 0, no moving card: both leader bases, then the tail. */
    reset();
    assert(SzDuel_DrawAllCard_Pipe(0x1000) == 0x7000);
    assert(strcmp(trace, "ASRZULULZHFOX") == 0);
    assert(zcount == 2 && zmodes[0] == 1 && zmodes[1] == 0);
    assert(leader_side[0] == 0 && leader_side[1] == 1 && leader_alpha[0] == 128 && leader_alpha[1] == 128);
    assert(leader_flag[0] == 1 && leader_flag[1] == 1);
    assert(leader_pos[0].x == 10.0f && leader_pos[0].y == 0.0f && leader_pos[0].z == 30.0f && leader_pos[0].w == 40.0f);
    assert(leader_pos[1].x == 11.0f && leader_pos[1].y == 0.0f);
    assert(sort_args[0] == 0x1000 + 9 && sort_args[1] == 0x1000);

    /* Mode 1, moving card present but inactive: same as none. */
    reset();
    mode_result = 1;
    data_00317490.item = &item;
    assert(SzDuel_DrawAllCard_Pipe(0x1000) == 0x7000 && strcmp(trace, "ASRZULULZHFOX") == 0);

    /* Active, kind 0, state not 29: player cell position, leader base with alpha 64. */
    reset();
    data_00317490.item = &item;
    item.active = -1;
    item.leader = 9;
    assert(SzDuel_DrawAllCard_Pipe(0x1000) == 0x7000);
    assert(strcmp(trace, "ASRZULULZCZLZHFOX") == 0);
    assert(calc_args[0] == 3 && calc_args[1] == 4 && calc_args[2] == 0);
    assert(leaders == 3 && leader_side[2] == 9 && leader_alpha[2] == 64 && leader_flag[2] == 0);
    assert(leader_pos[2].x == 1.0f && leader_pos[2].y == 1.0f && leader_pos[2].z == 3.0f && leader_pos[2].w == 4.0f);
    assert(zcount == 4 && zmodes[2] == 1 && zmodes[3] == 0 && sort_args[0] == 0x1000 + 12);

    /* State 29 without a request: still the player cell. */
    reset();
    data_00317490.item = &item;
    data_00317490.state = 29;
    item.active = -1;
    assert(SzDuel_DrawAllCard_Pipe(0x1000) == 0x7000 && strcmp(trace, "ASRZULULZQCZLZHFOX") == 0);

    /* State 29 with a request: the camera point; kind set but no block: nothing drawn. */
    reset();
    data_00317490.item = &item;
    data_00317490.state = 29;
    request_result = 1;
    item.active = -1;
    item.kind = 1;
    assert(SzDuel_DrawAllCard_Pipe(0x1000) == 0x7000 && strcmp(trace, "ASRZULULZQPHFOX") == 0);

    /* Kind set with a block: copy with scaled alpha and the position, card passed on. */
    reset();
    data_00317490.item = &item;
    data_00317490.state = 29;
    request_result = 1;
    item.active = -1;
    item.kind = 1;
    item.block = &source;
    source.color[3] = 200;
    source.pos.x = 99.0f;
    assert(SzDuel_DrawAllCard_Pipe(0x1000) == 0x7000 && strcmp(trace, "ASRZULULZQPNTZDZHFOX") == 0);
    assert(setup_item == &item && setup_alpha == 100 && source.color[3] == 200 && source.pos.x == 99.0f);
    assert(setup_pos.x == 5.0f && setup_pos.y == 1.0f && setup_pos.z == 7.0f && setup_pos.w == 8.0f);
    assert(draw_pos.x == 5.0f && draw_pos.y == 1.0f && zcount == 4 && zmodes[2] == 1 && zmodes[3] == 0);

    /* SzDuel_GetCardNode gives 0: the card argument is 0. */
    reset();
    data_00317490.item = &item;
    item.active = -1;
    item.kind = 1;
    item.block = &source;
    node_result = 0;
    assert(SzDuel_DrawAllCard_Pipe(0x1000) == 0x7000 && strcmp(trace, "ASRZULULZCNTZDZHFOX") == 0);
    assert(setup_item == 0 && setup_pos.x == 1.0f && setup_pos.y == 1.0f);
}

int main(void)
{
    check_all_card();
    puts("card_scene: ok");
    return 0;
}

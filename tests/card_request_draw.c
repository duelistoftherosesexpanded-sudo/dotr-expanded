/* Synthetic host tests for the card_request_draw group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/card_request_draw.h"

struct DotrCardRequest gSzDuel_aCardRequest[32];
struct DotrCardRequestColor data_002914d8 = {{1, 2, 3, 4}};

static struct DotrCardRequestCell cell, *cell_result;
static int map_calls, map_x, map_y, node_result, setups, zmodes[8], zcount;
static DotrCardRequestWord setup_item;
static struct DotrCardRequestBlock setup_block, draw_block;

struct DotrCardRequestCell *SzDuel_GetMapInfo(int x, int y)
{
    map_calls++;
    map_x = x;
    map_y = y;
    return cell_result;
}
int SzDuel_GetCardNode(struct DotrCardRequestBlock *block, int a)
{
    assert(block != 0 && a == 1);
    return node_result;
}
DotrCardRequestWord SzDuel_SetupCard2_Pipe(DotrCardRequestWord cursor, DotrCardRequestWord item,
                                           struct DotrCardRequestBlock *block)
{
    setups++;
    setup_item = item;
    setup_block = *block;
    return cursor + 1;
}
DotrCardRequestWord SzDuel_DrawCard_Pipe(DotrCardRequestWord cursor, struct DotrCardRequestBlock *block)
{
    draw_block = *block;
    return cursor + 0x10;
}
DotrCardRequestWord RhBase_SetZBufferMode(DotrCardRequestWord cursor, int a)
{
    zmodes[zcount++] = a;
    return cursor + 0x100;
}

static struct DotrCardRequestItem item;
static struct DotrCardRequest *request = &gSzDuel_aCardRequest[5];

static void reset(void)
{
    memset(gSzDuel_aCardRequest, 0, sizeof(gSzDuel_aCardRequest));
    memset(&item, 0, sizeof(item));
    memset(&setup_block, 0, sizeof(setup_block));
    map_calls = setups = zcount = 0;
    setup_item = -1;
    node_result = 1;
    cell.level = 128;
    cell_result = &cell;
    item.x = 6;
    item.y = 7;
    item.offset_x = 10;
    item.offset_z = -20;
    request->item = &item;
    request->block.position.value[0] = 1.0f;
    request->block.position.value[1] = 2.0f;
    request->block.position.value[2] = 3.0f;
    request->block.color[3] = 9;
}

/* Branch coverage: one case per outcome of each test in the function. */
static void check_draw_card_request(void)
{
    /* No request has a card: nothing called. */
    reset();
    request->item = 0;
    assert(SzDuel_DrawCardRequest_Pipe(0x1000) == 0x1000 && map_calls == 0 && setups == 0);

    /* One card on a cell of level 128: y = 35, offsets added, drawn between modes 1 and 0. */
    reset();
    assert(SzDuel_DrawCardRequest_Pipe(0x1000) == 0x1000 + 0x211);
    assert(map_calls == 1 && map_x == 6 && map_y == 7 && setups == 1);
    assert(setup_item == (DotrCardRequestWord)&item);
    assert(setup_block.position.value[0] == 11.0f && setup_block.position.value[1] == 35.0f);
    assert(setup_block.position.value[2] == -17.0f && draw_block.position.value[1] == 35.0f);
    assert(setup_block.unknown_82_bits_12_13 == 0 && setup_block.tint.value[0] == 0);
    assert(zcount == 2 && zmodes[0] == 1 && zmodes[1] == 0);
    /* The table entry itself is not written. */
    assert(request->block.position.value[0] == 1.0f && request->block.position.value[1] == 2.0f);

    /* Level 0 or below counts as 0. */
    reset();
    cell.level = -5;
    SzDuel_DrawCardRequest_Pipe(0);
    assert(setup_block.position.value[1] == 0.0f);

    /* Either selector byte: the two bits and the constant tint. */
    reset();
    item.unknown_49 = 1;
    SzDuel_DrawCardRequest_Pipe(0);
    assert(setup_block.unknown_82_bits_12_13 == 3 && memcmp(&setup_block.tint, &data_002914d8, 4) == 0);
    reset();
    item.unknown_4a = 1;
    SzDuel_DrawCardRequest_Pipe(0);
    assert(setup_block.unknown_82_bits_12_13 == 3 && setup_block.tint.value[3] == 4);

    /* No cell: y and tint kept, offsets still added. */
    reset();
    cell_result = 0;
    item.unknown_49 = 1;
    SzDuel_DrawCardRequest_Pipe(0);
    assert(setup_block.position.value[1] == 2.0f && setup_block.unknown_82_bits_12_13 == 0);
    assert(setup_block.position.value[0] == 11.0f && setup_block.position.value[2] == -17.0f);

    /* Hidden card: the copy is passed on unchanged. */
    reset();
    item.hidden = -1;
    SzDuel_DrawCardRequest_Pipe(0);
    assert(setups == 1 && setup_block.position.value[0] == 1.0f && setup_block.position.value[1] == 2.0f);
    assert(setup_block.position.value[2] == 3.0f);

    /* Bit 31 set: looked up but not drawn. */
    reset();
    request->block.flag_31 = -1;
    assert(SzDuel_DrawCardRequest_Pipe(0x1000) == 0x1000 && map_calls == 1 && setups == 0 && zcount == 0);

    /* Bit 15 set: drawn only with a non-zero alpha. */
    reset();
    request->block.flag_15 = -1;
    assert(SzDuel_DrawCardRequest_Pipe(0x1000) == 0x1000 + 0x211 && setups == 1);
    reset();
    request->block.flag_15 = -1;
    request->block.color[3] = 0;
    assert(SzDuel_DrawCardRequest_Pipe(0x1000) == 0x1000 && setups == 0);

    /* SzDuel_GetCardNode gives 0: the card argument is 0. */
    reset();
    node_result = 0;
    assert(SzDuel_DrawCardRequest_Pipe(0x1000) == 0x1000 + 0x211 && setup_item == 0);

    /* Bounds: entries 0 and 31 are both visited. */
    reset();
    gSzDuel_aCardRequest[0] = *request;
    gSzDuel_aCardRequest[31] = *request;
    assert(SzDuel_DrawCardRequest_Pipe(0) == 3 * 0x211 && map_calls == 3);
}

int main(void)
{
    check_draw_card_request();
    puts("card_request_draw: ok");
    return 0;
}

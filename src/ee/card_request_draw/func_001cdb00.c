#include "dotr/ee/card_request_draw.h"

/* Draws the 32 card requests. For each entry with a card: looks up the card's cell, copies
 * the 144-byte block and skips it when bit 31 of the word at +0x80 is set, or bit 15 is set
 * with alpha 0. Unless the card's top bit at +0x57 is set, a present cell sets y to
 * max(level, 0) * 35 / 128 and, when either byte at +0x49/+0x4a is non-zero, sets the two
 * bits at +0x82 bits 12-13 and copies the constant tint; then the card's x/z offsets are
 * added. The copy is set up (with the card, or 0 when SzDuel_GetCardNode gives 0) and drawn
 * between Z buffer modes 1 and 0. */
DotrCardRequestWord SzDuel_DrawCardRequest_Pipe(DotrCardRequestWord cursor)
{
    struct DotrCardRequestColor color;
    struct DotrCardRequestBlock block;
    struct DotrCardRequest *request;
    int i;
    struct DotrCardRequestCell *cell;
    int level;
    struct DotrCardRequestItem *item;

    request = gSzDuel_aCardRequest;
    color = data_002914d8;
    for (i = 0; i < 32; i++, request++) {
        if (request->item != 0) {
            item = request->item;
            cell = SzDuel_GetMapInfo(item->x, item->y);
            block = request->block;
            if (!block.flag_31 && (!block.flag_15 || block.color[3] != 0)) {
                if (!item->hidden) {
                    if (cell != 0) {
                        level = cell->level > 0 ? cell->level : 0;
                        block.position.value[1] = (float)(level * 35) / 128.0f;
                        if (item->unknown_49 != 0 || item->unknown_4a != 0) {
                            block.unknown_82_bits_12_13 = 3;
                            block.tint = color;
                        }
                    }
                    block.position.value[0] += (float)item->offset_x;
                    block.position.value[2] += (float)item->offset_z;
                }
                item = SzDuel_GetCardNode(&block, 1) ? item : 0;
                cursor = SzDuel_SetupCard2_Pipe(cursor, (DotrCardRequestWord)item, &block);
                cursor = RhBase_SetZBufferMode(cursor, 1);
                cursor = RhBase_SetZBufferMode(SzDuel_DrawCard_Pipe(cursor, &block), 0);
            }
        }
    }
    return cursor;
}

#include "dotr/ee/item_fills.h"

void SzDuel_InitCardInfo(struct DotrFillItem *unit, int cardId)
{
    int i;
    int n;

    unit->source = 0;
    unit->cardModel = 0;
    for (i = 0; i < 5; i++) {
        unit->pairs[i].unknown_words[0] = -1;
        unit->pairs[i].unknown_words[1] = -1;
    }
    unit->unknown_30 = 0;
    unit->xOffsetTerrain = 0;
    unit->yOffsetTerrain = 0;
    unit->apBoost = 0;
    unit->dpBoost = 0;
    unit->id = 999;
    unit->kind = 255;
    unit->attr = 255;
    unit->unknown_40[0] = 7;
    unit->unknown_40[1] = 7;
    unit->unknown_40[2] = 7;
    unit->unknown_40[3] = 7;
    unit->team = 2;
    unit->slot = 11;
    unit->modelId = 84;
    unit->unknown_47[0] = 0;
    unit->unknown_47[1] = 0;
    unit->unknown_47[2] = 0;
    unit->unknown_47[3] = 0;
    unit->unknown_47[4] = 0;
    unit->unknown_47[5] = 0;
    unit->cardHighlight = 128;
    unit->ctlModeC = 1;
    unit->unknown_4f = 7;
    unit->unknown_50[0] = 128;
    unit->unknown_50[1] = 128;
    unit->unknown_50[2] = 128;
    unit->unknown_53[0] = 0;
    unit->unknown_53[1] = 0;
    unit->unknown_53[2] = 0;
    unit->unknown_56.unknown_bits_0_1 = 0;
    unit->unknown_56.unknown_bits_2_3 = 0;
    unit->unknown_56.unknown_bits_4_5 = 0;
    unit->unknown_56.unknown_bits_6_7 = 0;
    unit->status.unknown_bit_0 = 0;
    unit->status.unknown_bit_1 = 0;
    unit->status.unknown_bit_2 = 0;
    unit->status.unknown_bit_3 = 0;
    unit->status.unknown_bit_4 = 0;
    unit->status.unknown_bit_5 = 0;
    unit->status.unknown_bit_6 = 0;
    unit->status.unknown_bit_7 = 0;
    unit->modelStatusFlags.flags.unknown_bit_0 = 0;
    unit->modelStatusFlags.flags.unknown_bit_1 = 0;
    unit->modelStatusFlags.flags.unknown_bit_2 = 0;
    unit->modelStatusFlags.flags.unknown_bit_3 = 0;
    unit->modelStatusFlags.wide.unknown_bits_4_9 = -1;
    if (cardId == 671) {
        cardId = 999;
    }
    if (cardId >= 0 && cardId < 854) {
        unit->id = cardId;
        unit->source = func_001dd0e0_fill(cardId);
        unit->kind = unit->source->kind;
        unit->attr = unit->source->unknown_02_low;
        n = unit->source->effectId;
        if (n != 0xffff) {
            if (cardId >= 683) {
                unit->pairs[0] = EffIdx_S_fill[n];
            } else {
                for (i = 0; i < 5; i++) {
                    unit->pairs[i] = EffIdx_M_fill[n][i];
                }
            }
        }
    }
}

/* Card effects and leader abilities: the game's tables, or a card's own (effects.json, written into
 * effects_gen.c by tools/dlc_effects.py). docs/topics/card-effects.md describes the tables.
 *
 *   EffIdx_M  0x2c9df0  monsters: rows of five slots (ATTACK, MOVEMENT, NATURE, FLIP, DESTRUCTION),
 *                       each two words, chosen by the card record's effect row (bytes 4-5)
 *   EffIdx_S  0x2cc5f0  other cards: rows of one slot
 *   EffIdx_L  0x2c3338  leader abilities: a row of five slots (nine words) per monster card 0-682
 *
 * The four readers of the tables are ported (mod.json) to ask here: SzDuel_InitCardInfo (a card's
 * effects when it is put on the field), SzDuel_GetUnitConstEffect2, SzDuel_AddUnit2 (the leader's
 * abilities) and the deck builder's ability page. A card with effects of its own gets them; a new
 * card otherwise keeps its record's row (its donor's, or effect_from's), and its donor's leader
 * abilities. */

#include "dlc_cards.h"
#include "dlc_effects.h"

#define EFF_L ((unsigned char *)0x2c3338)
#define EFF_M ((unsigned char *)0x2c9df0)
#define EFF_S ((unsigned char *)0x2cc5f0)
#define OWN_ROW 0x7ffe   /* any row but 0xffff: the card has effects of its own */

static const unsigned int no_abilities[10] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
};

const struct DlcEffectCard *DlcEffect_Find(int card);

static const struct DlcEffectCard *find(int card)
{
    return DlcEffect_Find(card);
}

const struct DlcEffectCard *DlcEffect_Find(int card)
{
    int k;

    for (k = 0; k < dlc_effect_count; k++) {
        if (dlc_effect_cards[k].card == card) {
            return &dlc_effect_cards[k];
        }
    }
    return 0;
}

int DlcEffect_Row(int card, int row)
{
    const struct DlcEffectCard *e = find(card);

    return e != 0 && e->has_effect ? OWN_ROW : row;
}

void *DlcEffect_Monster(int card, int row)
{
    const struct DlcEffectCard *e = find(card);

    if (e != 0 && e->has_effect) {
        return (void *)e->effect;
    }
    return EFF_M + 40 * row;
}

void *DlcEffect_Spell(int card, int row)
{
    const struct DlcEffectCard *e = find(card);

    if (e != 0 && e->has_effect) {
        return (void *)e->effect;
    }
    return EFF_S + 8 * row;
}

void *DlcEffect_Leader(int card)
{
    const struct DlcEffectCard *e = find(card);

    if (e != 0 && e->has_leader) {
        return (void *)e->leader;
    }
    if (DlcCard_IsNew(card)) {
        card = DlcCard_Donor(card);
    }
    if (card < 0 || card >= 683 || card == 671) {
        return (void *)no_abilities;
    }
    return EFF_L + 40 * card;
}

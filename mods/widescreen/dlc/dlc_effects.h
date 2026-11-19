/* Cards with effects or leader abilities of their own (effects.json -> effects_gen.c, written by
 * tools/dlc_effects.py; used by effects.c). */

#ifndef DLC_EFFECTS_H
#define DLC_EFFECTS_H

/* Effects stacked on a slot, after the slot's own (stack.c): at most this many more per slot. */
#define DLC_STACK_MAX 4

struct DlcEffectCard {
    short card;
    char has_effect;            /* effect: five slots for a monster, the first slot otherwise */
    char has_leader;            /* leader: the nine ability words (and a tenth, kept) */
    unsigned int effect[10];
    unsigned int leader[10];
    unsigned char stacked[5];   /* per slot: how many more effects */
    unsigned int extra[5][DLC_STACK_MAX][2];
};

extern const struct DlcEffectCard dlc_effect_cards[];
extern const int dlc_effect_count;

#endif

/* Card eras: the newest era of cards to play with, chosen in the title's MOD OPTIONS
 * (modoptions.c) and kept with the save (card_save.c, the mods record).
 *
 * A new card newer than the era chosen counts as disabled (DlcCard_Status, cards.c): it cannot be
 * obtained, it is left out of the chest (card_bag.c), and a deck that holds it plays its donor in
 * duels (DlcCard_DuelCard). Choosing a later era brings them all back. The game's own cards are
 * all OG, so always there. */

#include "dlc_cards.h"

static int chosen = DLC_ERA_COUNT - 1;   /* everything, until a choice or a save says otherwise */
static int picked;                      /* chosen on the title screen since the game started */

static const char *names[DLC_ERA_COUNT] = {"OG", "GX", "5DS", "ZEXAL", "ARC-V", "VRAINS", "MODERN"};

int DlcEra_Chosen(void)
{
    return chosen;
}

const char *DlcEra_Name(int era)
{
    return era >= 0 && era < DLC_ERA_COUNT ? names[era] : "";
}

void DlcEra_Set(int era)
{
    if (era >= 0 && era < DLC_ERA_COUNT) {
        chosen = era;
        picked = 1;
    }
}

void DlcEra_Load(int era)
{
    if (!picked && era >= 0 && era < DLC_ERA_COUNT) {
        chosen = era;
    }
}

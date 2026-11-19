#include "dotr/ee/side_table.h"

int lSzDuel_IsDeckReady(int side, int deckNo)
{
    int count = SzData_GetDeckCount(side, deckNo);
    int value = SzData_GetLeader(side, deckNo);

    if (count >= 40) {
        if (value == 999) {
            /* An empty three-term test is needed for the original's dead
               branch here; which terms it had cannot be told from the code. */
            if (!side && !deckNo && !count) {
            }
        } else {
            return 1;
        }
    }
    return 0;
}

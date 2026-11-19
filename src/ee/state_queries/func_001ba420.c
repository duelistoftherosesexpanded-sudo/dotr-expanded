#include "dotr/ee/state_queries.h"

int lSzDuel_HitPassword(unsigned char *key)
{
    signed char *shifts = passwordKeyHannibal;
    unsigned char code[8];
    struct DotrCodeEntry *entry;
    int id;
    int i;
    int shift;

    for (id = 0; id < 854; id++) {
        entry = (struct DotrCodeEntry *)SzDuel_GetCardConstInfo(id);
        if (entry != 0 && entry->coded) {
            for (i = 0; i < 8; i++) {
                shift = shifts[(id + i) % 8];
                code[i] = (entry->code[i] + shift + 256) % 256;
                if (code[i] != key[i]) {
                    break;
                }
            }
            if (i >= 8) {
                return id;
            }
        }
    }
    return -1;
}

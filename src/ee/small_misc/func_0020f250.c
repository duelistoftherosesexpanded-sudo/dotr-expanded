#include "dotr/ee/small_misc.h"

/* Stores the header's own address at +0x0c and adds it to the word at +0x04
 * of each of the header's counted entries. */
void RhText_BindAddress(struct DotrMiscPackedHeader *header)
{
    struct DotrMiscPackedEntry48 *pTexturePartEntry;
    int i;
    int count;
    header->unknown_0c = (DotrMiscAddress)header;
    count = header->unknown_58;
    pTexturePartEntry = (struct DotrMiscPackedEntry48 *)((char *)header + header->unknown_10);
    for (i = 0; i < count; i++) {
        pTexturePartEntry->unknown_04 = (DotrMiscAddress)header + pTexturePartEntry->unknown_04;
        pTexturePartEntry++;
    }
}
